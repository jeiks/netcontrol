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
from netcontrol.ui import *
import backend
from configServerEmail import Configurar
from bind.configfile import ConfigFile

class Email(CategoryPlugin):
    text = 'WebMail' 
    icon = '/dl/email/icon.png'
    folder = 'servers' 

    def on_init(self):
        be = backend.Config(self.app)
        self.dominio = None
        self.info_restart = None
        
        """self.serv_roundcube = None
        self.home = None
        self.serv_ldap = None
        self.dc = None
        self.cn = None
        self.senha = None
        self.server_bind = None"""
 
    def on_session_start(self):
        self._editing = None
        
 
    def get_ui(self):                        
        lista_dominios = ConfigFile('/etc/bind/named.conf.local')
        open('/tmp/1','w').write(str(dir(lista_dominios)))
        if lista_dominios.getZones() == []:
             ui = self.app.inflate("email:msg")
        else:
            ui = self.app.inflate('email:main')
            msg = ui.find('msg')
            email = open('/var/lib/netcontrol/plugins/email/email').read()
            if email == "":
                msg.append(
                    UI.Label(size=3,text="Nenhum dominio de email foi definido" )
                )
                info = ui.find('info')
                info.append(UI.Label(size=3,text="Defina um dominio de Email"))
            else:
                msg.append(
                    UI.Label(size=3,text="Servidor de email configurado para %s"%email )
                )
                info = ui.find('info')
                info.append(UI.Label(size=3,text="Mudar o dominio de Email"))
            lista = ui.find('dominios')
            for i in lista_dominios.getZones():
                if i.getName().find('in-addr') == -1:
                    lista.append( UI.SelectOption(value=i.getName(),text=i.getName()) )
        if self.info_restart == True:
            t = ui = self.app.inflate("email:restart")
            t.find("restart").append(  UI.DTR(  UI.Label(text=self._resposta)  )  )
            self.info_restart = False
        return ui
        
    @event('dialog/submit')
    def on_dialog(self, event, params, vars = None):
        if params[0] == 'msg_user':
            if vars.getvalue('action','') == 'OK':
                open('/home/joao/ui.e','w').write('deu certo')
        
    @event('form/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'form_dominio':
            if vars.getvalue('action','') == 'confirmar_dominio':
                #indice = int(vars.getvalue('dominios','none'))
                self.dominio = vars.getvalue('dominios','none')
                open('/var/lib/netcontrol/plugins/email/email','w').write(self.dominio)
                Configurar().config_roundcube('localhost',self.dominio)
                Configurar().config_postfix(self.dominio,'/home')
                Configurar().config_authldaprc()
                Configurar().config_bind(self.dominio)
                Configurar().conf_roundcube_apache()
                Configurar().config_authdaemon()
                self.info_restart = Configurar().restart()
                
