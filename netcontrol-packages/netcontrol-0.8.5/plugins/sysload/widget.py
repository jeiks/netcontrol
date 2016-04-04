# -*- coding: utf-8 -*-
"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.ui import *
from netcontrol import apis
from netcontrol.com import implements, Plugin
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol import apis


class LoadWidget(Plugin):
    implements(apis.dashboard.IWidget)
    title = 'System load'
    icon = '/dl/sysload/widget.png'
    name = 'System load'
    style = 'linear'

    def get_ui(self, cfg, id=None):
        stat = self.app.get_backend(apis.sysstat.ISysStat)
        ui = self.app.inflate('sysload:load')
        load = stat.get_load()
        ui.find('1m').set('text', load[0])
        ui.find('5m').set('text', load[1])
        ui.find('15m').set('text', load[2])
        return ui

    def handle(self, event, params, cfg, vars=None):
        pass

    def get_config_dialog(self):
        return None

    def process_config(self, event, params, vars):
        pass


class RamWidget(Plugin):
    implements(apis.dashboard.IWidget)
    title = 'RAM'
    icon = '/dl/sysload/widget_mem.png'
    name = 'Memória'
    style = 'normal'

    def get_ui(self, cfg, id=None):
        stat = self.app.get_backend(apis.sysstat.ISysStat)
        ru, rt = stat.get_ram()
        return UI.HContainer(
            UI.ProgressBar(value=ru, max=rt, width=220),
            UI.Label(text=str_fsize(ru)),
        )

    def handle(self, event, params, cfg, vars=None):
        pass

    def get_config_dialog(self):
        return None

    def process_config(self, vars):
        pass


class SwapWidget(Plugin):
    implements(apis.dashboard.IWidget)
    title = 'Swap'
    icon = '/dl/sysload/widget_swap.png'
    name = 'Swap'
    style = 'normal'

    def get_ui(self, cfg, id=None):
        stat = self.app.get_backend(apis.sysstat.ISysStat)
        su, st = stat.get_swap()
        return UI.HContainer(
            UI.ProgressBar(value=su, max=int(st)+1, width=220),
            UI.Label(text=str_fsize(su)),
        )

    def handle(self, event, params, cfg, vars=None):
        pass

    def get_config_dialog(self):
        return None

    def process_config(self, vars):
        pass
