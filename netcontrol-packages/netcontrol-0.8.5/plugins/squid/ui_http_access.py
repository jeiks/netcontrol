# -*- coding: utf-8 -*-
from netcontrol import apis
from netcontrol.com import *
from netcontrol.ui import *


class SquidRules(Plugin):
    implements(apis.squid.IPluginPart)

    weight = 20
    title = 'Regras'

    tab = 0
    cfg = 0
    parent = None

    def init(self, parent, cfg, tab):
        self.parent = parent
        self.cfg = cfg
        self.tab = tab
        parent._shuffling_rules = False
        parent._adding_rules = False

    def get_ui(self):
        t = UI.DT()
        t.append(UI.DTR(UI.Label(text='Lista'), UI.Label(text='Acesso'), UI.Label(text='ACL'), UI.Label(), header=True))
        for a in self.cfg.rules:
            t.append(
                UI.DTR(
                    UI.Label(text=a[0]),
                    UI.Label(text=a[1]),
                    UI.Label(text=a[2]),
                    UI.DTD(UI.TipIcon(
                        icon='/dl/core/ui/stock/delete.png',
                        text='Excluir', id='del_rules/' + a[0] + '/' + a[1] + '/' + a[2]
                    )
                )
              ))
        vc = UI.VContainer(t,
                UI.HContainer(
                    UI.Button(text='Adicionar', id='add_rules'),
                    UI.Button(text='Aleatório', id='shuffle_rules')
                )
             )

        if self.parent._shuffling_rules:
            vc.append(self.get_ui_rules_shuffler())
        if self.parent._adding_rules:
            vc.append(self.get_ui_add())

        return vc

    def get_ui_add(self):
        li = UI.SelectInput(name='list')
        for a in self.cfg.access_lists:
            li.append(UI.SelectOption(text=a, value=a))

        c = UI.LT(
                UI.LTR(
                    UI.Label(text='Lista de acesso:'),
                    li
                ),
                UI.LTR(
                    UI.Radio(value='allow', text='Permitir', name='type', checked=True),
                    UI.Radio(value='deny', text='Negar', name='type')
                ),
                UI.LTR(
                    UI.Label(text='ACLs'),
                    UI.TextInput(name='acl')
                )
            )
        return UI.DialogBox(c, title='Adicionar regra de acesso', id='dlgAddRules')

    def get_ui_rules_shuffler(self):
        li = UI.SortList(id='list')
        for p in self.cfg.rules:
            s = ' '.join(p)
            li.append(UI.SortListItem(UI.Label(text=s), id=s))

        return UI.DialogBox(li, title='Acesso HTTP aleatório', id='dlgRules')

    def on_click(self, event, params, vars=None):
        if params[0] == 'shuffle_rules':
            self.parent._tab = self.tab
            self.parent._shuffling_rules = True
        if params[0] == 'add_rules':
            self.parent._tab = self.tab
            self.parent._adding_rules = True
        if params[0] == 'del_rules':
            self.parent._tab = self.tab
            self.cfg.rules.remove((params[1], params[2], params[3]))
            self.cfg.save()

    def on_submit(self, event, params, vars=None):
        if params[0] == 'dlgRules':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                l = vars.getvalue('list', '').split('|')
                self.cfg.rules = []
                for s in l:
                    t = s.split(' ')[0]
                    n = s.split(' ')[1]
                    v = ' '.join(s.split(' ')[2:])
                    self.cfg.rules.append((t, n, v))
                self.cfg.save()
            self.parent._shuffling_rules = False
        if params[0] == 'dlgAddRules':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                l = vars.getvalue('list', '')
                t = vars.getvalue('type', '')
                a = vars.getvalue('acl', '')
                self.cfg.rules.append((l, t, a))
                self.cfg.save()
            self.parent._adding_rules = False
