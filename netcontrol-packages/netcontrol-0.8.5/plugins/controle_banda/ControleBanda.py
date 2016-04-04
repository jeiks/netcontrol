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

class Filtro:
	ip 		= None
	mascara = None
	banda   = None
	iden 	= None
	medida  = None
	
	def __init__(self):
		pass


class ContrBanda(object):
	
	bash 		  = os.system
	totalBanda    = None
	interfaceRede = None
	contFiltros	  = None
	medida 		  = None
	
	
	# contrutor com funcao de preparar os arquivos a serem manipulados
	def __init__(self):
		if not os.access('/etc/netcontrol/controle_banda',7):
			try: os.makedirs('/etc/netcontrol/controle_banda', 0755)
			except: pass
		try:
			open('/etc/netcontrol/controle_banda/config','r')
		except:
			open('/etc/netcontrol/controle_banda/config','w')
		try:
			open('/etc/netcontrol/controle_banda/filtros','r')
		except:
			open('/etc/netcontrol/controle_banda/filtros','w')
	
	# incrementar indice dos filtros
	def incrementaFiltro(self,cont):
		arq = ConfigParser()
		arq.read('/etc/netcontrol/controle_banda/config')
		arq.set('configuracao','contador_filtros',cont)
		arq.write( open("/etc/netcontrol/controle_banda/config","w") )
		del arq
		
	
	#metodo para cconverter medida em K ou M para bits	
	def convBits(self,medida,banda):
		banda = int(banda)
		if medida == 'KBytes':
			return int( banda * 8 )
			
		elif medida == 'MBytes':
			return int( banda * 1024 * 8 )
		else:
			return None
	
	# escreve no arquivos de configuracoes
	def setConfigFile(self, interface, banda, medida):
		arquivo = ConfigParser()
		arquivo.read('/etc/netcontrol/controle_banda/config')
		if not arquivo.has_section('configuracao'):
			arquivo.add_section('configuracao')
		arquivo.set( 'configuracao', 'interface', interface )
		arquivo.set( 'configuracao', 'banda'    , banda )
		arquivo.set( 'configuracao', 'medida'   , medida)
		arquivo.set( 'configuracao', 'contador_filtros'   , '1')
		arquivo.write( open('/etc/netcontrol/controle_banda/config','w') )
		self.interfaceRede = interface
		del arquivo
		
	# retorna uma dicionario com as configuracoes escritas no arquivo
	def getConfigFile(self,dirr='/etc/netcontrol/controle_banda/config'):
		arquivo = ConfigParser()
		arquivo.read(dirr)
		if arquivo.has_section('configuracao'):
			interface = arquivo.get('configuracao','interface')
			banda     = arquivo.get('configuracao','banda')
			medida    = arquivo.get('configuracao','medida')
			ID	      = arquivo.get('configuracao','contador_filtros')
			del arquivo
			return {'interface' : interface, 'banda' : banda, 'medida' : medida, 'contador_filtros' : ID}
		else:
			return {'interface' : None, 'banda' : None, 'medida' : None, 'contador_filtros' : None}
	
	#deleta todas as configuracoes
	def delConfiguracoes(self):
		interface = self.getConfigFile().get('interface')
		banda     = self.convBits( self.getConfigFile().get('medida') , self.getConfigFile().get('banda') )
		self.bash("tc qdisc del dev %s root handle 1: htb default 10" % interface )
		self.bash("tc class del dev %s root classid 1:0 htb rate %s" % (interface , banda))
		open('/etc/netcontrol/controle_banda/config','w').write('')
		open('/etc/netcontrol/controle_banda/filtros','w').write('')
	
	
	# seta as configuracoes do controlde banda
	def setConfiguracoes(self):
		interface = self.getConfigFile().get('interface')
		banda     = self.convBits( self.getConfigFile().get('medida') , self.getConfigFile().get('banda') )
		self.bash("tc qdisc add dev %s root handle 1: htb default 10" % interface )
		self.bash("tc class add dev %s parent 1: classid 1:1 htb rate %s ceil %s" % (interface , banda, banda))
	
	#remover filtro do arquivo
	def delFiltroFile(self,ip,ALL = False):
		if ALL:
			open('/etc/netcontrol/controle_banda/filtros','w').write('')
			return True
		arquivo = ConfigParser()
		arquivo.read('/etc/netcontrol/controle_banda/filtros')
		if arquivo.has_section(ip):
			arquivo.remove_section(ip)
			arquivo.write( open('/etc/netcontrol/controle_banda/filtros','w') )
			del arquivo
			return True
		else:
			del arquivo
			return False
	
	# escreve no arquivo filtro adicionando o filtro desejado
	def addFiltroFile(self, ip, mascara, banda, nome, medida, ID = None ):
		arquivo = ConfigParser()
		arquivo.read('/etc/netcontrol/controle_banda/filtros')
		
		if not arquivo.has_section(ip):
			arquivo.add_section(ip)
			arquivo.set( ip, 'mascara',  mascara )
			arquivo.set( ip, 'banda'  ,  banda )
			arquivo.set( ip, 'nome'   ,  nome )
			arquivo.set( ip, 'medida' ,  medida )
			if ID == None:
				ID = int( self.getConfigFile().get('contador_filtros') ) + 1
			arquivo.set( ip, 'iden' ,  ID )
			arquivo.write( open('/etc/netcontrol/controle_banda/filtros','w') )
			self.incrementaFiltro(ID)
			return True
		else:
			return False
		
		del arquivo
	
	# le o arquivo que contem os filtros e retorna seus respctivos valores
	def getFiltrosFile(self):
		arquivo = ConfigParser()
		arquivo.read('/etc/netcontrol/controle_banda/filtros')
		filtros    = arquivo.sections()
		filtrosAll = []
		for filtro in filtros:
			f = Filtro()
			f.ip 	  = filtro
			f.mascara = arquivo.get( filtro, 'mascara')
			f.banda   = arquivo.get( filtro, 'banda')
			f.nome	  = arquivo.get( filtro, 'nome')
			f.medida  = arquivo.get( filtro, 'medida')
			f.iden    = arquivo.get( filtro, 'iden')
			filtrosAll.append( f )
			del f
		del arquivo
		del filtros
		return filtrosAll
	
	# adiciona todos os filtros lidos no arquivo '/etc/netcontrol/controle_banda/filtros'
	def addFiltro(self,ip=None,ALL=False):
		self.interfaceRede = self.getConfigFile().get('interface')
		self.totalBanda    = self.getConfigFile().get('banda')
		if ALL:
			for filtro in self.getFiltrosFile():
				self.bash("tc class add dev %s parent 1:1 classid 1:%s htb rate %s ceil %s"%(self.interfaceRede,filtro.iden,filtro.banda,self.totalBanda))
				self.bash("tc filter add dev %s protocol ip parent 1:0 prio 1 u32 match ip src %s/%s match ip dport 80 0xffff flowid 1:%s"%(self.interfaceRede,filtro.ip,filtro.mascara,filtro.iden))
			return True
		else:
			for filtro in self.getFiltrosFile():
				if filtro.ip == ip:
					self.bash("tc class add dev %s parent 1:1 classid 1:%s htb rate %s ceil %s"%(self.interfaceRede,filtro.iden,filtro.banda,self.totalBanda))
					self.bash("tc filter add dev %s protocol ip parent 1:0 prio 1 u32 match ip src %s/%s match ip dport 80 0xffff flowid 1:%s"%(self.interfaceRede,filtro.ip,filtro.mascara,filtro.iden))
					return True
	
	# remove filtros
	def delFiltro(self,ip=None,ALL=False):
		arq = ConfigParser()
		self.interfaceRede = self.getConfigFile().get('interface')
		self.totalBanda = self.getConfigFile().get('banda')
		arq.read('/etc/netcontrol/controle_banda/filtros')
		
		if ALL:
			self.delFiltroFile(ip)
                        self.bash("tc qdisc del dev %s root handle 1: htb default 10" % self.interfaceRede )
                        self.bash("tc class del dev %s root classid 1:0 htb rate %s" % (self.interfaceRede , self.totalBanda))
                        self.setConfiguracoes()
			self.addFiltro(ALL=True)
			return True
		else:
                        try:
                           ip = arq.sections()[0]
                        except:
                           return False
                        iden    = arq.get(ip,'iden')
                        banda   = arq.get(ip,'banda')
                        mascara = arq.get(ip,'mascara')
                        ip = self.getFiltrosFile()[0].ip
                        mascara =self.getFiltrosFile()[0].mascara
                        #iden = self.getFiltrosFile()[0].iden
			self.delFiltroFile(ip)
			self.bash("tc filter del dev %s protocol ip parent 1:0 prio 1 u32 match ip src %s/%s match ip dport 80 0xffff flowid 1:%s"%(self.interfaceRede,ip,mascara,iden))

