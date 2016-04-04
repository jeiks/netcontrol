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

#define INITIAL_LINE_BUFFER_SIZE 32768

struct longlinestruct
{
   //! The buffer to store the data read from the log file.
   char *buffer;
   //! The size of the buffer.
   size_t size;
   //! The number of bytes stored in the buffer.
   size_t length;
   //! The position of the beginning of the current string.
   size_t start;
   //! The position of the end of the current string.
   size_t end;
};

longline longline_create(void)
{
   longline line;

   line=malloc(sizeof(*line));
   if (line==NULL) return(NULL);
   line->size=INITIAL_LINE_BUFFER_SIZE;
   line->buffer=malloc(line->size);
   if (line->buffer==NULL) {
      free(line);
      return(NULL);
   }
   line->start=0;
   line->end=0;
   line->length=0;
   return(line);
}

void longline_reset(longline line)
{
   if (line!=NULL) {
      line->start=0;
      line->end=0;
      line->length=0;
   }
}

char *longline_read(FILE *fp_in,longline line)
{
   int i;
   char *newbuf;
   size_t nread;

   if (line==NULL || line->buffer==NULL) return(NULL);

   while (true) {
      for (i=line->end ; i<line->length && (line->buffer[i]=='\n' || line->buffer[i]=='\r') ; i++);
      if (i<line->length) {
         line->end=i;
         break;
      }
      nread=(feof(fp_in)!=0) ? 0 : fread(line->buffer,1,line->size,fp_in);
      if (nread==0) return(NULL);
      line->length=nread;
      line->end=0;
   }

   line->start=line->end;
   while (true) {
      for (i=line->end ; i<line->length ; i++) {
         if ((unsigned char)line->buffer[i]>=' ') continue;
         if (line->buffer[i]=='\n' || line->buffer[i]=='\r') break;
      }

      line->end=i;
      if (line->end<line->length) break;

      if (line->start>0) {
         for (i=line->start ; i<line->length ; i++) line->buffer[i-line->start]=line->buffer[i];
         line->length-=line->start;
         line->end-=line->start;
         line->start=0;
      }
      if (line->length>=line->size) {
         line->size+=8192;
         newbuf=realloc(line->buffer,line->size);
         if (!newbuf) {
            debuga(_("Not enough memory to read one more line from the input log file\n"));
            exit(EXIT_FAILURE);
         }
         line->buffer=newbuf;
      }
      nread=(feof(fp_in)!=0) ? 0 : fread(line->buffer+line->length,1,line->size-line->length,fp_in);
      if (nread==0) {
         if (line->end<=line->start) return(NULL);
         if (line->end>=line->size) {
            line->end=line->size;
            line->size++;
            newbuf=realloc(line->buffer,line->size);
            if (!newbuf) {
               debuga(_("Not enough memory to read one more line from the input log file\n"));
               exit(EXIT_FAILURE);
            }
            line->buffer=newbuf;
         }
         line->buffer[line->end]='\0';
         return(line->buffer+line->start);
      }
      line->length+=nread;
   }
   line->buffer[line->end++]='\0';
   return(line->buffer+line->start);
}

void longline_destroy(longline *line_ptr)
{
   longline line;

   if (line_ptr==NULL || *line_ptr==NULL) return;
   line=*line_ptr;
   *line_ptr=NULL;
   if (line->buffer!=NULL) free(line->buffer);
   free(line);
}
