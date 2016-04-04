"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.com import *


class IDNSConfig(Interface):
    nameservers = None

    def save(self):
        pass


class Nameserver:
    cls = ''
    address = ''
