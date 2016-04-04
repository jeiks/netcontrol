#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *
from ConfigParser import ConfigParser

class ProftpLDAP:
      """
      @author: <joaopauloctga@gmail.com>
      """
      _LDAPserver = None
      _LDAPInfo   = None
      _LDAPAuth   = None
      _teste = None
      
      def __init__(self):
        arq = ConfigParser()
        arq.read('/etc/ldap/netcontrol')
        self._LDAPserver = arq.get('base','server')
        self._LDAPInfo   = arq.get('base','bindDN')
        self._LDAPAuth   = arq.get('base','adminPW')
        
      def getLDAPConfs(self):
          return {"server":self._LDAPserver,"info":self._LDAPInfo,"autenticar":self._LDAPAuth}
          
      def setConfigLDAP(self):
          arq = open('/var/lib/netcontrol/plugins/proftpd/templates/ldap.conf').read()
          arq = arq.replace('[SERVIDOR]',self._LDAPserver).replace('[DC]',self._LDAPInfo).replace('[ADMIN_PWD]',self._LDAPAuth)
          open('/etc/proftpd/ldap.conf','w').write(arq)
          
          arq = open('/etc/proftpd/proftpd.conf').read()
          arq = arq.replace("#Include /etc/proftpd/ldap.conf","Include /etc/proftpd/ldap.conf")
          open('/etc/proftpd/proftpd.conf','w').write(arq)
          
          arq = open('/etc/proftpd/modules.conf').read()
          arq = arq.replace('#LoadModule mod_ldap.c','LoadModule mod_ldap.c').replace('#LoadModule mod_quotatab_ldap.c','LoadModule mod_quotatab_ldap.c')
          arq = open('/etc/proftpd/modules.conf','w').write(arq)

class Usuario():
    def __init__(self, nome=None, check=None):
        self.check = check
        self.nome = nome

class Config(Plugin):
    implements(IConfigurable)
    name = 'ProFTPD'
    icon = '/dl/proftpd/icon.png'
    id = 'proftpd'
 
    def list_files(self):
        return ['/etc/proftpd']# puxando arquivo que contem os hosts do pc
 
    def __getCon(self):
		try:
			from ConfigParser import ConfigParser
			cfg = ConfigParser()
			cfg.read('/etc/ldap/netcontrol')
			server  = cfg.get('base', 'server')
			bindDN  = cfg.get('base', 'bindDN')
			adminPW = cfg.get('base', 'adminPW')
			del cfg
			from netcontrolldap.netcontrolldap import LDAPConnection
			con = LDAPConnection(server, ssl=True,
						admPasswd=adminPW, baseDN=bindDN)

			return con
		except:
			return False
    def read(self):
        usuarios = []
        lista = []

        con = self.__getCon()
        
        if con is False:
			return False
        
        if con is None:
            for i in open('/etc/passwd','r').readlines():
                if str(i.split(':')[6]) == "/bin/bash\n" or str(i.split(':')[6]) == "/bin/sh\n":
                    usuarios.append(str(i.split(':')[0]))
        else:
            addedRoot = False
            for i in con.getUsers():
                if int(i[1]['uidNumber'][0]) >= 1000 and int(i[1]['uidNumber'][0]) != 65534 or int(i[1]['uidNumber'][0]) == 0:
                  usuarios.append(i[1]['uid'][0])
        del con

        f = open('/etc/ftpusers').read()
        for i in usuarios:
            if f.find(i+'\n') == -1:
                lista.append(Usuario(i,True))
            else:
                lista.append(Usuario(i,False))
        
        return lista
 
    def save(self, arquivo):
        d = ''
        for h in arquivo:
            d += '%s\n' % (h)
        ConfManager.get().save('proftpd', '/etc/proftpd/proftpd.conf', d)
        ConfManager.get().commit('proftpd')
