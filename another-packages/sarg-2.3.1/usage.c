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

void usage(const char *prog)
{
  printf(_("Usage: %s [options...]\n"), prog);
  puts  (_("     -a Hostname or IP address"));
  puts  (_("     -b Useragent log"));
  puts  (_("     -c Exclude file"));
  puts  (_("     -d Date from-until dd/mm/yyyy-dd/mm/yyyy"));
  puts  (_("     -e Email address to send reports (stdout for console)"));
  printf(_("     -f Config file (%s/sarg.conf)\n"),SYSCONFDIR);
  puts  (_("     -g Date format [e=Europe -> dd/mm/yyyy, u=USA -> mm/dd/yyyy]"));
  puts  (_("     -h This help"));
  puts  (_("     -i Reports by user and IP address"));
  puts  (_("     -l Input log"));
  puts  (_("     -n Resolve IP Address"));
  puts  (_("     -o Output dir"));
  puts  (_("     -p Use Ip Address instead of userid (reports)"));
  puts  (_("     -s Accessed site [Eg. www.microsoft.com, www.netscape.com]"));
  puts  (_("     -t Time [HH, HH:MM]"));
  puts  (_("     -u User"));
  puts  (_("     -w Temporary dir"));
  puts  (_("     -x Debug messages"));
  puts  (_("     -z Process messages"));
  puts  (_("     -convert Convert the access.log file to a legible date"));
  puts  (_("     -split Split the log file by date in -d parameter"));
  printf("\n\t%s-%s\n",PGM,VERSION);
  puts  ("\thttp://sarg.sourceforge.net");
  puts  (_("\n\tPlease donate to the sarg project:"));
  puts  ("\t\thttp://sarg.sourceforge.net/donations.php\n");

  return;
}
