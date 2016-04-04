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
from netcontrol.ui import *
import backend

class UsersLdapPlugin(CategoryPlugin):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    text = 'Usr. de domínio'
    icon = '/dl/users_ldap/icon.png'
    folder = 'system'

    def on_init(self):
        be = backend.Config(self.app)
        self.users = be.getUserList()
        self.groups = be.getGroupList()

    def on_session_start(self):
        self._editUser       = None
        self._editUserFields = None
        self._editUserAlert  = ''
        self._addUser        = False
        self._addUserFields  = None
        self._addUserAlert   = ''
        self._delUser        = None

        self._addGroup       = False
        self._addGroupAlert  = ''
        self._addGroupFields = None
        self._delGroup       = None
        self._editGroup      = None

        self._editUserGroups = None
        self._editGroupUsers = None
        self._editUG         = False

    def get_ui(self):
        #EDIT USER________________________________________________________
        if self._editUser is not None:
            ui = self.app.inflate('users_ldap:edituser')
            if self._editUserAlert == '':
                t = {'login': 'uid', 'name':'cn',
                     'surname':'sn', 'homeDirectory': 'homeDirectory',
                     'address':'homePostalAddress', 'mail':'mail',
                     'phone':'homePhone', 'loginShell': 'loginShell'
                    }
                for i in t.keys():
                    try: ui.find(i).set('value', self._editUser[ t[i] ][0])
                    except: pass
            else:
                for i in self._editUserFields.keys():
                    try: ui.find(i).set('value', self._editUserFields[i])
                    except: pass
                    
                ui.find('alert').set('text', self._editUserAlert)
                self._editUserAlert  = ''
                self._editUserFields = None

            return ui
        #END: EDIT USER____________________________________________________

        #ADD USER__________________________________________________________
        if self._addUser:
            ui = self.app.inflate('users_ldap:edituser')

            if self._addUserFields is not None:
                for i in self._addUserFields.keys():
                    try: ui.find(i).set('value', self._addUserFields[i])
                    except: pass
                    
            ui.find('alert').set('text', self._addUserAlert)
            self._addUserAlert  = ''
            self._addUserFields = None

            return ui
        #END: ADD USER_____________________________________________________

        #ADD GROUP_________________________________________________________
        if self._addGroup:
            ui = self.app.inflate('users_ldap:editgroup')

            if self._addGroupFields is not None:
                for i in self._addGroupFields.keys():
                    try: ui.find(i).set('value', self._addGroupFields[i])
                    except: pass
                    
            ui.find('alert').set('text', self._addGroupAlert)
            self._addGroupAlert  = ''
            self._addGroupFields = None

            return ui
        #END: ADD GROUP____________________________________________________


        #DEL USER__________________________________________________________
        if self._delUser is not None:
            ui = self.app.inflate('users_ldap:confirm')
            ui.find('msg').set('text','Deseja realmente apagar o usuário %s?' % str(self._delUser['uid'][0]))
            return ui
        #END: DEL USER_____________________________________________________

        #DEL GROUP_________________________________________________________
        if self._delGroup is not None:
            ui = self.app.inflate('users_ldap:confirm')
            ui.find('msg').set('text','Deseja realmente apagar o grupo %s?' % str(self._delGroup['cn'][0]))
            return ui
        #END: DEL GROUP____________________________________________________

        # Editar grupos do usuario_________________________________________
        if self._editUserGroups is not None:
            ui = self.app.inflate('users_ldap:editug')
            c = ui.find('checkboxes')
            UG = backend.Config(self.app).getUG( 
                                      user=self._editUserGroups['uid'][0] )
            for i in UG:
                c.append(
                        UI.CheckBox(
                            text=i[0], id=i[0], name=i[0], checked=i[1]
                        ))
            if UG == []:
                ui.find('msg').set('text', 'Primeiro você deve adicionar um grupo ao sistema.')
                ui.remove('checkboxes')
                self._editUG = False
            else:
                ui.find('msg').set('text', 'Escolha os grupos desse usuário:')
                self._editUG = True
            return ui
        #__________________________________________________________________

        # Editar usuarios do grupo_________________________________________
        if self._editGroupUsers is not None:
            ui = self.app.inflate('users_ldap:editug')
            c = ui.find('checkboxes')
            UG = backend.Config(self.app).getUG(
                    group=self._editGroupUsers['cn'][0] )
            for i in UG:
                c.append(
                        UI.CheckBox(
                            text=i[0], id=i[0], name=i[0], checked=i[1]
                        ))

            if UG == []:
                ui.find('msg').set('text', 'Primeiro você deve adicionar um usuário ao sistema.')
                ui.remove('checkboxes')
                self._editUG = False
            else:
                ui.find('msg').set('text', 'Escolha os usuários desse grupo:')
                self._editUG = True
            return ui
        #__________________________________________________________________

        ui = self.app.inflate('users_ldap:main')
        u = ui.find('userList')
        g = ui.find('groupList')

        if len(self.users)  == 0: ui.remove('userList')
        if len(self.groups) == 0: ui.remove('groupList')

        for i in self.users:
            try:
                u.append(UI.DTR(
                    UI.Label(text=i['uid'][0]),
                    UI.Label(text=i['cn'][0]),
                    UI.HContainer(
                        UI.TipIcon(
                            icon='/dl/core/ui/stock/edit.png',
                            id='editU/' + str(self.users.index(i)),
                            text='Editar usuário'
                        ),
                        UI.TipIcon(
                            icon='/dl/core/ui/stock/edit.png',
                            id='editGU/' + str(self.users.index(i)),
                            text='Editar Grupos do usuário'
                        ),
                        UI.TipIcon(
                            icon='/dl/core/ui/stock/delete.png',
                            id='delU/' + str(self.users.index(i)),
                            text='Remover usuário'
                        )
                    ),
                ))
            except: pass
        
        for i in self.groups:
            try:
                g.append(UI.DTR(
                    UI.Label(text=i['cn'][0]),
                    UI.HContainer(
                        UI.TipIcon(
                            icon='/dl/core/ui/stock/edit.png',
                            id='editUG/' + str(self.groups.index(i)),
                            text='Editar Usuários do grupo'
                        ),
                        UI.TipIcon(
                            icon='/dl/core/ui/stock/delete.png',
                            id='delG/' + str(self.groups.index(i)),
                            text='Remover grupo'
                    )
                    ),
                ))
            except: pass

        return ui

    @event('button/click')
    def on_click(self, event, params, vars = None):
        # from xml
        if   params[0] == 'addUser':
            self._addUser = True
            return
        elif params[0] == 'addGroup':
            self._addGroup = True
            return

        # from this ui
        try:
            if params[0][-1] == 'U':
                component = self.users [ int(params[1]) ]
            else:
                component = self.groups[ int(params[1]) ]
        except:
            component = None
        if component is None: return

        if   params[0] == 'editU':
            self._editUser = component
        elif params[0] == 'editG':
            self._editGroup = component
        elif params[0] == 'delU':
            self._delUser  = component
        elif params[0] == 'delG':
            self._delGroup  = component
        elif params[0] == 'editGU':
            self._editUserGroups = component
        elif params[0] == 'editUG':
            self._editGroupUsers = component

    def __editUserConfig(self, params, vars):
        if vars.getvalue('action', '') == 'OK':
            fields = {}
            for i in ['login','passwd1',
                      'passwd2','name','surname',
                      'homeDirectory','address',
                      'mail','phone','loginShell']:
                v = vars.getvalue(i,'').strip()
                if v <> '': fields[i] = v
            
            self._editUserFields = fields

            if fields['login'] <> self._editUser['uid'][0]:
                fields['login'] = self._editUser['uid'][0]
                self._editUserAlert = 'O campo login não pode ser modificado.'
                return

            if fields.has_key('passwd1'):
                if not fields.has_key('passwd2'):
                    fields['passwd2'] = ''

                if fields['passwd1'] == fields['passwd2'] and fields['passwd2'] <> '':
                    fields['password'] = fields.pop('passwd1')
                    fields.pop('passwd2')
                else:
                    self._editUserAlert = 'As senhas não conferem!'
                    return

            backend.Config(self.app).editUser( self._editUser, fields )
            del fields

        self._editUser = None

    def __addUserConfig(self, params, vars):
        if vars.getvalue('action', '') == 'OK':
            fields = {}
            self._addUserAlert = ''
            for i in ['login','passwd1',
                      'passwd2','name','surname',
                      'homeDirectory','address',
                      'mail','phone','loginShell']:
                v = vars.getvalue(i,'').strip()
                fields[i] = v
                if v == '':
                    self._addUserAlert = 'Preencha todos os campos.'

	    if self._addUserAlert <> '':
                self._addUserFields = fields
                return

            if vars.getvalue('homeDirectory', '') == '/home/' or \
               vars.getvalue('homeDirectory', '') == '/home' or  \
	       vars.getvalue('homeDirectory', '').split('/') <= 1:
                self._addUserAlert  = 'O Diretório Pessoal não pode ter esse valor! Modifique-o.'
                self._addUserFields = fields
                return

            self._addUserFields = fields
            if self._addUserAlert <> '': return

            if fields['login'].find(' ') <> -1:
                self._addUserAlert = 'O login não pode ter espaços em branco.'
                return

            for i in fields['login']:
                if not i.isalpha() and not i.isdigit():
                    self._editUserAlert = 'O Login não pode ter caracteres diferentes de letras e números.'
                    return

            if fields['passwd1'] == fields['passwd2'] and fields['passwd2'] <> '':
                fields['passwd'] = fields.pop('passwd1')
                fields.pop('passwd2')
            else:
                self._addUserAlert = 'As senhas não conferem!'
                return

            backend.Config(self.app).addUser( fields )
            del fields

        self._addUser       = False
        self._addUserFields = None

    @event('dialog/submit')
    def on_dialog_submit(self, event, params, vars = None ):
        # Edit or add user
        if params[0] == 'dlgEditUser':
            if self._editUser:
                self.__editUserConfig(params, vars)
                return
            if self._addUser:
                self.__addUserConfig(params, vars)
                return
        # Del User
        elif params[0] == 'dlgConfirmDel':
            if vars.getvalue('action', '') == 'OK':
                if   self._delUser is not None:
                    backend.Config(self.app).delUser ( self._delUser  )
                elif self._delGroup is not None:
                    backend.Config(self.app).delGroup( self._delGroup )
            self._delUser  = None
            self._delGroup = None
        # Add Group
        elif params[0] == 'dlgEditGroup':
            if vars.getvalue('action', '') == 'OK':
                v = vars.getvalue('name', '').strip()
                if v == '':
                    self._addGroupAlert = 'Informe um nome'
                    return

                for i in v:
                    if not i.isalpha() and not i.isdigit():
                        self._addGroupAlert = 'O Grupo não pode ter caracteres diferentes de letras e números.'
                        self._addGroupFields = {'name':v}
                        return

                backend.Config(self.app).addGroup( v )
            self._addGroup       = False
            self._addGroupFields = None
            self._addGroupAlert  = ''
        # add user to group or group to user
        elif params[0] == 'dlgEditUG':
            if vars.getvalue('action', '') == 'OK' and \
                self._editUG:
                tmpAdd = []
                tmpDel = []
                # editar grupos do usuario
                if self._editUserGroups is not None:
                    for i in self.groups:
                        g = i['cn'][0]
                        if vars.getvalue( g , '0' ) == '1':
                            tmpAdd.append(g)
                        else:
                            tmpDel.append(g)
                    backend.Config(self.app).addUserGroups( self._editUserGroups['uid'][0], tmpAdd ) 
                    backend.Config(self.app).delUserGroups( self._editUserGroups['uid'][0], tmpDel )

                # editar usuarios do grupo
                elif self._editGroupUsers is not None:
                    for i in self.users:
                        u = i['uid'][0]
                        if vars.getvalue( u , '0' ) == '1':
                            tmpAdd.append(u)
                        else:
                            tmpDel.append(u)
                    backend.Config(self.app).addGroupUsers( self._editGroupUsers['cn'][0], tmpAdd )
                    backend.Config(self.app).delGroupUsers( self._editGroupUsers['cn'][0], tmpDel )

                del tmpAdd
                del tmpDel

            self._editUserGroups = None
            self._editGroupUsers = None
            self._editUG = False
