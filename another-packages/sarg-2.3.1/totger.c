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

int totalger(const char *dirname, int debug, const char *outdir)
{
   FILE *fp_in;
   long long int tnacc=0;
   long long int tnbytes=0;
   long long int telap=0;
   long long int tincache=0, toucache=0;
   char wger[MAXLEN];
   char *warea;
   longline line;
   struct generalitemstruct item;

   snprintf(wger,sizeof(wger),"%s/sarg-general",dirname);
   if((fp_in=fopen(wger,"r+"))==NULL) {
      debuga(_("(totger) Cannot open file %s\n"),wger);
      exit(EXIT_FAILURE);
   }

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the temporary file %s\n"),wger);
      exit(EXIT_FAILURE);
   }

   while((warea=longline_read(fp_in,line))!=NULL)
   {
      ger_read(warea,&item,wger);
      tnacc+=item.nacc;
      tnbytes+=item.nbytes;
      telap+=item.nelap;
      tincache+=item.incache;
      toucache+=item.oucache;
   }

   longline_destroy(&line);

   if (fseek(fp_in,0,SEEK_END)==-1) {
      debuga(_("Failed to move to the end of %s - %s\n"),wger,strerror(errno));
      exit(EXIT_FAILURE);
   }

   /*
   This complicated printf is due to Microsoft's inability to comply with any standard. Msvcrt is unable
   to print a long long int unless it is exactly 64-bits long.
   */
   if (fprintf(fp_in,"TOTAL\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\n",(uint64_t)tnacc,(uint64_t)tnbytes,(uint64_t)telap,(uint64_t)tincache,(uint64_t)toucache)<0) {
      debuga(_("Failed to write the total line in %s\n"),wger);
      exit(EXIT_FAILURE);
   }
   if (fclose(fp_in)==EOF) {
      debuga(_("Failed to close file %s - %s\n"),wger,strerror(errno));
      exit(EXIT_FAILURE);
   }

   return (0);
}
