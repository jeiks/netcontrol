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

void topsites(void)
{

   FILE *fp_in, *fp_ou;

   char *buf;
   char *url;
   char *ourl=NULL;
   char csort[255];
   char general[MAXLEN];
   char general2[MAXLEN];
   char general3[MAXLEN];
   char sites[MAXLEN];
   char report[MAXLEN];
   const char *sortf;
   const char *sortt;
   long long int nacc;
   long long int nbytes;
   long long int ntime;
   long long int tnacc=0;
   long long int tnbytes=0;
   long long int tntime=0;
   long long int twork1=0, twork2=0, twork3=0;
   int regs=0;
   int cstatus;
   int url_len;
   int ourl_size=0;
   struct getwordstruct gwarea;
   longline line;
   struct generalitemstruct item;

   if(Privacy)
      return;

   sprintf(general,"%s/sarg-general",outdirname);
   sprintf(sites,"%s/sarg-sites",outdirname);
   sprintf(general2,"%s/sarg-general2",outdirname);
   sprintf(general3,"%s/sarg-general3",outdirname);

   if ((ReportType & REPORT_TYPE_TOPUSERS) == 0)
      sprintf(report,"%s/index.html",outdirname);
   else
      sprintf(report,"%s/topsites.html",outdirname);

   sprintf(csort,"sort -k 4,4 -o \"%s\" \"%s\"",general2,general);
   cstatus=system(csort);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }

   if((fp_in=fopen(general2,"r"))==NULL) {
     debuga(_("(topsites) Cannot open log file %s\n"),general2);
     debuga(_("sort command: %s\n"),csort);
     exit(EXIT_FAILURE);
   }

   if((fp_ou=fopen(general3,"w"))==NULL) {
     debuga(_("(topsites) Cannot open log file %s\n"),general3);
     exit(EXIT_FAILURE);
   }

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read file %s\n"),general2);
      exit(EXIT_FAILURE);
   }

   while((buf=longline_read(fp_in,line))!=NULL) {
      ger_read(buf,&item,general2);
      if(item.total) continue;

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
         regs++;
      }

      if(strcmp(item.url,ourl) != 0) {
         /*
         This complicated printf is due to Microsoft's inability to comply with any standard. Msvcrt is unable
         to print a long long int unless it is exactly 64-bits long.
         */
         fprintf(fp_ou,"%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%s\n",(uint64_t)tnacc,(uint64_t)tnbytes,(uint64_t)tntime,ourl);
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
         tnacc=0;
         tnbytes=0;
         tntime=0;
      }

      tnacc+=item.nacc;
      tnbytes+=item.nbytes;
      tntime+=item.nelap;
   }
   fclose(fp_in);
   unlink(general2);
   longline_destroy(&line);

   if (ourl) {
      /*
      This complicated printf is due to Microsoft's inability to comply with any standard. Msvcrt is unable
      to print a long long int unless it is exactly 64-bits long.
      */
      fprintf(fp_ou,"%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%s\n",(uint64_t)tnacc,(uint64_t)tnbytes,(uint64_t)tntime,ourl);
      free(ourl);
   }

   fclose(fp_ou);

   if((TopsitesSort & TOPSITE_SORT_CONNECT) != 0) {
      sortf="-k 1,1 -k 2,2";
   } else {
      sortf="-k 2,2 -k 1,1";
   }
   if((TopsitesSort & TOPSITE_SORT_REVERSE) != 0) {
      sortt="-r";
   } else {
      sortt="";
   }

   sprintf(csort,"sort %s -n %s -o \"%s\" \"%s\"",sortt,sortf,sites,general3);
   cstatus=system(csort);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }
   if((fp_in=fopen(sites,"r"))==NULL) {
      debuga(_("(topsites) Cannot open log file %s\n"),sites);
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }

   unlink(general2);
   unlink(general3);

   if((fp_ou=fopen(report,"w"))==NULL) {
     debuga(_("(topsites) Cannot open log file %s\n"),report);
     exit(EXIT_FAILURE);
   }

   write_html_header(fp_ou,(IndexTree == INDEX_TREE_DATE) ? 3 : 1,_("Top sites"));
   fputs("<tr><td class=\"header_c\">",fp_ou);
   fprintf(fp_ou,_("Period: %s"),period.html);
   fputs("</td></tr>\n",fp_ou);
   fputs("<tr><th class=\"header_c\">",fp_ou);
   fprintf(fp_ou,_("Top %d sites"),TopSitesNum);
   fputs("</th></tr>\n",fp_ou);
   close_html_header(fp_ou);

   fputs("<div class=\"report\"><table cellpadding=\"1\" cellspacing=\"2\">\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("NUM"),_("ACCESSED SITE"),_("CONNECT"),_("BYTES"),_("TIME"));

   regs=0;
   ntopsites = 0;

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read file %s\n"),sites);
      exit(EXIT_FAILURE);
   }

   while(regs<TopSitesNum && (buf=longline_read(fp_in,line))!=NULL) {
      getword_start(&gwarea,buf);
      if (getword_atoll(&nacc,&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),sites);
         exit(EXIT_FAILURE);
      }
      if (nacc == 0) continue;
      if (getword_atoll(&nbytes,&gwarea,'\t')<0 || getword_atoll(&ntime,&gwarea,'\t')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),sites);
         exit(EXIT_FAILURE);
      }
      if (getword_ptr(buf,&url,&gwarea,'\t')<0) {
         debuga(_("The url is invalid in file %s\n"),sites);
         exit(EXIT_FAILURE);
      }

      twork1=nacc;
      twork2=nbytes;
      twork3=ntime;

      strcpy(wwork1,fixnum(twork1,1));
      strcpy(wwork2,fixnum(twork2,1));
      strcpy(wwork3,fixtime(twork3));

      fprintf(fp_ou,"<tr><td class=\"data\">%d</td><td class=\"data2 link\">",++regs);

      if(BlockIt[0] != '\0') {
         fprintf(fp_ou,"<a href=\"%s%s?url=\"",wwwDocumentRoot,BlockIt);
         output_html_url(fp_ou,url);
         fputs("\"><img src=\"../images/sarg-squidguard-block.png\"></a>&nbsp;",fp_ou);
      }

      fputs("<a href=\"http://",fp_ou);
      output_html_url(fp_ou,url);
      fputs("\">",fp_ou);
      output_html_string(fp_ou,url,100);
      fprintf(fp_ou,"</a></td><td class=\"data\">%s</td><td class=\"data\">%s</td><td class=\"data\">%s</td></tr>\n",wwork1,wwork2,wwork3);
   }
   fclose(fp_in);
   longline_destroy(&line);

   fputs("</table></div>\n",fp_ou);
   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in file %s\n"),report);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close file %s - %s\n"),report,strerror(errno));

   return;

}
