#-*- coding: utf-8 -*-
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

from netcontrolldap.netcontrolldap import *
from ConfigParser import ConfigParser

class Config(Plugin):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    implements(IConfigurable)
    name = 'Ldap Users'
    icon = '/dl/users_ldap/icon.png'
    id = 'myhosts'

    def list_files(self):
        return ['/etc/hosts']

    def __getCon(self):
        cfg = ConfigParser()
        cfg.read('/etc/ldap/netcontrol')
        server   = cfg.get('base', 'server')
        bindDN   = cfg.get('base', 'bindDN')
        adminPW  = cfg.get('base', 'adminPW')
        SAM = False
        try: SAM = bool(cfg.get('base', 'SAM'))
        except: pass

        del cfg
        con = LDAPConnection(server, ssl=True,
                    admPasswd=adminPW, baseDN=bindDN,
                    SAM=SAM)

        return con

    def getUserList(self):
        try:
            con = self.__getCon()
            
            ret = []
            for i in con.getUsers():
                ret.append(i[1])
            
            del con

            ret.sort( key=lambda x: x['cn'][0].lower()+x['uid'][0].lower() )
            return ret
        except: return []

    def getGroupList(self):
        try:
            con = self.__getCon()
            
            ret = []
            for i in con.getGroups():
                ret.append(i[1])
            
            del con
            return ret
        except: return []

    def editUser(self, userDN, fields):
        t = {'name':'cn',
             'surname':'sn', 'homeDirectory': 'homeDirectory',
             'address':'homePostalAddress', 'mail':'mail',
             'phone':'homePhone', 'loginShell': 'loginShell',
             'password':'userPassword'
             }
        change = {}
        for i in t.keys():
            if not userDN.has_key(t[i])   : userDN[t[i]] = ['']
            if not fields.has_key(i): fields[i] = ''
            if userDN[t[i]][0] <> fields[i]:
                change[t[i]] = fields[i]

        if change == {}: return
        if change.has_key('userPassword'):
            from passlib.hash import ldap_salted_sha1
            change['userPassword'] = \
                    ldap_salted_sha1.encrypt(
                            change['userPassword'])

        con = self.__getCon()
        for i in change.keys():
                con.modify("uid=%s" % userDN['uid'][0], i, change[i])

        del con

    def addUser(self, fields):
        con = self.__getCon()
        fields['createHome']    = True
        fields['createMailDir'] = True

        con.addUser(**fields)
        g = ['tty','disk','mail','news','proxy',
                 'dialout','fax','voice','cdrom',
                 'floppy','tape','audio','video',
                 'plugdev','games','users',
                 'ssh','messagebus','powerdev',
                 'scanner','saned']
        con.addUserToGroups(fields['login'], g)
        del con

    def delUser(self, user):
        #verificar se é o unico usuario do grupo
        #caso positivo, apagar também
        open('/tmp/backend','w').write(str(user)+'\n')
        con = self.__getCon()
        con.delUser( user['uid'][0] )
        g = con.searchGroup( user['uid'][0] )
        if g <> []:
            m = g[0][1]['memberUid']
            if len(m) == 0:
                con.delGroup( user['uid'][0] )
            elif len(m) == 1:
                if m[0] == user['uid'][0]:
                    con.delGroup( user['uid'][0] )

        del con

    def addGroup(self, name):
        con = self.__getCon()
        con.addGroup( name )
        del con

    def delGroup(self, group):
        con = self.__getCon()
        con.delGroup( group['cn'][0] )
        del con

    def getUG(self, user=None, group=None):
        con = self.__getCon()

        ret=[]
        if user is not None:
            g = con.getGroups()
            if g <> []:
                for i in g:
                    try:
                        i[1]['memberUid'].index(user)
                        ret.append( [ i[1]['cn'][0], True ] )
                    except:
                        ret.append( [ i[1]['cn'][0], False ]  )

        elif group is not None:
            u = con.getUsers()
            g = con.getGroup(group)
            if u <> [] and g <> []:
                for i in u:
                    try:
                        g[0][1]['memberUid'].index( i[1]['uid'][0] )
                        ret.append( [ i[1]['uid'][0], True ] )
                    except:
                        ret.append( [ i[1]['uid'][0], False ] )

        del con
        return ret

    # grupos do usuario
    def addUserGroups(self, user, groups):
        con = self.__getCon()
        con.addUserToGroups(user, groups)
        del con

    def delUserGroups(self, user, groups):
        con = self.__getCon()
        con.removeUserFromGroups(user, groups)
        del con

    # usuarios do grupo
    def addGroupUsers(self, group, users):
        con = self.__getCon()
        for i in users:
            con.addUserToGroups(i, group)
        del con

    def delGroupUsers(self, group, users):
        con = self.__getCon()
        for i in users:
            con.removeUserFromGroups(i, group)
        del con

