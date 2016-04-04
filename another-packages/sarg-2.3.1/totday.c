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

void day_totalize(const char *tmp, const struct userinfostruct *uinfo, int indexonly)
{

   FILE *fp_in, *fp_ou;

   char buf[200];
   char date[20];
   long long int hour;
   long long int bytes;
   long long int elap;
   long long int tbytes[MAX_DATETIME_DAYS*24];
   long long int telap[MAX_DATETIME_DAYS*24];
   int day,month,year;
   int daylist[MAX_DATETIME_DAYS];
   int ndaylist;
   int daynum;
   int dayidx;
   int i;
   char wdirname[2048];
   char arqout[2048];
   char colsep;
   struct getwordstruct gwarea;

   if(indexonly) return;
   if((ReportType & REPORT_TYPE_USERS_SITES) == 0) return;
   if (datetimeby==0) return;

   if (snprintf(wdirname,sizeof(wdirname),"%s/%s.htmp",tmp,uinfo->filename)>=sizeof(wdirname)) {
      debuga(_("File name too long: %s/%s%s\n"),tmp,uinfo->filename,".htmp");
      exit(EXIT_FAILURE);
   }
   if (snprintf(arqout,sizeof(arqout),"%s/%s.day",tmp,uinfo->filename)>=sizeof(arqout)) {
      debuga(_("File name too long: %s/%s%s\n"),tmp,uinfo->filename,".day");
      exit(EXIT_FAILURE);
   }

   if((fp_in=fopen(wdirname,"r"))==NULL) {
      debuga(_("(totday) Cannot open log file %s\n"),wdirname);
      exit(EXIT_FAILURE);
   }

   memset(tbytes,0,sizeof(tbytes));
   memset(telap,0,sizeof(tbytes));
   ndaylist=0;

   while(fgets(buf,sizeof(buf),fp_in)!=NULL) {
      fixendofline(buf);
      getword_start(&gwarea,buf);
      if (getword(date,sizeof(date),&gwarea,'\t')<0 || getword_atoll(&hour,&gwarea,'\t')<0) {
         debuga(_("There is a broken record or garbage in file %s\n"),wdirname);
         exit(EXIT_FAILURE);
      }
      if (sscanf(date,"%d/%d/%d",&day,&month,&year)!=3) continue;
      if (day<1 || day>31 || month<1 || month>12 || year>9999) continue;
      if (hour<0 || hour>=24) continue;
      daynum=(year*10000)+(month*100)+day;
      for (dayidx=0 ; dayidx<ndaylist && daynum!=daylist[dayidx] ; dayidx++);
      if (dayidx>=ndaylist) {
         if (dayidx>=sizeof(daylist)/sizeof(*daylist)) {
            debuga(_("Too many different dates in %s\n"),wdirname);
            exit(EXIT_FAILURE);
         }
         daylist[ndaylist++]=daynum;
      }
      i=dayidx*24+hour;
      if ((datetimeby & DATETIME_BYTE)!=0) {
         colsep=((datetimeby & DATETIME_ELAP)!=0) ? '\t' : '\0';
         if (getword_atoll(&bytes,&gwarea,colsep)<0) {
            debuga(_("Invalid number of bytes in file %s\n"),wdirname);
            exit(EXIT_FAILURE);
         }
         tbytes[i]+=bytes;
      }
      if ((datetimeby & DATETIME_ELAP)!=0) {
         if (getword_atoll(&elap,&gwarea,'\0')<0) {
            debuga(_("Invalid elapsed time in file %s\n"),wdirname);
            exit(EXIT_FAILURE);
         }
         telap[i]+=elap;
      }
   }
   fclose(fp_in);

   if((fp_ou=fopen(arqout,"w"))==NULL) {
     debuga(_("(totday) Cannot open log file %s\n"),arqout);
     exit(EXIT_FAILURE);
   }

   for (i=0 ; i<sizeof(tbytes)/sizeof(*tbytes) ; i++) {
      if (tbytes[i]==0 && telap[i]==0) continue;
      dayidx=i/24;
      if (dayidx>sizeof(daylist)/sizeof(*daylist)) continue;
      hour=i%24;
      daynum=daylist[dayidx];
      day=daynum%100;
      month=(daynum/100)%100;
      year=daynum/10000;
      fprintf(fp_ou,"%d/%d/%d\t%d",day,month,year,(int)hour);
      if ((datetimeby & DATETIME_BYTE)!=0) fprintf(fp_ou,"\t%"PRIu64"",(uint64_t)tbytes[i]);
      if ((datetimeby & DATETIME_ELAP)!=0) fprintf(fp_ou,"\t%"PRIu64"",(uint64_t)telap[i]);
      fputs("\n",fp_ou);
   }

   if (fclose(fp_ou)==EOF) {
      debuga(_("Failed to close file %s - %s\n"),arqout,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (unlink(wdirname)==-1) {
      debuga(_("Cannot delete temporary file %s - %s\n"),wdirname,strerror(errno));
      exit(EXIT_FAILURE);
   }
   return;
}
