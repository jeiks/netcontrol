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

void htmlrel(void)
{
   DIR *dirp;
   FILE *fp_in, *fp_ou, *fp_ip, *fp_ip2, *fp_usr;

   struct dirent *direntp;
   long long int nnbytes=0, unbytes=0, tnbytes=0, totbytes=0, totbytes2=0;
   long long int totelap=0, totelap2=0, nnelap=0, unelap=0, tnelap=0;
   long long int incache=0, oucache=0, tnincache=0, tnoucache=0, twork=0;
   long long int ltemp;
   long long int ntotuser;
   long long int userbytes, userelap;
   char *buf;
   char arqin[MAXLEN], arqou[MAXLEN], arqper[MAXLEN], arqip[MAXLEN];
   char *url, tmsg[50], csort[MAXLEN];
   char user[MAXLEN], duser[MAXLEN];
   char user_ip[MAXLEN], olduserip[MAXLEN], tmp2[MAXLEN], tmp3[MAXLEN];
   char denied_report[255];
   char *str;
   char warea[MAXLEN];
   char totuser[8];
   char tmp6[MAXLEN];
   char *user_url;
   long long int tnacc=0, ttnacc=0, unacc=0;
   double perc=0, perc2=0, ouperc=0, inperc=0;
   int count;
   int cstatus;
   const char txtext[]=".txt";
   const char *sort_field;
   const char *sort_order;
   int dlen;
   char siteind[MAX_TRUNCATED_URL];
   struct getwordstruct gwarea;
   longline line,line1;
   struct generalitemstruct item;
   const struct userinfostruct *uinfo;

   if((ReportType & REPORT_TYPE_USERS_SITES) == 0) return;

   strcpy(tmp2,tmp);
   strcat(tmp2,"/sargtmp.unsort");

   strcpy(tmp3,tmp);
   strcat(tmp3,"/sargtmp.log");

   snprintf(arqper,sizeof(arqper),"%s/sarg-general",outdirname);
   if ((fp_in = fopen(arqper, "r")) == 0){
      debuga(_("(html2) Cannot open file %s\n"),arqper);
      exit(EXIT_FAILURE);
   }
   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read file %s\n"),arqper);
      exit(EXIT_FAILURE);
   }
   ttnacc=0;
   totbytes=0;
   totelap=0;
   while((buf=longline_read(fp_in,line))!=NULL) {
      ger_read(buf,&item,arqper);
      if(item.total) {
         ttnacc+=item.nacc;
         totbytes+=item.nbytes;
         totelap+=item.nelap;
      }
   }
   fclose(fp_in);
   longline_destroy(&line);

   snprintf(arqper,sizeof(arqper),"%s/sarg-users",outdirname);
   if ((fp_in = fopen(arqper, "r")) == 0){
      debuga(_("(html11) Cannot open file %s\n"),arqper);
      exit(EXIT_FAILURE);
   }
   if (!fgets(totuser,sizeof(totuser),fp_in)) {
      debuga(_("(html11) read error in %s\n"),arqper);
      exit(EXIT_FAILURE);
   }
   fclose(fp_in);
   ntotuser=my_atoll(totuser);
   if (ntotuser<=0) ntotuser=1;

   greport_prepare();

   sort_labels(&sort_field,&sort_order);

   if ((dirp = opendir(tmp)) == NULL) {
      debuga(_("Failed to open directory %s - %s\n"),tmp,strerror(errno));
      exit(EXIT_FAILURE);
   }
   while ( (direntp = readdir( dirp )) != NULL ) {
      dlen=strlen(direntp->d_name)-(sizeof(txtext)-1);
      if (dlen<0) continue;
      if(strcmp(direntp->d_name+dlen,txtext) != 0)
         continue;

      count=1;
      if (dlen>0) {
         if (dlen>=sizeof(user)) continue;
         strncpy(user,direntp->d_name,dlen);
         user[dlen]=0;
      } else {
         user[0]=0;
      }

      uinfo=userinfo_find_from_file(user);
      if (!uinfo) {
         debuga(_("Unknown user ID %s in directory %s\n"),user,tmp);
         exit(EXIT_FAILURE);
      }
      str=strrchr(user,'.');
      if (str)
         strcpy(denied_report,str+1);
      else
         strcpy(denied_report,user);

      if (snprintf(warea,sizeof(warea),"%s/%s",outdirname,user)>=sizeof(warea)) {
         debuga(_("Destination directory too long: %s/%s\n"),outdirname,user);
         exit(EXIT_FAILURE);
      }
      mkdir(warea,0755);

      report_day(uinfo);
      greport_day(uinfo);

      if (snprintf(arqin,sizeof(arqin),"%s/%s",tmp,direntp->d_name)>=sizeof(arqin)) {
         debuga(_("Input file name too long: %s/%s\n"),tmp,direntp->d_name);
         exit(EXIT_FAILURE);
      }
      if (snprintf(arqou,sizeof(arqou),"%s/%s/%s.html",outdirname,uinfo->filename,uinfo->filename)>=sizeof(arqou)) {
         debuga(_("Output file name too long: %s/%s/%s.html\n"),outdirname,uinfo->filename,uinfo->filename);
         exit(EXIT_FAILURE);
      }
      if (snprintf(duser,sizeof(duser),"%s/%s/denied_%s.html",tmp,direntp->d_name,denied_report)>=sizeof(duser)) {
         debuga(_("File name too long: %s/%s/denied_%s.html\n"),tmp,direntp->d_name,denied_report);
         exit(EXIT_FAILURE);
      }
      if(access(duser, R_OK) != 0)
         denied_report[0]='\0';

      if ((fp_in = fopen(arqin, "r")) == 0){
         debuga(_("(html3) Cannot open file %s\n"),arqin);
         exit(EXIT_FAILURE);
      }

      if ((line=longline_create())==NULL) {
         debuga(_("Not enough memory to read file %s\n"),arqin);
         exit(EXIT_FAILURE);
      }

      tnacc=0;
      tnbytes=0;
      tnelap=0;
      tnincache=0;
      tnoucache=0;
      while((buf=longline_read(fp_in,line))!=NULL) {
         getword_start(&gwarea,buf);
         if (getword_atoll(&ltemp,&gwarea,'\t')<0) {
            debuga(_("There is a broken number of access in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         tnacc+=ltemp;
         if (getword_atoll(&ltemp,&gwarea,'\t')<0) {
            debuga(_("There is a broken downloaded size in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         tnbytes+=ltemp;
         if (getword_ptr(NULL,NULL,&gwarea,'\t')<0) {
            debuga(_("There is a broken url in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword_skip(MAXLEN,&gwarea,'\t')<0) {
            debuga(_("There is a broken access code in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword_atoll(&ltemp,&gwarea,'\t')<0) {
            debuga(_("There is a broken elapsed time in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         tnelap+=ltemp;
         if (getword_atoll(&ltemp,&gwarea,'\t')<0) {
            debuga(_("There is a broken in-cache volume in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         tnincache+=ltemp;
         if (getword_atoll(&ltemp,&gwarea,'\n')<0) {
            debuga(_("There is a broken out-cache volume in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         tnoucache+=ltemp;
      }

      rewind(fp_in);

      if ((fp_ou = fopen(arqou, "w")) == 0){
         debuga(_("(html5) Cannot open file %s\n"),arqou);
         exit(EXIT_FAILURE);
      }

      write_html_header(fp_ou,(IndexTree == INDEX_TREE_DATE) ? 4 : 2,_("User report"));
      fprintf(fp_ou,"<tr><td class=\"header_c\">%s:&nbsp;%s</td></tr>\n",_("Period"),period.html);
      fprintf(fp_ou,"<tr><td class=\"header_c\">%s:&nbsp;%s</td></tr>\n",_("User"),uinfo->label);
      fputs("<tr><td class=\"header_c\">",fp_ou);
      fprintf(fp_ou,_("Sort:&nbsp;%s, %s"),sort_field,sort_order);
      fputs("</td></tr>\n",fp_ou);
      fprintf(fp_ou,"<tr><th class=\"header_c\">%s</th></tr>\n",_("User report"));
      close_html_header(fp_ou);

      fputs("<div class=\"report\"><table cellpadding=\"2\" cellspacing=\"1\">\n",fp_ou);

      if(denied_report[0]!='\0') {
         fprintf(fp_ou,"<tr><td class=\"header_l\" colspan=\"11\"><a href=\"denied_%s.html\">%s</a> %s</td></tr>\n",denied_report,_("SmartFilter"),_("Report"));
      }

      fprintf(fp_ou,"<tr><th></th><th class=\"header_l\">%s</th>",_("ACCESSED SITE"));

      if((UserReportFields & USERREPORTFIELDS_CONNECT) != 0)
         fprintf(fp_ou,"<th class=\"header_l\">%s</th>",_("CONNECT"));
      if((UserReportFields & USERREPORTFIELDS_BYTES) != 0)
         fprintf(fp_ou,"<th class=\"header_l\">%s</th>",_("BYTES"));
      if((UserReportFields & USERREPORTFIELDS_SETYB) != 0)
         fprintf(fp_ou,"<th class=\"header_l\">%%%s</th>",_("BYTES"));
      if((UserReportFields & USERREPORTFIELDS_IN_CACHE_OUT) != 0)
         fprintf(fp_ou,"<th class=\"header_c\" colspan=\"2\">%s</th>",_("IN-CACHE-OUT"));
      if((UserReportFields & USERREPORTFIELDS_USED_TIME) != 0)
         fprintf(fp_ou,"<th class=\"header_l\">%s</th>",_("ELAPSED TIME"));
      if((UserReportFields & USERREPORTFIELDS_MILISEC) != 0)
         fprintf(fp_ou,"<th class=\"header_l\">%s</th>",_("MILLISEC"));
      if((UserReportFields & USERREPORTFIELDS_PTIME) != 0)
         fprintf(fp_ou,"<th class=\"header_l\">%%%s</th>",_("TIME"));

      fputs("</tr>\n",fp_ou);

      if(debug) {
         debuga(_("Making report: %s\n"),uinfo->id);
      }

      while((buf=longline_read(fp_in,line))!=NULL) {
         getword_start(&gwarea,buf);
         if (getword_atoll(&twork,&gwarea,'\t')<0) {
            debuga(_("There is a broken number of access in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword_atoll(&nnbytes,&gwarea,'\t')<0) {
            debuga(_("There is a broken number of bytes in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword_ptr(buf,&url,&gwarea,'\t')<0) {
            debuga(_("There is a broken url in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword(tmsg,sizeof(tmsg),&gwarea,'\t')<0) {
            debuga(_("There is a broken access code in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword_atoll(&nnelap,&gwarea,'\t')<0) {
            debuga(_("There is a broken elapsed time in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword_atoll(&incache,&gwarea,'\t')<0) {
            debuga(_("There is a broken in cache column in file %s\n"),arqin);
            exit(EXIT_FAILURE);
         }
         if (getword_atoll(&oucache,&gwarea,'\n')<0) {
            debuga(_("There is a broken out of cache column in file %s (%d)\n"),arqin,__LINE__);
            exit(EXIT_FAILURE);
         }

         if(UserReportLimit<=0 || count<=UserReportLimit) {
            fputs("<tr>",fp_ou);

            if(IndexTree == INDEX_TREE_DATE)
               sprintf(tmp6,"../%s",ImageFile);
            else
               strcpy(tmp6,"../../images");

            if((ReportType & REPORT_TYPE_SITE_USER_TIME_DATE) != 0) {
               url_to_file(url,siteind,sizeof(siteind));
               fprintf(fp_ou,"<td class=\"data\"><a href=\"tt%s-%s.html\"><img src=\"%s/datetime.png\" title=\"%s\" alt=\"T\"></a></td>",uinfo->filename,siteind,tmp6,_("date/time report"));
            } else {
               fprintf(fp_ou,"<td class=\"data\"></td>");
            }

            if(Privacy)
               fprintf(fp_ou,"<td class=\"data2\"><span style=\"color:%s;\">%s</span></td>",PrivacyStringColor,PrivacyString);
            else {
               fputs("<td class=\"data2\">",fp_ou);
               if(BlockIt[0]!='\0') {
                  fprintf(fp_ou,"<a href=\"%s%s?url=",wwwDocumentRoot,BlockIt);
                  output_html_url(fp_ou,url);
                  fprintf(fp_ou,"\"><img src=\"%s/sarg-squidguard-block.png\"></a>&nbsp;",tmp6);
               }
               fputs("<a href=\"http://",fp_ou);
               output_html_url(fp_ou,url);
               fputs("\">",fp_ou);
               output_html_string(fp_ou,url,100);
               fputs("</a></td>",fp_ou);
            }

            if((UserReportFields & USERREPORTFIELDS_CONNECT) != 0)
               fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum(twork,1));
            if((UserReportFields & USERREPORTFIELDS_BYTES) != 0)
               fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum(nnbytes,1));
            if((UserReportFields & USERREPORTFIELDS_SETYB) != 0) {
               perc=(tnbytes) ? nnbytes * 100. / tnbytes : 0.;
               fprintf(fp_ou,"<td class=\"data\">%3.2lf%%</td>",perc);
            }
            if((UserReportFields & USERREPORTFIELDS_IN_CACHE_OUT) != 0) {
               inperc=(nnbytes) ? incache * 100. / nnbytes : 0.;
               ouperc=(nnbytes) ? oucache * 100. / nnbytes : 0.;
               fprintf(fp_ou,"<td class=\"data\">%3.2lf%%</td><td class=\"data\">%3.2lf%%</td>",inperc,ouperc);
            }
            if((UserReportFields & USERREPORTFIELDS_USED_TIME) != 0)
               fprintf(fp_ou,"<td class=\"data\">%s</td>",buildtime(nnelap));
            if((UserReportFields & USERREPORTFIELDS_MILISEC) != 0)
               fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum2(nnelap,1));
            if((UserReportFields & USERREPORTFIELDS_PTIME) != 0) {
               perc2=(tnelap) ? nnelap * 100. / tnelap : 0.;
               fprintf(fp_ou,"<td class=\"data\">%3.2lf%%</td>",perc2);
            }

            if(strncmp(tmsg,"OK",2) != 0)
               fprintf(fp_ou,"<td class=\"data\">%s</td>",_("DENIED"));

            fputs("</tr>\n",fp_ou);
            count++;
         }

         if(iprel) {
            if (snprintf(arqip,sizeof(arqip),"%s/%s.ip",tmp,uinfo->filename)>=sizeof(arqip)) {
               debuga(_("File name too long: %s/%s.ip\n"),tmp,uinfo->filename);
               exit(EXIT_FAILURE);
            }

            if ((fp_ip = fopen(arqip, "r")) == 0){
               debuga(_("(html6) Cannot open file %s\n"),arqip);
               exit(EXIT_FAILURE);
            }

            if ((fp_ip2 = MY_FOPEN(tmp2, "a")) == 0){
               debuga(_("(html7) Cannot open file %s\n"),tmp2);
               exit(EXIT_FAILURE);
            }

            if ((line1=longline_create())==NULL) {
               debuga(_("Not enough memory to read file %s\n"),arqip);
               exit(EXIT_FAILURE);
            }
            while((buf=longline_read(fp_ip,line1))!=NULL) {
               getword_start(&gwarea,buf);
               if (getword(user_ip,sizeof(user_ip),&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken user IP in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if (getword_ptr(buf,&user_url,&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken url in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if (strncmp(user_url,url,strlen(url))!=0) continue;
               if (getword_skip(15,&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken day in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if (getword_skip(15,&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken time in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if (getword_atoll(&userbytes,&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken size in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if (getword_atoll(&userelap,&gwarea,'\0')<0) {
                  debuga(_("Maybe you have a broken elapsed time in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               fprintf(fp_ip2,"%s\t%"PRIu64"\t%"PRIu64"\n",user_ip,(uint64_t)userbytes,(uint64_t)userelap);
            }
            longline_destroy(&line1);

            fclose(fp_ip);
            fclose(fp_ip2);

            sprintf(csort,"sort -n -T \"%s\" -k 1,1 -k 2,2 -o \"%s\" \"%s\"",tmp,tmp3,tmp2);
            cstatus=system(csort);
            if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
               debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
               debuga(_("sort command: %s\n"),csort);
               exit(EXIT_FAILURE);
            }

            if ((fp_ip = MY_FOPEN(tmp3, "r")) == 0) {
               debuga(_("(html8) Cannot open file %s\n"),tmp3);
               exit(EXIT_FAILURE);
            }

            olduserip[0]='\0';

            if ((line1=longline_create())==NULL) {
               debuga(_("Not enough memory to read file %s\n"),arqip);
               exit(EXIT_FAILURE);
            }
            while((buf=longline_read(fp_ip,line1))!=NULL) {
               getword_start(&gwarea,buf);
               if (getword(user_ip,sizeof(user_ip),&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken user IP in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if (getword_atoll(&userbytes,&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken size in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if (getword_atoll(&userelap,&gwarea,'\0')<0) {
                  debuga(_("Maybe you have a broken elapsed time in your %s file\n"),tmp3);
                  exit(EXIT_FAILURE);
               }
               if(strcmp(user_ip,olduserip) != 0) {
                  if (olduserip[0]!='\0') {
                     fprintf(fp_ou,"<tr><td></td><td class=\"data\">%s</td>",olduserip);
                     if((UserReportFields & USERREPORTFIELDS_CONNECT) != 0)
                        fputs("<td></td>",fp_ou);
                     if((UserReportFields & USERREPORTFIELDS_BYTES) != 0)
                        fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum(unbytes,1));
                     if((UserReportFields & USERREPORTFIELDS_SETYB) != 0)
                        fputs("<td></td>",fp_ou);
                     if((UserReportFields & USERREPORTFIELDS_IN_CACHE_OUT) != 0)
                        fputs("</td><td></td><td></td>",fp_ou);
                     if((UserReportFields & USERREPORTFIELDS_USED_TIME) != 0)
                        fprintf(fp_ou,"<td class=\"data\">%s</td>",buildtime(unelap));
                     if((UserReportFields & USERREPORTFIELDS_MILISEC) != 0)
                        fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum2(unelap,1));
                     fputs("</tr>\n",fp_ou);
                  }

                  strcpy(olduserip,user_ip);
                  unacc=0;
                  unbytes=0;
                  unelap=0;
               }

               unbytes+=userbytes;
               unelap+=userelap;
            }

            fclose(fp_ip);
            longline_destroy(&line1);

            unlink(tmp2);
            unlink(tmp3);

            if (olduserip[0]!='\0') {
               fprintf(fp_ou,"<tr><td></td><td class=\"data\">%s</td>",olduserip);
               if((UserReportFields & USERREPORTFIELDS_CONNECT) != 0)
                  fputs("<td></td>",fp_ou);
               if((UserReportFields & USERREPORTFIELDS_BYTES) != 0)
                  fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum(unbytes,1));
               if((UserReportFields & USERREPORTFIELDS_SETYB) != 0)
                  fputs("<td></td>",fp_ou);
               if((UserReportFields & USERREPORTFIELDS_IN_CACHE_OUT) != 0)
                  fputs("</td><td></td><td></td>",fp_ou);
               if((UserReportFields & USERREPORTFIELDS_USED_TIME) != 0)
                  fprintf(fp_ou,"<td class=\"data\">%s</td>",buildtime(unelap));
               if((UserReportFields & USERREPORTFIELDS_MILISEC) != 0)
                  fprintf(fp_ou,"<td class=\"data\">%s</td>",fixnum2(unelap,1));
               fputs("</tr>\n",fp_ou);
            }
         }

         unacc=0;
         unbytes=0;
         unelap=0;
      }

      fclose(fp_in);
      longline_destroy(&line);

      if(iprel)
         unlink(arqip);
      unlink(arqin);

      if((UserReportFields & USERREPORTFIELDS_TOTAL) != 0) {
         fprintf(fp_ou,"<tr><th></th><th class=\"header_l\">%s</th>",_("TOTAL"));
         if((UserReportFields & USERREPORTFIELDS_CONNECT) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",fixnum(tnacc,1));
         if((UserReportFields & USERREPORTFIELDS_BYTES) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",fixnum(tnbytes,1));
         if((UserReportFields & USERREPORTFIELDS_SETYB) != 0) {
            perc=(totbytes) ? tnbytes *100. / totbytes :0.;
            fprintf(fp_ou,"<th class=\"header_r\">%3.2lf%%</th>",perc);
         }
         if((UserReportFields & USERREPORTFIELDS_IN_CACHE_OUT) != 0) {
            inperc=(tnbytes) ? tnincache * 100. / tnbytes : 0.;
            ouperc=(tnbytes) ? tnoucache * 100. / tnbytes : 0.;
            fprintf(fp_ou,"<th class=\"header_r\">%3.2lf%%</th><th class=\"header_r\">%3.2lf%%</th>",inperc,ouperc);
         }
         if((UserReportFields & USERREPORTFIELDS_USED_TIME) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",buildtime(tnelap));
         if((UserReportFields & USERREPORTFIELDS_MILISEC) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",fixnum2(tnelap,1));
         if((UserReportFields & USERREPORTFIELDS_PTIME) != 0) {
            perc2=(totelap) ? tnelap * 100. / totelap : 0.;
            fprintf(fp_ou,"<th class=\"header_r\">%3.2lf%%</th>",perc2);
         }
         fputs("</tr>\n",fp_ou);
      }

      if(PerUserLimit > 0) {
         if(tnbytes > (PerUserLimit*1000000)) {
            limit_flag=0;
            if(access(PerUserLimitFile, R_OK) == 0) {
               if((fp_usr = fopen(PerUserLimitFile, "r")) == 0) {
                  debuga(_("(html9) Cannot open file %s\n"),PerUserLimitFile);
                  exit(EXIT_FAILURE);
               }
               while(fgets(tmp6,sizeof(tmp6),fp_usr)!=NULL) {
                  fixendofline(tmp6);
                  if(strcmp(tmp6,uinfo->label) == 0) {
                     limit_flag=1;
                     break;
                  }
               }
               fclose(fp_usr);
            }

            if(!limit_flag) {
               if((fp_usr = fopen(PerUserLimitFile, "a")) == 0) {
                  debuga(_("(html10) Cannot open file %s\n"),PerUserLimitFile);
                  exit(EXIT_FAILURE);
               }
               fprintf(fp_usr,"%s\n",uinfo->label);
               fclose(fp_usr);

               if(debug)
                  debuga(_("User %s limit exceeded (%d MB). Added to file %s\n"),uinfo->label,PerUserLimit,PerUserLimitFile);
            }
         }
      }

//      if(indexonly) {
//	unlink(arqou);
//	continue;
//      }

      if ((ReportType & REPORT_TYPE_TOPUSERS) != 0 && (UserReportFields & USERREPORTFIELDS_AVERAGE) != 0) {
         totbytes2=totbytes/ntotuser;
         totelap2=totelap/ntotuser;

         fprintf(fp_ou,"<tr><th></th><th class=\"header_l\">%s</th>",_("AVERAGE"));
         if((UserReportFields & USERREPORTFIELDS_CONNECT) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",fixnum(ttnacc/ntotuser,1));
         if((UserReportFields & USERREPORTFIELDS_BYTES) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",fixnum(totbytes2,1));
         fprintf(fp_ou,"<th></th><th></th><th></th>");
         if((UserReportFields & USERREPORTFIELDS_USED_TIME) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",buildtime(totelap2));
         if((UserReportFields & USERREPORTFIELDS_MILISEC) != 0)
            fprintf(fp_ou,"<th class=\"header_r\">%s</th>",fixnum2(totelap2,1));
         if((UserReportFields & USERREPORTFIELDS_PTIME) != 0) {
            perc2 = (totelap) ? totelap2 * 100. / totelap : 0.;
            fprintf(fp_ou,"<th class=\"header_r\">%3.2lf%%</th>",perc2);
         }
         fputs("</tr>\n",fp_ou);
      }

      fputs("</table></div>\n",fp_ou);
      if (write_html_trailer(fp_ou)<0)
         debuga(_("Write error in file %s\n"),arqou);
      if (fclose(fp_ou)==EOF)
         debuga(_("Failed to close file %s - %s\n"),arqou,strerror(errno));

      htaccess(uinfo);
   }

   (void)closedir(dirp);
   greport_cleanup();

   return;
}
