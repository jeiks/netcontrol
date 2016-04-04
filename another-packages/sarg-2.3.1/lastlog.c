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

void mklastlog(const char *outdir)
{

   FILE *fp_in, *fp_ou;
   DIR *dirp;
   struct dirent *direntp;
   char buf[MAXLEN];
   char temp[MAXLEN];
   char warea[MAXLEN];
   char ftime[128];
   int  ftot=0;
   time_t t;
   struct tm *local;
   struct stat statb;
   int cstatus;
   struct getwordstruct gwarea;

   if(LastLog <= 0)
      return;

   snprintf(temp,sizeof(temp),"%slastlog1",outdir);
   if((fp_ou=fopen(temp,"w"))==NULL) {
     debuga(_("(lastlog) Cannot open temporary file %s\n"),temp);
     exit(EXIT_FAILURE);
   }

   if ((dirp = opendir(outdir)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),outdir,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ((direntp = readdir( dirp )) != NULL ){
      if(strchr(direntp->d_name,'-') == 0)
         continue;

      snprintf(warea,sizeof(warea),"%s%s",outdir,direntp->d_name);
      if (stat(warea,&statb) == -1) {
         debuga(_("Failed to get the creation time of %s\n"),warea);
         continue;
      }
      t=statb.st_ctime;
      local = localtime(&t);
      strftime(ftime, sizeof(ftime), "%Y%m%d%H%M%S", local);
      fprintf(fp_ou,"%s\t%s\n",ftime,direntp->d_name);
      ftot++;
   }

   closedir( dirp );
   fclose(fp_ou);

   snprintf(buf,sizeof(buf),"sort -n -k 1,1 -o \"%slastlog\" \"%s\"",outdir,temp);
   cstatus=system(buf);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),buf);
      exit(EXIT_FAILURE);
   }

   unlink(temp);

   if(ftot<=LastLog) {
      snprintf(temp,sizeof(temp),"%slastlog",outdir);
      if(access(temp, R_OK) == 0)
         unlink(temp);
      return;
   }

   ftot-=LastLog;

   snprintf(temp,sizeof(temp),"%slastlog",outdir);
   if((fp_in=fopen(temp,"r"))==NULL) {
     debuga(_("(lastlog) Cannot open temporary file %s\n"),temp);
     exit(EXIT_FAILURE);
   }

   while(ftot>0 && fgets(buf,sizeof(buf),fp_in)!=NULL) {
      fixendofline(buf);
      getword_start(&gwarea,buf);
      if (getword(warea,sizeof(warea),&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),temp);
         exit(EXIT_FAILURE);
      }

      if(debug)
         debuga(_("Removing old report file %s\n"),gwarea.current);
      if (snprintf(temp,sizeof(temp),"%s%s",outdir,gwarea.current)>=sizeof(temp)) {
         debuga(_("Directory name too long: %s%s\n"),outdir,gwarea.current);
         exit(EXIT_FAILURE);
      }
      unlinkdir(temp,0);
      ftot--;
   }

   fclose(fp_in);
   snprintf(temp,sizeof(temp),"%slastlog",outdir);
   if (unlink(temp) == -1) {
      debuga(_("Failed to delete the file %s\n"),temp);
   }

   return;
}
