# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Written by
    Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com>
    João Paulo Constantino <joaopauloctga@gmail.com>
    for FluxSoftwares)

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
from netcontrol import apis

from ConfigParser import ConfigParser
from netcontrolldap.netcontrolldap import LDAPConnection

fileSync = '/tmp/netcontrolControleWEB.pid'

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
    name = 'ControleWeb'
    icon = '/dl/controle_web/icon.png'
    id = 'ControleWeb'
    baseDB = '/var/lib/squidguard/db'
    templatesDir = '/var/lib/netcontrol/plugins/controle_web/templates'
    squidGCfg = '/etc/squidguard/squidGuard.conf'

    def list_files(self):
        return []

    def _execute(self, command, daemon=False):
        '''
        @return:
            [ (Boolean) Coomand executed with sucess ,
              (tuple) (stdout,stderr)                  ]
        '''
        cmd = command.split()
        if daemon:
            from time import sleep
            from os import access
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

    def __getBaseDN(self):
        cfg = ConfigParser()
        try:
            cfg.read('/etc/ldap/netcontrol')
            return cfg.get('base', 'bindDN')
        except:
            return ''

    def __getConLDAP(self):
        cfg = ConfigParser()
        try:
            cfg.read('/etc/ldap/netcontrol')
            server   = cfg.get('base', 'server')
            bindDN   = cfg.get('base', 'bindDN')
            adminPW  = cfg.get('base', 'adminPW')
        except:
            return None
        SAM = False
        try: SAM = bool(cfg.get('base', 'SAM'))
        except: pass

        del cfg
        con = LDAPConnection(server, ssl=True,
                    admPasswd=adminPW, baseDN=bindDN,
                    SAM=SAM)

        return con

    def __getConfig(self):
        cfg = None
        try:
            cfg = ConfigParser()
            cfg.read('/etc/netcontrol/controleweb.cfg')
        except:
            pass

        return cfg

    def __getConGroup(self):
        #retorna uma lista com os grupos, composta pelas
        #   suas listas de acesso
        cfg = self.__getConfig()
        ret = {}

        if cfg is None: return ret

        try:
            grupos = cfg.sections()
            for i in grupos:
                if i <> 'ips':
                    ret[i] = cfg.items(i)
                    if ret[ i ] <> []:
                        ret[i] = map(lambda x: (x[0], x[1]=='True'), ret[i])
        except:
            pass
        
        return ret

    def __getConIP(self, group):
        #retorna uma lista com os grupos, composta pelas
        #   suas listas de acesso
        cfg = self.__getConfig()
        try:
            return cfg.get(group,'ips').split()
        except:
            return []

    def groupExists(self, group):
        cfg = self.__getConfig()
        try:
            if cfg.has_section(group):
                return True
        except:
            pass

        ldapGroup = group
        if self.__getConLDAP() is None:
            return False

        ret = self.__getConLDAP().search(
                filter='(&(objectClass=posixGroup)(cn=%s))'%ldapGroup,
                attrs=['cn'])

        if ret == []: return False
        else:         return True

    def getGroups(self):
        # retorna os grupos de proxy do LDAP
        gruposCfg  = self.__getConGroup()
        gruposProxy = {}

        for i in gruposCfg.keys():
            if i.startswith('_'):
                gruposProxy[i] = ( self.__getConIP(i), gruposCfg[i])

        if self.__getConLDAP() is None:
            return gruposProxy

        gruposLDAP = map(
                lambda x: x[1]['cn'][0] ,
                self.__getConLDAP().search(
                    filter='(&(objectClass=posixGroup)(cn=proxy*))' ,
                    attrs=['cn'])
                )
        try:    gruposLDAP.remove( 'proxy' )
        except: pass

        for i in gruposLDAP:
            try:
                membros = \
                        self.__getConLDAP().findGroup(i)[0][1]['memberUid']
            except:
                membros = []

            gruposProxy[i] = membros

        for i in gruposProxy.keys():
            
            if gruposCfg.has_key( i ):
                gruposProxy[i] = (gruposProxy[i],gruposCfg[i])
            else:
                gruposProxy[i] = (gruposProxy[i], [])

        #print 'Grupos:', gruposProxy.keys()
        #for i in gruposProxy.keys():
        #    print 'Usuarios:', gruposProxy[i][0]
        #    print 'Listas:', gruposProxy[i][1]

        del gruposCfg
        del gruposLDAP

        return gruposProxy

    def getAllUsers(self):
        if self.__getConLDAP() is None:
            return False

        ret = []

        try:
            usuarios = self.__getConLDAP().getUsers()
            for i in usuarios:
                if int(i[1]['uidNumber'][0]) >= 1000 and\
                   i[1]['uidNumber'][0] <> '65534':
                    ret.append( "%s (%s)" % (
                                    i[1]['uid'][0],
                                    i[1]['displayName'][0]
                              ))
        except: pass

        return ret

    def getUsersFrom(self, ldapGroup):
        if self.__getConLDAP() is None:
            return False
       
        users = self.__getConLDAP().search(
                filter='(&(objectClass=posixGroup)(cn=%s))' % ldapGroup,
                attrs=['memberUid']
                )
       
        if users <> []:
            try:
                users = users[0][1]['memberUid']
            except:
                users = []

        return users

    def removeUserFromProxy(self, user):
        if self.__getConLDAP() is None:
            return False

        grupos = map(
                lambda x: x[1]['cn'][0],
                self.__getConLDAP().search(
                        filter='(&(memberUid=%s)(objectClass=posixGroup)(cn=proxy*))' % user,
                        attrs=['cn'])
                )
        try: grupos.remove('proxy')
        except: pass
        ret = self.__getConLDAP().removeUserFromGroups(user, grupos)
        if ret[0] == 0: return True
        else: return False

    def addUserToGroups(self, user, ldapGroup):
        if self.__getConLDAP() is None:
            return False

        self.removeUserFromProxy( user )
        ret = self.__getConLDAP().addUserToGroups(user, ldapGroup)
        if ret[0] == 0: return True
        else: return False

    def setSquidGroups(self, group, SquidGroups):
        cfg = self.__getConfig()
        if cfg is None: return False

        try:
            if not cfg.has_section( group ):
                cfg.add_section( group )

            for i in SquidGroups:
                cfg.set( group, i[0],
                         i[1]=='1' and 'True' or 'False' )
        
            cfgFile = open('/etc/netcontrol/controleweb.cfg','w')
            cfg.write( cfgFile )
            cfgFile.close()
            del cfgFile
            del cfg
            return True
        except:
            return False

    def getSquidGroups(self, group):
        # obtem os grupos do squid baseado no grupo
        # LDAP fornecido
        grupos = self.getAllSquidGroups()
        g = str( group )
        for i in grupos.keys():
            if g.find("'%s'," % grupos[i][0]) <> -1:
                grupos[i] = [
                    grupos[i][0],
                    g.split("'%s', "%grupos[i][0])[1].startswith('True')
                ]
        return grupos

    def getAllSquidGroups(self):
        groups = {
                'Anúncios de publicidade': ['ads', True],
                'Conteúdo adulto, mas não pornô': ['adult', False],
                'Agressividade': ['aggressive', False],
                'Spyware': ['antispyware', False],
                'Nú Artístico': ['artnudes', True],
                'Download de Áudio e Vídeo': ['audio-video', False],
                'Bancos Online': ['banking', True],
                'Informação de cerveja e licores': ['beerliquorinfo', False],
                'Venda de cerveja e licores': ['beerliquorsale', False],
                'Materiais para Celulares e Móveis': ['cellphones', False],
                'Bate-Papos': ['chat', False],
                'Material para bebês': ['childcare', False],
                'Roupas': ['clothing', False],
                'Culinária': ['culinary', False],
                'Relacionamento': ['dating', False],
                'Protetor de Tela, Background, etc.': ['desktopsillies', False],
                'Discadores': ['dialers', False],
                'Drogas': ['drugs', False],
                'Compras Online': ['ecommerce', False],
                'Entreterimento (filmes, livros, etc.)': ['entertainment', True],
                'Educação': ['frencheducation', True],
                'Jogos de azar': ['gambling', False],
                'Jardinagem': ['gardening', False],
                'Governamentais': ['government', False],
                'Hacking/Cracking': ['hacking', False],
                'Reparo doméstico': ['homerepair', False],
                'Higiene': ['hygiene', False],
                'Mensageiros': ['instantmessaging', False],
                'Venda de Jóias': ['jewelry', False],
                'Empregos': ['jobsearch', False],
                'Infantil': ['kidstimewasting', False],
                'E-mails': ['mail', True],
                'Naturalismo': ['naturism', False],
                'Notícias': ['news', True],
                'Leilões online': ['onlineauctions', False],
                'Jogos online': ['onlinegames', False],
                'Pagamentos': ['onlinepayment', False],
                'Finanças': ['personalfinance', False],
                'Animais de estimação': ['pets', False],
                'Sites falsos': ['phishing', False],
                'Pornografia': ['porn', False],
                'Proxies': ['proxy', False],
                'Rádio e Televisão': ['radio', False],
                'Religião': ['religion', False],
                'Toques de celular': ['ringtones', False],
                'Sites de Busca': ['searchengines', True],
                'Sexualidade': ['sexuality', True],
                'Notícias de Esporte': ['sportnews', False],
                'Esportes': ['sports', False],
                'Spyware para download': ['spyware', False],
                'Atualizações de software': ['updatesites', False],
                'Férias': ['vacation', False],
                'Violência': ['violence', False],
                'Sites com vírus': ['virusinfected', False],
                'Software pirata': ['warez', False],
                'Clima e tempo': ['weather', True],
                'Armas': ['weapons', False],
                'Webmail': ['webmail', True],
                'Conteúdo infantil': ['whitelist', True],
                }

        return groups

    def getPersLiberados(self, group):
        try:
            if group.startswith('_'):
                group = 'iplist'+group
            group = group.replace(' ','_')
            Dir = '%s/blacklists/%s' % (self.baseDB,group)
            return map(lambda x: x.replace('\n',''),
                       open(Dir+'/liberados').readlines() )
        except:
            return []

    def getPersBloqueados(self, group):
        try:
            if group.startswith('_'):
                group = 'iplist'+group
            group = group.replace(' ','_')
            Dir = '%s/blacklists/%s' % (self.baseDB,group)
            return map(lambda x: x.replace('\n',''),
                       open(Dir+'/bloqueados').readlines() )
        except:
            return []

    def setPersLiberados(self, group, urls):
        if group.startswith('_'):
            group = 'iplist'+group
        group = group.replace(' ','_')
        from os import makedirs
        Dir = '%s/blacklists/%s' % (self.baseDB,group)
        try:
            makedirs( Dir, 0755 )
        except:
            pass

        try:
            open('%s/liberados' % Dir, 'w').writelines(
                    map(lambda x: x.strip()+'\n', urls)
                )
        except:
            pass

    def setPersBloqueados(self, group, urls):
        if group.startswith('_'):
            group = 'iplist'+group
        group = group.replace(' ','_')
        from os import makedirs
        Dir = '%s/blacklists/%s' % (self.baseDB,group)
        try:
            makedirs( Dir, 0755 )
        except:
            pass

        try:
            open('%s/bloqueados' % Dir, 'w').writelines(
                    map(lambda x: x.strip()+'\n', urls)
                )
        except:
            pass

    def addUrlLiberados(self, group, newUrl):
        if type(newUrl) != tuple and type(newUrl) != list:
            newUrl = [newUrl]

        try:
            newUrl = map(lambda x: x.strip(), newUrl)
            while True: newUrl.remove('')
        except:
            pass

        try:
            self.setPersLiberados(
                group,
                self.getPersLiberados(group) + newUrl )
            return True
        except:
            return False

    def addUrlBloqueados(self, group, newUrl):
        if type(newUrl) != tuple and type(newUrl) != list:
            newUrl = [newUrl]

        if newUrl == '':
            return False

        try:
            self.setPersBloqueados(
                    group,
                    self.getPersBloqueados(group) + newUrl )
            return True
        except:
            return False

    def delUrlLiberados(self, group, url):
        url = url.strip()
        if url == '':
            return False

        l = self.getPersLiberados(group)
        try:
            l.remove( url )
            self.setPersLiberados(group, l)
            return True
        except:
            return False

    def delUrlBloqueados(self, group, url):
        url = url.strip()
        if url == '':
            return False

        l = self.getPersBloqueados(group)
        try:
            l.remove( url )
            self.setPersBloqueados(group, l)
            return True
        except:
            return False

    def addLdapGroup(self, ldapGroup):
        if self.__getConLDAP() is None:
            return False
        try:
            if self.__getConLDAP().addGroup( 'proxy'+ldapGroup )[0] == 0:
                return True
            else:
                return False
        except:
            return False

    def delLdapGroup(self, ldapGroup):
        if self.__getConLDAP() is None:
            return False

        try:
            if not ldapGroup.startswith('proxy'):
                ldapGroup = 'proxy'+ldapGroup

            if self.__getConLDAP().delGroup( ldapGroup )[0] <> 0:
                return False
        except:
           return False
      
        try:
            cfg = self.__getConfig()
            tmp = cfg.remove_section( ldapGroup )
            cfg.write(
                open('/etc/netcontrol/controleweb.cfg','w') )
            del cfg
        except:
            pass

        return True

    def setGroupIPs(self, ips, group):
        if type(ips) <> str:
            ips_str = ''
            for i in ips:
                ips_str += i+' '
            ips = ips_str

        cfg = self.__getConfig()
        try:
            if not cfg.has_section(group):
                cfg.add_section(group)

            cfg.set(group, 'ips', ips.strip())
            cfg.write(open('/etc/netcontrol/controleweb.cfg','w'))
            return True
        except:
            return False

    def getIPsFrom(self, group):
        return self.__getConIP(group)

    def getIPsFromAsString(self, group):
        ret = ''
        for i in self.getIPsFrom(group):
            ret += i+' '
        return ret.strip()

    def removeIPFromGroup(self, ip, group):
        ips = self.__getConIP(group)
        try:
            if type(ip) == str:
                ip = ip.split()
            for i in ip:
                ips.remove(i)
        except:
            pass

        return self.setGroupIPs(ips, group)

    def addIPToGroup(self, ip, group):
        if type(ip) == str:
            ip = ip.split()
        elif type(ip) == tuple:
            ip = map(lambda x: x, ip)

        self.removeIPFromGroup(ip, group)

        ips = self.__getConIP(group)
        ips = ips + ip
        ips.sort()
        return self.setGroupIPs(ips, group)

    def delIPGroup(self, group):
        open('/tmp/group','w').write(str(group))
        cfg = self.__getConfig()
        try:
            cfg.remove_section(group)
            cfg.write(open('/etc/netcontrol/controleweb.cfg','w'))
            return True
        except:
            return False

    def addIPGroup(self, group):
        if not group.startswith('_'):
            group = '_'+group

        cfg = self.__getConfig()
        try:
            if cfg.has_section(group):
                return False

            cfg.add_section(group)
            cfg.write(open('/etc/netcontrol/controleweb.cfg','w'))
            return True
        except:
            return False

    def applyAllSettings(self):
        try:
            squid   = open(self.templatesDir+'/squid.conf').read()
            squidg  = open(self.templatesDir+'/squidGuard.conf').read()
            acl     = open(self.templatesDir+'/acl').read()
            src     = open(self.templatesDir+'/src').read()
            src_ips = open(self.templatesDir+'/src_ips').read()
            html    = open(self.templatesDir+'/html').read()
            pers    = open(self.templatesDir+'/pers').read()
            acl_default = open(self.templatesDir+'/acl_default').read()

            basedn = self.__getBaseDN()
            if basedn == '': return False

            grupos = self.getGroups()
            if grupos == {}: return False


            ACLs = ''
            SRCs = ''
            PERs = ''
            buildDB = []
            #para cada grupo de usuarios
            for g in grupos.keys():
                bloqueados = ''
                liberados  = ''

                Group_is_IP = False
                if g.startswith('_'):
                    name = 'iplist'+g
                    Group_is_IP = True
                else:
                    name = g

                name = name.replace(' ','_')
                Dir = '%s/blacklists/%s' % (self.baseDB,g)
                
                # personalizados: DESTs
                if len(self.getPersLiberados(g)) > 0:
                    PERs += pers.replace(
                                '[[NAME]]', name+'liberados').replace(
                                '[[DIR]]',  name).replace(
                                '[[FILE]]', 'liberados')
                    bloqueados +=  '%sliberados ' % name
                    buildDB.append(name+'/liberados')

                if len(self.getPersBloqueados(g)) > 0:
                    PERs += pers.replace(
                                '[[NAME]]', name+'bloqueados').replace(
                                '[[DIR]]',  name).replace(
                                '[[FILE]]', 'bloqueados')
                    bloqueados += '!%sbloqueados ' % name
                    buildDB.append(name+'/bloqueados')


                sG = self.getSquidGroups( grupos[g][1] )
                # para cada grupo do squid
                for t in sG.keys():
                    if sG[t][1]:
                        liberados  +=  '%s ' % sG[t][0]
                    else:
                        bloqueados += '!%s ' % sG[t][0]

                # gerando as ACLs para cada grupo
                if (Group_is_IP and self.getIPsFrom(g) <> []) or not Group_is_IP:
                    ACLs += acl.replace(
                            '[[NAME]]', name).replace(
                            '[[SQUID_GROUPS]]', bloqueados+liberados)
                        
                # gerando os SRCs para cada grupo
                if Group_is_IP:
                    ips = ''
                    for ip in self.getIPsFrom(g):
                        ips += '\t\tip %s\n' % ip
                    if ips <> '':
                        SRCs +=src_ips.replace(
                                '[[NAME]]', name).replace(
                                '[[IPS]]', ips)
                else:
                    SRCs += src.replace(
                            '[[NAME]]', name).replace(
                            '[[GROUP]]', g.replace(' ','%20')).replace(
                            '[[BINDDN]]', basedn)

            squid = squid.replace('[[BINDDN]]', basedn)
            open('/etc/squid3/squid.conf','w').write( squid )

            squidg  = squidg.replace('[[SQUIDGUARD_DB]]', self.baseDB)
            squidg += SRCs
            squidg += PERs
            squidg += 'acl {\n%s%s}' % (ACLs, acl_default)
            open('/etc/squidguard/squidGuard.conf','w').write( squidg )
            open('/var/www/block.html','w').write( html )

            
            for i in buildDB:
                Dir = '%s/blacklists/%s' % (self.baseDB,i)
                self._execute('chown -R proxy\: "%s"' % Dir)
                self._execute('su -c \'/usr/bin/squidGuard -c %s -C "%s"\' proxy' % (self.squidGCfg,Dir), True)

            self._execute('chown -R proxy:proxy "%s"' % self.baseDB)

            try:
                from socket import socket, AF_INET, SOCK_STREAM, SHUT_RDWR
                s = socket(AF_INET, SOCK_STREAM)
                s.settimeout(3)
                s.connect(('127.0.0.1', 3128))
                s.shutdown(SHUT_RDWR)
                self._execute('/etc/init.d/squid3 force-reload', True)
            except:
                self._execute('/etc/init.d/squid3 restart', True)

            return True
        except:
            return False
