"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.ui import *
from netcontrol.com import implements, Plugin
from netcontrol.api import *
from netcontrol import apis


class MuninWidget(Plugin):
    implements(apis.dashboard.IWidget)
    title = 'Munin'
    icon = '/dl/munin/icon.png'
    name = 'Munin graph'
    style = 'normal'
    hidden = True

    def get_ui(self, cfg, id=None):
        self.title = cfg['title']
        return UI.Image(file=cfg['url'], width=330)

    def handle(self, event, params, cfg, vars=None):
        pass

    def get_config_dialog(self):
        return None

    def process_config(self, event, params, vars):
        pass
