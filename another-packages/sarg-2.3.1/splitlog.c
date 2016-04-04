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

void splitlog(const char *arq, char *df, int dfrom, int duntil, int convert)
{
   FILE *fp_in;
   char *buf;
   char data[30];
   char dia[11];
   time_t tt;
   int idata=0;
   struct tm *t;
   struct getwordstruct gwarea;
   longline line;

   if(arq[0] == '\0')
      arq="/var/log/squid/access.log";

   if((fp_in=MY_FOPEN(arq,"r"))==NULL) {
      debuga(_("(splitlog) Cannot open log file %s - %s\n"),arq,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the log file %s\n"),arq);
      exit(EXIT_FAILURE);
   }

   while((buf=longline_read(fp_in,line))!=NULL) {
      getword_start(&gwarea,buf);
      if (getword(data,sizeof(data),&gwarea,' ')<0) {
         debuga(_("Invalid date found in file %s\n"),arq);
         exit(EXIT_FAILURE);
      }
      tt=atoi(data);
      t=localtime(&tt);

      if(dfrom) {
         idata=(t->tm_year+1900)*10000+(t->tm_mon+1)*100+t->tm_mday;
         if(idata < dfrom || idata > duntil)
            continue;
      }

      if(!convert) {
         printf("%s %s\n",data,gwarea.current);
      } else {
         if(df[0]=='e')
            strftime(dia, sizeof(dia), "%d/%m/%Y", t);
         else
            strftime(dia, sizeof(dia), "%m/%d/%Y", t);

         printf("%s %02d:%02d:%02d %s\n",dia,t->tm_hour,t->tm_min,t->tm_sec,gwarea.current);
      }
   }

   longline_destroy(&line);
   if (fclose(fp_in)==EOF) {
      debuga(_("Failed to close file %s - %s\n"),arq,strerror(errno));
   }
}
