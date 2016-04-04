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
from netcontrol.ui import *
from netcontrol import apis
from netcontrol.utils import *
from netcontrolrede.netcontrolrede import MascRede

import backend

class ControleWebPlugin(CategoryPlugin):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
             João Paulo Constantino <joaopauloctga@gmail.com>
    """
    text = 'Controle Web'
    icon = '/dl/controle_web/icon.png'
    folder = 'system'

    def on_init(self):
        be = backend.Config(self.app)
        self._grupos = be.getGroups()

    def on_session_start(self):
        self._tab = 0
        self._tree = TreeManager()
        self._selected = None
        self._addGroup = False
        self._addGroupError = None
    
    def get_ui(self):
        if self._addGroup:
            ui = self.app.inflate('controle_web:addgroup')
            if self._addGroupError is not None:
                ui.find('dlgAddGroupMsg').set('text',self._addGroupError)
                self._addGroupError = None
            return ui

        ui = self.app.inflate('controle_web:main')
        ui.append('tree', self.get_ui_tree())
        ui.find('tabs').set('active', self._tab)

        users  = None
        tusers = None

        if not backend.Config(self.app).groupExists(self._selected):
            self._selected = None

        if self._selected is not None:
            if self._selected.startswith('_'):
                # IPs
                ui.remove('0')
                if self._tab == 0:
                    self._tab = 1
                    ui.find('tabs').set('active', self._tab)
                IPList    = backend.Config(self.app).getIPsFrom(self._selected)
                IPList_ui = ui.find('lista_IPs')
                if IPList_ui is not None:
                    if IPList == []:
                        IPList_ui.append(
                                UI.DTR().append(
                                    UI.DTD().append(UI.Center(
                                        UI.Label(text="Nenhum IP cadastrado")
                                ))))
                    else:
                        for i in IPList:
                            IPList_ui.append(
                                UI.DTR().append(
                                      UI.DTD().append(
                                          UI.Center(UI.Label(text=i))
                                      )
                                  ).append(
                                      UI.DTD().append( UI.Center().append(
                                          UI.TipIcon(icon='/dl/core/ui/stock/delete.png',
                                                     id='deleteIP/'+self._selected+'/'+i,
                                                     warning="Remover o IP %s"%i,
                                                     text='Apagar')
                                      ))
                                  ))
                # FIM: IPs
            else:
                # Usuarios
                ui.remove('1')
                if self._tab == 1:
                    self._tab = 0
                    ui.find('tabs').set('active', self._tab)
                tusers = backend.Config(self.app).getAllUsers()
                users  = backend.Config(self.app).getUsersFrom(self._selected)

                if tusers == []:
                    ui.find('todos_usuarios').append(
                            UI.DTR().append(
                                UI.DTD().append(UI.Center(
                                    UI.Label(text="Nenhum usuário cadastrado")
                            ))))
                    users = []

                if users == []:
                    ui.find('usuarios').append(
                            UI.DTR().append(
                                UI.DTD().append( UI.Center(
                                    UI.Label(text="Nenhum usuário adicionado")
                            ))))

                for i in tusers:
                    u = i.split()[0]
                    try:
                        tmp = users.pop( users.index(u) )
                        ui.find('usuarios').append(
                                UI.DTR().append(
                                    UI.DTD().append(
                                        UI.Checkbox(text="   "+i, id='u/'+u, name='u/'+u)
                                )))
                    except:
                        ui.find('todos_usuarios').append(
                                UI.DTR().append(
                                    UI.DTD().append(
                                        UI.Checkbox(text="   "+i, id='t/'+u, name='t/'+u)
                                )))
                #FIM: Usuarios

            # Listas do Squid
            grupos = backend.Config(self.app).getSquidGroups(
                        self._grupos[self._selected][1] 
                     )
            keys = grupos.keys()
            keys.sort()
            
            vertical = False
            v1 = UI.VContainer(width='310', height='100%')
            v2 = UI.VContainer(width='310', height='100%')
            for i in keys:
                if vertical:
                    v1.append( UI.CheckBox(
                            text='   '+i,
                            id='gruposSquid',
                            name=grupos[i][0],
                            checked=grupos[i][1],
                            height="50%"
                            ))
                else:
                    v2.append( UI.CheckBox(
                            text='   '+i,
                            id='gruposSquid',
                            name=grupos[i][0],
                            checked=grupos[i][1],
                            height="50%"
                            ))

                vertical = not vertical

            ui.find('sites_grupos').append(v1).append(v2)
            ui.find('titulo').set('text',
                    'Grupo selecionado: '+self._selected[5:])

            # Personalizados
            l = backend.Config(self.app).getPersLiberados (self._selected)
            b = backend.Config(self.app).getPersBloqueados(self._selected)
            if l <> []:
                ll = ui.find('sites_liberados')
                for i in l:
                    ll.append(
                              UI.DTR().append(
                                  UI.DTD().append(
                                      UI.Label(text=i)
                                  )
                              ).append(
                                  UI.DTD().append( UI.Center().append(
                                      UI.TipIcon(icon='/dl/core/ui/stock/delete.png',
                                                 id='deleteLib/'+self._selected+'/'+i,
                                                 text='Apagar')
                                  ))
                              ))
            if b <> []:
                lb = ui.find('sites_bloqueados')
                for i in b:
                    lb.append(
                              UI.DTR().append(
                                  UI.DTD().append(
                                      UI.Label(text=i)
                                  )
                              ).append(
                                  UI.DTD().append( UI.Center().append(
                                      UI.TipIcon(icon='/dl/core/ui/stock/delete.png',
                                                 id='deleteBlo/'+self._selected+'/'+i,
                                                 text='Apagar')
                                  ))
                              ))

        else:
            ui.find('titulo').set('text',
                    'Selecione o grupo desejado abaixo')
            ui.remove('conteudo')


        return ui

        #print 'Grupos:', gruposProxy.keys()
        #for i in gruposProxy.keys():
        #    print 'Usuarios:', gruposProxy[i][0]
        #    print 'Listas:', gruposProxy[i][1]


    def get_ui_tree(self):
        # para adicionar os grupos de usuários na árvore
        root = UI.TreeContainer(text='Grupos', id='/')

        for i in self._grupos.keys():
            root.append(
                    UI.TreeContainerNode(
                        UI.HContainer(
                            UI.TipIcon(
                                icon='/dl/core/ui/stock/delete.png',
                                id='%s/%s' % (i.startswith('_') and 'delIPGroup' or 'delLdapGroup',i),
                                warning="Remover o grupo %s." % (
                                            i.startswith('_') and i[1:]+' (IPs)' or i[5:]+' (Usuários)'
                                        ),
                                text='Remover Grupo'
                            ), UI.LinkLabel(
                               text=i.startswith('_') and i[1:]+' (IPs)' or i[5:]+' (Usuários)',
                               id='select/'+i,
                               name='select/'+i
                            )),
                            active=False ))

        if self._grupos == {}:
            root.append(
                    UI.TreeContainerNode(
                        UI.Label(
                            text="Sem grupos adicionados."
                       )))

        self._tree.apply(root)
        root['expanded'] = True
        return root

    @event('dialog/submit')
    @event('form/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'frm_usr':
            if   vars.getvalue('action','') == 'addUsers':
                for i in vars.list[1:]:
                    name, value = i.name, int(i.value)
                    name = name.split('/')
                    if name[0] == 't' and value == 1:
                        try:
                            backend.Config(self.app).addUserToGroups(
                                    name[1], self._selected)
                        except:pass
            elif vars.getvalue('action','') == 'delUsers':
                for i in vars.list[1:]:
                    name, value = i.name, int(i.value)
                    name = name.split('/')
                    if name[0] == 'u' and value == 1:
                        try:
                            backend.Config(self.app).removeUserFromProxy(name[1])
                        except:pass
            self._tab = 0
        elif params[0] == 'frm_addlib':
            if   vars.getvalue('action','') == 'addLiberado':
                url = vars.getvalue('site_lib','').split(',')
                url = map(lambda x: x.split('://')[-1].split('/')[0], url)
                backend.Config(self.app).addUrlLiberados( self._selected, url )

            elif vars.getvalue('action','') == 'addBloqueado':
                url = vars.getvalue('site_bloq','').split(',')
                url = map(lambda x: x.split('://')[-1].split('/')[0], url)
                backend.Config(self.app).addUrlBloqueados(self._selected, url )
            self._tab = 3

        elif params[0] == 'frm_setSquidGroups':
            if vars.getvalue('action','') == 'setSquidGroups':
                lista = []
                for i in vars.list[1:]:
                    name, value = i.name, i.value
                    lista.append( (name,value) )
                backend.Config(self.app).setSquidGroups(self._selected, lista)
                del lista
            self._tab = 2
        elif params[0] == 'dlg_addGroup':
            if vars.getvalue('action','') == 'OK':
                nameGroup   = vars.getvalue('nameGroup','').strip()
                typeGroupIP = vars.getvalue('typeGroupIP','') == '1'
                if nameGroup == '':
                    self._addGroupError = 'Nome inválido!'
                elif self._grupos.has_key(nameGroup):
                    self._addGroupError = 'Este grupo já existe!'
                else:
                    valid = True
                    for i in nameGroup:
                        if not ( ( ord(i) >= ord('a') and ord(i) <= ord('z') ) or \
                                 ( ord(i) >= ord('A') and ord(i) <= ord('Z') ) or \
                                 ( ord(i) >= ord('0') and ord(i) <= ord('9') ) or \
                                 ( ord(i) == ord(' ') ) ):
                            valid = False
                    if valid:
                        #se for grupo de IPs
                        if typeGroupIP:
                            if backend.Config(self.app).addIPGroup(nameGroup):
                                self._addGroup = False
                            else:
                                self._addGroupError = 'Erro ao cadastrar grupo!'
                        #se for grupo de usuarios
                        else:                        
                            if backend.Config(self.app).addLdapGroup(nameGroup):
                                self._addGroup = False
                            else:
                                self._addGroupError = 'Erro ao cadastrar grupo!'
                    else:
                        self._addGroupError = "Utilize somente letras, números e espaços!"

            elif vars.getvalue('action','') == 'Cancel':
                self._addGroup = False
        elif params[0] == 'frm_IPList':
            if vars.getvalue('action','') == 'addNewIP':
                ip = vars.getvalue('inputIP','')
                if ip <> '':
                    backend.Config(self.app).addIPToGroup(ip, self._selected)

    @event('button/click')
    @event('linklabel/click')
    @event('treecontainer/click')
    def on_click(self, event, params, vars = None):
        if params[0]   == 'select':
            self._selected = params[1]
            self._tab = 0
        elif params[0] == 'deleteLib':
            backend.Config(self.app).delUrlLiberados(params[1], params[2])
            self._tab = 3
        elif params[0] == 'deleteBlo':
            backend.Config(self.app).delUrlBloqueados(params[1], params[2])
            self._tab = 3
        elif params[0] == 'addGroup':
            self._addGroup = True
        elif params[0] == 'delLdapGroup':
            if backend.Config(self.app).delLdapGroup( params[1] ):
                self._selected = None
        elif params[0] == 'delIPGroup':
            if backend.Config(self.app).delIPGroup( params[1] ):
                self._selected = None
        elif params[0] == 'deleteIP':
            backend.Config(self.app).removeIPFromGroup(
                            params[2] + (len(params) == 4 and '/'+params[3] or ''),
                            params[1])
        elif params[0] == 'applyAllSettings':
            backend.Config(self.app).applyAllSettings()
