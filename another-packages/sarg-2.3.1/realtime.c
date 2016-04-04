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

static int getdata(char*, FILE*);
static void datashow(const char *);
static void getlog(void);
static void header(void);

void realtime(void)
{

   getlog();

}

static void getlog(void)
{
   FILE *tmp, *fp;
   char template1[255]="/var/tmp/sargtpl1.XXXXXX";
   char template2[255]="/var/tmp/sargtpl2.XXXXXX";
   char cmd[512];
   char *buf;
   int  fd1,fd2;
   int cstatus;
   longline line;

   init_usertab(UserTabFile);

   fd1 = mkstemp(template1);
   fd2 = mkstemp(template2);

   if((fd1 == -1 ) || ((tmp = fdopen (fd1, "w+" )) == NULL)  ) {    /* failure, bail out */
       debuga(_("(realtime) mkstemp error - %s\n"),strerror(errno));
       exit(EXIT_FAILURE);
   }

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the log file\n"));
      exit(EXIT_FAILURE);
   }

   sprintf(cmd,"tail -%d \"%s\"",realtime_access_log_lines,AccessLog[0]);
   fp = popen(cmd, "r");
   if (!fp) {
      debuga(_("Failed to get the %d trailing lines of %s: %s\n"),realtime_access_log_lines,AccessLog[0],strerror(errno));
      debuga(_("tail command: %s\n"),cmd);
      exit(EXIT_FAILURE);
   }
   while((buf=longline_read(fp,line)) != NULL )
      if (getdata(buf,tmp)<0) {
         debuga(_("Maybe a broken record or garbage was returned by %s\n"),cmd);
         exit(EXIT_FAILURE);
      }
   pclose(fp);
   fclose(tmp);
   longline_destroy(&line);

   sprintf(cmd,"sort -r -k 1,1 -k 2,2 -o \"%s\" \"%s\"",template2,template1);
   cstatus=system(cmd);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),cmd);
      exit(EXIT_FAILURE);
   }
   unlink(template1);
   datashow(template2);
}

static int getdata(char *rec, FILE *ftmp)
{
   time_t tt;
   struct tm *t;
   char dat[128];
   char tbuf[128];
   char typ[128];
   char warea[MAXLEN];
   char user[MAX_USER_LEN];
   char ip[25];
   char *url;
   struct getwordstruct gwarea;

   getword_start(&gwarea,rec);
   if (getword(dat,sizeof(dat),&gwarea,' ')<0) {
      debuga(_("The time stamp at column 1 is too long\n"));
      return(-1);
   }
   if (getword(warea,sizeof(warea),&gwarea,' ')<0) {
      debuga(_("The connection duration at column 2 is too long\n"));
      return(-1);
   }
   while(strcmp(warea,"") == 0 && gwarea.current[0] != '\0')
      if (getword(warea,sizeof(warea),&gwarea,' ')<0) {
         return(-1);
      }
   if (getword(ip,sizeof(ip),&gwarea,' ')<0) {
      debuga(_("The IP address at column 3 is too long\n"));
      return(-1);
   }
   if (getword_skip(MAXLEN,&gwarea,' ')<0) {
      debuga(_("The status at column 4 is too long\n"));
      return(-1);
   }
   if (getword_skip(MAXLEN,&gwarea,' ')<0) {
      debuga(_("The size at column 5 is too long\n"));
      return(-1);
   }
   if (getword(typ,sizeof(typ),&gwarea,' ')<0) {
      debuga(_("The action at column 6 is too long\n"));
      return(-1);
   }
   if(strncmp(typ,"CONNECT",7) == 0) {
      if (getword_ptr(rec,&url,&gwarea,' ')<0) {
         debuga(_("The URL at column 7 is too long\n"));
         return(-1);
      }
      if (getword(user,sizeof(user),&gwarea,' ')<0) {
         debuga(_("The user ID at column 8 is too long\n"));
         return(-1);
      }
   }else {
      if (getword_skip(MAXLEN,&gwarea,'/')<0) {
         debuga(_("The URL at column 7 is too long\n"));
         return(-1);
      }
      if (getword_skip(MAXLEN,&gwarea,'/')<0) {
         debuga(_("The URL at column 7 is too long\n"));
         return(-1);
      }
      if (getword_ptr(rec,&url,&gwarea,'/')<0) {
         debuga(_("The URL at column 7 is too long\n"));
         return(-1);
      }
      if (getword_skip(MAXLEN,&gwarea,' ')<0) {
         debuga(_("The data at column 8 is too long\n"));
         return(-1);
      }
      if (getword(user,sizeof(user),&gwarea,' ')<0) {
         debuga(_("The user at column 9 is too long\n"));
         return(-1);
      }
   }

   if(strncmp(user,"-",1) == 0 && RealtimeUnauthRec==REALTIME_UNAUTH_REC_IGNORE)
      return(0);

   tt=atoi(dat);
   t=localtime(&tt);
   if(strncmp(DateFormat,"u",1) == 0)
      strftime(tbuf, sizeof(tbuf), "%Y-%m-%d\t%H:%M", t);
   else if(strncmp(DateFormat,"e",1) == 0)
      strftime(tbuf, sizeof(tbuf), "%d-%m-%Y\t%H:%M", t);

   fprintf(ftmp,"%s\t%s\t%s\t%s\t%s\n",tbuf,ip,user,url,typ);
   return(0);
}

static void datashow(const char *tmp)
{
   FILE *fin;
   char dat[128];
   char tim[128];
   char *buf;
   char *url;
   char *ourl=NULL;
   char ouser[MAX_USER_LEN]="";
   char typ[128];
   char user[MAX_USER_LEN];
   char u2[MAX_USER_LEN];
   char ip[25];
   int url_len;
   int ourl_size=0;
   struct getwordstruct gwarea;
   longline line;

   if((fin=fopen(tmp,"r"))==NULL) {
      debuga(_("(realtime) open error %s - %s\n"),tmp,strerror(errno));
      exit(EXIT_FAILURE);
   }

   header();

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the log file\n"));
      exit(EXIT_FAILURE);
   }

   while((buf=longline_read(fin,line))!=NULL) {
      fixendofline(buf);
      getword_start(&gwarea,buf);
      if (getword(dat,sizeof(dat),&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),tmp);
         exit(EXIT_FAILURE);
      }
      if (getword(tim,sizeof(tim),&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),tmp);
         exit(EXIT_FAILURE);
      }
      if (getword(ip,sizeof(ip),&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),tmp);
         exit(EXIT_FAILURE);
      }
      if (getword(user,sizeof(user),&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),tmp);
         exit(EXIT_FAILURE);
      }
      if(strlen(dat) < 3 || strlen(user) < 1) continue;
      if (getword_ptr(buf,&url,&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken url in your %s file\n"),tmp);
         exit(EXIT_FAILURE);
      }
      if (getword(typ,sizeof(typ),&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),tmp);
         exit(EXIT_FAILURE);
      }
      if(strstr(RealtimeTypes,typ) == 0)
         continue;

      if(strcmp(ouser,user) == 0 && ourl && strcmp(ourl,url) == 0)
         continue;

      if(UserIp)
         strcpy(user,ip);
      strcpy(u2,user);
      if(Ip2Name)
         ip2name(u2,sizeof(u2));
      user_find(name, sizeof(name), u2);

      printf("<tr><td class=\"data\">%s %s</td><td class=\"data3\">%s</td><td class=\"data3\">%s</td><td class=\"data3\">%s</td><td class=\"data2\"><a href=\"http://%s\">%s</td></tr>\n",dat,tim,ip,name,typ,url,url);
      strcpy(ouser,user);

      url_len=strlen(url);
      if (!ourl || url_len>=ourl_size) {
         ourl_size=url_len+1;
         ourl=realloc(ourl,ourl_size);
         if (!ourl) {
            debuga(_("Not enough memory to store the url\n"));
            exit(EXIT_FAILURE);
         }
      }
      strcpy(ourl,url);
   }
   longline_destroy(&line);
   if (ourl) free(ourl);

   puts("</table>\n</div>\n</body>\n</html>\n");
   fclose(fin);
   unlink(tmp);
   fflush(NULL);

}

static void header(void)
{
   puts("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"");
   puts(" \"http://www.w3.org/TR/html4/loose.dtd\">\n");
   puts("<html>\n");
   puts("<head>\n");
   if(realtime_refresh)
      printf("  <meta http-equiv=refresh content=\"%d\" url=\"sarg-php/sarg-realtime.php\"; charset=\"%s\">\n",realtime_refresh,CharSet);
   else
      printf("  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\">\n",CharSet);
   css(stdout);
   puts("</head>\n");
   printf("<body style=\"font-family:%s;font-size:%s;background-color:%s;background-image:url(%s)\">\n",FontFace,TitleFontSize,BgColor,BgImage);
   puts("<div align=\"center\"><table cellpadding=\"1\" cellspacing=\"1\">\n");
   printf("<tr><th class=\"title_l\" colspan=\"10\">SARG %s</th></tr>\n",_("Realtime"));
   printf("<tr><th class=\"text\" colspan=\"10\">%s: %d s</th></tr>\n",_("Auto refresh"),realtime_refresh);
   printf("<tr><th class=\"header_c\">%s</th><th class=\"header_c\">%s</th><th class=\"header_c\">%s</th><th class=\"header_c\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("DATE/TIME"),_("IP/NAME"),_("USERID"),_("TYPE"),_("ACCESSED SITE"));
}
