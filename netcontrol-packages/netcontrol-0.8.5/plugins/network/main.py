# -*- coding: utf-8 -*-
"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.ui import *
from netcontrol.api import CategoryPlugin, event

from api import *
import os
from ConfigFaces import ConfigRedes




class NetworkPlugin(CategoryPlugin):
    text = 'Rede'
    icon = '/dl/network/icon.png'
    folder = 'hardware'

    def on_init(self):
      self._configIfaces = ConfigRedes()
      self._configIfaces.setAllRedes()
      self.net_config = self.app.get_backend(INetworkConfig)

    def on_session_start(self):
        self._editing_iface = ""
        self._info = None
        self._refresh = False
        self._addFace = False
        self._erroFace = " "
        self._refresh = False

    def get_ui(self):
        ui = self.app.inflate('network:main')
        ti = ui.find('list')
        
        if self._addFace:
          a = self.app.inflate('network:addFace')
          t = a.find('iface')
          if self._erroFace <> " ":
            a.find('erro').set('text',self._erroFace)
          for i in self._configIfaces.getRedesProc():
            t.append(
              UI.SelectOption( value=i, text=i )
            )
          self._erroFace = " "
          self._addFace = False
          return a

        for x in self.net_config.interfaces:
            i = self.net_config.interfaces[x]
            if i.name <> 'lo':
              ti.append(UI.DTR(
                              UI.Label(text=i.name),
                              UI.Label(text=i.devclass),
                              UI.Label(text=self.net_config.get_ip(i)),
                              UI.HContainer(
                                  UI.Image(file='/dl/network/%s.png'%('up' if i.up else 'down')),
                                  UI.Label(text=('Up' if i.up else 'Down')),
                              ),
                              UI.HContainer(
                                  UI.TipIcon(icon='/dl/core/ui/stock/info.png',
                                      text='Info', id='info/' + i.name),
                                  UI.TipIcon(icon='/dl/core/ui/stock/edit.png',
                                      text='Edit', id='editiface/' + i.name),
                                  UI.TipIcon(icon=('/dl/core/ui/stock/%s.png'%('service-run' if not i.up else 'service-stop')) if i.devclass <> "Interface Virtual" else '/dl/core/ui/stock/delete.png', 
                                      text=('Down' if i.up else 'Up') if i.devclass <>  "Interface Virtual" else "Remover Interface Virtual", 
                                      id=('if' + ('down' if i.up else 'up') + '/' + i.name) if i.devclass <> "Interface Virtual" else ("excluir/%s"%i.name),
                                      warning='Bring %s interface %s' % (('Down' if i.up else 'Up'), i.name)),
                              )
                            )
             )
                

        c = ui.find('main')
        if self._info is not None:
            c.append(
                UI.DialogBox(
                    self.net_config.get_info(self.net_config.interfaces[self._info]),
                    id='dlgInfo', 
                    hidecancel=True
                ))
        
        if self._editing_iface != "":
            cnt = UI.TabControl(active=0)
            for x in self.net_config.interfaces[self._editing_iface].bits:
                cnt.add(x.title, x.get_ui())
            dlg = UI.DialogBox(
                        cnt,
                        id="dlgEditIface"
                    )
            c.append(dlg)

        return ui
    @event('form/submit')
    @event('button/click')
    @event('linklabel/click')
    def on_ll_click(self, event, params, vars=None):
        if params[0] == 'info':
            self._info = params[1]
        if params[0] == 'editiface':
            self._editing_iface = params[1]
            self._refresh = True
        if params[0] == 'ifup':
            self.net_config.up(self.net_config.interfaces[params[1]])
            self._refresh = True
            self._refresh = True
        if params[0] == 'ifdown':
            self.net_config.down(self.net_config.interfaces[params[1]])
            self._refresh = True
        if params[0] == 'addFace':
          self._addFace = True
          self._refresh = True
        if params[0] == 'excluir':
          self._configIfaces.delIFaceVirtual(params[1])
          self._refresh = True
        if params[0] == 'reiniciarRede':
          self._configIfaces._execute('/etc/init.d/networking restart',True)
 
    @event('dialog/submit')
    def on_dlg_submit(self, event, params, vars=None):
        if params[0] == 'dlgEditIface':
            if vars.getvalue('action', '') == 'OK':
                i = self.net_config.interfaces[self._editing_iface]
                for x in i.bits:
                    x.apply(vars)
                self.net_config.save()
                self._configIfaces.ifconfig_refresh_iface( self._editing_iface, vars.getvalue('address'), vars.getvalue('netmask'), vars.getvalue('gateway') )
            self._editing_iface = ''
        if params[0] == 'dlgInfo':
            self._info = None
            
        if params[0] == 'addIFace':
            if vars.getvalue('action', '') == 'OK':
              if not vars.getvalue('ip'):
                self._addFace = True
                self._erroFace = "Todos os campos são obrigatórios !"
              else:
                self._configIfaces.addIfaceVirtual( str(vars.getvalue('iface')), str( vars.getvalue('ip') ), str( vars.getvalue('masc') ) )
