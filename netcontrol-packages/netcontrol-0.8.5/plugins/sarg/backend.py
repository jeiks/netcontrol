# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> e João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

import re
import os
import restart
import cStringIO
import ho.pisa as pisa
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *
from netcontrol import apis
class SquidSarg(object):
  periodos = []
  acesso = []
  sites = []
  folder = '/var/lib/sarg'
  
  def __init__(self):
    print 'to -do'
    
  def geraPDF(self,nome,texto):
        arq = open('/var/lib/netcontrol/plugins/sarg/files/%s.pdf'%(nome),'wb')
        pdf = pisa.CreatePDF(texto, arq)
        arq.close()
  def formatDate(self,mes):
      try:
          de,ate = mes.split('-')[0],mes.split('-')[1]
          dano = str(de[0:4])
          dm   = str(de[4:7]+'/')
          ddia = str(de[7:9]+'/')
          aano = str(ate[0:4])
          am   = str(ate[4:7]+'/')
          adia = str(ate[7:9]+'/')
          return 'De: '+ddia+' '+dm+' '+dano+' : Até '+adia+' '+am+' '+aano
      except:
          return mes    
  
  def getPeriodos(self):
    for mes in os.listdir(self.folder):
      if mes <> 'images' and mes <> 'index.html' and mes <> 'Daily' and mes <> 'Weekly' and mes <> 'Monthly':
		  try: self.periodos.index(mes)
		  except: self.periodos.append(mes)
    return self.periodos
  
  def getAcesso(self):
	dicio = {}
	for mes in self.getPeriodos():
		dicio[mes] = []
		for acessos in os.listdir('%s/%s/'% (self.folder,mes)):
			if acessos.find('.html') == -1 and acessos.find('sarg') == -1:
				dicio[mes].append(acessos)
			
	return dicio

  def getSites(self,dirr):
	  for site in os.listdir('%s/%s'%(self.folder,dirr)):
		  self.sites.append(site)
	  return self.sites
  
  def refreshReports(self,r):
      if r == 1:
		  a = os.system('sarg')
      elif r == 2:
		  restart.restart('/etc/init.d/squid3 restart',True)
	  
    

class Config(Plugin):
    implements(IConfigurable)
    name = 'Relatório WEB'
    icon = '/dl/hosts/icon.png'
    id = 'sarg'

    def list_files(self):
        return ['/etc/hosts']

    def read(self):
        ss = ConfManager.get().load('hosts', '/etc/hosts').split('\n')
        r = []
        for s in ss:
            if s != '' and s[0] != '#':
                try:
                    s = s.split()
                    h = Host()
                    try:
                        h.ip = s[0]
                        h.name = s[1]
                        for i in range(2, len(s)):
                            h.aliases += '%s ' % s[i]
                        h.aliases = h.aliases.rstrip();
                    except:
                        pass
                    r.append(h)
                except:
                    pass

        return r
    
        
    def save(self, hh):
        d = ''
        for h in hh:
            d += '%s\t%s\t%s\n' % (h.ip, h.name, h.aliases)
        ConfManager.get().save('hosts', '/etc/hosts', d)
        ConfManager.get().commit('hosts')


        
