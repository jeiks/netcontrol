"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.api import ModuleConfig
from main import *


class GeneralConfig(ModuleConfig):
    target = NotepadPlugin
    platform = ['any']
    
    labels = {
        'dir': 'Initial directory'
    }
    
    dir = '/etc'
   
