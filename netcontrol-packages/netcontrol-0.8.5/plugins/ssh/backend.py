#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Written by João Paulo Contantino <joaopaulo@fluxsoftwares.com> for FluxSoftwares)
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
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *

class Arquivos():
    sshd1 = ''
    backup = ''

class Config(Plugin):
    """
    @author: <joaopauloctga@gmail.com>
    """
    implements(IConfigurable)
    name = 'ssh'
    icon = '/dl/ssh/icon.png'
    id = 'ssh'
 
    def list_files(self):
        return ['/etc/ssh/sshd_config']# puxando arquivo que contem os hosts do pc
 
    def read(self):
        arquivo = Arquivos()
        arquivo.sshd1 = ConfManager.get().load('ssh', '/etc/ssh/sshd_config')
        arquivo.backup = arquivo.sshd1
        return arquivo
 
    def save(self, arquivo):
        d = ''
        for i in arquivo:
            d += '%s\n' % (i)
        ConfManager.get().save('ssh', '/etc/ssh/sshd_config', d)
        ConfManager.get().commit('ssh')
