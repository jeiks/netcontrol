"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.api import ModuleConfig
from main import *


class GeneralConfig(ModuleConfig):
    target = LogsPlugin
    platform = ['any']
    
    labels = {
        'dir': 'Log directory'
    }
    
    dir = '/var/log'
   
