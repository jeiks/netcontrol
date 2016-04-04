# -*- coding: utf-8 -*-
from netcontrol import apis
from netcontrol.com import *
from netcontrol.ui import *


class SquidBindings(Plugin):
    implements(apis.squid.IPluginPart)

    weight = 15
    title = 'Bindings'

    tab = 0
    cfg = 0
    parent = None

    def init(self, parent, cfg, tab):
        self.parent = parent
        self.cfg = cfg
        self.tab = tab
        parent._adding_http_binding = False
        parent._adding_https_binding = False

    def get_ui(self):
        t1 = UI.DT()
        t1.append(UI.DTR(UI.DTH(UI.Label(text='Host')), UI.DTH(UI.Label(text='Porta')), UI.DTH(), header=True))
        for a in self.cfg.http_port:
            t1.append(
                UI.DTR(
                    UI.Label(text=a[0]),
                    UI.Label(text=a[1]),
                    UI.TipIcon(
                        icon='/dl/core/ui/stock/delete.png',
                        text='Excluir', id='del_http_binding/' + a[0] + '/' + a[1]
                    )
                )
              )
        t2 = UI.DT()
        t2.append(UI.DTR(UI.DTH(UI.Label(text='Host')), UI.DTH(UI.Label(text='Port')), UI.DTH(), header=True))
        for a in self.cfg.https_port:
            t2.append(
                UI.DTR(
                    UI.Label(text=a[0]),
                    UI.Label(text=a[1]),
                    UI.TipIcon(
                        icon='/dl/core/ui/stock/delete.png',
                        text='Excluir', id='del_https_binding/' + a[0] + '/' + a[1]
                    )
                )
              )

        v1 = UI.VContainer(UI.Label(text='HTTP', size=3), t1, UI.Button(text='Adicionar', id='add_http_binding'))
        v2 = UI.VContainer(UI.Label(text='HTTPS', size=3), t2, UI.Button(text='Adicionar', id='add_https_binding'))

        c = UI.HContainer(v1, UI.Spacer(width=20), v2)

        if self.parent._adding_http_binding or self.parent._adding_https_binding:
            c.append(self.get_ui_add())

        return c

    def get_ui_add(self):
        c = UI.HContainer(
                UI.LT(
                    UI.LTR(
                        UI.Label(text='Host:'),
                        UI.TextInput(name='host')
                    ),
                    UI.LTR(
                        UI.Label(text='Porta:'),
                        UI.TextInput(name='port')
                    )
                )
            )
        return UI.DialogBox(c, title='Adicionar binding', id='dlgAddBinding')

    def on_click(self, event, params, vars=None):
        if params[0] == 'add_http_binding':
            self.parent._tab = self.tab
            self.parent._adding_http_binding = True
        if params[0] == 'add_https_binding':
            self.parent._tab = self.tab
            self.parent._adding_https_binding = True
        if params[0] == 'del_http_binding':
            self.parent._tab = self.tab
            self.cfg.http_port.remove((params[1], params[2]))
            self.cfg.save()
        if params[0] == 'del_https_binding':
            self.parent._tab = self.tab
            self.cfg.https_port.remove((params[1], params[2]))
            self.cfg.save()

    def on_submit(self, event, params, vars=None):
        if params[0] == 'dlgAddBinding':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                h = vars.getvalue('host', '')
                p = vars.getvalue('port', '')
                if self.parent._adding_http_binding:
                    self.cfg.http_port.append((h, p))
                if self.parent._adding_https_binding:
                    self.cfg.https_port.append((h, p))
                self.cfg.save()
            self.parent._adding_http_binding = False
            self.parent._adding_https_binding = False
