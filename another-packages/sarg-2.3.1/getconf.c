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

#define SET_LIST(list) list,sizeof(list)/sizeof(list[0])

extern numlist hours, weekdays;

struct param_list
{
   //! The name of the value of the parameter.
   const char *name;
   //! The bit to set if the value is found.
   unsigned long int value;
   //! The value is invalid if any bit of this mask is set in the parameter.
   unsigned long int exclude;
};

struct sort_list
{
   //! The name of the value of the parameter.
   const char *name;
   //! The bit to set if the value is found.
   unsigned long int value;
};

static struct param_list report_type_values[]=
{
   {"users_sites",REPORT_TYPE_USERS_SITES,0},
   {"topusers",REPORT_TYPE_TOPUSERS,0},
   {"topsites",REPORT_TYPE_TOPSITES,0},
   {"sites_users",REPORT_TYPE_SITES_USERS,0},
   {"date_time",REPORT_TYPE_DATE_TIME,0},
   {"denied",REPORT_TYPE_DENIED,0},
   {"auth_failures",REPORT_TYPE_AUTH_FAILURES,0},
   {"site_user_time_date",REPORT_TYPE_SITE_USER_TIME_DATE,0},
   {"downloads",REPORT_TYPE_DOWNLOADS,0},
};

static struct param_list data_field_values[]=
{
   {"user",DATA_FIELD_USER,0},
   {"date",DATA_FIELD_DATE,0},
   {"time",DATA_FIELD_TIME,0},
   {"url",DATA_FIELD_URL,0},
   {"connect",DATA_FIELD_CONNECT,0},
   {"bytes",DATA_FIELD_BYTES,0},
   {"in_cache",DATA_FIELD_IN_CACHE,0},
   {"out_cache",DATA_FIELD_OUT_CACHE,0},
   {"elapsed",DATA_FIELD_ELAPSED,0},
};

static struct param_list topuserfields_values[]=
{
   {"NUM",TOPUSERFIELDS_NUM,0},
   {"DATE_TIME",TOPUSERFIELDS_DATE_TIME,0},
   {"USERID",TOPUSERFIELDS_USERID,0},
   {"CONNECT",TOPUSERFIELDS_CONNECT,0},
   {"BYTES",TOPUSERFIELDS_BYTES,0},
   {"%BYTES",TOPUSERFIELDS_SETYB,0},
   {"SETYB",TOPUSERFIELDS_SETYB,0},
   {"IN-CACHE-OUT",TOPUSERFIELDS_IN_CACHE_OUT,0},
   {"USED_TIME",TOPUSERFIELDS_USED_TIME,0},
   {"MILISEC",TOPUSERFIELDS_MILISEC,0},
   {"%TIME",TOPUSERFIELDS_PTIME,0},
   {"TOTAL",TOPUSERFIELDS_TOTAL,0},
   {"AVERAGE",TOPUSERFIELDS_AVERAGE,0},
};

static struct param_list userreportfields_values[]=
{
   {"CONNECT",USERREPORTFIELDS_CONNECT,0},
   {"BYTES",USERREPORTFIELDS_BYTES,0},
   {"%BYTES",USERREPORTFIELDS_SETYB,0},
   {"SETYB",USERREPORTFIELDS_SETYB,0},
   {"IN-CACHE-OUT",USERREPORTFIELDS_IN_CACHE_OUT,0},
   {"USED_TIME",USERREPORTFIELDS_USED_TIME,0},
   {"MILISEC",USERREPORTFIELDS_MILISEC,0},
   {"%TIME",USERREPORTFIELDS_PTIME,0},
   {"TOTAL",USERREPORTFIELDS_TOTAL,0},
   {"AVERAGE",USERREPORTFIELDS_AVERAGE,0},
};

static struct param_list index_values[]=
{
   {"yes",INDEX_YES,~INDEX_YES},
   {"no",INDEX_NO,~INDEX_NO},
   {"only",INDEX_ONLY,~INDEX_ONLY},
};

static struct param_list index_tree_values[]=
{
   {"date",INDEX_TREE_DATE,~INDEX_TREE_DATE},
   {"file",INDEX_TREE_FILE,~INDEX_TREE_FILE},
};

static struct param_list ntml_userformat_values[]=
{
   {"user",NTLMUSERFORMAT_USER,~NTLMUSERFORMAT_USER},
   {"domainname+username",NTLMUSERFORMAT_DOMAINUSER,~NTLMUSERFORMAT_DOMAINUSER},
};

static struct param_list recnouser_values[]=
{
   {"ip",RECORDWITHOUTUSER_IP,~RECORDWITHOUTUSER_IP},
   {"ignore",RECORDWITHOUTUSER_IGNORE,~RECORDWITHOUTUSER_IGNORE},
   {"everybody",RECORDWITHOUTUSER_EVERYBODY,~RECORDWITHOUTUSER_EVERYBODY},
};

static struct param_list datafileurl_values[]=
{
   {"ip",DATAFILEURL_IP,~DATAFILEURL_IP},
   {"name",DATAFILEURL_NAME,~DATAFILEURL_NAME},
};

static struct param_list displayvalue_values[]=
{
   {"bytes",DISPLAY_BYTES,~DISPLAY_BYTES},
   {"abbreviation",DISPLAY_ABBREV,~DISPLAY_ABBREV},
};

static struct param_list datetime_values[]=
{
   {"elap",DATETIME_ELAP,0},
   {"bytes",DATETIME_BYTE,0},
};

static struct param_list realtime_unauth_values[]=
{
   {"show",REALTIME_UNAUTH_REC_SHOW,~REALTIME_UNAUTH_REC_SHOW},
   {"ignore",REALTIME_UNAUTH_REC_IGNORE,~REALTIME_UNAUTH_REC_IGNORE},
};

struct sort_list topuser_sort[]=
{
   {"BYTES",TOPUSER_SORT_BYTES},
   {"USER",TOPUSER_SORT_USER},
   {"CONNECT",TOPUSER_SORT_CONNECT},
   {"TIME",TOPUSER_SORT_TIME},
};

struct sort_list topsite_sort[]=
{
   {"BYTES",TOPSITE_SORT_BYTES},
   {"CONNECT",TOPSITE_SORT_CONNECT},
};

struct sort_list user_sort[]=
{
   {"BYTES",USER_SORT_BYTES},
   {"SITE",USER_SORT_SITE},
   {"CONNECT",USER_SORT_CONNECT},
   {"TIME",USER_SORT_TIME},
};

static int is_param(const char *param,const char *buf)
{
   int plen;

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   return(1);
}

static int getparam_string(const char *param,char *buf,char *value,int value_size)
{
   int plen;

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   while (*buf && (unsigned char)*buf<=' ') buf++;

   if (strlen(buf)>=value_size) {
      debuga(_("The string value of parameter \"%s\" is too long\n"),param);
      exit(EXIT_FAILURE);
   }
   strcpy(value,buf);
   fixnone(value);
   return(1);
}

static int getparam_quoted(const char *param,char *buf,char *value,int value_size)
{
   int plen;
   int i;

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   while (*buf && (unsigned char)*buf<=' ') buf++;

   if (*buf != '\"') {
      debuga(_("Missing double quote after parameter \"%s\"\n"),param);
      exit(EXIT_FAILURE);
   }
   buf++;

   value_size--;
   for (i=0 ; i<value_size && *buf && *buf!='\"' ; i++) {
      value[i]=*buf++;
   }
   value[i]='\0';

   if (*buf != '\"') {
      debuga(_("Missing double quote after parameter \"%s\" or value is more than %d bytes long\n"),param,value_size);
      exit(EXIT_FAILURE);
   }
   fixnone(value);
   return(1);
}

static int getparam_2words(const char *param,char *buf,char *word1,int word1_size,char *word2,int word2_size)
{
   int plen;
   int i;

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   while (*buf && (unsigned char)*buf<=' ') buf++;

   for (i=0 ; i<word1_size && *buf && (unsigned char)*buf>' ' ; i++)
      word1[i]=*buf++;
   if (i>=word1_size) {
      debuga(_("The first word of parameter \"%s\" is more than %d bytes long\n"),param,word1_size-1);
      exit(EXIT_FAILURE);
   }
   if (*buf!=' ') {
      debuga(_("Missing second word for parameter \"%s\"\n"),param);
      exit(EXIT_FAILURE);
   }
   word1[i]=0;

   while (*buf && (unsigned char)*buf<=' ') buf++;

   for (i=0 ; i<word2_size && *buf && (unsigned char)*buf>' ' ; i++)
      word2[i]=*buf++;
   if (i>=word2_size) {
      debuga(_("The second word of parameter \"%s\" is more than %d bytes long\n"),param,word2_size-1);
      exit(EXIT_FAILURE);
   }
   word2[i]=0;

   fixnone(word1);
   fixnone(word2);
   return(1);
}

static int getparam_int(const char *param,char *buf,int *value)
{
   int plen;
   int next;

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   while (*buf && (unsigned char)*buf<=' ') buf++;

   next=0;
   if (sscanf(buf,"%d%n",value,&next) != 1 || (unsigned char)buf[next] > ' ') {
      debuga(_("The integer value of parameter \"%s\" is invalid\n"),param);
      exit(EXIT_FAILURE);
   }
   return(1);
}

static int getparam_bool(const char *param,char *buf,bool *value)
{
   int plen;
   int i;
   const char *bool_str="yes,true,on,1";

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   while (*buf && (unsigned char)*buf<=' ') buf++;

   *value=false;
   for ( ; *bool_str ; bool_str+=i) {
      for (i=0 ; bool_str[i] && bool_str[i]!=',' ; i++);
      if (strncasecmp(bool_str,buf,i)==0) {
         *value=true;
         break;
      }
      if (bool_str[i]==',') i++;
   }
   return(1);
}

static int getparam_list(const char *param,struct param_list *options,int noptions,char *buf,unsigned long int *value)
{
   int plen;
   char *str;
   int i;

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   while (*buf && (unsigned char)*buf<=' ') buf++;

   *value=0UL;
   while (*buf) {
      str=buf;
      while (*str && (unsigned char)*str>' ' && *str!=';') str++;
      if (*str) {
         *str++='\0';
         while (*str && ((unsigned char)*str<=' ' || *str==';')) str++;
      }
      for (i=0 ; i<noptions && strcasecmp(buf,options[i].name) ; i++);
      if (i>=noptions) {
         debuga(_("Unknown value \"%s\" for parameter \"%s\"\n"),buf,param);
         exit(EXIT_FAILURE);
      }
      if ((*value & options[i].exclude)!=0) {
         debuga(_("Value \"%s\" conflicts with other selected values for parameter \"%s\"\n"),buf,param);
         exit(EXIT_FAILURE);
      }
      *value|=options[i].value;
      buf=str;
   }
   return(1);
}

static int getparam_sort(const char *param,struct sort_list *options,int noptions,char *buf,unsigned long int *value)
{
   int plen;
   char *str, *order;
   int i;

   plen=strlen(param);
   if (strncmp(buf,param,plen) != 0) return(0);
   buf+=plen;
   if ((unsigned char)*buf>' ') return(0);
   while (*buf && (unsigned char)*buf<=' ') buf++;

   str=buf;
   order=NULL;
   while (*str && (unsigned char)*str>' ') str++;
   if (*str) {
      *str++='\0';
      while (*str && (unsigned char)*str<=' ') str++;
      order=str;
   }
   for (i=0 ; i<noptions && strcasecmp(buf,options[i].name) ; i++);
   if (i>=noptions) {
      debuga(_("Unknown sort criterion \"%s\" for parameter \"%s\"\n"),buf,param);
      exit(EXIT_FAILURE);
   }
   *value|=options[i].value;

   if (order) {
      str=order;
      while (*str && (unsigned char)*str>' ') str++;
      if (*str) {
         *str++='\0';
         while (*str && (unsigned char)*str<=' ') str++;
      }
      if (strcasecmp(order,"reverse")==0 || strcasecmp(order,"D")==0) {
         *value|=SORT_REVERSE;
      } else if (strcasecmp(order,"normal")!=0 && strcasecmp(order,"A")!=0) {
         debuga(_("Unknown sort order \"%s\" for parameter \"%s\"\n"),order,param);
         exit(EXIT_FAILURE);
      }
   }

   buf=str;
   return(1);
}

static void parmtest(char *buf)
{
   char wbuf[2048];
   struct getwordstruct gwarea;

   while (*buf && (unsigned char)*buf<=' ') buf++;

   if(*buf == '#' || *buf == '\0')
      return;

   if(debugz)
      printf(_("SARG: TAG: %s\n"),buf);

   if (getparam_string("background_color",buf,BgColor,sizeof(BgColor))>0) return;

   if (getparam_string("text_color",buf,TxColor,sizeof(TxColor))>0) return;

   if (getparam_string("text_bgcolor",buf,TxBgColor,sizeof(TxBgColor))>0) return;

   if (getparam_string("title_color",buf,TiColor,sizeof(TiColor))>0) return;

   if (getparam_string("logo_image",buf,LogoImage,sizeof(LogoImage))>0) return;

   if (getparam_quoted("logo_text",buf,LogoText,sizeof(LogoText))>0) return;

   if (getparam_string("logo_text_color",buf,LogoTextColor,sizeof(LogoTextColor))>0) return;

   if (getparam_string("background_image",buf,BgImage,sizeof(BgImage))>0) return;

   if (getparam_bool("show_sarg_info",buf,&ShowSargInfo)>0) return;

   if (getparam_bool("show_sarg_logo",buf,&ShowSargLogo)>0) return;

   if (getparam_string("font_face",buf,FontFace,sizeof(FontFace))>0) return;

   if (getparam_string("header_color",buf,HeaderColor,sizeof(HeaderColor))>0) return;

   if (getparam_string("header_bgcolor",buf,HeaderBgColor,sizeof(HeaderBgColor))>0) return;

   if (getparam_string("font_size",buf,FontSize,sizeof(FontSize))>0) return;

   if (getparam_string("header_font_size",buf,HeaderFontSize,sizeof(HeaderFontSize))>0) return;

   if (getparam_string("title_font_size",buf,TitleFontSize,sizeof(TitleFontSize))>0) return;

   if (getparam_2words("image_size",buf,Width,sizeof(Width),Height,sizeof(Height))>0) return;

   if (getparam_quoted("title",buf,Title,sizeof(Title))>0) return;

   if (getparam_bool("resolve_ip",buf,&Ip2Name)>0) return;

   if (getparam_bool("user_ip",buf,&UserIp)>0) return;

   if (getparam_string("max_elapsed",buf,MaxElapsed,sizeof(MaxElapsed))>0) return;

   if (is_param("date_format",buf)) {
      getword_start(&gwarea,buf);
      if (getword_multisep(wbuf,sizeof(wbuf),&gwarea,' ')<0) {
         debuga(_("Maybe you have a broken record or garbage in \"date_format\" parameter\n"));
         exit(EXIT_FAILURE);
      }
      strncpy(DateFormat,gwarea.current,1);
      fixnone(DateFormat);
      return;
   }

   if (is_param("hours",buf)) {
      if( getnumlist( buf, &hours, 24, 24 ) ) {
         debuga(_("Error: Invalid syntax in hours tag!\n"));
         exit( 1 );
      }
      return;
   }

   if (is_param("weekdays",buf)) {
      if( getnumlist( buf, &weekdays, 7, 7 ) ) {
         debuga(_("Error: Invalid syntax in weekdays tag!\n"));
         exit( 1 );
      }
      return;
   }

   if (getparam_sort("topuser_sort_field",SET_LIST(topuser_sort),buf,&TopuserSort)>0) return;

   if (getparam_sort("user_sort_field",SET_LIST(user_sort),buf,&UserSort)>0) return;

   if (is_param("access_log",buf)>0) {
      if (AccessLogFromCmdLine==0) {
         if (NAccessLog>=MAXLOGS) {
            debuga(_("Too many log files in configuration file\n"));
            exit(EXIT_FAILURE);
         }
         getparam_string("access_log",buf,AccessLog[NAccessLog],MAX_LOG_FILELEN);
         NAccessLog++;
      }
      return;
   }

   if (is_param("redirector_log",buf)>0) {
      if (RedirectorLogFromCmdLine==0) {
         if (NRedirectorLogs>=MAX_REDIRECTOR_LOGS) {
            debuga(_("Too many redirector log files in configuration file\n"));
            exit(EXIT_FAILURE);
         }
         getparam_string("redirector_log",buf,RedirectorLogs[NRedirectorLogs],MAX_REDIRECTOR_FILELEN);
         NRedirectorLogs++;
      }
      return;
   }

   if (getparam_string("useragent_log",buf,UserAgentLog,sizeof(UserAgentLog))>0) return;

   if (getparam_string("exclude_hosts",buf,ExcludeHosts,sizeof(ExcludeHosts))>0) return;

   if (getparam_string("exclude_codes",buf,ExcludeCodes,sizeof(ExcludeCodes))>0) return;

   if (getparam_string("exclude_users",buf,ExcludeUsers,sizeof(ExcludeUsers))>0) return;

   if (getparam_string("password",buf,PasswdFile,sizeof(PasswdFile))>0) return;

   if (getparam_string("temporary_dir",buf,TempDir,sizeof(TempDir))>0) return;

   if (getparam_list("report_type",SET_LIST(report_type_values),buf,&ReportType)>0) return;

   if (getparam_string("output_dir",buf,OutputDir,sizeof(OutputDir))>0) return;

   if (getparam_string("output_email",buf,OutputEmail,sizeof(OutputEmail))>0) return;

   if (getparam_2words("per_user_limit",buf,PerUserLimitFile,sizeof(PerUserLimitFile),wbuf,sizeof(wbuf))>0) {
      PerUserLimit=atoi(wbuf);
      return;
   }

   if (getparam_int("lastlog",buf,&LastLog)>0) return;

   if (getparam_bool("remove_temp_files",buf,&RemoveTempFiles)>0) return;

   if (getparam_string("replace_index",buf,ReplaceIndex,sizeof(ReplaceIndex))>0) return;

   if (getparam_list("index_tree",SET_LIST(index_tree_values),buf,&IndexTree)>0) return;

   if (getparam_list("index",SET_LIST(index_values),buf,&Index)>0) return;

   if (getparam_bool("overwrite_report",buf,&OverwriteReport)>0) return;

   if (getparam_list("records_without_userid",SET_LIST(recnouser_values),buf,&RecordsWithoutUser)>0) return;

   if (getparam_bool("use_comma",buf,&UseComma)>0) return;

   if (getparam_string("mail_utility",buf,MailUtility,sizeof(MailUtility))>0) return;

   if (getparam_int("topsites_num",buf,&TopSitesNum)>0) return;

   if (getparam_int("topuser_num",buf,&TopUsersNum)>0) return;

   if (getparam_string("usertab",buf,UserTabFile,sizeof(UserTabFile))>0) return;

   if (getparam_string("index_sort_order",buf,IndexSortOrder,sizeof(IndexSortOrder))>0) return;

   if (getparam_sort("topsites_sort_order",SET_LIST(topsite_sort),buf,&TopsitesSort)>0) return;

   if (getparam_bool("long_url",buf,&LongUrl)>0) return;

   if (getparam_string("dansguardian_conf",buf,DansGuardianConf,sizeof(DansGuardianConf))>0) return;

   if (getparam_string("squidguard_conf",buf,SquidGuardConf,sizeof(SquidGuardConf))>0) return;

   if (getparam_list("date_time_by",SET_LIST(datetime_values),buf,&datetimeby)>0) return;

   if (getparam_string("charset",buf,CharSet,sizeof(CharSet))>0) {
      ccharset(CharSet);
      return;
   }

   if (getparam_quoted("user_invalid_char",buf,UserInvalidChar,sizeof(UserInvalidChar))>0) return;

   if (getparam_quoted("include_users",buf,IncludeUsers+1,sizeof(IncludeUsers)-2)>0) {
      IncludeUsers[0]=':';
      strcat(IncludeUsers,":");
      return;
   }

   if (getparam_quoted("exclude_string",buf,ExcludeString,sizeof(ExcludeString))>0) return;

   if (getparam_bool("privacy",buf,&Privacy)>0) return;

   if (getparam_quoted("privacy_string",buf,PrivacyString,sizeof(PrivacyString))>0) return;

   if (getparam_string("privacy_string_color",buf,PrivacyStringColor,sizeof(PrivacyStringColor))>0) return;

   if (getparam_bool("show_successful_message",buf,&SuccessfulMsg)>0) return;

   if (getparam_bool("show_read_statistics",buf,&ShowReadStatistics)>0) return;

   if (getparam_list("topuser_fields",SET_LIST(topuserfields_values),buf,&TopUserFields)>0) return;

   if (getparam_bool("bytes_in_sites_users_report",buf,&BytesInSitesUsersReport)>0) return;

   if (getparam_list("user_report_fields",SET_LIST(userreportfields_values),buf,&UserReportFields)>0) return;

   if (getparam_string("datafile",buf,DataFile,sizeof(DataFile))>0) return;

   if (getparam_quoted("datafile_delimiter",buf,DataFileDelimiter,sizeof(DataFileDelimiter))>0) return;

   if (getparam_list("datafile_fields",SET_LIST(data_field_values),buf,&DataFileFields)>0) return;

   if (getparam_list("datafile_url",SET_LIST(datafileurl_values),buf,&DataFileUrl)>0) return;

   if (getparam_string("parsed_output_log",buf,ParsedOutputLog,sizeof(ParsedOutputLog))>0) return;

   if (getparam_string("parsed_output_log_compress",buf,ParsedOutputLogCompress,sizeof(ParsedOutputLogCompress))>0) return;

   if (getparam_list("displayed_values",SET_LIST(displayvalue_values),buf,&DisplayedValues)>0) return;

   if (getparam_int("authfail_report_limit",buf,&AuthfailReportLimit)>0) return;

   if (getparam_int("denied_report_limit",buf,&DeniedReportLimit)>0) return;

   if (getparam_int("siteusers_report_limit",buf,&SiteUsersReportLimit)>0) return;

   if (getparam_int("dansguardian_report_limit",buf,&DansGuardianReportLimit)>0) return;

   if (getparam_int("squidguard_report_limit",buf,&SquidGuardReportLimit)>0) return;

   if (getparam_int("user_report_limit",buf,&UserReportLimit)>0) return;

   if (getparam_int("download_report_limit",buf,&DownloadReportLimit)>0) return;

   if (getparam_string("www_document_root",buf,wwwDocumentRoot,sizeof(wwwDocumentRoot))>0) return;

   if (getparam_string("block_it",buf,BlockIt,sizeof(BlockIt))>0) return;

   if (getparam_string("external_css_file",buf,ExternalCSSFile,sizeof(ExternalCSSFile))>0) return;

   if (getparam_bool("user_authentication",buf,&UserAuthentication)>0) return;

   if (getparam_string("AuthUserTemplateFile",buf,wbuf,sizeof(wbuf))>0) {
      char dir[MAXLEN];

      if (is_absolute(wbuf)) {
         if (strlen(wbuf)>=sizeof(AuthUserTemplateFile)) {
            debuga(_("Template file name is too long in parameter \"AuthUserTemplateFile\"\n"));
            exit(EXIT_FAILURE);
         }
         strcpy(AuthUserTemplateFile,wbuf);
      } else {
         strcpy(dir,ConfigFile);
         if (snprintf(AuthUserTemplateFile,sizeof(AuthUserTemplateFile),"%s/%s",dirname(dir),wbuf)>=sizeof(AuthUserTemplateFile)) {
            debuga(_("Template file name is too long in parameter \"AuthUserTemplateFile\"\n"));
            exit(EXIT_FAILURE);
         }
      }
      return;
   }

   if (is_param("download_suffix",buf)) {
      char warea[MAXLEN];

      getparam_quoted("download_suffix",buf,warea,sizeof(warea));
      set_download_suffix(warea);
      return;
   }

   if (getparam_bool("graphs",buf,&Graphs)>0) return;

   if (getparam_string("graph_days_bytes_bar_color",buf,GraphDaysBytesBarColor,sizeof(GraphDaysBytesBarColor))>0) return;

   if (getparam_string("redirector_log_format",buf,RedirectorLogFormat,sizeof(RedirectorLogFormat))>0) return;
   if (getparam_string("squidguard_log_format",buf,RedirectorLogFormat,sizeof(RedirectorLogFormat))>0) {
      debuga(_("squidguard_log_format is deprecated and has been replaced by redirector_log_format. Please update your configuration file.\n"));
      return;
   }

   if (getparam_bool("redirector_filter_out_date",buf,&RedirectorFilterOutDate)>0) return;
   if (getparam_bool("redirector_ignore_date",buf,&RedirectorFilterOutDate)>0) {
      /*
      Due to an old bug in sarg before version 2.3, the option was having the opposite action than implied by the name.
      */
      debuga(_("redirector_ignore_date is deprecated and has been replaced by redirector_filter_out_date that does the action implied by its name as opposed to redirector_ignore_date. Please update your configuration file.\n"));
      RedirectorFilterOutDate=!RedirectorFilterOutDate;
      return;
   }
   if (getparam_bool("squidguard_ignore_date",buf,&RedirectorFilterOutDate)>0) {
      debuga(_("squidguard_ignore_date is deprecated and has been replaced by redirector_filter_out_date that does the action implied by its name as opposed to squidguard_ignore_date. Please update your configuration file.\n"));
      RedirectorFilterOutDate=!RedirectorFilterOutDate;
      return;
   }

   if (getparam_bool("dansguardian_filter_out_date",buf,&DansguardianFilterOutDate)>0) return;
   if (getparam_bool("dansguardian_ignore_date",buf,&DansguardianFilterOutDate)>0) {
      debuga(_("dansguardian_ignore_date is deprecated and has been replaced by dansguardian_filter_out_date that does the action implied by its name as opposed to dansguardian_ignore_date. Please update your configuration file.\n"));
      DansguardianFilterOutDate=!DansguardianFilterOutDate;
      return;
   }

   if (getparam_string("ulimit",buf,Ulimit,sizeof(Ulimit))>0) return;

   if (getparam_list("ntlm_user_format",SET_LIST(ntml_userformat_values),buf,&NtlmUserFormat)>0) return;

   if (getparam_string("realtime_types",buf,RealtimeTypes,sizeof(RealtimeTypes))>0) return;

   if (getparam_list("realtime_unauthenticated_records",SET_LIST(realtime_unauth_values),buf,&RealtimeUnauthRec)>0) return;

   if (getparam_int("realtime_refresh_time",buf,&realtime_refresh)>0) return;

   if (getparam_int("realtime_access_log_lines",buf,&realtime_access_log_lines)>0) return;

   if (getparam_string("LDAPHost",buf,LDAPHost,sizeof(LDAPHost))>0) return;

   if (getparam_int("LDAPPort",buf,&LDAPPort)>0) return;

   if (getparam_int("LDAPProtocolVersion",buf,&LDAPProtocolVersion)>0) return;

   if (getparam_string("LDAPBindDN",buf,LDAPBindDN,sizeof(LDAPBindDN))>0) return;

   if (getparam_string("LDAPBindPW",buf,LDAPBindPW,sizeof(LDAPBindPW))>0) return;

   if (getparam_string("LDAPBaseSearch",buf,LDAPBaseSearch,sizeof(LDAPBaseSearch))>0) return;

   if (getparam_string("LDAPFilterSearch",buf,LDAPFilterSearch,sizeof(LDAPFilterSearch))>0) return;

   if (getparam_string("LDAPTargetAttr",buf,LDAPTargetAttr,sizeof(LDAPTargetAttr))>0) return;

   if (getparam_string("graph_font",buf,GraphFont,sizeof(GraphFont))>0) return;

   if(strstr(buf,"squid24") != 0) {
      squid24=true;
      return;
   }

   if(strstr(buf,"byte_cost") != 0) {
      getword_start(&gwarea,buf);
      if (getword_multisep(wbuf,sizeof(wbuf),&gwarea,' ')<0) {
         debuga(_("The \"byte_cost\" parameter of the configuration file is invalid\n"));
         exit(EXIT_FAILURE);
      }
      cost=atol(gwarea.current);
      if (getword_multisep(wbuf,sizeof(wbuf),&gwarea,' ')<0) {
         debuga(_("The \"byte_cost\" parameter of the configuration file is invalid\n"));
         exit(EXIT_FAILURE);
      }
      nocost=my_atoll(gwarea.current);
      return;
   }

   printf(_("SARG: Unknown option %s\n"),buf);
}

void getconf(void)
{

   FILE *fp_in;
   char buf[MAXLEN];

   if(debug)
      debuga(_("Loading configuration from %s\n"),ConfigFile);

   if ((fp_in = fopen(ConfigFile, "r")) == NULL) {
      debuga(_("(getconf) Cannot open file %s\n"),ConfigFile);
      exit(EXIT_FAILURE);
   }

   while (fgets(buf, sizeof(buf), fp_in) != NULL) {
      fixendofline(buf);

      if(debugm)
         printf("SYSCONFDIR %s\n",buf);

      parmtest(buf);

   }

   fclose(fp_in);
   return;
}
