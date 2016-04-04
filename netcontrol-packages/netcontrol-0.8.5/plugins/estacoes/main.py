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
from netcontrol.ui import *
import backend
import os 

class Estacoes(CategoryPlugin):
    """
    @author: João Paulo Constantino <joaopauloctga@gmail.com>
    """
    text = "NetControl's"
    icon = '/dl/network/icon.png'
    folder = 'system' 

    def on_init(self):
        be = backend.Config(self.app)
        self.hosts = []
 
    def on_session_start(self):
        # _vars are session-persisted
        self._editing = None # this will indicate if we are in editing dialog

    def __isNetControl(self, host, port=8000):
        """
        @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
        """
        from socket import socket, AF_INET, SOCK_STREAM, SHUT_RDWR
        s = socket(AF_INET, SOCK_STREAM)
        s.settimeout(3)
        try:
            s.connect((host, port))
            s.shutdown(SHUT_RDWR)
            from urllib import urlopen
            page = urlopen('http://%s:8000' % host).read().upper()
            if page.find('NETCONTROL') <> -1 <> -1:
                return True
            else:
                return False
        except:
            return False
 
    def get_ui(self):
        from urllib import urlopen
        ui = self.app.inflate('estacoes:main')
        t = ui.find('list')
                
        for x in os.popen('/usr/bin/arp-scan -q -l').read().split('\n\n')[0].split('Starting')[1].split('\n')[1:]:
            tmpH = x.split()[0]
            if self.__isNetControl(tmpH):
                    self.hosts.append(x.split())

        for h in self.hosts:
            t.append(UI.DTR(
                UI.OutlinkLabel(url=str("http://"+h[0]+":8000"),text="Acessar "+h[0])
            ))
 
        if self._editing is not None:
            try:
                h = self.hosts[self._editing]
            except:
                h = backend.Host()
            d = self.app.inflate('estacoes:edit') # inflate and fill the dialog
            d.find('name').set('value', h)
            ui.append('main', d) # and append it to main UI
 
        return ui
 
    @event('button/click')
    def on_click(self, event, params, vars = None):
        if params[0] == 'add':
            self._editing = len(self.hosts) # new host
        if params[0] == 'edit':
            self._editing = int(params[1]) # selected host
        if params[0] == 'del':
            self.hosts.pop(int(params[1]))
            backend.Config(self.app).save(self.hosts)
 
 
    @event('dialog/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'dlgEdit':
            if vars.getvalue('action', '') == 'OK':
                h = backend.Host()
                h.name = vars.getvalue('name', 'none')
                try:
                    self.hosts[self._editing] = h
                except:
                    self.hosts.append(h)
                backend.Config(self.app).save(self.hosts)
            self._editing = None
