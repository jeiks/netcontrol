# -*- coding: utf-8 -*-
from netcontrol.api import *
from main import *


class GeneralConfig(ModuleConfig):
    target = SquidConfig
    platform = ['debian', 'centos', 'arch', 'gentoo', 'mandriva']
    
    labels = {
        'cfg_file': 'Arquivo de configuração'
    }
    
    cfg_file = '/etc/squid3/squid.conf'
   
   
class BSDConfig(GeneralConfig):
    implements((IModuleConfig, -100))
    platform = ['freebsd']
    
    cfg_file = '/usr/local/etc/squid/squid.conf'
