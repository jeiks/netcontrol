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


void index_only(const char *dirname,int debug)
{
   DIR *dirp;
   struct dirent *direntp;
   char remove[MAXLEN];

   if ((dirp = opendir(dirname)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),dirname,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ( (direntp = readdir( dirp )) != NULL ){
      if(strcmp(direntp->d_name,".") == 0 || strcmp(direntp->d_name,"..") == 0 || strcmp(direntp->d_name, "index.html") == 0)
         continue;

      if (snprintf(remove,sizeof(remove),"%s/%s",dirname,direntp->d_name)>=sizeof(remove)) {
         debuga(_("Name of the file to remove is too long: %s/%s\n"),dirname,direntp->d_name);
         continue;
      }
      if (unlink(remove) == -1) {
         debuga(_("Failed to remove the file %s\n"),remove);
      }
   }

   (void)closedir( dirp );

   return;
}
