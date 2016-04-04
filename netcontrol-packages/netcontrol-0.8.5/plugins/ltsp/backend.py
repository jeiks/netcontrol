# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *

from ltsp.ltsp import LTSP
from execute import execute

class Config(Plugin):
    implements(IConfigurable)
    name = 'LTSP'
    icon = '/dl/ltsp/icon.png'
    id = 'ltsp'

    def list_files(self):
        return ['/etc/ltsp']# puxando arquivo que contem os hosts do pc
 
    def getInterfaces(self):
        from os import access
        if access('/proc/net/dev', 4):
            ret = []
            for i in open('/proc/net/dev').readlines():
                if i.find(':')    <> -1 and \
                    i.find('lo')   == -1 and \
                    i.find('vbox') == -1:
                        ret.append(i.split(':')[0].strip())
            return ret

        return []

    def getClients(self):
        l = LTSP()
        ret = l.getMachines()
        del l
        
        return ret

    def getServer(self):
        l = LTSP()
        ret = l.getServer()
        del l
        if len(ret) <> 5:
            ret = ['','','','','']

        return ret

    def getRangeLTSP(self):
        l = LTSP()
        ret = l.getRange()
        del l

        return ret

    def getAppsLTSP(self):
        l = LTSP()
        ret = l.getGlobalConfig('localApps')
        del l
        if ret == '': ret = "False"
        return ret

    def delRangeLTSP(self):
        l = LTSP()
        if l.delRange():
            l.writeDHCP()
            l.writeLTSP()

        del l
        self.restartDHCP()

    def setRangeLTSP(self, start, end, apps):
        start = start.strip()
        end   =   end.strip()
        apps  =  apps.strip()

        if apps == "cliente": apps = True
        else:                 apps = False

        l = LTSP()
        retRange = True
        retAPPS  = True

        r = l.setGlobalConfig('localApps', apps)
        if not r: retAPPS = False
        
        if start <> '' and end <> '':
            r = l.setRange( start, end )
            if not r: retRange = False
        else:
            retRange = False

        if retRange:
            l.writeDHCP()
        if retAPPS:
            l.writeLTSP()
            self.writeTFTPConfig()

        del l            

        self.restartDHCP()
        return retRange, retAPPS

    def setServer(self, ip, masc, iface):
        ip    =    ip.strip()
        masc  =  masc.strip()
        iface = iface.strip()
        if ip == '' or masc == '' or iface == '':
            return False

        #try:
        l = LTSP()
        l.setServer(ip, masc, iface)
        l.writeDHCP()
        l.writeLTSP()
        self.restartDHCP()
        
        self.writeTFTPConfig()
        self.writeINETD()
        self.writeNBD()
        self.writeEXPORTS()
        self.writeXDMCPConfig()
       
        return True
        #except:
            #return False

    def saveMachine(self, ip, mac, app):
        ip  =  ip.strip()
        mac = mac.strip()
        app = app.strip()
        if ip == '' or mac == '' or app == '':
            return False

        if app == "cliente": app = True
        else:                app = False

        l = LTSP()
        ret = l.setMachine(mac, ip, localApps=app)
        l.writeDHCP()
        l.writeLTSP()
        self.writeTFTPConfig()
        self.restartDHCP()
        del l
        return ret

    def delMachine(self, mac):
        l = LTSP()
        ret = l.delMachine(mac)
        l.writeDHCP()
        del l
        
        self.restartDHCP()
        return ret
    
    def ltspBuildClient(self, mirror='', packages=''):
        mirror = mirror.strip()
        if mirror == '':
            mirror = 'http://ftp.br.debian.org/debian'
        
        if packages <> '':
            packages = '--late-packages %s ' % packages

        base = '/opt/ltsp/'
        execute("rm -rf %s" % base)
        execute("ltsp-build-client --purge-chroot --security-mirror none --base %s --mirror %s %s" % (base,mirror,packages) )
        execute("ltsp-update-image")
        self.writeNBD()

    def getLTSPInfo(self):
        erro = 1
        ret = []
        try:
            l = LTSP()
            l.writeDHCP()
            l.writeLTSP()
        except:
            ret.append('Erro %d: Não foi possível criar a configuração inicial do Boot Remoto.' % erro +\
                       'Acesse a aba "Configurar Interface de Rede" ' +\
                       'e escolha uma interface para seu servidor de Boot Remoto.')
            erro+=1
        
        try:
            self.writeEXPORTS()
        except:
            ret.append('Erro %d: A exportação de diretórios não pôde ser configurada.' % erro +\
                       'Verifique se o pacote "nfs-kernel-server" está instalado e se o arquivo ' +\
                       '"/etc/exports" possui permissão de escrita.')
            erro+=1

        try:
            self.writeINETD()
        except:
            ret.append('Erro %d: O fornecimento de sessão remota não pôde ser configurado.'  % erro +\
                       'Verifique se o pacote "openbsd-inetd" está instalado e se o arquivo' +\
                       '"/etc/inetd.conf" possui permissão de escrita.')
            erro+=1

        try:
            self.writeNBD()
        except:
            ret.append('Erro %d: O fornecimento da imagem dos clientes não pôde ser configurada.' % erro  +\
                       'Verifique se o pacote "nbd-server" está instalado e se o arquivo' +\
                       '"/etc/nbd-server/config" possui permissão de escrita.')
            erro+=1

        try:
            self.writeTFTPConfig()
        except:
            ret.append('Erro %d: O fornecimento de PXE para os clientes na rede não pôde ser configurado.' % erro +\
                       'Verifique se o pacote "tftpd-hpa" está instalado e se o arquivo' +\
                       '"/etc/default/tftpd-hpa" possui permissão de escrita.')
            erro+=1

        try:
            self.writeXDMCPConfig()
        except:
            ret.append('Erro %d: O fornecimento de sessão remota não pôde ser configurado.'  % erro +\
                       'Verifique se o pacote "gdm" está instalado e se o arquivo' +\
                       '"/etc/gdm3/daemon.conf" possui permissão de escrita.')
            erro+=1

        from os import access
        if not access('/opt/ltsp/images/i386.img', 4):
            ret.append('Erro %d: A imagem dos clientes não foi criada. Acesse a aba ' % erro +\
                       '"Configurar sistema dos clientes" e crie uma nova base de boot para os clientes.')
            erro+=1

        if ret == []:
            ret.append("O sistema não detectou nenhum problema na configuração do servidor de Boot Remoto.")

        return ret

    def hasImageCreated(self):
        from os import access
        return access('/opt/ltsp/images/i386.img', 4)

    def writeTFTPConfig(self):
        open('/etc/default/tftpd-hpa','w').write("""
TFTP_USERNAME="tftp"
TFTP_DIRECTORY="/srv/tftp"
TFTP_ADDRESS="0.0.0.0:69"
TFTP_OPTIONS="--secure"
""")
        self.restartTFTP()

    def writeXDMCPConfig(self):
        from ConfigParser import ConfigParser
        section  = 'xdmcp'
        fileName = '/etc/gdm3/daemon.conf'
        c = ConfigParser()
        c.read( fileName )
        tmp = ''
        try:
            tmp = c.get(section,'Enabled')
        except:
            pass

        if tmp.upper() <> 'TRUE':
            if not c.has_section( section ):
                c.add_section( section )

            c.set(section,'Enabled', 'true')
            c.write( open( fileName,'w') )

            self.restartGDM()

    def writeINETD(self):
        ldminfod = [False, '9571', '9571 stream tcp nowait nobody /usr/sbin/tcpd /usr/sbin/ldminfod\n']
        nbdswapd = [False, '9572', '9572 stream tcp nowait nobody /usr/sbin/tcpd /usr/sbin/nbdswapd\n']
        #Disabled
        #nbdrootd = [False, '2000', '2000 stream tcp nowait nobody /usr/sbin/tcpd /usr/sbin/nbdrootd /opt/ltsp/images/i386.img\n']
        nbdrootd = [False, '2000', '']
        contents = open('/etc/inetd.conf').readlines()
        new_c = []
        for i in contents:
            aux = i.split('#')[0].strip()
            if aux <> '' and aux <> '\n':
                for serv in [ldminfod, nbdrootd, nbdswapd]:
                    if not serv[0]:
                        if aux.split()[0] == serv[1]:
                            i = serv[2]
                            serv[0] = True
            new_c.append(i)
        for serv in [ldminfod, nbdrootd, nbdswapd]:
            if not serv[0]:
                new_c.append(serv[2])
        open('/etc/inetd.conf','w').writelines(new_c)
        self.restartINETD()

    def writeNBD(self):
        from ConfigParser import ConfigParser
        fileName = '/etc/nbd-server/config'
        c = ConfigParser()
        c.add_section('generic')
        c.set('generic', 'user', 'nbd')
        c.set('generic','group', 'nbd')

        c.add_section('exports')
        c.set('exports', 'exportname', '/opt/ltsp/images/i386.img')
        c.set('exports', 'port', '2000')
        c.set('exports', 'readonly', 'true')
        c.write( open(fileName,'w') )

        self.restartNBD()

    def writeEXPORTS(self):
        i386 = [False, '/opt/ltsp/i386  *(rw,no_subtree_check)\n']
        home = [False, '/home           *(rw,no_subtree_check)\n']

        contents = open('/etc/exports').readlines()
        new_c = []
        for i in contents:
            aux = i.split("#")[0]
            if aux <> '' and aux <> '\n':
                try:
                    for line in [i386, home]:
                        if not line[0]:
                            if aux[0] == line[1].split()[0]:
                                i = line[1]
                                line[0] = True
                except: pass
            new_c.append(i)

        for line in [i386, home]:
            if not line[0]:
                new_c.append(line[1])
        
        open('/etc/exports','w').writelines( new_c )
        self.restartEXPORTS()

    def restartEXPORTS(self):
        execute("/etc/init.d/nfs-kernel-server restart","True")

    def restartNBD(self):
        execute("/etc/init.d/nbd-server restart","True")

    def restartDHCP(self):
        execute("/etc/init.d/isc-dhcp-server restart","True")
    
    def restartTFTP(self):
        execute("/etc/init.d/tftpd-hpa restart","True")
    
    def restartGDM(self):
        execute("/etc/init.d/gdm3 restart","True")

    def restartINETD(self):
        execute("/etc/init.d/openbsd-inetd restart","True")
