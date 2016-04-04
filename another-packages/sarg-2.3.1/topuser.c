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

void topuser(void)
{

   FILE *fp_in = NULL, *fp_ou = NULL, *fp_top1 = NULL, *fp_top2 = NULL, *fp_top3 = NULL;
   long long int ttnbytes=0, ttnacc=0, tnacc=0;
   long long int tnbytes=0, ttnelap=0, tnelap=0;
   long long int tnincache=0, tnoucache=0, ttnincache=0, ttnoucache=0;
   long long int nbytes;
   long long int nacc;
   long long int elap, incac, oucac;
   double perc=0.00;
   double perc2=0.00;
   double inperc=0.00, ouperc=0.00;
   int posicao=0;
   char olduser[MAX_USER_LEN], csort[MAXLEN];
   char wger[MAXLEN], top1[MAXLEN], top2[MAXLEN], top3[MAXLEN];
   char user[MAX_USER_LEN], tusr[MAXLEN];
   char ipantes[MAXLEN], nameantes[MAXLEN];
   const char *sfield="-n -k 2,2";
   const char *order;
   const char *sort_field;
   const char *sort_order;
   char title[80];
   char *warea;
   int  totuser=0;
   int  topcount=0;
   int cstatus;
   struct getwordstruct gwarea;
   longline line;
   struct generalitemstruct item;
   const struct userinfostruct *uinfo;

   ipantes[0]='\0';
   nameantes[0]='\0';

   ntopuser = 0;
   snprintf(wger,sizeof(wger),"%s/sarg-general",outdirname);
   if((fp_in=fopen(wger,"r"))==NULL) {
      debuga(_("(topuser) Cannot open file %s\n"),wger);
      exit(EXIT_FAILURE);
   }

   snprintf(top2,sizeof(top2),"%s/top.tmp",outdirname);
   if((fp_top2=fopen(top2,"w"))==NULL) {
      debuga(_("(topuser) Cannot open file %s\n"),top2);
      exit(EXIT_FAILURE);
   }

   olduser[0]='\0';
   totuser=0;

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the file %s\n"),wger);
      exit(EXIT_FAILURE);
   }

   while((warea=longline_read(fp_in,line))!=NULL) {
      ger_read(warea,&item,wger);
      if(item.total) continue;
      if(strcmp(olduser,item.user) != 0) {
         totuser++;

         if (olduser[0] != '\0') {
            /*
            This complicated printf is due to Microsoft's inability to comply with any standard. Msvcrt is unable
            to print a long long int unless it is exactly 64-bits long.
            */
            fprintf(fp_top2,"%s\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\n",olduser,(uint64_t)tnbytes,(uint64_t)tnacc,(uint64_t)tnelap,(uint64_t)tnincache,(uint64_t)tnoucache);

            ttnbytes+=tnbytes;
            ttnacc+=tnacc;
            ttnelap+=tnelap;
            ttnincache+=tnincache;
            ttnoucache+=tnoucache;
         }
         strcpy(olduser,item.user);
         tnbytes=0;
         tnacc=0;
         tnelap=0;
         tnincache=0;
         tnoucache=0;
      }

      tnbytes+=item.nbytes;
      tnacc+=item.nacc;
      tnelap+=item.nelap;
      tnincache+=item.incache;
      tnoucache+=item.oucache;
   }
   fclose(fp_in);
   longline_destroy(&line);

   if (olduser[0] != '\0') {
      /*
      This complicated printf is due to Microsoft's inability to comply with any standard. Msvcrt is unable
      to print a long long int unless it is exactly 64-bits long.
      */
      fprintf(fp_top2,"%s\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\t%"PRIu64"\n",olduser,(uint64_t)tnbytes,(uint64_t)tnacc,(uint64_t)tnelap,(uint64_t)tnincache,(uint64_t)tnoucache);

      ttnbytes+=tnbytes;
      ttnacc+=tnacc;
      ttnelap+=tnelap;
      ttnincache+=tnincache;
      ttnoucache+=tnoucache;
   }
   fclose(fp_top2);

   if((TopuserSort & TOPUSER_SORT_USER) != 0) {
      sfield="-k 1,1";
      sort_field=_("user");
   } else if((TopuserSort & TOPUSER_SORT_CONNECT) != 0) {
      sfield="-n -k 3,3";
      sort_field=_("connect");
   } else if((TopuserSort & TOPUSER_SORT_TIME) != 0) {
      sfield="-n -k 4,4";
      sort_field=_("time");
   } else {
      sort_field=_("bytes");
   }

   if((TopuserSort & TOPUSER_SORT_REVERSE) == 0) {
      order="";
      sort_order=_("normal");
   } else {
      order="-r";
      sort_order=_("reverse");
   }

   snprintf(top1,sizeof(top1),"%s/top",outdirname);
   sprintf(csort,"sort -T \"%s\" %s %s -o \"%s\" \"%s\"", tmp, order, sfield, top1, top2);
   cstatus=system(csort);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }

   if((fp_top1=fopen(top1,"r"))==NULL) {
      debuga(_("(topuser) Cannot open file %s\n"),top1);
      exit(EXIT_FAILURE);
   }

   unlink(top2);

   snprintf(top3,sizeof(top3),"%s/index.html",outdirname);
   if((fp_top3=fopen(top3,"w"))==NULL) {
      debuga(_("(topuser) Cannot open file %s\n"),top3);
      exit(EXIT_FAILURE);
   }

   snprintf(title,sizeof(title),_("SARG report for %s"),period.text);
   write_html_header(fp_top3,(IndexTree == INDEX_TREE_DATE) ? 3 : 1,title);
   fputs("<tr><td class=\"header_c\">",fp_top3);
   fprintf(fp_top3,_("Period: %s"),period.html);
   fputs("</td></tr>\n",fp_top3);
   fputs("<tr><td class=\"header_c\">",fp_top3);
   fprintf(fp_top3,_("Sort: %s, %s"),sort_field,sort_order);
   fputs("</td></tr>\n",fp_top3);
   fprintf(fp_top3,"<tr><th class=\"header_c\">%s</th></tr>\n",_("Top users"));
   close_html_header(fp_top3);

   fputs("<div class=\"report\"><table cellpadding=\"1\" cellspacing=\"2\">\n",fp_top3);

   if((ReportType & REPORT_TYPE_TOPSITES) != 0 && !Privacy) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"topsites.html\">%s</a></td></tr>\n",_("Top sites"));
   if((ReportType & REPORT_TYPE_SITES_USERS) != 0 && !Privacy) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"siteuser.html\">%s</a></td></tr>\n",_("Sites & Users"));
   if(dansguardian_count) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"dansguardian.html\">%s</a></td></tr>\n",_("DansGuardian"));
   if(squidguard_count) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"redirector.html\">%s</a></td></tr>\n",_("Redirector"));
   if ((ReportType & REPORT_TYPE_DOWNLOADS) != 0 && download_count && !Privacy && ndownload) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"download.html\">%s</a></td></tr>\n",_("Downloads"));
   if ((ReportType & REPORT_TYPE_DENIED) != 0 && denied_count && !Privacy) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"denied.html\">%s</a></td></tr>\n",_("Denied accesses"));
   if ((ReportType & REPORT_TYPE_AUTH_FAILURES) != 0 && authfail_count && !Privacy) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"authfail.html\">%s</a></td></tr>\n",_("Authentication Failures"));
   if(smartfilter) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"smartfilter.html\">%s</a></td></tr>\n",_("SmartFilter"));
   if(UserAgentLog[0] != '\0' && useragent_count) fprintf(fp_top3,"<tr><td class=\"link\" colspan=\"0\"><a href=\"useragent.html\">%s</a></td></tr>\n",_("Useragent"));
   fputs("<tr><td></td></tr>\n</table></div>\n",fp_top3);

   if ((ReportType & REPORT_TYPE_TOPUSERS) == 0) {
      fputs("</body>\n</html>\n",fp_top3);
      fclose (fp_top3);
      return;
   }

   fputs("<div class=\"report\"><table cellpadding=\"1\" cellspacing=\"2\">\n",fp_top3);
   fputs("<tr>",fp_top3);

   if((TopUserFields & TOPUSERFIELDS_NUM) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("NUM"));
   if((TopUserFields & TOPUSERFIELDS_DATE_TIME) !=0 && (ReportType & REPORT_TYPE_DATE_TIME) != 0)
      fputs("<th class=\"header_l\"></th>",fp_top3);
   if((TopUserFields & TOPUSERFIELDS_USERID) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("USERID"));
   if((TopUserFields & TOPUSERFIELDS_CONNECT) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("CONNECT"));
   if((TopUserFields & TOPUSERFIELDS_BYTES) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("BYTES"));
   if((TopUserFields & TOPUSERFIELDS_SETYB) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%%%s</th>",_("BYTES"));
   if((TopUserFields & TOPUSERFIELDS_IN_CACHE_OUT) != 0)
      fprintf(fp_top3,"<th class=\"header_c\" colspan=\"2\">%s</th>",_("IN-CACHE-OUT"));
   if((TopUserFields & TOPUSERFIELDS_USED_TIME) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("ELAPSED TIME"));
   if((TopUserFields & TOPUSERFIELDS_MILISEC) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("MILLISEC"));
   if((TopUserFields & TOPUSERFIELDS_PTIME) != 0)
      fprintf(fp_top3,"<th class=\"header_l\">%%%s</th>",_("TIME"));

   fputs("</tr>\n",fp_top3);

   ntopuser = 0;

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read the downloaded files\n"));
      exit(EXIT_FAILURE);
   }

   while((warea=longline_read(fp_top1,line))!=NULL) {
      getword_start(&gwarea,warea);
      if (getword(user,sizeof(user),&gwarea,'\t')<0) {
         debuga(_("There is a broken user in file %s\n"),top1);
         exit(EXIT_FAILURE);
      }
      if (getword_atoll(&nbytes,&gwarea,'\t')<0) {
         debuga(_("There is a broken number of bytes in file %s\n"),top1);
         exit(EXIT_FAILURE);
      }
      if (getword_atoll(&nacc,&gwarea,'\t')<0) {
         debuga(_("There is a broken number of access in file %s\n"),top1);
         exit(EXIT_FAILURE);
      }
      if (getword_atoll(&elap,&gwarea,'\t')<0) {
         debuga(_("There is a broken elpased time in file %s\n"),top1);
         exit(EXIT_FAILURE);
      }
      if (getword_atoll(&incac,&gwarea,'\t')<0) {
         debuga(_("There is a broken in-cache size in file %s\n"),top1);
         exit(EXIT_FAILURE);
      }
      if (getword_atoll(&oucac,&gwarea,'\n')<0) {
         debuga(_("There is a broken out-of-cache size in file %s\n"),top1);
         exit(EXIT_FAILURE);
      }
      if(nacc < 1)
         continue;
      ntopuser = 1;
      if(TopUsersNum > 0 && topcount >= TopUsersNum) break;
      tnbytes=nbytes;
      tnelap=elap;

      uinfo=userinfo_find_from_id(user);
      if (!uinfo) {
         debuga(_("Unknown user ID %s in file %s\n"),user,top1);
         exit(EXIT_FAILURE);
      }

      fputs("<tr>",fp_top3);

      posicao++;
      if((TopUserFields & TOPUSERFIELDS_NUM) != 0)
         fprintf(fp_top3,"<td class=\"data\">%d</td>",posicao);

      if((TopUserFields & TOPUSERFIELDS_DATE_TIME) !=0 && (ReportType & REPORT_TYPE_DATE_TIME) != 0) {
         fputs("<td class=\"data2\">",fp_top3);
#ifdef HAVE_GD
         if(Graphs && GraphFont[0]!='\0') {
            //fprintf(fp_top3,"<a href=\"%s/graph_day.png\"><img src=\"%s/graph.png\" title=\"%s\" alt=\"G\"></a>&nbsp;",uinfo->filename,ImageFile,_("Graphic"));
            fprintf(fp_top3,"<a href=\"%s/graph.html\"><img src=\"%s/graph.png\" title=\"%s\" alt=\"G\"></a>&nbsp;",uinfo->filename,ImageFile,_("Graphic"));
         }
#endif
         fprintf(fp_top3,"<a href=\"%s/d%s.html\"><img src=\"%s/datetime.png\" title=\"%s\" alt=\"T\"></a></td>",uinfo->filename,uinfo->filename,ImageFile,_("date/time report"));
      } else {
         sprintf(val1,"%s/d%s.html",outdirname,uinfo->filename);
         unlink(val1);
      }
      if((TopUserFields & TOPUSERFIELDS_USERID) != 0) {
         if((ReportType & REPORT_TYPE_USERS_SITES) == 0)
            fprintf(fp_top3,"<td class=\"data2\">%s</td>",name);
         else
            fprintf(fp_top3,"<td class=\"data2\"><a href=\"%s/%s.html\">%s</a></td>",uinfo->filename,uinfo->filename,uinfo->label);
      }
      if((TopUserFields & TOPUSERFIELDS_CONNECT) != 0)
         fprintf(fp_top3,"<td class=\"data\">%s</td>",fixnum(nacc,1));
      if((TopUserFields & TOPUSERFIELDS_BYTES) != 0)
         fprintf(fp_top3,"<td class=\"data\">%s</td>",fixnum(tnbytes,1));
      if((TopUserFields & TOPUSERFIELDS_SETYB) != 0) {
         perc=(ttnbytes) ? tnbytes * 100. / ttnbytes : 0.;
         fprintf(fp_top3,"<td class=\"data\">%3.2lf%%</td>",perc);
      }
      if((TopUserFields & TOPUSERFIELDS_IN_CACHE_OUT) != 0) {
         inperc=(tnbytes) ? incac * 100. / tnbytes : 0.;
         ouperc=(tnbytes) ? oucac * 100. / tnbytes : 0.;
         fprintf(fp_top3,"<td class=\"data\">%3.2lf%%</td><td class=\"data\">%3.2lf%%</td>",inperc,ouperc);
#ifdef ENABLE_DOUBLE_CHECK_DATA
         if (fabs(inperc+ouperc-100.)>=0.01) {
            debuga(_("The total of the in-cache and cache-miss is not 100%% at position %d (user %s)\n"),posicao,uinfo->label);
         }
#endif
      }
      if((TopUserFields & TOPUSERFIELDS_USED_TIME) != 0)
         fprintf(fp_top3,"<td class=\"data\">%s</td>",buildtime(tnelap));
      if((TopUserFields & TOPUSERFIELDS_MILISEC) != 0)
         fprintf(fp_top3,"<td class=\"data\">%s</td>",fixnum2(tnelap,1));
      if((TopUserFields & TOPUSERFIELDS_PTIME) != 0) {
         perc2=(ttnelap) ? elap * 100. / ttnelap : 0.;
         fprintf(fp_top3,"<td class=\"data\">%3.2lf%%</td>",perc2);
      }

      fputs("</tr>\n",fp_top3);

      topcount++;
   }
   fclose(fp_top1);
   unlink(top1);
   longline_destroy(&line);

   if((TopUserFields & TOPUSERFIELDS_TOTAL) != 0) {
      fputs("<tr>",fp_top3);
      if((TopUserFields & TOPUSERFIELDS_NUM) != 0)
         fputs("<td></td>",fp_top3);
      if((TopUserFields & TOPUSERFIELDS_DATE_TIME) !=0 && (ReportType & REPORT_TYPE_DATE_TIME) != 0)
         fputs("<td></td>",fp_top3);
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("TOTAL"));

      if((TopUserFields & TOPUSERFIELDS_CONNECT) != 0)
         fprintf(fp_top3,"<th class=\"header_r\">%s</th>",fixnum(ttnacc,1));
      if((TopUserFields & TOPUSERFIELDS_BYTES) != 0)
         fprintf(fp_top3,"<th class=\"header_r\">%15s</th>",fixnum(ttnbytes,1));
      if((TopUserFields & TOPUSERFIELDS_SETYB) != 0)
         fputs("<td></td>",fp_top3);
      if((TopUserFields & TOPUSERFIELDS_IN_CACHE_OUT) != 0)
      {
         inperc=(ttnbytes) ? ttnincache * 100. / ttnbytes : 0.;
         ouperc=(ttnbytes) ? ttnoucache *100. / ttnbytes : 0.;
         fprintf(fp_top3,"<th class=\"header_r\">%3.2lf%%</th><th class=\"header_r\">%3.2lf%%</th>",inperc,ouperc);
#ifdef ENABLE_DOUBLE_CHECK_DATA
         if (fabs(inperc+ouperc-100.)>=0.01) {
            debuga(_("The total of the in-cache and cache-miss is not 100%%\n"));
         }
#endif
      }
      if((TopUserFields & TOPUSERFIELDS_USED_TIME) != 0)
         fprintf(fp_top3,"<th class=\"header_r\">%s</th>",buildtime(ttnelap));
      if((TopUserFields & TOPUSERFIELDS_MILISEC) != 0)
         fprintf(fp_top3,"<th class=\"header_r\">%s</th>",fixnum2(ttnelap,1));

      fputs("</tr>\n",fp_top3);
   }

   if(ntopuser && (TopUserFields & TOPUSERFIELDS_AVERAGE) != 0) {
      fputs("<tr>",fp_top3);
      if((TopUserFields & TOPUSERFIELDS_NUM) != 0)
         fputs("<td></td>",fp_top3);
      if((TopUserFields & TOPUSERFIELDS_DATE_TIME) !=0 && (ReportType & REPORT_TYPE_DATE_TIME) != 0)
         fputs("<td></td>",fp_top3);
      fprintf(fp_top3,"<th class=\"header_l\">%s</th>",_("AVERAGE"));

      if((TopUserFields & TOPUSERFIELDS_CONNECT) != 0)
         fprintf(fp_top3,"<th class=\"header_r\">%s</th>",fixnum(ttnacc/totuser,1));
      if((TopUserFields & TOPUSERFIELDS_BYTES) != 0) {
         tnbytes=(totuser) ? ttnbytes / totuser : 0;
         fprintf(fp_top3,"<th class=\"header_r\">%15s</th>",fixnum(tnbytes,1));
      }
      if((TopUserFields & TOPUSERFIELDS_SETYB) != 0)
         fputs("<td></td>",fp_top3);
      if((TopUserFields & TOPUSERFIELDS_IN_CACHE_OUT) != 0)
         fputs("<td></td><td></td>",fp_top3);
      if((TopUserFields & TOPUSERFIELDS_USED_TIME) != 0)
         fprintf(fp_top3,"<th class=\"header_r\">%s</th>",buildtime(ttnelap/totuser));
      if((TopUserFields & TOPUSERFIELDS_MILISEC) != 0)
         fprintf(fp_top3,"<th class=\"header_r\">%s</th>",fixnum2(ttnelap/totuser,1));
      fputs("</tr>\n",fp_top3);
   }

   fputs("</table></div>\n",fp_top3);
   if (write_html_trailer(fp_top3)<0)
      debuga(_("Write error in top user list %s\n"),top3);
   if (fclose(fp_top3)==EOF)
      debuga(_("Failed to close the top user list %s - %s\n"),top3,strerror(errno));

   snprintf(tusr,sizeof(tusr),"%s/sarg-users",outdirname);
   if((fp_ou=fopen(tusr,"w"))==NULL) {
      debuga(_("(topuser) Cannot open file %s\n"),tusr);
      exit(EXIT_FAILURE);
   }
   fprintf(fp_ou,"%d\n",totuser);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close file %s - %s\n"),tusr,strerror(errno));

   return;
}
