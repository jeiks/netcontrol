# -*- coding: utf-8 -*-
from netcontrol.ui import *
from netcontrol.api import *

import backend


class FirewallWizardPlugin(CategoryPlugin):
    text = 'Assistente Firewall'
    icon = '/dl/iptables_wizard/icon.png'
    folder = 'system'
    allCfgTabs = { 'cfgENTRADA':1,
                   'cfgSAIDA'  :2,
                   'cfgDMZ'    :4,
                   'cfgSQL'    :5,
                   'cfgEXTRAS' :6,
                   'cfgVPN'    :7,
                   'cfgOvNg'   :8 }

    def on_init(self):
        pass

    def on_session_start(self):
        self._tab = 0
        self._viewInetCfg    = None
        self._viewInetValues = None
        self._msgIfaces      = ''

    def getContainerCheck(self, iface):
        esqInt = UI.ScrollContainer()
        dirInt = UI.ScrollContainer()
        netifaces = backend.Config( self.app ).readIfaces(virtual=True, net=iface)
        if netifaces == []:
            return UI.Label(text="Nenhuma interface de rede configurada!")

        for i in netifaces:
            tmp = UI.CheckBox( text=i[0], name=i[0], id=i[0], value=i[0], checked= i[1]  )
            if i[0].find(':') == -1:
                esqInt.append( tmp )
            else:
                dirInt.append( tmp )
            del tmp

        return UI.HContainer().append(esqInt).append(dirInt)

    def getContainerRadio(self, iface):
        tmpName = 'iface%sRadio' % iface
        ret = UI.HContainer()
        for i in backend.Config( self.app ).readIfaces(net=iface):
            ret.append( UI.Radio ( text=i[0], name=tmpName  , id=tmpName , value=i[0], checked= i[1] ) )
            ret.append( UI.Spacer( width="10" ) )
        return ret

    def getTreeIfaces(self, iface):
        ret = UI.TreeContainer(text="Interfaces", id="/", expanded=True)
        for i in backend.Config( self.app ).readIfaces(virtual=True, net=iface):
            if i[1]:
                ret.append( UI.TreeContainerNode(UI.LinkLabel(text=i[0], id='viewInetCfg/'+i[0])) )
        return ret

    def get_ui(self):
        ui = self.app.inflate('iptables_wizard:main')
        ui.find( 'msgIfaces'  ).set('text', self._msgIfaces)

        ui.find( 'ifaceSAIDA' ).append( self.getContainerRadio( 'SAIDA' ) )
        ui.find(  'ifaceDMZ'  ).append( self.getContainerRadio(  'DMZ'  ) )
        ui.find(  'ifaceSQL'  ).append( self.getContainerRadio(  'SQL'  ) )
        ui.find('ifaceINTERNA').append( self.getContainerCheck('INTERNA') )
       
        ui.find('listIntNets').append( self.getTreeIfaces('INTERNA') )

        if self._viewInetCfg is not None:
            ui.find('textIfaceINTERNA').set('text', 'Interface selecionada: "%s"' % self._viewInetCfg)
            for i in self._viewInetValues.keys():
                try:
                    ui.find(i).set('checked', self._viewInetValues[i] == '1')
                    ui.find(i).set('value', self._viewInetValues[i])
                except: pass
        else:
            ui.find('textIfaceINTERNA').set('text', 'Selecione uma das interfaces abaixo:')
            ui.find('optIntNets').set('hide', True)
        ui.find('tabs').set('active', self._tab)

        for cfg in self.allCfgTabs.keys():
            for i in backend.Config( self.app ).getConfigSection(cfg).iteritems():
                try:
                    ui.find(i[0]).set('checked', i[1] == '1')
                    ui.find(i[0]).set('value', i[1])
                except: pass

        return ui

    def updateViewInet(self, iface):
        self._viewInetCfg    = iface
        self._viewInetValues = backend.Config( self.app ).getConfigSection(self._viewInetCfg)

    @event('button/click')
    @event('form/submit')
    def on_click(self, event, params, vars = None):
        if params[0].startswith('iface'):
            self._tab = 0
            if params[0] == 'ifaceSAIDA':
                backend.Config( self.app ).saveIfaces(params[0][5:], vars.getvalue('ifaceSAIDARadio','') )
            elif params[0] == 'ifaceDMZ':
                backend.Config( self.app ).saveIfaces(params[0][5:], vars.getvalue('ifaceDMZRadio','') )
            elif params[0] == 'ifaceSQL':
                backend.Config( self.app ).saveIfaces(params[0][5:], vars.getvalue('ifaceSQLRadio','') )
            elif params[0] == 'ifaceINTERNA':
                ifaceToAdd = ""
                for i in vars.list[1:]:
                    try:
                        if int(i.value) == 1: ifaceToAdd += i.name+' '
                    except: pass
                backend.Config( self.app ).saveIfaces(params[0][5:], ifaceToAdd)

        elif params[0] == 'frm_IntNets':
            values = {}
            for i in vars.list[1:]:
                values[i.name] = i.value
            backend.Config( self.app ).saveConfigSection(self._viewInetCfg, values)
            self.updateViewInet( self._viewInetCfg )

        elif params[0].startswith('frm_cfg'):
            for i in self.allCfgTabs.iteritems():
                if params[0] == 'frm_'+i[0]:
                    self._tab = i[1]
                    break

            values = {}
            for i in vars.list[1:]:
                values[i.name] = i.value
            backend.Config( self.app ).saveConfigSection(params[0][4:], values)

        elif params[0] == 'aplicarConfig':
            if not backend.Config( self.app ).applyAllSettings():
                self._msgIfaces = 'Atenção: Especifique todas as interfaces antes de aplicar as configurações'
            else:
                self._msgIfaces = ''

    @event('linklabel/click')
    def on_linklabel(self, event, params, vars = None):
        if params[0] == 'viewInetCfg':
            self._tab = 3
            self.updateViewInet( params[1] )
