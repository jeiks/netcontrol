# -*- coding: utf-8 -*-
from netcontrol import apis
from netcontrol.com import *
from netcontrol.ui import *


class SquidRefPats(Plugin):
    implements(apis.squid.IPluginPart)

    weight = 40
    title = 'Padrões de atualização'

    tab = 0
    cfg = 0
    parent = None


    def init(self, parent, cfg, tab):
        self.parent = parent
        self.cfg = cfg
        self.tab = tab
        parent._adding_ref_pat = False
        parent._editing_ref_pat = -1

    def get_ui(self):
        t = UI.DT()
        t.append(UI.DTR(
                UI.Label(text='Expressão regular'),
                UI.Label(text='Min'),
                UI.Label(text='%'),
                UI.Label(text='Max'),
                UI.Label(text='Opções'),
                UI.Label(),
                header=True
              ))

        i = 0
        for a in self.cfg.ref_pats:
            t.append(
                UI.DTR(
                    UI.Label(text=a[0]),
                    UI.Label(text=a[1]),
                    UI.Label(text=a[2]),
                    UI.Label(text=a[3]),
                    UI.Label(text=a[4]),
                    UI.HContainer(
                        UI.TipIcon(icon='/dl/core/ui/stock/edit.png', text='Editar', id='edit_ref_pat/' + str(i)),
                        UI.TipIcon(icon='/dl/core/ui/stock/delete.png', text='Excluir', id='del_ref_pat/' + str(i))
                    ),
                )
              )
            i += 1

        vc = UI.VContainer(
                 t,
                 UI.Button(text='Adicionar', id='add_ref_pat'),
             )

        if self.parent._adding_ref_pat:
            vc.append(self.get_ui_add())
        if self.parent._editing_ref_pat != -1:
            a = self.cfg.ref_pats[self.parent._editing_ref_pat]
            vc.append(self.get_ui_edit(a))

        return vc

    def get_ui_add(self):
        c = UI.HContainer(
                UI.LT(
                    UI.LTR(
                        UI.Label(text='Expressão regular:'),
                        UI.TextInput(name='regex')
                    ),
                    UI.LTR(
                        UI.Label(text='Min:'),
                        UI.TextInput(name='min')
                    ),
                    UI.LTR(
                        UI.Label(text='Porcentagem:'),
                        UI.TextInput(name='perc')
                    ),
                    UI.LTR(
                        UI.Label(text='Max:'),
                        UI.TextInput(name='max')
                    ),
                    UI.LTR(
                        UI.Label(text='Opções:'),
                        UI.TextInput(name='opts')
                    )
                )
            )
        return UI.DialogBox(c, title='Adicionar padrão de atualização', id='dlgAddRefPat')

    def get_ui_edit(self, a):
        c = UI.HContainer(
                UI.LT(
                    UI.LTR(
                        UI.Label(text='Expressão regular:'),
                        UI.TextInput(name='regex', value=a[0])
                    ),
                    UI.LTR(
                        UI.Label(text='Min:'),
                        UI.TextInput(name='min', value=a[1])
                    ),
                    UI.LTR(
                        UI.Label(text='Porcentagem:'),
                        UI.TextInput(name='perc', value=a[2])
                    ),
                    UI.LTR(
                        UI.Label(text='Max:'),
                        UI.TextInput(name='max', value=a[3])
                    ),
                    UI.LTR(
                        UI.Label(text='Opções:'),
                        UI.TextInput(name='opts', value=a[4])
                    )
                )
            )
        return UI.DialogBox(c, title='Editar padrão de atualização', id='dlgEditRefPat')

    def on_click(self, event, params, vars=None):
        if params[0] == 'add_ref_pat':
            self.parent._tab = self.tab
            self.parent._adding_ref_pat = True
        if params[0] == 'del_ref_pat':
            self.parent._tab = self.tab
            self.cfg.ref_pats.pop(int(params[1]))
        if params[0] == 'edit_ref_pat':
            self.parent._tab = self.tab
            self.parent._editing_ref_pat = int(params[1])

    def on_submit(self, event, params, vars=None):
        if params[0] == 'dlgAddRefPat':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                r = vars.getvalue('regex', '')
                mn = vars.getvalue('min', '')
                p = vars.getvalue('perc', '')
                mx = vars.getvalue('max', '')
                o = vars.getvalue('opts', '')
                self.cfg.ref_pats.append((r, mn, p, mx, o))
                self.cfg.save()
            self.parent._adding_ref_pat = False
        if params[0] == 'dlgEditRefPat':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                r = vars.getvalue('regex', '')
                mn = vars.getvalue('min', '')
                p = vars.getvalue('perc', '')
                mx = vars.getvalue('max', '')
                o = vars.getvalue('opts', '')
                self.cfg.ref_pats[self.parent._editing_ref_pat] = (r, mn, p, mx, o)
                self.cfg.save()
            self.parent._editing_ref_pat = -1
