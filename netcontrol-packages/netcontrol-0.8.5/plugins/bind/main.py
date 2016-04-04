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
from netcontrol.ui import *

from bind.zone import Zone
from bind.zonefile import ZoneFile

import backend

fileSync = '/tmp/netcontrolBindControl.pid'

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


class BindPlugin(CategoryPlugin):
    text = 'DNS'
    icon = '/dl/bind/icon.png'
    folder = 'servers'

    def on_init(self):
        be = backend.Config(self.app)
        self.zones = be.read()

    def on_session_start(self):
        self._editing  = None
        self._tab      = 0
        self._editZone = None
        self._restartBind = None
        # zona atual
        self._currentDB = None
        # conteudo da zona atual
        self._zf            = None
        # adicionar registros
        self._addRecord     = False
        self._addDirective  = False
        # editar registros
        self._editSOA       = None
        self._editDirective = None

    def _execute(self, command, daemon=False):
        '''
        @return:
            [ (Boolean) Coomand executed with sucess ,
              (tuple) (stdout,stderr)                  ]
        '''
        cmd = command.split()
        print cmd[0], '::' , cmd[1:]
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

    def _getZoneDB(self, zone, zf=None):
        from time import strftime
        self._currentDB = zone

        zName   = zone.getName()
        zFile   = zone.getFile()
        if zf is None:
            self._zf = ZoneFile()
            if self._zf.openZoneFile( zFile ) is False:
                try:
                    open(zFile, 'w').close()
                except:
                    return UI.Label('O arquivo não pôde ser acessado!')

            if self._zf.openZoneFile( zFile ) is False:
                return UI.Label('O arquivo não pôde ser aberto!')
        else:
            self._zf = zf

        labelTop0 = UI.Label ( text = "Zona: "+zName.upper() , bold=True )
        labelTop1 = UI.Label ( text = "Arquivo: "+zFile )

        # Buttons
        butSave   = UI.Button( id = 'saveDB', text="Salvar DB",
                       icon="/dl/core/ui/stock/save.png",
                       onclick="form", form="frmEditDB")
        butCancel = UI.Button( id = 'cancelDB', text="Cancelar",
                       icon="/dl/core/ui/stock/dialog-cancel.png")
        butAddDir = UI.Button( id = 'addDirective', text="Adicionar Diretiva",
                        icon="/dl/core/ui/stock/add.png")
        butAddRec = UI.Button( id = 'addRecord', text="Adicionar Registro",
                        icon="/dl/core/ui/stock/add.png")
                        
        buttons   = UI.VContainer(
                                   UI.HContainer( butAddDir, butAddRec ),
                                   UI.HContainer( butCancel, butSave ) )
        #END:Buttons
        
        # DIRETIVAS
        bloco = UI.DT( width="100%" )
        bloco.append( UI.DTR(
                        UI.DTD(
                                UI.Label(text='Diretivas:', bold=True),
                                colspan = 4
                              )
                            )
                    )
        d = self._zf.getDirectives()
        for i in d.keys():
            bloco.append( UI.DTR(
                                UI.DTD(UI.Label( text=i ),colspan=2),
                                UI.Label( text=d[i] ),
                                UI.HContainer(
                                    UI.TipIcon(
                                        icon='/dl/core/ui/stock/edit.png',
                                        id='editDirectiveField/%s'%i),
                                        text='Editar Diretiva'
                                    ),
                        ))
        del d
        #END: DIRETIVAS
        
        # SOA
        bloco.append( UI.DTR( UI.DTD(
                        UI.Label ( text="Registro de Autoridade:", bold=True ),
                        colspan=4
                     )))
        s = self._zf.getSOA()
        
        for i in s.keys():
            bloco.append( UI.DTR(
                        UI.DTD(UI.Label ( text=i     ),colspan=2),
                        UI.Label ( text=s[i] ),
                        UI.HContainer(
                            UI.TipIcon(
                            icon='/dl/core/ui/stock/edit.png',
                            id='editSOAField/%s'%i,
                            text='Editar campo SOA'
                         ),
                    )))
        del s
        #END: SOA
        
        #REGISTROS
        bloco.append( UI.DTR( UI.DTD(
                        UI.Label ( text="Demais registros:", bold=True ),
                        colspan=4
                     )))
        r = self._zf.getRecords()
        text4Label = []
        for i in r.keys():
            text4Label.append("%s[§SEP§]%s[§SEP§]%s%s%s[§SEP§]%s" %\
                                (r[i]['Tipo'],r[i]['Nome'],r[i]['IP'],r[i]['HostName'], r[i]['Texto'], str(i))
                            )

        text4Label.sort()
        for i in text4Label:
            sp = i.split('[§SEP§]')
            bloco.append( UI.DTR(
                                UI.Label ( text=sp[0] ),
                                UI.Label ( text=sp[1] ),
                                UI.Label ( text=sp[2] ),
                                UI.HContainer(
                                    UI.TipIcon(
                                        icon='/dl/core/ui/stock/delete.png',
                                        id=('delRecordField/%s'%sp[3]),
                                        text='Delete',
                                        msg='Remover'
                                    )
                            )
                        ))
        del r
        #END:REGISTROS
        
        block  = UI.VContainer( labelTop0, labelTop1, bloco, buttons )

        return UI.DTR( block )

    # adiciona:
    #  tab0: as zonas na lista com botão de editar e remover
    #  tab1: as zonas no select
    def set_ui_zones(self, ui):
        l  = ui.find('listzones')
        s  = ui.find('selectZones')
        for pos in range( len(self.zones) ):
            z = self.zones[pos]
            s.append( UI.DTR(
                UI.SelectOption( value=pos, text=z.getName() )
                ))
            
            zoneName = UI.Label( text = z.getName(), bold=True )
            zoneType = UI.Label( text = z.getType() )
            zoneFile = UI.Label( text = z.getFile() )
                
                
            l.append(UI.DTR(
                zoneName, zoneType, zoneFile,
                UI.HContainer(
                    UI.TipIcon(
                        icon='/dl/core/ui/stock/edit.png',
                        id='edit/' + str(self.zones.index(z)),
                        text='Editar'
                    ),
                    UI.TipIcon(
                        icon='/dl/core/ui/stock/delete.png',
                        id='del/' + str(self.zones.index(z)),
                        text='Delete',
                        warning='Remover %s' % z.getName()
                    )
                ),
            ))

    # Trata a tab1, referente a acao de editar zona
    # Cuida do formulário da zona e também da lista e botões referentes
    def set_ui_editZone(self, ui):
        f  = ui.find('frmEditZone')
        d  = ui.find('divEditZone')
        if self._editZone <> None:
            ui.remove('selectZones')
            ui.remove('labelZonas')
            z = self.zones[self._editZone]
            d.append( self._getZoneDB(z, self._zf) )
        else:
            f.append( UI.DTR(
                UI.Button(form="frmEditZone", onclick="form",
                    text="Editar Zona", name="formEditButton",
                    id="formEditButton",
                    icon="/dl/core/ui/stock/service-run.png"
                    )
                ))

    def set_ui_AddRecord(self, ui):
        if (not self._addDirective) and (not self._addRecord):
            ui.remove('dlgAddRecord')
            return False

        d  = ui.find('dlgAddRecord')
        tx = ui.find('textTypeRecord')
        ty = ui.find('typeRecord')
        
        #____________________
        if self._addDirective:
            tx.set( 'text' , 'Diretiva:' )
            v = [ 'ORIGIN', 'INCLUDE', 'TTL' ]
            
            ### Removendo as diretivas que já existem na zona,
            ### pois elas devem ser editadas e não inseridas
            tmp = self._zf.getDirectives()
            for i in tmp.keys():
                try:    v.remove(i)
                except: pass
            # Se não sobrou nenhuma, ele desabitilita a inserção
            if v == []:
                self._addDirective = False
                ui.remove('dlgAddRecord')
                return False
            ###
            for i in v:
                ty.append( UI.SelectOption( value='Directive:'+i, text=i ) )
            d.append( UI.Formline(
                        UI.TextInput(name='recordValue', value=''),
                        text='Valor:'
                    ))

        #___________________
        elif self._addRecord:
            tx.set( 'text' , 'Registro:' )
            v = { 'A':'IPv4', 'AAAA':'IPv6', 'CNAME':'Nome Canônico do Host',
                  'NS':'Nome do Servidor', 'TXT':'Texto do Host',
                  'MX': 'Servidor de E-mail', 'PTR':'Ponteiro' }
            keys = v.keys()
            keys.sort()
            for i in keys:
                ty.append( UI.SelectOption( value='Record:'+i,
                                            text=( "%s [%s]" % (v[i],i) ) ) )
            
            d.append( UI.Formline(
                        UI.TextInput(name='recordValueSource', value=''),
                        text='1º Campo:'
                    ))
            d.append( UI.Formline(
                        UI.TextInput(name='recordValueTarget', value=''),
                        text='2º Campo:'
                    ))
            d.append( UI.Formline(
                        UI.TextInput(name='recordValuePr', value=''),
                        text='Prioridade (somente para servidor de e-mail):'
                    ))
            d.append( UI.Formline(
                        UI.Label(text='Em dúvidas, verifique a documentação.')
                    ))

    def get_ui(self):
        ui = self.app.inflate('bind:main')
        ui.find('tabs').set('active', self._tab)
        l  = ui.find('listzones')
        f  = ui.find('frmEditZone')
        s  = ui.find('selectZones')
        d  = ui.find('divEditZone')

        # lista(tab0) e select(tab1) com as zonas existentes
        self.set_ui_zones(ui)
        # tab1: adiciona e remove UI's da tab de edição
        self.set_ui_editZone(ui)
        # formulario de adicionar Diretiva ou Registro
        self.set_ui_AddRecord(ui)

        ### tab0: formulário para editar uma zona
        if self._editing is not None:
            try:
                z = self.zones[self._editing]
                # definindo valores (do xml: 'id=')
                ui.find('name').set( 'value' , z.getName() )
                ui.find('type').set( 'value' , z.getType() )
                ui.find('file').set( 'value' , z.getFile() )
            except:
                pass
        else:
            ui.remove('dlgEditZone')
        ###

        ### Para reiniciar o BIND
        if self._restartBind is not None:
            try:
                self._execute("/etc/init.d/bind9 restart",daemon=True)
                ui.find('bindCmd').append( UI.Label( text= 'Bind reiniciado' ) )
            except:
                ui.find('bindCmd').append(
                        UI.Label(text="Ocorreu um erro ao reiniciar o bind9"))
                pass
        else:
                ui.remove('dlgRestartBind')
        ###

        ### Para editar SOA
        if self._editSOA is None:
            ui.remove('dlgEditSOA')
        else:
            ui.find('dlgEditSOA').append(UI.FormLine(UI.Label(text=self._editSOA[0])))
            ui.find('SOAValue').set( 'value', self._zf.getSOAField( self._editSOA[1] ) )
        ###
        
        ### Para editar Diretivas
        if self._editDirective is None:
            ui.remove('dlgEditDirective')
        else:
            ui.find('dlgEditDirective').append(UI.FormLine(UI.Label(text=self._editDirective)))
            ui.find('DirectiveValue').set( 'value', self._zf.getDirectiveField( self._editDirective ) )


        return ui

    @event('button/click')                                                    
    def on_click(self, event, params, vars = None):
        # se for botao id 'add' (xml)
        if params[0] == 'add':
            self._editing = len( self.zones )
            self._tab = 0
        # se for id 'edit/'
        elif params[0] == 'edit':
            self._editing = int( params[1] )
            self._tab = 0
        # se for id 'del/'
        elif params[0] == 'del':
            try:
                from os import rename
                f = self.zones[ int(params[1]) ].getFile()
                rename(f, f+'.bck')
            except: 
                pass
            try:    self.zones.pop( int(params[1]) )
            except: pass
            backend.Config(self.app).save( self.zones )
            self.on_init()
            self._tab = 0
        #se o botao for reiniciar
        elif params[0] == 'restart':
            self._restartBind = True
            self._tab = 0
        # se o botão for cancelar edicao
        elif params[0] == 'cancelDB':
            self._editZone = None
            self._currentDB = None
            self._getZoneDB = None
            self._zf = None
            self._tab = 1
        elif params[0] == 'editDomainField':
            # modificar aqui
            print 'to-do'
            self._tab = 1
        elif params[0] == 'delRecordField':
            self._zf.removeRecord(params[1])
            self._tab = 1
        elif params[0] == 'addDirective':
            self._addDirective = True
            self._tab = 1
        elif params[0] == 'addRecord':
            self._addRecord = True
            self._tab = 1
        elif params[0] == 'editDirectiveField':
            self._editDirective = params[1]
        elif params[0] == 'editSOAField':
            _tzf = ""
            if   params[1] == 'Nome da Zona': _tzf = 'zoneName'
            elif params[1] == 'Dominio do Servidor': _tzf = 'serverDomain'
            elif params[1] == 'E-mail do Responsável': _tzf = 'emailResp'
            elif params[1] == 'Número Serial': _tzf = 'serial'
            elif params[1] == 'Tempo de Atualização': _tzf = 'refresh'
            elif params[1] == 'Retry' : _tzf = 'retry'
            elif params[1] == 'Expiração': _tzf = 'expire'
            elif params[1] == 'Tempo de cache negativa': _tzf = 'negative'
            
            self._editSOA = [ params[1] , _tzf ]
            
            
    @event('dialog/submit')
    def on_submit(self, event, params, vars = None):
        # se for o dialogo
        if params[0] == 'dlgEditZone':
            v = vars.getvalue('value', '')
            if vars.getvalue('action', '') == 'OK':
                # se a pessoa digitar um nome de arquivo inválido
                tmp = vars.getvalue('file', '').replace('/','')
                if tmp == "etcbindlocalzonesdb." or \
                   tmp == "" or (not tmp.startswith("etcbind")) or \
                   tmp == "etcbindlocalzones" or \
                   vars.getvalue('name','') == "":
                    return None
                # cria a zona
                z = Zone()
                # obtendo valores (do xml: 'name=')
                z.setName( vars.getvalue('name', 'none') )
                z.setType( vars.getvalue('type', 'none') )
                z.setFile( vars.getvalue('file', 'none') )
                # coloca no lugar da antiga: 'edit/'
                try:
                    self.zones[self._editing] = z
                # ou adiciona uma nova: 'add'
                except:
                    self.zones.append(z)
                # salva alteracoes
                backend.Config(self.app).save( self.zones )
                self.on_init()
        elif params[0] == 'dlgAddRecord':
            if vars.getvalue('action', '') == 'OK':
                if vars.getvalue('typeRecord','').split(':')[0] == 'Directive':
                    r = vars.getvalue('typeRecord','').split(':')[1]
                    v = vars.getvalue('recordValue','')
                    if v <> "":
                        self._zf.setDirective("%s %s" % (r,v))
                    del r
                    del v
                elif vars.getvalue('typeRecord','').split(':')[0] == 'Record':
                    r = vars.getvalue('typeRecord','').split(':')[1]
                    s = vars.getvalue('recordValueSource','')
                    p = vars.getvalue('recordValuePr','')
                    t = vars.getvalue('recordValueTarget','')
                    if r <> 'MX':
                        p = ''
                    elif p == '':
                        p = 10
                    self._zf.insertRecord( "%s IN %s %s %s"%(s,r,p,t) )
        elif params[0] == 'dlgEditSOA':
            if vars.getvalue('action', '') == 'OK' and vars.getvalue('SOAValue','') != '':
                self._zf.setSOAbyFields( **{self._editSOA[1]:vars.getvalue('SOAValue','')} )
        elif params[0] == 'dlgEditDirective':
            if vars.getvalue('action', '') == 'OK' and vars.getvalue('DirectiveValue','') != '':
                self._zf.setDirective( "%s %s" % (self._editDirective, vars.getvalue('DirectiveValue','')) )

        # fecha janelas de formulario
        self._editing       = None
        self._restartBind   = None
        self._addDirective  = False
        self._addRecord     = False
        self._editSOA       = None
        self._editDirective = None
        

    @event('form/submit')
    def on_submit_form(self, event, params, vars = None):
        # formulario EditZone
        if params[0] == 'frmEditZone':
            self._tab = 1
            self._editZone = int( vars.getvalue('selectZones', '') )
        # formulario frmEditDB
        elif params[0] == 'frmEditDB':
            backend.Config(self.app).saveZoneFile( self._currentDB, self._zf )
            self._tab = 1
            self._currentDB = None
            self._zf = None
            self._editZone = None
