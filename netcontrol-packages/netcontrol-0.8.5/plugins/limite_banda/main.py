#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
from netcontrol.api import *
from netcontrol.ui import *
import backend
from GerenciaEtc import GerenteConfigs

class Limite_Banda(CategoryPlugin):

	text = 'Limite. Banda' 
	icon = '/dl/controle_banda/icon.png'
	folder = 'system'
    
    
	def on_init(self):
		be = backend.Config(self.app)
      
	def on_session_start(self):
		self._view  = None
		self._error = None
		self._nomeAnt = None
        
 
	def get_ui(self):
		gerenteCNF = GerenteConfigs()
		infoServer = gerenteCNF.getServer()
		
		ui = self.app.inflate('limite_banda:main')
		
		
		if self._view <> None:
			if self._view == "addRegra":
				edit = self.app.inflate('limite_banda:addConfig')
				if self._error <> None:
					edit.find('erro').set('text',self._error)
				self._error = None
			if self._view[0] == "editRede" or self._view[0] == "editPc":
				edit = self.app.inflate('limite_banda:edit')
				values = gerenteCNF.searchConfig( self._view[1] )
				self._nomeAnt = values['nome']
				edit.find('nome')           .set('value',values['nome'])
				edit.find('ip')             .set('value',values['ip'])
				edit.find('banda_liberada') .set('value',values['banda_liberada'])
			return edit			
		
		banda = ui.find("total_banda")
		banda.set('value',"%s Mbits"%infoServer["total_banda"])
		serv = ui.find("interface")
		for i in gerenteCNF.getInterfaces():
			serv.append(
				UI.SelectOption( value=i, text=i , selected = ( banda["iface"] == i and True or False ) )
			)
		
		pcs = ui.find("lista_de_pcs")
		redes = ui.find("lista_de_redes")
		
		

		if len( gerenteCNF.getConfigs(m=True)["maquina"] ) >= 0:
			for i in gerenteCNF.getConfigs(m=True)["maquina"]:
				pcs.append(
					UI.DTR(
						UI.DTD(
							UI.Center(UI.Label(text=i["nome"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["ip"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["prio"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text="%s Kbits"%i["banda_liberada"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text="%s Kbits"%i["banda_down"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["delimitado"] == "yes" and "sim" or "não"))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["isolado"] == "yes" and "sim" or "não"))
						),
						UI.DTD(
							UI.Center(
								#UI.TipIcon(text="Editar as configurações da máquina %s"%i["ip"], icon="/dl/core/ui/stock/edit.png",   id="editPc/%s"%i["nome"]),
								UI.TipIcon(text="Excluir a máquina %s"%i["ip"],                  icon="/dl/core/ui/stock/delete.png", id="delPc/%s/%s"%(i["nome"],i["ip"])),
							)
						)
					)
				)
		else:
			pcs.append( UI.Center( UI.Label( text="Nehuma máquina configurada..." ) ))
        
        
		if len( gerenteCNF.getConfigs(r=True)["rede"] ) >= 0:
			for i in gerenteCNF.getConfigs(r=True)["rede"]:
				redes.append(
					UI.DTR(
						UI.DTD(
							UI.Center(UI.Label(text=i["nome"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["ip"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["prio"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text="%s Kbits"%i["banda_liberada"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text="%s Kbits"%i["banda_down"]))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["delimitado"] == "yes" and "sim" or "não"))
						),
						UI.DTD(
							UI.Center(UI.Label(text=i["isolado"] == "yes" and "sim" or "não"))
						),
						UI.DTD(
							UI.Center(
								#UI.TipIcon(text="Editar as configurações da rede %s"%i["ip"], icon="/dl/core/ui/stock/edit.png",   id="editRede/%s"%i["nome"]),
								UI.TipIcon(text="Excluir a rede %s"%i["ip"],                  icon="/dl/core/ui/stock/delete.png", id="delRede/%s/%s"%(i["nome"],i["ip"])),
							)
						)
					)
				)
		else:
			redes.append( UI.Center( UI.Label( text="Nehuma rede configurada..." ) ))
			
		del gerenteCNF
		return ui

	@event('button/click')
	def on_click(self, event, params, vars = None):
		gerenteCNF = GerenteConfigs()
		if params[0] == "addRegra":
			self._view = params[0]
		elif params[0] == 'delRede':
			gerenteCNF.delConfig(params[1], params[2]+'/'+params[3], 'r')
		elif params[0] == 'delPc':
			gerenteCNF.delConfig(params[1], params[2], 'm')
		elif params[0] == "excluirRedes":
			gerenteCNF.delAllConfigs(r=True)
		elif params[0] == "excluiMaquinas":
			gerenteCNF.delAllConfigs(m=True)
		elif params[0] == "aplicarConfigs":
			gerenteCNF.compilaRegras()
		elif params[0] == "delAllConfigs":
			gerenteCNF.delAllConfigs( r=True, m=True )
		del gerenteCNF
			
	@event('dialog/submit')
	@event('form/submit')
	def on_submit(self, event, params, vars = None):
		gerenteCNF = GerenteConfigs()
		if params[0] == "defServerNet":
			if vars.getvalue('action') == "defineServidor":
				t = int(vars.getvalue("total_banda").split()[0])
				gerenteCNF.defineServ( vars.getvalue("interface"), vars.getvalue("total_banda").split()[0], str(t/10))
				
		elif params[0] == "add":
			if vars.getvalue('action') == 'OK':
				if vars.getvalue("nome") and vars.getvalue("ip") and vars.getvalue("banda_liberada") and vars.getvalue("delimitado") and vars.getvalue("isolado") and vars.getvalue("nome") and vars.getvalue("prioridade"):
					t = int(vars.getvalue("banda_liberada")) / 10
					aux = vars.getvalue('ip')
					if aux.find('/') <> -1:
						tipo = 'r'
					else:
						tipo = 'm'
					gerenteCNF.addConfig( vars.getvalue("banda_liberada"), t, vars.getvalue("prioridade"), vars.getvalue("delimitado"), vars.getvalue("isolado"), vars.getvalue("ip"), vars.getvalue("nome"), tipo )
					self._view = None
				else:
					self._error = "Todos campos tem que serem preenchidos..."
			else:
				self._view = None
		del gerenteCNF
