# -*- coding: utf-8 -*-
"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.ui import *
from netcontrol.com import implements
from netcontrol.api import *
from netcontrol.utils import *

from backend import *


class FirewallPlugin(CategoryPlugin):
    text = 'Firewall'
    icon = '/dl/iptables/icon.png'
    folder = 'system'

    defactions = ['ACCEPT', 'DROP', 'REJECT', 'LOG', 'EXIT', 'MASQUERADE']

    def on_init(self):
        self.cfg = Config(self.app)
        self.cfg.load()

    def on_session_start(self):
        self._tab = 0
        self._shuffling = None
        self._shuffling_table = None
        self._adding_chain = None
        self._editing_table = None
        self._editing_chain = None
        self._editing_rule = None
        self._error = None

    def get_ui(self):
        ui = self.app.inflate('iptables:main')
        if self.cfg.has_autostart():
            btn = ui.find('autostart')
            btn.set('text', 'Desativar auto iniciar')
            btn.set('id', 'noautostart')


        tc = UI.TabControl(active=self._tab)
        ui.append('root', tc)

        if len(self.cfg.tables) == 0:
            self.cfg.load_runtime()

        for t in self.cfg.tables:
            t = self.cfg.tables[t]
            vc = UI.VContainer(spacing=15)
            for ch in t.chains:
                ch = t.chains[ch]
                uic = UI.FWChain(tname=t.name, name=ch.name, default=ch.default)
                idx = 0
                for r in ch.rules:
                    uic.append(
                        UI.FWRule(
                            action=r.action,
                            desc=('' if r.action in self.defactions else r.action + ' ') + r.desc,
                            id='%s/%s/%i'%(t.name,ch.name,idx)
                        ))
                    idx += 1
                vc.append(uic)
            vc.append(UI.Button(icon='/dl/core/ui/stock/add.png', text='Adicionar nova regra em '+t.name, id='addchain/'+t.name))
            tc.add(t.name, vc)

        if self._error is not None and len(self._error) > 0:
            self.put_message('warn', self._error)
            self._error = None

        if self._shuffling != None:
            ui.append('root', self.get_ui_shuffler())

        if self._adding_chain != None:
            ui.append('root', UI.InputBox(id='dlgAddChain', text='Chain name:'))

        if self._editing_rule != None:
            ui.append('root', self.get_ui_edit_rule(
                        rule=self.cfg.tables[self._editing_table].\
                                      chains[self._editing_chain].\
                                      rules[self._editing_rule]
                    ))

        return ui

    def get_ui_edit_rule(self, rule=Rule()):
        protocols = (('TCP','tcp'), ('UDP','udp'), ('ICMP','icmp'))

        tc = UI.TabControl(active='r0')
        tc.add('Main',
            UI.Container(
                UI.Formline(
                    UI.Radio(text='Aceitar', name='caction', value='ACCEPT',     checked=rule.action=="ACCEPT"),
                    UI.Radio(text='Apagar',   name='caction', value='DROP',       checked=rule.action=="DROP"),
                    UI.Radio(text='Rejeitar', name='caction', value='REJECT',     checked=rule.action=="REJECT"),
                    UI.Radio(text='Log',    name='caction', value='LOG',        checked=rule.action=="LOG"),
                    UI.Radio(text='Masq',   name='caction', value='MASQUERADE', checked=rule.action=="MASQUERADE"),
                    UI.Radio(text='Sair',   name='caction', value='EXIT',       checked=rule.action=="EXIT"),
                    UI.Radio(text='Executar série:', name='caction', value='RUN',    checked=rule.action not in self.defactions),
                    UI.TextInput(name='runchain', value=rule.action),
                    text='Ação'
                ),
                UI.Formline(
                    rule.get_ui_select('protocol', protocols),
                    text='Protocolo'
                ),
                UI.Formline(
                    rule.get_ui_text('source'),
                    text='IP Fonte',
                    help='Você pode especificar sua faixa de IP, por exemplo: 192.168.0.0/24'
                ),
                UI.Formline(
                    rule.get_ui_text('destination'),
                    text='IP Destino',
                ),
                UI.Formline(
                    rule.get_ui_text('mac_source'),
                    text='MAC Fonte'
                ),
                UI.Formline(
                    rule.get_ui_select('in_interface', self.cfg.get_devices()),
                    text='Interface de entrada'
                ),
                UI.Formline(
                    rule.get_ui_select('out_interface', self.cfg.get_devices()),
                    text='Interface de saída'
                ),
                UI.Formline(
                    rule.get_ui_bool('fragmented'),
                    text='Fragmentação'
                ),
                UI.Formline(
                    UI.TextInput(name='modules', value=' '.join(rule.modules)),
                    text='Módulos',
                    help='Módulo IPTables adicional',
                ),
                UI.Formline(
                    UI.TextInput(name='options', value=' '.join(rule.miscopts)),
                    text='Opções adicionais',
                ),
            ), id='r0')

        tc.add('TCP/UDP',
            UI.Container(
                UI.Formline(
                    rule.get_ui_text('sport'),
                    text='Porta',
                    help='Pode-se utilizar listas e faixas como 80:85,8000 até 15 portas',
                ),
                UI.Formline(
                    rule.get_ui_text('dport'),
                    text='Porta destino'
                ),
                UI.Formline(
                    rule.get_ui_flags(),
                    text='Flags TCP',
                ),
                UI.Formline(
                    rule.get_ui_states(),
                    text='Estados TCP',
                ),
            ), id='r1')

        return UI.DialogBox(tc, id='dlgEditRule', miscbtn='Apagar', miscbtnid='deleterule')

    def get_ui_shuffler(self):
        li = UI.SortList(id='list')
        for r in self.cfg.tables[self._shuffling_table].chains[self._shuffling].rules:
            li.append(
                UI.SortListItem(
                    UI.FWRule(action=r.action, desc=r.desc, id=''),
                    id=r.raw
                ))

        return UI.DialogBox(li, id='dlgShuffler')

    @event('button/click')
    def on_click(self, event, params, vars=None):
        if params[0] == 'apply':
            self._error = self.cfg.apply_now()
        if params[0] == 'autostart':
            self.cfg.set_autostart(True)
        if params[0] == 'noautostart':
            self.cfg.set_autostart(False)
        if params[0] == 'loadruntime':
            self.cfg.load_runtime()
        if params[0] == 'setdefault':
            self._tab = self.cfg.table_index(params[1])
            self.cfg.tables[params[1]].chains[params[2]].default = params[3]
            self.cfg.save()
        if params[0] == 'shuffle':
            self._tab = self.cfg.table_index(params[1])
            self._shuffling_table = params[1]
            self._shuffling = params[2]
        if params[0] == 'addchain':
            self._tab = self.cfg.table_index(params[1])
            self._adding_chain = params[1]
        if params[0] == 'deletechain':
            self._tab = self.cfg.table_index(params[1])
            self.cfg.tables[params[1]].chains.pop(params[2])
            self.cfg.save()
        if params[0] == 'addrule':
            self._tab = self.cfg.table_index(params[1])
            self._editing_table = params[1]
            self._editing_chain = params[2]
            ch = self.cfg.tables[self._editing_table].\
                         chains[self._editing_chain]
            self._editing_rule = len(ch.rules)
            ch.rules.append(Rule('-A %s -j ACCEPT'%params[2]))
            self.cfg.save()

        if params[0] == 'deleterule':
            self.cfg.tables[self._editing_table].\
                     chains[self._editing_chain].\
                     rules.pop(self._editing_rule)
            self._editing_chain = None
            self._editing_table = None
            self._editing_rule = None
            self.cfg.save()

    @event('fwrule/click')
    def on_fwrclick(self, event, params, vars=None):
        self._tab = self.cfg.table_index(params[0])
        self._editing_table = params[0]
        self._editing_chain = params[1]
        self._editing_rule = int(params[2])

    @event('dialog/submit')
    def on_submit(self, event, params, vars):
        if params[0] == 'dlgAddChain':
            if vars.getvalue('action', '') == 'OK':
                n = vars.getvalue('value', '')
                if n == '': return
                self.cfg.tables[self._adding_chain].chains[n] = Chain(n, '-')
                self.cfg.save()
            self._adding_chain = None
        if params[0] == 'dlgShuffler':
            if vars.getvalue('action', '') == 'OK':
                d = vars.getvalue('list', '').split('|')
                ch = self.cfg.tables[self._shuffling_table].chains[self._shuffling]
                ch.rules = []
                for s in d:
                    ch.rules.append(Rule(s))
                self.cfg.save()
            self._shuffling = None
            self._shuffling_table = None
        if params[0] == 'dlgEditRule':
            if vars.getvalue('action', '') == 'OK':
                open('/tmp/teste','w').write(str(vars))
                self.cfg.tables[self._editing_table].\
                         chains[self._editing_chain].\
                          rules[self._editing_rule].apply_vars(vars)
                self.cfg.save()
            self._editing_chain = None
            self._editing_table = None
            self._editing_rule = None
