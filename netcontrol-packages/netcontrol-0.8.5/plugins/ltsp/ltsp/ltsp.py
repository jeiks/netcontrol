#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from ConfigParser import ConfigParser as Cfg

class LTSP:
    __dirTargetDHCP  = '/etc/ltsp/'
    __fileTargetDHCP = '/etc/ltsp/dhcpd.conf'
    __dirTargetLTSP  = '/srv/tftp/ltsp/images/'
    __fileTargetLTSP = '/srv/tftp/ltsp/images/lts.conf'
    __configLTSPFile = '/var/lib/netcontrol/plugins/ltsp/ltsp.conf'

    __pxe = """\
\tif substring( option vendor-class-identifier, 0, 9 ) = "PXEClient" {
\t\tfilename "/ltsp/i386/pxelinux.0";
\t} else {
\t\tfilename "/ltsp/i386/nbi.img";
\t}
"""

    def __init__( self, fileTargetDHCP=None, fileTargetLTSP=None, configLTSPFile=None ):
        if fileTargetDHCP is not None:
            self.__fileTargetDHCP = fileTargetDHCP
        if fileTargetLTSP is not None:
            self.__fileTargetLTSP = fileTargetLTSP
        if configLTSPFile is not None:
            self.__configLTSPFile = configLTSPFile
        
        self.createDirForConfigFiles()
        self.testConfigFiles()

    def testConfigFiles(self):
        from os import access
        ret = True
        for i in [ self.__fileTargetDHCP, self.__fileTargetLTSP,
                   self.__configLTSPFile ]:
            if not access(i, 6):
                try:    open(i,'w').write('')
                except: ret = False
        return ret

    def createDirForConfigFiles(self):
        from os import makedirs, access, chown
        for i in [self.__dirTargetDHCP, self.__dirTargetLTSP]:
            if not access(i, 7):
                try:    makedirs(i, 0711)
                except: pass
                try:    chown(i, 0711)
                except: pass

    def getServer(self):
        c = Cfg()
        c.read(self.__configLTSPFile)
        ret = []
        try:
            ret.append(c.get('server', 'iface'))     #0
            ret.append(c.get('server', 'ip'))        #1
            ret.append(c.get('server', 'masc'))      #2
            ret.append(c.get('server', 'broadcast')) #3
            ret.append(c.get('server', 'rede'))      #4
        except:
            ret = ['','','','','']
        del c

        return ret

    def getMachines(self):
        c = Cfg()
        c.read(self.__configLTSPFile)
        ret = []
        for section in c.sections():
            if section <> 'server':
                try:
                    local = c.get(section, 'localapps')
                    if local.upper().startswith('Y') or \
                       local.upper().startswith('S') or \
                       local.upper().startswith('T'):
                           local = True
                    else:
                           local = False
                except: local = False
                try:
                    ret.append( [section, c.get(section, 'ip'), local] )
                except: pass
        del c
        return ret

    def setMachine(self, mac, ip=None, localApps=False):
        mac = mac.strip()
        if len(mac.split(':')) <> 6:
            return False

        try:    ip = ip.strip()
        except: pass
        
        c = Cfg()
        c.read(self.__configLTSPFile)
        if not c.has_section(mac):
            c.add_section(mac)

        if ip is not None:
            from rede import Rede
            s = self.getServer()
            masc = s[2]
            rede = s[4]
            del s
            
            try:
                i = Rede(ip, masc)
            except:
                return False

            if i.getRedeAsStr() <> rede:
                return False
            
            c.set(mac, 'ip', ip)
        c.set(mac, 'localapps', str(localApps))
        c.write( open(self.__configLTSPFile,'w') )
        del c
        
        return True

    def delMachine(self, mac):
        c = Cfg()
        c.read( self.__configLTSPFile )
        try:
            c.remove_section(mac)
            c.write( open(self.__configLTSPFile,'w') )
            return True
        except:
            return False

    def getGlobalConfig(self, key):
        section = 'globalConfig'
        c = Cfg()
        c.read(self.__configLTSPFile)
        ret = ''
        if c.has_section( section ):
            ret = c.get(section, key)
        del c

        return ret

    def setGlobalConfig(self, key, value):
        section = 'globalConfig'
        c = Cfg()
        c.read(self.__configLTSPFile)
        if not c.has_section( section ):
            c.add_section( section )
        c.set(section, key, value)
        c.write( open(self.__configLTSPFile,'w')  )
        del c
        
        return True

    def delRange(self):
        section = 'range'
        c = Cfg()
        c.read(self.__configLTSPFile)
        ret = False
        if c.has_section( section ):
            c.remove_section( section )
            c.write( open(self.__configLTSPFile,'w')  )
            ret = True
        del c
        
        return ret

    def setRange(self, start, end):
        s = self.getServer()
        masc = s[2]
        rede = s[4]
        
        from rede import Rede
        try:
            i = Rede(start, masc)
            f = Rede(end  , masc)
        except:
            return False

        if i.getRedeAsStr() <> rede:
            return False
        if f.getRedeAsStr() <> rede:
            return False
        
        section = 'range'
        c = Cfg()
        c.read(self.__configLTSPFile)
        if not c.has_section( section ):
            c.add_section( section )
        
        c.set(section, 'start', start)
        c.set(section, 'end'  , end)
        c.write( open(self.__configLTSPFile,'w')  )
        del c

        return True

    def getRange(self):
        section = 'range'
        c = Cfg()
        c.read(self.__configLTSPFile)

        ret = []
        if c.has_section( section ):
            ret.append( c.get(section, 'start') )
            ret.append( c.get(section, 'end') )

        del c

        return ret

    def setServer(self, ip, masc, iface):
        from rede import Rede
        r = Rede(ip, masc)
        M = self.getMachines()
        
        c = Cfg()
        if not c.has_section('server'):
            c.add_section('server')
        c.set('server', 'ip', r.getIPAsStr())
        c.set('server', 'masc', r.getMascAsStr())
        c.set('server', 'rede', r.getRedeAsStr())
        c.set('server', 'broadcast', r.getBroadcastAsStr())
        c.set('server', 'iface', iface)
        c.write( open(self.__configLTSPFile,'w')  )
        
        for i in M:
            self.setMachine(i[0], i[1], i[2])

        del c

    def writeDHCP(self):
        contents = []
        p = {}
        c = Cfg()
        c.read(self.__configLTSPFile)
        for i in ['ip','masc','rede', 'broadcast']:
            try:
                p[i] = c.get('server', i)
            except:
                pass
        # testando se existe server
        if not p.has_key('ip'):
            return False

        del c

        contents.append('# Auto-Configured by NetControl\n')
        contents.append("# Don't modify these lines, please use NetControl for this\n\n\n")
        contents.append('authoritative;\n\n')
        contents.append('subnet %s netmask %s {\n' % (p['rede'],p['masc']))
        r = self.getRange()
        if r <> []:
            contents.append('\trange %s %s;\n' % (r[0], r[1]))
        #contents.append('\toption domain-name %s;\n' % p['ip'])
        contents.append('\toption domain-name-servers %s;\n' % p['ip'])
        contents.append('\toption broadcast-address %s;\n' % p['broadcast'])
        contents.append('\toption routers %s;\n' % p['ip'])
        contents.append('\toption subnet-mask %s;\n' % p['masc'])
        contents.append('\toption root-path "/opt/ltsp/i386";\n')
        contents.append(self.__pxe)
        contents.append('\n')
        ident = 1
        for i in self.getMachines():
            contents.append('\thost client%d {\n' % ident)
            contents.append('\thardware ethernet %s;\n' % i[0])
            contents.append('\tfixed-address %s;\n' % i[1])
            contents.append('\t}\n\n')
            ident+=1
        contents.append('}\n')
        open(self.__fileTargetDHCP, 'w').writelines(contents)
        return True

    def writeLTSP(self):
        l = self.getGlobalConfig('localApps')
        if l.upper() == 'TRUE': l = 'True'
        else: l = 'False'
        contents = []
        contents.append('[default]\n')
        contents.append('\tLOCALDEV=%s\n'%l)
        contents.append('\tSOUND=True\n')
        contents.append('\n')
        for i in self.getMachines():
            contents.append('[%s]\n' % i[0])
            contents.append('\tLOCAL_APPS=%s\n' % str(i[2]))
            contents.append('\tLOCAL_APPS_MENU=%s\n' % str(i[2]))
            contents.append('\n')
        try:
            open(self.__fileTargetLTSP, 'w').writelines(contents)
            return True
        except:
            return False
