#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

import re
import os

from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *
from netcontrol import apis

fileSync = '/tmp/netcontrolNTOP.pid'

# Copyright: (c) 2011 phobie <pydaemon.phobie@safersignup.com>
# License: CC-by-sa 3.0
def exec_as_daemon(s_path_cmd, l_args=[]):
    import os
    i_pid = os.fork()
    if i_pid != 0:
        # Always remember to gobble your zombie children
        os.wait()
        # Back to parent
        return
    # Detach from parent
    os.setsid()
    # Do not block any mounts
    os.chdir('/')
    # Reset file creation rights
    os.umask(0)
    i_pid = os.fork()
    if i_pid != 0:
        open( fileSync, 'w' ).write(str(i_pid))
        # Close the direct child process
        os._exit(0)
    # Get the maximum count of open file handles
    try:
        import resource
        i_fd_max = resource.getrlimit(resource.RLIMIT_NOFILE)[1]
        if i_fd_max == resource.RLIM_INFINITY:
            i_fd_max = 1024
    except ImportError:
        i_fd_max = 1024
    # Try to close all possible file handles
    for i_cur_fd in range(0, i_fd_max):
        try:
            os.close(i_cur_fd)
        except OSError:
            pass
    # Assosiate STDIN with /dev/null
    os.open(os.devnull if hasattr(os, "devnull") else '/dev/null', os.O_RDWR)
    # STDOUT to /dev/null
    os.dup2(0, 1)
    # STDERR to /dev/null
    os.dup2(0, 2)
    l_args.insert(0, s_path_cmd)
    # Let the clone become a other process

    os.execv(s_path_cmd, l_args)


class Config(Plugin):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    implements(IConfigurable)
    name = 'Ntop'
    icon = '/dl/ntop/icon.png'
    id = 'ntop'

    def list_files(self):
        return None

    def _execute(self, command, daemon=False):
        '''
        @return:
            [ (Boolean) Coomand executed with sucess ,
              (tuple) (stdout,stderr)                  ]
        '''
        cmd = command.split()
        print cmd[0], '::' , cmd[1:]
        if daemon:
            from time import sleep
            open( fileSync ,'w').write('1')
            exec_as_daemon(cmd[0], cmd[1:])
            a = open( fileSync ).read()
            while os.access('/proc/%s'%a, 1):
                sleep(1)
                a = open( fileSync ).read()
        else:
            from os import system
            system(command)
        return [ True, 'Success' ]

    def ConfigureIfaces(self):
        ifaces = []
        try:
            tmp = open('/proc/net/igmp').readlines()[1:]
            for i in tmp:
                if i[0] <> '\t':
                    ifc = i.split()[1]
                    if not ifc.startswith('lo') and not ifc.startswith('vbox'):
                        ifaces.append(ifc)
        except:
            return None

        ifaces = ','.join(ifaces)
        if ifaces == ',': ifaces = ''
        try:
            arq = open('/var/lib/ntop/init.cfg','w')
            arq.write('USER="ntop"\n')
            arq.write('INTERFACES="%s"'%ifaces)
            arq.close()
        except: pass
        self._execute( '/etc/init.d/ntop restart', daemon=True )
