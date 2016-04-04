# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA

"""

class TLSFiles:
    '''
    Create TLS Files for LDAP authentication:
        ca.key, ca.pem, server.key, server.pem
        
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    '''
    _t_keys = ['org','unit','locality','state','country','cn','serial','exp','email']
    _templ  = 'organization = "${org}"\n'+  \
              'unit = "${unit}"\n'+         \
              'locality = "${locality}"\n'+ \
              'state = "${state}"\n'+       \
              'country = ${country}\n'+     \
              'cn = "${cn}"\n'+             \
              'serial = ${serial}\n'+       \
              'expiration_days = ${exp}\n'+ \
              'email = "${email}"\n'
    _certsDir = '/etc/ldap/certs'
    _verbose  = True
    _vverbose = True

    def __init__(self, verbose=False, vverbose=False):
        '''
        Simple clean certFile object
	    @param  verbose: show ERRORs and INFOs messages
	    @param vverbose: show gnutls messages
        '''
        self.certFile   = None
        self.certFileCA = None
        self._verbose  = verbose
        self._vverbose = vverbose

    def _execute(self, command):
        '''
        @return:
            [ (Boolean) Coomand executed with sucess ,
              (tuple) (stdout,stderr)                  ]
        '''
        from subprocess import Popen, PIPE
        ret  = []
        cmd = Popen(command, shell=True,
                    stdout=PIPE, stderr=PIPE)
	w = cmd.wait()
        if w == 0: ret.append(True)
        else: ret.append(False)            
        ret.append( cmd.communicate() )
	if self._verbose:
		try: print ret[1][1]
		except: pass

        return ret

    def createTLSFiles(self, directory=None, default=False):
        """
            @param directory: directory to save TLS Files
                              if None, a temp dir will be created
	        @param   default: create files in /etc/ldap/certs
        """
        from sys import stderr
        if self.certFile is None:
            if self._verbose:
                stderr.write('ERROR: First you have to generate Template\n')
            return None

        if default:
            directory = self._certsDir

        if directory is None:
            from tempfile import mkdtemp
            directory = mkdtemp()
        if self._verbose:
            print ( 'INFO: The files are being created at '+directory )

        from os import mkdir, unlink
        try: mkdir( directory )
        except OSError, e:
            if e.strerror == 'File exists':
                if self._verbose:
                    print ( "INFO: The files in %s will be replaced" % directory )
                pass
            else:
                if self._verbose:
                    stderr.write('ERROR: Failed to create directory')
                return None

        ca_key = directory+'/ca.key'
        ca_pem = directory+'/ca.pem'
        server_key = directory+'/server.key'
        server_pem = directory+'/server.pem'

        for i in [ ca_key,ca_pem,server_key,server_pem ]:
            try: unlink(i)
            except: pass

        gnutlsCommands = [
            'certtool --generate-privkey --outfile ca.key', #private key
            'certtool --generate-self-signed --load-privkey ca.key '+ #certificate
                      '--outfile ca.pem --template templCAFile',

            'certtool --generate-privkey --outfile server.key', #server key
            'certtool --generate-certificate --load-privkey '+  #certificate
                      'server.key --outfile server.pem --load-ca-certificate '+
                      'ca.pem --load-ca-privkey ca.key --template templFile'
                         ]

	from time import sleep
	for cmd in gnutlsCommands:
            c = cmd.replace('ca.key',ca_key).\
                    replace('ca.pem',ca_pem).\
                    replace('server.key',server_key).\
                    replace('server.pem',server_pem).\
                    replace('templFile', self.certFile.name).\
                    replace('templCAFile', self.certFileCA.name)

            resp = self._execute(c)
            if not resp[0]:
                if self._verbose:
                    stderr.write( 'ERROR: An error occuried with the command:\n\t'+cmd+'\n'+str(resp)+'\n')
                return None

        return directory

    def generateTemplate(self, organization, unit, locality, state,
                         country, cn, email, serial='007',
                         expiration_days='99999' ):
        '''
            Generate template file
        '''
        from string import Template
        from tempfile import NamedTemporaryFile
        
        if cn.find('dc=') <> -1:
            cn = cn.replace('dc=','.').replace(',','')
            if cn[0] == '.': cn = cn[1:]

        args = [organization, unit, locality, state, country,
                cn, serial, expiration_days, email ]
        # Temporary File and Directory
        self.certFile   = NamedTemporaryFile(delete=False)
	self.certFileCA = NamedTemporaryFile(delete=False)
        # Getting file content from Template
        content = Template(self._templ).substitute( **dict( zip(self._t_keys,args) ) )
        # Writing the file
        self.certFile.write( content )
	self.certFileCA.write( content+'ca\ncert_signing_key\n' )
	self.certFile.close()
	self.certFileCA.close()

    def __getCertsAttrs(self, anotherDirectory=None):
        if anotherDirectory is not None:
            p = anotherDirectory
        else:
            p = self._certsDir

        
        ret = { 'olcTLSCACertificateFile': ('%s/ca.pem'%p),
                'olcTLSCertificateFile': ('%s/server.pem'%p),
                'olcTLSCertificateKeyFile': ('%s/server.key'%p) }

        return ret

    def registerTLSLdap( self, adminPasswd, adminName='admin'):
        """
        Register TLS Files in LDAP Base
        Only to 'localhost' Server
        """
        from passlib.hash import ldap_salted_sha1
        from os import listdir, access

        #_________
        #START: FIX this... it's so ugly ####
        olcDatabaseFile = ''
        baseDir = '/etc/ldap/slapd.d/cn=config'
        if not access(baseDir, 7):
            print 'Error to access ldap config dir base'
            return [ 3, 'Error to access ldap config dir base' ]
        for arq in listdir( baseDir ):
            if     arq.startswith('olcDatabase=') \
               and arq.find('config.ldif') <> -1:
                olcDatabaseFile = baseDir+'/'+arq
                break

        if olcDatabaseFile == '':
            return [ 1 , 'Error to find olcDatabase=.*config.ldif' ]
    
        passwd = ldap_salted_sha1.encrypt(adminPasswd)
        contents = []
        for line in open(olcDatabaseFile).readlines():
            # insert olcRootPW after olcRootDN
            if line.find('olcRootDN') <> -1:
                contents.append(line)
                contents.append('olcRootPW: %s\n'%passwd)
            # delete olcRootPW if exists
            elif line.find('olcRootPW') == -1:
                contents.append(line)
        
        from netcontrolldap import LDAPControl
        lc = LDAPControl()
        lc.stopLDAP()

        open(olcDatabaseFile,'w').writelines( contents )
        del contents

        if not lc.startLDAP():
            return [ 2 , 'Error starting LDAP with new config passwd' ]
        # STOP: FIX this
        #_________

        # Adding certificates ______________________________________
        from netcontrolldap import LDAPConnection
        tmp = LDAPConnection( server='localhost', ssl=False,
                              baseDN='cn=config', admName=adminName,
                              admPasswd=adminPasswd)
        from time import sleep
        myTry = 1
        while tmp.search() == [] and myTry < 5:
            tmp.closeConnection()
            myTry += 1
            sleep(1)

        ca = self.__getCertsAttrs()
        stats=0
        for i in ca.keys():
            stats += tmp.modify( idn='',
                                 attr=i,
                                 value=ca[i] )[0]

        if stats > 0: return [ 1 , 'Error to modify certs' ]

        del tmp
        #___________________________________________________________

        # Enabling ldaps
        defDebianFile = '/etc/default/slapd'
        dFile = open(defDebianFile).readlines()
        ADDED = False
        LADD  = 'SLAPD_SERVICES="ldap:/// ldaps:/// ldapi:///"\n'
        contents = []
        for i in dFile:
            if i.split('#')[0].find('SLAPD_SERVICES') <> -1:
                ADDED=True
                contents.append( LADD )
            else:
                contents.append(i)

        if not ADDED: contents.append( LADD )

        open(defDebianFile,'w').writelines( contents )
        del contents
        # _______________

        lc.stopLDAP()
        return lc.startLDAP()
