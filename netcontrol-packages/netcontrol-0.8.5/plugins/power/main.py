# -*- coding: utf-8 -*-
"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
Uploader: João Paulo Constantino <joaopaulo@fluxsoftwares.com>
"""
from netcontrol.ui import *
from netcontrol import version
from netcontrol.com import implements
from netcontrol.api import *
from netcontrol.utils import shell

from backend import *


class PowerPlugin(CategoryPlugin):
    text = 'Ger. Energia'
    icon = '/dl/power/icon.png'
    folder = 'hardware'

    def get_ui(self):
        ui = self.app.inflate('power:main')
        
        els = ui.find('list')

        if len(get_ac_adapters()) == 0:
            els.append(UI.Label(text='Nenhum adaptador AC encontrado'))
            
        for ac in get_ac_adapters():
            img = 'present' if ac.present else 'none'
            st = 'Active' if ac.present else 'Offline'
            els.append(UI.ElementBox(UI.HContainer(
                          UI.Image(file='/dl/power/ac-%s.png'%img),
                          UI.VContainer(
                              UI.Label(text='AC Adapter %s' % ac.name, size=2),
                              UI.Label(text=st)
                          )
                      )))

        if len(get_batteries()) == 0:
            els.append(UI.Label(text='Nenhuma bateria encontrada'))

        for bat in get_batteries():
            if bat.present:
                img = str(int((bat.charge+19)/20)*20)
            else:
                img = '0'
            st = 'Active' if bat.present else 'Offline'
            if bat.present:
                st += ' - %i%%' % bat.charge

            els.append(UI.ElementBox(UI.HContainer(
                          UI.Image(file='/dl/power/battery-%s.png'%img),
                          UI.VContainer(
                              UI.Label(text='Bateria %s' % bat.name, size=2),
                              UI.Label(text=st)
                          )
                      )))

        return ui


    @event('button/click')
    def on_aclick(self, event, params, vars=None):
        if params[0] == 'shutdown':
            os.system('halt')
        if params[0] == 'reboot':
            os.system('reboot')

