#!/usr/bin/env python
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Written by Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> for FluxSoftwares)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of 
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

from sys import argv, exit

if len(argv) <> 2:
    print 'Informe a maquina'
    exit(1)

from netcontrolldap import netcontrolldap
from pprint import pprint
from ConfigParser import ConfigParser

cfg = ConfigParser()
cfg.read('/etc/ldap/netcontrol')
server  = cfg.get('base', 'server')
bindDN  = cfg.get('base', 'bindDN')                       
adminPW = cfg.get('base', 'adminPW')
del cfg

o = netcontrolldap.LDAPConnection( 'localhost', ssl=False, admPasswd=adminPW, baseDN=bindDN )

print o.addMachine( argv[1] )

o.closeConnection()

exit(0)
