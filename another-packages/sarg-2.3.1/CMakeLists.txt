CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(sarg C)
SET(sarg_VERSION 2)
SET(sarg_REVISION "3.1")
SET(sarg_BUILD "")
SET(sarg_BUILDDATE "Sep-18-2010")

INCLUDE(AddFileDependencies)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckCCompilerFlag)

DEFINE_PROPERTY(GLOBAL PROPERTY enable-htmldir BRIEF_DOCS "--enable-htmldir=htmldir"
                        FULL_DOCS "Select htmldir as the root of your WWW documents")

SET(PACKAGE_NAME "sarg")
SET(PACKAGE_VERSION "${sarg_VERSION}.${sarg_REVISION}${sarg_BUILD}")

MACRO(SET_ABS_SOURCE VAR DOC)
   IF(${VAR} AND NOT ${VAR}_SOURCE)
      SET(PATH "${${VAR}}")
      IF(NOT IS_ABSOLUTE ${PATH})
         SET(PATH "${CMAKE_INSTALL_PREFIX}/${PATH}")
      ENDIF(NOT IS_ABSOLUTE ${PATH})
      GET_FILENAME_COMPONENT(SOURCE_PATH "${PATH}" ABSOLUTE)
      SET(${VAR}_SOURCE "${SOURCE_PATH}" CACHE PATH "${DOC}")
   ENDIF(${VAR} AND NOT ${VAR}_SOURCE)
ENDMACRO(SET_ABS_SOURCE VAR DOC)

SET_ABS_SOURCE(SYSCONFDIR "The full path of the configuration directory to be compiled in the program")
SET_ABS_SOURCE(LANGDIR "The full path of the language directory to be compiled in the program")
SET_ABS_SOURCE(FONTDIR "The full path of the fonts directory to be compiled in the program")
SET_ABS_SOURCE(IMAGEDIR "The full path of the images directory to be compiled in the program")

SET(SYSCONFDIR "etc/sarg" CACHE PATH "The configuration directory relative to the install directory")
SET(BINDIR "bin" CACHE PATH "The directory to install the exec relative to the install directory")
SET(MANDIR "share/man" CACHE PATH "The directory to copy the man page relative to the install directory")
SET(LANGDIR "share/sarg/languages" CACHE PATH "The directory to copy the languages files relative to the install directory")
SET(FONTDIR "share/sarg/fonts" CACHE PATH "The directory to copy the fonts relative to the install directory")
SET(IMAGEDIR "share/sarg/images" CACHE PATH "The directory to copy the images files relative to the install directory")
SET(SARGPHPDIR "share/sarg/sarg-php" CACHE PATH "The directory to copy sarg-php relative to the install directory")

# Sources to compile
INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}") #Make it find this config.h before the one in src/include
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")
SET(SRC util.c log.c report.c topuser.c email.c sort.c html.c
       totger.c index.c getconf.c usage.c decomp.c ip2name.c
       useragent.c exclude.c convlog.c totday.c repday.c datafile.c
       indexonly.c splitlog.c lastlog.c topsites.c siteuser.c css.c
       smartfilter.c denied.c authfail.c charset.c
       squidguard_log.c squidguard_report.c auth.c download.c grepday.c
       dansguardian_log.c dansguardian_report.c realtime.c btree_cache.c
       usertab.c userinfo.c longline.c)

FOREACH(f ${SRC})
   ADD_FILE_DEPENDENCIES(${f} ${CMAKE_BINARY_DIR}/config.h ${CMAKE_SOURCE_DIR}/include/conf.h ${CMAKE_SOURCE_DIR}/include/info.h ${CMAKE_SOURCE_DIR}/include/defs.h)
ENDFOREACH(f)

ADD_EXECUTABLE(sarg ${SRC})

SET_TARGET_PROPERTIES(sarg PROPERTIES VERSION "${sarg_VERSION}.${sarg_REVISION}.${sarg_BUILD}")

SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_FLAGS "$ENV{CFLAGS} -Wall -Wno-sign-compare")
SET_TARGET_PROPERTIES(sarg PROPERTIES LINK_FLAGS "$ENV{LDFLAGS}")

CHECK_C_COMPILER_FLAG("-Wextra" HAVE_WEXTRA)
IF(HAVE_WEXTRA)
   # Not supported by gcc 3.3
   GET_TARGET_PROPERTY(TMPCFLAGS sarg COMPILE_FLAGS)
   SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_FLAGS "${TMPCFLAGS} -Wextra -Wno-unused-parameter")
ENDIF(HAVE_WEXTRA)

CHECK_C_COMPILER_FLAG("-Werror=implicit-function-declaration" HAVE_WERROR_IMPLICIT_FUNCTION)
IF(HAVE_WERROR_IMPLICIT_FUNCTION)
   GET_TARGET_PROPERTY(TMPCFLAGS sarg COMPILE_FLAGS)
   SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_FLAGS "${TMPCFLAGS} -Werror=implicit-function-declaration")
ENDIF(HAVE_WERROR_IMPLICIT_FUNCTION)

CHECK_C_COMPILER_FLAG("-Werror=format" HAVE_WERROR_FORMAT)
IF(HAVE_WERROR_FORMAT)
   GET_TARGET_PROPERTY(TMPCFLAGS sarg COMPILE_FLAGS)
   SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_FLAGS "${TMPCFLAGS} -Werror=format")
ENDIF(HAVE_WERROR_FORMAT)

# Required header files
CHECK_INCLUDE_FILE(stdio.h HAVE_STDIO_H)
CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILE(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILE(sys/time.h HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE(time.h HAVE_TIME_H)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILE(sys/dirent.h HAVE_SYS_DIRENT_H)
CHECK_INCLUDE_FILE(dirent.h HAVE_DIRENT_H)
CHECK_INCLUDE_FILE(sys/socket.h HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILE(netdb.h HAVE_NETDB_H)
CHECK_INCLUDE_FILE(arpa/inet.h HAVE_ARPA_INET_H)
CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE(netinet/in.h HAVE_NETINET_H)
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(ctype.h HAVE_CTYPE_H)
CHECK_INCLUDE_FILE(errno.h HAVE_ERRNO_H)
CHECK_INCLUDE_FILE(sys/resource.h HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILE(sys/wait.h HAVE_SYS_WAIT_H)
CHECK_INCLUDE_FILE(stdarg.h HAVE_STDARG_H)
CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(limits.h HAVE_LIMITS_H)
CHECK_INCLUDE_FILE(math.h HAVE_MATH_H)
CHECK_INCLUDE_FILE(locale.h HAVE_LOCALE_H)
CHECK_INCLUDE_FILE(execinfo.h HAVE_EXECINFO_H)
CHECK_INCLUDE_FILE(libintl.h HAVE_LIBINTL_H)
CHECK_INCLUDE_FILE(libgen.h HAVE_LIBGEN_H)
CHECK_INCLUDE_FILE(stdbool.h HAVE_STDBOOL_H)
CHECK_INCLUDE_FILE(getopt.h HAVE_GETOPT_H)

IF(!HAVE_GETOPT_H)
   MESSAGE(SEND_ERROR "getopt.h is required to compile sarg")
ENDIF(!HAVE_GETOPT_H)

# Windows need lib wsock32 and ws2_32
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
   CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS_H)
   CHECK_INCLUDE_FILE(winsock.h HAVE_WINSOCK_H)
   CHECK_INCLUDE_FILE(ws2tcpip.h HAVE_WS2TCPIP_H)

   FIND_LIBRARY(WSOCK_LIB NAMES wsock32 DOC "The winsock library")
   IF(WSOCK_LIB)
      TARGET_LINK_LIBRARIES(sarg wsock32)
      LIST(APPEND CMAKE_REQUIRED_LIBRARIES wsock32)
   ENDIF(WSOCK_LIB)

   FIND_LIBRARY(WSOCK2_LIB NAMES ws2_32 DOC "The winsock 2 library")
   IF(WSOCK2_LIB)
      TARGET_LINK_LIBRARIES(sarg ws2_32)
      LIST(APPEND CMAKE_REQUIRED_LIBRARIES ws2_32)
   ENDIF(WSOCK2_LIB)

   ADD_DEFINITIONS(-DWINDOWS)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

CHECK_FUNCTION_EXISTS(bzero HAVE_BZERO)
CHECK_FUNCTION_EXISTS(backtrace HAVE_BACKTRACE)
CHECK_FUNCTION_EXISTS(symlink HAVE_SYMLINK)
CHECK_FUNCTION_EXISTS(lstat HAVE_LSTAT)
CHECK_FUNCTION_EXISTS(getnameinfo HAVE_GETNAMEINFO)
CHECK_FUNCTION_EXISTS(inet_aton HAVE_INET_ATON)

# Find gd
CHECK_INCLUDE_FILE(gd.h HAVE_GD_H)
IF(HAVE_GD_H)
   CHECK_INCLUDE_FILE(gdfontl.h HAVE_GDFONTL_H)
   CHECK_INCLUDE_FILE(gdfontt.h HAVE_GDFONTT_H)
   CHECK_INCLUDE_FILE(gdfonts.h HAVE_GDFONTS_H)
   CHECK_INCLUDE_FILE(gdfontmb.h HAVE_GDFONTMB_H)
   CHECK_INCLUDE_FILE(gdfontg.h HAVE_GDFONTG_H)
   FIND_LIBRARY(GD_LIBRARY NAMES gd bgd DOC "The GD library")
   IF(GD_LIBRARY)
      #CHECK_LIBRARY_EXISTS(gd gdImagePng "." HAVE_GD_LIB)
      TARGET_LINK_LIBRARIES(sarg ${GD_LIBRARY})
      SET(HAVE_GD_LIB GD_LIBRARY CACHE PATH DOC "True IF GD was found")
      SET(HAVE_GD_LIB CACHE BOOL DOC "True IF must use GD")
   ENDIF(GD_LIBRARY)
ENDIF(HAVE_GD_H)

# Find iconv
OPTION(ENABLE_ICONV "Enable the usage of iconv" ON)
IF(ENABLE_ICONV)
   CHECK_INCLUDE_FILE(iconv.h HAVE_ICONV_H)
   IF(HAVE_ICONV_H)
      FIND_PATH(ICONV_INCLUDE_PATH NAMES iconv.h)
      FIND_LIBRARY(ICONV_LIBRARY NAMES iconv DOC "The ICONV library")
      IF(ICONV_LIBRARY)
         TARGET_LINK_LIBRARIES(sarg ${ICONV_LIBRARY})
      ENDIF(ICONV_LIBRARY)
      CHECK_FUNCTION_EXISTS(iconv_open HAVE_ICONV)

      IF(NOT USE_CONST_ICONV)
         SET(ICONV_CONST_FILE ${CMAKE_BINARY_DIR}/consticonv.c)
         FILE(WRITE ${ICONV_CONST_FILE} "
#include <iconv.h>
#ifdef __CLASSIC_C__
int main(){
  int ac;
  char*av[];
#else
int main(int ac, char*av[]){
#endif
  iconv_t localtoutf=(iconv_t)-1;
  const char *str=\"\";
  char *sstr=\"\";
  size_t slen=0, sslen=0;

  iconv (localtoutf, (const char **)&str, &slen, &sstr, &sslen);
  if(ac > 1000)
    {
    return *av[0];
    }
  return 0;
}
")
         TRY_COMPILE(USE_CONST_ICONV ${CMAKE_BINARY_DIR} ${ICONV_CONST_FILE} CMAKE_FLAGS -DLINK_LIBRARIES="iconv")
         FILE(REMOVE ${ICONV_CONST_FILE})
         IF(USE_CONST_ICONV)
            SET(ICONV_CONST "const" CACHE STRING "The type of char to use with iconv")
            MESSAGE(STATUS "iconv requires const char **")
         ELSEIF(USE_CONST_ICONV)
            SET(ICONV_CONST "" CACHE STRING "The type of char to use with iconv")
            MESSAGE(STATUS "iconv requires char **")
         ENDIF(USE_CONST_ICONV)
         SET(USE_CONST_ICONV TRUE CACHE BOOL "Use const char ** with iconv")
      ENDIF(NOT USE_CONST_ICONV)
   ENDIF(HAVE_ICONV_H)
ENDIF(ENABLE_ICONV)

# Find ldap
CHECK_INCLUDE_FILE(ldap.h HAVE_LDAP_H)
IF(HAVE_LDAP_H)
   FIND_LIBRARY(LDAP_LIBRARY NAMES ldap DOC "The ldap library")
   IF(LDAP_LIBRARY)
      TARGET_LINK_LIBRARIES(sarg ${LDAP_LIBRARY})
      SET(HAVE_LDAP LDAP_LIBRARY CACHE PATH DOC "True if LDAP was found")
      SET(HAVE_LDAP CACHE BOOL DOC "True if must use LDAP")
   ENDIF(LDAP_LIBRARY)
ENDIF(HAVE_LDAP_H)

# Support for large files
OPTION(ENABLE_LARGEFILE "Enable the usage of large files" ON)
IF(ENABLE_LARGEFILE)
   IF(LFS_SEARCHED MATCHES "^LFS_SEARCHED$")
      SET(SEARCH_LARGEFILE TRUE)
   ENDIF(LFS_SEARCHED MATCHES "^LFS_SEARCHED$")
ENDIF(ENABLE_LARGEFILE)

IF(SEARCH_LARGEFILE)
   SET(LFS_SEARCHED TRUE CACHE INTERNAL "LFS has been searched")
   MESSAGE(STATUS "Looking for large file support")
   SET(LFS_FILE "${CMAKE_BINARY_DIR}/lfsfopen64.c")
   FILE(WRITE ${LFS_FILE} "
#include <stdio.h>
int main(void)
{
FILE *f = fopen64 (\"/tmp/foo\",\"r\");
return(0);
}
")
   TRY_COMPILE(HAVE_FOPEN64 ${CMAKE_BINARY_DIR} ${LFS_FILE} COMPILE_DEFINITIONS "-Wall -Werror")
   IF(HAVE_FOPEN64)
      MESSAGE(STATUS "Looking for large file support - found")
   ELSE(HAVE_FOPEN64)
      SET(LFS_NEED_DEFINE FALSE CACHE BOOL "Does LFS require _LARGEFILE64_SOURCE ?")
      MESSAGE(STATUS "Looking for large file support with _LARGEFILE64_SOURCE")
      FILE(WRITE ${LFS_FILE} "
#define _LARGEFILE64_SOURCE
#include <stdio.h>
int main(void)
{
FILE *f = fopen64 (\"/tmp/foo\",\"r\");
return(0);
}
")
      TRY_COMPILE(HAVE_FOPEN64 ${CMAKE_BINARY_DIR} ${LFS_FILE} OUTPUT_VARIABLE LFS_OUTPUT)
      IF(HAVE_FOPEN64)
         MESSAGE(STATUS "Looking for large file support with _LARGEFILE64_SOURCE - found")
         SET(LFS_NEED_DEFINE TRUE CACHE BOOL "Does LFS require _LARGEFILE64_SOURCE ?" FORCE)
      ELSE(HAVE_FOPEN64)
         MESSAGE("Failed with output ${LFS_OUTPUT}")
      ENDIF(HAVE_FOPEN64)
   ENDIF(HAVE_FOPEN64)
   FILE(REMOVE ${LFS_FILE})
ENDIF(SEARCH_LARGEFILE)

IF(LFS_NEED_DEFINE)
   ADD_DEFINITIONS(-D_LARGEFILE64_SOURCE)
ENDIF(LFS_NEED_DEFINE)

# Find rlim_t
IF(HAVE_SYS_RESOURCE_H)
   SET(CMAKE_EXTRA_INCLUDE_FILES "sys/resource.h")
ENDIF(HAVE_SYS_RESOURCE_H)
CHECK_TYPE_SIZE(rlim_t RLIM_T)
IF(HAVE_RLIM_T)
   IF(RLIM_T STREQUAL 4)
      SET(RLIM_STRING "%d")
   ELSEIF(RLIM_T STREQUAL 8)
      SET(RLIM_STRING "%lli")
   ELSE(RLIM_T STREQUAL 4)
      MESSAGE("Unknown rlim_t size")
   ENDIF(RLIM_T STREQUAL 4)
#ELSE(HAVE_RLIM_T)
#   MESSAGE("Cannot detect the size of your system's rlim_t type")
ENDIF(HAVE_RLIM_T)

CHECK_FUNCTION_EXISTS(mkstemp HAVE_MKSTEMP)
IF(NOT HAVE_MKSTEMP)
   # MingW provide mkstemps in libiberty but doesn't declare it in any header file
   FIND_LIBRARY(IBERTY_LIB NAMES iberty DOC "The library of mingw containing mkstemps")
   IF(IBERTY_LIB)
      TARGET_LINK_LIBRARIES(sarg iberty)
   ENDIF(IBERTY_LIB)
ENDIF(NOT HAVE_MKSTEMP)

# Gettext and friends
IF(NOT HAVE_LC_MESSAGES)
   SET(FILES)
   IF(HAVE_LOCALE_H)
      LIST(APPEND FILES "locale.h")
   ENDIF(HAVE_LOCALE_H)
   IF(HAVE_LIBINTL_H)
      LIST(APPEND FILES "libintl.h")
   ENDIF(HAVE_LIBINTL_H)
   LIST(LENGTH FILES NFILES)
   IF(NFILES GREATER 0)
      CHECK_SYMBOL_EXISTS(LC_MESSAGES "${FILES}" HAVE_LC_MESSAGES)
   ENDIF(NFILES GREATER 0)
ENDIF(NOT HAVE_LC_MESSAGES)

# System particularities
IF(CMAKE_SYSTEM_NAME STREQUAL "solaris")
   TARGET_LINK_LIBRARIES(sarg socket nsl)
   #SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_DEFINITIONS="-DSOLARIS ${COMPILE_DEFINITIONS}")
   ADD_DEFINITIONS(-DSOLARIS)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "solaris")

OPTION(ENABLE_EXTRA_PROTECT "Enable compile and runtime extra protections" OFF)
IF(ENABLE_EXTRA_PROTECT)
   GET_TARGET_PROPERTY(TMPCFLAGS sarg COMPILE_FLAGS)
   SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_FLAGS "${TMPCFLAGS} -fstack-protector -D_FORTIFY_SOURCE=2 -Werror")
   CHECK_C_COMPILER_FLAG("-Wformat -Werror=format-security" HAVE_WERROR_FORMAT_SECURITY)
   IF(HAVE_WERROR_FORMAT_SECURITY)
      GET_TARGET_PROPERTY(TMPCFLAGS sarg COMPILE_FLAGS)
      SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_FLAGS "${TMPCFLAGS} -Wformat -Werror=format-security")
   ENDIF(HAVE_WERROR_FORMAT_SECURITY)
ENDIF(ENABLE_EXTRA_PROTECT)

# Compile sarg for debugging
OPTION(ENABLE_DEBUG "Compile sarg with debugging symbols" OFF)
IF(ENABLE_DEBUG)
   GET_TARGET_PROPERTY(TMPCFLAGS sarg COMPILE_FLAGS)
   SET_TARGET_PROPERTIES(sarg PROPERTIES COMPILE_FLAGS "${TMPCFLAGS} -g")
ENDIF(ENABLE_DEBUG)

# Enable double check of the data written in the reports
OPTION(ENABLE_DOUBLE_CHECK_DATA "Make sarg double check the data it manipulates and output a warning if an error is found" OFF)

# Save the configuration for the project
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/include/config.h.in" "${CMAKE_BINARY_DIR}/config.h" @ONLY)

# Installation of the project
INSTALL(TARGETS sarg DESTINATION "${BINDIR}")
INSTALL(FILES sarg.conf DESTINATION "${SYSCONFDIR}" RENAME sarg.conf.exemple)
INSTALL(FILES exclude_codes user_limit_block css.tpl DESTINATION "${SYSCONFDIR}")
INSTALL(FILES sarg.1 DESTINATION "${MANDIR}")
INSTALL(DIRECTORY languages/ DESTINATION "${LANGDIR}" PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY images/ DESTINATION "${IMAGEDIR}" PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY fonts/ DESTINATION "${FONTDIR}" FILES_MATCHING PATTERN "*.ttf" PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY sarg-php/ DESTINATION "${SARGPHPDIR}" PATTERN ".svn" EXCLUDE)

# Package creation
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Squid Analysis Report Generator")
SET(CPACK_PACKAGE_VERSION_MAJOR "${sarg_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${sarg_REVISION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${sarg_BUILD}")
SET(CPACK_GENERATOR TGZ)
SET(CPACK_SOURCE_GENERATOR TGZ)
SET(CPACK_SOURCE_IGNORE_FILES "/.svn/" "/debian/" "/html/" "Makefile$" "config\\\\.log$" "config\\\\.status$" ".*~$" "\\\\.o$")
INCLUDE(CPack)
