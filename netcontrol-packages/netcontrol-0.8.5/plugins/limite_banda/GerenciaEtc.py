#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
import os
from ConfigParser import ConfigParser
from pprint import pprint
from GerenciaShaper import GerenciaShaper

class GerenteConfigs( object ):
	
	arqCnf     = None
	arqMaquina = "/var/lib/netcontrol/plugins/limite_banda/etc/conf_maquina"
	arqRede    = "/var/lib/netcontrol/plugins/limite_banda/etc/conf_rede"
	gerShaper  = GerenciaShaper()
		
	def __init__(self,tipo=None):
		pass
	
	
	def addConfig(self, banda_liberada, banda_down, prio, getSobra, libeSobra, ip, nome, tipo):
		self.arqCnf = tipo == "m" and self.arqMaquina  or self.arqRede
		arquivo = ConfigParser()
		arquivo.read( self.arqCnf )
		ret = "Success"
		delimitado = getSobra == "sim" and "yes" or "no"
		isolado    = libeSobra == "sim" and "yes" or "no"
		if not arquivo.has_section( nome ) and not self.searchIP( ip, tipo ):
			arquivo.add_section( nome )
			arquivo.set( nome, "banda_liberada", banda_liberada )
			arquivo.set( nome, "banda_down"    , banda_down)
			arquivo.set( nome, "prio"          , prio )
			arquivo.set( nome, "delimitado"    , delimitado )
			arquivo.set( nome, "isolado"       , isolado )
			arquivo.set( nome, "ip"            , ip )
			arquivo.write( open(self.arqCnf,'w') )
			#ipORrede, nome, banda_liberada, banda_down, prio, delimitado, isolado
			self.gerShaper.addRegra( ip, nome, banda_liberada, banda_down, prio, delimitado, isolado )
		else:
			ret =  "Nome ou IP já cadastrado..."
		del arquivo
		#self.gerShaper.compilaRegras()
		return ret
	
	
	def delConfig(self, nome, ip,tipo =None):
		#self.decrementaRegras()
		ret  = True
		idem = "%s%s"%( ip.replace('.','_').replace('/','_'), nome.replace( ' ','_' ) )
		open('/tmp/teste','w').write( idem )
		self.gerShaper.delRegra( idem )
		self.arqCnf = tipo == "m" and self.arqMaquina  or self.arqRede
		arquivo = ConfigParser()
		arquivo.read( self.arqCnf )
		if arquivo.has_section(nome):
			arquivo.remove_section(nome)
			arquivo.write( open(self.arqCnf,'w') )
			del arquivo
		ret =  False
		#self.gerShaper.compilaRegras()
		return ret
	
	
	def getConfigs(self, m=False, r=False):
		ret = {"maquina":[], "rede": []}
		arquivo = ConfigParser()
		if m:
			arquivo.read( self.arqMaquina )
			for s in arquivo.sections():
				if int( arquivo.get(s,"prio") ) <= 3:
					prio = "Alta"
				elif int( arquivo.get(s,"prio") ) <= 5:
					prio = "Média"
				else:
					prio = "Baixa"
				ret["maquina"].append(
					{
						"ip"            : arquivo.get(s,"ip"),
						"nome"          : s,
						"prio"			: prio,
						"delimitado"    : arquivo.get(s,"delimitado"),
						"isolado"       : arquivo.get(s,"isolado"),
						"banda_down"    : arquivo.get(s,"banda_down"),
						"banda_liberada": arquivo.get(s,"banda_liberada"),
					}
				)
		if r:
			arquivo.read( self.arqRede )
			for s in arquivo.sections():
				if int( arquivo.get(s,"prio") ) <= 3:
					prio = "Alta"
				elif int( arquivo.get(s,"prio") ) <= 5:
					prio = "Média"
				else:
					prio = "Baixa"
				ret["rede"].append(
					{
						"ip"            : arquivo.get(s,"ip"),
						"nome"          : s,
						"prio"			: prio,
						"delimitado"    : arquivo.get(s,"delimitado"),
						"isolado"       : arquivo.get(s,"isolado"),
						"banda_down"    : arquivo.get(s,"banda_down"),
						"banda_liberada": arquivo.get(s,"banda_liberada"),
					}
				)
				
		return ret
	"""
	def editConfig(self, nomeAntigo, banda_liberada, banda_down, prio, getSobra, libeSobra, ip, nome, tipo):
		self.arqCnf = tipo == "m" and self.arqMaquina  or self.arqRede
		arquivo = ConfigParser()
		arquivo.read( self.arqCnf )
		
		if arquivo.has_section( nomeAntigo ):
			self.delConfig( nome,tipo )
			self.addConfig(banda_liberada, banda_down, prio, getSobra, libeSobra, ip, nome, tipo)
			del arquivo
			return True
		return False
	"""
	def searchIP(self,ip,tipo):
		self.arqCnf = tipo == "m" and self.arqMaquina  or self.arqRede
		arquivo = ConfigParser()
		arquivo.read( self.arqCnf )
		for s in arquivo.sections():
			if arquivo.get( s, "ip" ) == ip:
				return True
		return False

	def getInterfaces(self):
		os.system("ifconfig | grep 'Ethernet' | cat > /tmp/iface")
		f = open('/tmp/iface','r').readlines()
		interfaces = []
		for i in f:
			if i.find("Ethernet") <> -1:
				interfaces.append( i.split()[0] )
		return interfaces
	
	
	def searchConfig(self, nome):
		arquivo = ConfigParser()
		arquivo.read( self.arqRede )
		if arquivo.has_section( nome ):
			return {
					"ip"            : arquivo.get(nome,"ip"),
					"nome"          : nome,
					"prio"          : arquivo.get(nome,"prio"),
					"delimitado"    : arquivo.get(nome,"delimitado"),
					"isolado"       : arquivo.get(nome,"isolado"),
					"banda_down"    : arquivo.get(nome,"banda_down"),
					"banda_liberada": arquivo.get(nome,"banda_liberada"),
			}
		else:
			arquivo.read( self.arqMaquina )
		if arquivo.has_section( nome ):
			return {
					"ip"            : arquivo.get(nome,"ip"),
					"nome"          : nome,
					"prio"          : arquivo.get(nome,"prio"),
					"delimitado"    : arquivo.get(nome,"delimitado"),
					"isolado"       : arquivo.get(nome,"isolado"),
					"banda_down"    : arquivo.get(nome,"banda_down"),
					"banda_liberada": arquivo.get(nome,"banda_liberada"),
					"isolado"		: arquivo.get(nome,"isolado"),
					"delimitado"	: arquivo.get(nome,"delimitado"),
			}
	
	
	def defineServ(self, iface, banda, banda10):
		arq = "/var/lib/netcontrol/plugins/limite_banda/etc/config_server"
		open(arq,'w').write('')
		arquivo = ConfigParser()
		arquivo.read(arq)
		arquivo.add_section("servidor")
		arquivo.set("servidor", "interface", iface)
		arquivo.set("servidor", "banda"    , banda)
		arquivo.set("servidor", "banda10"  , banda10)
		arquivo.write( open(arq,'w') )
		del arquivo
	
	def getServer(self):
		arquivo = ConfigParser()
		arquivo.read("/var/lib/netcontrol/plugins/limite_banda/etc/config_server")
		info = {"iface":"Não definido", "total_banda":"Não definido", "banda10":"Não definido" }
		#try:
		info["iface"]       = arquivo.get( "servidor", "interface" )
		info["total_banda"] = arquivo.get( "servidor", "banda" )
		info["banda10"]     = arquivo.get( "servidor", "banda10" )
		#except:
		#	pass
		return info
		
	def compilaRegras(self):
		self.gerShaper.compilaRegras()
		
	
	def delAllConfigs(self, r=False, m=False):
		if r:
			open(self.arqRede,'w').write('')
		if m:
			open(self.arqMaquina,'w').write('')
		if r and m:
			self.gerShaper.delRegra(ALL=True)
		
	def decrementaRegras(self):
		arq = ConfigParser()
		arq.read("/var/lib/netcontrol/plugins/limite_banda/etc/config_server")
		i = int(arq.get('contador_regras','contador'))
		i = i <= 0 and 1 or i
		arq.set('contador_regras', 'contador', i-1)
		arq.write( open("/var/lib/netcontrol/plugins/limite_banda/etc/config_server",'w') )
