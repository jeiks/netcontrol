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
from backend import Config, SyncLdapRedmine as ldap
from os import access, F_OK

class Helpdesk(CategoryPlugin):
	"""
	@author: <joaopauloctga@gmail.com>
	"""
	text = 'Help Desk'
	icon = '/dl/helpdesk/icon.png'
	folder = 'servers' 

	def on_init(self):
		pass
 
	def on_session_start(self):
		self._view = None
		
	def get_ui(self):
		
		if not ldap().conn:
			ui = self.app.inflate("helpdesk:error")
			ui.find("error").set('text',"É necessário configurar a base LDAP")
			return ui
		ui = self.app.inflate('helpdesk:main')		
		ldap().syncDB()
		
		if access("/var/www/helpdesk", F_OK):
			ui.remove("ativar")
		else:
			ui.remove("desativar")
		
		ui.find("linkHelp").set('url', "http://%s/helpdesk/"%self.app.environ['SERVER_NAME'])
		if Config().getConfig()["fonte_config"] == "redmine" and open('/var/lib/netcontrol/plugins/helpdesk/config').read().split('=')[1] == 'True':
			ui.find("titulo").set("text","Servidor configurado")
		elif Config().getConfig()["fonte_config"] == "arquivo_ldap" or open('/var/lib/netcontrol/plugins/helpdesk/config').read().split('=')[1] == 'False':
			ui.find("titulo").set("text","Servidor ainda não foi configurado ou está desativado, informações abaixo foram coletadas do arquivo de configuração.")
			ui.remove("linkHelp")
			
		for i in Config.indice:
			ui.find( i ).set('value', Config().getConfig()[i] )
		
		if self._view == "listarUsers":
			users = self.app.inflate('helpdesk:view')
			msg = users.find('users')
			l = []
			for i in ldap().getUsersLDAP():
				j = ldap().getInfoUser(i)
				msg.append(
					UI.DTR(
						UI.DTH( UI.Label( text= j["login"] if j["login"] <> "" else "uid", size=2    )     ),
						UI.DTH( UI.Label( text= j["nome"] if j["nome"] <> "" else "giverName", size=2    )  ),
						UI.DTH( UI.Label( text= j["sobrenome"] if j["sobrenome"] <> "" else "sN", size=2    )  ),
						UI.DTH( UI.Label( text= j["mail"] if j["mail"] <> "NULL" else "sem registro", size=2   )  )
					)
				)
			self._view = None
			return users
			
				
		
		return ui
 

	@event('button/click')                                                    
	def on_click(self, event, params, vars = None):
		from os import system
		if params[0] == "listarUsers":
			self._view = params[0]
		elif params[0] == "desativar":
			Config().delConfig()
			open('/var/lib/netcontrol/plugins/helpdesk/config','w').write('servidor=False')
			system("rm /var/www/helpdesk")
		elif params[0] == "ativar":
			open('/var/lib/netcontrol/plugins/helpdesk/config','w').write('servidor=True')
			system("ln -s /usr/share/redmine/public/ /var/www/helpdesk")
			system("chown -R www-data:www-data /var/www/helpdesk/")
 
	@event('dialog/submit')
	def on_dialog(self, event, params, vars = None):
		pass
        
	@event('form/submit')
	def on_submit(self, event, params, vars = None):
		if params[0] == "configurarServer":
			if vars.getvalue("action",'') == "configServer":
				Config().setConfig( 
					vars.getvalue('nomeServidor') , vars.getvalue('enderecoServidor') , \
					vars.getvalue('porta') , vars.getvalue('contaLDAP') , vars.getvalue('senhaContLDAP') ,\
					vars.getvalue('baseDN') , vars.getvalue('usuario') , vars.getvalue('nomeUser') , \
					vars.getvalue('sobrenome') , vars.getvalue('email'), vars.getvalue('protSSL')
				)
