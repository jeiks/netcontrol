# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

class Rede:
	__ip   = None
	__masc = None
	__rede = None

	def __init__(self, ip, masc):
		self.setIP( ip )
		self.setMasc( masc )

	def __setRede(self):
		if self.__ip is not None and self.__masc is not None:
			self.__rede  = map( lambda x,y: x&y, self.__ip, self.__masc )

	def setIP(self, ip):
		self.__ip = map( lambda x: int(x),   ip.split('.') )
		self.__setRede()

	def setMasc(self, masc):
		self.__masc = map( lambda x: int(x), masc.split('.') )
		self.__setRede()

	def getIP(self):
		return self.__ip[:]

	def getMasc(self):
		return self.__masc[:]

	def getRede(self):
		return self.__rede[:]

	def getBroadcast(self):
		r = self.getRede()
		m = self.getMasc()

		return [ (255-m[0])|r[0],
		         (255-m[1])|r[1],
			 (255-m[2])|r[2],
			 (255-m[3])|r[3] ]

	def __retStr(self, f):
		return "%d.%d.%d.%d" % (f[0], f[1], f[2], f[3])

	def getIPAsStr(self):
		return self.__retStr( self.getIP() )

	def getMascAsStr(self):
		return self.__retStr( self.getMasc() )

	def getRedeAsStr(self):
		return self.__retStr( self.getRede() )

	def getBroadcastAsStr(self):
		return self.__retStr( self.getBroadcast() )

	def getQntIPs(self):
		m = self.getMasc()
		return (256-m[0])*(256-m[1])*(256-m[2])*(256-m[3])

	def ipIsValid(self, ip):
		tmp = Rede(ip,self.getMascAsStr())

		if self.getRedeAsStr() == tmp.getRedeAsStr():
			return True
		else:	return False

#ip   = raw_input('Digite o IP: ')
#masc = raw_input('Digite a máscara: ')
#ip = '10.10.0.200'
#masc = '255.255.255.248'

#a = Rede(ip, masc)
#print 'IP:', a.getIPAsStr()
#print 'Masc:', a.getMascAsStr()
#print 'Rede:', a.getRedeAsStr()
#print 'Broadcast:', a.getBroadcast()
#print 'Qnt. IPs:', a.getQntIPs()
