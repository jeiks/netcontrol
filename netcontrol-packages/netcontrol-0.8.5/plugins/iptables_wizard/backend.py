#-*- coding: utf-8 -*-
import re
import os

from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *
from netcontrol import apis


class Config(Plugin):
    implements(IConfigurable)
    name = 'IPTAssist'
    icon = '/dl/iptables_wizard/icon.png'
    id = 'IPTAssist'
    configFile = '/etc/netcontrol/iptables_wizard.cfg'
    procPathName = '/proc/sys/net/ipv4/'
    procFiles = {'proc1':'icmp_echo_ignore_broadcasts',
                 'proc2':'conf/all/accept_source_route',
                 'proc3':'tcp_syncookies',
                 'proc4':'conf/all/accept_redirects',
                 'proc5':'conf/all/send_redirects',
                 'proc6':'conf/all/rp_filter',
                 'proc7':'conf/all/log_martians',
                 'proc8':'ip_forward'}

    def list_files(self):
        return []

    def __getCfg(self):
        from ConfigParser import ConfigParser
        cfg = ConfigParser()
        ret = cfg.read( self.configFile )

        return cfg

    def readIfaces(self, net, fisica=True, virtual=False):
        try:
            ifaceNET = self.__getCfg().get(net, 'ifaces').split()
        except:
            ifaceNET = []

        ret = []
        for i in open('/etc/network/interfaces').readlines():
            i = i.split('#')[0]
            if i.find('iface') <> -1 and i.find('lo') == -1:
                try:    i = i.split()[1].strip()
                except: continue
                if (virtual and i.find(':') <> -1) or \
                   (fisica  and i.find(':') == -1):
                       try:
                           ret.append([i, ifaceNET.index(i) <> -1 ])
                       except:
                           ret.append( [i, False] )

        return ret

    def saveIfaces(self, net, ifaces):
        if type(ifaces) <> str:
            tmp = ''
            for i in ifaces:
                tmp += i+' '
            ifaces = tmp

        cfg = self.__getCfg()
        if not cfg.has_section(net):
            cfg.add_section(net)

        cfg.set(net, 'ifaces', ifaces)
        cfg.write( open(self.configFile, 'w') )

        del cfg

    def saveConfigSection(self, section, values):
        cfg = self.__getCfg()
        if not cfg.has_section( section ):
            cfg.add_section( section )

        for i in values.keys():
            cfg.set(section, i, values[i])

        cfg.write( open(self.configFile, 'w') )

        del cfg

    def getConfigSection(self, section):
        try:
            ret = {}
            for i in self.__getCfg().items( section ):
                ret[ i[0] ] = i[1]
            return ret
        except:
            return {}

    def iptables(self, cmd):
        from os import system
        system('iptables %s' % cmd)
        #open('/tmp/iptables','a').write(cmd+'\n')

    def clearFirewall(self):
        from os import system

        iptables = self.iptables

        #Limpando as regras existentes
        for i in self.procFiles.itervalues():
            open(self.procPathName+i, 'w').write('0\n')

        iptables('-F')
        iptables('-t nat -F')
        iptables('-P INPUT   ACCEPT')
        iptables('-P OUTPUT  ACCEPT')
        iptables('-P FORWARD ACCEPT')

    def getIPMasc(self, iface):
        from netifaces import ifaddresses
        from netcontrolrede.netcontrolrede import Rede

        try:
            info = ifaddresses(iface)[2][0]
            info = Rede(info['addr'], info['netmask'])
            return info.getRedeAsStr()+'/'+info.getMascAsStr()
        except:
            return ''

    def getCfgBoolean(self, dic, key):
        if dic.has_key(key):
            return dic[key] == '1'
        else:
            return False

    def applyRule(self, dic, key, cmd):
        if self.getCfgBoolean(dic, key):
            if type(cmd) == tuple or type(cmd) == list:
                for i in cmd: self.iptables(i)
            else:
                self.iptables(cmd)

    def applyAllSettings(self):
        from os import system

        cfg = self.__getCfg()
        iptables  = self.iptables
        applyRule = self.applyRule

        #Limpando as regras existentes
        self.clearFirewall()

        #Carregando módulos
        system('modprobe nf_conntrack_ftp')
        system('modprobe nf_nat_ftp')

        #Permitindo tudo para interface loopback
        iptables('-A INPUT -i lo -j ACCEPT')
        iptables('-A OUTPUT -o lo -j ACCEPT')
        
        #Permitindo conexão ao netcontrol
        iptables('-I INPUT -p tcp --dport 8000 -j ACCEPT')

        #### Cfg EXTRAS (opções no /proc)
        extra = self.getConfigSection( 'cfgEXTRAS' )
        for i in extra.iteritems():
            try:
                open( self.procPathName+self.procFiles[ i[0] ], 'w').write( i[1]+'\n' )
            except: pass

        del extra

        #Obtendo as interfaces
        try:
            ifaceEXT = cfg.get('SAIDA', 'ifaces')
            ifaceDMZ = cfg.get('DMZ'  , 'ifaces')
            ifaceSQL = cfg.get('SQL'  , 'ifaces')
            ifaceINT = cfg.get('INTERNA', 'ifaces').split()
        except:
            return False

        ### Cfg ENTRADA
        entrada = self.getConfigSection( 'cfgENTRADA' )

        applyRule(entrada, 'cfgentrada1', '-P INPUT DROP')
        applyRule(entrada, 'cfgentrada2', '-A INPUT -p icmp -m limit --limit 2/s -j ACCEPT')
        applyRule(entrada, 'cfgentrada3', '-A INPUT -p tcp --dport 22 -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT')
        applyRule(entrada, 'cfgentrada4', '-A INPUT -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT')
        applyRule(entrada, 'cfgentrada5', '-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT')

        del entrada

        ### Cfg SAIDA
        saida = self.getConfigSection( 'cfgSAIDA' )

        applyRule(saida, 'saida1', '-P OUTPUT DROP')
        applyRule(saida, 'saida2', '-A OUTPUT -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT')

        del saida

        ### Cfg DMZ
        dmz = self.getConfigSection( 'cfgDMZ' )

        redeDMZ = self.getIPMasc(ifaceDMZ)
        applyRule(dmz, 'cfgdmz1', [
                  '-A FORWARD -i %s -o %s -j ACCEPT' % (ifaceDMZ, ifaceEXT),
                  '-A FORWARD -i %s -o %s -j ACCEPT' % (ifaceEXT, ifaceDMZ)
                  ])

        for i in ifaceINT:
            applyRule(dmz, 'cfgdmz2', [
                      '-A FORWARD -i %s -o %s -j ACCEPT' % (ifaceDMZ, i),
                      '-A FORWARD -i %s -o %s -j ACCEPT' % (i, ifaceDMZ),
                      ])

        if dmz.has_key('portasliberadasdmz'):
                if self.getCfgBoolean(dmz, 'cfgdmz1'):
                    for port in dmz['portasliberadasdmz'].split():
                        iptables('-t nat -A PREROUTING -i %s %s -p tcp --dport %s -j ACCEPT' % (ifaceEXT, redeDMZ == '' and '' or '-d '+redeDMZ, port))
                        iptables('-A FORWARD -i %s -o %s %s -p tcp --dport %s -j ACCEPT' %(ifaceEXT, ifaceDMZ, redeDMZ == '' and '' or '-d '+redeDMZ, port))
                        iptables('-t nat -A POSTROUTING -i %s -o %s -p tcp --dport %s -j MASQUERADE' % (ifaceEXT, ifaceDMZ, port))
                if self.getCfgBoolean(dmz, 'cfgdmz2'):
                    for port in dmz['portasliberadasdmz'].split():
                        for i in ifaceINT:
                            tmpRede = self.getIPMasc( i )
                            iptables('-t nat -A POSTROUTING -o %s %s -p tcp --dport %s -j MASQUERADE'  %  ( ifaceDMZ , tmpRede <> '' and '-s '+tmpRede or '', port ))
                            iptables('-A FORWARD -i %s %s -o %s -p tcp --dport %s -j ACCEPT' % (i, tmpRede <> '' and '-s '+tmpRede or '', ifaceDMZ, port) )
                            iptables('-A FORWARD -i %s %s -o %s -p tcp --sport %s -j ACCEPT' % (ifaceDMZ, tmpRede <> '' and '-d '+tmpRede or '', i, port) )

        ### Cfg SQL
        sql = self.getConfigSection( 'cfgSQL' )

        redeSQL = self.getIPMasc(ifaceSQL)
        applyRule(sql, 'cfgsql1', [
                  '-A FORWARD -i %s -o %s -j ACCEPT' % (ifaceSQL, ifaceEXT),
                  '-A FORWARD -i %s -o %s -j ACCEPT' % (ifaceEXT, ifaceSQL)
                  ])

        for i in ifaceINT:
            applyRule(sql, 'cfgsql2', [
                      '-A FORWARD -i %s -o %s -j ACCEPT' % (ifaceSQL, i),
                      '-A FORWARD -i %s -o %s -j ACCEPT' % (i, ifaceSQL),
                      ])

        if sql.has_key('portasliberadassql'):
                if self.getCfgBoolean(sql, 'cfgsql1'):
                    for i in sql['portasliberadassql'].split():
                        iptables('-t nat -A PREROUTING -i %s %s -p tcp --dport %s -j ACCEPT' % (ifaceEXT, redeSQL == '' and '' or '-d '+redeSQL, port))
                        iptables('-A FORWARD -i %s -o %s %s -p tcp --dport %s -j ACCEPT' %(ifaceEXT, ifaceSQL, redeSQL == '' and '' or '-d '+redeSQL, port))
                        iptables('-t nat -A POSTROUTING -i %s -o %s -p tcp --dport %s -j MASQUERADE' % (ifaceEXT, ifaceSQL, port))
                if self.getCfgBoolean(sql, 'cfgsql2'):
                    for port in sql['portasliberadassql'].split():
                        for i in ifaceINT:
                            tmpRede = self.getIPMasc( i )
                            iptables('-t nat -A POSTROUTING -o %s %s -p tcp --dport %s -j MASQUERADE'  %  ( ifaceSQL , tmpRede <> '' and '-s '+tmpRede or '', port ))
                            iptables('-A FORWARD -i %s %s -o %s -p tcp --dport %s -j ACCEPT' % (i, tmpRede <> '' and '-s '+tmpRede or '', ifaceSQL, port) )
                            iptables('-A FORWARD -i %s %s -o %s -p tcp --sport %s -j ACCEPT' % (ifaceSQL, tmpRede <> '' and '-d '+tmpRede or '', i, port) )

        del sql

        ### Cfg VPN
        vpn = self.getConfigSection( 'cfgVPN' )
        if vpn.has_key('servidoresvpn') and self.getCfgBoolean(vpn, 'cfgvpn'):
            for i in vpn['servidoresvpn'].replace('\n',' ').split():
                iptables( '-t nat -I POSTROUTING -o %s -d %s -p tcp --dport 1723 -j MASQUERADE' % (ifaceDMZ, i) )
                iptables( '-t nat -I POSTROUTING -o %s -d %s -p udp --dport 1701 -j MASQUERADE' % (ifaceDMZ, i) )
                iptables( '-t nat -I POSTROUTING -o %s -d %s -p 47               -j MASQUERADE' % (ifaceDMZ, i) )
                
                iptables( '-I FORWARD -s %s -p tcp --sport 1723 -j ACCEPT' % i )
                iptables( '-I FORWARD -s %s -p udp --sport 1701 -j ACCEPT' % i )
                iptables( '-I FORWARD -s %s -p tcp --sport 47   -j ACCEPT' % i )
                
                iptables( '-I FORWARD -d %s -p tcp --dport 1723 -j ACCEPT' % i )
                iptables( '-I FORWARD -d %s -p udp --dport 1701 -j ACCEPT' % i )
                iptables( '-I FORWARD -d %s -p tcp --dport 47   -j ACCEPT' % i )

        del vpn

        ### Cfg OvNg
        ovng = self.getConfigSection( 'cfgOvNg' )
        if ovng.has_key('hostsmsn'):
            for ip in ovng['hostsmsn'].replace('\n',' ').split():
                for port in '21','1863','4452','6891:6900':
                    iptables('-I FORWARD -s %s -o %s -p tcp --dport %s -j ACCEPT' % (ip, ifaceEXT, port))
                    iptables('-I FORWARD -d %s -i %s -p tcp --sport %s -j ACCEPT' % (ip, ifaceEXT, port))
                    iptables('-t nat -I POSTROUTING -o %s -s %s -p tcp --dport %s -j MASQUERADE' % (ifaceEXT, ip, port))

        if ovng.has_key('hostslivres'):
            for ip in ovng['hostslivres'].replace('\n',' ').split():
                iptables('-I INPUT -s %s -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT' % ip)
                iptables('-t nat -I PREROUTING -s %s -j ACCEPT' % ip)
                iptables('-I FORWARD -d %s -i %s -m state --state ESTABLISHED,RELATED -j ACCEPT' % (ip, ifaceEXT))
                iptables('-I FORWARD -s %s -o %s -j ACCEPT' % (ip, ifaceEXT))
                iptables('-t nat -I POSTROUTING -s %s -o %s -j MASQUERADE' % (ip, ifaceEXT))

        del ovng

        ### Cfg Internas
        for i in ifaceINT:
            intRede = self.getIPMasc( i )
            # configuração dessa interface
            intCfg  = self.getConfigSection( i )
            # liberando conexoes esternas para essa interface
            iptables('-A FORWARD %s -o %s -i %s -m state --state ESTABLISHED,RELATED -j ACCEPT' % (intRede <> '' and '-d '+intRede or '', i, ifaceEXT))
            # aceitando conexoes dessa interface no servidor
            iptables('-A INPUT -i %s %s -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT' % (i, intRede == '' and '' or '-s '+intRede))

            # proxy transparente, redirecionamento de portas
            if intCfg.has_key('portasproxy') and self.getCfgBoolean(intCfg, 'habilitarproxytransp'):
                for port in intCfg['portasproxy'].split():
                    iptables('-t nat -A PREROUTING -i %s %s -p tcp -m tcp --dport %s -j REDIRECT --to-ports 3128' % (i, intRede == '' and '' or '-s '+intRede ,port))

            # outras portas para acesso por MASQUERADE
            if intCfg.has_key('portasliberadasinterna') and intRede <> '':
                for port in intCfg['portasliberadasinterna'].split():
                    iptables('-A FORWARD -i %s -s %s -o %s -p tcp --dport %s -j ACCEPT' % (i, intRede, ifaceEXT, port))
                    iptables('-A FORWARD -o %s -s %s -i %s -p tcp --sport %s -j ACCEPT' % (i, intRede, ifaceEXT, port))
                    iptables('-t nat -A POSTROUTING -s %s -o %s -p tcp --dport %s -j MASQUERADE' %(intRede, ifaceEXT, port))

            # liberando encaminhamento para essa rede
            applyRule(intCfg, 'interna1', [ '-A FORWARD -i %s -j ACCEPT' % i,
                                            '-A FORWARD -o %s -j ACCEPT' % i ] )
            # liberando FTP
            applyRule(intCfg, 'interna2', [ '-t nat -A POSTROUTING -p tcp --sport 1024: --dport 1024: -m state --state ESTABLISHED,RELATED -o %s -j MASQUERADE' % ifaceEXT,
                                            '-A FORWARD -i %s -p tcp --sport 1024: --dport 1024: -m state --state ESTABLISHED,RELATED -j ACCEPT' % i,
                                            '-A FORWARD -o %s -p tcp --sport 1024: --dport 1024: -m state --state ESTABLISHED,RELATED -j ACCEPT' % i ])
            # liberando forwarding para DMZ
            applyRule(intCfg, 'interna3', [ '-A FORWARD -i %s -o %s -j ACCEPT' % (i, ifaceDMZ),
                                            '-A FORWARD -o %s -i %s -j ACCEPT' % (i, ifaceDMZ) ])
            # liberando forwarding para SQL
            applyRule(intCfg, 'interna4', [ '-A FORWARD -i %s -o %s -j ACCEPT' % (i, ifaceSQL),
                                            '-A FORWARD -o %s -i %s -j ACCEPT' % (i, ifaceSQL) ])

            # aplicando política DROP
            applyRule(intCfg, 'intusardrop', [ '-A FORWARD -i %s -j DROP' % i,
                                               '-A FORWARD -o %s -j DROP' % i  ] )
        ######
        return True
