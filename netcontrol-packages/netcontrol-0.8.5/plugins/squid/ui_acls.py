# -*- coding: utf-8 -*-
from netcontrol import apis
from netcontrol.com import *
from netcontrol.ui import *


class SquidACLs(Plugin):
    implements(apis.squid.IPluginPart)

    weight = 20
    title = 'ACLs'

    tab = 0
    cfg = 0
    parent = None

    acl_types = [
        ('Source IP', 'src'),
        ('Destination IP', 'dst'),
        ('URL regex', 'url_regex'),
        ('User-agent regex', 'browser'),
        ('Client local IP', 'myip'),
        ('Client MAC', 'arp'),
        ('Source domain', 'srcdomain'),
        ('Destination domain', 'dstdomain'),
        ('Source domain regex', 'srcdom_regex'),
        ('Destination domain regex', 'dstdom_regex'),
        ('Souce AS number', 'src_as'),
        ('Destination AS number', 'dst_as'),
        ('Cache peer name', 'peername'),
        ('Time', 'time'),
        ('URL-path regex', 'urlpath_regex'),
        ('Destination port', 'port'),
        ('Squid port', 'myport'),
        ('Protocol', 'proto'),
        ('HTTP method', 'method'),
        ('HTTP status code', 'http_status'),
        ('HTTP referer regex', 'referer_regex'),
        ('Ident', 'ident'),
        ('Ident regex', 'ident_regex'),
        ('External process auth', 'proxy_auth'),
        ('External process auth regex', 'proxy_auth_regex'),
        ('SNMP community', 'snmp_community'),
        ('Connections per IP limit', 'maxconn'),
        ('IP per user limit', 'max_user_ip'),
        ('Request MIME regex', 'req_mime_type'),
        ('Request header regex', 'req_header'),
        ('Reply MIME regex', 'rep_mime_type'),
        ('Reply header regex', 'rep_header'),
        ('External helper', 'external'),
        ('User certificate', 'user_cert'),
        ('CA certificate', 'ca_cert'),
        ('External helper - user', 'ext_user'),
        ('External helper - user regex', 'ext_user_regex')
       ]

    def init(self, parent, cfg, tab):
        self.parent = parent
        self.cfg = cfg
        self.tab = tab
        parent._shuffling_acls = False
        parent._adding_acls = False
        parent._editing_acl = ''

    def get_ui(self):
        t = UI.DT()
        t.append(UI.DTR(
            UI.DTH(UI.Label(text='Nome')), 
            UI.DTH(UI.Label(text='Tipo')), 
            UI.DTH(UI.Label(text='Valor')),
            UI.DTH(), 
            header=True))
        for a in self.cfg.acls:
            try:
                tp = filter(lambda x: x[1] == a[1], self.acl_types)[0][0]
            except:
                tp = a[1]
            t.append(
                UI.DTR(
                    UI.Label(text=a[0]),
                    UI.Label(text=tp),
                    UI.Label(text=a[2]),
                    UI.HContainer(
                        UI.TipIcon(icon='/dl/core/ui/stock/edit.png', text='Editar', id='edit_acl/' + a[0]),
                        UI.TipIcon(icon='/dl/core/ui/stock/delete.png', text='Excluir', id='del_acl/' + a[0] + '/' + a[1] + '/' + a[2])
                    ),
                )
              )
        vc = UI.VContainer(t,
                UI.HContainer(
                    UI.Button(text='Adicionar', id='add_acl'),
                    UI.Button(text='Aleatório', id='shuffle_acls')
                )
             )

        if self.parent._shuffling_acls:
            vc.append(self.get_ui_acls_shuffler())
        if self.parent._adding_acls:
            vc.append(self.get_ui_add())
        if self.parent._editing_acl != '':
            a = filter(lambda x: x[0] == self.parent._editing_acl, self.cfg.acls)[0]
            vc.append(self.get_ui_edit(a[0], a[1], a[2]))

        return vc

    def get_ui_add(self):
        li = UI.Select(name='type')
        for (d, v) in self.acl_types:
            li.append(UI.SelectOption(text=d, value=v))

        c = UI.HContainer(
                UI.LT(
                    UI.LTR(
                        UI.Label(text='Nome:'),
                        UI.TextInput(name='name')
                    ),
                    UI.LTR(
                        UI.Label(text='Tipo:'),
                        li
                    ),
                    UI.LTR(
                        UI.Label(text='Parâmetro:'),
                        UI.TextInput(name='value')
                    )
                )
            )
        return UI.DialogBox(c, title='Adicionar ACL', id='dlgAddACL')

    def get_ui_edit(self, n, t, p):
        li = UI.Select(name='type')
        for (d, v) in self.acl_types:
            li.append(UI.SelectOption(text=d, value=v, selected=(v==t)))

        c = UI.HContainer(
                UI.LT(
                    UI.LTR(
                        UI.Label(text='Nome:'),
                        UI.TextInput(name='name', value=n)
                    ),
                    UI.LTR(
                        UI.Label(text='Tipo:'),
                        li
                    ),
                    UI.LTR(
                        UI.Label(text='Parâmetro:'),
                        UI.TextInput(name='value', value=p)
                    )
                )
            )
        return UI.DialogBox(c, title='Editar ACL', id='dlgEditACL')

    def get_ui_acls_shuffler(self):
        li = UI.SortList(id='list')
        for p in self.cfg.acls:
            s = ' '.join(p)
            li.append(UI.SortListItem(UI.Label(text=s), id=s))

        return UI.DialogBox(li, title='ACLs Aleatórios', id='dlgACLs')

    def on_click(self, event, params, vars=None):
        if params[0] == 'shuffle_acls':
            self.parent._tab = self.tab
            self.parent._shuffling_acls = True
        if params[0] == 'add_acl':
            self.parent._tab = self.tab
            self.parent._adding_acls = True
        if params[0] == 'del_acl':
            self.parent._tab = self.tab
            self.cfg.acls.remove((params[1], params[2], params[3]))
        if params[0] == 'edit_acl':
            self.parent._tab = self.tab
            self.parent._editing_acl = params[1]

    def on_submit(self, event, params, vars=None):
        if params[0] == 'dlgACLs':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                l = vars.getvalue('list', '').split('|')
                self.cfg.acls = []
                for s in l:
                    n = s.split(' ')[0]
                    t = s.split(' ')[1]
                    v = ' '.join(s.split(' ')[2:])
                    self.cfg.acls.append((n, t, v))
                self.cfg.save()
            self.parent._shuffling_acls = False
        if params[0] == 'dlgAddACL':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                n = vars.getvalue('name', '')
                t = vars.getvalue('type', '')
                v = vars.getvalue('value', '')
                self.cfg.acls.append((n, t, v))
                self.cfg.save()
            self.parent._adding_acls = False
        if params[0] == 'dlgEditACL':
            self.parent._tab = self.tab
            if vars.getvalue('action', '') == 'OK':
                n = vars.getvalue('name', '')
                t = vars.getvalue('type', '')
                v = vars.getvalue('value', '')
                for x in range(0, len(self.cfg.acls)):
                    if self.cfg.acls[x][0] == self.parent._editing_acl:
                        self.cfg.acls[x] = (n, t, v)
                self.cfg.save()
            self.parent._editing_acl = ''
