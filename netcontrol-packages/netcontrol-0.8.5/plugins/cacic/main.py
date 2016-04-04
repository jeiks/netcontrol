#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

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
import os
import MySQLdb
from cacicColeta import Redes,Coleta,ConfigCacic
from installCacic import InstallCacic
fileSync = '/tmp/netcontrolCacicControl.pid'

def exec_as_daemon(s_path_cmd, l_args=[]):
    i_pid = os.fork()
    if i_pid != 0:
        # Always remember to gobble your zombie children
        os.wait()
        # Back to parent
        return
    # Detach from parent
    os.setsid()
    # Do not block any mounts
    os.chdir('/')
    # Reset file creation rights
    os.umask(0)
    i_pid = os.fork()
    if i_pid != 0:
        open( fileSync, 'w' ).write(str(i_pid))
        # Close the direct child process
        os._exit(0)
    # Get the maximum count of open file handles
    try:
        import resource
        i_fd_max = resource.getrlimit(resource.RLIMIT_NOFILE)[1]
        if i_fd_max == resource.RLIM_INFINITY:
            i_fd_max = 1024
    except ImportError:
        i_fd_max = 1024
    # Try to close all possible file handles
    for i_cur_fd in range(0, i_fd_max):
        try:
            os.close(i_cur_fd)
        except OSError:
            pass
    # Assosiate STDIN with /dev/null
    os.open(os.devnull if hasattr(os, "devnull") else '/dev/null', os.O_RDWR)
    # STDOUT to /dev/null
    os.dup2(0, 1)
    # STDERR to /dev/null
    os.dup2(0, 2)
    l_args.insert(0, s_path_cmd)
    # Let the clone become a other process

    os.execv(s_path_cmd, l_args)
    
class Cacic(CategoryPlugin):
    """
    @author: <joaopauloctga@gmail.com>
    """
    text = 'Inventário hw/sw'
    icon = '/dl/cacic/icon.png'
    folder = 'apps'

    def _execute(self, command, daemon=False):
        '''
        @return:
            [ (Boolean) Coomand executed with sucess ,
              (tuple) (stdout,stderr)                  ]
        '''
        cmd = command.split()
        print cmd[0], '::' , cmd[1:]
        if daemon:
            from time import sleep
            from os import access
            open( fileSync ,'w').write('1')
            exec_as_daemon(cmd[0], cmd[1:])
            a = open( fileSync ).read()
            while access('/proc/%s'%a, 1):
                sleep(1)
                a = open( fileSync ).read()
        else:
            from os import system
            system(command)
        return [ True, 'Success' ]
  
 
    def on_session_start(self):
        self._tree = TreeManager()
        self._getAgente = False
        self._view = None
        self._ldb = []
        self._defcacic = None
        self._install = InstallCacic
        self._errorInstall = [False,None]
        self._download = None

    def get_ui(self):
      ui= self.app.inflate('cacic:main')
      coleta = Coleta()
      ob = Redes()
      config = ConfigCacic()
      # faz download do arquivo que o usuario escolheu
      if self._download <> None:
          if self._download == "/dl/cacic/pycacic.deb":
              ui.append('imprimePC', UI.IFrame(width="0%", height="0%", src="/dl/cacic/pycacic.deb"))
              self._download = None
          elif self._download == "/dl/cacic/agente-windows-2.4.zip":
              ui.append('imprimePC', UI.IFrame(width="0%", height="0%", src="/dl/cacic/agente-windows-2.4.zip"))
              self._download = None
      
      
      # aqui onde mostra a unica opcao de instalar o servidor cacic
      ca = ob.getComputadores()
      ce = coleta.getComputador()
      if coleta.getServer()[0] == 'False' or ca == False or ce == False:
        return self.app.inflate('cacic:install')
      
      
      # testando se houve erro na instalação do cacic
      if self._errorInstall[0]:
        error = self.app.inflate('cacic:erroInstall')
        e = error.find('t1')
        e.append(
          UI.Label(text="%s"% self._errorInstall[1],size=4)
        )
        return error
      # pega o pycacic escolhido pelo usuario
      if self._getAgente:
          self._getAgente = False
          agente = self.app.inflate('cacic:getAgente')
          ip_server = self.app.environ['SERVER_NAME'].replace("http://","")
          if ip_server == "127.0.0.1" or ip_server == "localhost":
			  ip_server = "Digite aqui IP do servidor."
          agente.find('server').set('value', ip_server)
          return agente

      root = UI.TreeContainer(id='/', text='Redes Monitoradas')
      try:ui.find('senha').set('value',config.getConfigAgentes()[0][1])
      except:pass
      
      try:ui.find('passwd').set('text','Sua senha atual é = %s'%config.getConfigAgentes()[0][1])
      except: pass
      select = ui.find('inicia_coleta')
      dic = {"TU":"Imediatamente após a inicialização do CACIC","10":"10 minutos após a inicialização do CACIC","20":"20 minutos após a inicialização do CACIC","30":"30 minutos após a inicialização do CACIC","1":"1 hora após a inicialização do CACIC"}
      try:
        for op in dic.values():
          if op == dic.get(str(config.getConfigAgentes()[0][0])):
             select.append(UI.SelectOption(value=op,text=op,selected="True"))
          else:
             select.append(UI.SelectOption(value=op,text=op,selected="False"))
      except:
          pass      
      select = ui.find('intervalo_coleta')
      dic = {"2":"A cada 2 horas","4":"A cada 4 horas","6":"A cada 6 horas","8":"A cada 8 horas","10":"A cada 10 horas"}
      try:
          for op in dic.values():
              if op == dic.get(str(config.getConfigAgentes()[0][2])):
                 select.append(UI.SelectOption(value=op,text=op,selected="True"))
              else:
                 select.append(UI.SelectOption(value=op,text=op,selected="False"))
      except:
          pass      
      chaves = ob.getComputadores().keys()
      dicio = ob.getComputadores()
      for i in chaves:
          t = UI.TreeContainer(id='/'+i,text=i,expanded=True,)
          for j in dicio[i]:
              pc = UI.LinkLabel(text=j,id=j)
              t.append(UI.TreeContainerNode(pc))
          root.append(t)
      ui.append('tree',root)
      root['expanded'] = True
      if self._view is not None: 
          comp = UI.Table( height="100%", width="100%" )
          for k in coleta.getComputador():
              if k[8] == self._view:
                  for l in k:
                      if l is not None  and l is not "":
                          comp.append(
                                  UI.DTR(
                                      UI.DTH(UI.Label(size=1, text=coleta.indice[int(k.index(l))])),
                                      UI.DTH(UI.Label(size=1, text=str(l)))
                                  ),
                          )
          ui.append('imprimePC', UI.FullDialog(width="650px").append(UI.ScrollContainer(width="100%", height="100%").append(comp)) )
          self._view = None
      return ui
       
    def get_debs(self):
        ret = []
        for i in os.listdir('/var/lib/netcontrol/plugins/cacic/pycacic/'):
            if i[-4:] == '.deb': ret.append(i)
        return ret
            
    @event('button/click')                                                    
    def on_click(self, event, params, vars = None):
        if params[0] == 'install_serv':
          self._view = 'install_serv'
          self._errorInstall = [False,None]
          if self._install().setFilesCacic():
            if self._install().setAcesso():
              if self._install().installDB():
                if self._install().setConfig():
                  pass
                else:
                  self._errorInstall = [True,'Falha ao realizar configurações no arquivo /etc/php5/apache2/php.ini,(Verifique se os pacotes php5 e apache2 estão instaldos corretamente.)']
              else:
                  self._errorInstall = [True,'Falha ao criar o banco de dados.(Verifique se não a nenhum banco de dados com o nome cacic e se o mysql está funcionando corretamente.)']
            else:
              self._errorInstall = [True,'Falha ao dar permissoes aos arquivos !!!(Verifique a permissão do diretório /var/www/cacic2)']
          else:
              self._errorInstall = [True,'Falha ao copiar arquivos para o diretório /var/www/']
        elif params[0] == 'del_serv':
          self._install().delCacic()

        elif params[0] == "getAgente":
            self._getAgente = True
        self._view = None
           


    @event('form/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'conf_agentes':
           dic = {"Imediatamente após a inicialização do CACIC":'i',"10 minutos após a inicialização do CACIC":"10","20 minutos após a inicialização do CACIC":"20","30 minutos após a inicialização do CACIC":"30","1 hora após a inicialização do CACIC":"1"}
           ini = dic[vars.getvalue('inicia_coleta')]
           senha = vars.getvalue('senha')
           dic = {"A cada 2 horas":"2","A cada 4 horas":"4","A cada 6 horas":"6","A cada 8 horas":"8","A cada 10 horas":"10"}
           ex = dic[vars.getvalue('intervalo_coleta')]
           ConfigCacic().setConfigAgentes(ini,senha,ex)

 
    

    @event('dialog/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'getAgenteEstacao':
           if vars.getvalue('action') == 'OK':
              self._download = 'Linux'
              server = vars.getvalue('server')
              arq = vars.getvalue('arq')
              if arq == 'linux':
                debs = self.get_debs()
                for i in debs:
                    os.remove('/var/lib/netcontrol/plugins/cacic/pycacic/%s'%i)
                cmd = 'python /var/lib/netcontrol/plugins/cacic/pycacic/setservice.py http://%s' % server
                self._execute( cmd, False )
                debs = self.get_debs()
                if debs == []:
                    print 'Erro'
                else:
                    os.system('ln -fs /var/lib/netcontrol/plugins/cacic/pycacic/%s /var/lib/netcontrol/plugins/cacic/files/pycacic.deb' % debs[0])
                self._download = "/dl/cacic/pycacic.deb"
              else:
                  self._download = "/dl/cacic/agente-windows-2.4.zip"
        if params[0] == 'conf_agentes':
           dic = {"Imediatamente após a inicialização do CACIC":'i',"10 minutos após a inicialização do CACIC":"10","20 minutos após a inicialização do CACIC":"20","30 minutos após a inicialização do CACIC":"30","1 hora após a inicialização do CACIC":"1"}
           ini = dic[vars.getvalue('inicia_coleta')]
           senha = vars.getvalue('senha')
           dic = {"A cada 2 horas":"2","A cada 4 horas":"4","A cada 6 horas":"6","A cada 8 horas":"8","A cada 10 horas":"10"}
           ex = dic[vars.getvalue('intervalo_coleta')]
           ConfigCacic().setConfigAgentes(ini,senha,ex)

    @event('linklabel/click')
    def on_link(self, event, params, vars=None):
        self._view = params[0]
    
    @event('treecontainer/click')
    def on_tclick(self, event, params, vars=None):
        self._tree.node_click('/'.join(params))
        return '' 
