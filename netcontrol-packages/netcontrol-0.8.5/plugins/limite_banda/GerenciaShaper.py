#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from os import system, listdir
from ConfigParser import ConfigParser
from pprint import pprint
import exec_bash


class GerenciaShaper( object ):
	
	etc              = '/etc/shaper/'
	bash             = system
	device           = None
	banda_total      = None
	banda_total_down = None
	
	
	def __init__(self):
		arquivo = ConfigParser()
		arquivo.read( '/var/lib/netcontrol/plugins/limite_banda/etc/config_server' )
		self.device           = arquivo.get('servidor','interface')
		self.banda_total      = arquivo.get('servidor','banda')
		self.banda_total_down = arquivo.get('servidor','banda10')
		
		
	#RATE, WEIGHT, PRIO, RULE, BOUNDED, ISOLATED
	def addRegra(self, ipORrede, nome, banda_liberada, banda_down, prio, delimitado, isolado):
		#system("tc qdisc del dev %s root"%self.device)
		arquivo = ConfigParser()
		arquivo.read( '/var/lib/netcontrol/plugins/limite_banda/etc/config_server' )
		
		if not arquivo.has_section("contador_regras"):
			arquivo.add_section("contador_regras")
			arquivo.set("contador_regras","contador",1)
			arquivo.write( open('/var/lib/netcontrol/plugins/limite_banda/etc/config_server','w') )
			cont = "1"
		else:
			cont = int(arquivo.get("contador_regras", "contador")) + 1
			arquivo.set("contador_regras","contador", cont)
			arquivo.write( open('/var/lib/netcontrol/plugins/limite_banda/etc/config_server','w') )
			cont = str(cont)
		cont = "000%s"%(cont)
		ip   = ipORrede.replace('.','_').replace('/','_')
		user = nome.replace(' ','_')
		nome = "cbq-%s.%s%s-in"%(cont[-4:],ip,user)
		regra = "DEVICE=%s,%s,%s\n" \
				"RATE=%sKbit\n"   \
				"WEIGHT=%sKbit\n" \
				"PRIO=%s\n"       \
				"RULE=%s\n"       \
				"BOUNDED=%s\n"    \
				"ISOLATED=%s"%(self.device, self.banda_total, self.banda_total_down, banda_liberada, banda_down, prio, ipORrede, delimitado, isolado)
		open('%s%s'%(self.etc,nome),'w').write(regra)
		
	def delRegra(self, nome=None, ALL = False):
		system("tc qdisc del dev %s root"%self.device)
		a = open('/tmp/teste1','a')
		if ALL:
			system("rm /etc/shaper/*")
		else:
			busca = "%s-in"%(nome)
			arquivos = listdir("/etc/shaper")
			for arq in arquivos:
				a.write(arq+' '+busca)
				if arq.find(busca) <> -1:
					system('rm /etc/shaper/%s'%arq)
					break
		a.close()
	
	def compilaRegras(self):
		open('/tmp/compile','w').write( str(system('/etc/init.d/shaper compile')) )
		open('/tmp/compile1','w').write( str(system('/etc/init.d/shaper start')) )
		open('/tmp/compile2','w').write( str(system('/etc/init.d/shaper start')) )
	
