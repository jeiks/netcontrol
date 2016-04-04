#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Written by Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> for FluxSoftwares)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of 
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *

fileSync = '/tmp/netcontrolLDAPBackend.pid'

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
    name = 'LDAP Server'
    icon = '/dl/ldap_server/icon.png'
    id = 'myhosts'
    __netcontrolControlFile = '/etc/ldap/netcontrol'

    def list_files(self):
        try:
            from os import access
            if not access(self.__netcontrolControlFile, 6):
                from os import chmod
                open(self.__netcontrolControlFile,6).write('')
                chmod(self.__netcontrolControlFile, 0600)
        except:
            pass
        return [ '/etc/ldap/netcontrol' ]

    def __getCon(self):
        from ConfigParser import ConfigParser
        cfg = ConfigParser()
        cfg.read('/etc/ldap/netcontrol')
        server   = cfg.get('base', 'server')
        bindDN   = cfg.get('base', 'bindDN')
        adminPW  = cfg.get('base', 'adminPW')
        SAM = False
        try: SAM = bool(cfg.get('base', 'SAM'))
        except: pass

        del cfg
        from netcontrolldap.netcontrolldap import LDAPConnection
        con = LDAPConnection(server, ssl=True,
                    admPasswd=adminPW, baseDN=bindDN,
                    SAM=SAM)

        return con

    def __getInfo(self):
        from ConfigParser import ConfigParser
        cfg = ConfigParser()
        cfg.read('/etc/ldap/netcontrol')
        server   = cfg.get('base', 'server')
        bindDN   = cfg.get('base', 'bindDN')
        domain   = cfg.get('base', 'workgroup')
        del cfg
        return [ server , bindDN, domain ]

    def has_ldap_installed(self):
        try:
            con = self.__getCon()
            if con.initConnection()[0] == 0:
                con.closeConnection()
                return [ True , self.__getInfo() ]
            else:
                return [ False , [] ]
        except:
            return [ False , [] ]

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
            while access('/proc/%s'%a, 1):
                sleep(1)
                a = open( fileSync ).read()
        else:
            from os import system
            system(command)
        return [ True, 'Success' ]

    def __addExtraUsers(self):
        """
        Add system Users in ldap base
        """
        con = self.__getCon()

        # adding system groups
        extraUsers = map( lambda x: x.replace('\n','').split(':'),
                                 open('/etc/passwd').readlines() )
        for i in extraUsers:
            login  = i[0]
            uid    = i[2]
            gid    = i[3]
            name   = i[4].split(',')[0]
            home   = i[5]
            lshell = i[6]
            if login == 'root': continue

            try:
                passwd = open('/etc/shadow').read().split(login+':')[1].split(':')[0]
            except:
                passwd = ''

            createHome = home.startswith('/home')
            try: print con.addUser( login, name, passwd, uidNumber=uid,
                                    gidNumber=gid, homeDirectory=home,
                                    createHome=createHome,
                                    createMailDir=createHome,
                                    loginShell=lshell,
                                    encPasswdWithSHA=False)
            except: pass
        del con

    def __addExtraGroups(self):
        """
        Add system Groups in ldap base
        """
        con = self.__getCon()

        # adding system groups
        #extraGroups = map( lambda x: x.split(':')[0:3:2],
        #                         open('/etc/group').readlines() )
        extraGroups = map( lambda x:
                           x.replace('\n','').replace(':x:',':').split(':'),
                           open('/etc/group').readlines() )

        for i in extraGroups:
            if i[2] <> '':
                try: con.addGroup( i[0], i[1], i[2].split(','))
                except: pass
            else:
                try: con.addGroup( i[0] , i[1] )
                except: pass
        del con
 
    def __nslcdConf(self):
        from ConfigParser import ConfigParser
        cfg = ConfigParser()
        cfg.read('/etc/ldap/netcontrol')
        bindDN  = cfg.get('base', 'bindDN')
        del cfg
        
        c = []
        uri  = False
        base = False
        for line in open('/etc/nslcd.conf').readlines():
            i = line.split('#')[0]
            if i.find('uri') <> -1:
                c.append('uri ldap://localhost\n')
                uri=True
            elif i.find('base ') <> -1:
                c.append( 'base %s\n' % bindDN )
                base = True
            else:
                c.append(line)
        if not  uri: c.append('uri ldap://localhost\n')
        if not base: c.append('base %s\n' % bindDN)

        arq = '/etc/nslcd.conf'
        open(arq,'w').writelines(c)

        from os import chown, chmod
        gid = open('/etc/group').read().split('nslcd')[1].split(':')[2]
        chmod(arq, 0640)
        chown(arq,0,int(gid))

        self._execute('/etc/init.d/nslcd restart', daemon=True)

    def createNewLdap(self, fields):
        TAG = '#ADDED_NETCONTROLLDAP'
        hosts = []
        for i in open('/etc/hosts').readlines():
            if i.find( TAG ) == -1:
                hosts.append(i)
        hosts.append( "127.0.0.1\t%s\t%s\n" %(fields['domain'],TAG) )
        open('/etc/hosts','w').writelines(hosts)
        from netcontrolldap.netcontrolldap import LDAPControl
        
        con = LDAPControl()
        con.cleanLDAP()
        con.setSSLAttributes( unit=fields['organization'],
                              locality=fields['locality'],
                              state=fields['state'],
                              country=fields['country'],
                              email=fields['email'],
                              expiration_days=fields['expiration_days'],
                              serial=fields['serial'] )
        ret = con.createNewBase( organization=fields['organization'],
                                 domain=fields['domain'],
                                 password=fields['password'],
                                 ssl=True, workgroup=fields['workgroup'] )
        del con

        try:    self.__nslcdConf()
        except: pass
        
        con = self.__getCon()
        con.setSAM()
        con.addUser('root', 'Administrador do Sistema', fields['password'],
                    uidNumber='0', gidNumber='0')
        con.closeConnection()
       
        try:    self.__addExtraUsers()
        except: pass
        try:    self.__addExtraGroups()
        except: pass
        
        return ret
