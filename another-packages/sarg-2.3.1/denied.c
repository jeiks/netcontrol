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

static void show_ignored_denied(FILE *fp_ou,int count)
{
   char ignored[80];

   snprintf(ignored,sizeof(ignored),ngettext("%d more denied access not shown here&hellip;","%d more denied accesses not shown here&hellip;",count),count);
   fprintf(fp_ou,"<tr><td class=\"data\"></td><td class=\"data\"></td><td class=\"data\"></td><td class=\"data2\"><i>%s</i></td></tr>\n",ignored);
}

void gen_denied_report(void)
{

   FILE *fp_in = NULL, *fp_ou = NULL;

   char *buf;
   char *url;
   char denied_in[MAXLEN];
   char report[MAXLEN];
   char ip[MAXLEN];
   char oip[MAXLEN];
   char user[MAXLEN];
   char ouser[MAXLEN];
   char ouser2[MAXLEN];
   char data[15];
   char hora[15];
   bool  z=false;
   int  count=0;
   int day,month,year;
   bool new_user;
   struct getwordstruct gwarea;
   longline line;
   struct userinfostruct *uinfo;
   struct tm t;

   ouser[0]='\0';
   ouser2[0]='\0';

   sprintf(denied_in,"%s/denied.log",tmp);
   if(!denied_count) {
      unlink(denied_in);
      return;
   }

   sprintf(report,"%s/denied.html",outdirname);

   if((fp_in=MY_FOPEN(denied_in,"r"))==NULL) {
     debuga(_("(denied) Cannot open log file %s\n"),denied_in);
     exit(EXIT_FAILURE);
   }

   if((fp_ou=MY_FOPEN(report,"w"))==NULL) {
     debuga(_("(denied) Cannot open log file %s\n"),report);
     exit(EXIT_FAILURE);
   }

   write_html_header(fp_ou,(IndexTree == INDEX_TREE_DATE) ? 3 : 1,_("Denied"));
   fputs("<tr><td class=\"header_c\">",fp_ou);
   fprintf(fp_ou,_("Period: %s"),period.html);
   fputs("</td></tr>\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_c\">%s</th></tr>\n",_("Denied"));
   close_html_header(fp_ou);

   fputs("<div class=\"report\"><table cellpadding=\"0\" cellspacing=\"2\">\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("USERID"),_("IP/NAME"),_("DATE/TIME"),_("ACCESSED SITE"));

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the denied accesses\n"));
      exit(EXIT_FAILURE);
   }

   while((buf=longline_read(fp_in,line))!=NULL) {
      getword_start(&gwarea,buf);
      if (getword(data,sizeof(data),&gwarea,'\t')<0 || getword(hora,sizeof(hora),&gwarea,'\t')<0 ||
          getword(user,sizeof(user),&gwarea,'\t')<0 || getword(ip,sizeof(ip),&gwarea,'\t')<0) {
         debuga(_("There is a broken record or garbage in file %s\n"),denied_in);
         exit(EXIT_FAILURE);
      }
      if (getword_ptr(buf,&url,&gwarea,'\t')<0) {
         debuga(_("There is a broken url in file %s\n"),denied_in);
         exit(EXIT_FAILURE);
      }
      if (sscanf(data,"%d/%d/%d",&day,&month,&year)!=3) continue;
      computedate(year,month,day,&t);
      strftime(data,sizeof(data),"%x",&t);

      uinfo=userinfo_find_from_id(user);
      if (!uinfo) {
         debuga(_("Unknown user ID %s in file %s\n"),user,denied_in);
         exit(EXIT_FAILURE);
      }

      new_user=false;
      if(!z) {
         strcpy(ouser,user);
         strcpy(oip,ip);
         z=true;
         new_user=true;
      } else {
         if(strcmp(ouser,user) != 0) {
            strcpy(ouser,user);
            new_user=true;
         }
         if(strcmp(oip,ip) != 0) {
            strcpy(oip,ip);
            new_user=true;
         }
      }

      if(DeniedReportLimit) {
         if(strcmp(ouser2,uinfo->label) == 0) {
            count++;
         } else {
            if(count>DeniedReportLimit && DeniedReportLimit>0)
               show_ignored_denied(fp_ou,count-DeniedReportLimit);
            count=1;
            strcpy(ouser2,uinfo->label);
         }
         if(count > DeniedReportLimit)
            continue;
      }

      fputs("<tr>",fp_ou);
      if (new_user)
         fprintf(fp_ou,"<td class=\"data\"><a href=\"%s/%s.html\">%s</a></td><td class=\"data\">%s</td>",uinfo->filename,uinfo->filename,uinfo->label,ip);
      else
         fputs("<td class=\"data\"></td><td class=\"data\"></td>",fp_ou);
      fprintf(fp_ou,"<td class=\"data\">%s-%s</td><td class=\"data2\">",data,hora);
      if(BlockIt[0] != '\0') {
         fprintf(fp_ou,"<a href=\"%s%s?url=",wwwDocumentRoot,BlockIt);
         output_html_url(fp_ou,url);
         fprintf(fp_ou,"\"><img src=\"%s/sarg-squidguard-block.png\"></a>&nbsp;",ImageFile);
      }
      fputs("<a href=\"http://",fp_ou);
      output_html_url(fp_ou,url);
      fputs("\">http://",fp_ou);
      output_html_string(fp_ou,url,100);
      fputs("</a></td></tr>\n",fp_ou);
   }
   fclose(fp_in);
   longline_destroy(&line);

   if(count>DeniedReportLimit && DeniedReportLimit>0)
      show_ignored_denied(fp_ou,count-DeniedReportLimit);

   fputs("</table></div>\n",fp_ou);
   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in file %s\n"),report);
   if (fclose(fp_ou)<0)
      debuga(_("Failed to close file %s - %s\n"),report,strerror(errno));

   if (unlink(denied_in)==-1)
      debuga(_("Failed to delete the file \"%s\" after processing it - %s\n"),denied_in,strerror(errno));

   return;
}
