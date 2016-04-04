"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
import os

from netcontrol.com import *
from netcontrol.utils import *
from netcontrol import apis


class BSDServiceManager(Plugin):
    implements(apis.services.IServiceManager)
    platform = ['freebsd']

    def list_all(self):
        r = []
        for s in os.listdir('/etc/rc.d') + os.listdir('/usr/local/etc/rc.d'):
            svc = apis.services.Service()
            svc.name = s
            svc.status = 'running' \
                    if os.path.exists('/var/run/%s.pid'%s)\
                    else 'stopped'
            r.append(svc)

        return sorted(r, key=lambda s: s.name)

    def get_path(self, name):
        if os.path.exists('/etc/rc.d/' + name):
            return '/etc/rc.d/' + name
        return '/usr/local/etc/rc.d/' + name

    def get_status(self, s):
        return 'running' if os.path.exists('/var/run/%s.pid'%s) else 'stopped'

    def start(self, name):
        shell(self.get_path(name) + ' start')

    def stop(self, name):
        shell(self.get_path(name) + ' stop')

    def restart(self, name):
        shell(self.get_path(name) + ' restart')
