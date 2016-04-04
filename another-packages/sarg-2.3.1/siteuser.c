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

void siteuser(void)
{

   FILE *fp_in, *fp_ou;

   char *buf;
   char wuser[MAXLEN];
   char *ourl;
   char csort[255];
   char general[MAXLEN];
   char general2[MAXLEN];
   char sites[MAXLEN];
   char report[MAXLEN];
   int regs=0;
   int ucount=0;
   int ourl_size;
   int url_len;
   char *users;
   long long int obytes;
   int cstatus;
   longline line;
   struct generalitemstruct item;
   const struct userinfostruct *uinfo;

   if(Privacy)
      return;

   nsitesusers = 0;
   sprintf(general,"%s/sarg-general",outdirname);
   sprintf(sites,"%s/sarg-sites",outdirname);
   sprintf(general2,"%s/sarg-general2",outdirname);
   sprintf(report,"%s/siteuser.html",outdirname);

   sprintf(csort,"sort -k 4,4 -k 1,1 -o \"%s\" \"%s\"",general2,general);
   cstatus=system(csort);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }

   if((fp_in=fopen(general2,"r"))==NULL) {
     debuga(_("(siteuser) Cannot open log file %s\n"),general2);
     debuga(_("sort command: %s\n"),csort);
     exit(EXIT_FAILURE);
   }

   if((fp_ou=fopen(report,"w"))==NULL) {
     debuga(_("(siteuser) Cannot open log file %s\n"),report);
     exit(EXIT_FAILURE);
   }

   write_html_header(fp_ou,(IndexTree == INDEX_TREE_DATE) ? 3 : 1,_("Sites & Users"));
   fputs("<tr><td class=\"header_c\">",fp_ou);
   fprintf(fp_ou,_("Period: %s"),period.html);
   fputs("</td></tr>\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_c\">%s</th></tr>\n",_("Sites & Users"));
   close_html_header(fp_ou);

   fputs("<div class=\"report\"><table cellpadding=\"0\" cellspacing=\"2\">\n",fp_ou);
   if(BytesInSitesUsersReport)
      fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("NUM"),_("ACCESSED SITE"),_("BYTES"),_("USERS"));
   else
      fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("NUM"),_("ACCESSED SITE"),_("USERS"));

   ourl=NULL;
   ourl_size=0;
   obytes=0;

   if((users=(char *) malloc(204800))==NULL){
      debuga(_("ERROR: Cannot load. Memory fault\n"));
      exit(EXIT_FAILURE);
   }
   strcpy(users," ");

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read file %s\n"),general2);
      exit(EXIT_FAILURE);
   }

   while((buf=longline_read(fp_in,line))!=NULL) {
      ger_read(buf,&item,general2);
      if(item.total) continue;
      uinfo=userinfo_find_from_id(item.user);
      if (!uinfo) {
         debuga(_("Unknown user ID %s in file %s\n"),item.user,general2);
         exit(EXIT_FAILURE);
      }

      if (item.nacc > 0) nsitesusers = 1;
      if(!regs) {
         url_len=strlen(item.url);
         if (!ourl || url_len>=ourl_size) {
            ourl_size=url_len+1;
            ourl=realloc(ourl,ourl_size);
            if (!ourl) {
               debuga(_("Not enough memory to store the url\n"));
               exit(EXIT_FAILURE);
            }
         }
         strcpy(ourl,item.url);
         obytes=item.nbytes;
         regs++;
      }

      sprintf(wuser," %s ",uinfo->label);
      if(strstr(users,wuser) == 0 && strcmp(item.url,ourl) == 0) {
         strcat(users,uinfo->label);
         strcat(users," ");
         ucount++;
         if(ucount>4) {
            strcat(users,"<br>");
            ucount=0;
         }
      }

      if(SiteUsersReportLimit) {
         if(regs >= SiteUsersReportLimit)
            continue;
      }

      if(strcmp(item.url,ourl) != 0 && nsitesusers) {
         fprintf(fp_ou,"<tr><td class=\"data\">%d</td><td class=\"data2\">",regs);
         if(BlockIt[0]!='\0') {
            fprintf(fp_ou,"<a href=\"%s%s?url=",wwwDocumentRoot,BlockIt);
            output_html_url(fp_ou,ourl);
            fputs("\"><img src=\"../images/sarg-squidguard-block.png\"></a>&nbsp;",fp_ou);
         }
         fputs("<a href=\"http://",fp_ou);
         output_html_url(fp_ou,ourl);
         fputs("\">",fp_ou);
         output_html_string(fp_ou,ourl,100);
         fputs("</a></td>",fp_ou);

         if(BytesInSitesUsersReport)
            fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum(obytes,1));
         fprintf(fp_ou,"<td class=\"data2\">%s</td></tr>\n",users);

         regs++;
         ucount=0;
         strcpy(users,uinfo->label);
         strcat(users," ");
         url_len=strlen(item.url);
         if (url_len>=ourl_size) {
            ourl_size=url_len+1;
            ourl=realloc(ourl,ourl_size);
            if (!ourl) {
               debuga(_("Not enough memory to store the url\n"));
               exit(EXIT_FAILURE);
            }
         }
         strcpy(ourl,item.url);
         obytes=item.nbytes;
      }
   }
   fclose(fp_in);
   longline_destroy(&line);

   if(nsitesusers) {
      fprintf(fp_ou,"<tr><td class=\"data\">%d</td><td class=\"data2\"><a href=\"http://",regs);
      output_html_url(fp_ou,ourl);
      fputs("\">",fp_ou);
      output_html_string(fp_ou,ourl,100);
      fprintf(fp_ou,"</a></td><td class=\"data2\">%s</td></tr>\n",users);
   }
   if (ourl) free(ourl);

   unlink(general2);

   fputs("</table></div>\n",fp_ou);
   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in file %s\n"),report);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close file %s - %s\n"),report,strerror(errno));

   if(users)
      free(users);

   return;
}
