# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from netcontrol.api import *
from netcontrol.ui import *
import backend
import netifaces as ni

class LTSPPlugin(CategoryPlugin):
    text = 'Boot Remoto'
    icon = '/dl/ltsp/icon.png'
    folder = 'servers'
    
    def on_init(self):
        be = backend.Config(self.app)
        self.ifaces   = be.getInterfaces()
        self.clientes = be.getClients()
 
    def on_session_start(self):
        self._editing   = None
        self._tab       = 0
        self._msgError  = ''
        self._msgError2 = ''
        self._msgError3 = ''
        self._infoLTSP  = False
 
    def get_ui(self):
        ui  = self.app.inflate('ltsp:main')
        cli = ui.find('listaClientes')
        server = ui.find('servidorConfig')

        ui.find('tabs').set( 'active', self._tab )

        for i in self.ifaces:
            if i <> 'lo':
                ui.find('iface').append( UI.SelectOption( value=i,text=i ) )

        # Criação da lista de clientes
        for i in self.clientes:
            if i[2]: local = 'Cliente'
            else:    local = 'Servidor'

            cli.append(
                UI.DTR(
                    UI.DTH(UI.Image(file='/dl/ltsp/icon.png')),
                    UI.DTH(UI.Label(text=i[0])),
                    UI.DTH(UI.Label(text=i[1])),
                    UI.DTH(UI.Label(text=local)),
                    UI.DTH(
                            UI.TipIcon(
                                icon='/dl/core/ui/stock/edit.png',
                                id='edit/' + str(self.clientes.index(i)),
                                text='Editar Cliente'
                            ),
                            UI.TipIcon(
                                icon='/dl/core/ui/stock/delete.png',
                                id='remover/' + str(self.clientes.index(i)),
                                text='Remover Usuário'
                            )
                        )
                ))
        if len(self.clientes) == 0:
            ui.remove('listaClientes')

        # Adicionando as informações da interface
        lista = backend.Config(self.app).getServer()
        server.append(
            UI.DTR(
                UI.DTH(UI.Label(text="Interface configurada:")),
                UI.DTH(UI.Label(text="IP:")),
                UI.DTH(UI.Label(text="Máscara:")),
                UI.DTH(UI.Label(text="Broadcast:")),
                UI.DTH(UI.Label(text="Rede:")),
            ))
        server.append(
            UI.DTR(
                UI.DTH(UI.Label(text=lista[0])),
                UI.DTH(UI.Label(text=lista[1])),
                UI.DTH(UI.Label(text=lista[2])),
                UI.DTH(UI.Label(text=lista[3])),
                UI.DTH(UI.Label(text=lista[4])),
            ))

        # Exibir mensagem de erro da tab 1
        if self._msgError <> '':
            ui.find('msgError').set('text', self._msgError)
            self._msgError = ''
        
        # Exibir mensagem de erro 1 da tab 2
        elif self._msgError2 <> '':
            ui.find('msgError2Spacer').set('height', '10')
            ui.find('msgError2').set('text', self._msgError2)
            self._msgError2 = ''

        # Exibir mensagem de erro 2 da tab 2
        elif self._msgError3 <> '':
            ui.find('msgError3Spacer').set('height', '10')
            ui.find('msgError3').set('text', self._msgError3)
            self._msgError3 = ''

        #Range e Apps
        r = backend.Config(self.app).getRangeLTSP()
        if r <> []:
            ui.find('msgRange').set('text', 'Faixa de IPs de %s até %s' % (r[0], r[1]))
            ui.find('divRange').append( UI.Button( id="delFaixa", name="delFaixa",
                                                   text="Remover Faixa de IPs",
                                                   action="delFaixa" ))
        del r
        r = backend.Config(self.app).getAppsLTSP()
        if r.upper() == 'TRUE': #LocalAPPS ativo
            r = 'Execução de aplicativos configurada para ser nos clientes.'
        else:
            r = 'Execução de aplicativos configurada para ser no servidor.'
        ui.find('msgApps').set('text', r)

        # Edição de um cliente
        if self._editing is not None:
            if self._editing == 'addFaixa':
                d = self.app.inflate('ltsp:faixa')
                ui.append('main', d)
            else:
                d = self.app.inflate('ltsp:edit')
                try:
                    h = self.clientes[self._editing]
                    d.find('mac').set('value', h[0])
                    d.find('ip').set('value', h[1])
                    if h[2]:
                        d.find('selectC').set('selected', 'True')
                    else:
                        d.find('selectS').set('selected', 'True')
                except:
                    pass

                ui.append('main', d)
            self._tab = 1

        if self._infoLTSP:
            ui.remove('preInfo')
            pi = ui.find('postInfo')
            fields = backend.Config(self.app).getLTSPInfo()
            toAdd = UI.VContainer()
            for i in fields:
                toAdd.append(UI.Label(text=i, size="2"))
                toAdd.append(UI.Spacer(height="10"))
            pi.append(toAdd)
            pi.append(UI.Button(text="OK", form="info", id="ok", name="ok"))
            self._infoLTSP = False

        if backend.Config(self.app).hasImageCreated():
            ui.find('infoImageCreated').set('text', 'Info: Uma imagem foi configurada para os clientes.')
        else:
            ui.find('infoImageCreated').set('text', 'Info: Nenhuma imagem foi configurada para os clientes.')

        return ui
 
    @event('button/click')
    def on_click(self, event, params, vars = None):
        # remover o Range
        if params[0] == 'delFaixa':
            ret = backend.Config(self.app).delRangeLTSP()
            self._tab = 1

        if params[0] == 'setFaixa':
            self._editing = 'addFaixa'
            self._tab = 1
        
        if params[0] == 'edit':
            self._editing = int(params[1])
            self._tab = 1
        
        if params[0] == 'remover':
            ret = backend.Config(self.app).delMachine( self.clientes[int(params[1])][0] )
            #if not ret: ERROR
            self._tab = 1
 
    @event('dialog/submit')
    @event('form/submit')
    def on_submit(self, event, params, vars = None):
        # Edição de um cliente
        if params[0] == 'dlgEdit':
            if vars.getvalue('action', '') == 'OK':
                ip = vars.getvalue('ip', '')
                mac = vars.getvalue('mac', '')
                app = vars.getvalue('app', '')
                ret = backend.Config(self.app).saveMachine(ip, mac, app)
                #if not ret: ERROR

            self._editing = None

        # Adicionar uma faixa DHCP
        elif params[0] == 'addFaixa':
            if vars.getvalue('action', '') == 'OK':
                ip_inicial = vars.getvalue('inicial', '')
                ip_final   = vars.getvalue('final', '')
                apps       = vars.getvalue('apps')
                retRange, retAPPS = backend.Config(self.app).setRangeLTSP(ip_inicial, ip_final, apps)

                if not retRange:
                    if ip_inicial.strip() <> '' and ip_final.strip() <> '':
                        self._msgError3 = 'Erro ao adicionar a faixa de IPs. '+\
                                          'A faixa adicionada não está na mesma '+\
                                          'faixa da rede do servidor.'

            self._editing = None
            self._tab = 1

        # Configurar a interface do Servidor
        elif params[0] == "interface":
            if vars.getvalue('action','') == 'cadastrar_servidor':
                iface = vars.getvalue('iface','none')
                try:    ip = ni.ifaddresses(iface)[2][0]['addr']
                except: ip = ''
                try:    masc = ni.ifaddresses(iface)[2][0]['netmask']
                except: masc = ''
                
                ret = backend.Config(self.app).setServer(ip, masc, iface)
                if not ret:
                    self._msgError = 'A nova interface escolhida não está configurada.'+\
                                     'Vá primeiro ao módulo "Redes" e defina um IP e uma máscara.'
            self._tab = 0

        # Adicionar um cliente
        elif params[0] == "clientes":
            self._tab = 1
            if vars.getvalue('action','') == 'addCliente':
                mac = vars.getvalue('clientMAC','none')
                ip  = vars.getvalue('clientIP','none')
                app = vars.getvalue('clientAPP','none')
                ret = backend.Config(self.app).saveMachine(ip, mac, app)
                if not ret:
                    self._msgError2 = 'ERRO: ou o MAC é inválido ou o IP não está na mesma rede de seu servidor.'
                    return
        elif params[0] == "clientImage":
            if vars.getvalue('action','') == 'criar_imagem':
                mirror = vars.getvalue('mirror','')
                backend.Config(self.app).ltspBuildClient( mirror=mirror )
		self._tab = 2
        elif params[0] == 'info':
            self._tab = 3
            if vars.getvalue('action','') == 'getInfoLTSP':
                self._infoLTSP = True
