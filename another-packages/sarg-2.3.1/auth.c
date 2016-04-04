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

void htaccess(const struct userinfostruct *uinfo)
{
   char htname[MAXLEN];
   char line[MAXLEN];
   FILE *fp_in;
   FILE *fp_auth;
   size_t i,nread;

   if(!UserAuthentication)
      return;

   if (snprintf(htname,sizeof(htname),"%s/%s/.htaccess",outdirname,uinfo->filename)>=sizeof(htname)) {
      debuga(_("File name too long: %s/%s/.htaccess\n"),outdirname,uinfo->filename);
      exit(EXIT_FAILURE);
   }
   if((fp_auth=fopen(htname,"w"))==NULL) {
      debuga(_("(auth) Cannot open file: %s - %s\n"),htname,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if ((fp_in=fopen(AuthUserTemplateFile,"r"))==NULL) {
      debuga(_("(auth) Cannot open template file: %s - %s\n"),AuthUserTemplateFile,strerror(errno));
      exit(EXIT_FAILURE);
   }

   while((nread=fread(line,1,sizeof(line),fp_in))!=0) {
      for (i=0 ; i<nread ; i++)
         if (line[i]=='%' && i+2<nread && line[i+1]=='u' && !isalpha(line[i+2])) {
            fputs(uinfo->id,fp_auth);
            i++;
         } else {
            fputc(line[i],fp_auth);
      }
   }
   fclose(fp_auth);
   fclose(fp_in);

   return;
}
