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

void ccharset(char *CharSet)
{
   if(strcmp(CharSet,"Latin2") == 0) strcpy(CharSet,"ISO-8859-2");
   else if(strcmp(CharSet,"Latin3") == 0) strcpy(CharSet,"ISO-8859-3");
   else if(strcmp(CharSet,"Latin4") == 0) strcpy(CharSet,"ISO-8859-4");
   else if(strcmp(CharSet,"Cyrillic") == 0) strcpy(CharSet,"ISO-8859-5");
   else if(strcmp(CharSet,"Arabic") == 0) strcpy(CharSet,"ISO-8859-6");
   else if(strcmp(CharSet,"Greek") == 0) strcpy(CharSet,"ISO-8859-7");
   else if(strcmp(CharSet,"Hebrew") == 0) strcpy(CharSet,"ISO-8859-8");
   else if(strcmp(CharSet,"Latin5") == 0) strcpy(CharSet,"ISO-8859-9");
   else if(strcmp(CharSet,"Latin6") == 0) strcpy(CharSet,"ISO-8859-10");
   else if(strcmp(CharSet,"Windows-1251") == 0) strcpy(CharSet,"Windows-1251");
   else if(strcmp(CharSet,"Japan") == 0) strcpy(CharSet,"EUC-JP");
   else if(strcmp(CharSet,"Koi8-r") == 0) strcpy(CharSet,"KOI8-R");
   else if(strcmp(CharSet,"UTF-8") == 0) strcpy(CharSet,"UTF-8");
   return;
}
