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
#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h> //define getnameinfo on windows 
#endif

void ip2name(char *ip,int ip_len)
{
#ifdef HAVE_GETNAMEINFO
   struct sockaddr_storage sa;
   char host[NI_MAXHOST];
   int n1,n2,n3,n4,next=0;
   int error;

   memset(&sa,0,sizeof(sa));
   if (sscanf(ip,"%d.%d.%d.%d%n",&n1,&n2,&n3,&n4,&next)==4 && ip[next]=='\0') {
      struct sockaddr_in *s4=(struct sockaddr_in *)&sa;
      if (inet_pton(AF_INET,ip,&s4->sin_addr)!=1) return;
      sa.ss_family=AF_INET;
   } else {
      struct sockaddr_in6 *s6=(struct sockaddr_in6 *)&sa;
      if (inet_pton(AF_INET6,ip,&s6->sin6_addr)!=1) return;
      sa.ss_family=AF_INET6;
   }
   error=getnameinfo((struct sockaddr *)&sa,sizeof(sa),host,sizeof(host),NULL,0,0);
   if (error==0)
   {
      strncpy(ip,host,ip_len-1);
      ip[ip_len-1]='\0';
   } else {
      debuga(_("IP to name resolution (getnameinfo) on IP address %s failed with error %d - %s\n"),ip,error,gai_strerror(error));
   }
#else
   struct in_addr addr;
   struct hostent *hp;
   char **p;

#ifdef HAVE_INET_ATON
   if (inet_aton(ip,&addr) == 0)
      return;
#else
   addr.s_addr=inet_addr(ip);
   if (addr.s_addr==-1) return;
#endif

   hp = gethostbyaddr((void *)&addr, sizeof (addr), AF_INET);
   if (hp == NULL)
      return;

   for (p = hp->h_addr_list; *p != 0; p++) {
      struct in_addr in;

      (void) memcpy(&in.s_addr, *p, sizeof (in.s_addr));
      strncpy(ip,hp->h_name,ip_len-1);
      ip[ip_len-1]=0;
   }
#endif
   return;
}

void name2ip(char *name)
{
   struct in_addr ia;
   struct hostent *hp;
   char *port;
   char n1[4];
   char n2[4];
   char n3[4];
   char n4[4];
   struct getwordstruct gwarea;

   port=strchr(name,':');
   if (port) *port=0;

   if((hp=gethostbyname(name))==NULL)
      return;

   memcpy(&ia.s_addr,hp->h_addr_list[0],sizeof(ia.s_addr));
   ia.s_addr=ntohl(ia.s_addr);
   getword_start(&gwarea,inet_ntoa(ia));
   if (getword(n4,sizeof(n4),&gwarea,'.')<0 || getword(n3,sizeof(n3),&gwarea,'.')<0 ||
         getword(n2,sizeof(n2),&gwarea,'.')<0 || getword(n1,sizeof(n1),&gwarea,0)<0) {
      printf("SARG: Maybe you have a broken record or garbage in your %s ip address.\n",gwarea.beginning);
      exit(EXIT_FAILURE);
   }
   sprintf(name,"%s.%s.%s.%s",n1,n2,n3,n4);

   return;
}
