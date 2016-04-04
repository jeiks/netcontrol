"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.ui import *
from netcontrol import apis
from netcontrol.com import implements, Plugin

from main import *


class PowerWidget(Plugin):
    implements(apis.dashboard.IWidget)
    title = 'Uptime'
    icon = '/dl/power/widget.png'
    name = 'Uptime'
    style = 'linear'
    
    def get_ui(self, cfg, id=None):
        return UI.Label(text=get_uptime())
        
    def handle(self, event, params, cfg, vars=None):
        pass
    
    def get_config_dialog(self):
        return None
        
    def process_config(self, vars):
        pass
                
