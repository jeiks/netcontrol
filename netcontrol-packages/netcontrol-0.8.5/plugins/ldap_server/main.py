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
from netcontrol.ui import *
from netcontrol import apis
import backend

class LDAPServerPlugin(CategoryPlugin):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    text = 'Autenticação'
    icon = '/dl/ldap_server/icon.png'
    folder = 'servers'

    _fields = ['organization','domain','locality',
               'state','country','email',
               'expiration_days','serial',
               'passwd1','passwd2', 'workgroup']

    def on_init(self):
        be = backend.Config(self.app)

    def on_session_start(self):
        self._error = ''
        self._autoFields = {}
        self._newLDAP = False
        self._newLDAPCreated = False

    def get_ui(self):
        ui = self.app.inflate('ldap_server:main')
        hasLdapInstalled = backend.Config(self.app).\
                           has_ldap_installed()
        if self._newLDAPCreated:
            ui.remove('formNewLdap')
            ui.remove('boxMessage')
            self._newLDAPCreated = False
        elif hasLdapInstalled[0] and not self._newLDAP:
            # so mensagem
            ui.remove('formNewLdap')
            ui.remove('boxLDAPCreated')
            ui.find('bmDomainW').set('text',
                    'Domínio Active Directory: '+hasLdapInstalled[1][2])
            ui.find('bmDomain').set('text',
                    'Domínio: '+hasLdapInstalled[1][0])
            ui.find('bmDN').set('text',
                    'DN: '+hasLdapInstalled[1][1])
        else:
            # nova base ldap
            ui.remove('boxMessage')
            ui.remove('boxLDAPCreated')
            if not self._newLDAP: ui.remove('cancelNewLDAP')
            ui.find('msgBoxNewLdap').set('text', self._error)
            self._error = ''
            for i in self._autoFields.keys():
                try: ui.find(i).set('value',self._autoFields[i])
                except: pass

        return ui

    @event('button/click')
    def on_click(self, event, params, vars = None):
        if params[0] == 'createNewLdap':
            self._newLDAP = True


    @event('form/submit')
    def on_form_submit(self, event, params, vars = None):
        if params[0] == 'formNewLdap':
            v = vars.getvalue('action','')
            if v == 'cancelNewLdap':
                self._newLDAP = False
                return None

            # Verifying the fields
            ret = False
            args = {}
            for i in self._fields:
                v = vars.getvalue(i,'').strip()
                if v == '':
                    self._error = 'Info: Preencha todos os campos!'
                    ret = True
                self._autoFields[i] = v
            if ret: return None
          
            # Getting passwords with 'spaces'
            self._autoFields['passwd1'] = vars.getvalue('passwd1','')
            self._autoFields['passwd2'] = vars.getvalue('passwd2','')

            if self._autoFields['passwd1'] <> \
               self._autoFields['passwd2']:
                   self._error = 'Erro: Digite novamente as '+\
                                 'senhas, pois elas não coincidem!'
                   return None

            newDomain = self._autoFields['domain'].lower().replace(' ','')
            if newDomain <> self._autoFields['domain']:
                self._error = 'Info: O campo "Domínio" foi '+\
                              'modificado para um valor válido. '+\
                              "Clique em 'Criar nova base LDAP' "+\
                              'para confirmar a modificação.'
                self._autoFields['domain'] = newDomain
                return None

            workgroup = self._autoFields['workgroup'].strip().upper()
            if self._autoFields['workgroup'] <> workgroup:
                self._autoFields['workgroup'] = workgroup
                self._error = 'Info: O campo "Grupo de Trabalho" foi '+\
                              'modificado para um nome válido.'+\
                              "Clique em 'Criar nova base LDAP' "+\
                              'para confirmar a modificação.'
                return None
            self._autoFields['password'] = self._autoFields['passwd1']
            self._autoFields.pop('passwd1')
            self._autoFields.pop('passwd2')
            backend.Config(self.app).createNewLdap( self._autoFields )
            self._newLDAPCreated = True
            self._newLDAP = False
            self._autoFields = {}

