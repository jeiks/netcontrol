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

static void make_date_index(void);
static void make_file_index(void);
static void file_index_to_date_index(const char *entry);
static void date_index_to_file_index(const char *entry);

void make_index(void)
{
   DIR *dirp;
   struct dirent *direntp;
   char wdir[MAXLEN];

   if(LastLog > 0) mklastlog(outdir);

   if(Index == INDEX_NO) {
      sprintf(wdir,"%sindex.html",outdir);
      if(access(wdir, R_OK) == 0) unlink(wdir);
      return;
   }

   if(debug) debuga(_("Making index.html\n"));

   // convert any old report hierarchy
   if ((dirp = opendir(outdir)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),outdir,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ((direntp = readdir( dirp )) != NULL) {
      if(isdigit(direntp->d_name[0]) && isdigit(direntp->d_name[1])) {
         if(IndexTree == INDEX_TREE_DATE)
            file_index_to_date_index(direntp->d_name);
         else
            date_index_to_file_index(direntp->d_name);
      }
   }
   closedir(dirp);

   if(IndexTree == INDEX_TREE_DATE) {
      make_date_index();
   } else {
      make_file_index();
   }
}

static void make_date_index(void)
{
   FILE *fp_ou, *fp_ou2, *fp_ou3;
   DIR *dirp, *dirp2, *dirp3;
   struct dirent *direntp;
   struct dirent *direntp2;
   struct dirent *direntp3;
   char yearindex[MAXLEN];
   char yeardir[MAXLEN];
   char yearnum[10];
   char monthindex[MAXLEN];
   char monthdir[MAXLEN];
   char monthname1[9], monthname2[9];
   char nmonth[30];
   char monthnum[10];
   char dayindex[MAXLEN];
   char daynum[10];
   char title[80];
   int yearsort[150];
   int nyears;
   int year;
   int monthsort[144];
   int nmonths;
   int m1, m2, month;
   int daysort[31*31];
   int ndays;
   int d1, d2, day;
   int i, y, m, d;
   int order;

   sprintf(yearindex,"%sindex.html",outdir);

   nyears=0;
   if ((dirp = opendir(outdir)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),outdir,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ((direntp = readdir( dirp )) != NULL) {
      if(strlen(direntp->d_name) > 4 || !isdigit(direntp->d_name[0]) || !isdigit(direntp->d_name[1]) ||
         !isdigit(direntp->d_name[2]) || !isdigit(direntp->d_name[3])) continue;
      year=atoi(direntp->d_name);
      if (nyears>=sizeof(yearsort)/sizeof(yearsort[0])) {
         /*
         If too many years are listed in the directory, we ignore the earliest years. The yearsort array
         is big enough to accomodate the most ambitious use of sarg but this safety is added to prevent
         a crash should the directory be polluted by other entries.
         */
         if (year>yearsort[0]) {
            for (i=1 ; i<nyears && year>yearsort[i] ; i++)
               yearsort[i-1]=yearsort[i];
            yearsort[i-1]=year;
         }
      } else {
         for (i=nyears ; i>0 &&  year<yearsort[i-1] ; i--) {
            yearsort[i]=yearsort[i-1];
         }
         yearsort[i]=year;
         nyears++;
      }
   }
   closedir( dirp );

   order=(strcmp(IndexSortOrder,"A") == 0) ? 1 : -1;

   if((fp_ou=fopen(yearindex,"w"))==NULL) {
      debuga(_("(index) Cannot open file %s - %s\n"),yearindex,strerror(errno));
      exit(EXIT_FAILURE);
   }
   write_html_header(fp_ou,0,ngettext("SARG report","SARG reports",nyears));
   close_html_header(fp_ou);
   fputs("<div class=\"index\"><table cellpadding=\"1\" cellspacing=\"2\">\n<tr><td></td><td></td></tr>\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("YEAR"),_("SIZE"));
   for (y=0 ; y<nyears ; y++) {
      if (order>0)
         year=yearsort[y];
      else
         year=yearsort[nyears-1-y];
      sprintf(yearnum,"%04d",year);
      fprintf(fp_ou,"<tr><td class=\"data2\"><a href=\"%s/index.html\">%s</a></td><td class=\"data2\">%s</td></tr>\n",yearnum,yearnum,get_size(outdir,yearnum));
      sprintf(yeardir,"%s%s",outdir,yearnum);
      // Year dir
      nmonths=0;
      if ((dirp2 = opendir(yeardir)) == NULL) {
         debuga(_("Failed to open directory %s - %s\n"),yeardir,strerror(errno));
         exit(EXIT_FAILURE);
      }
      while ((direntp2 = readdir( dirp2 )) != NULL) {
         if(!isdigit(direntp2->d_name[0]) || !isdigit(direntp2->d_name[1])) continue;
         i=-1;
         if (sscanf(direntp2->d_name,"%d%n",&m1,&i)!=1 || m1<=0 || m1>12 || i<0) continue;
         if (direntp2->d_name[i]=='-') {
            if (sscanf(direntp2->d_name+i+1,"%d",&m2)!=1 || m2<m1 || m2>12) continue;
         } else if (direntp2->d_name[i]!='\0') {
            continue;
         } else {
            m2=0;
         }
         if (nmonths>=sizeof(monthsort)/sizeof(monthsort[0])) {
            debuga(_("Too many month directories in %s\nSupernumerary entries are ignored\n"),yeardir);
            break;
         }
         month=m1*16+m2;
         for (i=nmonths ; i>0 &&  month<monthsort[i-1] ; i--) {
            monthsort[i]=monthsort[i-1];
         }
         monthsort[i]=month;
         nmonths++;
      }
      closedir(dirp2);
      sprintf(monthindex,"%s/index.html",yeardir);
      if((fp_ou2=fopen(monthindex,"w"))==NULL) {
         debuga(_("(index) Cannot open file %s - %s\n"),monthindex,strerror(errno));
         exit(EXIT_FAILURE);
      }
      snprintf(title,sizeof(title),ngettext("SARG: report for %04d","SARG: reports for %04d",nmonths),year);
      write_html_header(fp_ou2,1,title);
      close_html_header(fp_ou2);
      fputs("<div class=\"index\"><table cellpadding=\"1\" cellspacing=\"2\">\n<tr><td></td><td></td></tr>\n",fp_ou2);
      fprintf(fp_ou2,"<tr><th class=\"header_l\">%s/%s</th></tr>\n",_("YEAR"),_("MONTH"));
      for (m=0 ; m<nmonths ; m++) {
         if (order>0)
            month=monthsort[m];
         else
            month=monthsort[nmonths-1-m];
         m1=month / 16;
         if(month % 16 != 0) {
            m2=month % 16;
            sprintf(monthnum,"%02d-%02d",m1,m2);
            sprintf(monthname1,"%02d",m1);
            sprintf(monthname2,"%02d",m2);
            name_month(monthname1,sizeof(monthname1));
            name_month(monthname2,sizeof(monthname2));
            sprintf(nmonth,"%s-%s",monthname1,monthname2);
         } else {
            sprintf(nmonth,"%02d",m1);
            sprintf(monthnum,"%02d",m1);
            name_month(nmonth,sizeof(nmonth));
         }
         fprintf(fp_ou2,"<tr><td class=\"data2\"><a href=\"%s/index.html\">%s %s</a></td></tr>\n",monthnum,yearnum,nmonth);

         sprintf(monthdir,"%s/%s",yeardir,monthnum);
         // month dir
         ndays=0;
         if ((dirp3 = opendir(monthdir)) == NULL) {
            debuga(_("Failed to open directory %s - %s\n"),monthdir,strerror(errno));
            exit(EXIT_FAILURE);
         }
         while ((direntp3 = readdir( dirp3 )) != NULL) {
            if(!isdigit(direntp3->d_name[0]) && !isdigit(direntp3->d_name[1])) continue;
            i=-1;
            if (sscanf(direntp3->d_name,"%d%n",&d1,&i)!=1 || d1<=0 || d1>31 || i<0) continue;
            if (direntp3->d_name[i]=='-') {
               if (sscanf(direntp3->d_name+i+1,"%d",&d2)!=1 || d2<d1 || d2>31) continue;
            } else if (direntp3->d_name[i]!='\0') {
               continue;
            } else {
               d2=0;
            }
            if (ndays>=sizeof(daysort)/sizeof(daysort[0])) {
               debuga(_("Too many day directories in %s\nSupernumerary entries are ignored\n"),monthdir);
               break;
            }
            day=d1*32+d2;
            for (i=ndays ; i>0 &&  day<daysort[i-1] ; i--) {
               daysort[i]=daysort[i-1];
            }
            daysort[i]=day;
            ndays++;
         }
         closedir(dirp3);
         sprintf(dayindex,"%s/index.html",monthdir);
         if((fp_ou3=fopen(dayindex,"w"))==NULL) {
            debuga(_("(index) Cannot open file %s - %s\n"),dayindex,strerror(errno));
            exit(EXIT_FAILURE);
         }
         snprintf(title,sizeof(title),ngettext("SARG: report for %04d/%02d","SARG: reports for %04d/%02d",ndays),year,month);
         write_html_header(fp_ou3,2,title);
         close_html_header(fp_ou3);
         fputs("<div class=\"index\"><table cellpadding=\"1\" cellspacing=\"2\">\n<tr><td></td><td></td></tr>\n",fp_ou3);
         fprintf(fp_ou3,"<tr><th class=\"header_l\">%s/%s/%s</th></tr>\n",_("YEAR"),_("MONTH"),_("DAYS"));
         for (d=0 ; d<ndays ; d++) {
            if (order>0)
               day=daysort[d];
            else
               day=daysort[ndays-1-d];
            d1=day / 32;
            if(day % 32 != 0) {
               d2=day % 32;
               sprintf(daynum,"%02d-%02d",d1,d2);
            } else {
               sprintf(daynum,"%02d",d1);
            }
            fprintf(fp_ou3,"<tr><td class=\"data2\"><a href=\"%s/index.html\">%s %s %s</a></td></tr>\n",daynum,yearnum,nmonth,daynum);
         }
         fputs("</table></div>\n",fp_ou3);
         if (write_html_trailer(fp_ou3)<0)
            debuga(_("Write error in the index %s\n"),dayindex);
         if (fclose(fp_ou3)==EOF)
            debuga(_("Failed to close the index file %s - %s\n"),dayindex,strerror(errno));
      }
      fputs("</table></div>\n",fp_ou2);
      if (write_html_trailer(fp_ou2)<0)
         debuga(_("Write error in the index %s\n"),monthindex);
      if (fclose(fp_ou2)==EOF)
         debuga(_("Failed to close the index file %s - %s\n"),monthindex,strerror(errno));
   }

   fputs("</table></div>\n",fp_ou);
   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in the index %s\n"),yearindex);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close the index file %s - %s\n"),yearindex,strerror(errno));
}

static void make_file_index(void)
{
   #define MAX_CREATION_DATE 15
   FILE *fp_ou;
   DIR *dirp;
   struct dirent *direntp;
   char wdir[MAXLEN];
   char data[80];
   char tbytes[20];
   char media[20];
   char ftime[128];
   char day[6], mon[8], year[40], hour[10];
   int iyear, imonth, iday, ihour, iminute, isecond, idst;
   int nsort;
   int nallocated;
   int order;
   int i;
   int tuser;
   struct getwordstruct gwarea;
   struct sortstruct
   {
      int year, month, day, sortnum;
      char creationdate[MAX_CREATION_DATE];
      char *dirname;
      char date[60];
   } **sortlist, *item, **tempsort;

   sprintf(wdir,"%sindex.html",outdir);

   order=(strcmp(IndexSortOrder,"A") == 0) ? 1 : -1;

   if ((dirp = opendir(outdir)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),outdir,strerror(errno));
      exit(EXIT_FAILURE);
   }

   nsort=0;
   nallocated=0;
   sortlist=NULL;
   while ((direntp = readdir( dirp )) != NULL) {
      if (strchr(direntp->d_name,'-') == 0) continue;
      item=malloc(sizeof(*item));
      if (!item) {
         debuga(_("not enough memory to sort the index\n"));
         exit(EXIT_FAILURE);
      }
      if(strcmp(df,"u") == 0) {
         item->year=atoi(direntp->d_name);
         item->month=conv_month(direntp->d_name+4);
         item->day=atoi(direntp->d_name+7);
      } else {
         item->year=atoi(direntp->d_name+5);
         item->month=conv_month(direntp->d_name+2);
         item->day=atoi(direntp->d_name);
      }
      item->sortnum=(item->year*16+item->month)*32+item->day;
      obtdate(outdir,direntp->d_name,data);
      if (sscanf(data,"%d-%d-%d %d:%d:%d %d",&iyear,&imonth,&iday,&ihour,&iminute,&isecond,&idst)==7) {
         formatdate(data,sizeof(data),iyear,imonth,iday,ihour,iminute,isecond,idst);
         snprintf(item->creationdate,sizeof(item->creationdate),"%04d%02d%02d%02d%02d%02d",iyear,imonth,iday,ihour,iminute,isecond);
      } else {
         /*
         Old code to parse a date stored by sarg before 2.2.6.1 in the sarg-date file of each report directory.
         */
         getword_start(&gwarea,data);
         if (getword_skip(16,&gwarea,' ')<0) {
            debuga(_("Maybe you have a broken week day in your %s%s/sarg-date file\n"),outdir,direntp->d_name);
            exit(EXIT_FAILURE);
         }
         if (getword_multisep(mon,sizeof(mon),&gwarea,' ')<0) {
            debuga(_("Maybe you have a broken month in your %s%s/sarg-date file\n"),outdir,direntp->d_name);
            exit(EXIT_FAILURE);
         }
         if (getword_multisep(day,sizeof(day),&gwarea,' ')<0) {
            debuga(_("Maybe you have a broken day in your %s%s/sarg-date file\n"),outdir,direntp->d_name);
            exit(EXIT_FAILURE);
         }
         if (getword_multisep(hour,sizeof(hour),&gwarea,' ')<0) {
            debuga(_("Maybe you have a broken time in your %s%s/sarg-date file\n"),outdir,direntp->d_name);
            exit(EXIT_FAILURE);
         }
         do {
            if (getword_multisep(year,sizeof(year),&gwarea,' ')<0) {
               debuga(_("Maybe you have a broken year in your %s%s/sarg-date file\n"),outdir,direntp->d_name);
               exit(EXIT_FAILURE);
            }
         } while (year[0] && !isdigit(year[0])); //skip time zone information with spaces until the year is found
         if (sscanf(hour,"%d:%d:%d",&ihour,&iminute,&isecond)!=3) {
            debuga(_("Maybe you have a broken time in your %s%s/sarg-date file\n"),outdir,direntp->d_name);
            exit(EXIT_FAILURE);
         }
         buildymd(day,mon,year,ftime);
         snprintf(item->creationdate,sizeof(item->creationdate),"%s%02d%02d%02d",ftime, ihour, iminute, isecond);
      }
      item->dirname=strdup(direntp->d_name);
      if (!item->dirname) {
         debuga(_("Not enough memory to store the directory name \"%s\" in the index\n"),direntp->d_name);
         exit(EXIT_FAILURE);
      }
      strncpy(item->date,data,sizeof(item->date));
      if (nsort+1>nallocated) {
         nallocated+=10;
         tempsort=realloc(sortlist,nallocated*sizeof(*item));
         if (!tempsort) {
            debuga(_("not enough memory to sort the index\n"));
            exit(EXIT_FAILURE);
         }
         sortlist=tempsort;
      }
      for (i=nsort ; i>0 ; i--) {
         if (item->sortnum>sortlist[i-1]->sortnum) break;
         if (item->sortnum==sortlist[i-1]->sortnum) {
            if (strcmp(item->creationdate,sortlist[i-1]->creationdate)>=0) break;
         }
         sortlist[i]=sortlist[i-1];
      }
      sortlist[i]=item;
      nsort++;
   }

   closedir( dirp );

   if((fp_ou=fopen(wdir,"w"))==NULL) {
      debuga(_("(index) Cannot open file %s\n"),wdir);
      exit(EXIT_FAILURE);
   }
   write_html_header(fp_ou,0,ngettext("SARG report","SARG reports",nsort));
   close_html_header(fp_ou);
   fputs("<div class=\"index\"><table cellpadding=\"1\" cellspacing=\"2\">\n<tr><td></td><td></td></tr>\n",fp_ou);
   fprintf(fp_ou,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("FILE/PERIOD"),_("CREATION DATE"),_("USERS"),_("BYTES"),_("AVERAGE"));
   for (i=0 ; i<nsort ; i++) {
      if (order>0)
         item=sortlist[i];
      else
         item=sortlist[nsort-i-1];
      tuser=obtuser(outdir,item->dirname);
      obttotal(outdir,item->dirname,tbytes,tuser,media);
      fprintf(fp_ou,"<tr><td class=\"data2\"><a href='%s/%s'>%s</a></td><td class=\"data2\">%s</td><td class=\"data\">%d</td><td class=\"data\">%s</td><td class=\"data\">%s</td></tr>\n",item->dirname,ReplaceIndex,item->dirname,item->date,tuser,tbytes,media);
   }
   fputs("</table></div>\n",fp_ou);
   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in the index %s\n"),wdir);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close the index file %s - %s\n"),wdir,strerror(errno));

   if (sortlist) {
      for (i=0 ; i<nsort ; i++) {
         free(sortlist[i]->dirname);
         free(sortlist[i]);
      }
      free(sortlist);
   }
}

static void file_index_to_date_index(const char *entry)
{
   int y1, y2, m1, m2, d1, d2;
   int i, j;
   int ndirlen;
   int monthlen;
   char sm1[8], sm2[8];
   char olddir[MAXLEN], newdir[MAXLEN];

   if(strlen(entry) < 19) return;

   y1=0;
   y2=0;
   memset(sm1,0,sizeof(sm1));
   memset(sm2,0,sizeof(sm2));
   d1=0;
   d2=0;
   i=0;
   if(strcmp(df,"u") == 0) {
      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         y1=y1*10+(entry[i++]-'0');
      if (j!=4) return;
      for (j=0 ; j<sizeof(sm1)-1 && entry[i] && isalpha(entry[i]) ; j++)
         sm1[j]=entry[i++];
      if (j!=3) return;
      sm1[j]='\0';
      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         d1=d1*10+(entry[i++]-'0');
      if (j!=2) return;

      if (entry[i++]!='-') return;

      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         y2=y2*10+(entry[i++]-'0');
      if (j!=4) return;
      for (j=0 ; j<sizeof(sm2)-1 && entry[i] && isalpha(entry[i]) ; j++)
         sm2[j]=entry[i++];
      if (j!=3) return;
      sm2[j]='\0';
      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         d2=d2*10+(entry[i++]-'0');
      if (j!=2) return;
   } else if(strcmp(df,"e") == 0) {
      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         d1=d1*10+(entry[i++]-'0');
      if (j!=2) return;
      for (j=0 ; j<sizeof(sm1)-1 && entry[i] && isalpha(entry[i]) ; j++)
         sm1[j]=entry[i++];
      if (j!=3) return;
      sm1[j]='\0';
      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         y1=y1*10+(entry[i++]-'0');
      if (j!=4) return;

      if (entry[i++]!='-') return;

      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         d2=d2*10+(entry[i++]-'0');
      if (j!=2) return;
      for (j=0 ; j<sizeof(sm2)-1 && entry[i] && isalpha(entry[i]) ; j++)
         sm2[j]=entry[i++];
      if (j!=3) return;
      sm2[j]='\0';
      for (j=0 ; entry[i] && isdigit(entry[i]) ; j++)
         y2=y2*10+(entry[i++]-'0');
      if (j!=4) return;
   } else
      return;

   m1=conv_month(sm1);
   m2=conv_month(sm2);
   ndirlen=sprintf(newdir,"%s%04d",outdir,y1);
   if(access(newdir, R_OK) != 0) mkdir(newdir,0755);
   if(m1 != m2) ndirlen+=sprintf(newdir+ndirlen,"/%02d-%02d",m1,m2);
   else ndirlen+=sprintf(newdir+ndirlen,"/%02d",m1);
   if(access(newdir, R_OK) != 0) mkdir(newdir,0755);
   monthlen=ndirlen;
   if(d1!=d2) ndirlen+=sprintf(newdir+ndirlen,"/%02d-%02d",d1,d2);
   else ndirlen+=sprintf(newdir+ndirlen,"/%02d",d1);

   sprintf(olddir,"%s%s",outdir,entry);
   if (rename(olddir,newdir)) {
      debuga(_("(index) rename error from \"%s\" to \"%s\" - %s\n"),olddir,newdir,strerror(errno));
      exit(EXIT_FAILURE);
   }

   strcpy(newdir+monthlen,"/images");
   if(access(newdir, R_OK) != 0) {
#ifdef HAVE_SYMLINK
      char linkdir[MAXLEN];

      sprintf(linkdir,"%simages",outdir);
      if (symlink(linkdir,newdir)) {
         debuga(_("failed to create link \"%s\" to \"%s\" - %s\n"),linkdir,newdir,strerror(errno));
         exit(EXIT_FAILURE);
      }
#else
      char cmd[MAXLEN];
      int cstatus;

      sprintf(cmd,"ln -s \"%simages\" \"%s/images\"",outdir,newdir);
      cstatus=system(cmd);
      if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
         debuga(_("command return status %d\n"),WEXITSTATUS(cstatus));
         debuga(_("command: %s\n"),cmd);
         exit(EXIT_FAILURE);
      }
#endif
   }
}

static void date_index_to_file_index(const char *entry)
{
   int y1, next;
   int m1, m2;
   int d1, d2;
   int val1len;
   int i, j;
   char val1[MAXLEN];
   const char *sm1, *sm2;
   char *str;
   char newdir[MAXLEN], olddir[MAXLEN];
   DIR *dirp2, *dirp3;
   struct dirent *direntp2;
   struct dirent *direntp3;

   if(strlen(entry) != 4) return;

   next=-1;
   if (sscanf(entry,"%d%n",&y1,&next)!=1 || next<0 || entry[next]) return;

   val1len=snprintf(val1,sizeof(val1),"%s%s",outdir,entry);
   dirp2 = opendir(val1);
   if (!dirp2) return;
   while ((direntp2 = readdir( dirp2 )) != NULL) {
      if(!isdigit(direntp2->d_name[0]) || !isdigit(direntp2->d_name[1])) continue;
      i=0;
      str=direntp2->d_name;
      m1=0;
      for (j=0 ; j<2 && str[i] && isdigit(str[i]) ; j++)
         m1=(m1*10)+(str[i++]-'0');
      if (j>=2) continue;
      sm1=conv_month_name(m1);
      if (str[i]=='-') {
         i++;
         m2=0;
         for (j=0 ; j<2 && str[i] && isdigit(str[i]) ; j++)
            m2=(m2*10)+(str[i++]-'0');
         if (j>=2) continue;
         sm2=conv_month_name(m2);
      } else if (!str[i]) {
         sm2=sm1;
      } else {
         continue;
      }

      sprintf(val1+val1len,"/%s",direntp2->d_name);
      dirp3 = opendir(val1);
      if (!dirp3) continue;
      while ((direntp3 = readdir( dirp3 )) != NULL) {
         if(!isdigit(direntp3->d_name[0]) || !isdigit(direntp3->d_name[1])) continue;
         i=0;
         str=direntp3->d_name;
         d1=0;
         for (j=0 ; str[i] && isdigit(str[i]) ; j++)
            d1=d1*10+(str[i++]-'0');
         if (j!=2) continue;
         if (str[i]=='-') {
            i++;
            d2=0;
            for (j=0 ; str[i] && isdigit(str[i]) ; j++)
               d2=d2*10+(str[i++]-'0');
            if (j!=2) continue;
         } else if (!str[i]) {
            d2=d1;
         } else {
            continue;
         }

         if(strcmp(df,"u") == 0) sprintf(newdir,"%s%04d%s%02d-%04d%s%02d",outdir,y1,sm1,d1,y1,sm2,d2);
         else if(strcmp(df,"e") == 0) sprintf(newdir,"%s%02d%s%04d-%02d%s%04d",outdir,d1,sm1,y1,d2,sm2,y1);
         else continue;
         sprintf(olddir,"%s%04d/%s/%s",outdir,y1,direntp2->d_name,direntp3->d_name);
         if(rename(olddir,newdir)) {
            debuga(_("(index) rename error from \"%s\" to \"%s\" - %s\n"),olddir,newdir,strerror(errno));
            exit(EXIT_FAILURE);
         }
      }
      closedir( dirp3 );
   }
   closedir( dirp2 );

   /*!
   \bug The links to the images in the reports are broken after moving the directories
   as the the HTML files are not at the right level for the images any more.
   */
}

