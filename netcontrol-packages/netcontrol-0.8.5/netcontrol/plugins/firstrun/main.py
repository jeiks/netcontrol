# -*- coding: utf-8 -*-
from netcontrol.api import *
from netcontrol.ui import *
from netcontrol.utils import *
from netcontrol.plugmgr import RepositoryManager

class FirstRun(CategoryPlugin, URLHandler):
    text = 'Assistente de primeira execução e validação'
    icon = None
    folder = None

    def on_session_start(self):
        if self.app.gconfig.get('netcontrol', 'firstrun', '') == 'no':
            self._step = 2
        else:
            self._step = 1

    def get_ui(self):
        ui = self.app.inflate('firstrun:main')
        step = self.app.inflate('firstrun:step%i'%self._step)
        ui.append('content', step)

        return ui

    @event('form/submit')
    def on_event(self, event, params, vars=None):
        if params[0] == 'frmChangePassword':
            login = vars.getvalue('login', '').replace(' ','')
            password = vars.getvalue('password', '').replace(' ','')
            if login == '' or password == '':
                self.put_message('err', 'Entre com login e senha válidos!')
            else:
                self.app.gconfig.remove_option('users', 'admin')
                self.app.gconfig.set('users', login, hashpw(password))
                self.app.gconfig.set('netcontrol', 'firstrun', 'no')
                self.app.gconfig.save()
                self._step = 2
