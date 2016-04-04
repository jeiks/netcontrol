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
from netcontrol import apis
from netcontrol.utils import *
from netcontrolrede.netcontrolrede import MascRede

import backend

class DHCPPlugin(CategoryPlugin):
    """
    @authr: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    text = 'DHCP'
    icon = '/dl/dhcp/icon.png'
    folder = 'servers'

    def on_init(self):
        be = backend.Config(self.app)
        self._allSubnets = be.getSubnets()

    def on_session_start(self):
        self._editHost = None
        self._editSubnet = None
        self._tree = TreeManager()
        self._net = None
        self._info = None
        self._dlgMessage = None
        self._mainMessage = None

    def get_ui(self):
        ui = self.app.inflate('dhcp:main')
        ui.append('tree', self.get_ui_tree())

        #para adicionar os hosts
        if self._net <> None:
            hosts = backend.Config(self.app).getHostsFrom(self._net)
            if hosts == []:
                ui.append('data', UI.Label(text="Sem máquinas adicionadas"))
    
            div = UI.DIV(style='text-valign: center')
            for i in hosts:
                ip   = i['ip']
                masc = MascRede(i['masc']).getMascAsInt()
                mac  = i['mac']
                name = i['name']
                lenName = len(i['name'])
                ltsp = i['ltsp'] == 'True' and '1' or '0'
                div.append(
                        UI.Button(id="editHost/%s/%s/%s/%s/%s" % (name, mac, ip, masc, ltsp),
                                    text="Nome: %s\n%s/%s" % (name, ip, masc),
                                    action="editHost", design="fixedsize",
                                    icon=ltsp=='1' and '/dl/dhcp/terminal.png' or '/dl/dhcp/desktop.png'
                                    ))
            ui.append('data', div)

        if self._mainMessage is not None:
            ui.find('mainMessage').set('text', self._mainMessage)
            self._mainMessage = None

        # SUBNET______________________
        if   self._editSubnet is None:
            ui.remove('dlgSubnet')
        elif self._editSubnet <> []:
            ui.find('ip').set('value', self._editSubnet[0])
            ui.find('mascara').set('value', MascRede(self._editSubnet[1]).getMascAsStr())
            ui.find('roteador').set('value', self._editSubnet[2])
            ui.find('dns').set('value', self._editSubnet[3])
            if self._dlgMessage <> None:
                ui.find('dlgMessageSubnet').set('text', self._dlgMessage)
                self._dlgMessage = None
        # END:SUBNET___________________

        # HOST____________________
        if self._editHost is None:
            ui.remove('dlgHost')
        elif self._editHost <> []:
            ui.find('name').set('value', self._editHost[0])
            ui.find('mac').set('value', self._editHost[1])
            ui.find('ip').set('value', self._editHost[2])
            ui.find('masc').set('value', MascRede(self._editHost[3]).getMascAsStr())
            if len(self._editHost) == 5:
                ui.find( self._editHost[4] ).set('selected', 'True')
            else:
                ui.remove('delHostOption')
            if self._dlgMessage <> None:
                ui.find('dlgMessageHost').set('text', self._dlgMessage)
                self._dlgMessage = None
        else:
            ui.remove('delHostOption')
            try:
                if self._net is not None:
                    from netcontrolrede.netcontrolrede import Rede
                    tnet = self._net.split('/')
                    tmp = Rede( tnet[0], tnet[1] )
                    ui.find('ip').set('value', tmp.getRedeAsStr())
                    ui.find('masc').set('value', tmp.getMascAsStr())
                    ui.find('formLineIP1').set('text', 'Obs.: O IP e a máscara foram preenchidos com')
                    ui.find('formLineIP2').set('text', 'a rede escolhida, modifique-o para o novo IP')
                    del tnet
                    del tmp
            except:
                pass
        # END:HOST________________

        if self._info is None:
            ui.remove('dlgInfo')
        else:
            from psutil import get_process_list
            isExecuting = False
            for i in get_process_list():
                if i.name == 'dhcpd':
                    isExecuting = True
            if isExecuting:
                ui.find('dlgInfoTitle').set('style','text-align: center')
                ui.find('dlgInfo1').set('text', 'Processo reiniciado com sucesso!')
                ui.remove('dlgInfo2')
                ui.remove('divError')
            else:
                vc = UI.VContainer()
                s = False
                for i in self._info:
                    if i.find('**') <> -1:
                        s = not s
                        continue
                    if not s:
                        vc.append(UI.Label(text=i))
                ui.find('divError').append(vc)
            self._info = None

        return ui

    def get_ui_tree(self):
        # para adicionar as redes
        root = UI.TreeContainer(text='Redes disponíveis', id='/')

        for i in self._allSubnets:
            rede = '%s/%s' % (i['ip'],
                                     MascRede(i['masc']).getMascAsInt())
            editValue = '%s/%s/%s' % (rede,
                        i.has_key('router') and i['router'] or '',
                        i.has_key('nameserver') and i['nameserver'] or '')
            hh = UI.HContainer()
            ll = UI.LinkLabel( text=rede, id='view/%s'%rede )
            b1 = UI.TipIcon( icon="/dl/core/ui/stock/edit.png",
                             id="editSubnet/%s" % editValue,
                             action="editSubnet/%s" % rede)
            b2 = UI.TipIcon( icon="/dl/core/ui/stock/delete.png",
                             id="delSubnet/%s" % editValue,
                             warning="Remover a rede %s." % rede,
                             action="delSubnet/%s" % rede)
            hh.append(b1).append(b2).append(ll)
            tc = UI.TreeContainerNode( hh, active=False)
            root.append(tc)

        self._tree.apply(root)
        root['expanded'] = True
        return root

    @event('dialog/submit')
    def on_submit(self, event, params, vars = None):
        def isInt(f):
            for i in f.split('.'):
                try: int(i)
                except: return False
            return True

        if   params[0] == 'dlgSubnet':
            if vars.getvalue('action', '') == 'OK':
                ip       = vars.getvalue('ip','')
                mascara  = vars.getvalue('mascara','')
                roteador = vars.getvalue('roteador','')
                dns      = vars.getvalue('dns','')

                newSubnet = self._editSubnet == None
                self._editSubnet = []
                self._editSubnet.append(ip)
                self._editSubnet.append(mascara)
                self._editSubnet.append(roteador)
                self._editSubnet.append(dns)
                if len(ip.split('.')) <> 4 or \
                   not isInt(ip):
                    self._dlgMessage = 'ERRO: IP inválido!'
                    return

                if (len(mascara.split('.')) <> 4 and \
                    len(mascara.split('.')) <> 1) or \
                   not isInt(mascara):
                    self._dlgMessage = 'ERRO: Máscara inválida!'
                    return

                if roteador <> '' and \
                   (len(roteador.split('.')) <> 4 or not isInt(roteador)):
                    self._dlgMessage = 'ERRO: IP do roteador inválido!'
                    return

                if dns <> '' and \
                   (len(dns.split('.')) <> 4 or not isInt(dns)):
                    self._dlgMessage = 'ERRO: IP do DNS inválido!'
                    return

                ret = backend.Config(self.app).addSubnet(ip,
                                     mascara, roteador, dns)
                if ret[0] == '':
                    self._dlgMessage = 'ERRO: Informações erradas'
                    return

                self._mainMessage = ( 'Foi %s a rede %s com a máscara %s' % (
                                       newSubnet and 'adicionada' or 'modificada',
                                       ret[0], ret[1]) )

            self._editSubnet = None

        elif params[0] == 'dlgHost':
            if vars.getvalue('action', '') == 'OK':
                name = vars.getvalue('name', '')
                mac  = vars.getvalue('mac', '')
                ip   = vars.getvalue('ip', '')
                masc = vars.getvalue('masc', '')
                ltsp = vars.getvalue('ltsp', '') == '1'
                self._editHost = []
                self._editHost.append(name)
                self._editHost.append(mac)
                self._editHost.append(ip)
                self._editHost.append(masc)
                if name == '' or \
                   mac  == '' or \
                   ip   == '' or \
                   masc == '' or \
                   ltsp == '':
                    self._dlgMessage = 'ERRO: Complete todos os campos para continuar!'
                    return

                if len(mac.split(':')) <> 6 or \
                   mac == '00:00:00:00:00:00':
                    self._dlgMessage = 'ERRO: O MAC está incorreto!'
                    return

                if len(ip.split('.')) <> 4 or \
                   not isInt(ip):
                    self._dlgMessage = 'ERRO: IP inválido!'
                    return

                if (  len(masc.split('.')) <> 4   and \
                      len(masc.split('.')) <> 1 )  or \
                   not isInt(masc):
                    self._dlgMessage = 'ERRO: Máscara inválida!'
                    return

                if vars.getvalue('delHost', '') == '1':
                    backend.Config(self.app).delHost(mac)
                else:
                    backend.Config(self.app).addHost(name, mac, ip, masc, ltsp)

            self._editHost   = None

    @event('button/click')
    @event('linklabel/click')
    @event('treecontainer/click')
    def on_click(self, event, params, vars = None):
        if   params[0] == 'view':
            self._net = params[1]+'/'+params[2]
        elif params[0] == 'addSubnet':
            self._editSubnet = []
        elif params[0] == 'editSubnet':
            self._editSubnet = [ params[1], params[2], params[3], params[4] ]
        elif params[0] == 'addHost':
            self._editHost   = []
        elif params[0] == 'editHost':
            self._editHost = [ params[1], params[2], params[3], params[4], 'ltsp'+params[5] ]
        elif params[0] == 'delSubnet':
            backend.Config(self.app).delSubnet(params[1], params[2])
        elif params[0] == 'restartDHCP':
            self._info = backend.Config(self.app).restartDHCP()

