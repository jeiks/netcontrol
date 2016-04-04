"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.com import *
from netcontrol.apis import API
from netcontrol import apis
from netcontrol.utils import shell_status, shell


class RCConf(API):
    class RCConf(Plugin):
        multi_instance = True
        file = '/etc/rc.conf'

        def has_param(self, param):
            return shell_status('grep \'%s=\' %s'%(param,self.file)) == 0

        def get_param(self, param):
            s = shell('grep \'^%s=\' %s'%(param,self.file)).split('=')[1].strip()
            return s.strip('"')

        def set_param(self, param, value, near=None):
            d = open(self.file).read().split('\n')
            f = open(self.file, 'w')
            done = False
            for s in d:
                if (not done) and (near is not None) and s.startswith(near):
                    if param is not None:
                        f.write('%s="%s"\n'%(param,value))
                    done = True
                if not (('=' in s) and s.split('=')[0].strip() == param):
                    f.write(s + '\n')
            if not done and param is not None:
                f.write('%s="%s"\n'%(param,value))
            f.close()
