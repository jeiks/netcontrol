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

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#define REPORT_EVERY_X_LINES 5000
#define MAX_OPEN_USER_FILES 10

struct userfilestruct
{
   struct userfilestruct *next;
   struct userinfostruct *user;
   FILE *file;
};

/*@null@*/static char *userfile=NULL;

numlist weekdays = { { 0, 1, 2, 3, 4, 5, 6 }, 7 };
numlist hours = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
             13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 }, 24 };

static void getusers(const char *pwdfile, int debug);

int main(int argc,char *argv[])
{
   enum isa_col_id {
      ISACOL_Ip,
      ISACOL_UserName,
      ISACOL_Date,
      ISACOL_Time,
      ISACOL_TimeTaken,
      ISACOL_Bytes,
      ISACOL_Uri,
      ISACOL_Status,
      ISACOL_Last //last entry of the list !
   };
   enum InputLogFormat {
      ILF_Unknown,
      ILF_Squid,
      ILF_Common,
      ILF_Sarg,
      ILF_Isa,
      ILF_Last //last entry of the list !
   };

   FILE *fp_in = NULL, *fp_denied=NULL, *fp_authfail=NULL, *fp_log=NULL;

   char sz_Download_Unsort[ 20000 ] ;
   FILE * fp_Download_Unsort = NULL ;

   extern int optind;
   extern int optopt;
   extern char *optarg;

   char data[255];
   char elap[255];
   char ip[MAXLEN];
   char tam[255];
   char fun[MAXLEN];
   char wuser[MAXLEN];
   char smartfilter[MAXLEN];
   char dia[128];
   char mes[30];
   char ano[30];
   char hora[30];
   char wtemp[MAXLEN];
   char date[255];
   char arq[255];
   char arq_log[255];
   char hm[15], hmf[15], hmr[15];
   int  chm=0;
   char uagent[MAXLEN];
   char hexclude[MAXLEN];
   char csort[MAXLEN];
   int cstatus;
   char tbuf2[128];
   char zip[20];
   char *str;
   char tmp3[MAXLEN];
   char denied_unsort[MAXLEN];
   char denied_sort[MAXLEN];
   char authfail_unsort[MAXLEN];
   char start_hour[128];
   char end_hour[128];
   char *linebuf;
   char hostname[512];
   char *url;
   char *urly;
   char user[MAX_USER_LEN];
   enum InputLogFormat ilf;
   int ilf_count[ILF_Last];
   int  ch;
   int  x;
   int  errflg=0;
   int  puser=0;
   bool  fhost=false;
   bool  dns=false;
   bool  fuser=false;
   int  idata=0;
   int  mindate=0;
   int  maxdate=0;
   int  iarq=0;
   int isa_ncols=0,isa_cols[ISACOL_Last];
   bool from_stdin;
   bool from_pipe;
   int blen;
   int maxopenfiles;
   int nopen;
   bool id_is_ip;
   long totregsl=0;
   long totregsg=0;
   long totregsx=0;
   bool totper=false;
   long int max_elapsed=0;
   long long int iyear, imonth, iday;
   bool realt;
   bool userip;
   struct tm tt;
   struct tm *t;
   unsigned long recs1=0UL;
   unsigned long recs2=0UL;
   int OutputNonZero = REPORT_EVERY_X_LINES ;
   bool download_flag=false;
   char *download_url=NULL;
   struct getwordstruct gwarea;
   longline line;
   time_t tnum;
   struct stat logstat;
   struct userinfostruct *uinfo;
   struct userfilestruct *first_user_file, *ufile, *ufile1, *prev_ufile;
   static int split=0;
   static int convert=0;
   static int output_css=0;
   int option_index;
   static struct option long_options[]=
   {
      {"convert",no_argument,&convert,1},
      {"css",no_argument,&output_css,1},
      {"split",no_argument,&split,1},
      {0,0,0,0}
   };

#ifdef HAVE_LOCALE_H
   setlocale(LC_TIME,"");
#endif

#if defined(ENABLE_NLS) && defined(HAVE_LOCALE_H)
   if (!setlocale (LC_ALL, "")) {
      fprintf(stderr,"SARG: Cannot set the locale LC_ALL to the environment variable\n");
      exit(EXIT_FAILURE);
   }
   if (!bindtextdomain (PACKAGE_NAME, LOCALEDIR)) {
      fprintf(stderr,"SARG: Cannot bind to text domain %s in directory %s (%s)\n",PACKAGE_NAME,LOCALEDIR,strerror(errno));
      exit(EXIT_FAILURE);
   }
   if (!textdomain (PACKAGE_NAME)) {
      fprintf(stderr,"SARG: Cannot set gettext domain for %s PACKAGE_NAME (%s)\n",PACKAGE_NAME,strerror(errno));
      exit(EXIT_FAILURE);
   }
#endif //ENABLE_NLS

   BgImage[0]='\0';
   LogoImage[0]='\0';
   LogoText[0]='\0';
   PasswdFile[0]='\0';
   OutputEmail[0]='\0';
   UserAgentLog[0]='\0';
   ExcludeHosts[0]='\0';
   ExcludeUsers[0]='\0';
   ConfigFile[0]='\0';
   code[0]='\0';
   LastLog=0;
   ReportType=0UL;
   UserTabFile[0]='\0';
   BlockIt[0]='\0';
   ExternalCSSFile[0]='\0';
   RedirectorLogFormat[0]='\0';
   NRedirectorLogs=0;
   for (ilf=0 ; ilf<ILF_Last ; ilf++) ilf_count[ilf]=0;

   sprintf(ExcludeCodes,"%s/exclude_codes",SYSCONFDIR);
   strcpy(GraphDaysBytesBarColor,"orange");
   strcpy(BgColor,"#ffffff");
   strcpy(TxColor,"#000000");
   strcpy(TxBgColor,"lavender");
   strcpy(TiColor,"darkblue");
   strcpy(Width,"80");
   strcpy(Height,"45");
   strcpy(LogoTextColor,"#000000");
   strcpy(HeaderColor,"darkblue");
   strcpy(HeaderBgColor,"#dddddd");
   strcpy(LogoTextColor,"#006699");
   strcpy(FontSize,"9px");
   strcpy(TempDir,"/tmp");
   strcpy(OutputDir,"/var/www/html/squid-reports");
   Ip2Name=false;
   strcpy(DateFormat,"u");
   OverwriteReport=false;
   RemoveTempFiles=true;
   strcpy(ReplaceIndex,"index.html");
   Index=INDEX_YES;
   RecordsWithoutUser=RECORDWITHOUTUSER_IP;
   UseComma=0;
   strcpy(MailUtility,"mailx");
   TopSitesNum=100;
   TopUsersNum=0;
   UserIp=0;
   TopuserSort=TOPUSER_SORT_BYTES | TOPUSER_SORT_REVERSE;
   UserSort=USER_SORT_BYTES | USER_SORT_REVERSE;
   TopsitesSort=TOPSITE_SORT_CONNECT | TOPSITE_SORT_REVERSE;
   LongUrl=0;
   strcpy(FontFace,"Verdana,Tahoma,Arial");
   datetimeby=DATETIME_BYTE;
   strcpy(CharSet,"ISO-8859-1");
   Privacy=0;
   strcpy(PrivacyString,"***.***.***.***");
   strcpy(PrivacyStringColor,"blue");
   SuccessfulMsg=true;
   TopUserFields=TOPUSERFIELDS_NUM | TOPUSERFIELDS_DATE_TIME | TOPUSERFIELDS_USERID | TOPUSERFIELDS_CONNECT |
                 TOPUSERFIELDS_BYTES | TOPUSERFIELDS_SETYB | TOPUSERFIELDS_IN_CACHE_OUT |
                 TOPUSERFIELDS_USED_TIME | TOPUSERFIELDS_MILISEC | TOPUSERFIELDS_PTIME |
                 TOPUSERFIELDS_TOTAL | TOPUSERFIELDS_AVERAGE;
   UserReportFields=USERREPORTFIELDS_CONNECT | USERREPORTFIELDS_BYTES | USERREPORTFIELDS_SETYB |
                    USERREPORTFIELDS_IN_CACHE_OUT | USERREPORTFIELDS_USED_TIME | USERREPORTFIELDS_MILISEC |
                    USERREPORTFIELDS_PTIME | USERREPORTFIELDS_TOTAL | USERREPORTFIELDS_AVERAGE;
   strcpy(DataFileDelimiter,";");
   DataFileFields=DATA_FIELD_USER | DATA_FIELD_DATE | DATA_FIELD_TIME | DATA_FIELD_URL | DATA_FIELD_CONNECT |
                  DATA_FIELD_BYTES | DATA_FIELD_IN_CACHE | DATA_FIELD_OUT_CACHE | DATA_FIELD_ELAPSED;
   ShowReadStatistics=true;
   strcpy(IndexSortOrder,"D");
   ShowSargInfo=true;
   ShowSargLogo=true;
   strcpy(ParsedOutputLog,"no");
   strcpy(ParsedOutputLogCompress,"/bin/gzip -f");
   DisplayedValues=DISPLAY_ABBREV;
   strcpy(HeaderFontSize,"9px");
   strcpy(TitleFontSize,"11px");
   strcpy(AuthUserTemplateFile,"sarg_htaccess");
   set_download_suffix("7z,ace,arj,avi,bat,bin,bz2,bzip,cab,com,cpio,dll,doc,dot,exe,gz,iso,lha,lzh,mdb,mov,mp3,mpeg,mpg,mso,nrg,ogg,ppt,rar,rtf,shs,src,sys,tar,tgz,vcd,vob,wma,wmv,zip");
   Graphs=true;
#if defined(FONTDIR)
   strcpy(GraphFont,FONTDIR"/DejaVuSans.ttf");
#else
   GraphFont[0]='\0';
#endif
   strcpy(Ulimit,"20000");
   NtlmUserFormat=NTLMUSERFORMAT_DOMAINUSER;
   IndexTree=INDEX_TREE_FILE;
   strcpy(RealtimeTypes,"GET,PUT,CONNECT");
   RealtimeUnauthRec=REALTIME_UNAUTH_REC_SHOW;
   RedirectorFilterOutDate=true;
   DansguardianFilterOutDate=true;
   DataFileUrl=DATAFILEURL_IP;
   strcpy(MaxElapsed,"28800000");
   BytesInSitesUsersReport=0;
   UserAuthentication=0;
   strcpy(LDAPHost,"127.0.0.1");
   LDAPPort=389;
   LDAPProtocolVersion=3;
   LDAPBindDN[0]='\0';
   LDAPBindPW[0]='\0';
   LDAPBaseSearch[0]='\0';
   strcpy(LDAPFilterSearch, "(uid=%s)");
   strcpy(LDAPTargetAttr, "cn");

   dia[0]='\0';
   mes[0]='\0';
   ano[0]='\0';
   hora[0]='\0';
   tmp[0]='\0';
   tmp3[0]='\0';
   wtemp[0]='\0';
   us[0]='\0';
   date[0]='\0';
   df[0]='\0';
   uagent[0]='\0';
   hexclude[0]='\0';
   addr[0]='\0';
   hm[0]='\0';
   hmf[0]='\0';
   site[0]='\0';
   outdir[0]='\0';
   elap[0]='\0';
   email[0]='\0';
   zip[0]='\0';
   UserInvalidChar[0]='\0';
   DataFile[0]='\0';
   SquidGuardConf[0]='\0';
   DansGuardianConf[0]='\0';
   start_hour[0]='\0';
   end_hour[0]='\0';

   denied_count=0;
   download_count=0;
   authfail_count=0;
   dansguardian_count=0;
   squidguard_count=0;
   useragent_count=0;
   DeniedReportLimit=10;
   AuthfailReportLimit=10;
   DansGuardianReportLimit=10;
   SquidGuardReportLimit=10;
   DownloadReportLimit=50;
   UserReportLimit=0;
   debug=0;
   debugz=0;
   debugm=0;
   iprel=false;
   userip=false;
   realt=false;
   realtime_refresh=3;
   realtime_access_log_lines=1000;
   cost=0.01;
   nocost=50000000;
   ndownload=0;
   squid24=false;
   dfrom=0;
   duntil=0;

   bzero(IncludeUsers, sizeof(IncludeUsers));
   bzero(ExcludeString, sizeof(ExcludeString));
   first_user_file=NULL;
   memset(&period,0,sizeof(period));

   NAccessLog=0;
   for(x=0; x<MAXLOGS; x++)
      AccessLog[x][0]='\0';
   AccessLogFromCmdLine=0;
   RedirectorLogFromCmdLine=0;

   strcpy(Title,_("Squid User Access Report"));

   while((ch = getopt_long_only(argc, argv, "a:b:c:d:e:f:g:u:l:L:o:s:t:w:hijmnprvxyz",long_options,&option_index)) != -1){
      switch(ch)
      {
         case 0:
            break;
         case 'a':
            strcpy(addr,optarg);
            break;
         case 'b':
            strcpy(uagent,optarg);
            break;
         case 'c':
            strcpy(hexclude,optarg);
            break;
         case 'd':
            strncpy(date,optarg,sizeof(date)-1);
            date[sizeof(date)-1]='\0';
            date_from(date, &dfrom, &duntil);
            break;
         case 'e':
            strcpy(email,optarg);
            break;
         case 'f':
            strcpy(ConfigFile,optarg);
            break;
         case 'g':
            strcpy(df,optarg);
            break;
         case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
         case 'i':
            iprel=true;
            break;
         case 'l':
            if (NAccessLog>=MAXLOGS) {
               debuga(_("Too many log files passed on command line with option -l.\n"));
               exit(EXIT_FAILURE);
            }
            if (strlen(optarg)>=MAX_LOG_FILELEN) {
               debuga(_("Log file name too long passed on command line with option -l: %s\n"),optarg);
               exit(EXIT_FAILURE);
            }
            strcpy(AccessLog[NAccessLog],optarg);
            NAccessLog++;
            AccessLogFromCmdLine++;
            break;
         case 'L':
            if (NRedirectorLogs>MAX_REDIRECTOR_LOGS) {
               debuga(_("Too many redirector logs passed on command line with option -L.\n"));
               exit(EXIT_FAILURE);
            }
            if (strlen(optarg)>=MAX_REDIRECTOR_FILELEN) {
               debuga(_("Redirector log file name too long passed on command line with opton -L: %s\n"),optarg);
               exit(EXIT_FAILURE);
            }
            strcpy(RedirectorLogs[NRedirectorLogs],optarg);
            NRedirectorLogs++;
            RedirectorLogFromCmdLine++;
            break;
         case 'm':
            debugm++;
            break;
         case 'n':
            dns=true;
            break;
         case 'o':
            strcpy(outdir,optarg);
            break;
         case 'p':
            userip=true;
            break;
         case 'r':
            realt=true;
            break;
         case 's':
            strcpy(site,optarg);
            break;
         case 't':
         {
            int h,m;

            if(strstr(optarg,"-") == 0) {
              strcpy(hm,optarg);
              strcpy(hmf,optarg);
            } else {
               getword_start(&gwarea,optarg);
               if (getword(hm,sizeof(hm),&gwarea,'-')<0 || getword(hmf,sizeof(hmf),&gwarea,'\0')<0) {
                  debuga(_("The time range passed on the command line with option -t is invalid\n"));
                  exit(EXIT_FAILURE);
               }
            }
            if(sscanf(hm,"%d:%d",&h,&m)!=2) {
               debuga(_("Time period must be MM or MM:SS. Exit\n"));
               exit(EXIT_FAILURE);
            }
            sprintf(hm,"%02d%02d",h,m);
            if(sscanf(hmf,"%d:%d",&h,&m)!=2) {
               debuga(_("Time period must be MM or MM:SS. Exit\n"));
               exit(EXIT_FAILURE);
            }
            sprintf(hmf,"%02d%02d",h,m);
            break;
         }
         case 'u':
            strcpy(us,optarg);
            break;
         case 'v':
            version();
            break;
         case 'w':
            strcpy(tmp,optarg);
            break;
         case 'x':
            debug++;
            break;
         case 'y':
            langcode++;
            break;
         case 'z':
            debugz++;
            break;
         /*case ':':
            debuga(_("Option -%c require an argument\n"),optopt);
            errflg++;
            break;*/
         case '?':
            usage(argv[0]);
            exit(EXIT_FAILURE);
         default:
            abort();
      }
   }

   if (errflg>0) {
      usage(argv[0]);
      exit(2);
   }

   if (optind<argc) {
      for (iarq=optind ; iarq<argc ; iarq++) {
         if (NAccessLog>=MAXLOGS) {
            debuga(_("Too many log files passed on command line.\n"));
            exit(EXIT_FAILURE);
         }
         if (strlen(argv[iarq])>=MAX_LOG_FILELEN) {
            debuga(_("Log file name too long passed on command line: %s\n"),argv[iarq]);
            exit(EXIT_FAILURE);
         }
         strcpy(AccessLog[NAccessLog],argv[iarq]);
         NAccessLog++;
         AccessLogFromCmdLine++;
      }
   }

   if(debug) debuga(_("Init\n"));

   if(ConfigFile[0] == '\0') snprintf(ConfigFile,sizeof(ConfigFile),"%s/sarg.conf",SYSCONFDIR);
   if(access(ConfigFile, R_OK) != 0) {
      debuga(_("Cannot open config file: %s - %s\n"),ConfigFile,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if(access(ConfigFile, R_OK) == 0)
      getconf();

   if(userip) UserIp=true;

   if(dns) Ip2Name=true;

   if(realt) {
      realtime();
      exit(EXIT_SUCCESS);
   }

   if(IndexTree == INDEX_TREE_FILE)
      strcpy(ImageFile,"../images");
   else
      strcpy(ImageFile,"../../../images");

   dataonly=0;
   if(DataFile[0] != '\0')
      dataonly++;

   if(NAccessLog == 0) {
      strcpy(AccessLog[0],"/var/log/squid/access.log");
      NAccessLog++;
   }

   if(output_css) {
      css_content(stdout);
      exit(EXIT_SUCCESS);
   }
   if(split) {
      for (iarq=0 ; iarq<NAccessLog ; iarq++)
         splitlog(AccessLog[iarq], df, dfrom, duntil, convert);
      exit(EXIT_SUCCESS);
   }
   if(convert) {
      for (iarq=0 ; iarq<NAccessLog ; iarq++)
         convlog(AccessLog[iarq], df, dfrom, duntil);
      exit(EXIT_SUCCESS);
   }

   load_excludecodes(ExcludeCodes);

   if(access(PasswdFile, R_OK) == 0) {
      getusers(PasswdFile,debug);
      puser++;
   }

   if(hexclude[0] == '\0')
      strcpy(hexclude,ExcludeHosts);
   if(hexclude[0] != '\0') {
      gethexclude(hexclude,debug);
      fhost=true;
   }

   if(ReportType == 0) {
      ReportType=REPORT_TYPE_TOPUSERS | REPORT_TYPE_TOPSITES | REPORT_TYPE_USERS_SITES |
                 REPORT_TYPE_SITES_USERS | REPORT_TYPE_DATE_TIME | REPORT_TYPE_DENIED |
                 REPORT_TYPE_AUTH_FAILURES | REPORT_TYPE_SITE_USER_TIME_DATE | REPORT_TYPE_DOWNLOADS;
   }

   if(access(ExcludeUsers, R_OK) == 0) {
      getuexclude(ExcludeUsers,debug);
      fuser=true;
   }

   indexonly=0;
   if(fuser) {
      if(is_indexonly())
         indexonly++;
   }
   if(strcmp(ExcludeUsers,"indexonly") == 0) indexonly++;
   if(Index == INDEX_ONLY) indexonly++;

   if(MaxElapsed[0] != '\0') max_elapsed=atol(MaxElapsed);

   if(outdir[0] == '\0') strcpy(outdir,OutputDir);
   strcat(outdir,"/");

   if(uagent[0] == '\0') strcpy(uagent,UserAgentLog);

   if(tmp[0] == '\0') strcpy(tmp,TempDir);
   else strcpy(TempDir,tmp);
   /*
   For historical reasons, the temporary directory is the subdirectory "sarg" of the path
   provided by the user.
   */
   strcat(tmp,"/sarg");

   if (tmp[0]!='\0' && strncmp(outdir,tmp,strlen(tmp))==0) {
      debuga(_("The output directory \"%s\" must be outside of the temporary directory \"%s\"\n"),outdir,tmp);
      exit(EXIT_FAILURE);
   }

   if(df[0] == '\0') strcpy(df,DateFormat);
   else strcpy(DateFormat,df);

   if(df[0] == '\0') {
      strcpy(df,"u");
      strcpy(DateFormat,"u");
   }
   if (df[0]=='w')
      IndexTree=INDEX_TREE_FILE;

   if(email[0] == '\0' && OutputEmail[0] != '\0') strcpy(email,OutputEmail);

   if(email[0] != '\0') {
      my_mkdir(tmp);
      strcpy(outdir,tmp);
      strcat(outdir,"/");
   }

   if(access(tmp, R_OK) == 0) {
      unlinkdir(tmp,1);
   }
   my_mkdir(tmp);
   snprintf(denied_unsort,sizeof(denied_unsort),"%s/denied.log.unsort",tmp);
   snprintf(denied_sort,sizeof(denied_sort),"%s/denied.log",tmp);
   snprintf(authfail_unsort,sizeof(authfail_unsort),"%s/authfail.log.unsort",tmp);

   if(debug) {
      debuga(_("Parameters:\n"));
      debuga(_("          Hostname or IP address (-a) = %s\n"),addr);
      debuga(_("                   Useragent log (-b) = %s\n"),uagent);
      debuga(_("                    Exclude file (-c) = %s\n"),hexclude);
      debuga(_("                 Date from-until (-d) = %s\n"),date);
      debuga(_("   Email address to send reports (-e) = %s\n"),email);
      debuga(_("                     Config file (-f) = %s\n"),ConfigFile);
      if(strcmp(df,"e") == 0)
         debuga(_("                     Date format (-g) = Europe (dd/mm/yyyy)\n"));
      if(strcmp(df,"u") == 0)
         debuga(_("                     Date format (-g) = USA (mm/dd/yyyy)\n"));
      if(strcmp(df,"w") == 0)
         debuga(_("                     Date format (-g) = Sites & Users (yyyy/ww)\n"));
      debuga(_("                       IP report (-i) = %s\n"),(iprel) ? _("Yes") : _("No"));
      for (iarq=0 ; iarq<NAccessLog ; iarq++)
         debuga(_("                       Input log (-l) = %s\n"),AccessLog[iarq]);
      for (iarq=0 ; iarq<NRedirectorLogs ; iarq++)
         debuga(_("                  Redirector log (-L) = %s\n"),RedirectorLogs[iarq]);
      debuga(_("              Resolve IP Address (-n) = %s\n"),(Ip2Name) ? _("Yes") : _("No"));
      debuga(_("                      Output dir (-o) = %s\n"),outdir);
      debuga(_("Use Ip Address instead of userid (-p) = %s\n"),(UserIp) ? _("Yes") : _("No"));
      debuga(_("                   Accessed site (-s) = %s\n"),site);
      debuga(_("                            Time (-t) = %s\n"),hm);
      debuga(_("                            User (-u) = %s\n"),us);
      debuga(_("                   Temporary dir (-w) = %s\n"),tmp);
      debuga(_("                  Debug messages (-x) = %s\n"),(debug) ? _("Yes") : _("No"));
      debuga(_("                Process messages (-z) = %s\n"),(debugz) ? _("Yes") : _("No"));
      debuga("\n");
   }

   if(debugm) {
      printf(_("Parameters:\n"));
      printf(_("          Hostname or IP address (-a) = %s\n"),addr);
      printf(_("                   Useragent log (-b) = %s\n"),uagent);
      printf(_("                    Exclude file (-c) = %s\n"),hexclude);
      printf(_("                 Date from-until (-d) = %s\n"),date);
      printf(_("   Email address to send reports (-e) = %s\n"),email);
      printf(_("                     Config file (-f) = %s\n"),ConfigFile);
      if(strcmp(df,"e") == 0)
         printf(_("                     Date format (-g) = Europe (dd/mm/yyyy)\n"));
      if(strcmp(df,"u") == 0)
         printf(_("                     Date format (-g) = USA (mm/dd/yyyy)\n"));
      if(strcmp(df,"w") == 0)
         printf(_("                     Date format (-g) = Sites & Users (yyyy/ww)\n"));
      printf(_("                       IP report (-i) = %s\n"),(iprel) ? _("Yes") : _("No"));
      for (iarq=0 ; iarq<NAccessLog ; iarq++)
         printf(_("                       Input log (-l) = %s\n"),AccessLog[iarq]);
      for (iarq=0 ; iarq<NRedirectorLogs ; iarq++)
         printf(_("                  Redirector log (-L) = %s\n"),RedirectorLogs[iarq]);
      printf(_("              Resolve IP Address (-n) = %s\n"),(Ip2Name) ? _("Yes") : _("No"));
      printf(_("                      Output dir (-o) = %s\n"),outdir);
      printf(_("Use Ip Address instead of userid (-p) = %s\n"),(UserIp) ? _("Yes") : _("No"));
      printf(_("                   Accessed site (-s) = %s\n"),site);
      printf(_("                            Time (-t) = %s\n"),hm);
      printf(_("                            User (-u) = %s\n"),us);
      printf(_("                   Temporary dir (-w) = %s\n"),tmp);
      printf(_("                  Debug messages (-x) = %s\n"),(debug) ? _("Yes") : _("No"));
      printf(_("                Process messages (-z) = %s\n"),(debugz) ? _("Yes") : _("No"));
      printf(_("sarg version: %s\n"),VERSION);
   }

   if(debug)
      debuga(_("sarg version: %s\n"),VERSION);

#ifdef ENABLE_DOUBLE_CHECK_DATA
   debuga(_("Sarg compiled to report warnings if the output is inconsistent\n"));
#endif

   maxopenfiles=MAX_OPEN_USER_FILES;
#ifdef HAVE_RLIM_T
   if (Ulimit[0] != '\0') {
      struct rlimit rl;
      long l1, l2;
      int rc=0;

#if defined(RLIMIT_NOFILE)
      getrlimit (RLIMIT_NOFILE, &rl);
#elif defined(RLIMIT_OFILE)
      getrlimit (RLIMIT_OFILE, &rl);
#else
#warning "No rlimit resource for the number of open files"
#endif
      l1 = rl.rlim_cur;
      l2 = rl.rlim_max;

      rl.rlim_cur = atol(Ulimit);
      rl.rlim_max = atol(Ulimit);
#if defined(RLIMIT_NOFILE)
      rc=setrlimit (RLIMIT_NOFILE, &rl);
#elif defined(RLIMIT_OFILE)
      rc=setrlimit (RLIMIT_OFILE, &rl);
#else
#warning "No rlimit resource for the number of open files"
#endif
      if(rc == -1) {
            debuga(_("setrlimit error - %s\n"),strerror(errno));
      }

      if(debug)
         debuga("Maximum file descriptor: cur=%ld max=%ld, changed to cur="RLIM_STRING" max="RLIM_STRING,l1,l2,rl.rlim_cur,rl.rlim_max);
   }
#endif

   init_usertab(UserTabFile);

   if ((line=longline_create())==NULL) {
      debuga(_("Not enough memory to read a log file\n"));
      exit(EXIT_FAILURE);
   }

   snprintf(sz_Download_Unsort,sizeof(sz_Download_Unsort),"%s/download.unsort", tmp);

   if(DataFile[0]=='\0') {
      if((ReportType & REPORT_TYPE_DENIED) != 0) {
         if((fp_denied=MY_FOPEN(denied_unsort,"w"))==NULL) {
            debuga(_("(log) Cannot open file: %s - %s\n"),denied_unsort,strerror(errno));
            exit(EXIT_FAILURE);
         }
      }

      if((ReportType & REPORT_TYPE_DENIED) != 0 || (ReportType & REPORT_TYPE_AUTH_FAILURES) != 0) {
         if((fp_authfail=MY_FOPEN(authfail_unsort,"w"))==NULL) {
            debuga(_("(log) Cannot open file: %s - %s\n"),authfail_unsort,strerror(errno));
            exit(EXIT_FAILURE);
         }
      }
   }

   for (iarq=0 ; iarq<NAccessLog ; iarq++) {
      strcpy(arq,AccessLog[iarq]);

      strcpy(arqtt,arq);

      if(strcmp(arq,"-")==0) {
         if(debug)
            debuga(_("Reading access log file: from stdin\n"));
         fp_in=stdin;
         from_stdin=true;
      } else {
         if (date[0]!='\0') {
            if (stat(arq,&logstat)!=0) {
               debuga(_("Cannot get the modification time of input log file %s (%s). Processing it anyway\n"),arq,strerror(errno));
            } else {
               struct tm *logtime=localtime(&logstat.st_mtime);
               if ((logtime->tm_year+1900)*10000+(logtime->tm_mon+1)*100+logtime->tm_mday<dfrom) {
                  debuga(_("Ignoring old log file %s\n"),arq);
                  continue;
               }
            }
         }
         fp_in=decomp(arq,&from_pipe);
         if(fp_in==NULL) {
            debuga(_("(log) Cannot open log file: %s - %s\n"),arq,strerror(errno));
            exit(EXIT_FAILURE);
         }
         if(debug) debuga(_("Reading access log file: %s\n"),arq);
         from_stdin=false;
      }
      ilf=ILF_Unknown;
      download_flag=false;
      // pre-read the file only if we have to show stats
      if(ShowReadStatistics && !from_stdin && !from_pipe) {
         size_t nread,i;
         bool skipcr=false;
         char tmp4[MAXLEN];

         recs1=0UL;
         recs2=0UL;

         while ((nread=fread(tmp4,1,sizeof(tmp4),fp_in))>0) {
            for (i=0 ; i<nread ; i++)
               if (skipcr) {
                  if (tmp4[i]!='\n' && tmp4[i]!='\r') {
                     skipcr=false;
                  }
               } else {
                  if (tmp4[i]=='\n' || tmp4[i]=='\r') {
                     skipcr=true;
                     recs1++;
                  }
               }
         }
         rewind(fp_in);
         printf(_("SARG: Records in file: %lu, reading: %3.2f%%"),recs1,(float) 0);
         putchar('\r');
         fflush( stdout ) ;
      }

      longline_reset(line);

      while ((linebuf=longline_read(fp_in,line))!=NULL) {
         blen=strlen(linebuf);

         if (ilf==ILF_Unknown) {
            if(strncmp(linebuf,"#Software: Mic",14) == 0) {
               fixendofline(linebuf);
               if (debug)
                  debuga(_("Log is from Microsoft ISA: %s\n"),linebuf);
               ilf=ILF_Isa;
               ilf_count[ilf]++;
               continue;
            }

            if(strncmp(linebuf,"*** SARG Log ***",16) == 0) {
               if (getperiod_fromsarglog(arqtt,&period)<0) {
                  debuga(_("The name of the file is invalid: %s\n"),arq);
                  exit(EXIT_FAILURE);
               }
               ilf=ILF_Sarg;
               ilf_count[ilf]++;
               continue;
            }
         }

         if(!fp_log && strcmp(ParsedOutputLog, "no") != 0 && ilf!=ILF_Sarg) {
            if(access(ParsedOutputLog,R_OK) != 0) {
               my_mkdir(ParsedOutputLog);
            }
            sprintf(arq_log,"%s/sarg_temp.log",ParsedOutputLog);
            if((fp_log=MY_FOPEN(arq_log,"w"))==NULL) {
               debuga(_("(log) Cannot open log file: %s - %s\n"),arq_log,strerror(errno));
               exit(EXIT_FAILURE);
            }
            fputs("*** SARG Log ***\n",fp_log);
         }

         recs2++;
         if( ShowReadStatistics && !from_stdin && !from_pipe && --OutputNonZero<=0) {
           double perc = recs2 * 100. / recs1 ;
           printf(_("SARG: Records in file: %lu, reading: %3.2lf%%"),recs2,perc);
           putchar('\r');
           fflush (stdout);
           OutputNonZero = REPORT_EVERY_X_LINES ;
         }
         if(blen < 58) continue;
         if(strstr(linebuf,"HTTP/0.0") != 0) continue;
         if(strstr(linebuf,"logfile turned over") != 0) continue;
         if(linebuf[0] == ' ') continue;

         // exclude_string
         if(ExcludeString[0] != '\0') {
            bool exstring=false;
            getword_start(&gwarea,ExcludeString);
            while(strchr(gwarea.current,':') != 0) {
               if (getword_multisep(val1,sizeof(val1),&gwarea,':')<0) {
                  debuga(_("Maybe you have a broken record or garbage in your exclusion string\n"));
                  exit(EXIT_FAILURE);
               }
               if((str=(char *) strstr(linebuf,val1)) != (char *) NULL ) {
                  exstring=true;
                  break;
               }
            }
            if(!exstring && (str=(char *) strstr(linebuf,gwarea.current)) != (char *) NULL )
                  exstring=true;
            if(exstring) continue;
         }

         totregsl++;
         if(debugm)
            printf("BUF=%s\n",linebuf);

         t=NULL;
         if (ilf==ILF_Squid || ilf==ILF_Common || ilf==ILF_Unknown) {
            getword_start(&gwarea,linebuf);
            if (getword(data,sizeof(data),&gwarea,' ')<0) {
               debuga(_("Maybe you have a broken time in your access.log file\n"));
               exit(EXIT_FAILURE);
            }
            if((str=(char *) strchr(data, '.')) != (char *) NULL && (str=(char *) strchr(str+1, '.')) != (char *) NULL ) {
               strcpy(ip,data);
               strcpy(elap,"0");
               if(squid24) {
                  if (getword(user,sizeof(user),&gwarea,' ')<0 || getword_skip(255,&gwarea,' ')<0) {
                     debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
               } else {
                  if (getword_skip(255,&gwarea,' ')<0 || getword(user,sizeof(user),&gwarea,' ')<0) {
                     debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
               }
               if (getword(data,sizeof(data),&gwarea,']')<0 || getword_skip(MAXLEN,&gwarea,'"')<0 ||
                     getword(fun,sizeof(fun),&gwarea,' ')<0) {
                  debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword_ptr(linebuf,&url,&gwarea,' ')<0) {
                  debuga(_("Maybe you have a broken url in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword_skip(MAXLEN,&gwarea,' ')<0) {
                  debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword(code2,sizeof(code2),&gwarea,' ')<0) {
                  debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword(tam,sizeof(tam),&gwarea,' ')<0) {
                  debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if((str=(char *) strchr(gwarea.current, ' ')) != (char *) NULL ) {
                  if (getword(code,sizeof(code),&gwarea,' ')<0) {
                     debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
               } else {
                  if (getword(code,sizeof(code),&gwarea,'\0')<0) {
                     debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
               }

               if ((str = strchr(code, ':')) != NULL)
                  *str = '/';

               if(strcmp(tam,"\0") == 0)
                  strcpy(tam,"0");

               ilf=ILF_Common;
               ilf_count[ilf]++;

               getword_start(&gwarea,data+1);
               if (getword_multisep(data,sizeof(data),&gwarea,':')<0){
                  debuga(_("Maybe you have a broken date in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword_multisep(hora,sizeof(hora),&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken date in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               getword_start(&gwarea,data);
               if (getword_atoll(&iday,&gwarea,'/')<0){
                  debuga(_("Maybe you have a broken date in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword(mes,sizeof(mes),&gwarea,'/')<0){
                  debuga(_("Maybe you have a broken date in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword_atoll(&iyear,&gwarea,'/')<0){
                  debuga(_("Maybe you have a broken date in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }

               imonth=month2num(mes)+1;
               idata=builddia(iday,imonth,iyear);
               computedate(iyear,imonth,iday,&tt);
               t=&tt;
            }

            if(ilf==ILF_Unknown || ilf==ILF_Squid) {
               if (getword(elap,sizeof(elap),&gwarea,' ')<0) {
                  debuga(_("Maybe you have a broken elapsed time in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               while(strcmp(elap,"") == 0 && gwarea.current[0] != '\0')
                  if (getword(elap,sizeof(elap),&gwarea,' ')<0) {
                     debuga(_("Maybe you have a broken elapsed time in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
               if(strlen(elap) < 1) continue;
               if (getword(ip,sizeof(ip),&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken client IP address in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword(code,sizeof(code),&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken result code in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword(tam,sizeof(tam),&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken amount of data in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword(fun,sizeof(fun),&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken request method in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword_ptr(linebuf,&url,&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken url in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (getword(user,sizeof(user),&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken user ID in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               ilf=ILF_Squid;
               ilf_count[ilf]++;

               tnum=atoi(data);
               t=localtime(&tnum);
               if (t == NULL) {
                  debuga(_("Cannot convert the timestamp from the squid log file\n"));
                  exit(EXIT_FAILURE);
               }

               strftime(tbuf2, sizeof(tbuf2), "%H%M", t);

               idata=(t->tm_year+1900)*10000+(t->tm_mon+1)*100+t->tm_mday;
            }
         }
         if (ilf==ILF_Sarg) {
            getword_start(&gwarea,linebuf);
            if (getword(data,sizeof(data),&gwarea,'\t')<0){
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword(hora,sizeof(hora),&gwarea,'\t')<0) {
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword(user,sizeof(user),&gwarea,'\t')<0) {
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword(ip,sizeof(ip),&gwarea,'\t')<0) {
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword_ptr(linebuf,&url,&gwarea,'\t')<0){
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword(tam,sizeof(tam),&gwarea,'\t')<0){
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword(code,sizeof(code),&gwarea,'\t')<0){
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword(elap,sizeof(elap),&gwarea,'\t')<0){
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword(smartfilter,sizeof(smartfilter),&gwarea,'\0')<0){
               debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            getword_start(&gwarea,data);
            if (getword_atoll(&iday,&gwarea,'/')<0 || iday<1 || iday>31){
               debuga(_("Maybe you have a broken date in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword_atoll(&imonth,&gwarea,'/')<0 || imonth<1 || imonth>12){
               debuga(_("Maybe you have a broken date in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword_atoll(&iyear,&gwarea,'\0')<0){
               debuga(_("Maybe you have a broken date in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            idata=builddia(iday,imonth,iyear);
            computedate(iyear,imonth,iday,&tt);
            t=&tt;
         }
         if (ilf==ILF_Isa) {
            if (linebuf[0] == '#') {
               int ncols,cols[ISACOL_Last];

               fixendofline(linebuf);
               getword_start(&gwarea,linebuf);
               // remove the #Fields: column at the beginning of the line
               if (getword_skip(1000,&gwarea,' ')<0){
                  debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               for (ncols=0 ; ncols<ISACOL_Last ; ncols++) cols[ncols]=-1;
               ncols=0;
               while(gwarea.current[0] != '\0') {
                  if (getword(val1,sizeof(val1),&gwarea,'\t')<0){
                     debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  if(strcmp(val1,"c-ip") == 0) cols[ISACOL_Ip]=ncols;
                  if(strcmp(val1,"cs-username") == 0) cols[ISACOL_UserName]=ncols;
                  if(strcmp(val1,"date") == 0) cols[ISACOL_Date]=ncols;
                  if(strcmp(val1,"time") == 0) cols[ISACOL_Time]=ncols;
                  if(strcmp(val1,"time-taken") == 0) cols[ISACOL_TimeTaken]=ncols;
                  if(strcmp(val1,"sc-bytes") == 0) cols[ISACOL_Bytes]=ncols;
                  if(strcmp(val1,"cs-uri") == 0) cols[ISACOL_Uri]=ncols;
                  if(strcmp(val1,"sc-status") == 0) cols[ISACOL_Status]=ncols;
                  ncols++;
               }
               if (cols[ISACOL_Ip]>=0) {
                  isa_ncols=ncols;
                  for (ncols=0 ; ncols<ISACOL_Last ; ncols++)
                     isa_cols[ncols]=cols[ncols];
               }
               continue;
            }
            if (!isa_ncols) continue;
            getword_start(&gwarea,linebuf);
            for (x=0 ; x<isa_ncols ; x++) {
               if (getword_ptr(linebuf,&str,&gwarea,'\t')<0) {
                  debuga(_("Maybe you have a broken record or garbage in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               if (x==isa_cols[ISACOL_Ip]) {
                  if (strlen(str)>=sizeof(ip)) {
                     debuga(_("Maybe you have a broken IP in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(ip,str);
               } else if (x==isa_cols[ISACOL_UserName]) {
                  if (strlen(str)>=sizeof(user)) {
                     debuga(_("Maybe you have a broken user ID in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(user,str);
               } else if (x==isa_cols[ISACOL_Date]) {
                  if (strlen(str)>=sizeof(data)) {
                     debuga(_("Maybe you have a broken date in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(data,str);
               } else if (x==isa_cols[ISACOL_Time]) {
                  if (strlen(str)>=sizeof(hora)) {
                     debuga(_("Maybe you have a broken time in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(hora,str);
               } else if (x==isa_cols[ISACOL_TimeTaken]) {
                  if (strlen(str)>=sizeof(elap)) {
                     debuga(_("Maybe you have a broken download duration in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(elap,str);
               } else if (x==isa_cols[ISACOL_Bytes]) {
                  if (strlen(str)>=sizeof(tam)) {
                     debuga(_("Maybe you have a broken download size in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(tam,str);
               } else if (x==isa_cols[ISACOL_Uri]) {
                  url=str;
               } else if (x==isa_cols[ISACOL_Status]) {
                  if (strlen(str)>=sizeof(code)) {
                     debuga(_("Maybe you have a broken access code in your %s file\n"),arq);
                     exit(EXIT_FAILURE);
                  }
                  strcpy(code,str);
               }
            }

            if(strcmp(code,"401") == 0 || strcmp(code,"403") == 0 || strcmp(code,"407") == 0) {
               sprintf(val1,"DENIED/%s",code);
               strcpy(code,val1);
            }
            getword_start(&gwarea,data);
            if (getword_atoll(&iyear,&gwarea,'-')<0){
               debuga(_("Maybe you have a broken year in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword_atoll(&imonth,&gwarea,'-')<0){
               debuga(_("Maybe you have a broken month in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }
            if (getword_atoll(&iday,&gwarea,'\0')<0){
               debuga(_("Maybe you have a broken day in your %s file\n"),arq);
               exit(EXIT_FAILURE);
            }

            idata=builddia(iday,imonth,iyear);
            computedate(iyear,imonth,iday,&tt);
            t=&tt;
         }
         if (t==NULL) {
            debuga(_("Unknown input log file format\n"));
            break;
         }

         strftime(dia, sizeof(dia), "%d/%m/%Y", t);
         snprintf(hora,sizeof(hora),"%02d:%02d:%02d",t->tm_hour,t->tm_min,t->tm_sec);

         if(debugm)
            printf("DATE=%s IDATA=%d DFROM=%d DUNTIL=%d\n",date,idata,dfrom,duntil);

         if(date[0] != '\0'){
            if(idata < dfrom || idata > duntil) continue;
         }

         // Record only hours usage which is required
         if (t) {
            if( bsearch( &( t -> tm_wday ), weekdays.list, weekdays.len,
                                             sizeof( int ), compar ) == NULL )
               continue;

            if( bsearch( &( t -> tm_hour ), hours.list, hours.len,
                                             sizeof( int ), compar ) == NULL )
               continue;
         }


         if(strlen(user) > MAX_USER_LEN) {
            if (debugm) printf(_("User ID too long: %s\n"),user);
            totregsx++;
            continue;
         }

         // include_users
         if(IncludeUsers[0] != '\0') {
            sprintf(val1,":%s:",user);
            if((str=(char *) strstr(IncludeUsers,val1)) == (char *) NULL )
               continue;
         }

         if(vercode(code)) {
            if (debugm) printf(_("Excluded code: %s\n"),code);
            totregsx++;
            continue;
         }

         if(testvaliduserchar(user))
            continue;

#if 0
         if((str = strstr(user,"%20")) != NULL) {
            /*
            This is a patch introduced to solve bug #1624251 reported at sourceforge but
            the side effect is to truncate the name at the first space and merge the reports
            of people whose name is identical up to the first space.

            The old code used to truncate the user name at the first % if a %20 was
            found anywhere in the string. That means the string could be truncated
            at the wrong place if another % occured before the %20. This new code should
            avoid that problem and only truncate at the space. There is no bug
            report indicating that anybody noticed this.
            */
            *str='\0';
         }

         /*
         Code prior to 2.2.7 used to replace any %xx by a dot as long as a %5c was
         found in the user name.
         */
         while((str = strstr(user,"%5c")) != NULL) {
            *str='.';
            for (x=3 ; str[x] ; x++) str[x-2]=str[x];
         }
#endif

         urly=url;

         if(ilf!=ILF_Sarg) {
            /*
            The full URL is not saved in sarg log. There is no point in testing the URL to detect
            a downloaded file.
            */
            download_flag=is_download_suffix(url);
            if (download_flag) {
               download_url=url;
               download_count++;
            }
         } else
            download_flag=false;

         // remove any protocol:// at the beginning of the URL
         if ((str = strchr(url,'/')) != NULL && str[1] == '/') {
            int i;

            str+=2;
            for (i=0 ; str[i] ; i++)
               url[i]=str[i];
            url[i]='\0';
         }

         if(!LongUrl) {
            url_hostname(url,hostname,sizeof(hostname));
            url=hostname;
         }

         if(url[0] == '\0') continue;

         if(addr[0] != '\0'){
            if(strcmp(addr,ip)!=0) continue;
         }
         if(fhost) {
            if(!vhexclude(url)) {
               if (debugm) printf(_("Excluded site: %s\n"),url);
               totregsx++;
               continue;
            }
         }

         if(hm[0] != '\0') {
            hmr[0]='\0';
            chm++;
            getword_start(&gwarea,hora);
            while(chm) {
               if (getword_multisep(warea,sizeof(warea),&gwarea,':')<0){
                  debuga(_("Maybe you have a broken time in your %s file\n"),arq);
                  exit(EXIT_FAILURE);
               }
               strncat(hmr,warea,2);
               chm--;
            }
            strncat(hmr,gwarea.current,2);

            if(atoi(hmr) < atoi(hm) || atoi(hmr) > atoi(hmf)) continue;
         }

         if(site[0] != '\0'){
            if(strstr(url,site)==0) continue;
         }

         if(UserIp) {
            strcpy(user,ip);
            id_is_ip=true;
         } else {
            id_is_ip=false;
            if(strcmp(user,"-") == 0 || strcmp(user," ") == 0 || strcmp(user,"") == 0) {
               if(RecordsWithoutUser == RECORDWITHOUTUSER_IP) {
                  strcpy(user,ip);
                  id_is_ip=true;
               }
               if(RecordsWithoutUser == RECORDWITHOUTUSER_IGNORE)
                  continue;
               if(RecordsWithoutUser == RECORDWITHOUTUSER_EVERYBODY)
                  strcpy(user,"everybody");
            } else {
               strlow(user);
               if(NtlmUserFormat == NTLMUSERFORMAT_USER) {
                  if((str = strchr(user,'_')) != 0) {
                     strcpy(warea,str+1);
                     strcpy(user,warea);
                  }
                  if((str = strchr(user,'+')) != 0) {
                     strcpy(warea,str+1);
                     strcpy(user,warea);
                  }
               }
            }
         }

         if(us[0] != '\0'){
            if(strcmp(user,us)!=0) continue;
         }

         if(puser) {
            sprintf(wuser,":%s:",user);
            if(strstr(userfile, wuser) == 0)
               continue;
         }

         if(fuser) {
            if(!vuexclude(user)) {
               if (debugm) printf(_("Excluded user: %s\n"),user);
               totregsx++;
               continue;
            }
         }

         if(strcmp(user,"-") ==0 || strcmp(user," ") ==0 || strcmp(user,"") ==0 || strcmp(user,":") ==0)
            continue;

         if(max_elapsed) {
            if(atol(elap)>max_elapsed) {
               elap[0]='0';
               elap[1]='\0';
            }
         }

         if((str=(char *) strstr(linebuf, "[SmartFilter:")) != (char *) NULL ) {
            fixendofline(str);
            sprintf(smartfilter,"\"%s\"",str+1);
         } else sprintf(smartfilter,"\"\"");

         nopen=0;
         prev_ufile=NULL;
         for (ufile=first_user_file ; ufile && strcmp(user,ufile->user->id)!=0 ; ufile=ufile->next) {
            prev_ufile=ufile;
            if (ufile->file) nopen++;
         }
         if (!ufile) {
            ufile=malloc(sizeof(*ufile));
            if (!ufile) {
               debuga(_("Not enough memory to store the user %s\n"),user);
               exit(EXIT_FAILURE);
            }
            memset(ufile,0,sizeof(*ufile));
            ufile->next=first_user_file;
            first_user_file=ufile;
            uinfo=userinfo_create(user);
            ufile->user=uinfo;
            uinfo->id_is_ip=id_is_ip;
         } else {
            if (prev_ufile) {
               prev_ufile->next=ufile->next;
               ufile->next=first_user_file;
               first_user_file=ufile;
            }
         }

         if (ufile->file==NULL) {
            if (nopen>=maxopenfiles) {
               x=0;
               for (ufile1=first_user_file ; ufile1 ; ufile1=ufile1->next) {
                  if (ufile1->file!=NULL) {
                     if (x>=maxopenfiles) {
                        if (fclose(ufile1->file)==EOF) {
                           debuga(_("Failed to close the log file of user %s - %s\n"),ufile1->user->id,strerror(errno));
                           exit(EXIT_FAILURE);
                        }
                        ufile1->file=NULL;
                     }
                     x++;
                  }
               }
            }
            if (snprintf (tmp3, sizeof(tmp3), "%s/%s.unsort", tmp, ufile->user->filename)>=sizeof(tmp3)) {
               debuga(_("Temporary user file name too long: %s/%s.unsort\n"), tmp, ufile->user->filename);
               exit(EXIT_FAILURE);
            }
            if ((ufile->file = MY_FOPEN (tmp3, "a")) == NULL) {
               debuga(_("(log) Cannot open temporary file: %s - %s\n"), tmp3, strerror(errno));
               exit (1);
            }
         }

         /*if ( strcmp ( user , sz_Last_User ) != 0 ) {
            if ( fp_Write_User )
               fclose( fp_Write_User ) ;
            sprintf (tmp3, "%s/%s.unsort", tmp, user);

            if ((fp_Write_User = MY_FOPEN (tmp3, "a")) == NULL) {
               fprintf (stderr, "%s: (log) %s: %s - %s\n", argv[0], _("Cannot open temporary file"), tmp3, strerror(errno));
               exit (1);
            }
            strcpy( sz_Last_User , user ) ;
         }*/
         if (fprintf(ufile->file, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",dia,hora,ip,url,tam,code,elap,smartfilter)<=0) {
            debuga(_("Write error in the log file of user %s\n"),user);
            exit(EXIT_FAILURE);
         }

         if(fp_log && ilf!=ILF_Sarg)
            fprintf(fp_log, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",dia,hora,user,ip,url,tam,code,elap,smartfilter);

         totregsg++;

         if(!dataonly && download_flag && download_url && strstr(code,"DENIED") == 0) {
            ndownload = 1;

            if ( ! fp_Download_Unsort ) {
               if ((fp_Download_Unsort = MY_FOPEN ( sz_Download_Unsort, "a")) == NULL) {
                  debuga(_("(log) Cannot open temporary file: %s - %s\n"),sz_Download_Unsort, strerror(errno));
                  exit (1);
               }
            }
            fprintf(fp_Download_Unsort,"%s\t%s\t%s\t%s\t%s\n",dia,hora,user,ip,download_url);
         }

         if((ReportType & REPORT_TYPE_DENIED) != 0 || (ReportType & REPORT_TYPE_AUTH_FAILURES) != 0) {
            if(fp_denied && strstr(code,"DENIED/403") != 0) {
               fprintf(fp_denied, "%s\t%s\t%s\t%s\t%s\n",dia,hora,user,ip,urly);
               denied_count++;
            }
            if(fp_authfail && (strstr(code,"DENIED/401") != 0 || strstr(code,"DENIED/407") != 0)) {
               fprintf(fp_authfail, "%s\t%s\t%s\t%s\t%s\n",dia,hora,user,ip,urly);
               authfail_count++;
            }
         }

         if (ilf!=ILF_Sarg) {
            if(!totper || idata<mindate){
               mindate=idata;
               memcpy(&period.start,t,sizeof(*t));
               strcpy(start_hour,tbuf2);
            }
            if (!totper || idata>maxdate) {
               maxdate=idata;
               memcpy(&period.end,t,sizeof(*t));
            }
            totper=true;
         }

         if(debugm){
            printf("IP=\t%s\n",ip);
            printf("USER=\t%s\n",user);
            printf("ELAP=\t%s\n",elap);
            printf("DATE=\t%s\n",dia);
            printf("TIME=\t%s\n",hora);
            printf("FUNC=\t%s\n",fun);
            printf("URL=\t%s\n",url);
            printf("CODE=\t%s\n",code);
            printf("LEN=\t%s\n",tam);
         }
      }
      if (!from_stdin) {
         if (from_pipe)
            pclose(fp_in);
         else {
            fclose(fp_in);
            if( ShowReadStatistics )
               printf(_("SARG: Records in file: %lu, reading: %3.2f%%\n"),recs1, (float) 100 );
         }
      }
   }

   if (debug)
      debuga(_("   Records read: %ld, written: %ld, excluded: %ld\n"),totregsl,totregsg,totregsx);

   longline_destroy(&line);
   if ( fp_Download_Unsort )
     fclose (fp_Download_Unsort);

   for (ufile=first_user_file ; ufile ; ufile=ufile1) {
      ufile1=ufile->next;
      if (ufile->file!=NULL) fclose(ufile->file);
      free(ufile);
   }

   free_download();
   free_excludecodes();
   free_exclude();

   if(debug) {
      int totalcount=0;

      for (ilf=0 ; ilf<ILF_Last ; ilf++) totalcount+=ilf_count[ilf];

      if(ilf_count[ILF_Common]>0 && ilf_count[ILF_Squid]>0)
         debuga(_("Log with mixed records format (squid and common log)\n"));

      if(ilf_count[ILF_Common]>0 && ilf_count[ILF_Squid]==0)
         debuga(_("Common log format\n"));

      if(ilf_count[ILF_Common]==0 && ilf_count[ILF_Squid]>0)
         debuga(_("Squid log format\n"));

      if(ilf_count[ILF_Sarg]>0)
         debuga(_("Sarg log format\n"));

      if(totalcount==0 && totregsg)
         debuga(_("Log with invalid format\n"));
   }

   if(!totregsg){
      debuga(_("No records found\n"));
      debuga(_("End\n"));
      if(fp_denied) fclose(fp_denied);
      if(fp_authfail) fclose(fp_authfail);
      userinfo_free();
      if(userfile) free(userfile);
      close_usertab();
      exit(EXIT_SUCCESS);
   }

   if (date[0]!='\0') {
      char date0[30], date1[30];

      strftime(date0,sizeof(date0),"%d/%m/%Y",&period.start);
      strftime(date1,sizeof(date1),"%d/%m/%Y",&period.end);
      debuga(_("Period covered by log files: %s-%s\n"),date0,date1);
      getperiod_fromrange(&period,dfrom,duntil);
   }
   if (getperiod_buildtext(&period)<0) {
      debuga(_("Failed to build the string representation of the date range\n"));
      exit(EXIT_FAILURE);
   }

   if(debugz){
      debugaz("data",dia);
      debugaz("period",period.text);
   }

   if(debug)
      debuga(_("Period: %s\n"),period.text);

//   fclose(fp_ou);
   if(fp_denied)
      fclose(fp_denied);
   if(fp_authfail)
      fclose(fp_authfail);

   if(fp_log != NULL) {
      fclose(fp_log);
      strcpy(end_hour,tbuf2);
      strftime(val2,sizeof(val2),"%d%m%Y",&period.start);
      strftime(val1,sizeof(val1),"%d%m%Y",&period.end);
      sprintf(val4,"%s/sarg-%s_%s-%s_%s.log",ParsedOutputLog,val2,start_hour,val1,end_hour);
      if (rename(arq_log,val4)) {
         debuga(_("failed to rename %s to %s - %s\n"),arq_log,val4,strerror(errno));
      } else {
         strcpy(arq_log,val4);

         if(strcmp(ParsedOutputLogCompress,"nocompress") != 0 && ParsedOutputLogCompress[0] != '\0') {
            /*
            No double quotes around ParsedOutputLogCompress because it may contain command line options. If double quotes are
            necessary around the command name, put them in the configuration file.
            */
            sprintf(val1,"%s \"%s\"",ParsedOutputLogCompress,arq_log);
            cstatus=system(val1);
            if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
               debuga(_("command return status %d\n"),WEXITSTATUS(cstatus));
               debuga(_("command: %s\n"),val1);
               exit(EXIT_FAILURE);
            }
         }
      }
      if(debug)
         debuga(_("Sarg parsed log saved as %s\n"),arq_log);
   }

   if(DataFile[0] == '\0' && (ReportType & REPORT_TYPE_DENIED) != 0) {
      sprintf(csort,"sort -T \"%s\" -k 3,3 -k 5,5 -o \"%s\" \"%s\"",tmp,denied_sort,denied_unsort);
      cstatus=system(csort);
      if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus)) {
         debuga(_("sort command return status %d\n"),WEXITSTATUS(cstatus));
         debuga(_("sort command: %s\n"),csort);
         exit(EXIT_FAILURE);
      }
      unlink(denied_unsort);
   }

   sort_users_log(tmp, debug);

   if(DataFile[0] != '\0')
      data_file(tmp);
   else
      gerarel();

   if((ReportType & REPORT_TYPE_DENIED) != 0)
      unlink(denied_sort);

   if(strcmp(tmp,"/tmp") != 0) {
      unlinkdir(tmp,0);
   }

   userinfo_free();
   if(userfile)
      free(userfile);
   close_usertab();

   if(debug)
      debuga(_("End\n"));

   exit(EXIT_SUCCESS);

}


static void getusers(const char *pwdfile, int debug)
{

   FILE *fp_usr;
   char buf[255];
   char *str;
   long int nreg=0;

   if(debug)
      debuga(_("Loading password file from %s\n"),pwdfile);

   if ((fp_usr = fopen(pwdfile, "r")) == NULL) {
      debuga(_("(getusers) Cannot open file %s - %s\n"),pwdfile,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (fseek(fp_usr, 0, SEEK_END)==-1) {
      debuga(_("Failed to move till the end of the users file %s: %s\n"),pwdfile,strerror(errno));
      exit(EXIT_FAILURE);
   }
   nreg = ftell(fp_usr);
   if (nreg<0) {
      debuga(_("Cannot get the size of file %s\n"),pwdfile);
      exit(EXIT_FAILURE);
   }
   nreg = nreg+5000;
   if (fseek(fp_usr, 0, SEEK_SET)==-1) {
      debuga(_("Failed to rewind the users file %s: %s\n"),pwdfile,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if((userfile=(char *) malloc(nreg))==NULL){
      debuga(_("malloc error (%ld)\n"),nreg);
      exit(EXIT_FAILURE);
   }

   bzero(userfile,nreg);
   strcpy(userfile,":");

   while(fgets(buf,sizeof(buf),fp_usr)!=NULL) {
      str=strchr(buf,':');
      if (!str) {
         debuga(_("You have an invalid user in your %s file\n"),pwdfile);
         exit(EXIT_FAILURE);
      }
      str[1]='\0';
      strcat(userfile,buf);
   }

   fclose(fp_usr);

   return;
}
