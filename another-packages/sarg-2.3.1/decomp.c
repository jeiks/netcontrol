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

FILE *decomp(const char *arq, bool *pipe)
{
   char cmd[1024];
   int arqlen;

   if(access(arq, R_OK) != 0) {
      debuga(_("File not found: %s\n"),arq);
      exit(EXIT_FAILURE);
   }

   arqlen=strlen(arq);
   if(arqlen>3 && strcmp(arq+arqlen-3,".gz") == 0) {
      debuga(_("Decompressing log file \"%s\" with zcat\n"),arq);
      if (snprintf(cmd,sizeof(cmd),"zcat \"%s\"",arq)>=sizeof(cmd)) {
         debuga(_("decompression command too long for log file %s\n"),arq);
         exit(EXIT_FAILURE);
      }
      *pipe=true;
      return(popen(cmd,"r"));
   }

   if(arqlen>4 && strcmp(arq+arqlen-4,".bz2") == 0) {
      debuga(_("Decompressing log file \"%s\" with bzcat\n"),arq);
      if (snprintf(cmd,sizeof(cmd),"bzcat \"%s\"",arq)>=sizeof(cmd)) {
         debuga(_("decompression command too long for log file %s\n"),arq);
         exit(EXIT_FAILURE);
      }
      *pipe=true;
      return(popen(cmd,"r"));
   }

   if(arqlen>2 && strcmp(arq+arqlen-2,".Z") == 0) {
      debuga(_("Decompressing log file \"%s\" with zcat\n"),arq);
      if (snprintf(cmd,sizeof(cmd),"zcat \"%s\"",arq)>=sizeof(cmd)) {
         debuga(_("decompression command too long for log file %s\n"),arq);
         exit(EXIT_FAILURE);
      }
      *pipe=true;
      return(popen(cmd,"r"));
   }

   *pipe=false;
   return(MY_FOPEN(arq,"r"));
}
