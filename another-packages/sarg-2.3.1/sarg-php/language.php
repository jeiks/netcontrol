<?php

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

global $language;
global $SargConf;

function sarg_config($line,$clave) {
   if ( 0 == strncmp("#", $line, 1))
      return;
   if (preg_match("/language/i", $line)) {
      global $language;
      $l = explode(' ', $line);
      list(, $lang) = $l;
      $lang=preg_replace('/\s+/','',$lang);
      if ( 0 == strcmp("English", $lang))
         $language = "en-EN";
      else if ( 0 == strcmp("Portuguese", $lang))
         $language = "pt_BR";
      else if ( 0 == strcmp("French", $lang))
         $language = "fr";
      else if ( 0 == strcmp("Russian", $lang))
         $language = "ru";
      return;
   }
}


$lines=file($SargConf);
array_walk($lines,'sarg_config');

putenv("LANG=$language");
setlocale(LC_ALL, $language);
$domain = 'messages';
bindtextdomain($domain, "./locale");
textdomain($domain);
return;

?>
