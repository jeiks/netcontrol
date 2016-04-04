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

#ifdef HAVE_LDAP_H
#define LDAP_DEPRECATED 1

#include <ldap.h>
#include <ldap_cdefs.h>
#include <ldap_features.h>
#endif //HAVE_LDAP_H

enum UserTabEnum
{
   //! Users matched against the ::UserTabFile file.
   UTT_File,
   //! Users matched agains a LDAP.
   UTT_Ldap,
   //! No user matching performed.
   UTT_None
};

enum UserTabEnum which_usertab=UTT_None;

static char *userfile=NULL;

#ifdef HAVE_LDAP_H
static LDAP *ldap_handle=NULL;
#endif //HAVE_LDAP_H

static void init_file_usertab(const char *UserTabFile)
{
   FILE *fp_usr;
   long int nreg;
   char buf[MAXLEN];
   int z1, z2;

   if((fp_usr=fopen(UserTabFile,"r"))==NULL) {
      debuga(_("(usertab) Cannot open file %s - %s\n"),UserTabFile,strerror(errno));
      exit(EXIT_FAILURE);
   }
   if (fseek(fp_usr, 0, SEEK_END)==-1) {
      debuga(_("Failed to move till the end of the usertab file %s: %s\n"),UserTabFile,strerror(errno));
      exit(EXIT_FAILURE);
   }
   nreg = ftell(fp_usr);
   if (nreg<0) {
      debuga(_("Cannot get the size of file %s"),UserTabFile);
      exit(EXIT_FAILURE);
   }
   nreg += 100;
   if (fseek(fp_usr, 0, SEEK_SET)==-1) {
      debuga(_("Failed to rewind the usertab file %s: %s\n"),UserTabFile,strerror(errno));
      exit(EXIT_FAILURE);
   }
   if((userfile=(char *) malloc(nreg))==NULL){
      debuga(_("ERROR: Cannot load. Memory fault"));
      exit(EXIT_FAILURE);
   }
   userfile[0]='\t';
   z2=1;
   while(fgets(buf,sizeof(buf),fp_usr)!=NULL) {
      if (buf[0]=='#') continue;
      fixendofline(buf);
      z1=0;
      while(buf[z1] && (unsigned char)buf[z1]>' ') {
         if (z2+3>=nreg) { //need at least 3 additional bytes for the minimum string "\n\t\0"
            debuga(_("The list of the users is too long in your %s file.\n"),UserTabFile);
            exit(EXIT_FAILURE);
         }
         userfile[z2++]=buf[z1++];
      }
      while(buf[z1] && (unsigned char)buf[z1]<=' ') z1++;
      userfile[z2++]='\n';
      while(buf[z1] && (unsigned char)buf[z1]>=' ') {
         if (z2+2>=nreg) { //need at least 2 additional bytes for "\t\0"
            debuga(_("The list of the users is too long in your %s file.\n"),UserTabFile);
            exit(EXIT_FAILURE);
         }
         userfile[z2++]=buf[z1++];
      }
      while(userfile[z2-1]==' ') z2--;
      userfile[z2++]='\t';
   }
   userfile[z2]='\0';
   fclose(fp_usr);
}

static void get_usertab_name(const char *user,char *name,int namelen)
{
   char warea[MAXLEN];
   char *str;

   namelen--;
   sprintf(warea,"\t%s\n",user);
   if((str=(char *) strstr(userfile,warea)) == (char *) NULL ) {
      strncpy(name,user,namelen);
      name[namelen]=0;
   } else {
      str=strchr(str+1,'\n');
      str++;
      for(z1=0; *str != '\t' && z1<namelen ; z1++) {
         name[z1]=*str++;
      }
      name[z1]=0;
   }
}

#ifdef HAVE_LDAP_H
static void init_ldap_usertab(void) {
   /* Setting LDAP connection and initializing cache */
   ldap_handle = NULL;
   if ((ldap_handle = ldap_init(LDAPHost, LDAPPort)) == NULL) {
      debuga(_("Unable to connect to LDAP server %s on port %d\n"), LDAPHost, LDAPPort);
      exit(EXIT_FAILURE);
   }

   int ldap_protocol_version = LDAPProtocolVersion;
   if (ldap_set_option(ldap_handle, LDAP_OPT_PROTOCOL_VERSION, &ldap_protocol_version) != LDAP_SUCCESS) {
      debuga(_("Could not set LDAP protocol version %d\n"), ldap_protocol_version);
      exit(EXIT_FAILURE);
   }

   /* Bind to the LDAP server. */
   int rc;
   rc = ldap_simple_bind_s( ldap_handle, LDAPBindDN, LDAPBindPW );
   if ( rc != LDAP_SUCCESS ) {
      debuga(_("Cannot bind to LDAP server: %s\n"), ldap_err2string(rc));
      exit(EXIT_FAILURE);
   }

   /* Initializing cache */

   init_cache();
}

static void get_ldap_name(const char *userlogin,char *mappedname,int namelen)
{
   /* Start searching username in cache */
   // According to rfc2254 section 4, only *()\ and NUL must be escaped. This list is rather conservative !
   const char strictchars[] = " ~!@^&(){}|<>?:;\"\'\\[]`,\r\n\0";
   char filtersearch[256], *searched_in_cache;
   char searchloginname[3*MAX_USER_LEN];
   char *attr, **vals;
   LDAPMessage *result, *e;
   BerElement *ber;
   int i;

   for (i=0 ; i<sizeof(searchloginname)-1 && *userlogin ; userlogin++) {
      if (strchr(strictchars,*userlogin)) {
         // escape character according to rfc2254 section 4
         if (i+3>=sizeof(searchloginname)-1) break;
         i+=sprintf(searchloginname+i,"\\%02X",*userlogin);
      } else {
         searchloginname[i++]=*userlogin;
      }
   }
   searchloginname[i]='\0';

   if (!(searched_in_cache = search_in_cache(searchloginname))) {
      snprintf(filtersearch, sizeof(filtersearch), LDAPFilterSearch, searchloginname, searchloginname, searchloginname, searchloginname, searchloginname);

      /* Search record(s) in LDAP base */

      int rc= ldap_search_s(ldap_handle, LDAPBaseSearch, LDAP_SCOPE_SUBTREE, filtersearch, NULL, 0, &result);
      if ( rc != LDAP_SUCCESS ) {
         debuga(_("LDAP search failed: %s\n"), ldap_err2string(rc));
         strncpy(mappedname,searchloginname,namelen-1);
         mappedname[namelen-1]='\0';
         return;
      }

      if (!(e = ldap_first_entry(ldap_handle, result)))
         insert_to_cache(searchloginname, searchloginname);
      else
         for (attr = ldap_first_attribute(ldap_handle, e, &ber); attr != NULL; attr = ldap_next_attribute(ldap_handle, e, ber)) {
            if (!strcasecmp(attr, LDAPTargetAttr)) {
               if ((vals = (char **)ldap_get_values(ldap_handle, e, attr))!=NULL) {
                  insert_to_cache(searchloginname, vals[0]);
                  strncpy(mappedname, vals[0],namelen-1);
                  mappedname[namelen-1]='\0';
                  ldap_memfree(vals);
               }
               ldap_memfree(attr);
               break;
            }
            ldap_memfree(attr);
         }
         ldap_msgfree(result);
   } else {
       strncpy(mappedname, searched_in_cache,namelen-1);
       mappedname[namelen-1]='\0';
   }
}
#endif //HAVE_LDAP_H

void init_usertab(const char *UserTabFile)
{
   if (strcmp(UserTabFile, "ldap") == 0) {
      if(debug)
         debuga(_("Loading User table: %s\n"),UserTabFile);
#ifdef HAVE_LDAP_H
      which_usertab=UTT_Ldap;
      init_ldap_usertab();
#else
      debuga(_("LDAP module not compiled in sarg\n"));
      exit(EXIT_FAILURE);
#endif //HAVE_LDAP_H
   } else if (UserTabFile[0] != '\0') {
      if(debug)
         debuga(_("Loading User table: %s\n"),UserTabFile);
      which_usertab=UTT_File;
      init_file_usertab(UserTabFile);
   } else {
      which_usertab=UTT_None;
   }
}

void user_find(char *mappedname, int namelen, const char *userlogin)
{
   if (which_usertab==UTT_File) {
      get_usertab_name(userlogin,mappedname,namelen);
   }
#ifdef HAVE_LDAP_H
   else if (which_usertab==UTT_Ldap) {
      get_ldap_name(userlogin,mappedname,namelen);
   }
#endif //HAVE_LDAP_H
   else {
      strncpy(mappedname,userlogin,namelen-1);
      mappedname[namelen-1]='\0';
   }
}

void close_usertab(void)
{
#ifdef HAVE_LDAP_H
   if (ldap_handle) {
      destroy_cache();
      ldap_unbind(ldap_handle);
      ldap_handle=NULL;
   }
#endif //HAVE_LDAP_H
   if(userfile) {
      free(userfile);
      userfile=NULL;
   }
}

