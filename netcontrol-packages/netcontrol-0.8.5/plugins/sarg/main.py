# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> e João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from netcontrol.ui import *
from netcontrol.com import implements
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol import apis
from backend import SquidSarg, Config
import datetime
import os


    
class Sarg(CategoryPlugin):
    text = 'Relatório WEB'
    icon = '/dl/sarg/icon.png'
    folder = 'apps'
    toReplace = { 'ACCESSED SITE': 'Site Acessado',
						'CONNECT': 'Conexão',
						'BYTES': 'Bytes',
						'%BYTES': '% Bytes',
						'IN-CACHE-OUT': 'Cache: Entrada-Saída',
						'ELAPSED TIME': 'Tempo de Duração',
						'MILLISEC': 'Milisegundos',
						'%TIME':'%Tempo',
						'AVERAGE' : 'Média',
						'TIME':'Tempo',
						'DATE':'Data',
						'"../../images/' : '"/dl/sarg/images/' }
    dic = {'download.html':'Downloads',
                   'topsites.html':'Sites mais acessados',
                   'siteuser.html':'Sites mais acessados por usuários',
                   'authfail.html':'Conexões Falhas',
                   'denied.html':'Acessos negados'}
    def on_session_start(self):
        self._tree = TreeManager()
        self._view = None
        self.sarg = SquidSarg()
        self._download = None
        os.system('rm -rf /var/lib/netcontrol/plugins/sarg/files/*.pdf')

    def get_ui(self):
      from os import listdir
      ui= self.app.inflate('sarg:main')
      
      # fazer download do pdf requerido
      if self._download is not None:
          ui.append('imprimePC', UI.IFrame(width="0%", height="0%", src="/dl/sarg/%s.pdf" % (self._download) ))                                                        
          self._download = None
      try: del root
      except: pass
      root = UI.TreeContainer(id='/', text='Períodos')   
      a = SquidSarg().getAcesso().keys()
      b = SquidSarg().getAcesso()
      p = []
      
      # imprime na tela os reports
      for i in a:
          t = UI.TreeContainer(id='/'+i,text=SquidSarg().formatDate(i),expanded=True,)
          t1 = UI.TreeContainer(text="Listar por Hosts")
          t2 = UI.TreeContainer(text="Listar por Usuários")
          t3 = UI.TreeContainer(text="Estátisticas")
          addT1 = False
          addT2 = False
          addT3 = False
          for j in b[i]:
              pc = UI.TreeContainerNode( UI.TipIcon(text="Gerar PDF",icon="/dl/sarg/pdf-icon.png", id="pdf/"+str(i)+'/'+str(j)) ).append( UI.LinkLabel(text=j.replace('_','.'),id=str(i)+'/'+str(j)) ) 
              if len( j.replace('_','.').split('.') ) == 4:
                  t1.append(pc)
                  addT1 = True
              else:
                  t2.append(pc)
                  addT2 = True
          
          for j in os.listdir('/var/lib/sarg/%s/'%i):
            if j.find('html') <> -1 and j.find('index.html') == -1:
                pc = UI.TreeContainerNode( UI.TipIcon(text="Gerar PDF",icon="/dl/sarg/pdf-icon.png",id="pdfEsta/"+str(i)+'/'+str(j)), id="pdf/"+str(i)+'/'+str(j) ).append( UI.LinkLabel( text=self.dic[j],id=str('estatisticas')+'/'+str(i)+'/'+str(j)) )
                t3.append(pc)
                addT3 = True
        
          if addT1: t.append(t1)
          if addT2: t.append(t2)
          if addT3: t.append(t3)
          
          t['expanded'] = False
          root.append(t)
      ui.append('tree',root)
      root['expanded'] = True

      if self._view is None:
          ui.remove('contSarg')
      #elif self._view == 'refresh_reports':

      elif self._view == 'refresh_reports':
		  print 'faz nada'

      elif self._view == 'estatisticas':
		s = open('/var/lib/sarg/%s'%(self.h)).read()
		for i in self.toReplace.keys():
		 s = s.replace(i, self.toReplace[i])
		texto = ''
		add = False
		for i in s.split('\n'):
			if i.find('class="report"') <> -1:
				add = True
			elif i.find('class="info"') <> -1:
				break
			if add: texto += i
		ui.find('contSarg').append(UI.FullDialog( UI.ScrollContainer(width="100%", height="100%").append(UI.DIV(style="font-size: 15px").append(UI.CustomHTML(html=texto)))))
		self._view = None
		  
      else:
		  #ui.find('site').set('src','http://192.168.10.160/html/squid-reports/%s/%s.html'%(self._view, self._view.split('/')[-1]))
		  s = open('/var/lib/sarg/%s/%s.html'%(self._view, self._view.split('/')[-1])).read()
		  for i in self.toReplace.keys():
			  s = s.replace(i, self.toReplace[i])
		  texto = ''
		  add = False
		  for i in s.split('\n'):
			  if i.find('class="report"') <> -1:
				  add = True
			  elif i.find('class="info"') <> -1:
				  break
			  if add: texto += i
		  ui.find('contSarg').append(UI.FullDialog( UI.ScrollContainer(width="100%", height="100%").append(UI.DIV(style="font-size: 8px").append(UI.CustomHTML(html=texto)))))
		  self._view = None
      return ui
       
        
    @event('button/click')
    def on_click(self, event, params, vars = None):
      if params[0] == 'pdf':
        s = open('/var/lib/sarg/'+str( params[1])+'/'+params[2]+'/'+params[2]+'.html' ).read()
        for i in self.toReplace.keys():
            s = s.replace(i, self.toReplace[i])
            data = datetime.datetime.now()
            info = params[2]
            if len( info.replace('_','.').split('.') ) == 4:
                info = params[2].replace('_','.')
                aux = "IP"
            else:
                aux = "Usuário"
            texto = '<html>'
            texto += """
                    <head>
                        <style>
                            th,td,tr {
                                text-align:center;
                            }
                            td, tr{
                                border: 1px solid black;
                            }
                        </style>
                    </head>
                    <body>
                    <center><h1>Flux Netcontrol &copy; - contato@fluxsoftwares.com - tel: (33) 3322-6309</h1></center>
                    <h3>
                        Data: %d/%d/%d - %d:%d:%d<br>
                        %s: %s
                    </h3>
            
                """ % ( data.day,data.month,data.year, data.hour, data.minute, data.second, aux, info)
            add = False
            for i in s.split('\n'):
                if i.find('class="report"') <> -1:
                    add = True
                elif i.find('class="info"') <> -1:
                    break
                if add: texto += i
        texto+="</body></html>"
        SquidSarg().geraPDF('relatorio_%s'%params[2],texto)
        self._download = 'relatorio_%s'%params[2]

      if params[0] == 'pdfEsta':
        s = open('/var/lib/sarg/'+str(params[1])+'/'+params[2]).read()
        for i in self.toReplace.keys():
            s = s.replace(i, self.toReplace[i])
            data = datetime.datetime.now()
            info = params[2]
            if len(info.replace('_','.').split('.')) == 4:
                info = params[2].replace('_','.')
                aux = "IP"
            else:
                aux = "Usuário"
            texto = '<html><body><head> <meta content="text/html; charset=UTF-8" http-equiv="content-type"/></head>'
            texto += """
                    <center><h1>NetControl</h1></center>
                    <h3>
                        Data: %d/%d/%d - %d:%d:%d<br>
                        %s: %s
                    </h3>
            
                """ % ( data.day,data.month,data.year, data.hour, data.minute, data.second, aux, self.dic[info] )
            add = False
            for i in s.split('\n'):
                if i.find('class="report"') <> -1:
                    add = True
                elif i.find('class="info"') <> -1:
                    break
                if add: texto += i
        texto+="</body></html>"
        SquidSarg().geraPDF('relatorio_%s'%params[2],texto)
        self._download = 'relatorio_%s'%params[2]
    
      if params[0] == 'voltar':
		  self._view = None
      elif params[0] == 'refresh_reports':
		  SquidSarg().refreshReports(1)
		  self._view = 'refresh_reports'
        

    @event('linklabel/click')
    def on_link(self, event, params, vars=None):
		if params[0] == 'estatisticas':
			self._view = 'estatisticas'
			self.h = str(params[1])+'/'+str(params[2])
			
		else:
			self._view = str(params[0]) +'/'+ str(params[1])
    
    @event('treecontainer/click')
    def on_tclick(self, event, params, vars=None):
        self._tree.node_click('/'.join(params))
        return '' 
