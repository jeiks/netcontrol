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
import os
from ControleBanda import ContrBanda
      
 
class Contr_Banda(CategoryPlugin):
    """
    @author: <joaopauloctga@gmail.com> & <jacsonrcsilva@gmail.com>
    """
    text = 'Contr. Banda' 
    icon = '/dl/controle_banda/icon.png'
    folder = 'system'
    errorFiltro = None
    def on_init(self):
        be = backend.Config(self.app)
        self.hosts = be.read()
        
    def getInterface(self):
      if os.access('/proc/net/dev', 4):
	ret = []
	for i in open('/proc/net/dev').readlines():
	  if i.find(':')    <> -1 and i.find('lo')   == -1 and i.find('vbox') == -1 and i.find('pan0') == -1:
	    ret.append(i.split(':')[0].strip())
	return ret

      return []
      
    def on_session_start(self):
        self._editing = None
        
 
    def get_ui(self):
        config = ContrBanda()
        confBanda = config.getConfigFile()
        # testando se a alguma configuracao de banda
	if confBanda.get('interface') == None or confBanda.get('medida') == None or confBanda.get('banda') == None:
	  ui = self.app.inflate('controle_banda:cad_banda')
	  lista = ui.find('interface')
	  for i in self.getInterface():
	    lista.append( UI.SelectOption(value=i,text=i) )
        # caso tenha uma banda já configurada segue adiante com opcoes de redefinir o controle de banda e cadastrar novos filtros etc
	else:
	  ui = self.app.inflate('controle_banda:main')
	  lista = ui.find('interface')
	  ui.find('banda').set('value',confBanda.get('banda'))  # mostrando a quantidade de banda que esta configurada
	  
	  
	  med = ui.find('tipo_medida')
	  if confBanda.get('medida') == 'MBytes':
            med.append(
                            UI.TipIcon(
                                icon='/dl/core/ui/stock/info.png',
                                text='Quantidade de banda em MBytes',
                            )
                        )
          else:
              med.append(
                            UI.TipIcon(
                                icon='/dl/core/ui/stock/info.png',
                                text='Quantidade de banda em KBytes',
                            )
                        )
	  
	  mascaras = ui.find('mascara')
	  for i in range(33):
	    mascaras.append( UI.SelectOption(value=int(32 - i),text=int(32 - i)) )
          # imprimindo interfaces que podem ser definidas
	  for i in self.getInterface():
	    if i == confBanda.get('interface'):
	      lista.append( UI.SelectOption(value=i,text=i,selected=True) )
	  
	  # imprimindo os filtros cadastrados se tiverem
	  t = ui.find('list')
	  if self.hosts == []:
	    t.append(UI.DTR(UI.DTH(UI.Label(text="Nenhum filtro encontrado..."))))
	  else:
	    for h in self.hosts:
		t.append(UI.DTR(
		    UI.Label(text=h.ip+'/'+h.mascara),
		    UI.Label(text=(h.banda+' '+h.medida)),
		    UI.Label(text=h.nome),
		    UI.HContainer(
			UI.TipIcon(
			    icon='/dl/core/ui/stock/delete.png',
			    id='del/'+str(h.ip),
			    text='Remover filtro',
			)
		    ),
		))
	
	  
        if self._editing is not None:
            # caso o usuario tente cadastrar uma banda sem passar nenhum parametro
	    if self._editing == 'error_cadastrar_banda':
	      e = ui.find('error')
	      e.append(UI.Label(text="Todos os campos tem que ser preenchidos !!!",size=3))
            # trata o erro quando o usuario tenta exluir todos os filtros sendo que nao a nenhum a ser excluido
	    elif self._editing == 'error_apagar_todos_filtros':
              e = ui.find('error')
              e.append( UI.Label(text='Não a nenhum filtro a ser apagado...') )
              self._editing = None
	    # caso haja algum erro no cadastro de filtros
	    elif self._editing == 'error_cadastrar_filtro':
              if self.errorFiltro:
                e = self.app.inflate('controle_banda:msgerro')
                e.find('msg').set('text','Filtro já cadastrado !')
                ui.append('error',e)
                self._editing = None
              else:
		e = self.app.inflate('controle_banda:msgerro')
                e.find('msg').set('text','Todos os campos tem que ser preenchidos !')
                ui.append('error',e)
                self._editing = None
	    else:
		
	      try:
		  h = self.hosts[self._editing]
	      except:
		  h = backend.Host()
	      d = self.app.inflate('controle_banda:edit') # inflate and fill the dialog
	      d.find('ip').set('value', h.ip)
	      masc = d.find('mascara')
	      for i in range(33):
		if str(h.mascara) == str(32-i):
		  masc.append( UI.SelectOption(value=int(32 - i),text=int(32 - i),selected=True ) )
		else:
		  masc.append( UI.SelectOption(value=int(32 - i),text=int(32 - i)) )
	      d.find('banda').set('value', h.banda)
	      med = d.find('medida')
	      for i in ['Mbits','Kbits']:
		if h.medida == i:
		  med.append( UI.SelectOption(value=i,text=i, selected=True) )
		else:
		  med.append( UI.SelectOption(value=i,text=i) )
	      d.find('id_pc').set('value', h.id_pc)
	      ui.append('lista_de_ips', d) # and append it to main UI
 
        return ui
 
    @event('button/click')
    def on_click(self, event, params, vars = None):
        if params[0] == 'edit':
            self._editing = int(params[1]) # selected host
        elif params[0] == 'del':
            config = ContrBanda()
            config.delFiltro(ip=params[1],ALL=True)
            del config
        elif params[0] == 'apagar_conf':
            config = ContrBanda()
            config.delConfiguracoes()
            del config
	elif params[0] == 'apagar_filtros':
            config = ContrBanda()
            if not config.delFiltro():
              self._editing = 'error_apagar_todos_filtros'
            del config
            
 
       
        
    @event('form/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == "cad_banda":
            if vars.getvalue('action','') == 'cadastrar_banda':
	      a = vars.list[2]
	      if vars.getvalue('interface') == '' or vars.getvalue('banda') == '' or a.value == '':
		self._editing = 'error_cadastrar_banda'
	      else:
                config = ContrBanda()
                config.setConfigFile(vars.getvalue('interface'),vars.getvalue('banda'),a.value)
                config.setConfiguracoes()
                del config
                self._editing = None
                
        elif params[0] == "cad_ip":
            if vars.getvalue('action','') == 'cadastrar_ip':
	      if vars.getvalue('ip') == '' or vars.getvalue('mascara') == '' or vars.getvalue('banda_pc') == '' or vars.getvalue('') == 'id_pc' or vars.list[-2].value == '':
		self._editing = 'error_cadastrar_filtro'
	      else:		  
		ip = '%s'%vars.getvalue('ip')
		mascara = '%s' %vars.getvalue('mascara')
		banda = '%s'%vars.getvalue('banda_pc')
		medida = '%s'%vars.list[-2].value
		id_pc = '%s' %vars.getvalue('id_pc')
		aux = ContrBanda()
		if aux.addFiltroFile(ip,mascara,banda,id_pc,medida):
                    aux.addFiltro(ip=ip)
                    self.errorFiltro = False
                else:
                    self.errorFiltro = True
                    self._editing    = 'error_cadastrar_filtro'
		del aux
