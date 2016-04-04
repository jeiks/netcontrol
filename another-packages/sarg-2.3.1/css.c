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

void css_content(FILE *fp_css)
{
   fprintf(fp_css,".logo {font-family:Verdana,Tahoma,Arial;font-size:11px;color:%s;text-align:center;vertical-align:middle;border:none;padding:0px;margin-bottom:5px;}\n",LogoTextColor);
   fprintf(fp_css,".logo th {padding:0px;}\n");
   fprintf(fp_css,".logo img {vertical-align:middle;padding:0px;border:0px none;}\n");
   fprintf(fp_css,".body {font-family:%s;font-size:%s;color:%s;background-color:%s;background-image:url(%s);}\n",FontFace,TitleFontSize,TxColor,BgColor,BgImage);
   fprintf(fp_css,".info {font-family:%s;font-size:10px;text-align:center;margin-top:1em;margin-bottom:1em;}\n", FontFace);
   fprintf(fp_css,".info a:link,a:visited {font-family:%s;color:#0000FF;font-size:10px;text-decoration:none;}\n", FontFace);

   fprintf(fp_css,".title {width:100%%;text-align:center;margin-bottom:1em;}\n");
   fprintf(fp_css,"div.title > table {margin:auto;}\n");
   fprintf(fp_css,".title_c {font-family:%s;font-size:%s;color:%s;background-color:%s;text-align:center;}\n",FontFace,TitleFontSize,TiColor,BgColor);
   fprintf(fp_css,".title_l {font-family:%s;font-size:%s;color:%s;background-color:%s;text-align:left;}\n",FontFace,TitleFontSize,TiColor,BgColor);
   fprintf(fp_css,".title_r {font-family:%s;font-size:%s;color:%s;background-color:%s;text-align:right;}\n",FontFace,TitleFontSize,TiColor,BgColor);

   fprintf(fp_css,".index {width:100%%;text-align:center;}\n");
   fprintf(fp_css,"div.index > table {margin:auto;}\n");
   fprintf(fp_css,".report {width:100%%;text-align:center;}\n");
   fprintf(fp_css,"div.report > table {margin:auto;}\n");

   fprintf(fp_css,".header_l {font-family:%s;font-size:%s;color:%s;background-color:%s;text-align:left;border-right:1px solid #666666;border-bottom:1px solid #666666;}\n",FontFace,HeaderFontSize,HeaderColor,HeaderBgColor);
   fprintf(fp_css,".header_r {font-family:%s;font-size:%s;color:%s;background-color:%s;text-align:right;border-right:1px solid #666666;border-bottom:1px solid #666666;}\n",FontFace,HeaderFontSize,HeaderColor,HeaderBgColor);
   fprintf(fp_css,".header_c {font-family:%s;font-size:%s;color:%s;background-color:%s;text-align:center;border-right:1px solid #666666;border-bottom:1px solid #666666;}\n",FontFace,HeaderFontSize,HeaderColor,HeaderBgColor);

   fprintf(fp_css,".data {font-family:%s;color:%s;font-size:%s;background-color:%s;text-align:right;border-right:1px solid #6A5ACD;border-bottom:1px solid #6A5ACD;}\n", FontFace, TxColor, FontSize, TxBgColor);
   fprintf(fp_css,".data a:link,a:visited {font-family:%s;color:#0000FF;font-size:%s;background-color:%s;text-align:right;text-decoration:none;}\n", FontFace, FontSize, TxBgColor);

   fprintf(fp_css,".data2 {font-family:%s;color:%s;font-size:%s;background-color:%s;text-align:left;border-right:1px solid #6A5ACD;border-bottom:1px solid #6A5ACD;}\n", FontFace, TxColor, FontSize, TxBgColor);
   fprintf(fp_css,".data2 a:link,a:visited {font-family:%s;color:#0000FF;font-size:%s;text-align:left;background-color:%s;text-decoration:none;}\n", FontFace, FontSize, TxBgColor);

   fprintf(fp_css,".data3 {font-family:%s;color:%s;font-size:%s;background-color:%s;text-align:center;border-right:1px solid #6A5ACD;border-bottom:1px solid #6A5ACD;}\n", FontFace, TxColor, FontSize, TxBgColor);
   fprintf(fp_css,".data3 a:link,a:visited {font-family:%s;color:#0000FF;font-size:%s;text-align:center;background-color:%s;text-decoration:none;}\n", FontFace, FontSize, TxBgColor);

   fprintf(fp_css,".text {font-family:%s;color:%s;font-size:%s;background-color:%s;text-align:right;}\n", FontFace, TxColor, FontSize, TxBgColor);

   fprintf(fp_css,".link {font-family:%s;font-size:%s;color:#0000FF;}\n", FontFace, FontSize);
   fprintf(fp_css,".link a:link,a:visited {font-family:%s;font-size:%s;color:#0000FF;text-decoration:none;}\n", FontFace, FontSize);

   fprintf(fp_css,"a > img {border:none;}\n");
}

void css(FILE *fp_css)
{
   if(ExternalCSSFile[0] != '\0') {
      fprintf(fp_css,"<link rel=\"stylesheet\" href=\"%s\" type=\"text/css\">\n",ExternalCSSFile);
      return;
   }

   fprintf(fp_css,"<style type=\"text/css\">\n");
   css_content(fp_css);
   fputs("</style>\n",fp_css);
}
