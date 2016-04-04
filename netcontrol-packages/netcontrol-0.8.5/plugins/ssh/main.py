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
from netcontrol.ui import *
import subprocess
import backend
from restart import *
 
class Ssh(CategoryPlugin):
    """
    @author: <joaopauloctga@gmail.com>
    """
    text = 'SSH'
    icon = '/dl/ssh/icon.png'
    folder = 'servers' 

    def on_init(self):
        be = backend.Config(self.app)
        self.ssh = be.read().sshd1
        self._backup = be.read().backup
 
    def on_session_start(self):
        # _vars are session-persisted
        self._editing = None # this will indicate if we are in editing dialog
        self._texto = None
 
    def get_ui(self):
        ui = self.app.inflate('ssh:main')
        t = ui.find('texto')
 
        
 
        if self._editing == None:
            t = ui.find('texto').set('value',self.ssh)
        if self._editing == 'salvo':
            t = ui.find('texto').set('value',self._texto)
        if self._editing == 'restart':
            ui.remove('sshd')
            t = ui = self.app.inflate('ssh:edit')
            t.find('resposta').append(  UI.DTR(  UI.Label(text=self._resposta)  )  )
 
        return ui
 
    @event('dialog/submit')
    def on_dialog(self, event, params, vars = None):
        if params[0] == 'resposta_sshd':
            if vars.getvalue('action','') == 'OK':
                self._editing = None             
        
    @event('form/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'sshd':
            if vars.getvalue('action','') == 'salvar_config':
                self._texto = vars.getvalue('texto','none')
                open('/etc/ssh/sshd_config','w').write(self._texto)
                self._editing = 'salvo'
            if vars.getvalue('action','') == 'restart_ssh':
                restart('/etc/init.d/ssh restart',"True")
                #process = subprocess.Popen("/etc/init.d/ssh restart", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                #self._resposta = process.communicate()
                self._resposta = "SSH reiniciado com sucesso"
                self._editing = 'restart'
            if vars.getvalue('action','') == 'OK':
                self._editing = None             
