#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from string import Template

class Zone:
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    ztempl = \
    Template('zone "$zoneName" {\n\ttype $zoneType;'+
                       '\n\tfile "$zoneFile";\n};\n')
    zoneName = None
    zoneType = None
    zoneFile = None

    def __init_(self, zoneName = None,
                zoneType = None, zoneFile = None):
        self.setName(zoneName)
        self.setType(zoneType)
        self.setFile(zoneFile)
    
    def setName(self, ZoneName):
        self.ZoneName = ZoneName

    def setType(self, ZoneType):
        self.ZoneType = ZoneType

    def setFile(self, ZoneFile):
        self.ZoneFile = ZoneFile
    
    def getName(self):
        try: return( self.ZoneName )
        except: return None

    def getType(self):
        try: return (self.ZoneType)
        except: return None

    def getFile(self):
        try: return (self.ZoneFile)
        except: return None
    
    def getZone(self):
        return { 'zoneName':self.getName(),
                 'zoneType':self.getType(),
                 'zoneFile':self.getFile() }
    
    def getZoneAsNamedConf(self):
        return self.ztempl.substitute(**self.getZone())
    
    def printZone(self):
        print "Zone:", self.getName()
        print "Type:", self.getType()
        print "File:", self.getFile()

    def printZoneAsNamedConf(self, zone=None):
        if zone == None:
            print self.getZoneAsNamedConf()
        else:
            ( Zone( **zone ) ).printZoneAsNamedConf()
    
