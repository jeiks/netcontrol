<?php

/*
 * AUTHOR: Pedro Lineu Orso                         pedro.orso@gmail.com
 *                                                            1998, 2006
 * SARG Squid Analysis Report Generator            http://sarg-squid.org
 *
 * SARG donations:
 *      please look at http://sarg.sourceforge.net/donations.php
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

$file = $_GET['file'];
$url = $_GET['url'];
$language = $_GET['lang'];
$url = $url."\n";

putenv("LANG=$language");
setlocale(LC_ALL, $language);
$domain = 'messages';
bindtextdomain($domain, "./locale");
textdomain($domain);

$ha = fopen($file, 'a');
if ($file == false) {
   print (gettext ("Could not open the file"));
   echo '&nbsp;'.$file;
   exit;
}
else {
   $written = fwrite($ha, $url);
   fclose($ha);
   if ($written != strlen($url)) {
      print (gettext ("Write error"));
      exit;
   }
   print (gettext ("Done!"));
   echo "<br><br>\n";
   echo "<a href=\"javascript:history.go(-2)\">";
   print (gettext("Return"));
   echo "</a>&nbsp;";
   print (gettext (" to Sarg."));
}
