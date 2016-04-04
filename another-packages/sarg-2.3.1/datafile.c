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

void saverecs(FILE *fp_ou, const struct userinfostruct *uinfo, long long int nacc, char *url, long long int nbytes, char *ip, char *hora, char *dia, long long int nelap, long long int incache, long long int oucache);

void data_file(char *tmp)
{

   FILE *fp_in, *fp_ou=NULL;

   char *buf;
   char accdia[11], acchora[9], accip[MAXLEN], *accurl;
   char oldaccdia[11], oldacchora[9], oldaccip[MAXLEN];
   char *oldurl;
   char olduser[MAXLEN], acccode[50], oldacccode[50];
   char ipantes[MAXLEN], nameantes[MAXLEN];
   char accsmart[MAXLEN];
   char crc2[50];
   char *str;
   char tmp3[MAXLEN];
   char user[MAX_USER_LEN];
   char u2[MAX_USER_LEN];
   DIR *dirp;
   struct dirent *direntp;
   long long int nbytes=0;
   long long int nelap=0;
   long long int nacc=0;
   int rtotal=0;
   long long int incache=0;
   long long int oucache=0;
   long long int accbytes;
   long long int accelap;
   int dlen;
   int new_user;
   int same_url;
   int url_len;
   int ourl_size;
   struct getwordstruct gwarea;
   struct userinfostruct *uinfo;
   longline line;
   const char logext[]=".log";

   ipantes[0]='\0';
   nameantes[0]='\0';

   olduser[0]='\0';
   oldurl=NULL;
   ourl_size=0;

   if ((dirp = opendir(tmp)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),tmp,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ( (direntp = readdir( dirp )) != NULL ) {
      dlen=strlen(direntp->d_name)-(sizeof(logext)-1);
      if (dlen<=0) continue;
      if(strcmp(direntp->d_name+dlen,logext) != 0)
         continue;

      if (dlen>0) {
         if (dlen>=sizeof(user)) continue;
         strncpy(user,direntp->d_name,dlen);
         user[dlen]=0;
      } else {
         user[0]='\0';
      }

      uinfo=userinfo_find_from_file(user);
      if (!uinfo) {
         debuga(_("Ignoring unknown user file %s\n"),user);
         continue;
      }
      strcpy(u2,uinfo->id);
      if(Ip2Name && uinfo->id_is_ip) {
         strcpy(ipantes,u2);
         ip2name(u2,sizeof(u2));
         strcpy(nameantes,u2);
      }
      user_find(uinfo->label,MAX_USER_LEN, u2);

      if (snprintf(tmp3,sizeof(tmp3),"%s/%s",tmp,direntp->d_name)>=sizeof(tmp3)) {
         debuga(_("(datafile) directory path too long: %s/%s\n"),tmp,direntp->d_name);
         exit(EXIT_FAILURE);
      }

      if((fp_in=MY_FOPEN(tmp3,"r"))==NULL){
         debuga(_("(datafile) Cannot open file %s\n"),tmp3);
         exit(EXIT_FAILURE);
      }

      if ((line=longline_create())==NULL) {
         debuga(_("Not enough memory to read the downloaded files.\n"));
         exit(EXIT_FAILURE);
      }

      ttopen=0;
      new_user=1;
      while((buf=longline_read(fp_in,line))!=NULL) {
         getword_start(&gwarea,buf);
         if (getword(accdia,sizeof(accdia),&gwarea,'\t')<0 || getword(acchora,sizeof(acchora),&gwarea,'\t')<0 ||
             getword(accip,sizeof(accip),&gwarea,'\t')<0 ||
             getword_ptr(buf,&accurl,&gwarea,'\t')<0 || getword_atoll(&accbytes,&gwarea,'\t')<0 ||
             getword(acccode,sizeof(acccode),&gwarea,'\t')<0 || getword_atoll(&accelap,&gwarea,'\t')<0) {
            debuga(_("There is a broken record or garbage in file %s\n"),tmp3);
            exit(EXIT_FAILURE);
         }
         if (getword_skip(20000,&gwarea,'"')<0 || getword(accsmart,sizeof(accsmart),&gwarea,'"')<0) {
            debuga(_("There is an invalid smart info in file %s\n"),tmp3);
            exit(EXIT_FAILURE);
         }

         if(Ip2Name) {
            if(strcmp(accip,ipantes) != 0) {
               strcpy(ipantes,accip);
               ip2name(accip,sizeof(accip));
               strcpy(nameantes,accip);
            }
            else strcpy(accip,nameantes);
         }

         if(!rtotal){
            url_len=strlen(accurl);
            if (!oldurl || url_len>=ourl_size) {
               ourl_size=url_len+1;
               oldurl=realloc(oldurl,ourl_size);
               if (!oldurl) {
                  debuga(_("Not enough memory to store the url\n"));
                  exit(EXIT_FAILURE);
               }
            }
            strcpy(oldurl,accurl);
            strcpy(oldacccode,acccode);
            strcpy(oldaccip,accip);
            strcpy(oldaccdia,accdia);
            strcpy(oldacchora,acchora);
            new_user=0;
            rtotal++;
         }
         same_url=(strcmp(oldurl,accurl) == 0);

         if(!same_url || new_user){
            if(!fp_ou && (fp_ou=MY_FOPEN(DataFile,"w"))==NULL){
               debuga(_("(datafile) Cannot open file %s\n"),DataFile);
               exit(EXIT_FAILURE);
            }
            saverecs(fp_ou,uinfo,nacc,oldurl,nbytes,oldaccip,oldacchora,oldaccdia,nelap,incache,oucache);
            nacc=0;
            nbytes=0;
            nelap=0;
            incache=0;
            oucache=0;
            new_user=0;
         }

         nacc++;
         nbytes+=accbytes;
         nelap+=accelap;

         strcpy(crc2,acccode);
         str=strchr(crc2,'/');
         if (str) *str='\0';

         if(strstr(crc2,"MISS") != 0) oucache+=accbytes;
         else incache+=accbytes;

         if (!same_url) {
            url_len=strlen(accurl);
            if (url_len>=ourl_size) {
               ourl_size=url_len+1;
               oldurl=realloc(oldurl,ourl_size);
               if (!oldurl) {
                  debuga(_("Not enough memory to store the url\n"));
                  exit(EXIT_FAILURE);
               }
            }
            strcpy(oldurl,accurl);
         }
         new_user=0;
         strcpy(oldacccode,acccode);
         strcpy(oldaccip,accip);
         strcpy(oldaccdia,accdia);
         strcpy(oldacchora,acchora);
      }

      fclose(fp_in);
      longline_destroy(&line);
   }
   (void)closedir( dirp );
   if (oldurl) free(oldurl);
   if (fp_ou) fclose(fp_ou);

   if(debug)
      debuga(_("Datafile %s written successfully\n"),DataFile);
}

void saverecs(FILE *fp_ou, const struct userinfostruct *uinfo, long long int nacc, char *url, long long int nbytes, char *ip, char *hora, char *dia, long long int nelap, long long int incache, long long int oucache)
{
   char val[20];
   char name[512];

   if((DataFileFields & DATA_FIELD_USER) != 0) {
      fputs(uinfo->label,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_DATE) != 0) {
      fputs(dia,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_TIME) != 0) {
      fputs(hora,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_URL) != 0) {
      url_hostname(url,name,sizeof(name));
      if (DataFileUrl == DATAFILEURL_IP) name2ip(name);
      fputs(name,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_CONNECT) != 0) {
      my_lltoa(nacc,val,sizeof(val),0);
      fputs(val,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_BYTES) != 0) {
      my_lltoa(nbytes,val,sizeof(val),0);
      fputs(val,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_IN_CACHE) != 0) {
      my_lltoa(incache,val,sizeof(val),0);
      fputs(val,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_OUT_CACHE) != 0) {
      my_lltoa(oucache,val,sizeof(val),0);
      fputs(val,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }
   if((DataFileFields & DATA_FIELD_ELAPSED) != 0) {
      my_lltoa(nelap,val,sizeof(val),0);
      fputs(val,fp_ou);
      fputc(DataFileDelimiter[0],fp_ou);
   }

   fputc('\n',fp_ou);
}
