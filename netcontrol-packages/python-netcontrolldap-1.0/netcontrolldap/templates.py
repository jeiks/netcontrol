#-*- coding: utf-8 -*-

templateLogon="""\
Option Explicit
Dim objNetwork, objShell
On Error Resume Next

Set objShell   = CreateObject("Shell.Application")
Set objNetwork = CreateObject("WScript.Network")

' mapping home directory
objNetwork.RemoveNetworkDrive "H:"
objNetwork.MapNetworkDrive "H:", "\\\\@SERVER@\\homes"
objShell.NameSpace("H:").Self.Name = "Pasta Pessoal"

' mapping public directory
objNetwork.RemoveNetworkDrive "P:"
objNetwork.MapNetworkDrive "P:", "\\\\@SERVER@\\public"
objShell.NameSpace("P:").Self.Name = "Pasta Pública"


WScript.Quit
"""

templateSMB="""\
[global]
        workgroup = @WORKGROUP@
        netbios name = @SERVER@
        enable privileges = yes
        server string = Servidor Samba %v
        obey pam restrictions = No
        ldap passwd sync = Yes
        log level = 2
        syslog = 0
        log file = /var/log/samba/log.%m
        max log size = 100000
        time server = Yes
        socket options = TCP_NODELAY SO_RCVBUF=8192 SO_SNDBUF=8192
        mangling method = hash2
        Dos charset = cp850
        Unix charset = utf-8

        winbind cache time = 5
        idmap uid = 1000-3000
        idmap gid = 1000-3000
        logon script = logon.vbs
        logon drive  = Z:
        logon home   =
        logon path   =

        domain logons = Yes
        os level = 65
        preferred master = Yes
        domain master = Yes
        wins support = Yes
        passdb backend = ldapsam:ldap://localhost/
        ldap admin dn = cn=admin,@BINDDN@
        ldap suffix   = @BINDDN@
        ldap group suffix = @ORGGROUPS@
        ldap user suffix  = @ORGUSERS@
        ldap machine suffix = @ORGCOMP@
        ldap idmap suffix = @ORGUSERS@
        idmap backend = ldap:ldap://localhost/
        ldap ssl  = @SSL@
        ldap delete dn = Yes
        add machine script = @ADDMACHSCRIPT@ "%u"

        # printers configuration
        printer admin = @"Print Operators"
        load printers = Yes
        create mask = 0640
        directory mask = 0750
        nt acl support = No
        printing = cups
        printcap name = cups
        deadtime = 10
        guest account = nobody
        map to guest = Bad User
        dont descend = /proc,/dev,/etc,/lib,/lost+found,/initrd
        show add printer wizard = yes
        ; to maintain capital letters in shortcuts in any of the profile folders:
        preserve case = yes
        short preserve case = yes
        case sensitive = no
        @NEWGLOBALITEMS@

[homes]
        comment = Pasta de %U, %u
        path = /home/%U/@HOMEWINFILES@
        read only = No
        create mask = 0644
        directory mask = 0775
        browseable = No

[netlogon]
        path = /home/samba/netlogon/
        browseable = No
        read only = yes

[profiles]
        path = /home/samba/profiles
        read only = no
        create mask = 0600
        directory mask = 0700
        browseable = No
        guest ok = Yes
        profile acls = yes
        csc policy = disable
        force user = %U 
        valid users = %U @"Domain Admins"

[printers]
        comment = Impressoras da Rede
        printer admin = @"Print Operators"
        guest ok = yes 
        printable = yes
        path = /home/samba/spool/
        browseable = No
        read only  = Yes
        printable = Yes
        print command = /usr/bin/lpr -P%p -r %s
        lpq command = /usr/bin/lpq -P%p
        lprm command = /usr/bin/lprm -P%p %j

[print$]
        path = /home/samba/printers
        guest ok = No
        browseable = Yes
        read only = Yes
        valid users = @"Print Operators"
        write list = @"Print Operators"
        create mask = 0664
        directory mask = 0775

[public]
        comment = Pasta Publica
        path = /home/samba/public
        browseable = Yes 
        guest ok = Yes
        read only = No
        directory mask = 0775
        create mask = 0664
"""
