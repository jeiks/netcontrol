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

void dansguardian_log(void)
{

   FILE *fp_in = NULL, *fp_ou = NULL, *fp_guard = NULL;
   char buf[MAXLEN];
   char guard_in[MAXLEN];
   char guard_ou[MAXLEN];
   char loglocation[MAXLEN] = "/var/log/dansguardian/access.log";
   char year[10], mon[10], day[10];
   char hour[15];
   char user[MAXLEN], code1[255], code2[255];
   char ip[30];
   char wdata[127];
   char *url;
   char tmp6[MAXLEN];
   int  idata=0;
   int cstatus;
   int dfrom, duntil;
   struct getwordstruct gwarea;

   dfrom=(period.start.tm_year+1900)*10000+(period.start.tm_mon+1)*100+period.start.tm_mday;
   duntil=(period.end.tm_year+1900)*10000+(period.end.tm_mon+1)*100+period.end.tm_mday;

   snprintf(guard_in,sizeof(guard_in),"%s/dansguardian.unsort",tmp);
   snprintf(guard_ou,sizeof(guard_ou),"%s/dansguardian.log",tmp);

   if(access(DansGuardianConf, R_OK) != 0) {
      debuga(_("Cannot open DansGuardian config file: %s\n"),DansGuardianConf);
      exit(EXIT_FAILURE);
   }

   if((fp_guard=fopen(DansGuardianConf,"r"))==NULL) {
      debuga(_("(dansguardian) Cannot open log file: %s\n"),DansGuardianConf);
      exit(EXIT_FAILURE);
   }

   if((fp_ou=MY_FOPEN(guard_in,"a"))==NULL) {
      debuga(_("(dansguardian) Cannot open log file: %s\n"),guard_in);
      exit(EXIT_FAILURE);
   }

   while(fgets(buf,sizeof(buf),fp_guard)!=NULL) {
      fixendofline(buf);
      if(buf[0]=='#')
         continue;
      if(strstr(buf,"loglocation ") != 0) {
         getword_start(&gwarea,buf);
         if (getword_skip(MAXLEN,&gwarea,'\'')<0 || getword(loglocation,sizeof(loglocation),&gwarea,'\'')<0) {
            debuga(_("Maybe you have a broken record or garbage in your %s file\n"),DansGuardianConf);
            exit(EXIT_FAILURE);
         }
         break;
      }
   }

   if(debug)
      debuga(_("Reading DansGuardian log file: %s\n"),loglocation);

   if((fp_in=MY_FOPEN(loglocation,"r"))==NULL) {
      debuga(_("(dansguardian) Cannot open log file: %s\n"),loglocation);
      exit(EXIT_FAILURE);
   }

   while(fgets(buf,sizeof(buf),fp_in) != NULL) {
      if(strstr(buf," *DENIED* ") == 0)
         continue;
      getword_start(&gwarea,buf);
      if (getword(year,sizeof(year),&gwarea,'.')<0 || getword(mon,sizeof(mon),&gwarea,'.')<0 ||
          getword(day,sizeof(day),&gwarea,' ')<0 || getword(hour,sizeof(hour),&gwarea,' ')<0 ||
          getword(user,sizeof(user),&gwarea,' ')<0 || getword(ip,sizeof(ip),&gwarea,' ')<0 ||
          getword_skip(MAXLEN,&gwarea,'/')<0 || getword_skip(MAXLEN,&gwarea,'/')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),loglocation);
         exit(EXIT_FAILURE);
      }
      if (getword_ptr(buf,&url,&gwarea,' ')<0) {
         debuga(_("Maybe you have a broken url in your %s file\n"),loglocation);
         exit(EXIT_FAILURE);
      }
      if (getword_skip(255,&gwarea,' ')<0 ||
          getword(code1,sizeof(code1),&gwarea,' ')<0 || getword(code2,sizeof(code2),&gwarea,' ')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),loglocation);
         exit(EXIT_FAILURE);
      }
      sprintf(wdata,"%s%02d%s",year,atoi(mon),day);
      idata = atoi(wdata);

      if(DansguardianFilterOutDate) {
         if(idata < dfrom && idata > duntil)
            continue;
      }

      if (strcmp(user,"-") == 0) {
         strcpy(user,ip);
         bzero(ip, 30);
      }
      fprintf(fp_ou,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",user,idata,hour,ip,url,code1,code2);
      dansguardian_count++;
   }

   if(fp_in) fclose(fp_in);
   if(fp_guard) fclose(fp_guard);
   if(fp_ou) fclose(fp_ou);

   if(debug)
      debuga(_("Sorting file: %s\n"),guard_ou);

   snprintf(tmp6,sizeof(tmp6),"sort -k 1,1 -k 2,2 -k 4,4 \"%s\" -o \"%s\"",guard_in, guard_ou);
   cstatus=system(tmp6);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),tmp6);
      exit(EXIT_FAILURE);
   }
   unlink(guard_in);
}
