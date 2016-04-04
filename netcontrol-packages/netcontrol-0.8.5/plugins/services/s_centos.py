"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
import os

from netcontrol.com import *
from netcontrol.utils import *
from netcontrol import apis


class CentOSServiceManager(Plugin):
    implements(apis.services.IServiceManager)
    platform = ['centos', 'fedora', 'mandriva']

    def list_all(self):
        r = []
        for s in shell('chkconfig --list').split('\n'):
            s = ' '.join(s.split()[:-7])
            svc = apis.services.Service()
            svc.name = s
            svc.status = 'running' if 'running' in shell('/etc/init.d/%s status'%s) else 'stopped'
            if s != '':
                r.append(svc)

        return sorted(r, key=lambda s: s.name)

    def get_status(self, name):
        s = shell('service ' + name + ' status')
        return 'running' if 'running' in s else 'stopped'

    def start(self, name):
        shell('service ' + name + ' start')

    def stop(self, name):
        shell('service ' + name + ' stop')

    def restart(self, name):
        shell('service ' + name + ' --full-restart')
