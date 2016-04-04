/*
 * SARG Squid Analysis Report Generator      http://sarg.sourceforge.net
 *                                                            1998, 2010
 *
 * SARG donations:
 *      please look at http://sarg.sourceforge.net/donations.php
 * Support:
 *     http://sourceforge.net/projects/sarg/forums/forum/363374
 * ---------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#include "include/conf.h"
#include "include/defs.h"

static char **files_done = NULL;
static int nfiles_done = 0;


static void read_log(const char *wentp, FILE *fp_ou,int dfrom,int duntil)
{
   FILE *fp_in = NULL;
   char *buf;
   char leks[5], sep[2], res[MAXLEN];
   char hour[15];
   char source[128], list[128];
   char url[MAX_URL_LEN];
   char user[MAX_USER_LEN];
   char ip[25];
   long long int lmon, lday, lyear;
   int mon, day, year;
   int  idata=0;
   int  i;
   char *str;
   bool id_is_ip;
   struct getwordstruct gwarea;
   struct getwordstruct gwarea1;
   struct userinfostruct *uinfo;
   longline line;

   if(debug) {
      debuga(_("Reading redirector log file %s\n"),wentp);
   }

   /* With squidGuard, you can log groups in only one log file.
      We must parse each log files only one time.  Example :
      dest porn {
          domainlist porn/domains
          urllist    porn/urls
          log file1.log
      }
      dest aggressive {
          domainlist aggressive/domains
          urllist    aggressive/urls
          log file2.log
      }
      dest audio-video {
          domainlist audio-video/domains
          urllist    audio-video/urls
          log file1.log
      }
   */
   for (i=0; i<nfiles_done; i++)
      if (!strcmp(wentp, files_done[i])) return;

   nfiles_done++;
   files_done = realloc(files_done, nfiles_done*sizeof(char *));
   if (!files_done) {
      debuga(_("Not enough memory to store the name of the new redirector log to be read - %s\n"),strerror(errno));
      exit(EXIT_FAILURE);
   }
   files_done[nfiles_done-1] = strdup(wentp);
   if (!files_done[nfiles_done-1]) {
      debuga(_("Not enough memory to store the name of the new redirector log to be read - %s\n"),strerror(errno));
      exit(EXIT_FAILURE);
   }

   if ((fp_in=fopen(wentp,"r"))==NULL) {
      debuga(_("(squidguard) Cannot open log file %s\n"),wentp);
      exit(EXIT_FAILURE);
   }

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the redirector log\n"));
      exit(EXIT_FAILURE);
   }

   while ((buf=longline_read(fp_in,line)) != NULL) {
      getword_start(&gwarea,buf);
      if(RedirectorLogFormat[0] != '\0') {
         getword_start(&gwarea1,RedirectorLogFormat);
         leks[0]='\0';
         if (getword(leks,sizeof(leks),&gwarea1,'#')<0) {
            debuga(_("There is a broken record or garbage in your %s file\n"),wentp);
            exit(EXIT_FAILURE);
         }
         year=0;
         mon=0;
         day=0;
         hour[0]='\0';
         source[0]='\0';
         list[0]='\0';
         ip[0]='\0';
         user[0]='\0';
         url[0]='\0';
         while(strcmp(leks,"end") != 0) {
            if (getword(leks,sizeof(leks),&gwarea1,'#')<0 || getword(sep,sizeof(sep),&gwarea1,'#')<0) {
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),wentp);
               exit(EXIT_FAILURE);
            }
            if(strcmp(leks,"end") != 0) {
               if (getword_limit(res,sizeof(res),&gwarea,sep[0])<0) {
                  debuga(_("Maybe you have a broken record or garbage in your %s file\n"),wentp);
                  exit(EXIT_FAILURE);
               }
               if(strcmp(leks,"year") == 0) {
                  year=atoi(res);
               } else if(strcmp(leks,"mon") == 0) {
                  mon=atoi(res);
               } else if(strcmp(leks,"day") == 0) {
                  day=atoi(res);
               } else if(strcmp(leks,"hour") == 0) {
                  if (strlen(res)>=sizeof(hour)) {
                     debuga(_("Hour string too long in redirector log file %s\n"),wentp);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(hour,res);
               } else if(strcmp(leks,"source") == 0) {
                  if (strlen(res)>=sizeof(source)) {
                     debuga(_("Banning source name too long in redirector log file %s\n"),wentp);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(source,res);
               } else if(strcmp(leks,"list") == 0) {
                  if (strlen(res)>=sizeof(list)) {
                     debuga(_("Banning list name too long in redirector log file %s\n"),wentp);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(list,res);
               } else if(strcmp(leks,"ip") == 0) {
                  if (strlen(res)>=sizeof(ip)) {
                     debuga(_("IP address too long in redirector log file %s\n"),wentp);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(ip,res);
               } else if(strcmp(leks,"user") == 0) {
                  if (strlen(res)>=sizeof(user)) {
                     debuga(_("User ID too long in redirector log file %s\n"),wentp);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(user,res);
               } else if(strcmp(leks,"url") == 0) {
                  if (strlen(res)>=sizeof(url)) {
                     debuga(_("URL too long in redirector log file %s\n"),wentp);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(url,res);
               }
            }
         }
      } else {
         if (getword_atoll(&lyear,&gwarea,'-')<0 || getword_atoll(&lmon,&gwarea,'-')<0 ||
             getword_atoll(&lday,&gwarea,' ')<0) {
            debuga(_("Invalid date found in file %s\n"),wentp);
            exit(EXIT_FAILURE);
          }
          year=(int)lyear;
          mon=(int)lmon;
          day=(int)lday;
          if (getword(hour,sizeof(hour),&gwarea,' ')<0) {
             debuga(_("Invalid time found in file %s\n"),wentp);
             exit(EXIT_FAILURE);
          }
          if (getword_skip(MAXLEN,&gwarea,'(')<0 || getword(source,sizeof(source),&gwarea,'/')<0) {
             debuga(_("Invalid redirected source in file %s\n"),wentp);
             exit(EXIT_FAILURE);
          }
          if (getword(list,sizeof(list),&gwarea,'/')<0) {
             debuga(_("Invalid redirected list in file %s\n"),wentp);
             exit(EXIT_FAILURE);
          }
          if (getword_skip(MAXLEN,&gwarea,' ')<0 || getword_limit(url,sizeof(url),&gwarea,' ')<0) {
             debuga(_("Invalid URL in file %s\n"),wentp);
             exit(EXIT_FAILURE);
          }
          if (getword(ip,sizeof(ip),&gwarea,'/')<0) {
             debuga(_("Invalid source IP in file %s\n"),wentp);
             exit(EXIT_FAILURE);
          }
          if (getword_skip(MAXLEN,&gwarea,' ')<0 || getword(user,sizeof(user),&gwarea,' ')<0) {
             debuga(_("Invalid user in file %s\n"),wentp);
             exit(EXIT_FAILURE);
          }
          /*
          The URL may be "http://url:port/data" if the method is GET or simply "url:port/" if the method is CONNECT.
          The following code removes the protocol:// if it is detected and always truncates the URL after the domain name.
          It will fail if the URL doesn't start with the protocol and contains two consecutive / in the path (i.e.
          the URL is not normalized).
          */
          str=strchr(url,'/');
          if (str) {
             if (str[1]=='/') {
               str+=2;
               for (i=0 ; *str && *str!='/' ; i++) url[i]=*str++;
               url[i]='\0';
             } else {
               *str='\0';
             }
          }
      }

      //sprintf(warea,"%04d%02d%02d",year,mon,day);

      if(RedirectorFilterOutDate) {
         idata = year*10000+mon*100+day;
         if(idata < dfrom || idata > duntil)
            continue;
      }

      if(UserIp) {
         strcpy(user,ip);
         id_is_ip=true;
      } else {
         id_is_ip=false;
         if(strcmp(user,"-") == 0 || strcmp(user," ") == 0 || strcmp(user,"") == 0) {
            if(RecordsWithoutUser == RECORDWITHOUTUSER_IP) {
               strcpy(user,ip);
               id_is_ip=true;
            }
            if(RecordsWithoutUser == RECORDWITHOUTUSER_IGNORE)
               continue;
            if(RecordsWithoutUser == RECORDWITHOUTUSER_EVERYBODY)
               strcpy(user,"everybody");
         }
      }
      uinfo=userinfo_find_from_id(user);
      if (!uinfo) {
         uinfo=userinfo_create(user);
         uinfo->id_is_ip=id_is_ip;
         if(Ip2Name && id_is_ip) ip2name(user,sizeof(user));
         user_find(uinfo->label,MAX_USER_LEN, user);
      }
      fprintf(fp_ou,"%s\t%04d%02d%02d\t%s\t%s\t%s\t",uinfo->id,year,mon,day,hour,ip,url);
      if (source[0] && list[0])
         fprintf(fp_ou,"%s/%s\n",source,list);
      else if (source[0])
         fprintf(fp_ou,"%s\n",source);
      else
         fprintf(fp_ou,"%s\n",list);
      squidguard_count++;
   }
   fclose(fp_in);
   longline_destroy(&line);
   return;
}


void squidguard_log(void)
{
   FILE *fp_ou = NULL, *fp_guard = NULL;
   char buf[MAXLEN];
   char guard_in[MAXLEN];
   char guard_ou[MAXLEN];
   char logdir[MAXLEN];
   char user[MAXLEN];
   char tmp6[MAXLEN];
   int i;
   int  y;
   int cstatus;
   int dfrom, duntil;
   char *str;
   char *str2;

   str2 = user;

   if(SquidGuardConf[0] == '\0' && NRedirectorLogs == 0)
     return;

   sprintf(guard_in,"%s/redirector.unsort",tmp);
   sprintf(guard_ou,"%s/redirector.log",tmp);
   if((fp_ou=fopen(guard_in,"a"))==NULL) {
      debuga(_("(squidguard) Cannot open log file %s\n"),guard_in);
      exit(EXIT_FAILURE);
   }

   dfrom=(period.start.tm_year+1900)*10000+(period.start.tm_mon+1)*100+period.start.tm_mday;
   duntil=(period.end.tm_year+1900)*10000+(period.end.tm_mon+1)*100+period.end.tm_mday;

   if (NRedirectorLogs>0) {
      for (i=0 ; i<NRedirectorLogs ; i++)
         read_log(RedirectorLogs[i],fp_ou,dfrom,duntil);
   } else {
      if(access(SquidGuardConf, R_OK) != 0) {
         debuga(_("Cannot open squidGuard config file: %s\n"),SquidGuardConf);
         exit(EXIT_FAILURE);
      }

      if((fp_guard=fopen(SquidGuardConf,"r"))==NULL) {
         debuga(_("(squidguard) Cannot open log file %s\n"),SquidGuardConf);
         exit(EXIT_FAILURE);
      }

      logdir[0]=0;
      while(fgets(buf,sizeof(buf),fp_guard)!=NULL) {
         fixendofline(buf);
         if((str=get_param_value("logdir",buf))!=NULL) {
            /*
            We want to tolerate spaces inside the directory name but we must also
            remove the trailing spaces left by the editor after the directory name.
            This should not be a problem as nobody use a file name with trailing spaces.
            */
            for (y=strlen(str)-1 ; y>=0 && (unsigned char)str[y]<=' ' ; y--);
            if (y>=sizeof(logdir)-1) y=sizeof(logdir)-2;
            logdir[y+1] = '\0';
            while (y>=0) {
               logdir[y] = str[y];
               y--;
            }
         } else if((str=get_param_value("log",buf))!=NULL) {
            if((str2=get_param_value("anonymous",str))!=NULL)
               str=str2;

            /*
            If logdir is defined, we prepend it to the log file name, otherwise, we assume
            the log directive provides an absolute file name to the log file. Therefore,
            we don't need to add an additionnal / at the beginning of the log file name.
            */
            y=(logdir[0]) ? sprintf(wentp,"%s/",logdir) : 0;
            /*
            Spaces are allowed in the name of the log file. The file name ends at the first #
            because it is assumed it is an end of line comment. Any space before the # is then
            removed. Any control character (i.e. a character with a code lower than 32) ends
            the file name. That includes the terminating zero.
            */
            while((unsigned char)*str>=' ' && *str!='#' && y<sizeof(wentp)-1)
               wentp[y++]=*str++;
            if(*str=='#') {
               str--;
               while(*str==' ' && y>0) {
                  str--;
                  y--;
               }
            }
            wentp[y]=0;
            read_log(wentp,fp_ou,dfrom,duntil);
         }
      }
   }

   if (fp_guard) fclose(fp_guard);
   if (fp_ou) fclose(fp_ou);

   if (files_done) {
      for (y=0; y<nfiles_done; y++)
         if (files_done[y]) free(files_done[y]);
      free(files_done);
   }

   if(debug) {
      debuga(_("Sorting file: %s\n"),guard_ou);
   }

   sprintf(tmp6,"sort -k 1,1 -k 2,2 -k 4,4 \"%s\" -o \"%s\"",guard_in, guard_ou);
   cstatus=system(tmp6);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),tmp6);
      exit(EXIT_FAILURE);
   }

   unlink(guard_in);
   return;
}
