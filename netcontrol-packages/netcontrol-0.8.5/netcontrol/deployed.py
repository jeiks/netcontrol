import os
from netcontrol.utils import hashpw, shell
from ConfigParser import ConfigParser


RCFG_FILE = '/root/netcontrol-re.conf'

def reconfigure(cfg):
    if not os.path.exists(RCFG_FILE):
        return

    rcfg = ConfigParser()
    rcfg.read(RCFG_FILE)

    if rcfg.has_option('netcontrol', 'credentials'):
        u,p = rcfg.get('netcontrol', 'credentials').split(':')
        cfg.remove_option('users', 'admin')
        if not p.startswith('{SHA}'):
            p = hashpw(p)
        cfg.set('users', u, p)

    if rcfg.has_option('netcontrol', 'plugins'):
        for x in rcfg.get('netcontrol', 'plugins').split():
            shell('netcontrol-pkg get ' + x)

    if rcfg.has_option('netcontrol', 'ssl'):
        c,k = rcfg.get('netcontrol', 'ssl').split()
        cfg.set('ssl', '1')
        cfg.set('cert_key', k)
        cfg.set('cert_file', c)

    if rcfg.has_option('netcontrol', 'port'):
        cfg.set('netcontrol', 'bind_port', rcfg.get('netcontrol', 'port'))

    if rcfg.has_option('netcontrol', 'host'):
        cfg.set('netcontrol', 'bind_host', rcfg.get('netcontrol', 'host'))

    cfg.set('netcontrol', 'firstrun', 'no')
    cfg.save()
    os.unlink(RCFG_FILE)
