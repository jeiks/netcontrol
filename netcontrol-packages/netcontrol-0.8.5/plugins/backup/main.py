#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> e João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
from netcontrol.api import *
from netcontrol.ui import *
from datetime import datetime
import backend
import tarfile
import os
class Backup(CategoryPlugin):
    """
    @author: <joaopauloctga@gmail.com>
    """
    text = 'Backup' 
    icon = '/dl/backup/icon.png'
    folder = 'apps' 
    def on_init(self):
        be = backend.Config(self.app)
        self.hosts = be.read()
 
    def on_session_start(self):
        self._editing = None
 
    def get_ui(self):
        ui = self.app.inflate('backup:main')
        t = ui.find('list')
        for h in self.hosts:
            try:
                rot = h.split('.tar.gz')[0].split('_')[-2:]
                rot = "%0.2d/%0.2d/%d -  %0.2d:%0.2d" % (
                        int(rot[0][6:8]),int(rot[0][4:6]),int(rot[0][0:4]),
                        int(rot[1][0:2]),int(rot[1][2:4]) )
                ADD=True
            except:
                ADD=False
                rot = ''
            
            if ADD:
                t.append(UI.DTR(
                    UI.Label(text = h),
                    UI.Label(text = rot),
                    UI.HContainer(
                        UI.TipIcon(
                            icon='/dl/core/ui/stock/refresh.png',
                            id='restaurar/' + str(self.hosts.index(h)),
                            text='Restaurar'
                        ),
                        UI.TipIcon(
                            icon='/dl/core/ui/stock/delete.png',
                            id='del/' + str(self.hosts.index(h)),
                            text='Delete',
                            msg='Remove %s from hosts'%h
                        )
                    )
                ))
            else:
                t.append( UI.DTR(
                            UI.DTH(
                                UI.Label( text=("Desconhecido: %s"%h),bold=False ),
                                colspan=3
                            )
                        ))

        if self._editing == 'fazer_backup':
            
            bck = self.app.inflate('backup:bck')
            bck.find('nome').set('value','etc_backup')
            ui.append('main', bck) # and append it to main UI
        elif self._editing is not None:
            try:
                h = self.hosts[self._editing]
            except:
                h = backend.Host()
            d = self.app.inflate('backup:restaurar')
            msg = 'Deseja realmente restaurar o backup '
            msg += h
            d.find('restaurar_bck').set('text', msg)
            ui.append('main', d)
        
 
        return ui
 
    @event('button/click')
    def on_click(self, event, params, vars = None):
        if params[0] == 'fazer_backup':
            self._editing = 'fazer_backup'
        if params[0] == 'restaurar':
            self._editing = int(params[1]) 
        if params[0] == 'del':
            open('/tmp/hosts','w').write(str('/var/backups/netcontrol/'+self.hosts[int(params[1])] ))
            os.remove( str('/var/backups/netcontrol/'+self.hosts[int(params[1])] ) )
 
 
    @event('dialog/submit')
    def on_submit(self, event, params, vars = None):
        if params[0] == 'dlgEdit':
            if vars.getvalue('action', '') == 'OK':
                lista = os.listdir('/etc/')
                try:
                    lista.index('netcontrol-backup')
                except:
                    pass
                open('/etc/netcontrol-backup','w').write(str('/var/backups/netcontrol/'+self.hosts[self._editing]))
            self._editing = None
        
        if params[0] == 'bck':
            if vars.getvalue('action', '') == 'OK':
                lista = os.listdir('/var/backups')
                try:
                    lista.index('netcontrol')
                except:
                    os.mkdir('/var/backups/netcontrol',750)
                    pass
                
                date = datetime.today()
                data = date.strftime("%Y%m%d_%H%M")
                Dir  = '/var/backups/netcontrol'
                File = vars.getvalue('nome','').replace(' ','_').replace('.','-')
                Ext  = 'tar.gz'
                arquivo = "%s/%s_%s.%s" % (Dir, File, data, Ext)
                bck = tarfile.open(arquivo,'w:gz')
                diretorios = os.listdir('/etc/')
                for i in diretorios:
                    bck.add('/etc/'+i)
                bck.close()
                os.chmod(arquivo,600)
            self._editing = None    
