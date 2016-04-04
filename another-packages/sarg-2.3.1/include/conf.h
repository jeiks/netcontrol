#include "config.h"
#include "info.h"
#include "btree_cache.h"

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#if defined(HAVE_SYS_DIRENT_H) && !defined(HAVE_DIRENT_H)
#include <sys/dirent.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_TYPES_H
#include <types.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifdef HAVE_GD_H
#define HAVE_GD 1
#include <gd.h>
#endif
#ifdef HAVE_GDFONTL_H
#include <gdfontl.h>
#endif
#ifdef HAVE_GDFONTT_H
#include <gdfontt.h>
#endif
#ifdef HAVE_GDFONTS_H
#include <gdfonts.h>
#endif
#ifdef HAVE_GDFONTMB_H
#include <gdfontmb.h>
#endif
#ifdef HAVE_GDFONTG_H
#include <gdfontg.h>
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_WINSOCK_H
#include <winsock.h>
#endif
#ifdef HAVE_MATH_H
#include <math.h>
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif
#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#else
typedef int bool;
#define true 1
#define false 0
#endif

#if defined(HAVE_FOPEN64)
#define _FILE_OFFSET_BITS 64
#define MY_FOPEN fopen64
#else
#define MY_FOPEN fopen
#endif

#if !defined(HAVE_BZERO)
#define bzero(mem,size) memset(mem,0,size)
#endif

#if defined(IBERTY_LIB) && !defined(HAVE_MKSTEMP)
int mkstemps(char *template, int suffixlen);
#define mkstemp(template) mkstemps(template,0)
#endif

#if defined(ENABLE_NLS) && defined(HAVE_LIBINTL_H)
#include <libintl.h>
#define _(String) gettext(String)
#ifdef gettext_noop
#define N_(String) gettext_noop(String)
#else
#define N_(String) (String)
#endif
#else /* No NLS */
#define _(String) (String)
#define N_(String) (String)
#define ngettext(String,Strings,num) (Strings)
#define textdomain(String)
#define bindtextdomain(Domain,Directory)
#endif //NLS

#if defined(__MINGW32__)
#define mkdir(p,m) _mkdir(p)

#ifndef WIFEXITED
  #define WIFEXITED(S) 1
#endif

#ifndef WEXITSTATUS
  #define WEXITSTATUS(S) (S)
#endif

#endif /*__MINGW32__*/

#ifndef __GNUC__
#  define  __attribute__(x)
#endif

#define MAXLEN 20000
#define MAX_URL_LEN 40000
#define MAX_TRUNCATED_URL 512
#define MAX_USER_LEN 256
#define MAX_USER_FNAME_LEN 128
#define MAX_IP_LEN 64
#define MAX_DATETIME_LEN 32
#define MAXLOGS 255
#define MAX_LOG_FILELEN 1024
#define MAX_REDIRECTOR_LOGS 64
#define MAX_REDIRECTOR_FILELEN 1024
#define MAX_DATETIME_DAYS 90

#define REPORT_TYPE_USERS_SITES         0x0001UL
#define REPORT_TYPE_SITE_USER_TIME_DATE 0x0002UL
#define REPORT_TYPE_TOPUSERS            0x0004UL
#define REPORT_TYPE_TOPSITES            0x0008UL
#define REPORT_TYPE_SITES_USERS         0x0010UL
#define REPORT_TYPE_DATE_TIME           0x0020UL
#define REPORT_TYPE_DENIED              0x0040UL
#define REPORT_TYPE_AUTH_FAILURES       0x0080UL
#define REPORT_TYPE_DOWNLOADS           0x0100UL

#define DATA_FIELD_USER      0x0001UL
#define DATA_FIELD_DATE      0x0002UL
#define DATA_FIELD_TIME      0x0004UL
#define DATA_FIELD_URL       0x0008UL
#define DATA_FIELD_CONNECT   0x0010UL
#define DATA_FIELD_BYTES     0x0020UL
#define DATA_FIELD_IN_CACHE  0x0040UL
#define DATA_FIELD_OUT_CACHE 0x0080UL
#define DATA_FIELD_ELAPSED   0x0100UL

#define TOPUSERFIELDS_NUM          0x0001UL
#define TOPUSERFIELDS_DATE_TIME    0x0002UL
#define TOPUSERFIELDS_USERID       0x0004UL
#define TOPUSERFIELDS_CONNECT      0x0008UL
#define TOPUSERFIELDS_BYTES        0x0010UL
#define TOPUSERFIELDS_SETYB        0x0020UL
#define TOPUSERFIELDS_IN_CACHE_OUT 0x0040UL
#define TOPUSERFIELDS_USED_TIME    0x0080UL
#define TOPUSERFIELDS_MILISEC      0x0100UL
#define TOPUSERFIELDS_PTIME        0x0200UL
#define TOPUSERFIELDS_TOTAL        0x0400UL
#define TOPUSERFIELDS_AVERAGE      0x0800UL

#define USERREPORTFIELDS_CONNECT      0x0001UL
#define USERREPORTFIELDS_BYTES        0x0002UL
#define USERREPORTFIELDS_SETYB        0x0004UL
#define USERREPORTFIELDS_IN_CACHE_OUT 0x0008UL
#define USERREPORTFIELDS_USED_TIME    0x0010UL
#define USERREPORTFIELDS_MILISEC      0x0020UL
#define USERREPORTFIELDS_PTIME        0x0040UL
#define USERREPORTFIELDS_TOTAL        0x0080UL
#define USERREPORTFIELDS_AVERAGE      0x0100UL

#define INDEX_YES  0x0001UL
#define INDEX_NO   0x0002UL
#define INDEX_ONLY 0x0004UL

#define INDEX_TREE_DATE 0x0001UL
#define INDEX_TREE_FILE 0x0002UL

#define NTLMUSERFORMAT_USER       0x0001UL
#define NTLMUSERFORMAT_DOMAINUSER 0x0002UL

#define RECORDWITHOUTUSER_IP        0x0001UL
#define RECORDWITHOUTUSER_IGNORE    0x0002UL
#define RECORDWITHOUTUSER_EVERYBODY 0x0004UL

#define DATAFILEURL_IP   0x0001UL
#define DATAFILEURL_NAME 0x0002UL

#define DISPLAY_BYTES 0x0001UL
#define DISPLAY_ABBREV 0x0002UL

#define DATETIME_ELAP 0x0001UL
#define DATETIME_BYTE 0x0002UL

#define REALTIME_UNAUTH_REC_SHOW   0x0001UL
#define REALTIME_UNAUTH_REC_IGNORE 0x0002UL

#define SORT_REVERSE 0x0001

#define TOPUSER_SORT_REVERSE SORT_REVERSE
#define TOPUSER_SORT_BYTES   0x0002UL
#define TOPUSER_SORT_USER    0x0004UL
#define TOPUSER_SORT_CONNECT 0x0008UL
#define TOPUSER_SORT_TIME    0x0010UL

#define TOPSITE_SORT_REVERSE SORT_REVERSE
#define TOPSITE_SORT_BYTES   0x0002UL
#define TOPSITE_SORT_CONNECT 0x0004UL

#define USER_SORT_REVERSE SORT_REVERSE
#define USER_SORT_BYTES   0x0002UL
#define USER_SORT_SITE    0x0004UL
#define USER_SORT_CONNECT 0x0008UL
#define USER_SORT_TIME    0x0010UL

struct periodstruct
{
   //! The first date of the period.
   struct tm start;
   //! The last date of the period.
   struct tm end;
   //! The textual representation of the date.
   char text[40];
   //! The HTML representation of the date.
   char html[40];
};

char outdir[MAXLEN];
char outdirname[MAXLEN];
struct periodstruct period;
char code[MAXLEN];
char code2[MAXLEN];
char tmp[MAXLEN];
char parse_out[MAXLEN];
char arqtt[MAXLEN];
char html[MAXLEN];
char ConfigFile[MAXLEN];
char df[20];
int LastLog;
bool RemoveTempFiles;
char ReplaceIndex[256];
unsigned long int Index;
bool OverwriteReport;
unsigned long int RecordsWithoutUser;
bool UseComma;
char MailUtility[PATH_MAX];
int TopSitesNum;
int TopUsersNum;
char ExcludeCodes[256];
unsigned long int TopsitesSort;
unsigned long int ReportType;
char UserTabFile[255];
char warea[MAXLEN];
char name[MAXLEN];
bool LongUrl;
bool Ip2Name;
int NAccessLog;
char AccessLog[MAXLOGS][MAX_LOG_FILELEN];
int AccessLogFromCmdLine;
char Title[MAXLEN];
char BgColor[MAXLEN];
char BgImage[MAXLEN];
char TxColor[MAXLEN];
char TxBgColor[MAXLEN];
char TiColor[MAXLEN];
char LogoImage[MAXLEN];
char LogoText[MAXLEN];
char LogoTextColor[MAXLEN];
char Width[MAXLEN];
char Height[MAXLEN];
char FontFace[MAXLEN];
char HeaderColor[MAXLEN];
char HeaderBgColor[MAXLEN];
char FontSize[MAXLEN];
char PasswdFile[MAXLEN];
char TempDir[MAXLEN];
char OutputDir[MAXLEN];
char OutputEmail[MAXLEN];
unsigned long int TopuserSort;
unsigned long int UserSort;
char UserAgentLog[255];
char module[255];
char ExcludeHosts[255];
char ExcludeUsers[255];
char DateFormat[2];
char PerUserLimitFile[255];
int PerUserLimit;
bool UserIp;
char MaxElapsed[255];
unsigned long int datetimeby;
char CharSet[255];
char UserInvalidChar[255];
bool Graphs;
char GraphDaysBytesBarColor[255];
bool Privacy;
char PrivacyString[255];
char PrivacyStringColor[30];
char IncludeUsers[MAXLEN];
char ExcludeString[MAXLEN];
bool SuccessfulMsg;
unsigned long int TopUserFields;
unsigned long int UserReportFields;
char DataFile[MAXLEN];
char DataFileDelimiter[3];
unsigned long int DataFileFields;
unsigned long int DataFileUrl;
bool ShowReadStatistics;
char IndexSortOrder[5];
char DansGuardianConf[MAXLEN];
bool DansguardianFilterOutDate;
char SquidGuardConf[MAXLEN];
char SquidGuarddbHome[255];
char RedirectorLogFormat[4096];
int NRedirectorLogs;
char RedirectorLogs[MAX_REDIRECTOR_LOGS][MAX_REDIRECTOR_FILELEN];
int RedirectorLogFromCmdLine;
bool RedirectorFilterOutDate;
bool ShowSargInfo;
bool BytesInSitesUsersReport;
bool ShowSargLogo;
char ParsedOutputLog[MAXLEN];
char ParsedOutputLogCompress[512];
unsigned long int DisplayedValues;
char HeaderFontSize[5];
char TitleFontSize[5];
char wwwDocumentRoot[MAXLEN];
char ExternalCSSFile[MAXLEN];
char BlockIt[255];
unsigned long int NtlmUserFormat;
unsigned long int IndexTree;
bool UserAuthentication;
char AuthUserTemplateFile[1024];
char val1[MAXLEN];
char val2[MAXLEN];
char val3[MAXLEN];
char val4[MAXLEN];
char val5[MAXLEN];
char val6[MAXLEN];
char val7[MAXLEN];
char val8[MAXLEN];
char val9[MAXLEN];
char val10[MAXLEN];
char val11[MAXLEN];
char wwork1[MAXLEN];
char wwork2[MAXLEN];
char wwork3[MAXLEN];
char mask[MAXLEN];
char site[MAXLEN];
char us[50];
char email[MAXLEN];
char test[1];
char user2[MAXLEN];
char wentp[512];
char addr[MAXLEN];
char Ulimit[6];
char RealtimeTypes[1024];
char cmd[255];
char ImageFile[255];
unsigned long int RealtimeUnauthRec;
char LDAPHost[255];
char LDAPBindDN[512];
char LDAPBindPW[255];
int LDAPPort;
int LDAPProtocolVersion;
char LDAPBaseSearch[255];
char LDAPFilterSearch[512];
char LDAPTargetAttr[64];
char GraphFont[MAXLEN];

int  idate;
int  smartfilter;
int  denied_count;
int  download_count;
int  authfail_count;
int  dansguardian_count;
int  squidguard_count;
int  useragent_count;
int  limit_flag;
int  z1, z2, z3;
int  ttopen;
int  sarglog;
int  isalog;
int  dfrom;
int  duntil;
int  dataonly;
int  indexonly;
bool  iprel;
int  langcode;
int  debug;
int  debugz;
int  debugm;
int  AuthfailReportLimit;
int  DeniedReportLimit;
int  DownloadReportLimit;
int  SiteUsersReportLimit;
int  DansGuardianReportLimit;
int  SquidGuardReportLimit;
int  UserReportLimit;
int  realtime_refresh;
int  realtime_access_log_lines;
int  rc;
int  ntopsites;
int  nsitesusers;
int  nrepday;
int  ndownload;
int  ntopuser;
bool  squid24;

long long int nocost;
float cost;

typedef struct
{ int list[ 24 ];
  int len;
} numlist;

DIR *dirp;
struct dirent *direntp;
