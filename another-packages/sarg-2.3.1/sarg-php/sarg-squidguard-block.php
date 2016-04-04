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

// Change to squidGuard and sarg config file paths
$SargConf = "/usr/local/sarg/sarg.conf";
$squidGuardConf = "/usr/local/squidGuard/squidGuard.conf";

global $SargConf;
global $dbhome;
global $language;

include_once("language.php");
include_once("style.php");

$url = $_GET['url'];
$squidGuard_conf = '/usr/local/squidGuard/squidGuard.conf';

function parse_config($line,$clave) {
   if (preg_match("/dbhome/i", $line)) {
      global $dbhome;
      $l = explode(' ', $line);
      list(, $dbhome) = $l;
      $dbhome=preg_replace('/\s+/','',$dbhome);
   }
}

echo "<center>\n";
echo "<table>\n";
echo "<tr><td class=\"title\">";
print(gettext ("Sarg-SquidGuard - URL Blocking"));
echo "</td></tr>\n";
echo "<tr><td class=\"header2\">";
print(gettext ("Choose the rule set where"));
echo "<a href=\"http://$url\">&nbsp;$url</a> ";
print(gettext ("will by added"));
echo "</td></tr>\n";
echo "</table>\n";
echo "<table>\n";

$lines=file($squidGuardConf);
array_walk($lines,'parse_config');

if (is_dir($dbhome)) {
   if ($ha1 = opendir($dbhome)) {
      while (false !== ($file = readdir($ha1))) {
         if ($file == '.' || $file == '..') continue;
         echo "<tr><td class=\"header\">$file</td></tr>\n";
         $dir2 = $dbhome.'/'.$file;
         if (is_dir($dir2)) {
            if ($ha2 = opendir($dir2)) {
               while (false !== ($file2 = readdir($ha2))) {
                  if ($file2 == '.' || $file2 == '..') continue;
                     echo "<tr><td class=\"data2\"><a href=\"sarg-squidguard-block2.php?file=$dir2/$file2&url=$url&lang=$language\">$file2</a></td></tr>\n";
               }
            }
            closedir($ha2);
         }
      }
      closedir($ha1);
   }
}

echo "</table>\n";
echo "</html>\n";
?>
