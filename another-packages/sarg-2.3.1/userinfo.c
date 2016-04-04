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

//! The number of users to group in one unit.
#define USERS_PER_GROUP 50

/*! \brief Group the users in one allocation unit.
Structure to store a group of users and reduce the number of memory
allocations.
*/
struct usergroupstruct
{
   //! The next group of users.
   struct usergroupstruct *next;
   //! A group of users.
   struct userinfostruct list[USERS_PER_GROUP];
   //! The number of users stored in the list.
   int nusers;
};

//! The first group of users.
static struct usergroupstruct *first_user_group=NULL;

struct userinfostruct *userinfo_create(const char *userid)
{
   struct usergroupstruct *group, *last;
   struct userinfostruct *user;
   int i, lastuser;
   int skip;
   int flen;
   int count, clen;
   char cstr[9];

   last=NULL;
   for (group=first_user_group ; group ; group=group->next) {
      if (group->nusers<USERS_PER_GROUP) break;
      last=group;
   }

   if (!group) {
      group=malloc(sizeof(*group));
      if (!group) {
         debuga(_("Not enough memory to store the user\n"));
         exit(EXIT_FAILURE);
      }
      memset(group,0,sizeof(*group));
      if (last)
         last->next=group;
      else
         first_user_group=group;
   }
   user=group->list+group->nusers++;

   strncpy(user->id,userid,MAX_USER_LEN-1);
   user->id[MAX_USER_LEN-1]='\0';

   skip=0;
   for(i=0 ; userid[i] && i<MAX_USER_FNAME_LEN-1 ; i++) {
      if(isalnum(userid[i]) || userid[i]=='-' || userid[i]=='_') {
         user->filename[i]=userid[i];
         skip=0;
      } else {
         if (!skip) {
            user->filename[i]='_';
            skip=1;
         }
      }
   }
   user->filename[i]='\0';
   flen=i;

   count=0;
   for (group=first_user_group ; group ; group=group->next) {
      lastuser=(group->next) ? group->nusers : group->nusers-1;
      for (i=0 ; i<lastuser ; i++) {
         if (strcasecmp(user->filename,group->list[i].filename)==0) {
            clen=sprintf(cstr,"-%04X",count++);
            if (flen+clen<MAX_USER_FNAME_LEN)
               strcpy(user->filename+flen,cstr);
            else
               strcpy(user->filename+MAX_USER_FNAME_LEN-clen,cstr);
         }
      }
   }

   return(user);
}

void userinfo_free(void)
{
   struct usergroupstruct *group, *next;

   for (group=first_user_group ; group ; group=next) {
      next=group->next;
      free(group);
   }
   first_user_group=NULL;
}

struct userinfostruct *userinfo_find_from_file(const char *filename)
{
   struct usergroupstruct *group;
   int i;

   for (group=first_user_group ; group ; group=group->next) {
      for (i=0 ; i<group->nusers ; i++)
         if (strcmp(filename,group->list[i].filename)==0)
            return(group->list+i);
   }
   return(NULL);
}

struct userinfostruct *userinfo_find_from_id(const char *id)
{
   struct usergroupstruct *group;
   int i;

   for (group=first_user_group ; group ; group=group->next) {
      for (i=0 ; i<group->nusers ; i++)
         if (strcmp(id,group->list[i].id)==0)
            return(group->list+i);
   }
   return(NULL);
}
