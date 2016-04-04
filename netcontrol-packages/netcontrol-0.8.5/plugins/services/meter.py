# -*- coding: utf-8 -*-
"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.api import *
from netcontrol import apis


class ServiceMeter (BinaryMeter):
    name = 'Serviços'
    category = 'Software'
    transform = 'running'

    def get_variants(self):
        return [x.name for x in self.app.get_backend(apis.services.IServiceManager).list_all()]

    def init(self):
        self.mgr = self.app.get_backend(apis.services.IServiceManager)
        self.text = self.variant

    def get_value(self):
        return self.mgr.get_status(self.variant) == 'running'
