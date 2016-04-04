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

void squidguard_report(void)
{

   FILE *fp_in = NULL, *fp_ou = NULL;

   char *buf;
   char *url;
   char squidguard_in[MAXLEN];
   char report[MAXLEN];
   char ip[MAXLEN];
   char rule[255];
   char oip[MAXLEN];
   char user[MAXLEN];
   char ouser[MAXLEN];
   char data[15];
   char hora[15];
   char ouser2[255];
   char oname[MAXLEN];
   bool  z=false;
   int  count=0;
   long long int data2;
   bool new_user;
   struct getwordstruct gwarea;
   const struct userinfostruct *uinfo;
   struct tm t;
   longline line;

   ouser[0]='\0';
   ouser2[0]='\0';

   sprintf(squidguard_in,"%s/redirector.log",tmp);
   if(!squidguard_count) {
      unlink(squidguard_in);
      return;
   }

   snprintf(report,sizeof(report),"%s/redirector.html",outdirname);

   if((fp_in=fopen(squidguard_in,"r"))==NULL) {
     debuga(_("(squidguard) Cannot open log file %s\n"),squidguard_in);
     exit(EXIT_FAILURE);
   }

   if((fp_ou=fopen(report,"w"))==NULL) {
     debuga(_("(squidguard) Cannot open log file %s\n"),report);
     exit(EXIT_FAILURE);
   }

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the processed redirector log\n"));
      exit(EXIT_FAILURE);
   }

   write_html_header(fp_ou,(IndexTree == INDEX_TREE_DATE) ? 3 : 1,_("Redirector report"));
   fputs("<tr><td class=\"header_c\">",fp_ou);
   fprintf(fp_ou,_("Period: %s"),period.html);
   fputs("</td></tr>\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_c\">%s</th></tr>\n",_("Redirector report"));
   close_html_header(fp_ou);

   fputs("<div class=\"report\"><table cellpadding=1 cellspacing=2>\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("USERID"),_("IP/NAME"),_("DATE/TIME"),_("ACCESSED SITE"),_("RULE"));

   while((buf=longline_read(fp_in,line))!=NULL) {
      getword_start(&gwarea,buf);
      if (getword(user,sizeof(user),&gwarea,'\t')<0) {
         debuga(_("Invalid user in file %s\n"),squidguard_in);
         exit(EXIT_FAILURE);
      }
      if (getword_atoll(&data2,&gwarea,'\t')<0) {
         debuga(_("Invalid date in file %s\n"),squidguard_in);
         exit(EXIT_FAILURE);
      }
      if (getword(hora,sizeof(hora),&gwarea,'\t')<0) {
         debuga(_("Invalid time in file %s\n"),squidguard_in);
         exit(EXIT_FAILURE);
      }
      if (getword(ip,sizeof(ip),&gwarea,'\t')<0) {
         debuga(_("Invalid IP address in file %s\n"),squidguard_in);
         exit(EXIT_FAILURE);
      }
      if (getword_ptr(buf,&url,&gwarea,'\t')<0) {
         debuga(_("Invalid URL in file %s\n"),squidguard_in);
         exit(EXIT_FAILURE);
      }
      if (getword(rule,sizeof(rule),&gwarea,'\n')<0) {
         debuga(_("Invalid rule in file %s\n"),squidguard_in);
         exit(EXIT_FAILURE);
      }

      uinfo=userinfo_find_from_id(user);
      if (!uinfo) {
         debuga(_("Unknown user ID %s in file %s\n"),user,squidguard_in);
         exit(EXIT_FAILURE);
      }

      computedate(data2/10000,(data2/100)%10,data2%100,&t);
      strftime(data,sizeof(data),"%x",&t);

      new_user=false;
      if(!z) {
         strcpy(ouser,user);
         strcpy(oip,ip);
         strcpy(oname,ip);
         if (Ip2Name && !uinfo->id_is_ip) ip2name(oname,sizeof(oname));
         z=true;
         new_user=true;
      } else {
         if(strcmp(ouser,user) != 0) {
            strcpy(ouser,user);
            new_user=true;
         }
         if(strcmp(oip,ip) != 0) {
            strcpy(oip,ip);
            strcpy(oname,ip);
            if (Ip2Name && !uinfo->id_is_ip) ip2name(oname,sizeof(oname));
            new_user=true;
         }
      }

      if(SquidGuardReportLimit) {
         if(strcmp(ouser2,uinfo->label) == 0) {
            count++;
         } else {
            count=1;
            strcpy(ouser2,uinfo->label);
         }
         if(count >= SquidGuardReportLimit)
            continue;
      }

      if (new_user)
         fprintf(fp_ou,"<tr><td class=\"data2\">%s</td><td class=\"data2\">%s</td>",uinfo->label,ip);
      else
         fputs("<tr><td class=\"data2\"></td><td class=\"data2\"></td>",fp_ou);
      fprintf(fp_ou,"<td class=\"data2\">%s-%s</td><td class=\"data2\"><a href=\"http://",data,hora);
      output_html_url(fp_ou,url);
      fputs("\">",fp_ou);
      output_html_string(fp_ou,url,100);
      fprintf(fp_ou,"</a></td><td class=\"data2\">%s</td></tr>\n",rule);
   }
   fclose(fp_in);
   longline_destroy(&line);

   fputs("</table></div>\n",fp_ou);
   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in file %s\n"),report);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close file %s - %s\n"),report,strerror(errno));

   unlink(squidguard_in);

   return;
}
