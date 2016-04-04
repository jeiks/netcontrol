#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA

"""

from netcontrolrede.netcontrolrede import Rede
from tempfile import NamedTemporaryFile

fileSync = '/tmp/netcontrolpydhcp.pid'

# Based on:
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
        #ADDED:
        open( fileSync, 'w' ).write( str(i_pid) )
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

class ExecuteProgram:
    def _execute(self, command, daemon=False):
        '''
        @return:
            [ (Boolean) Coomand executed with sucess ,
                (tuple) (stdout,stderr)                  ]
        '''
        from os import access
        cmd = command.split()
        
        #DEBUG:print cmd[0], '::' , cmd[1:]
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


class DHCP(ExecuteProgram):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    _ltsp = """
    option root-path "%ROOTPATH%";
	if substring( option vendor-class-identifier, 0, 9 ) = "PXEClient" {
		filename "%%PXEOLD%";
	} else {
		filename "%%%PXENEW%";
	}

"""
    _dhcpDefaultLines = """
authoritative;
default-lease-time 600;
max-lease-time 7200;

"""
    _rootPath = 'ROOTPATH'
    _pxeOld   = 'PXEOLD'
    _pxeNew   = 'PXENEW'
    _configDir  = '/etc/netcontrol'
    _configFile = 'netcontroldhcp.cfg'
    _dhcpdDir   = '/etc/dhcp/'
    _dhcpdFile  = 'dhcpd.conf'

    def __init__(self, rootPath='/opt/lts/i386',
                 pxeOld="/ltsp/i386/pxelinux.0",
                 pxeNew="/ltsp/i386/nbi.img"):
        self.setRootPath(rootPath)
        self.setPxeOld(pxeOld)
        self.setPxeNew(pxeNew)

        self._configFile = self._configDir+'/'+self._configFile
        self.__getConf()

    def setRootPath(self, rootPath):
        rootPath = '%'+rootPath.replace('"','').replace("'",'')
        self._ltsp = self._ltsp.replace( '%'+self._rootPath+'%', rootPath )
        self._rootPath = rootPath

    def setPxeOld(self, pxeOld):
        pxeOld = '%%'+pxeOld.replace('"','').replace("'",'')
        self._ltsp = self._ltsp.replace( '%%'+self._pxeOld+'%', pxeOld)
        pxeOld = self._pxeOld

    def setPxeNew(self, pxeNew):
        pxeNew = '%%%'+pxeNew.replace('"','').replace("'",'')
        self._ltsp = self._ltsp.replace( '%%%'+self._pxeNew+'%', pxeNew)
        pxeNew = self._pxeNew

    def getLTSPConfig(self):
        return self._ltsp.replace('%', '')

    def __getConf(self):
        from ConfigParser import ConfigParser
        cfg = ConfigParser()
        cfg.read(self._configFile)
        return cfg

    def __writeConf(self, conf):
        try:
            from os import access, mkdir
            if not access(self._configDir, 7):
                mkdir (self._configDir, 0700)
            conf.write( open(self._configFile, 'w') )
            self.__writeDHCPD()
            return True
        except: return False

    def __writeDHCPD(self):
        from os import access
        if not access(self._dhcpdDir, 7):
            return False

        lines = []
        lines.append(self._dhcpDefaultLines)
        for i in self.getSubnets():
            lines.append('subnet %s netmask %s { \n' % (i['ip'],i['masc']))
            lines.append('\toption subnet-mask %s;\n' % i['masc'])
            if i['router'] <> '':
                lines.append('\toption routers %s;\n' % i['router'])
            if i['nameserver'] <> '':
                lines.append('\toption domain-name-servers %s;\n' % i['nameserver'])
            lines.append('}\n\n')

        for i in self.getHosts():
            lines.append('host %s {\n' % i['name'])
            lines.append('\thardware ethernet %s;\n' % i['mac'])
            lines.append('\tfixed-address %s;\n' % i['ip'])
            if bool(i['ltsp']):
                lines.append(self.getLTSPConfig())
            lines.append('}\n\n')

        open( self._dhcpdDir + '/' + self._dhcpdFile, 'w' ).writelines(lines)

    def restartDHCP(self):
        qntLines = len(open('/var/log/syslog').readlines())
        self._execute("/etc/init.d/isc-dhcp-server restart", daemon=True )
        syslog = open('/var/log/syslog').readlines()
        ret = []
        for i in syslog[qntLines:]:
            if i.find('dhcpd:') <> -1:
                ret.append(i.split('dhcpd:')[1])
        return ret

    def getSubnets(self):
        cfg = self.__getConf()
        subnets = []
        for i in cfg.sections():
            if i.find('.') <> -1:
                d = {}
                d['ip'] = i.split('/')[0]
                for f in cfg.items(i):
                    item  = f[0]
                    value = f[1]
                    d[item] = value
                subnets.append( d )
                del d
        del cfg
        return subnets

    def getHosts(self):
        cfg = self.__getConf()
        hosts = []
        for i in cfg.sections():
            if i.find(':') <> -1:
                d = {}
                d['mac'] = i
                for f in cfg.items(i):
                    item  = f[0]
                    value = f[1]
                    d[item] = value
                hosts.append( d )
                del d
        del cfg
        return hosts

    def addSubnet(self, ip, masc, router=None, nameServer=None):
        tmp = None
        try:
            tmp = Rede(ip, masc)
        except:
            return '',''

        rede    = tmp.getRedeAsStr()
        masc    = tmp.getMascAsStr()
        section = rede+'/'+masc
        del tmp

        cfg = self.__getConf()
        if not cfg.has_section( section ):
            cfg.add_section( section )
        cfg.set( section, 'masc', masc )

        if router is None: router = ''
        cfg.set(section, 'router', router)
        if nameServer is None: nameServer = ''
        cfg.set(section, 'nameserver', nameServer)

        self.__writeConf(cfg)
        del cfg

        return section, rede

    def delSubnet(self, net, masc):
        from netcontrolrede.netcontrolrede import MascRede, Rede
        masc  = MascRede(masc).getMascAsStr()
        try:
            cfg = self.__getConf()
            for i in self.getHosts():
                tnet  = Rede(i['ip'],i['masc']).getRedeAsStr()
                tmasc = Rede(i['ip'],i['masc']).getMascAsStr()
                if tnet == net and str(tmasc) == str(masc):
                    cfg.remove_section( i['mac'] )

            cfg.remove_section( '%s/%s' % (net,masc) )
            self.__writeConf(cfg)
            del cfg
            return True
        except:
            return False

    def testValidName(self, name):
        for i in self.getHosts():
            if name == i['name']:
                return False
        return True

    def addHost(self, name, mac, ip, masc, ltsp=False):
        from netcontrolrede.netcontrolrede import MascRede, Rede
        rede = Rede(ip, masc).getRedeAsStr()

        cfg = self.__getConf()
        if not cfg.has_section(rede):
            del cfg
            cfg = None
            self.addSubnet(rede, masc)

        if cfg == None:
            cfg = self.__getConf()

        nameCorrect = False
        newName = name <> '' and name or 'host'
        if not cfg.has_section(mac):
            cfg.add_section(mac)
        else:
            if cfg.get(mac, 'name') == newName:
                nameCorrect = True
            else:
                nameCorrect = False

        cfg.set(mac, 'ip', ip)
        cfg.set(mac, 'masc', MascRede(masc).getMascAsStr())
        cfg.set(mac, 'ltsp', ltsp)

        if not nameCorrect:
            #tratando o nome
            name    = name.replace(' ','_')
            newName = name
            cont = 1
            while not self.testValidName(newName):
                newName = "%s_%d" % (name,cont)
                cont   += 1

        cfg.set(mac, 'name', newName)

        self.__writeConf(cfg)
        del cfg

    def delHost(self, mac=None, name=None, ip=None):
        if mac is not None:
            try:
                cfg = self.__getConf()
                cfg.remove_section(mac)
                self.__writeConf(cfg)
                return True
            except: pass
        if name is not None:
            for i in self.getHosts():
                if i['name'] == name:
                    self.__getConf().remove_section(i['mac'])
                    return True
        if ip is not None:
            for i in self.getHosts():
                if i['ip'] == ip:
                    self.__getConf().remove_section(i['mac'])
                    return True

        return False
