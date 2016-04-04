#-*- coding: utf-8 -*-

"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA

"""

import ldap
import templates

from tempfile import NamedTemporaryFile

fileSync = '/tmp/netcontrolLDAPControlUsers.pid'

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

class DefAttr:
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    __groups = "Groups"
    __users  = "Users"
    __imap   = "Idmap"
    __computers   = "Computers"
    __netcontrol = "/etc/ldap/netcontrol"
    __createMachineScript = "/var/lib/netcontrol/plugins/ldap_server/createMachine.py"
    __homeWinDir = "winfiles"
    __samba = '/etc/samba/smb.conf'

    def getGroups(self):
        return self.__groups

    def getUsers(self):
        return self.__users

    def getComputers(self):
        return self.__computers

    def getImap(self):
        return self.__imap

    def getIdmap(self):
        return self.__imap

    def getFluxControlFileName(self):
        return self.__netcontrol

    def getCreateMachineScript(self):
        return self.__createMachineScript
    
    def getHomeWinDir(self):
        return self.__homeWinDir
    
    def getSamba(self):
        return self.__samba

class LDAPConnection(ExecuteProgram):
    """
    Class for LDAP Connection
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    __LDAPServer = None
    __SSL        = None
    __baseDN     = None
    __adminDN    = None
    __adminPW    = None
    __port       = None
    __con        = None
    __SAM        = None

    def __init__( self, server, ssl=False, port=None,
                        baseDN=None, admName='admin',
                        admPasswd='123456', SAM=True ):
        '''
        Constructor:
           It defines the values for the connection,
           but it don't stabilish the connection here

        @param    server: server name or address (ex.: localhost)
        @param       ssl: use SSL conection      (True|False)
        @param      port: port to connect
        @param    baseDN: server common name     (ex.: "dc=localhost" or
                                                       "localhost" or None
                                                    to use the 'server' param )
        @param   admName: admin name             (ex.: Administrator)
        @param admPasswd: admin password         (ex.: Secret, not say )
        '''

        # Port and SSL
        if ssl:
            self.__SSL = True
            ssl = 's'
            if port is None: port='636'
        else:
            self.__SSL = False
            ssl = ''
            if port is None: port ='389'
        self.__port = str(port)

        # LDAP Server
        self.__LDAPServer = "ldap%s://%s:%s" % (ssl, server, self.__port)
        if baseDN is None: baseDN = server

        # base DN
        if baseDN.find('=') == -1:
            self.__baseDN = 'dc='+baseDN.replace('.',',dc=')
        else:
            self.__baseDN = baseDN

        # adminDN
        self.__adminDN = "cn=%s,%s" % ( admName, self.__baseDN )

        # adminPW
        self.__adminPW = str( admPasswd )
        
        # SAM
        self.__SAM = SAM

        # Others
        self.__con = None

    def __del__(self):
        """
        Destructor
        """
        self.closeConnection()

    def __str__(self):
        ret  =   'Server:         ' + self.__LDAPServer
        ret += '\nUse SSL:        ' + str(self.__SSL)
        ret += '\nBase DN:        ' + self.__baseDN
        ret += '\nAdmin DN:       ' + self.__adminDN
        ret += '\nUse SAM:        ' + str(self.__SAM)
        ret += '\nAdmin password: ' + ('*'*8)
        return ret

    def initConnection(self, idn=None, passwd=None):
        """
        Init connection with LDAP, if user or passwd is None,
            the default values (for admin) are used

        @param    idn: Init DN, as uid=jeiks or cn=admin
                       It was created to you connect as another
                               user different of admin
        @param passwd: password
        @return list: [ int, str ], where:
                        0 -> Success
                        1 -> Error Credentials
                        2 -> Another Error
        """
        # set DN
        if ( idn is not None ) and ( passwd is not None ):
            userDN = "%s,%s" % (idn, self.__baseDN)
            userPW = passwd
            self._userDN = userDN
            self._userPW = userPW
        else:
            userDN = self.__adminDN
            userPW = self.__adminPW

        if self.isConnected(): self.closeConnection()

        try:
            if self.__SSL: ldap.set_option( ldap.OPT_X_TLS_REQUIRE_CERT,
                                           ldap.OPT_X_TLS_ALLOW     )
            con   = ldap.initialize( self.__LDAPServer )
            if self.__SSL: con.protocol_version=ldap.VERSION3
            con.bind_s( userDN , userPW , ldap.AUTH_SIMPLE )
        except ldap.INVALID_CREDENTIALS:
            return [1,'Invalid Credentials']
        except ldap.LDAPError:
            return [2, 'Connection Error']

        self.__con = con
        return [0, 'Success']

    def isConnected(self):
        """
        Verify if the connection is stabilished

        @return: True if the connection is estabilished
                 False if not
        """
        if self.__con is None: return False
        else: return True

    def __getCon(self):
        """
        (Design Pattern) Singleton

        To get the actual connection, use this module
        @return: the actual connection
        """
        if not self.isConnected():
            self.initConnection()

        return self.__con

    def closeConnection(self):
        """
        Close bind connection
        """
        try:
            self.__con.unbind()
            self.__con = None
        except: pass

        return True

    def useSAM(self):
        return self.__SAM

    def search(self, idn='', filter='(objectClass=*)', attrs=None):
        """
        Search elements in LDAP Base

        @param    idn: Init DN.
                       Ex: uid=jeiks or cn=admin.
        @param filter: Any person objects.
                       Ex: (objectclass=posixAccount)
                       It was develop based on RFC 2254
        @param  attrs: list with attributes to get from
        """
        if (type(attrs) <> list) and (attrs <> None):
            print 'search: attrs must be a list or None'
            return []
        

        if filter.find('(') == -1:
            filter = '('+filter+')'

        try:
            if self.useSAM():
                if idn.find("ou") == -1 and len(idn.split('=')) == 2:
                    if   idn.find('uid') <> -1:
                        #find uid
                        idn = '%s,ou=%s' % (idn,DefAttr().getUsers())
                    elif idn.find('cn') <> -1:
                        #find cn
                        idn = '%s,ou=%s' % (idn,DefAttr().getGroups())

            query = idn
            if idn.find(self.__baseDN) == -1:
                query = "%s,%s" % ( idn, self.__baseDN)
                if query[0] == ',': query = query.replace(',','',1)

            return self.__getCon().search_s( query ,
                                             ldap.SCOPE_SUBTREE, filter, attrs )
        except:
            return []

    def getUsers(self):
        """
        Return all LDAP users
        """
        uids = self.search(
                filter='(objectClass=posixAccount)',
                attrs=['*'])

        return uids

    def getUser(self, user):
        """
        Same as 'searchUser' method

        @param user: user name
        """
        return self.searchUser( user )
        
    def findUser(self, user):
        """
        Same as 'searchUser' method

        @param user: user name
        """
        return self.searchUser( user )

    def searchUser(self, user):
        """
        Search a specific user in LDAP Base

        @param user: user name
        @return: a dict with all user's attributes
        """
        if user.find('=') == -1:
            user = 'uid='+user
        return self.search( idn=user,
               filter='(|(objectClass=posixAccount)(objectClass=account))' )

    def getGroups(self):
        """
        Return all LDAP groups
        """
        gids = self.search(
                filter='(objectClass=posixGroup)',
                attrs=['*']
                )
        return gids

    def getGroup(self, group):
        """
        Same as 'searchGroup' method

        @param group: user name
        """
        return self.searchGroup( group )

    def findGroup(self, group):
        """
        Same as 'searchGroup' method

        @param group: user name
        """
        return self.searchGroup( group )

    def searchGroup(self, group):
        """
        Search a specific group in LDAP Base

        @param group: group name
        @return: a dict with all group's attributes
        """
        if group.find('=') == -1:
            group = 'cn='+group
        return self.search( idn=group,
                            filter='(objectClass=posixGroup)')

    def changeUserPasswd(self, user, oldPasswd, newPasswd):
        """
        This method change the user password behind the old password
        Authentication.
        It can be used to verify if the user is the authentic user

        @param      user: user login
        @param oldPasswd: old user password
        @param newPasswd: new user password
        @return a list: [ int, msg ], where
                         0 -> Success
                         1 -> Invalid User
                         2 -> Invalid password
        """
        user = user.split('=')[-1]
        if self.searchUser( user ) == []:
            return [1, 'Invalid User']
        
        # setting the connection attributes based at this instance
        s = self.__LDAPServer.split('/')[-1].split(':')[0]
        tmp = LDAPConnection( server=s, ssl=self.__SSL,
                              port=self.__port, baseDN=self.__baseDN )
        
        # Starting the connection with the user profile
        if self.useSAM():
            idn = 'uid=%s,ou=%s' % (user,DefAttr().getUsers())
        else:
            idn = 'uid=%s' % user
        
        resp = tmp.initConnection( idn=idn , passwd=oldPasswd )
        if resp[0] <> 0:
            return [2, 'Invalid password']

        # Changing the password
        ret = tmp.changePasswd( newPasswd )
        tmp.closeConnection()

        del tmp
        return ret

    def changePasswd( self, newPasswd ):
        """
        Change the admin password

        @param newPasswd: new password
        """
        if self.__getCon().passwd_s( 
                self._userDN, self._userPW, newPasswd
                    )[0] == ldap.RES_EXTENDED:
            return [0, 'Success']

    def changeUserLogin( self, oldLogin, newLogin ):
        """
        Same as changeUserUid
        Change the user login

        @param oldLogin: user login name
        @param newLogin: new login name for the user
        @return: [ int , str ], where
                   0 -> Sucess
                   1 -> Invalid User
                   2 -> Unknown Error
        """
        return self.changeUserUid( oldLogin, newLogin )

    def changeUserUid( self, oldUid, newUid ):
        """
        Change the user Uid (login)

        @param oldUid: name of the user
        @param newUid: new name for the user
        @return: [ int , str ], where
                   0 -> Sucess
                   1 -> Invalid User
                   2 -> Unknown Error
        """
        fullDN = "uid=%s,%s" % (oldUid, self.__baseDN)
        newRND = "uid=%s" % newUid

        if self.searchUser( oldUid ) == []:
            return [ 1, 'Invalid User' ]
        
        try:
            self.__getCon().modrdn_s( fullDN, newRND, True )
            return [ 0, 'Success' ]
        except:
            return [ 2, 'Unknown Error' ]

    def changeGroupName( self, oldName, newName ):
        """
        Same as changeGroupCN
        Change the group name
        
        @param oldName: group name
        @param newName: new group name
        """
        return self.changeGroupCN( oldName, newName )

    def changeGroupCN( self, oldCN, newCN ):
        """
        Change the specific CN in LDAP Base

        @param oldCN: group Common Name
        @param newCN: new Common Name
        """
        fullDN = "cn=%s,%s" % (oldCN, self.__baseDN)
        newRND = "cn=%s" % newCN

        if self.searchGroup( oldCN ) == []:
            return [ 1, 'Invalid identifier' ]

        try:
            self.__getCon().modrdn_s( fullDN, newRND, True )
            return [ 0, 'Success' ]
        except:
            return [ 2, 'Unknown Error' ]

    def createAttrsList( self, attrName, attrValues, actualQuery=None ):
        """
        Create Attributes List to use with 'addAttribute' method

        @param    attrName: attribute name
        @param  attrValues: values to be added in attrName
        @param actualQuery: old return of this method.
                            Specify this param to add new
                            attributes and/or values
        """
        if type( attrValues ) <> list:
            attrValues = [ str(attrValues) ]

        if actualQuery is None:
            actualQuery = []

        try:
            actualQuery.index( (attrName, attrValues) )
            return actualQuery
        except:
            wasFound = False
            for pos in range(len(actualQuery)):
                n,v = actualQuery[pos]
                if n == attrName:
                    for value in attrValues:
                        try:    v.index( value )
                        except: v.append( value )
                    actualQuery[pos] = n,v
                    wasFound = True
                    break

            if not wasFound:
                actualQuery.append( (attrName, attrValues) )

        return actualQuery

    def addNewObject( self, idn, attrs ):
        """
        This method add a NEW entry (DN) in LDAP Server
        with the attributes from attrs param.

        @param idn: init DN to concatenate with baseDN
        @param attrs: list of attributes to add in this new DN
        PS: Use 'createAttrsList' method to create 'attrs' list
        """
        dn = "%s,%s" % (idn, self.__baseDN)

        if self.search( idn ) <> []:
            return [ 1 , 'This dn already exists, use modify' ]

        try:
            self.__getCon().add_s( dn , attrs )
            return [ 0, 'Success' ]
        except ldap.LDAPError, e:
            try:    msg = e[0]['info']
            except: msg = ''
            return [ 2, 'Error adding new values [%s]' % msg ]

    def addSAMSchema(self):
        from ConfigParser import ConfigParser
        cfg = ConfigParser()
        cfg.read('/etc/ldap/netcontrol')
        adminPW = cfg.get('base', 'adminPW')
        del cfg

        tmp = LDAPConnection( server='localhost', ssl=False, baseDN='cn=config',
                              admName='admin', admPasswd=adminPW)
        ret = tmp.addNewObject('cn=samba,cn=schema', [
              ('objectClass',[ 'olcSchemaConfig' ]),
              ('cn', ['samba']),
              ('olcAttributeTypes', [
                "{0}( 1.3.6.1.4.1.7165.2.1.24 NAME 'sambaLMPassword' DESC "+\
                    "'LanManager Password' EQUALITY caseIgnoreIA5Match SYN"+\
                    "TAX 1.3.6.1.4.1.1466.115.121.1.26{32} SINGLE-VALUE )",
                "{1}( 1.3.6.1.4.1.7165.2.1.25 NAME 'sambaNTPassword' DESC "+\
                    "'MD4 hash of the unicode password' EQUALITY caseIgnor"+\
                    "eIA5Match SYNTAX 1.3.6.1.4.1.1466.115.121.1.26{32} SI"+\
                    "NGLE-VALUE )",
                "{2}( 1.3.6.1.4.1.7165.2.1.26 NAME 'sambaAcctFlags' DESC '"+\
                    "Account Flags' EQUALITY caseIgnoreIA5Match SYNTAX 1.3"+\
                    ".6.1.4.1.1466.115.121.1.26{16} SINGLE-VALUE )",
                "{3}( 1.3.6.1.4.1.7165.2.1.27 NAME 'sambaPwdLastSet' DESC "+\
                    "'Timestamp of the last password update' EQUALITY inte"+\
                    "gerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-"+\
                    "VALUE )",
                "{4}( 1.3.6.1.4.1.7165.2.1.28 NAME 'sambaPwdCanChange' DES"+\
                    "C 'Timestamp of when the user is allowed to update th"+\
                    "e password' EQUALITY integerMatch SYNTAX 1.3.6.1.4.1."+\
                    "1466.115.121.1.27 SINGLE-VALUE )",
                "{5}( 1.3.6.1.4.1.7165.2.1.29 NAME 'sambaPwdMustChange' DE"+\
                    "SC 'Timestamp of when the password will expire' EQUAL"+\
                    "ITY integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27"+\
                    " SINGLE-VALUE )",
                "{6}( 1.3.6.1.4.1.7165.2.1.30 NAME 'sambaLogonTime' DESC '"+\
                    "Timestamp of last logon' EQUALITY integerMatch SYNTAX"+\
                    " 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{7}( 1.3.6.1.4.1.7165.2.1.31 NAME 'sambaLogoffTime' DESC "+\
                    "'Timestamp of last logoff' EQUALITY integerMatch SYNT"+\
                    "AX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{8}( 1.3.6.1.4.1.7165.2.1.32 NAME 'sambaKickoffTime' DESC"+\
                    " 'Timestamp of when the user will be logged off autom"+\
                    "atically' EQUALITY integerMatch SYNTAX 1.3.6.1.4.1.14"+\
                    "66.115.121.1.27 SINGLE-VALUE )",
                "{9}( 1.3.6.1.4.1.7165.2.1.48 NAME 'sambaBadPasswordCount'"+\
                    " DESC 'Bad password attempt count' EQUALITY integerMa"+\
                    "tch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE"+\
                    " )",
                "{10}( 1.3.6.1.4.1.7165.2.1.49 NAME 'sambaBadPasswordTime'"+\
                    " DESC 'Time of the last bad password attempt' EQUALIT"+\
                    "Y integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 S"+\
                    "INGLE-VALUE )",
                "{11}( 1.3.6.1.4.1.7165.2.1.55 NAME 'sambaLogonHours' DESC"+\
                    " 'Logon Hours' EQUALITY caseIgnoreIA5Match SYNTAX 1.3"+\
                    ".6.1.4.1.1466.115.121.1.26{42} SINGLE-VALUE )",
                "{12}( 1.3.6.1.4.1.7165.2.1.33 NAME 'sambaHomeDrive' DESC "+\
                    "'Driver letter of home directory mapping' EQUALITY ca"+\
                    "seIgnoreIA5Match SYNTAX 1.3.6.1.4.1.1466.115.121.1.26"+\
                    "{4} SINGLE-VALUE )",
                "{13}( 1.3.6.1.4.1.7165.2.1.34 NAME 'sambaLogonScript' DES"+\
                    "C 'Logon script path' EQUALITY caseIgnoreMatch SYNTAX"+\
                    " 1.3.6.1.4.1.1466.115.121.1.15{255} SINGLE-VALUE )",
                "{14}( 1.3.6.1.4.1.7165.2.1.35 NAME 'sambaProfilePath' DES"+\
                    "C 'Roaming profile path' EQUALITY caseIgnoreMatch SYN"+\
                    "TAX 1.3.6.1.4.1.1466.115.121.1.15{255} SINGLE-VALUE )",
                "{15}( 1.3.6.1.4.1.7165.2.1.36 NAME 'sambaUserWorkstations"+\
                    "' DESC 'List of user workstations the user is allowed"+\
                    " to logon to' EQUALITY caseIgnoreMatch SYNTAX 1.3.6.1"+\
                    ".4.1.1466.115.121.1.15{255} SINGLE-VALUE )",
                "{16}( 1.3.6.1.4.1.7165.2.1.37 NAME 'sambaHomePath' DESC '"+\
                    "Home directory UNC path' EQUALITY caseIgnoreMatch SYN"+\
                    "TAX 1.3.6.1.4.1.1466.115.121.1.15{128} )",
                "{17}( 1.3.6.1.4.1.7165.2.1.38 NAME 'sambaDomainName' DESC"+\
                    " 'Windows NT domain to which the user belongs' EQUALI"+\
                    "TY caseIgnoreMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1."+\
                    "15{128} )",
                "{18}( 1.3.6.1.4.1.7165.2.1.47 NAME 'sambaMungedDial' DESC"+\
                    " 'Base64 encoded user parameter string' EQUALITY case"+\
                    "ExactMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.15{1050}"+\
                    " )",
                "{19}( 1.3.6.1.4.1.7165.2.1.54 NAME 'sambaPasswordHistory'"+\
                    " DESC 'Concatenated MD5 hashes of the salted NT passw"+\
                    "ords used on this account' EQUALITY caseIgnoreIA5Matc"+\
                    "h SYNTAX 1.3.6.1.4.1.1466.115.121.1.26{32} )",
                "{20}( 1.3.6.1.4.1.7165.2.1.20 NAME 'sambaSID' DESC 'Secur"+\
                    "ity ID' EQUALITY caseIgnoreIA5Match SUBSTR caseExactI"+\
                    "A5SubstringsMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.2"+\
                    "6{64} SINGLE-VALUE )",
                "{21}( 1.3.6.1.4.1.7165.2.1.23 NAME 'sambaPrimaryGroupSID'"+\
                    " DESC 'Primary Group Security ID' EQUALITY caseIgnore"+\
                    "IA5Match SYNTAX 1.3.6.1.4.1.1466.115.121.1.26{64} SIN"+\
                    "GLE-VALUE )",
                "{22}( 1.3.6.1.4.1.7165.2.1.51 NAME 'sambaSIDList' DESC 'S"+\
                    "ecurity ID List' EQUALITY caseIgnoreIA5Match SYNTAX 1"+\
                    ".3.6.1.4.1.1466.115.121.1.26{64} )",
                "{23}( 1.3.6.1.4.1.7165.2.1.19 NAME 'sambaGroupType' DESC "+\
                    "'NT Group Type' EQUALITY integerMatch SYNTAX 1.3.6.1."+\
                    "4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{24}( 1.3.6.1.4.1.7165.2.1.21 NAME 'sambaNextUserRid' DES"+\
                    "C 'Next NT rid to give our for users' EQUALITY intege"+\
                    "rMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VA"+\
                    "LUE )",
                "{25}( 1.3.6.1.4.1.7165.2.1.22 NAME 'sambaNextGroupRid' DE"+\
                    "SC 'Next NT rid to give out for groups' EQUALITY inte"+\
                    "gerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-"+\
                    "VALUE )",
                "{26}( 1.3.6.1.4.1.7165.2.1.39 NAME 'sambaNextRid' DESC 'N"+\
                    "ext NT rid to give out for anything' EQUALITY integer"+\
                    "Match SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VAL"+\
                    "UE )",
                "{27}( 1.3.6.1.4.1.7165.2.1.40 NAME 'sambaAlgorithmicRidBa"+\
                    "se' DESC 'Base at which the samba RID generation algo"+\
                    "rithm should operate' EQUALITY integerMatch SYNTAX 1."+\
                    "3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{28}( 1.3.6.1.4.1.7165.2.1.41 NAME 'sambaShareName' DESC "+\
                    "'Share Name' EQUALITY caseIgnoreMatch SYNTAX 1.3.6.1."+\
                    "4.1.1466.115.121.1.15 SINGLE-VALUE )",
                "{29}( 1.3.6.1.4.1.7165.2.1.42 NAME 'sambaOptionName' DESC"+\
                    " 'Option Name' EQUALITY caseIgnoreMatch SUBSTR caseIg"+\
                    "noreSubstringsMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1"+\
                    ".15{256} )",
                "{30}( 1.3.6.1.4.1.7165.2.1.43 NAME 'sambaBoolOption' DESC"+\
                    " 'A boolean option' EQUALITY booleanMatch SYNTAX 1.3."+\
                    "6.1.4.1.1466.115.121.1.7 SINGLE-VALUE )",
                "{31}( 1.3.6.1.4.1.7165.2.1.44 NAME 'sambaIntegerOption' D"+\
                    "ESC 'An integer option' EQUALITY integerMatch SYNTAX "+\
                    "1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{32}( 1.3.6.1.4.1.7165.2.1.45 NAME 'sambaStringOption' DE"+\
                    "SC 'A string option' EQUALITY caseExactIA5Match SYNTA"+\
                    "X 1.3.6.1.4.1.1466.115.121.1.26 SINGLE-VALUE )",
                "{33}( 1.3.6.1.4.1.7165.2.1.46 NAME 'sambaStringListOption"+\
                    "' DESC 'A string list option' EQUALITY caseIgnoreMatc"+\
                    "h SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
                "{34}( 1.3.6.1.4.1.7165.2.1.53 NAME 'sambaTrustFlags' DESC"+\
                    " 'Trust Password Flags' EQUALITY caseIgnoreIA5Match S"+\
                    "YNTAX 1.3.6.1.4.1.1466.115.121.1.26 )",
                "{35}( 1.3.6.1.4.1.7165.2.1.58 NAME 'sambaMinPwdLength' DE"+\
                    "SC 'Minimal password length (default: 5)' EQUALITY in"+\
                    "tegerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGL"+\
                    "E-VALUE )",
                "{36}( 1.3.6.1.4.1.7165.2.1.59 NAME 'sambaPwdHistoryLength"+\
                    "' DESC 'Length of Password History Entries (default: "+\
                    "0 => off)' EQUALITY integerMatch SYNTAX 1.3.6.1.4.1.1"+\
                    "466.115.121.1.27 SINGLE-VALUE )",
                "{37}( 1.3.6.1.4.1.7165.2.1.60 NAME 'sambaLogonToChgPwd' D"+\
                    "ESC 'Force Users to logon for password change (defaul"+\
                    "t: 0 => off, 2 => on)' EQUALITY integerMatch SYNTAX 1"+\
                    ".3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{38}( 1.3.6.1.4.1.7165.2.1.61 NAME 'sambaMaxPwdAge' DESC "+\
                    "'Maximum password age, in seconds (default: -1 => nev"+\
                    "er expire passwords)' EQUALITY integerMatch SYNTAX 1."+\
                    "3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{39}( 1.3.6.1.4.1.7165.2.1.62 NAME 'sambaMinPwdAge' DESC "+\
                    "'Minimum password age, in seconds (default: 0 => allo"+\
                    "w immediate password change)' EQUALITY integerMatch S"+\
                    "YNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{40}( 1.3.6.1.4.1.7165.2.1.63 NAME 'sambaLockoutDuration'"+\
                    " DESC 'Lockout duration in minutes (default: 30, -1 ="+\
                    "> forever)' EQUALITY integerMatch SYNTAX 1.3.6.1.4.1."+\
                    "1466.115.121.1.27 SINGLE-VALUE )",
                "{41}( 1.3.6.1.4.1.7165.2.1.64 NAME 'sambaLockoutObservati"+\
                    "onWindow' DESC 'Reset time after lockout in minutes ("+\
                    "default: 30)' EQUALITY integerMatch SYNTAX 1.3.6.1.4."+\
                    "1.1466.115.121.1.27 SINGLE-VALUE )",
                "{42}( 1.3.6.1.4.1.7165.2.1.65 NAME 'sambaLockoutThreshold"+\
                    "' DESC 'Lockout users after bad logon attempts (defau"+\
                    "lt: 0 => off)' EQUALITY integerMatch SYNTAX 1.3.6.1.4"+\
                    ".1.1466.115.121.1.27 SINGLE-VALUE )",
                "{43}( 1.3.6.1.4.1.7165.2.1.66 NAME 'sambaForceLogoff' DES"+\
                    "C 'Disconnect Users outside logon hours (default: -1 "+\
                    "=> off, 0 => on)' EQUALITY integerMatch SYNTAX 1.3.6."+\
                    "1.4.1.1466.115.121.1.27 SINGLE-VALUE )",
                "{44}( 1.3.6.1.4.1.7165.2.1.67 NAME 'sambaRefuseMachinePwd"+\
                    "Change' DESC 'Allow Machine Password changes (default"+\
                    ": 0 => off)' EQUALITY integerMatch SYNTAX 1.3.6.1.4.1"+\
                    ".1466.115.121.1.27 SINGLE-VALUE )",
                "{45}( 1.3.6.1.4.1.7165.2.1.68 NAME 'sambaClearTextPasswor"+\
                    "d' DESC 'Clear text password (used for trusted domain"+\
                    " passwords)' EQUALITY octetStringMatch SYNTAX 1.3.6.1"+\
                    ".4.1.1466.115.121.1.40 )",
                "{46}( 1.3.6.1.4.1.7165.2.1.69 NAME 'sambaPreviousClearTex"+\
                    "tPassword' DESC 'Previous clear text password (used f"+\
                    "or trusted domain passwords)' EQUALITY octetStringMat"+\
                    "ch SYNTAX 1.3.6.1.4.1.1466.115.121.1.40 )",
                ]),
            ('olcObjectClasses', [
                "{0}( 1.3.6.1.4.1.7165.2.2.6 NAME 'sambaSamAccount' DESC '"+\
                        "Samba 3.0 Auxilary SAM Account' SUP top AUXILIARY"+\
                        " MUST ( uid $ sambaSID ) MAY ( cn $ sambaLMPasswo"+\
                        "rd $ sambaNTPassword $ sambaPwdLastSet $ sambaLog"+\
                        "onTime $ sambaLogoffTime $ sambaKickoffTime $ sam"+\
                        "baPwdCanChange $ sambaPwdMustChange $ sambaAcctFl"+\
                        "ags $ displayName $ sambaHomePath $ sambaHomeDriv"+\
                        "e $ sambaLogonScript $ sambaProfilePath $ descrip"+\
                        "tion $ sambaUserWorkstations $ sambaPrimaryGroupS"+\
                        "ID $ sambaDomainName $ sambaMungedDial $ sambaBad"+\
                        "PasswordCount $ sambaBadPasswordTime $ sambaPassw"+\
                        "ordHistory $ sambaLogonHours ) )",
                "{1}( 1.3.6.1.4.1.7165.2.2.4 NAME 'sambaGroupMapping' DESC"+\
                        " 'Samba Group Mapping' SUP top AUXILIARY MUST ( g"+\
                        "idNumber $ sambaSID $ sambaGroupType ) MAY ( disp"+\
                        "layName $ description $ sambaSIDList ) )",
                "{2}( 1.3.6.1.4.1.7165.2.2.14 NAME 'sambaTrustPassword' DE"+\
                        "SC 'Samba Trust Password' SUP top STRUCTURAL MUST"+\
                        " ( sambaDomainName $ sambaNTPassword $ sambaTrust"+\
                        "Flags ) MAY ( sambaSID $ sambaPwdLastSet ) )",
                "{3}( 1.3.6.1.4.1.7165.2.2.15 NAME 'sambaTrustedDomainPass"+\
                        "word' DESC 'Samba Trusted Domain Password' SUP to"+\
                        "p STRUCTURAL MUST ( sambaDomainName $ sambaSID $ "+\
                        "sambaClearTextPassword $ sambaPwdLastSet ) MAY sa"+\
                        "mbaPreviousClearTextPassword )",
                "{4}( 1.3.6.1.4.1.7165.2.2.5 NAME 'sambaDomain' DESC 'Samb"+\
                        "a Domain Information' SUP top STRUCTURAL MUST ( s"+\
                        "ambaDomainName $ sambaSID ) MAY ( sambaNextRid $ "+\
                        "sambaNextGroupRid $ sambaNextUserRid $ sambaAlgor"+\
                        "ithmicRidBase $ sambaMinPwdLength $ sambaPwdHisto"+\
                        "ryLength $ sambaLogonToChgPwd $ sambaMaxPwdAge $ "+\
                        "sambaMinPwdAge $ sambaLockoutDuration $ sambaLock"+\
                        "outObservationWindow $ sambaLockoutThreshold $ sa"+\
                        "mbaForceLogoff $ sambaRefuseMachinePwdChange ) )",
                "{5}( 1.3.6.1.4.1.7165.2.2.7 NAME 'sambaUnixIdPool' DESC '"+\
                        "Pool for allocating UNIX uids/gids' SUP top AUXIL"+\
                        "IARY MUST ( uidNumber $ gidNumber ) )",
                "{6}( 1.3.6.1.4.1.7165.2.2.8 NAME 'sambaIdmapEntry' DESC '"+\
                        "Mapping from a SID to an ID' SUP top AUXILIARY MU"+\
                        "ST sambaSID MAY ( uidNumber $ gidNumber ) )",
                "{7}( 1.3.6.1.4.1.7165.2.2.9 NAME 'sambaSidEntry' DESC 'St"+\
                        "ructural Class for a SID' SUP top STRUCTURAL MUST"+\
                        " sambaSID )",
                "{8}( 1.3.6.1.4.1.7165.2.2.10 NAME 'sambaConfig' DESC 'Sam"+\
                        "ba Configuration Section' SUP top AUXILIARY MAY d"+\
                        "escription )",
                "{9}( 1.3.6.1.4.1.7165.2.2.11 NAME 'sambaShare' DESC 'Samb"+\
                        "a Share Section' SUP top STRUCTURAL MUST sambaSha"+\
                        "reName MAY description )",
                "{10}( 1.3.6.1.4.1.7165.2.2.12 NAME 'sambaConfigOption' DE"+\
                        "SC 'Samba Configuration Option' SUP top STRUCTURA"+\
                        "L MUST sambaOptionName MAY ( sambaBoolOption $ sa"+\
                        "mbaIntegerOption $ sambaStringOption $ sambaStrin"+\
                        "gListoption $ description ) )",
                    ])
            ])
        del tmp
        
        self.setSAMDomainName()
        return ret

    def setSAMDomainName(self):
        self._execute('/usr/bin/net getlocalsid')

    def getSAMSID(self):
        try:
            return self.search(filter='(objectClass=sambaDomain)')[0][1]['sambaSID'][0]
        except:
            self.setSAMDomainName()
            from time import sleep
            sleep(5)
            try:
                return self.search(filter='(objectClass=sambaDomain)')[0][1]['sambaSID'][0]
            except:
                return ['']

    def getSAMDomain(self):
        return self.search(filter='(objectClass=sambaDomain)')[0][1]['sambaDomainName'][0]

    def addOUUsers(self):
        return self.addOUPeople()

    def addOUPeople(self):
        aL = None
        for i in [ 'organizationalUnit', 'top',  ]:
            aL = self.createAttrsList( 'objectClass', i, aL )

        aL = self.createAttrsList('ou', DefAttr().getUsers(), aL )

        return self.addNewObject( 'ou=%s' % DefAttr().getUsers(), aL )

    def addOUGroups(self):
        aL = None
        for i in [ 'organizationalUnit', 'top' ]:
            aL = self.createAttrsList( 'objectClass', i, aL )

        aL = self.createAttrsList('ou', DefAttr().getGroups(), aL )

        return self.addNewObject( 'ou=%s' % DefAttr().getGroups(), aL )

    def addOUComputers(self):
        aL = None
        for i in [ 'organizationalUnit', 'top' ]:
            aL = self.createAttrsList( 'objectClass', i, aL )

        aL = self.createAttrsList('ou', DefAttr().getComputers(), aL )

        return self.addNewObject( 'ou=%s' % DefAttr().getComputers(), aL )

    def addDomainGroup(self, name, description, sambaSID,
                             sambaGroupType, gidNumber, cn=None, displayName=None,
                             extraObjectClass=None):

        if extraObjectClass is None: extraObjectClass = []
        elif type(extraObjectClass) <> tuple and type(extraObjectClass) <> list:
            extraObjectClass = [ extraObjectClass ]

        if cn          is None: cn          = name
        if displayName is None: displayName = name

        aL = None
        for i in ['posixGroup', 'top', 'sambaGroupMapping']+extraObjectClass:
            aL = self.createAttrsList( 'objectClass', i, aL )

        aL = self.createAttrsList( 'sambaGroupType', 
                                    sambaGroupType, aL )

        aL = self.createAttrsList('cn', cn, aL )
        aL = self.createAttrsList('description', description, aL)
        aL = self.createAttrsList('displayName', displayName, aL)
        aL = self.createAttrsList('gidNumber'  , gidNumber, aL)
        aL = self.createAttrsList('sambaSID'   , sambaSID, aL)
        try:
            if self.addNewObject( 'cn=%s,ou=Groups'%name, aL )[0] <> 2:
                return [ 0, 'Success' ]
        except:
            return [ 2, 'Error creating Domain Groups' ]


    def addAllDomainGroups(self):
        sambaSID = self.getSAMSID()
        ret = {}
        ret[0] = self.addDomainGroup('Domain Admins'   , 'Netbios Domain Administrators',
                            sambaSID=sambaSID+'-512', sambaGroupType='2', gidNumber='512')
        ret[1] = self.addDomainGroup('Domain Users'    , 'Netbios Domain Users',
                            sambaSID=sambaSID+'-513', sambaGroupType='2', gidNumber='513')
        ret[2] = self.addDomainGroup('Domain Guests'   , 'Netbios Domain Guests',
                            sambaSID=sambaSID+'-514', sambaGroupType='2', gidNumber='514')
        ret[3] = self.addDomainGroup('Domain Computers', 'Netbios Domain Computers',
                            sambaSID=sambaSID+'-515', sambaGroupType='2', gidNumber='515')
        ret[4] = self.addDomainGroup('Administrators', 'Netbios Administers to computer/sambaDomainName',
                            sambaSID='S-1-5-32-544', sambaGroupType='5', gidNumber='544')
        ret[5] = self.addDomainGroup('Account Operators', 'Netbios Members to manipulate user accounts',
                            sambaSID='S-1-5-32-548', sambaGroupType='5', gidNumber='548')
        problem = False
        for i in ret.keys():
            if ret[i] <> 0: problem = True

        if not problem:
            return [ 0 , 'Success' ]
        else:
            return [ 1 , 'Problem creating domain groups' ]

    def addSAMOrgUnits(self):
        r1 = self.addOUPeople()
        r2 = self.addOUGroups()
        r3 = self.addOUComputers()
        r4 = self.addAllDomainGroups()
        
        if r1[0] == 1 and r2[0] == 1 and r3[0] == 1 and r4[0] == 0:
            return [ 0 , 'SAM already exists' ]
        
        if r1[0] == 0 and r2[0] == 0 and r3[0] == 0 and r4[0] == 0:
            return [ 0 , 'SAM created' ]
        
        return [ 1 , 'Error creating SAM Org. Units' ]

    def isSAM(self):
        r1 = self.search( 'ou=%s' % DefAttr().getUsers() )
        r2 = self.search( 'ou=%s' % DefAttr().getGroups() )
        r3 = self.search( 'ou=%s' % DefAttr().getComputers() )
        
        if r1 <> [] and r2 <> [] and r3 <> []:
            return [ 0 , 'SAM exists' ]
        
        if r1 == [] and r2 == [] and r3 == []:
            return [ 1 , 'SAM not exists' ]
        
        return [ 2 , 'Some Organizations of SAM exists' ]

    def setSAM(self):
        self.addSAMSchema()
        self.addSAMOrgUnits()
        return self.isSAM()

    def getNextGidNumber(self):
        """
        Get the next GID number after 1000

        @return the next GID number after 1000
        """
        gids = self.search(
                filter='(objectClass=posixGroup)',
                attrs=['gidNumber']
                )
        gidNumber = 1000
        try:
            group = open('/etc/group').readlines()
            group = map( lambda x: int(x.split(':')[2]) , group )
            group.sort()
            try: group.remove(65534)
            except:pass
            if gidNumber < group[-1]:
                gidNumber = group[-1] + 1
        except: pass

        try:
            for dn,gidN in gids:
                gidN = [int(x) for x in gidN['gidNumber']]
                gidN.sort()
                try: gidN.remove(65534)
                except: pass
                if gidN[-1] >= gidNumber:
                    #65534 is nogroup
                    gidNumber = gidN[-1]+1
        except: pass

        return gidNumber

    def addGroup( self, group, gidNumber=None, users=None ):
        """
        Add a new group in LDAP base

        @param     group: Group Name
        @param gidNumber: specify the Gid Number
        @param     users: list of users to be added to this group
        """
        if type(users) <> list and users <> None and type(users) <> tuple:
            users = [ users ]
        if gidNumber is None:
            gidNumber = str( self.getNextUidNumber() )

        #Setting attributes
        aL = None
        for i in [ 'posixGroup', 'top' ]:
            aL = self.createAttrsList( 'objectClass', i, aL )

        aL = self.createAttrsList(        'cn',     group, aL )
        aL = self.createAttrsList( 'gidNumber', gidNumber, aL )
        if users is not None:
            for i in users:
                aL = self.createAttrsList( 'memberUid', i, aL )

        if self.useSAM():
            return self.addNewObject( 'cn=%s,ou=%s' % (group,DefAttr().getGroups()), aL )

        return self.addNewObject( 'cn='+group, aL )

    def getNextUidNumber(self):
        """
        Get the next UID number after 1000
        
        @return the next UID number after 1000
        """
        uids = self.search(
                filter='(objectClass=posixAccount)',
                attrs=['uidNumber'])
        uidNumber = 1000
        try:
            passwd = open('/etc/passwd').readlines()
            passwd = map( lambda x: int(x.split(':')[2]) , passwd )
            passwd.sort()
            try: passwd.remove(65534)
            except:pass
            try:
                if uidNumber <= passwd[-1]:
                    uidNumber = passwd[-1] + 1
            except: pass
        except: pass
        try:
            for dn,uidN in uids:
                uidN = [int(x) for x in uidN['uidNumber']]
                uidN.sort()
                try: uidN.remove(65534)
                except: pass
                try:
                    if uidN[-1] >= uidNumber:
                        #65534 is nobody
                        uidNumber = uidN[-1]+1
                except: pass
        except: pass

        return uidNumber

    def addMachine(self, name):
        from time import time
        if not self.useSAM(): return [3, 'Server not defined to SAM']

        if name[-1] <> '$': name += '$'

        aL = None
        for i in [ 'posixAccount', 'account', 'sambaSamAccount' ]:
            aL = self.createAttrsList( 'objectClass', i, aL )
        
        aL = self.createAttrsList( 'uid'        , name , aL )
        aL = self.createAttrsList( 'cn'         , name , aL )
        aL = self.createAttrsList( 'displayName', name , aL )
        aL = self.createAttrsList( 'gecos', 'Computer' , aL )
        aL = self.createAttrsList( 'description', 'Computer', aL )
        aL = self.createAttrsList( 'sambaLogonTime', '0', aL )
        aL = self.createAttrsList( 'sambaLogoffTime', '2147483647', aL )
        aL = self.createAttrsList( 'sambaKickoffTime', '2147483647', aL )
        aL = self.createAttrsList( 'sambaPwdCanChange', '0', aL )
        aL = self.createAttrsList( 'sambaPwdMustChange', '2147483647', aL )
        aL = self.createAttrsList( 'sambaPwdLastSet', str(int(time())), aL )

        uidNumber = str( self.getNextUidNumber() )

        aL = self.createAttrsList( 'uidNumber', uidNumber, aL )
        aL = self.createAttrsList( 'gidNumber', '515', aL )
        aL = self.createAttrsList( 'homeDirectory', '/dev/null', aL )
        aL = self.createAttrsList( 'loginShell', '/bin/false', aL )

        sid = self.getSAMSID()
        if sid == []: return [ 2, 'Error' ]

        org = []

        aL = self.createAttrsList( 'sambaSID', '%s-%s' % (sid,uidNumber), aL )
        aL = self.createAttrsList( 'sambaPrimaryGroupSID', '%s-515' % sid, aL )
        aL = self.createAttrsList( 'sambaAcctFlags', '[W          ]', aL )
        aL = self.createAttrsList( 'sambaDomainName', self.getSAMDomain(), aL )

        org = [ DefAttr().getComputers(), 'Domain Computers' ]
        ret = self.addNewObject('uid=%s,ou=%s' % (name, org[0]), aL)
        
        self._execute("bash -c \"smbpasswd -sa %s < <(echo -e '%s\n%s')\"" % (name, '', ''))
        self._execute("smbpasswd -e %s" % name)

        self._execute( '/etc/init.d/winbind restart', daemon=True )
        self._execute( '/etc/init.d/nslcd restart', daemon=True )
        self._execute( '/etc/init.d/samba restart', daemon=True )

        return [ 0, 'Success' ]


    def addUser( self, login, name, passwd, surname=None,
                       uidNumber=None,      gidNumber=None,
                       userClass='aluno',   homeDirectory=None,
                       createHome=False,    address=None,
                       mail=None,           phone=None,
                       loginShell='/bin/bash', encPasswdWithSHA=True,
                       createMailDir=False, Computer=False):
        """
        Add a new user in LDAP Base
        
        @param      login: login name
        @param       name: Full user name
        @param     passwd: user password
        @param    surname: family name or nick
                           if None, the last name will be used for
        @param  uidNumber: the UID number
                           if None, the next uid value will be user for
        @param  gidNumber: the GID number
                           if None, the next uid value will be user for
        @param  userClass: user class
        @param homeDirectory: the path of user home directory
                              if None, will be used /home/<login>
        @param createHome: True to create the user home directory
        @param    address: user address
                           If None, nothing will be setted.
                           After this, you can use 'setAddress'
        @param      email: user eletronic mail
                           If None, nothing will be setted.
                           After this, you can use 'setEmail'
        @param      phone: user phone number
                           If None, nothing will be setted.
                           After this, you can use 'setPhone'
        @param loginShell: Login Shell, the default is '/bin/bash'
        @param encPasswdWithSHA: if  True, password will be encrypted with SHA
                                 if False, password will be plain
        """
        # attrsList
        
        if createMailDir: createHome=True
        
        aL = None
        for i in [ 'top', 'posixAccount', 'shadowAccount',
                   'inetOrgPerson', 'organizationalPerson',
                   'person' ]:
            aL = self.createAttrsList( 'objectClass', i, aL )
        
        login = login.replace(' ','_').replace('#','_')

        aL = self.createAttrsList( 'uid'  , login , aL )
        aL = self.createAttrsList( 'cn'   , name  , aL )
        aL = self.createAttrsList( 'gecos', name  , aL )
        plainPassword = passwd
        if encPasswdWithSHA:
            from passlib.hash import ldap_salted_sha1
            passwd = ldap_salted_sha1.encrypt(passwd)

        aL = self.createAttrsList( 'userPassword', passwd, aL )

        if surname is None: surname = name.split()[-1]
        aL = self.createAttrsList( 'sn', surname, aL )

        if uidNumber is None:
            uidNumber = self.getNextUidNumber()

        uidNumber=str(uidNumber)

        aL = self.createAttrsList( 'uidNumber', uidNumber, aL )

        if gidNumber is None:
            try:
                gidNumber = self.findGroup(login)[0][1]['gidNumber'][0]
            except:
                gidNumber = self.getNextGidNumber()
                self.addGroup( login, gidNumber=gidNumber, users=login )

        gidNumber = str(gidNumber)

        aL = self.createAttrsList( 'gidNumber', gidNumber, aL )

        if homeDirectory is None: homeDirectory = '/home/'+login
        aL = self.createAttrsList( 'homeDirectory', homeDirectory, aL )

        aL = self.createAttrsList( 'loginShell', loginShell, aL )

        # extra:
        #if userClass is not None:
        #    aL = self.createAttrsList( 'userClass', userClass, aL )
        if address   is not None:
            aL = self.createAttrsList( 'homePostalAddress', address, aL )
        if mail      is not None:
            aL = self.createAttrsList( 'mail', mail, aL )
        if phone     is not None:
            aL = self.createAttrsList( 'homePhone', phone, aL )

        ret = [ 2, 'Error' ]
        # If is SAM
        if self.useSAM():
            self.setSAM()
            sid = self.getSAMSID()
            if sid == []: return ret
            
            org = []
            if Computer:
                ### Adding a computer
                aL = self.createAttrsList( 'objectClass', 'sambaSamAccount', aL )
                aL = self.createAttrsList( 'sambaSID', '%s-%s' % (sid,uidNumber), aL )
                aL = self.createAttrsList( 'displayName', name, aL )
                aL = self.createAttrsList( 'sambaAcctFlags', '[W]', aL )
                
                org = [ DefAttr().getComputers(), 'Domain Computers' ]
                ret = self.addNewObject('uid=%s,ou=%s' % (login, org[0]), aL)
                self.addUserToGroups(login, org[1])
            else:
                ### Adding a user
                aL = self.createAttrsList( 'objectClass', 'sambaSamAccount', aL )
                aL = self.createAttrsList( 'sambaSID', '%s-%s' % (sid,uidNumber), aL )
                aL = self.createAttrsList( 'sambaPrimaryGroupSID', sid+'-513', aL )
                aL = self.createAttrsList( 'displayName', name, aL )
                aL = self.createAttrsList( 'givenName', login, aL )
                aL = self.createAttrsList( 'sambaAcctFlags', '[U]', aL )
                aL = self.createAttrsList( 'sambaHomeDrive', 'H:', aL )
                serverName = open('/etc/samba/smb.conf').read().split('netbios name')[1].split('=')[1].split('\n')[0].strip()
                sambaHomePath = "\\\\%s%s" % (  serverName,
                                                "".join( map( 
                                                     lambda x: '\\'+x,
                                                     homeDirectory.split('/')[2:] )
                                                   ))
                aL = self.createAttrsList( 'sambaHomePath', sambaHomePath, aL )
                
                org = [ DefAttr().getUsers(), 'Domain Users' ]

                ret = self.addNewObject('uid=%s,ou=%s' % (login, org[0]), aL)
                self.addUserToGroups(login, org[1])

            self._execute("bash -c \"smbpasswd -sa %s < <(echo -e '%s\n%s')\"" % (login, plainPassword, plainPassword))
            self._execute("smbpasswd -e %s" % login)
        else:
            ### Adding a user without SAM
            ret = self.addNewObject('uid='+login, aL)

        if createHome and ret[0] == 0:
            from os import makedirs, chown
            try:
                makedirs(homeDirectory, 0700)
            except OSError, e:
                if str(e).find('File exists') == -1:
                    print 'Error creating home Directory %s' % homeDirectory
            except: pass

            try:
                chown(homeDirectory, int(uidNumber), int(gidNumber))
            except:
                print 'Error setting permissions of %s' % homeDirectory

            if self.useSAM():
                try:
                   makedirs(homeDirectory+'/'+DefAttr().getHomeWinDir(), 0700)
                except: pass

                try:
                   chown(homeDirectory+'/'+DefAttr().getHomeWinDir(), int(uidNumber), int(gidNumber))
                except: pass

            if createMailDir:
                for maildir in ['/Maildir',
                                '/Maildir/cur',
                                '/Maildir/new',
                                '/Maildir/tmp']:
                    try:
                        makedirs(homeDirectory+maildir, 0700)
                    except: pass
                    try:
                        chown(homeDirectory+maildir, int(uidNumber), int(gidNumber))
                    except:
                        print 'Error setting permissions of %s' % homeDirectory+maildir
                  

        return ret

    def addUserToGroups(self, user, groups):
        """
        Add a user in a list of groups
        
        @param   user: user to add in groups
        @param groups: list of groups to add user.
                       it can be the group name or his GID
        """
        if type(groups) <> list and type(groups) <> tuple:
            groups = [groups]

        groups = [str(x) for x in groups]

        if self.search('uid=%s'%user) == []:
            return [1, 'user not found']

        groupNames = []
        for g in groups:
            try:
                g = int(g)
                grp = self.search(
                    filter=('(&(structuralObjectClass=posixGroup)(gidNumber=%d))'% g)
                                 )
                if grp == []:
                    return [ 2, ('group "%s" not found'%g) ]
                groupNames.append( grp[0][0].split(',')[0].split('cn=')[1] )
            except:
                groupNames.append( g )
        
        ret = []
        for g in groupNames:
            if self.useSAM():
                idn = 'cn=%s,ou=%s' % (g, DefAttr().getGroups())
            else:
                idn = 'cn=%s' % g
            if self.modify( idn, 'memberUid', user, forceAdd=True )[0] <> 0:
                ret.append(g)

        if ret == []:
            return [ 0, 'Success' ]
        else:
            return [ 3, 'Fail to add in group(s): %s' % str(ret) ]

    def delUserFromGroups(self, user, groups):
        """
        Same as removeUserFromGroups

        @param   user: user to remove from groups
        @param groups: list of groups to remove the user.
                       it can be the group name or his GID
        """
        return self.removeUserFromGroups(user, groups)

    def removeUserFromGroups(self, user, groups):
        """
        Remove a user from a list of groups
        
        @param   user: user to remove from groups
        @param groups: list of groups to remove the user.
                       it can be the group name or his GID
        """
        if type(groups) <> list and type(groups) <> tuple:
            groups = [groups]

        groups = [str(x) for x in groups]

        ret = []
        groupNames = []
        for g in groups:
            try:
                g = int(g)
                grp = self.search(
                    filter=('(&(structuralObjectClass=posixGroup)(gidNumber=%d))'% g)
                                 )
                if grp == []:
                    ret.append( 2, ('group "%s" not found'%g) )
                else:
                    groupNames.append( grp[0][0].split(',')[0].split('cn=')[1] )
            except:
                groupNames.append( g )
        
        for g in groupNames:
            if self.modify( 'cn=%s'%g, 'memberUid', user, delete=True )[0] <> 0:
                ret.append(g)

        if ret == []:
            return [ 0, 'Success' ]
        else:
            return [ 3, 'Fail to remove in group(s): %s' % str(ret) ]

    def delMachine(self, user):
        """
        Same as 'removeUser'

        Remove a machine from LDAP Base
        
        @param       user: user login to remove
        @param removeHome: True to remove the user home directory
        """
        return self.removeMachine( user)

    def removeMachine(self, user):
        """
        Remove a machine from LDAP Base
        
        @param       user: user login to remove
        @param removeHome: True to remove the user home directory
        """
        if user[-1] <> '$': user+='$'
        if user.startswith('uid='):
            user = user.split(',')[0].split('=')[-1]

        if self.useSAM():
            user = 'uid=%s,ou=%s' % ( user , DefAttr().getComputers() )
        else:
            user = 'uid=%s' % user

        try:
            if self.useSAM():
                self._execute( "smbpasswd -d %s" % user )

            self.__getCon().delete_s( '%s,%s' % (user, self.__baseDN) )
            self.removeUserFromGroups( user, ['Domain Users'] )
            self.removeUserFromGroups( user, ['Domain Computers'] )
            return [ 0 , 'Success' ]
        except:
            return [ 1 , 'An error removing user %s' % user ]

    def delUser(self, user, removeHome=False):
        """
        Same as 'removeUser'

        Remove a user from LDAP Base
        
        @param       user: user login to remove
        @param removeHome: True to remove the user home directory
        """
        return self.removeUser( user, removeHome )

    def removeUser(self, user, removeHome=False):
        """
        Remove a user from LDAP Base
        
        @param       user: user login to remove
        @param removeHome: True to remove the user home directory
        """
        if user.startswith('uid='):
            user = user.split(',')[0].split('=')[-1]

        if self.useSAM():
            user = 'uid=%s,ou=%s' % ( user , DefAttr().getUsers() )
        else:
            user = 'uid=%s' % user

        if removeHome:
            try:
                homeDir = self.findUser(user)[0][1]['homeDirectory'][0]
                from os import chmod
                from shutil import rmtree
                chmod(homeDir, 0777)
                rmtree(homeDir, ignore_errors=True)
            except: pass

        try:
            if self.useSAM():
                self._execute( "smbpasswd -d %s" % user )

            self.__getCon().delete_s( '%s,%s' % (user, self.__baseDN) )
            self.removeUserFromGroups( user, ['Domain Users'] )
            self.removeUserFromGroups( user, ['Domain Computers'] )
            return [ 0 , 'Success' ]
        except:
            return [ 1 , 'An error removing user %s' % user ]

    def delGroup(self, group):
        """
        Same as 'removeGroup'
        Remove a group from LDAP Base
        
        @param group: group name to remove
        """
        return self.removeGroup( group )

    def removeGroup(self, group):
        """
        Remove a group from LDAP Base
        
        @param group: group name to remove
        """
        if group.startswith('cn='):
            group = group.split(',')[0].split('=')[-1]

        try:
            if self.useSAM():
                group = 'cn=%s,ou=%s' % (group,DefAttr().getGroups())
            else:
                group = 'cn=%s' % group
            self.__getCon().delete_s( '%s,%s' % (group, self.__baseDN) )
            return [ 0 , 'Success' ]
        except:
            return [ 1 , 'An error removing group %s' % group ]

    def setEmail(self, user, email):
        """
        Set user eletronic mail

        @param  user: user login
        @param email: email to be setted
        """
        return self.modify('uid='+user, 'mail', email)

    def setClass(self, user, userClass):
        """
        Set user class

        @param  user: user login
        @param email: class to be setted
        """
        return self.modify('uid='+user, 'userClass', userClass)

    def setAddress(self, user, address):
        """
        Set user address

        @param  user: user login
        @param email: address to be setted
        """
        return self.modify('uid='+user, 'homePostalAddress', address)

    def setPhone(self, user, phone):
        """
        Set user Phone Number

        @param  user: user login
        @param email: Phone Number to be setted
        """
        return self.modify('uid='+user, 'homePhone', phone)

    def setUserPassword(self, user, passwd, encPasswdWithSHA=True):
        """
        Set user class

        @param   user: user login
        @param passwd: new password to user
        @param encPasswdWithSHA: True to user encrypt password with SHA
                                 False to set plain password
        """
        if encPasswdWithSHA:
            from passlib.hash import ldap_salted_sha1
            password = ldap_salted_sha1.encrypt(password)
        return self.modify('uid='+user, 'userPassword', password)

    def modify( self, idn, attr, value, forceAdd=False, delete=False):
        """
        Modify (add, replace, delete) a entry in LDAP Base
        With the default values:
          - If the entry doesn't exists, it will be added
          - If the entry exists, it will be replaced

        @param      idn: init DN to modify.
                         Ex: if you use "uid=jeiks", the method will
                             have uid=jeiks,dc=<baseDNofInstance>
        @param     attr: attribute to modify
        @param    value: value to modify
        @param forceAdd: True to add the attibute if it already exists
        @param   delete: True to delete the entry if it exists
        """
        s = self.search(idn)
        if s == []:
            return [1, 'idn not found']

        typeModify = None
        if s[0][1].has_key(attr):
            if forceAdd:
                typeModify = ldap.MOD_ADD
            elif delete:
                typeModify = ldap.MOD_DELETE
                found=False
                for i in s[0][1][attr]:
                    if i == value: found=True
                if found == False:
                    return [ 2, 'value not found in this attribute' ]
            else:
                typeModify = ldap.MOD_REPLACE
        else:
            # at this case, the person ask to remove,
            # but it didnt exists.. then we have to exit
            if delete: return [0, 'Success']
            ##

            typeModify = ldap.MOD_ADD

        # creating mod_attrs
        mod_attrs = [ (typeModify, attr, value) ]
        # creating DN
        if self.useSAM():
            if idn.find('ou=') == -1:
                if idn.find('cn=') == 0:
                    idn += ',ou='+DefAttr().getGroups()
                elif  idn.find('uid=') == 0:
                    idn += ',ou='+DefAttr().getUsers()
        DN = "%s,%s" % ( idn, self.__baseDN )
        if DN[0] == ',': DN = DN.replace(',','',1)
        # trying to modify

        try:
            self.__getCon().modify_s( DN, mod_attrs  )
            return [0, 'Success']
        except:
            return [3, 'error to modify this idn']



from sys import stderr
from os import listdir, remove, access, chown, chmod, makedirs

def chown_r ( path, uid, gid ):
    from os import walk

    try:
        uid = int(uid)
    except:
        try:
            uid = int( open('/etc/passwd').read().split('openldap:')[1].split(':')[1] )
        except:
            return False

    try:
        gid = int(gid)
    except:
        try:
            gid = int( open('/etc/group').read().split('openldap:')[1].split(':')[1] )
        except:
            return False

    chown (path, uid, gid)
    for root, dirs, files in walk( path ):
        if dirs <> []:
            for i in dirs:
                chown( root+'/'+i, uid, gid )
        if files <> []:
            for i in files:
                chown( root+'/'+i, uid, gid )

class LDAPControl(ExecuteProgram):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    _ldapDBDir  = '/var/lib/ldap'
    _etcLDAP    = '/etc/ldap/slapd.d'
    _etcLDAPBkp = '/etc/ldap/pro_bck-slapd.d'
    _slapdInit  = '/usr/share/slapd/slapd.init.ldif'
    _ldifInit   = 'dn: @SUFFIX@\nobjectClass: top\n'+    \
                  'objectClass: dcObject\n'+             \
                  'objectClass: organization\n'+         \
                  'o: @ORG@\ndc: @DC@\n\n'+              \
                  'dn: cn=admin,@SUFFIX@\n'+             \
                  'objectClass: simpleSecurityObject\n'+ \
                  'objectClass: organizationalRole\n'+   \
                  'cn: admin\n'+                         \
                  'description: LDAP administrator\n'+   \
                  'userPassword: @PASSWORD@\n'
    _unit      = None
    _locality  = None
    _state     = None
    _country   = None
    _cn        = None
    _serial    = None
    _expiration_days = None
    _email     = None
    _sslSetted = None
    _SAM       = None
    
    _verbose = None

    def __init__(self, SAM=True, verbose=False):
        self._verbose = verbose
        self._SAM     = SAM

    def useSAM(self):
        return self._SAM

    def startLDAP(self):
        resp = self._execute('/etc/init.d/slapd start', daemon=True)
        if resp[0] is not True:
            if self._verbose:
                stderr.write('ERROR: slapd could not be started')
            return False
        return True

    def stopLDAP(self):
        resp = self._execute('/etc/init.d/slapd stop', daemon=True)
        if resp[0] is not True:
            if self._verbose:
                stderr.write("ERROR: slapd could not be finished")
            return False
        return True

    def cleanLDAP(self):
        '''
        Clean all LDAP Database
        '''
        # finalizando o processo slapd
        self.stopLDAP()

        # limpando conteúdo do diretório /var/lib/ldap
        from shutil import rmtree
        
        try: rmtree( '/etc/ldap/slapd.d' )
        except OSError, e:
            if str(e).find('No such file or directory'):
                pass
            else:
                return False
        
        try: rmtree( '/var/lib/ldap' )
        except OSError, e:
            if str(e).find('No such file or directory'):
                pass
            else:
                return False

        return True

    def setSSLAttributes(self, unit, locality, state, country,
                         email, expiration_days, serial='0007'):
        self._unit      = unit
        self._locality  = locality
        self._state     = state
        self._country   = country
        self._serial    = serial
        self._expiration_days = expiration_days
        self._email     = email
        self._sslSetted = True
        return True

    def setSAM(self, workgroup, bindDN, passwordPlain):
        hostname = ''
        try:
            hostname = open('/etc/hostname').read().replace('\n','')
        except:
            return False
        
        if workgroup == hostname:
            print "Workgroup and Hostname (Netbios Name) can't be the same"
            return False
        
        contents = templates.templateSMB
        fields = [ [ '@WORKGROUP@', workgroup],
                   [ '@SERVER@', hostname],
                   [ '@BINDDN@', bindDN],
                   [ '@ORGGROUPS@', 'ou='+DefAttr().getGroups() ],
                   [ '@ORGUSERS@', 'ou='+DefAttr().getUsers() ],
                   [ '@ORGCOMP@', 'ou='+DefAttr().getComputers() ],
                   [ '@SSL@','no' ],
                   [ '@ADDMACHSCRIPT@', DefAttr().getCreateMachineScript() ],
                   [ '@HOMEWINFILES@', DefAttr().getHomeWinDir() ],
                   [ '@NEWGLOBALITEMS@', ''] ]

        for i in fields:
            contents = contents.replace(i[0], i[1])

        open( DefAttr().getSamba(), 'w' ).write( contents )
        
        from os import makedirs, chmod
        dirs = [ '/home/samba/netlogon/',
                 '/home/samba/profiles',
                 '/home/samba/spool/',
                 '/home/samba/printers',
                 '/home/samba/public' ]
        for i in dirs:
            try:    makedirs(i, 0777)
            except: pass

        try: chmod('/home/samba/public', 0777)
        except: pass

        try:
            contents = templates.templateLogon.replace('@SERVER@', hostname)
            open('/home/samba/netlogon/logon.vbs', 'w').write(contents)
            del contents
        except: pass

        try:
            arq = '/etc/exports'
            homeOK = False
            for i in open(arq).readlines():
                if i.split('#')[0].strip().find('/home') == 0:
                    homeOK = True
            if not homeOK:
                open(arq,'a').write('/home   *(rw,async,no_subtree_check,no_root_squash,fsid=0)\n')
                self._execute( '/etc/init.d/nfs-kernel-server restart', daemon=True )
        except: pass

        self._execute( ('/usr/bin/smbpasswd -w %s' % passwordPlain) )
        self._execute( '/etc/init.d/slapd restart', daemon=True )
        self._execute( '/etc/init.d/winbind restart', daemon=True )
        self._execute( '/etc/init.d/nslcd restart', daemon=True )
        self._execute( '/etc/init.d/samba restart', daemon=True )

    def createNewBase(self, organization, domain, password,
                      backend='HDB', ssl=False,
                      plainPassword=False, workgroup='WORKGROUP'):
        if ssl and not self._sslSetted:
            print "First you have to set SSL attributes using 'setSSLAtttributes'"
            return False

        self.cleanLDAP()
        try:    makedirs('/etc/ldap/slapd.d', 0755)
        except: pass
        try:    makedirs('/var/lib/ldap', 0700)
        except: pass

        if not access('/etc/ldap/slapd.d', 7) and not access('/var/lib/ldap', 7):
            print 'Target directories cannot exists! Exiting...'
            return False

        # Configurando backend corretamente
        if   backend.upper() == 'HDB':
            backobj = 'olcHdbConfig'
            backend = 'hdb'
        elif backend.upper() == 'BDB':
            backobj = 'olcBdbConfig'
            backend = 'bdb'
        else:
            stderr.write('ERROR: Backend not recognized')
            return False

        # Salvando senha para utilizar abaixo
        passwordPlain = password
        # Configurando senha
        if not plainPassword:
            from passlib.hash import ldap_salted_sha1
            password = ldap_salted_sha1.encrypt(password)

        # Salvando para utilizar abaixo
        domainURL = domain.replace('dc=','.')
        if domainURL[0] == '.': domainURL = domainURL[1:]
        # Configurando domain
        if not domain.startswith('dc='):
            t = ''
            for i in domain.split('.'):
                t+=',dc='+i
            domain = t.replace(',','',1)

        # Criando arquivos iniciais de configuração
        ldap_init = open(self._slapdInit).read()
        fields = { '@BACKEND@':backend, '@BACKENDOBJECTCLASS@':backobj ,
                   '@SUFFIX@' :domain ,           '@PASSWORD@':password,
                   '@ORG@'    :organization,
                   '@DC@':domain.split(',')[0].replace('dc=','') }

        ldif_init = self._ldifInit
        for i in fields.keys():
            ldap_init = ldap_init.replace( i, fields[i] )
            ldif_init = ldif_init.replace( i, fields[i] )

        indexLine = 'olcDbIndex: uid,uidNumber,gidNumber,memberUid,objectClass,cn eq\n'
        if ldap_init.find('olcDbIndex') <> -1:
            ldap_init = ldap_init.replace( 'olcDbIndex:', indexLine + '#olcDbIndex:' )
        else:
            ldap_init.append( indexLine )

        #slapadd -F output -l T1 -b "cn=config"
        #atualizar permissões

        ldapInitFile = NamedTemporaryFile(delete=False)
        ldifInitFile = NamedTemporaryFile(delete=False)
        ldapInitFile.write( ldap_init )
        ldifInitFile.write( ldif_init )
        ldapInitFile.close()
        ldifInitFile.close()

        cmds = []
        cmds.append( '/usr/sbin/slapadd -F "%s" -l %s -b "cn=config"' % (self._etcLDAP,ldapInitFile.name) )
        cmds.append( '/usr/sbin/slapadd -F "%s" -l %s -b "%s"' % (self._etcLDAP,ldifInitFile.name, domain) )
        cmds.append( '/bin/su openldap slapindex' )

        # to crete secrets.tdb
        if self.useSAM():
            self.setSAM(workgroup, domain, passwordPlain)
        
        for cmd in cmds:
            resp = self._execute( cmd )
            if not resp[0]:
                print 'ERROR: An error occuried with the command:\n\t'+cmd+'\n'+str(resp)

        chown_r('/etc/ldap/slapd.d', 'openldap', 'openldap')
        chown_r('/var/lib/ldap', 'openldap', 'openldap')

        if not self.startLDAP():
            print 'ERROR: Could not start "slapd"'
            print 'ERROR: Could not configure ldad with default values'
            return False

        if ssl:
            from gnutls import TLSFiles
            tmp = TLSFiles()

            tmp.generateTemplate(organization=organization, unit=self._unit, locality=self._locality,
                                 state=self._state, country=self._country, cn=domainURL, 
                                 email=self._email, serial=self._serial, expiration_days=self._expiration_days)

            tmp.createTLSFiles( default=True )
            if not self.startLDAP():
                print 'ERROR: Could not start "slapd"'
                print 'ERROR: Could not configure ldap to use SSL'
                return False

            chown_r('/etc/ldap/certs', 'openldap', 'openldap')
            tmp.registerTLSLdap( adminPasswd=passwordPlain )

        arq_nslcd = '/etc/nslcd.conf'
        if access( arq_nslcd, 6):
            try:
                contents = open(arq_nslcd).readlines()
                new = []
                addedBase = False
                addedUri  = False
                for i in contents:
                    aux = i.split('#')[0]
                    if aux.find('base') <> -1 and aux.find('dc=') <> -1:
                        i = 'base %s\n' % domain
                        addedBase = True
                    elif aux.find('uri') <> -1 and aux.find('//') <> -1:
                        i = 'uri ldap://127.0.0.1\n'
                        addedUri  = True
                    new.append(i)
                if new == []:
                    new.append('uid nslcd\n')
                    new.append('gid nslcd\n')
                    new.append('uri ldap://127.0.0.1/\n')
                    new.append('base %s\n' % domain)
                    addedBase = True
                    addedUri  = True
                if not addedBase and not addedUri:
                    new.append('uri ldap://127.0.0.1/\n')
                    new.append('base %s\n' % domain)
                open(arq_nslcd,'w').writelines(new)
                self._execute('/etc/init.d/nslcd restart', daemon=True)
            except: pass

        # Saving
        try:
            from os import chmod
            netcontrolFile = '/etc/ldap/netcontrol'
            open(netcontrolFile, 'w').write("""\
[base]
server  = %s
bindDN  = %s
adminPW = %s
locality = %s
state    = %s
country  = %s
email    = %s
organization = %s
SAM          = %s
workgroup = %s
""" % (domainURL, domain, passwordPlain, self._locality, self._state, self._country, self._email, organization, str(self.useSAM()), workgroup))
            chmod( netcontrolFile, 0600 )
        except:
            pass

        return True
