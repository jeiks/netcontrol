# -*- coding: utf-8 -*-
"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.ui import *
from netcontrol.api import *

import backend


class HostsPlugin(CategoryPlugin):
    text = 'Nomes locais'
    icon = '/dl/hosts/icon.png'
    folder = 'system'

    def on_init(self):
        be = backend.Config(self.app)
        self.hosts = be.read()
        self.hostname = be.gethostname()

    def on_session_start(self):
        self._editing = None
        self._editing_self = False

    def get_ui(self):
        ui = self.app.inflate('hosts:main')
        t = ui.find('list')

        for h in self.hosts:
            t.append(UI.DTR(
                UI.Label(text=h.ip),
                UI.Label(text=h.name),
                UI.Label(text=h.aliases),
                UI.HContainer(
                    UI.TipIcon(
                        icon='/dl/core/ui/stock/edit.png',
                        id='edit/' + str(self.hosts.index(h)),
                        text='Edit'
                    ),
                    UI.TipIcon(
                        icon='/dl/core/ui/stock/delete.png',
                        id='del/' + str(self.hosts.index(h)),
                        text='Delete',
                        warning='Remove %s from hosts'%h.ip
                    )
                ),
            ))

        if self._editing is not None:
            try:
                h = self.hosts[self._editing]
            except:
                h = backend.Host()
            ui.find('ip').set('value', h.ip)
            ui.find('name').set('value', h.name)
            ui.find('aliases').set('value', h.aliases)
        else:
            ui.remove('dlgEdit')

        if self._editing_self:
            ui.find('dlgSelf').set('value', self.hostname)
        else:
            ui.remove('dlgSelf')

        return ui

    @event('button/click')
    def on_click(self, event, params, vars = None):
        if params[0] == 'add':
            self._editing = len(self.hosts)
        if params[0] == 'edit':
            self._editing = int(params[1])
        if params[0] == 'del':
            self.hosts.pop(int(params[1]))
            backend.Config(self.app).save(self.hosts)
        if params[0] == 'hostname':
            self._editing_self = True

    @event('dialog/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'dlgEdit':
            v = vars.getvalue('value', '')
            if vars.getvalue('action', '') == 'OK':
                h = backend.Host()
                h.ip = vars.getvalue('ip', 'none')
                h.name = vars.getvalue('name', 'none')
                h.aliases = vars.getvalue('aliases', '')
                try:
                    self.hosts[self._editing] = h
                except:
                    self.hosts.append(h)
                backend.Config(self.app).save(self.hosts)
            self._editing = None
        if params[0] == 'dlgSelf':
            v = vars.getvalue('value', '')
            if vars.getvalue('action', '') == 'OK':
                backend.Config(self.app).sethostname(v)
            self._editing_self = None
