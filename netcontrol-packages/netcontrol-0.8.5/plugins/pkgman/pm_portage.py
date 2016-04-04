"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""

import os
import subprocess
import lxml.etree

from netcontrol.com import *
from netcontrol.utils import shell, shell_bg
from netcontrol import apis


class PortagePackageManager(Plugin):
    implements(apis.pkgman.IPackageManager)
    platform = ['gentoo']
    _pending = {}

    def refresh(self, st):
        st.full = self.eix_parse(shell('eix \'-I*\' --xml'))
        st.upgradeable = self.eix_parse(shell('eix -u --xml'))
        st.pending = self._pending

    def get_lists(self):
        shell_bg('emerge --sync', output='/tmp/netcontrol-portage-output', deleteout=True)

    def search(self, q, st):
        return self.eix_parse(shell('eix --xml \'%s\''%q))

    def mark_install(self, st, name):
        st.pending[name] = 'install'

    def mark_remove(self, st, name):
        st.pending[name] = 'remove'

    def mark_cancel(self, st, name):
        del st.pending[name]

    def mark_cancel_all(self, st):
        st.pending = {}

    def apply(self, st):
        cmd = 'emerge '
        cmd2 = 'emerge --unmerge'
        for x in st.pending:
            if st.pending[x] == 'install':
                cmd += ' ' + x
            else:
                cmd2 += ' ' + x
        shell_bg('%s; %s'%(cmd,cmd2), output='/tmp/netcontrol-portage-output', deleteout=True)

    def is_busy(self):
        return os.path.exists('/tmp/netcontrol-portage-output')

    def get_busy_status(self):
        try:
            return open('/tmp/netcontrol-portage-output', 'r').read().splitlines()[-1]
        except:
            return ''

    def get_expected_result(self, st):
        return st.pending

    def abort(self):
        shell('pkill emerge')
        shell('rm /tmp/netcontrol-portage-output')

    def get_info(self, pkg):
        return self.eix_parse(shell('eix \'-I*\' --xml'))[pkg]

    def get_info_ui(self, pkg):
        return None

    def eix_parse(self, data):
        xml = lxml.etree.fromstring(data)
        r = {}

        for pkg in xml.findall('*/package'):
            try:
                p = apis.pkgman.Package()
                p.name = pkg.get('name')
                p.available = pkg.findall('version')[-1].get('id')
                if len(pkg.findall('version[@installed]')) == 0:
                    p.state = 'removed'
                else:
                    p.installed = pkg.findall('version[@installed]')[0].get('id')
                    p.version = p.installed
                p.description = pkg.find('description').text
                r[p.name] = p
                if len(r.keys()) > 250: break
            except:
                pass

        return r
