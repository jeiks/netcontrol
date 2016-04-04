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

struct hostip4struct
{
   //! The IP address.
   unsigned long int address;
   //! The mask to match the address of the URL.
   unsigned long int mask;
};

struct hostnamestruct
{
   //! The URL to match without any leading wildcard.
   char *url;
   //! The number of dots in the url if a wildcard is present or -1 if the address is complete (no wildcard)
   int ndots;
};

static struct hostip4struct *exclude_ip4=NULL;
static int num_exclude_ip4=0;
static struct hostnamestruct *exclude_name=NULL;
static int num_exclude_name=0;
static int ip4allocated=0;
static int nameallocated=0;

static char *excludeuser=NULL;

static void store_exclude_ip4(unsigned short int *addr,int mask)
{
   int i;

   if (num_exclude_ip4>=ip4allocated) {
      struct hostip4struct *temp;

      ip4allocated+=5;
      temp=realloc(exclude_ip4,ip4allocated*sizeof(*temp));
      if (temp==NULL) {
         debuga(_("Not enough memory to store the exlcluded IP addresses\n"));
         exit(EXIT_FAILURE);
      }
      exclude_ip4=temp;
   }
   exclude_ip4[num_exclude_ip4].address=0UL;
   for (i=0 ; i<4 ; i++)
      exclude_ip4[num_exclude_ip4].address=(exclude_ip4[num_exclude_ip4].address<<8) | (unsigned char)(addr[i] & 0xFFU);
   exclude_ip4[num_exclude_ip4].mask=(0xFFFFFFFFUL << (32-mask));
   num_exclude_ip4++;
}

static void store_exclude_url(char *url,int length)
{
   int start;
   int i;
   int ndots, firstdot;
   struct hostnamestruct *item;

   start=0;
   ndots=-1;
   firstdot=0;
   for (i=0 ; i<length ; i++)
      if (url[i]=='*') {
         firstdot=1;
      } else if (url[i]=='.') {
         if (firstdot) {
            firstdot=0;
            ndots=1;
            start=i+1;
         } else if (ndots>=0)
            ndots++;
      }
   if (start>=length || firstdot) return;
   if (start>0) {
      url+=start;
      length-=start;
   }

   if (num_exclude_name>=nameallocated) {
      struct hostnamestruct *temp;

      nameallocated+=5;
      temp=realloc(exclude_name,nameallocated*sizeof(*temp));
      if (temp==NULL) {
         debuga(_("Not enough memory to store the excluded URLs\n"));
         exit(EXIT_FAILURE);
      }
      exclude_name=temp;
   }

   item=exclude_name+num_exclude_name;
   num_exclude_name++;
   item->url=malloc(length+1);
   if (!item->url) {
      debuga(_("Not enough memory to store the excluded URLs\n"));
      exit(EXIT_FAILURE);
   }
   strncpy(item->url,url,length);
   item->url[length]='\0';
   item->ndots=(ndots>0) ? ndots : -1;
}


void gethexclude(const char *hexfile, int debug)
{
   FILE *fp_ex;
   char buf[255];
   int i;
   int ip_size;
   unsigned int value4, value6;
   unsigned short int addr[8];
   int addr_len;
   int mask, max_mask;

   if(access(hexfile, R_OK) != 0) {
      debuga(_("Cannot open exclude_hosts file: %s - %s\n"),hexfile,strerror(errno));
      exit(EXIT_FAILURE);
   }
   if(debug)
      debuga(_("Loading exclude host file from: %s\n"),hexfile);

   if ((fp_ex = fopen(hexfile, "r")) == NULL) {
      debuga(_("(gethexclude) Cannot open file %s - %s\n"),hexfile,strerror(errno));
      exit(EXIT_FAILURE);
   }

   while(fgets(buf,sizeof(buf),fp_ex)!=NULL){
      if(buf[0]=='#')
         continue;
      fixendofline(buf);
      ip_size=0x60  | 0x04;
      value4=0U;
      value6=0U;
      addr_len=0;
      for (i=0 ; (unsigned char)buf[i]>' ' && buf[i]!='/' ; i++) {
         if (ip_size & 0x04) {
            if (isdigit(buf[i])) {
               value4=value4*10+(buf[i]-'0');
               if (value4>0xFFU) ip_size&=~0x04;
            } else if (buf[i]=='.' && addr_len<4) {
               addr[addr_len++]=(unsigned short)(value4 & 0xFFU);
               value4=0U;
            } else {
               ip_size&=~0x04;
            }
         }
         if (ip_size & 0x60) {
            if (isdigit(buf[i])) {
               value6=(value6<<4)+(buf[i]-'0');
               if (value6>0xFFFFU) ip_size&=~0x60;
            } else if (toupper(buf[i])>='A' && toupper(buf[i])<='F') {
               value6=(value6<<4)+(toupper(buf[i])-'A'+10);
               if (value6>0xFFFFU) ip_size&=~0x60;
            } else if (buf[i]==':' && addr_len<8) {
               addr[addr_len++]=(unsigned short)(value6 & 0xFFFFU);
               value6=0U;
            } else {
               ip_size&=~0x60;
            }
         }
      }
      if (i==0) continue;
      if (ip_size & 0x04) {
         if (addr_len!=3)
            ip_size&=~0x04;
         else
            addr[addr_len++]=(unsigned short)(value4 & 0xFFU);
      }
      if (ip_size & 0x60) {
         if (addr_len>=8)
            ip_size&=~0x60;
         else
            addr[addr_len++]=(unsigned short)(value6 & 0xFFFFU);
      }
      if (ip_size) {
         max_mask=(ip_size & 0x04) ? 4*8 : 8*16;
         if (buf[i]=='/') {
            mask=atoi(buf+i+1);
            if (mask<0 || mask>max_mask) mask=max_mask;
         } else
            mask=max_mask;
         if (ip_size & 0x04)
            store_exclude_ip4(addr,mask);
         else {
            debuga(_("IPv6 addresses are not supported (found in %s)\n"),hexfile);
            exit(EXIT_FAILURE);
         }
      } else {
         store_exclude_url(buf,i);
      }
   }

   fclose(fp_ex);
   return;
}

int vhexclude(const char *url)
{
   int i, j;
   int length;
   int ip_size;
   unsigned int value4, value6;
   unsigned long int addr4;
   unsigned short int addr6[8];
   int addr_len;
   int dotpos[10];
   int ndots;

   ip_size=0x60  | 0x04;
   addr4=0UL;
   value4=0U;
   value6=0U;
   addr_len=0;
   for (i=0 ; (unsigned char)url[i]>' ' && url[i]!='/' && url[i]!='?'&& ((ip_size & 0x60)!=0 || url[i]!=':') && ip_size ; i++) {
      if (ip_size & 0x04) {
         if (isdigit(url[i])) {
            value4=value4*10+(url[i]-'0');
            if (value4>0xFFU) ip_size&=~0x04;
         } else if (url[i]=='.' && addr_len<4) {
            addr_len++;
            addr4=(addr4<<8) | (unsigned long int)(value4 & 0xFFU);
            value4=0U;
         } else {
            ip_size&=~0x04;
         }
      }
      if (ip_size & 0x60) {
         if (isdigit(url[i])) {
            value6=(value6<<4)+(url[i]-'0');
            if (value6>0xFFFFU) ip_size&=~0x60;
         } else if (toupper(url[i])>='A' && toupper(url[i])<='F') {
            value6=(value6<<4)+(toupper(url[i])-'A'+10);
            if (value6>0xFFFFU) ip_size&=~0x60;
         } else if (url[i]==':' && addr_len<8) {
            addr6[addr_len++]=(unsigned short)(value6 & 0xFFFFU);
            value6=0U;
         } else {
            ip_size&=~0x60;
         }
      }
   }
   if ((ip_size & 0x04) && addr_len==3) {
      if (exclude_ip4 == NULL) return(1);
      addr4=(addr4<<8) | (unsigned long int)(value4 & 0xFFU);
      for (i=0 ; i<num_exclude_ip4 ; i++)
         if (((exclude_ip4[i].address ^ addr4) & exclude_ip4[i].mask)==0) return(0);
   } else if ((ip_size & 0x60) && addr_len<8) {
      addr6[addr_len++]=(unsigned short)(value6 & 0xFFFFU);
   } else {
      if (exclude_name == NULL) return(1);
      ndots=0;
      for (length=0 ; (unsigned char)url[length]>' ' && url[length]!=':' && url[length]!='/' && url[length]!='?' ; length++)
         if (url[length]=='.') {
            /*
            We store the position of each dots of the URL to match it against any
            wildcard in the excluded list. The size of dotpos is big enough for the most
            ambitious URL but we have a safety mechanism that shift the positions should there be too
            many dots in the URL.
            */
            if (ndots<sizeof(dotpos)/sizeof(dotpos[0]))
               dotpos[ndots++]=length+1;
            else {
               for (j=1 ; j<ndots ; j++) dotpos[j-1]=dotpos[j];
               dotpos[ndots-1]=length+1;
            }
         }
      if (length>0) {
         for (i=0 ; i<num_exclude_name ; i++) {
            if (exclude_name[i].ndots>0) {
               const char *wurl=url;
               int len=length;
               if (exclude_name[i].ndots<=ndots) {
                  wurl+=dotpos[ndots-exclude_name[i].ndots];
                  len-=dotpos[ndots-exclude_name[i].ndots];
               }
               if (strncmp(exclude_name[i].url,wurl,len)==0 && exclude_name[i].url[len]=='\0') return(0);
            } else {
               if (strncmp(exclude_name[i].url,url,length)==0 && exclude_name[i].url[length]=='\0') return(0);
            }
         }
      }
   }

   return(1);
}


void getuexclude(const char *uexfile, int debug)
{

   FILE *fp_ex;
   char buf[255];
   long int nreg=0;

   if(debug)
      debuga(_("Loading exclude file from: %s\n"),uexfile);

   if ((fp_ex = fopen(uexfile, "r")) == NULL) {
      debuga(_("(gethexclude) Cannot open file %s - %s\n"),uexfile,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (fseek(fp_ex, 0, SEEK_END)==-1) {
      debuga(_("Failed to move till the end of the excluded users file %s: %s\n"),uexfile,strerror(errno));
      exit(EXIT_FAILURE);
   }
   nreg = ftell(fp_ex);
   if (nreg<0) {
      debuga(_("Cannot get the size of file %s\n"),uexfile);
      exit(EXIT_FAILURE);
   }
   nreg += 11;
   if (fseek(fp_ex, 0, SEEK_SET)==-1) {
      debuga(_("Failed to rewind the excluded users file %s: %s\n"),uexfile,strerror(errno));
      exit(EXIT_FAILURE);
   }

   if((excludeuser=(char *) malloc(nreg))==NULL){
      debuga(_("malloc error (%ld bytes required)\n"),nreg);
      exit(EXIT_FAILURE);
   }

   bzero(excludeuser,nreg);

   while(fgets(buf,sizeof(buf),fp_ex)!=NULL){
     if(strchr(buf,'#') != NULL)
        continue;
     fixendofline(buf);
     strcat(excludeuser,buf);
     strcat(excludeuser," ");
   }

   strcat(excludeuser,"*END* ");

   fclose(fp_ex);

   return;
}

int vuexclude(const char *user)
{
   const char *wuser;
   int len;

   if (excludeuser) {
      len=strlen(user);
      wuser=excludeuser;
      while ((wuser=strstr(wuser,user))!=NULL) {
         if (wuser[len]==' ') return(0);
         wuser+=len;
      }
   }

   return(1);
}

bool is_indexonly(void)
{
   if (excludeuser==NULL) return(false);
   return(strstr(excludeuser,"indexonly") != NULL);
}

void free_exclude(void)
{
   int i;

   if (exclude_ip4) {
      free(exclude_ip4);
      exclude_ip4=NULL;
   }

   if (exclude_name) {
      for (i=0 ; i<num_exclude_name ; i++)
         if (exclude_name[i].url) free(exclude_name[i].url);
      free(exclude_name);
      exclude_name=NULL;
   }

   if(excludeuser) {
      free(excludeuser);
      excludeuser=NULL;
   }
}
