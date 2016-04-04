"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.api import *
from netcontrol.com import *


class DebianNetworkCfg(Plugin):
    implements(IConfigurable)
    name = 'Network'
    id = 'network'
    platform = ['Debian', 'Ubuntu']
    
    def list_files(self):
        dir = '/etc/network/'
        return [dir+'*', dir+'*/*', dir+'*/*/*']
    
