# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *

from bind.configfile import ConfigFile
from bind.zonefile import ZoneFile

BIND_CONF_FILE = '/etc/bind/named.conf.local'

class Config(Plugin):
    implements(IConfigurable)
    name = 'Bind'
    icon = '/dl/bind/icon.png'
    id   = 'bind'

    configFile = None

    def list_files(self):
        return [ BIND_CONF_FILE ]

    def read(self):
        self.configFile = ConfigFile( BIND_CONF_FILE  )
        return self.configFile.getZones()

    def save(self, zone):
        ConfigFile.SaveFileWithZone( BIND_CONF_FILE, zone, newZone=True )

    def saveZoneFile(self, zone, zoneFileObject):
        open(zone.getFile(), 'w').write( ZoneFile.getZoneFileByZone (zoneFileObject) )
