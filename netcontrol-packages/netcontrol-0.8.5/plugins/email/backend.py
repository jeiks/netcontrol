#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> e João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *
 
 
class Dominios:
    def __init__(self):
        self.dominios = '';
 
 
class Config(Plugin):
    implements(IConfigurable)
    name = 'Email'
    icon = '/dl/email/icon.png'
    id = 'email'
 
    def list_files(self):
        return ['/etc/email_domains']
 
    def read(self):
        ss = ConfManager.get().load('email', '/etc/email_domains').split('\n')
        return ss[0]
 
    def save(self, dominios):
        d = ''
        for h in dominios:
            d += '%s\n' % (h.dominios)
        ConfManager.get().save('email', '/etc/email_domains', d)
        ConfManager.get().commit('email')
