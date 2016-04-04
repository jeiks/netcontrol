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
import subprocess
from restart import * 

class Proftpd(CategoryPlugin):
    """
    @author: <joaopauloctga@gmail.com>
    """
    text = 'FTP' 
    icon = '/dl/proftpd/icon.png'
    folder = 'servers' 

    def on_init(self):
        be = backend.Config(self.app)
        self.pro = be.read()
 
    def on_session_start(self):
        self._editing = None
        self._respsota = None
        self._msg = None
        
 
    def get_ui(self):
        ui = self.app.inflate('proftpd:main')
        try:
            backend.ProftpLDAP().setConfigLDAP()
        except:
            if self.pro is False:
                error = ui.find( 'error' )
                error.append( UI.Label(text="Nenhuma base de dados LDAP foi encontrada, configure uma base no modulo de Auntenticação para usar o serviço de FTP.", size=3) )
                return error
                
        if self._editing == None:
            ui = self.app.inflate('proftpd:main')
            a = ui.find('box')
            cont = 0
            a.append(UI.Table(id='box1'))
            a.append(UI.Table(id='box2'))
            b = ui.find('box1')
            c = ui.find('box2')
            separador = len(self.pro) / 2
            for i in self.pro:
                if cont < separador:
                    b.append(
                        UI.DTR(
                        UI.DTH(UI.Image(file='/dl/core/ui/stock/user.png')),
                        UI.DTH(UI.Label(text=i.nome,value=i.nome)),
                        UI.DTH(UI.Checkbox(checked=i.check,id=cont,name=i.nome))
                        )
                    )
                else:
                    c.append(
                        UI.DTR(
                        UI.DTH(UI.Image(file='/dl/core/ui/stock/user.png')),
                        UI.DTH(UI.Label(text=i.nome,value=i.nome)),
                        UI.DTH(UI.Checkbox(checked=i.check,id=cont,name=i.nome))
                        )
                    )
                cont += 1
        elif self._editing == "restart":
            t = ui = self.app.inflate("proftpd:edit")
            t.find("resposta").append(  UI.DTR(  UI.Label(text=self._resposta)  )  )
        elif self._editing == "boas_vindas":
            t= ui = self.app.inflate("proftpd:edita")
            t.find("texto").set('value',self._msg)
        return ui
    
    @event('dialog/submit')
    def on_dialog(self, event, params, vars = None):
        if params[0] == 'resposta_proftpd':
            if vars.getvalue('action','') == 'OK':
                self._editing = None
        elif params[0] == "msg_user":
            if vars.getvalue('action','') == 'OK':
                a = open("/usr/share/proftpd/templates/welcome.msg","w")
                a.write(str(vars.getvalue('texto','none')))
            self._editing = None
    
    @event('button/click')
    @event('form/submit')
    def on_click(self, event, params, vars = None):
        if vars.getvalue('action','') == 'msg':
            a = open("/usr/share/proftpd/templates/welcome.msg").read()
            self._editing = "boas_vindas"
            self._msg = a
        elif vars.getvalue('action','') == 'libUsers':
            a = open("/etc/ftpusers",'w')
            a.write('# /etc/ftpusers: list of users disallowed ftp access. See ftpusers(5).\n\n')
            a.close()
            restart("/etc/init.d/proftpd restart", True)
            self._resposta = "ProFTPD reiniciado com sucesso"
            self._editing = "restart"
                    
        elif vars.getvalue('action','') == 'blockUsers':
            a = open('/etc/ftpusers','w')
            a.write('# /etc/ftpusers: list of users disallowed ftp access. See ftpusers(5).\n\n')
            for i in vars.list[1:]:
                name, value = i.name, int(i.value)
                a.write( str( name + '\n' ) )
            for i in open('/etc/passwd').readlines():
                i = i.split(':')
                if int(i[2]) < 1000 and i[0] <> 'root':
                    a.write(i[0]+'\n')
            a.close()
            restart("/etc/init.d/proftpd restart","True")
            self._resposta = "ProFTPD reiniciado com sucesso"
            self._editing = "restart"
        elif params[0] == 'box':
            a = open('/etc/ftpusers','w')
            a.write('# /etc/ftpusers: list of users disallowed ftp access. See ftpusers(5).\n\n')
            for i in vars.list[1:]:
                name, value = i.name, int(i.value)
                if value == 0:
                    a.write( str( name + '\n' ) )
            for i in open('/etc/passwd').readlines():
                i = i.split(':')
                if int(i[2]) < 1000 and i[0] <> 'root':
                    a.write(i[0]+'\n')
            a.close()
            restart("/etc/init.d/proftpd restart","True")
            self._resposta = "ProFTPD reiniciado com sucesso"
            self._editing = "restart"
        elif params[0] == "msg_user":
            if vars.getvalue('action','') == 'OK':
                a = open("/usr/share/proftpd/templates/welcome.msg","w")
                a.write(str(vars.getvalue('texto','none')))
            self._editing = None
