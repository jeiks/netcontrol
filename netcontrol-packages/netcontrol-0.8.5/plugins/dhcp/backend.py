#-*- coding: utf-8 -*-
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
from netcontrol import apis


class Config(Plugin):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    implements(IConfigurable)
    name = 'DHCP'
    icon = '/dl/dhcp/icon.png'
    id = 'DHCP'

    def list_files(self):
        return []

    def addSubnet(self, ip, mascara, roteador, dns):
        from netcontroldhcp.netcontroldhcp import DHCP
        return DHCP().addSubnet(ip, mascara, roteador, dns)

    def addHost(self, name, mac, ip, masc, ltsp):
        from netcontroldhcp.netcontroldhcp import DHCP
        DHCP().addHost(name, mac, ip, masc, ltsp)

    def delSubnet(self, ip, masc):
        from netcontroldhcp.netcontroldhcp import DHCP
        DHCP().delSubnet(ip, masc)

    def delHost(self, mac):
        from netcontroldhcp.netcontroldhcp import DHCP
        DHCP().delHost(mac)

    def getSubnets(self):
        from netcontroldhcp.netcontroldhcp import DHCP
        ret = []
        for i in DHCP().getSubnets():
            ret.append(i)
        return ret

    def getHostsFrom(self, subnet):
        from netcontrolrede.netcontrolrede import Rede
        from netcontroldhcp.netcontroldhcp import DHCP
        ret = []
        subnet = subnet.split('/')
        for i in DHCP().getHosts():
            tsubnet = Rede(i['ip'], i['masc']).getRedeAsStr()
            tmasc   = Rede(i['ip'], i['masc']).getMascAsInt()
            if tsubnet == subnet[0] and str(tmasc) == str(subnet[1]):
                ret.append(i)
        return ret

    def restartDHCP(self):
        from netcontroldhcp.netcontroldhcp import DHCP
        return DHCP().restartDHCP()

