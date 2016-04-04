#!/usr/bin/env python
# NetControl is based on Ajenti:
# Original Author: Eugeny Pankov <e@ajenti.org>
# Original Site: http://ajenti.org/

from distutils.core import setup
from setuptools import find_packages

setup(
    name='netcontrol',
    version='0.8.5',
    description='Server administration panel',
    author='Jacson RC Silva',
    author_email='jacsonrcsilva@gmail.com',
    url='http://www.fluxsoftwares.com',
    packages = ['netcontrol'],
    scripts=['bin/netcontrol-panel', 'bin/netcontrol-pkg'],
    data_files=[
        ('/etc/netcontrol', ['packaging/files/netcontrol.conf']),
        ('/etc/netcontrol/users', ['packaging/files/admin.conf']),
        ('/etc/init.d', ['packaging/files/netcontrol']),
        ('/var/lib/netcontrol/plugins', ['packaging/files/.placeholder']),
    ],
)
