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

/*@null@*/static char *DownloadSuffix=NULL;
/*@null@*/static char **DownloadSuffixIndex=NULL;
static int NDownloadSuffix=0;

void download_report(void)
{

   FILE *fp_in = NULL, *fp_ou = NULL;

   char *buf;
   char *url;
   char report_in[MAXLEN];
   char report[MAXLEN];
   char ip[MAXLEN];
   char oip[MAXLEN];
   char user[MAXLEN];
   char ouser[MAXLEN];
   char ouser2[MAXLEN];
   char data[15];
   char hora[15];
   int  z=0;
   int  count=0;
   int i;
   int day,month,year;
   bool new_user;
   struct getwordstruct gwarea;
   longline line;
   struct userinfostruct *uinfo;
   struct tm t;

   ouser[0]='\0';
   ouser2[0]='\0';

   snprintf(report_in,sizeof(report_in),"%s/download.log",tmp);
   if(access(report_in, R_OK) != 0)
      return;

   snprintf(report,sizeof(report),"%s/download.html",outdirname);

   if((fp_in=MY_FOPEN(report_in,"r"))==NULL) {
      debuga(_("(download) Cannot open log file %s\n"),report_in);
      exit(EXIT_FAILURE);
   }

   if((fp_ou=MY_FOPEN(report,"w"))==NULL) {
      debuga(_("(download) Cannot open log file %s\n"),report);
      exit(EXIT_FAILURE);
   }

   write_html_header(fp_ou,(IndexTree == INDEX_TREE_DATE) ? 3 : 1,_("Downloads"));
   fputs("<tr><td class=\"header_c\">",fp_ou);
   fprintf(fp_ou,_("Period: %s"),period.html);
   fputs("</td></tr>\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_c\">%s</th></tr>\n",_("Downloads"));
   close_html_header(fp_ou);

   fputs("<div class=\"report\"><table cellpadding=\"0\" cellspacing=\"2\">\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("USERID"),_("IP/NAME"),_("DATE/TIME"),_("ACCESSED SITE"));

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the downloaded files\n"));
      exit(EXIT_FAILURE);
   }

   while((buf=longline_read(fp_in,line))!=NULL) {
      getword_start(&gwarea,buf);
      if (getword(data,sizeof(data),&gwarea,'\t')<0 || getword(hora,sizeof(hora),&gwarea,'\t')<0 ||
          getword(user,sizeof(user),&gwarea,'\t')<0 || getword(ip,sizeof(ip),&gwarea,'\t')<0) {
         debuga(_("There is a broken record or garbage in file %s\n"),report_in);
         exit(EXIT_FAILURE);
      }
      if (getword_ptr(buf,&url,&gwarea,'\t')<0) {
         debuga(_("There is a broken url in file %s\n"),report_in);
         exit(EXIT_FAILURE);
      }
      if (sscanf(data,"%d/%d/%d",&day,&month,&year)!=3) continue;
      computedate(year,month,day,&t);
      strftime(data,sizeof(data),"%x",&t);

      uinfo=userinfo_find_from_id(user);
      if (!uinfo) {
         debuga(_("Unknown user ID %s in file %s\n"),user,report_in);
         exit(EXIT_FAILURE);
      }
      new_user=false;
      if(!z) {
         strcpy(ouser,user);
         strcpy(oip,ip);
         z++;
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

      if(DownloadReportLimit) {
         if(strcmp(ouser2,uinfo->label) == 0) {
            count++;
         } else {
            count=1;
            strcpy(ouser2,uinfo->label);
         }
         if(count >= DownloadReportLimit)
            continue;
      }

      for (i=strlen(url)-1 ; i>=0 && (unsigned char)url[i]<' ' ; i--) url[i]=0;

      fputs("<tr>",fp_ou);
      if (new_user)
         fprintf(fp_ou,"<td class=\"data\">%s</td><td class=\"data\">%s</td>",uinfo->label,ip);
      else
         fputs("<td class=\"data\"></td><td class=\"data\"></td>",fp_ou);
      fprintf(fp_ou,"<td class=\"data\">%s-%s</td><td class=\"data2\">",data,hora);
      if(BlockIt[0]!='\0') {
         fprintf(fp_ou,"<a href=\"%s%s?url=\"",wwwDocumentRoot,BlockIt);
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

   fputs("</table></div>\n",fp_ou);
   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in file %s\n"),report);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close file %s - %s\n"),report,strerror(errno));

   unlink(report_in);

   return;
}

void free_download(void)
{
   if (DownloadSuffix) {
      free(DownloadSuffix);
      DownloadSuffix=NULL;
   }
   if (DownloadSuffixIndex) {
      free(DownloadSuffixIndex);
      DownloadSuffixIndex=NULL;
   }
   NDownloadSuffix=0;
}

void set_download_suffix(const char *list)
{
   char *str;
   int i, j, k;
   int cmp;

   free_download();

   DownloadSuffix=strdup(list);
   if (!DownloadSuffix) {
      debuga(_("Download suffix list too long\n"));
      exit(EXIT_FAILURE);
   }
   j = 1;
   for (i=0 ; list[i] ; i++)
      if (list[i] == ',') j++;
   DownloadSuffixIndex=malloc(j*sizeof(char *));
   if (!DownloadSuffixIndex) {
      debuga(_("Too many download suffixes\n"));
      exit(EXIT_FAILURE);
   }

   str = DownloadSuffix;
   for (i=0 ; DownloadSuffix[i] ; i++) {
      if (DownloadSuffix[i] == ',') {
         DownloadSuffix[i] = '\0';
         if (*str) {
            cmp = -1;
            for (j=0 ; j<NDownloadSuffix && (cmp=strcasecmp(str,DownloadSuffixIndex[j]))>0 ; j++);
            if (cmp != 0) {
               for (k=NDownloadSuffix ; k>j ; k--)
                  DownloadSuffixIndex[k]=DownloadSuffixIndex[k-1];
               NDownloadSuffix++;
               DownloadSuffixIndex[j]=str;
            }
         }
         str=DownloadSuffix+i+1;
      }
   }

   if (*str) {
      cmp = -1;
      for (j=0 ; j<NDownloadSuffix && (cmp=strcasecmp(str,DownloadSuffixIndex[j]))>0 ; j++);
      if (cmp != 0) {
         for (k=NDownloadSuffix ; k>j ; k--)
            DownloadSuffixIndex[k]=DownloadSuffixIndex[k-1];
         NDownloadSuffix++;
         DownloadSuffixIndex[j]=str;
      }
   }
}

bool is_download_suffix(const char *url)
{
   int urllen;
   int i;
   int down, up, center;
   const char *suffix;
   int cmp;
   const int max_suffix=10;

   if (DownloadSuffix == NULL || NDownloadSuffix == 0) return(false);

   urllen=strlen(url)-1;
   if (urllen<=0) return(false);
   if (url[urllen] == '.') return(false); //reject a single trailing dot
   for (i=0 ; i<urllen && (url[i]!='/' || url[i+1]=='/') && url[i]!='?' ; i++);
   if (i>=urllen) return(false); // url is a hostname without any path or file to download

   for (i=0 ; i<=max_suffix && i<urllen && url[urllen-i]!='.' ; i++)
      if (url[urllen-i] == '/' || url[urllen-i] == '?') return(false);
   if (i>max_suffix || i>=urllen) return(false);

   suffix=url+urllen-i+1;
   down=0;
   up=NDownloadSuffix-1;
   while (down<=up) {
      center=(down+up)/2;
      cmp=strcasecmp(suffix,DownloadSuffixIndex[center]);
      if (cmp == 0) return(true);
      if (cmp < 0)
         up = center-1;
      else
         down = center+1;
   }
   return(false);
}

