# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""

from netcontrol.ui import *
from netcontrol.api import *

import backend


class NtopPlugin(CategoryPlugin):
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    text = 'Tráfego Rede'
    icon = '/dl/ntop/icon.png'
    folder = 'apps'

    def on_init(self):
        backend.Config(self.app).ConfigureIfaces()

    def on_session_start(self):
        self._showNtop = False

    def get_ui(self):
        ui = self.app.inflate('ntop:main')
        if self._showNtop:
            ui.append('main',
                       UI.FullDialog().append(
                           UI.IFrame(
                               src=('http://%s:3000' % self.app.environ['SERVER_NAME']),
                               id='frmTop', width="100%", height="100%")
                     ))
            self._showNtop = False
        return ui

    @event('button/click')
    def on_click(self, event, params, vars = None):
        if params[0] == 'showNtop':
            self._showNtop = True

