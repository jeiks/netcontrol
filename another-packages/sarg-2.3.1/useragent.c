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

void useragent(void)
{

   FILE *fp_in = NULL, *fp_ou = NULL, *fp_ht = NULL;
   char buf[MAXLEN];
   char ip[MAXLEN], data[MAXLEN], agent[MAXLEN], user[MAXLEN];
   char ipbefore[MAXLEN], namebefore[MAXLEN];
   char tagent[MAXLEN];
   char user_old[MAXLEN]="$#%0a3bc6";
   char agent_old[MAXLEN]="$#%0a3bc6";
   char hfile[MAXLEN];
   char idate[MAXLEN], fdate[MAXLEN];
   char tmp2[MAXLEN];
   char tmp3[MAXLEN];
   char day[4],month[5],year[5], wdate[20];
   char csort[MAXLEN];
   int  agentot=0, agentot2=0, agentdif=0, cont=0, nagent;
   unsigned long totregsl=0;
   int cstatus;
   int ndate;
   double perc;
   struct getwordstruct gwarea, gwarea1;

   ip[0]='\0';
   data[0]='\0';
   agent[0]='\0';
   user[0]='\0';
   user_old[0]='\0';
   agent_old[0]='\0';
   ipbefore[0]='\0';
   namebefore[0]='\0';

   sprintf(tmp3,"%s/squagent.unsort",tmp);
   sprintf(tmp2,"%s/squagent.log",tmp);

   if((fp_in=fopen(UserAgentLog,"r"))==NULL) {
      debuga(_("(useragent) Cannot open file %s\n"),UserAgentLog);
      exit(EXIT_FAILURE);
   }

   if((fp_ou=fopen(tmp3,"w"))==NULL) {
      debuga(_("(useragent) Cannot open file %s\n"),tmp3);
      exit(EXIT_FAILURE);
   }

   if(debug) {
      debuga(_("Reading useragent log: %s\n"),UserAgentLog);
   }

   while(fgets(buf,sizeof(buf),fp_in)!=NULL) {
      totregsl++;
      getword_start(&gwarea,buf);
      if (getword(ip,sizeof(ip),&gwarea,' ')<0 || getword_skip(MAXLEN,&gwarea,'[')<0 ||
          getword(data,sizeof(data),&gwarea,' ')<0) {
         debuga(_("Maybe you have a broken record or garbage in your %s file\n"),UserAgentLog);
         exit(EXIT_FAILURE);
      }
      getword_start(&gwarea1,data);
      if (getword(day,sizeof(day),&gwarea1,'/')<0 || getword(month,sizeof(month),&gwarea1,'/')<0 ||
          getword(year,sizeof(year),&gwarea1,':')<0) {
         debuga(_("Maybe you have a broken date in your %s file\n"),UserAgentLog);
         exit(EXIT_FAILURE);
      }
      buildymd(day,month,year,wdate);
      ndate=atoi(wdate);
      if (ndate<dfrom) continue;
      if (ndate>duntil) break;
      if(totregsl == 1)
         strcpy(idate,data);
      strcpy(fdate,data);
      if (getword_skip(MAXLEN,&gwarea,'"')<0 || getword(agent,sizeof(agent),&gwarea,'"')<0) {
         debuga(_("Maybe you have a broken useragent entry in your %s file\n"),UserAgentLog);
         exit(EXIT_FAILURE);
      }

      if(gwarea.current[0]!='\0') {
         if (getword_skip(MAXLEN,&gwarea,' ')<0 || getword(user,sizeof(user),&gwarea,'\n')<0) {
            debuga(_("Maybe you have a broken record or garbage in your %s file\n"),UserAgentLog);
            exit(EXIT_FAILURE);
         }
         if(user[0] == '-')
            strcpy(user,ip);
         if(user[0] == '\0')
            strcpy(user,ip);
      } else {
         strcpy(user,ip);
      }

      fprintf(fp_ou,"%s\t%s\t%s\n",ip,agent,user);
      useragent_count++;
   }

   if(debug) {
      debuga(_("   Records read: %ld\n"),totregsl);
   }

   fclose(fp_in);
   if (fclose(fp_ou)==EOF) {
      debuga(_("Failed to close file %s - %s\n"),tmp3,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if(debug) {
      debuga(_("Sorting file: %s\n"),tmp2);
   }

   sprintf(csort,"sort -n -t \"\t\" -k 3,3 -k 2,2 -k 1,1 -o \"%s\" \"%s\"",tmp2,tmp3);
   cstatus=system(csort);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }
   if((fp_in=fopen(tmp2,"r"))==NULL) {
      debuga(_("(useragent) Cannot open file %s\n"),tmp2);
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }

   unlink(tmp3);

   snprintf(hfile,sizeof(hfile),"%s/useragent.html", outdirname);
   if((fp_ht=fopen(hfile,"w"))==NULL) {
      debuga(_("(useragent) Cannot open file %s\n"),hfile);
      exit(EXIT_FAILURE);
   }

   if(debug)
      debuga(_("Making Useragent report\n"));

   write_html_header(fp_ht,(IndexTree == INDEX_TREE_DATE) ? 3 : 1,_("Squid Useragent's Report"));
   fprintf(fp_ht,"<tr><th class=\"header_c\">%s</th></tr>\n",_("Squid Useragent's Report"));
   fprintf(fp_ht,"<tr><td class=\"header_c\">%s: %s - %s</td></tr>\n",_("Period"),idate,fdate);
   close_html_header(fp_ht);

   fputs("<br><br>\n",fp_ht);

   fputs("<div class=\"report\"><table cellpadding=\"0\" cellspacing=\"0\">\n",fp_ht);
   fputs("<tr><td>&nbsp;</td><td>&nbsp;</td></tr>",fp_ht);

   fprintf(fp_ht,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th></tr>\n",_("USERID"),_("AGENT"));

   while(fgets(buf,sizeof(buf),fp_in)!=NULL) {
      getword_start(&gwarea,buf);
      if (getword(ip,sizeof(ip),&gwarea,'\t')<0) {
         debuga(_("There is an invalid IP address in file %s\n"),tmp2);
         exit(EXIT_FAILURE);
      }

      if(Ip2Name) {
         if(strcmp(ip,ipbefore) != 0) {
            strcpy(ipbefore,ip);
            ip2name(ip,sizeof(ip));
            strcpy(namebefore,ip);
         } else strcpy(ip,namebefore);
      }

      if (getword(agent,sizeof(agent),&gwarea,'\t')<0) {
         debuga(_("There is an invalid useragent in file %s\n"),tmp2);
         exit(EXIT_FAILURE);
      }
      if (getword(user,sizeof(user),&gwarea,'\t')<0) {
         debuga(_("There is an invalid user ID in file %s\n"),tmp2);
         exit(EXIT_FAILURE);
      }

      if(strcmp(user,user_old) != 0) {
         fprintf(fp_ht,"<tr><td class=\"data2\">%s</td><td class=\"data2\">",user);
         output_html_string(fp_ht,agent,250);
         fputs("</td></tr>\n",fp_ht);
         strcpy(user_old,user);
         strcpy(agent_old,agent);
      } else if(strcmp(agent,agent_old) != 0) {
         fputs("<tr><td></td><td class=\"data2\">",fp_ht);
         output_html_string(fp_ht,agent,250);
         fputs("</td></tr>\n",fp_ht);
         strcpy(agent_old,agent);
      }
   }

   fputs("</table>\n",fp_ht);
   fclose(fp_in);

   sprintf(csort,"sort -t \"\t\" -k 2,2 -o \"%s\" \"%s\"",tmp3,tmp2);
   cstatus=system(csort);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }
   if((fp_in=fopen(tmp3,"r"))==NULL) {
      debuga(_("(useragent) Cannot open file %s\n"),tmp3);
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }

   unlink(tmp2);

   if((fp_ou=fopen(tmp2,"w"))==NULL) {
      debuga(_("(useragent) Cannot open file %s\n"),tmp2);
      exit(EXIT_FAILURE);
   }

   agent_old[0]='\0';
   cont=0;

   while(fgets(buf,sizeof(buf),fp_in)!=NULL) {
      getword_start(&gwarea,buf);
      if (getword(ip,sizeof(ip),&gwarea,'\t')<0) {
         debuga(_("There is an invalid IP address in file %s\n"),tmp3);
         exit(EXIT_FAILURE);
      }
      if (getword(agent,sizeof(agent),&gwarea,'\t')<0) {
         debuga(_("There is an invalid useragent in file %s\n"),tmp3);
         exit(EXIT_FAILURE);
      }

      if(!cont) {
         cont++;
         strcpy(agent_old,agent);
      }

      if(strcmp(agent,agent_old) != 0) {
         agentdif++;
         fprintf(fp_ou,"%06d %s\n",agentot,agent_old);
         strcpy(agent_old,agent);
         agentot2+=agentot;
         agentot=0;
      }
      agentot++;
   }
   agentdif++;
   fprintf(fp_ou,"%06d %s\n",agentot,agent);
   agentot2+=agentot;

   fclose(fp_in);
   if (fclose(fp_ou)==EOF) {
      debuga(_("Failed to close file %s - %s\n"),tmp3,strerror(errno));
      exit(EXIT_FAILURE);
   }

   unlink(tmp3);

   sprintf(csort,"sort -n -r -k 1,1 -o \"%s\" \"%s\"",tmp3,tmp2);
   cstatus=system(csort);
   if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
      debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }
   if((fp_in=fopen(tmp3,"r"))==NULL) {
      debuga(_("(useragent) Cannot open file %s\n"),tmp3);
      debuga(_("sort command: %s\n"),csort);
      exit(EXIT_FAILURE);
   }

   unlink(tmp2);

   fputs("<br><br>\n",fp_ht);

   fputs("<table cellpadding=\"0\" cellspacing=\"0\">\n",fp_ht);
   fprintf(fp_ht,"<tr><th class=\"header_l\">%s</th><th class=\"header_l\">%s</th><th class=\"header_c\">%%</th></tr>\n",_("AGENT"),_("TOTAL"));

   perc=0.;
   while(fgets(buf,sizeof(buf),fp_in)!=NULL) {
      fixendofline(buf);
      getword_start(&gwarea,buf);
      if (getword(tagent,sizeof(tagent),&gwarea,' ')<0) {
         debuga(_("There is an invalid useragent in file %s\n"),tmp3);
         exit(EXIT_FAILURE);
      }
      nagent=atoi(tagent);
      perc=(agentot2>0) ? nagent * 100. / agentot2 : 0.;

      fputs("<tr><td class=\"data2\">",fp_ht);
      output_html_string(fp_ht,gwarea.current,250);
      fprintf(fp_ht,"</td><td class=\"data\">%d</td><td class=\"data\">%3.2lf</td></tr>\n",nagent,perc);
   }
   fclose(fp_in);

   fputs("</table></div>\n",fp_ht);
   if (write_html_trailer(fp_ht)<0)
      debuga(_("Write error in file %s\n"),hfile);
   if (fclose(fp_ht)==EOF)
      debuga(_("Failed to close file %s - %s\n"),hfile,strerror(errno));

   unlink(tmp3);

   return;

}
