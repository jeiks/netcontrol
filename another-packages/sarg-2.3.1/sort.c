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

void tmpsort(void)
{

   DIR *dirp;
   struct dirent *direntp;
   int cstatus;
   const char tmpext[]=".utmp";
   int dlen;
   char csort[MAXLEN];
   char arqou[MAXLEN], arqin[MAXLEN], wnome[MAXLEN];
   const char *field1="2,2";
   const char *field2="1,1";
   const char *field3="3,3";
   const char *order;

   if(indexonly) return;
   if((ReportType & REPORT_TYPE_USERS_SITES) == 0) return;

   if((UserSort & USER_SORT_CONNECT) != 0) {
      field1="1,1";
      field2="2,2";
      field3="3,3";
   } else if((UserSort & USER_SORT_SITE) != 0) {
      field1="3,3";
      field2="2,2";
      field3="1,1";
   } else if((UserSort & USER_SORT_TIME) != 0) {
      field1="5,5";
      field2="2,2";
      field3="1,1";
   }

   if((UserSort & USER_SORT_REVERSE) == 0)
      order="";
   else
      order="-r";

   if ((dirp = opendir(tmp)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),tmp,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ((direntp = readdir( dirp )) != NULL ){
      dlen=strlen(direntp->d_name)-(sizeof(tmpext)-1);
      if (dlen<0) continue;
      if(strcmp(direntp->d_name+dlen,tmpext) != 0)
         continue;

      if (dlen>0) {
         if (dlen>=sizeof(wnome)) continue;
         strncpy(wnome,direntp->d_name,dlen);
         wnome[dlen]='\0';
      } else {
         wnome[0]='\0';
      }

      strcpy(arqou,tmp);
      strcat(arqou,"/");
      strcpy(arqin,arqou);
      strcat(arqou,wnome);
      strcat(arqin,direntp->d_name);

      if(debug) {
         debuga(_("Sorting file: %s\n"),arqou);
      }

      strcat(arqou,".txt");
      sprintf(csort,"sort -n -T \"%s\" %s -k %s -k %s -k %s -o \"%s\" \"%s\"",tmp,order,field1,field2,field3,arqou,arqin);
      cstatus=system(csort);
      if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
         debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
         debuga(_("sort command: %s\n"),csort);
         exit(EXIT_FAILURE);
      }
      unlink(arqin);

   }

   (void)closedir( dirp );
   return;
}

void sort_users_log(const char *tmp, int debug)
{
   DIR *dirp;
   struct dirent *direntp;
   char csort[MAXLEN];
   char user[MAXLEN];
   char wdname[MAXLEN];
   int cstatus;
   int dlen;
   int clen;
   const char unsortext[]=".unsort";

   if(debug) {
      debuga(_("pre-sorting files\n"));
   }

   if ((dirp = opendir(tmp)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),tmp,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ( (direntp = readdir( dirp )) != NULL ){
      dlen=strlen(direntp->d_name)-(sizeof(unsortext)-1);
      if (dlen<0) continue;
      if(strcmp(direntp->d_name+dlen,unsortext) != 0)
         continue;
      if(strcmp(direntp->d_name,"authfail.log.unsort") == 0)
         continue;

      if (dlen>0) {
         if (dlen>=sizeof(user)) continue;
         strncpy(user,direntp->d_name,dlen);
         user[dlen]=0;
      } else {
         user[0]='\0';
      }

      if(strcmp(direntp->d_name,"download.unsort") == 0)
         clen=snprintf(csort,sizeof(csort),"sort -T \"%s\" -k 3,3 -k 1,1 -k 2,2 -k 5,5 -o \"%s/%s.log\" \"%s/%s.unsort\"",
              tmp, tmp, user, tmp, user);
      else
         clen=snprintf(csort,sizeof(csort),"sort -T \"%s\" -k 4,4 -k 1,1 -k 2,2 -o \"%s/%s.log\" \"%s/%s.unsort\"",
              tmp, tmp, user, tmp, user);
      if (clen>=sizeof(csort)) {
         debuga(_("user name too long to sort %s\n"),csort);
         exit(EXIT_FAILURE);
      }
      cstatus=system(csort);
      if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
         debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
         debuga(_("sort command: %s\n"),csort);
         exit(EXIT_FAILURE);
      }
      if (snprintf(wdname,sizeof(wdname),"%s/%s.unsort",tmp,user)>=sizeof(wdname)) {
         debuga(_("user name too long for %s/%s.unsort\n"),tmp,user);
         exit(EXIT_FAILURE);
      }
      unlink(wdname);
   }
   (void)closedir( dirp );

   return;
}

void sort_labels(const char **label,const char **order)
{
   if((UserSort & USER_SORT_CONNECT) != 0) {
      *label=_("connect");
   } else if((UserSort & USER_SORT_SITE) != 0) {
      *label=_("site");
   } else if((UserSort & USER_SORT_TIME) != 0) {
      *label=_("time");
   } else {
      *label=_("bytes");
   }

   if((UserSort & USER_SORT_REVERSE) == 0)
      *order=_("normal");
   else
      *order=_("reverse");
}
