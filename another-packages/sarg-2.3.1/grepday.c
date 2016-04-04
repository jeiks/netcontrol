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

#if defined(HAVE_GD)

#if defined(HAVE_ICONV_H) && defined(gdFTEX_Unicode)
#include <iconv.h>
#define USE_ICONV 1
#endif

struct GraphDataStruct
{
   int lavender;
   int darkblue;
   int dimgray;
   int goldenrod;
   int goldenrod2;
   int gray;
   int silver;
   int black;
   //! The color of the top side of a graph bar.
   int color1;
   //! The color of the right side of a graph bar.
   int color2;
   //! The color of the front side of a graph bar.
   int color3;
   //! The libgd image we are drawing on.
   gdImage *im;
   //! An allocated buffer to convert the string into UTF-8.
   char *string;
   //! The number of bytes allocated for the string buffer.
   size_t string_size;
   //! The bottom border of the graph.
   int BottomGraph;
   //! The top border of the graph.
   int TopGraph;
   //! The left border of the graph.
   int LeftGraph;
   //! The right border of the graph.
   int RightGraph;
   //! The height at which the bottom depth border of the graph extends.
   int BottomDepth;
   //! The distance between two ticks on the horizontal axis.
   double XScale;
   //! The distance between two ticks on the vertical axis.
   double YScale;
   //! The exterior length of a tick on the scales.
   int TickLength;
   //! The distance, in pixels, between two ticks along the Y axis.
   int YTickSpace;
};

enum PlotType
{
   PTG_LinBin,
   PTG_LogBin,
   PTG_Time,
};

struct PlotStruct
{
   //! The data points to plot.
   long long int *datapoints;
   //! The number of points to plot.
   int npoints;
   //! The minimum data to plot along the Y axis.
   long long int ymin;
   //! The maximum data to plot along the Y axis.
   long long int ymax;
   //! The type of Y axis to draw.
   enum PlotType ytype;
   //! The label to write on the X axis.
   const char *XLabel;
   //! The label to write on the Y axis.
   const char *YLabel;
   //! The name of the output PNG file.
   const char *pngfile;
};

enum TextRefPos
{
   TRP_TopLeft,
   TRP_TopCenter,
   TRP_TopRight,
   TRP_BottomLeft,
   TRP_BottomCenter,
   TRP_BottomRight,
   TRP_CenterLeft,
   TRP_Center,
   TRP_CenterRight,
};

#ifdef USE_ICONV
//! The iconv object to convert the text from the locale character set to UTF-8.
iconv_t localtoutf=(iconv_t)-1;
#endif

static void Sarg_gdImageStringFT (struct GraphDataStruct *gdata, int fg, char *fontlist,
                         double ptsize, double angle, int x, int y, const char *string,enum TextRefPos RefPos)
{
   char *sstring;
   char *retval;
   int brect[8];
   int minx,miny,maxx,maxy;
   int i;

#ifdef USE_ICONV
   if (localtoutf!=(iconv_t)-1) {
      const char *str;
      char *sstr;
      size_t slen, sslen;

      slen = strlen(string) + 1; // We must include string termination character
      sslen = slen * 3;          // We assume that the UTF8 string will not be bigger than 3 times the original size.
      if (sslen>gdata->string_size) {
         sstring = (char *)realloc(gdata->string,sslen);
         if (!sstring) {
            debuga(_("realloc error (%"PRIu64" bytes required)\n"),(uint64_t)sslen);
            exit(EXIT_FAILURE);
         }
         gdata->string=(char *)sstring;
         gdata->string_size=sslen;
      } else {
         sstring=gdata->string;
         sslen=gdata->string_size;
      }

      str = string;
      sstr = sstring;
      if (iconv (localtoutf, (ICONV_CONST char **)&str, &slen, &sstr, &sslen)==-1) {
         debuga(_("(grepday) iconv failed to convert string \"%s\" from %s to UTF-8 - %s\n"),string,CharSet,strerror(errno));
         sstring=(char *)string; //show something sensible on the graph
      }
   } else {
      sstring=(char *)string; //show something sensible on the graph
   }
#else
   sstring=(char *)string;
#endif

   if (RefPos!=TRP_BottomLeft) {
      retval = gdImageStringFTEx (NULL, brect, fg, fontlist, ptsize, angle, 0, 0, sstring, gdFTEX_Unicode);
      if (retval) {
         debuga(_("libgd failed to calculate the bounding box of the text \"%s\": %s\n"),sstring,retval);
         exit(EXIT_FAILURE);
      }
      /*
      From libgd documentation, brect contains this without taking into account the angle:
      0  lower left corner, X position
      1  lower left corner, Y position
      2  lower right corner, X position
      3  lower right corner, Y position
      4  upper right corner, X position
      5  upper right corner, Y position
      6  upper left corner, X position
      7  upper left corner, Y position
      */
      minx=maxx=brect[0];
      miny=maxy=brect[1];
      for (i=2 ; i<7 ; i+=2) {
         if (minx>brect[i]) minx=brect[i];
         if (maxx<brect[i]) maxx=brect[i];
         if (miny>brect[i+1]) miny=brect[i+1];
         if (maxy<brect[i+1]) maxy=brect[i+1];
      }
   }

   switch (RefPos)
   {
      case TRP_TopLeft:
         y-=miny;
         break;

      case TRP_TopCenter:
         x-=(maxx-minx)/2;
         y-=miny;
         break;

      case TRP_TopRight:
         x-=maxx;
         y-=miny;
         break;

      case TRP_BottomLeft:
         break;

      case TRP_BottomCenter:
         x-=(maxx-minx)/2;
         break;

      case TRP_BottomRight:
         x-=maxx;
         break;

      case TRP_Center:
         x-=(maxx-minx)/2;
         y+=(maxy-miny)/2;
         break;

      case TRP_CenterLeft:
         y+=(maxy-miny)/2;
         break;

      case TRP_CenterRight:
         x-=maxx;
         y+=(maxy-miny)/2;
         break;
   }
   retval = gdImageStringFTEx (gdata->im, brect, fg, fontlist, ptsize, angle, x, y, sstring, gdFTEX_Unicode);
   if (retval) {
      debuga(_("libgd failed to render the text \"%s\": %s\n"),sstring,retval);
      exit(EXIT_FAILURE);
   }
}

static void bar(struct GraphDataStruct *gdata,int x1,double y,const char *label)
{
   gdPoint points[4];
   int val=0;
   int width2;

   val=gdata->BottomGraph+5-(int)(y*gdata->YScale+0.5);
   width2=(int)(gdata->XScale/4.);

   // front side of the bar
   if (val<gdata->BottomDepth)
      gdImageFilledRectangle(gdata->im, x1-width2, val, x1+width2, gdata->BottomDepth, gdata->color3);

   // top side of the bar
   points[0].x = x1-width2+5;
   points[0].y = val-5;
   points[1].x = x1-width2;
   points[1].y = val;
   points[2].x = x1+width2;
   points[2].y = val;
   points[3].x = x1+width2+5;
   points[3].y = val-5;
   gdImageFilledPolygon(gdata->im, points, 4, gdata->color1);

   gdImageLine(gdata->im, x1+2, val-2, x1+2, val-10, gdata->dimgray);
   gdImageFilledRectangle(gdata->im, x1-8, val-20, x1+12, val-10, gdata->goldenrod);
   gdImageRectangle(gdata->im, x1-8, val-20, x1+12, val-10, gdata->goldenrod2);

   Sarg_gdImageStringFT(gdata,gdata->black,GraphFont,6,0.0,x1+2,val-12,label,TRP_BottomCenter);

   // lateral side of the bar
   if (val<gdata->BottomDepth) {
      points[0].x = x1+width2+5;
      points[0].y = val-5;
      points[1].x = x1+width2;
      points[1].y = val;
      points[2].x = x1+width2;
      points[2].y = gdata->BottomDepth;
      points[3].x = x1+width2+5;
      points[3].y = gdata->BottomGraph;
      gdImageFilledPolygon(gdata->im, points, 4, gdata->color2);
   }

   return;
}

static int greport_compute_yaxis(struct PlotStruct *pdata,struct GraphDataStruct *gdata)
{
   double symin,symax;
   double range;
   double yscale;
   double ymag;

   if (pdata->ymin<0.) {
      debuga(_("Minimum for Y scale of the graph is out of range: %"PRId64"\n"),(int64_t)pdata->ymin);
      return(-1);
   }
   if (pdata->ymax<=0.) {
      debuga(_("Maximum for Y scale of the graph is out of range: %"PRId64"\n"),(int64_t)pdata->ymax);
      return(-1);
   }

   switch(pdata->ytype)
   {
      case PTG_LinBin:
         symin=(double)pdata->ymin;
         symax=(double)pdata->ymax;
         ymag=1000.;
         break;

      case PTG_LogBin:
         if (pdata->ymin>0.)
            symin=log(pdata->ymin);
         else
            symin=0.;
         symax=log(pdata->ymax);
         ymag=1000.;
         break;

      case PTG_Time:
         symin=(double)pdata->ymin;
         symax=(double)pdata->ymax;
         ymag=1.;
         break;

      default:
         debuga(_("Unknown type %d for Y axis scale\n"),pdata->ytype);
         return(-1);
   }
   gdata->YTickSpace=10;

   range=symax-symin;
   yscale=(double)(gdata->BottomGraph-gdata->TopGraph)/range;
   gdata->YScale=yscale;
   return(0);
}

static void greport_formatbin(double yval,int maxdigits,char *string,int slen)
{
   int len;
   char schar[]={'\0','k','M','G','T','P'};
   int scount;
   int i;
   int ndigits;

   for (scount=0 ; scount<sizeof(schar)/sizeof(*schar) && yval>=1000. ; scount++)
      yval/=1000.;
   if (yval<2.)
      ndigits=2;
   else if (yval<3.)
      ndigits=1;
   else
      ndigits=0;
   if (ndigits>maxdigits) ndigits=maxdigits;
   len=snprintf(string,slen,"%.*f",ndigits,(float)yval);
   if (UseComma)
      for (i=0 ; i<len ; i++)
         if (string[i]=='.') string[i]=',';
   if (schar[scount] && len<slen) {
      string[len++]=schar[scount];
      string[len]='\0';
   }
}

static void greport_draw_yaxis(struct PlotStruct *pdata,struct GraphDataStruct *gdata)
{
   double yval;
   int y0;
   int y;
   int yt;
   char YLabel[50];
   int xexterior;
   int xinterior;
   int xtick;

   y0=gdata->BottomGraph;
   yt=gdata->BottomDepth-gdata->BottomGraph;
   xexterior=gdata->LeftGraph-10;
   xinterior=gdata->LeftGraph;
   xtick=gdata->LeftGraph-10-gdata->TickLength;
   for(y=y0-gdata->YTickSpace ; y>=gdata->TopGraph ; y-=gdata->YTickSpace) {
      gdImageLine(gdata->im, xtick, y+yt, xexterior, y+yt, gdata->dimgray);
      gdImageLine(gdata->im, xexterior, y+yt, xinterior, y, gdata->dimgray);
      gdImageLine(gdata->im, xinterior, y, gdata->RightGraph, y, gdata->dimgray);
      switch (pdata->ytype)
      {
         case PTG_LinBin:
            yval=(double)(y0-y)/gdata->YScale+(double)pdata->ymin;
            greport_formatbin(yval,2,YLabel,sizeof(YLabel));
            break;

         case PTG_LogBin:
            yval=exp((double)(y0-y)/gdata->YScale+log(pdata->ymin));
            greport_formatbin(yval,2,YLabel,sizeof(YLabel));
            break;

         case PTG_Time:
         {
            int t;

            yval=(double)(y0-y)/gdata->YScale+(double)pdata->ymin;
            t=(int)(yval/60000.+0.5);
            snprintf(YLabel,sizeof(YLabel),"%02d:%02d",t/60,t%60);
            break;
         }
      }
      Sarg_gdImageStringFT(gdata,gdata->dimgray,GraphFont,7,0.0,xtick,y+yt,YLabel,TRP_CenterRight);
   }
}

static void greport_plot(const struct userinfostruct *uinfo,struct PlotStruct *pdata)
{
   FILE *pngout;
   int x, y;
   int day;
   int x1;
   char graph[MAXLEN];
   char s[15];
   char ftime[128];
   time_t t;
   struct tm *local;
   gdPoint points[4];
   struct GraphDataStruct gdata;
   const int ImgXSize=720;
   const int ImgYSize=480;
   const int LeftMargin=60;
   const int RightMargin=20;
   const int TopMargin=60;
   const int BottomMargin=60;
   const int TickLength=3;
   const int ZTickLength=5;
   double yval;
   char blabel[50];
   double logpmin;

   memset(&gdata,0,sizeof(gdata));

   gdata.im = gdImageCreate(ImgXSize, ImgYSize);
   gdata.BottomGraph=ImgYSize-BottomMargin;
   gdata.LeftGraph=LeftMargin;
   gdata.RightGraph=ImgXSize-RightMargin;
   gdata.TopGraph=TopMargin;
   gdata.BottomDepth=gdata.BottomGraph+5;
   gdata.XScale=(double)(gdata.RightGraph-gdata.LeftGraph)/(pdata->npoints+1);
   if (greport_compute_yaxis(pdata,&gdata)<0) return;
   gdata.TickLength=TickLength;

   // first allocated color is the background
   gdata.lavender = gdImageColorAllocate(gdata.im, 230, 230, 250);
   gdata.gray = gdImageColorAllocate(gdata.im, 192, 192, 192);
   gdata.silver = gdImageColorAllocate(gdata.im, 211, 211, 211);
   gdata.black = gdImageColorAllocate(gdata.im, 0, 0, 0);
   gdata.dimgray = gdImageColorAllocate(gdata.im, 105, 105, 105);
   gdata.darkblue = gdImageColorAllocate(gdata.im, 0, 0, 139);
   gdata.goldenrod = gdImageColorAllocate(gdata.im, 234, 234, 174);
   gdata.goldenrod2 = gdImageColorAllocate(gdata.im, 207, 181, 59);

   if(strcmp(GraphDaysBytesBarColor,"orange") == 0) {
      gdata.color1 = gdImageColorAllocate(gdata.im, 255, 233, 142);
      gdata.color2 = gdImageColorAllocate(gdata.im, 220, 163, 72);
      gdata.color3 = gdImageColorAllocate(gdata.im, 255, 198, 107);
   }
   else if(strcmp(GraphDaysBytesBarColor,"blue") == 0) {
      gdata.color1 = gdImageColorAllocate(gdata.im, 62, 80, 167);
      gdata.color2 = gdImageColorAllocate(gdata.im, 40, 51, 101);
      gdata.color3 = gdImageColorAllocate(gdata.im, 57, 73, 150);
   }
   else if(strcmp(GraphDaysBytesBarColor,"green") == 0) {
      gdata.color1 = gdImageColorAllocate(gdata.im,120,166,129);
      gdata.color2 = gdImageColorAllocate(gdata.im,84,113,82);
      gdata.color3 = gdImageColorAllocate(gdata.im,158,223,167);
   }
   else if(strcmp(GraphDaysBytesBarColor,"yellow") == 0) {
      gdata.color1 = gdImageColorAllocate(gdata.im,185,185,10);
      gdata.color2 = gdImageColorAllocate(gdata.im,111,111,10);
      gdata.color3 = gdImageColorAllocate(gdata.im,166,166,10);
   }
   else if(strcmp(GraphDaysBytesBarColor,"brown") == 0) {
      gdata.color1 = gdImageColorAllocate(gdata.im,97,45,27);
      gdata.color2 = gdImageColorAllocate(gdata.im,60,30,20);
      gdata.color3 = gdImageColorAllocate(gdata.im,88,41,26);
   }
   else if(strcmp(GraphDaysBytesBarColor,"red")  == 0){
      gdata.color1 = gdImageColorAllocate(gdata.im,185,10,10);
      gdata.color2 = gdImageColorAllocate(gdata.im,111,10,10);
      gdata.color3 = gdImageColorAllocate(gdata.im,166,10,10);
   } else {
      debuga(_("Unknown color \"%s\" requested for the graph. Using orange instead\n"),GraphDaysBytesBarColor);
      gdata.color1 = gdImageColorAllocate(gdata.im, 255, 233, 142);
      gdata.color2 = gdImageColorAllocate(gdata.im, 220, 163, 72);
      gdata.color3 = gdImageColorAllocate(gdata.im, 255, 198, 107);
   }

   // rectangle around the image
   gdImageRectangle(gdata.im, 0, 0, ImgXSize-1, ImgYSize-1, gdata.dimgray);
   // backtround of the graph
   gdImageFilledRectangle(gdata.im, LeftMargin, gdata.TopGraph, gdata.RightGraph, gdata.BottomGraph, gdata.silver);

   // depth of the left Y axis
   points[0].x = gdata.LeftGraph-10;
   points[0].y = gdata.TopGraph+5;
   points[1].x = gdata.LeftGraph-10;
   points[1].y = gdata.BottomDepth;
   points[2].x = gdata.LeftGraph;
   points[2].y = gdata.BottomGraph;
   points[3].x = gdata.LeftGraph;
   points[3].y = gdata.TopGraph;
   gdImageFilledPolygon(gdata.im, points, 4, gdata.gray);

   // depth of the bottom X axis
   points[0].x = gdata.LeftGraph;
   points[0].y = gdata.BottomGraph;
   points[1].x = gdata.LeftGraph-10;
   points[1].y = gdata.BottomDepth;
   points[2].x = gdata.RightGraph-10;
   points[2].y = gdata.BottomDepth;
   points[3].x = gdata.RightGraph;
   points[3].y = gdata.BottomGraph;
   gdImageFilledPolygon(gdata.im, points, 4, gdata.gray);

   // vertical exterior line of the depth
   gdImageLine(gdata.im, LeftMargin-10, TopMargin+5, LeftMargin-10, gdata.BottomDepth+ZTickLength, gdata.black);
   // horizontal exterior line of the depth
   gdImageLine(gdata.im, LeftMargin-10-ZTickLength, gdata.BottomDepth, gdata.RightGraph-10, gdata.BottomDepth, gdata.black);
   // diagonal line between the two depths
   gdImageLine(gdata.im, LeftMargin-10, gdata.BottomDepth, LeftMargin, gdata.BottomGraph, gdata.black);
   // vertical left line of the graph
   gdImageLine(gdata.im, LeftMargin, gdata.BottomGraph, LeftMargin, gdata.TopGraph, gdata.black);
   // horizontal bottom line of the graph
   gdImageLine(gdata.im, LeftMargin, gdata.BottomGraph, gdata.RightGraph, gdata.BottomGraph, gdata.black);
   // vertical right line of the graph
   gdImageLine(gdata.im, gdata.RightGraph, gdata.TopGraph, gdata.RightGraph, gdata.BottomGraph, gdata.black);
   // diagonal line to close the right of the bottom depth
   gdImageLine(gdata.im, gdata.RightGraph-10, gdata.BottomDepth, gdata.RightGraph, gdata.BottomGraph, gdata.black);

   // Y axis ticks
   greport_draw_yaxis(pdata,&gdata);

   // X axis ticks and labels
   for(y=1; y<=pdata->npoints; y++) {
      x=gdata.LeftGraph-10+(int)((double)y*gdata.XScale+0.5);
      gdImageLine(gdata.im, x, gdata.BottomDepth, x, gdata.BottomDepth+TickLength, gdata.dimgray);
      sprintf(s,"%02d",y);
      Sarg_gdImageStringFT(&gdata,gdata.dimgray,GraphFont,7,0.0,x,gdata.BottomDepth+TickLength+1,s,TRP_TopCenter);
   }

   t = time(NULL);
   local = localtime(&t);
   if(DateFormat[0]=='u')
      strftime(ftime, sizeof(ftime), "%b/%d/%Y %H:%M", local);
   if(DateFormat[0]=='e')
      strftime(ftime, sizeof(ftime), "%d/%b/%Y-%H:%M", local);

   x=ImgXSize*5/12;
   Sarg_gdImageStringFT(&gdata,gdata.darkblue,GraphFont,7,0.0,ImgXSize-10,ImgYSize-10,ftime,TRP_BottomRight);
   if(ShowSargInfo) Sarg_gdImageStringFT(&gdata,gdata.darkblue,GraphFont,10,0.0,x,15,_("SARG, "),TRP_BottomRight);
   Sarg_gdImageStringFT(&gdata,gdata.darkblue,GraphFont,10,0.0,x,15,Title,TRP_BottomLeft);
   sprintf(warea,_("Period: %s"),period.text);
   Sarg_gdImageStringFT(&gdata,gdata.darkblue,GraphFont,9,0.0,x,27,warea,TRP_BottomLeft);
   sprintf(warea,_("User: %s"),uinfo->label);
   Sarg_gdImageStringFT(&gdata,gdata.darkblue,GraphFont,9,0.0,x,38,warea,TRP_BottomLeft);

   Sarg_gdImageStringFT(&gdata,gdata.black,GraphFont,10,3.141592/2,15,ImgYSize/2,pdata->YLabel,TRP_CenterLeft);
   Sarg_gdImageStringFT(&gdata,gdata.black,GraphFont,10,0.0,ImgXSize/2,ImgYSize-20,pdata->XLabel,TRP_BottomCenter);

   logpmin=(pdata->ytype==PTG_LogBin && pdata->ymin>0.) ? log(pdata->ymin) : 0.;
   for (day=0 ; day<pdata->npoints ; day++) {
      if (pdata->datapoints[day]>0) {
         x1=gdata.LeftGraph-10+(int)((double)(day+1)*gdata.XScale+0.5);
         switch (pdata->ytype)
         {
            case PTG_LinBin:
               yval=(double)pdata->datapoints[day];
               if (yval<pdata->ymin)
                  yval=0.;
               else if (yval>pdata->ymax)
                  yval=pdata->ymax;
               else
                  yval-=pdata->ymin;
               greport_formatbin(pdata->datapoints[day],1,blabel,sizeof(blabel));
               break;
            case PTG_LogBin:
               yval=(double)pdata->datapoints[day];
               if (yval<=pdata->ymin)
                  yval=0.;
               else if (yval>pdata->ymax)
                  yval=log(pdata->ymax)-logpmin;
               else
                  yval=log(yval)-logpmin;
               greport_formatbin(pdata->datapoints[day],1,blabel,sizeof(blabel));
               break;
            case PTG_Time:
            {
               int t;

               yval=(double)pdata->datapoints[day];
               if (yval<pdata->ymin)
                  yval=0.;
               else if (yval>pdata->ymax)
                  yval=pdata->ymax;
               else
                  yval-=pdata->ymin;
               t=(int)(pdata->datapoints[day]/60000.);
               snprintf(blabel,sizeof(blabel),"%d:%02d",t/60,t%60);
               break;
            }
            default:
               yval=-1.;
               break;
         }
         if (yval>=0.) bar(&gdata,x1,yval,blabel);
      }
   }

   if (snprintf(graph,sizeof(graph),"%s/%s/%s",outdirname,uinfo->filename,pdata->pngfile)>=sizeof(graph)) {
      debuga(_("user name too long for %s/%s/%s\n"),outdirname,uinfo->filename,pdata->pngfile);
      exit(EXIT_FAILURE);
   }
   if((pngout=fopen(graph,"wb"))==NULL) {
      debuga(_("(grepday) Cannot open log file %s\n"),graph);
      exit(EXIT_FAILURE);
   }
   gdImagePng(gdata.im, pngout);
   fclose(pngout);
   gdImageDestroy(gdata.im);

   if (gdata.string) free(gdata.string);
}

#endif //HAVE_GD

void greport_prepare(void)
{
#ifdef HAVE_GD
   if(access(GraphFont, R_OK) != 0) {
      debuga(_("(grepday) Fontname %s not found\n"),GraphFont);
      exit(EXIT_FAILURE);
   }

#ifdef USE_ICONV
   localtoutf = iconv_open ("UTF-8", CharSet);
   if (localtoutf==(iconv_t)-1) {
      debuga(_("(grepday) iconv cannot convert from %s to UTF-8 - %s\n"),CharSet,strerror(errno));
   }
#endif

#endif //HAVE_GD
}

void greport_day(const struct userinfostruct *uinfo)
{
#ifdef HAVE_GD
   FILE *fp_in, *fp_ou;
   char wdirname[MAXLEN];
   char buf[MAXLEN];
   char colsep;
   int day;
   long long int llday;
   long long int bytes;
   long long int elap;
   long long int bytespoints[31];
   long long int elappoints[31];
   struct getwordstruct gwarea;
   struct PlotStruct pdata;

   if (snprintf(wdirname,sizeof(wdirname),"%s/%s.day",tmp,uinfo->filename)>=sizeof(wdirname)) {
      debuga(_("user name too long for %s/%s.day\n"),tmp,uinfo->filename);
      exit(EXIT_FAILURE);
   }
   if(access(wdirname, R_OK) != 0) {
      return;
   }
   if(!Graphs || GraphFont[0]=='\0') {
      unlink(wdirname);
      return;
   }

   if((fp_in=fopen(wdirname,"r"))==NULL) {
      debuga(_("(grepday) Cannot open log file %s\n"),wdirname);
      exit(EXIT_FAILURE);
   }

   memset(bytespoints,0,sizeof(bytespoints));
   memset(elappoints,0,sizeof(elappoints));
   while(fgets(buf,sizeof(buf),fp_in)!=NULL) {
      fixendofline(buf);
      getword_start(&gwarea,buf);
      if (getword_atoll(&llday,&gwarea,'/')<0) {
         debuga(_("Invalid date in file %s\n"),wdirname);
         exit(EXIT_FAILURE);
      }
      day=(int)llday;
      if (day<1 || day>31) continue;
      if (getword_skip(20,&gwarea,'\t')<0 || getword_skip(20,&gwarea,'\t')<0) {
         debuga(_("Invalid entry in file %s\n"),wdirname);
         exit(EXIT_FAILURE);
      }
      if ((datetimeby & DATETIME_BYTE)!=0) {
         colsep=((datetimeby & DATETIME_ELAP)!=0) ? '\t' : '\0';
         if (getword_atoll(&bytes,&gwarea,'\t')<0) {
            debuga(_("Invalid number of bytes in file %s\n"),wdirname);
            exit(EXIT_FAILURE);
         }
         bytespoints[day-1]+=bytes;
      }
      if ((datetimeby & DATETIME_ELAP)!=0) {
         if (getword_atoll(&elap,&gwarea,'\0')<0) {
            debuga(_("Invalid elapsed time in file %s\n"),wdirname);
            exit(EXIT_FAILURE);
         }
         elappoints[day-1]+=elap;
      }
   }
   fclose(fp_in);
   unlink(wdirname);

   if (snprintf(wdirname,sizeof(wdirname),"%s/%s/graph.html",outdirname,uinfo->filename)>=sizeof(wdirname)) {
      debuga(_("user name too long for %s/%s/%s\n"),outdirname,uinfo->filename,"graph.html");
      exit(EXIT_FAILURE);
   }
   if ((fp_ou=fopen(wdirname,"wt"))==NULL) {
     debuga(_("(grepday) Cannot open output file %s\n"),wdirname);
     exit(EXIT_FAILURE);
   }
   write_html_head(fp_ou,_("Graph report"));

   fputs("<table class=\"report\" cellpadding=\"0\" cellspacing=\"2\">\n", fp_ou);
   if((datetimeby & DATETIME_BYTE)!=0) {
      memset(&pdata,0,sizeof(pdata));
      pdata.datapoints=bytespoints;
      pdata.npoints=31;
      pdata.XLabel=_("DAYS");
      pdata.ymin=50LL*1000LL;
      pdata.ymax=5LL*1000LL*1000LL*1000LL;
      pdata.ytype=PTG_LogBin;
      pdata.YLabel=_("BYTES");
      pdata.pngfile="graph_day_byte.png";
      greport_plot(uinfo,&pdata);
      fprintf(fp_ou,"<tr><td><img src=\"%s\" alt=\"B\"></td></tr>\n",pdata.pngfile);
   }
   if((datetimeby & DATETIME_ELAP)!=0) {
      memset(&pdata,0,sizeof(pdata));
      pdata.datapoints=elappoints;
      pdata.npoints=31;
      pdata.XLabel=_("DAYS");
      pdata.ymin=0;
      pdata.ymax=86400000;
      pdata.ytype=PTG_Time;
      pdata.YLabel=_("ELAPSED TIME");
      pdata.pngfile="graph_day_elap.png";
      greport_plot(uinfo,&pdata);
      fprintf(fp_ou,"<tr><td><img src=\"%s\" alt=\"E\"></td></tr>\n",pdata.pngfile);
   }
   fputs("</table>\n",fp_ou);

   if (write_html_trailer(fp_ou)<0)
      debuga(_("Write error in file %s\n"),wdirname);
   if (fclose(fp_ou)==EOF)
      debuga(_("Failed to close file %s - %s\n"),wdirname,strerror(errno));
#endif //HAVE_GD

   return;
}

void greport_cleanup(void)
{
#ifdef HAVE_GD
   gdFontCacheShutdown();

#ifdef USE_ICONV
   if (localtoutf!=(iconv_t)-1)
      iconv_close (localtoutf);
#endif
#endif //HAVE_GD
}
