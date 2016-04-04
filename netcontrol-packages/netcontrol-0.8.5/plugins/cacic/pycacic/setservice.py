# -*- coding: UTF-8 -*-

"""

    Copyright 2000, 2001, 2002, 2003, 2004, 2005 Dataprev - Empresa de Tecnologia e Informações da Previdência Social, Brasil
    
    Este arquivo é parte do programa CACIC - Configurador Automático e Coletor de Informações Computacionais
    
    O CACIC é um software livre; você pode redistribui-lo e/ou modifica-lo dentro dos termos da Licença Pública Geral GNU como 
    publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença, ou (na sua opnião) qualquer versão.
    
    Este programa é distribuido na esperança que possa ser  util, mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUAÇÂO a qualquer
    MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU para maiores detalhes.
    
    Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, escreva para a Fundação do Software
    Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

"""

import sys
import os
import stat
import commands
from hashlib import md5
import re
import tempfile

from ccrypt import CCrypt

server = None

def getDir():
    va = sys.argv[0]
    if va[0] == "/":
        return os.path.dirname(va)
    else:
        return os.path.dirname(os.getcwd()+"/"+va)

def cmd_exists(cmd):
    return os.system(cmd+' > /dev/null 2>&1') == 0

DIR = getDir()


VERSION = "2.4.4.925"

def getSOLang():
    """Retorna o idioma padrão do sistema operacional"""
    so_lang = ''
    # tenta pegar idioma das variaveis de ambiente do python
    if os.environ.has_key('LANG'):
        so_lang = os.environ['LANG']
    # caso contrario pega do sistema
    else:
        so_lang = commands.getoutput('locale | grep LANG=')        
        so_lang[len('LANG='):]
    return so_lang.split('.')[0]


def writeService():
    f = open("/etc/init.d/cacic", "w")
    f.write("#!/bin/sh\n")
    f.write("""
### BEGIN INIT INFO
# Provides:          cacic
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: pycacic
# Description:       pycacic
### END INIT INFO
    """)
    f.write('if [ "$1" = "start" ]; then\n')
    Py = commands.getoutput("which python")
    f.write("(" + Py + " /usr/share/pycacic/cacic.py > /usr/share/pycacic/logs/agent.log 2>&1)&\n")
    f.write('fi\n')
    f.close()
    os.chmod("/etc/init.d/cacic", 0755)
    
def writeStartLink(number):
    commands.getoutput("ln -sf /etc/init.d/cacic /etc/rc%s.d/S99cacic" % number)
    

def writeCron():
    f = open("/etc/cron.hourly/chksis", "w")
    f.write("#!/bin/sh\n")
    f.write("# Cacic process monitor\n")
    f.write("\n")
    f.write("Cacic=''\n")
    f.write("Cacic=`ps x | grep cacic.py | grep -v grep 2> /dev/null`\n")
    f.write('if [ "$Cacic" = "" ]; then\n')
    f.write("(python /usr/share/pycacic/cacic.py 1> /dev/null 2>&1)& \n")
    f.write("fi")
    f.close()
    os.chmod("/etc/cron.hourly/chksis", 0755)
    
def writeMD5():
    f = open(DIR+"/cacic.tar")
    content = f.read()
    f.close()
    hexmd5 = md5()
    hexmd5.update(content)
    hexmd5 = hexmd5.hexdigest()
    f = open(TARGET_DIR+"/config/MD5SUM", "w")
    f.write(hexmd5)
    f.close()
    
def writeGnomeAutoStart():
    content = []
    content.append("[Desktop Entry]")
    content.append("Name=PyCacic")
    content.append("Comment=Configurador Automático e Coletor de Informações Computacionais")
    content.append("Exec=/usr/share/pycacic/gui.py")
    content.append("Icon=/usr/share/pycacic/img/logo.png")
    content.append("StartupNotify=true")
    content.append("Terminal=false")
    content.append("Type=Application")
    content.append("Categories=Utility;System;") 
    # AutoStart
    if os.path.exists('/etc/xdg/autostart/'):
        f = open("/etc/xdg/autostart/pycacic.desktop", "w")
        f.write('\n'.join(content))    
        f.close()
    # Menu Item
    if os.path.exists('/usr/share/applications/'):
        f = open("/usr/share/applications/pycacic.desktop", "w")
        f.write('\n'.join(content))
        f.close()
    
def install():
    print "Unpacking installation...",
    unpack()
    print "[OK]"
    
    """
    print "Installing PyCacic Service...",
    writeService()
    writeStartLink(2)
    writeStartLink(3)
    writeStartLink(4)
    writeStartLink(5)
    print "[OK]"
    """
    
    print "Generating Version Hash",
    writeMD5()
    print "[OK]"
    """
    print "Adding to cron ",
    writeCron()
    print "[OK]"
    
    print "Adding to Gnome AutoStart ",
    writeGnomeAutoStart()
    print "[OK]"
    """
    mkconfig()

def execute(cmd):
    return os.system(cmd+' > /dev/null 2>&1') == 0

def unpack():
    print "UPACKING ON "+TARGET_BASE
    execute('tar -xf '+DIR+'/cacic.tar -C '+TARGET_BASE)

def isPreconfigured():
    return not os.path.exists("/usr/share/pycacic/config/cacic.conf") and os.path.exists("/usr/share/pycacic/config/cacic.dat")

def configAndPackage(force = 0):
    """ Configura e opcionalmente gera um novo pacote """
    if not isPreconfigured() or force:
        mkconfig()
        resp = ''
        while (not resp in ('S', 'Y', 'N')):
            #resp = raw_input("Deseja gerar um novo pacote de instalação pre-configurado (Formato: .tgz) ? (Y/N)")
            resp = 'Y' #resp.upper()
        if resp in ('S', 'Y'):
            import os
            os.system("tar -C /usr/share -cf "+DIR+"/cacic.tar pycacic/")
            #print DIR+"/cacic.tar foi substituido pela versao configurada"
            tarname = "pycacic_"+VERSION+".preconf.tgz"
            os.system("tar --preserve-permissions -C "+DIR+"/.. -czf /tmp/"+tarname+" pycacic/")
            print "Gerado pacote de instalacao pre-configurado: /tmp/"+tarname
        resp = ''
        while (not resp in ('S', 'Y', 'N')):
            #resp = raw_input("Deseja gerar um novo pacote de instalação pre-configurado (Formato: .deb) ? (Y/N)")
            resp = 'Y' #resp.upper()
        if resp in ('S', 'Y'):
            import os
            resp = ''
            while (not resp in ('S', 'Y', 'N')):
                #resp = raw_input("Deseja que o coletor patrimonial seja invocado automaticamente após a instalação? (Y/N)")
                resp = 'Y' #resp.upper()
            if resp in ('S', 'Y'):
                f = open(DIR+"/gdeb/postinst" , 'a')
                str = 'if [ "$1" = "configure" ]; then\n'
                str+= '    if [ "$DISPLAY" = "" ]; then\n'
                str+= '        (nohup python /usr/share/pycacic/mapacacic.py > /dev/null 2>&1)\n'
                str+= '    else\n'
                str+= '        (nohup python /usr/share/pycacic/guimapacacic.py > /dev/null 2>&1)&\n'
                str+= '    fi\n'
                str+= 'fi\n'
                f.write(str)
                f.close()
            os.chmod(DIR+"/gdeb/gera-deb.sh", 0755)
            os.system('cd '+DIR+'/gdeb/;'+DIR+"/gdeb/gera-deb.sh")
    else:
        print "Preconfiguracao detectada!"


def mkconfig():
    """Abre console para configuracao do PyCacic"""
    from io import Writer
    print "\n- Bem-Vindo ao gerador de Pacotes do PyCacic ---"
    print "\n- Após preencher as informacoes abaixo os pacotes poderao ser gerados\n"
    op = ''
    while not op in ('S', 'Y'):
        #addr = raw_input("Endereço do  Servidor ('ex: http://<endereco>'): ").lower()
        addr = server
        if len(addr.split('//')) != 2:
            print "Endereco invalido"
            op = ''
        else:
            http = addr.split('//')[0]
            host = addr.split('//')[1]
            if not http in ('http:', 'https:') or host.strip() == '':
                print "Endereco invalido"
                op = ''            
            elif host.strip() == "localhost" or host.strip() == "127.0.0.1":
                print "Não é permitido utilizar um endereço da interface de loopback(127.0.0.1 ou localhost) para o gerente"
                op = ''
            else:            
                print "Testando conexão...",
                if commands.getoutput('ping %s -c 1; echo $?' % host)[-1:] != '0':
                    print "Erro ao tentar conectar ao servidor"
                    op = ''
                else:
                    print "[OK]"
                    #op = raw_input("\nOs dados estao corretos? (Y/N)").upper()
                    op= 'Y'
    # remove a barra do final
    if addr[len(addr)-1] == '/':
        addr = addr[:-1]
    # sava as configuracoes
    Writer.setServer('address', addr, CACIC_CONF, False)
    Writer.setServer('username', "USER_CACIC", CACIC_CONF, False)
    Writer.setServer('password', "PW_CACIC", CACIC_CONF, False)
    DEF_LANG = getSOLang()
    if DEF_LANG.find('LANG='):
        DEF_LANG = 'pt_BR'
    Writer.setPycacic('locale', DEF_LANG, CACIC_CONF, False)
    
    print "- Salvando e encriptando configuracao...",
    
    f = open(CACIC_CONF)
    content = f.read()
    f.close()
    
    cipher = CCrypt()
    crypted = cipher.encrypt(content)
    
    f = open(CACIC_CONF_ENC, "w")
    f.write(crypted)
    f.close()
    
    os.unlink(CACIC_CONF)
    print "[OK]"
    
    resp = ''
    while (not resp in ('S', 'Y', 'N')):
        #resp = raw_input("Deseja que o coletor patrimonial seja invocado automaticamente após a instalação? (Y/N)")
        resp = 'Y' #resp.upper()
    if resp in ('S', 'Y'):
        f = open(DIR+"/internal/postinst" , 'a')
        str = 'if [ "$$DISPLAY" = "" ]; then\n'
        str+= '    (nohup python /usr/share/pycacic/mapacacic.py > /dev/null 2>&1)\n'
        str+= 'else\n'
        str+= '    (nohup python /usr/share/pycacic/guimapacacic.py > /dev/null 2>&1)&\n'
        str+= 'fi\n'
        f.write(str)
        f.close()

def mkPackage(type, arch):
    if arch != '':
        ap = '-a '+arch
    else:
        ap = ''
    os.chmod('/.'+DIR+'/epm', 0755)
    output = 0
    output = execute('/.'+DIR+'/epm --output-dir '+DIR+' -g '+ap+' -f '+type+' pycacic '+DIR+'/pycacic.list')
    os.chmod('/.'+DIR+'/epm', 0644)
    return output

def mkDistList():
    f = open(DIR+'/pycacic.list', 'w')
    
    list = []
    list.append('%product PyCACIC')
    list.append('%copyright Dataprev')
    list.append('%vendor Dataprev')
    list.append('%license '+DIR+'/COPYING')
    list.append('%readme '+DIR+'/README')
    list.append('%format !portable')
    list.append('%requires python 2.3.0')
    list.append('%format all')
    list.append('%description Configurador Automatico e Coletor de Informacoes Computacionais')
    list.append('%version '+VERSION)
    list.append('%postinstall <'+DIR+'/internal/postinst')
    list.append('%preremove <'+DIR+'/internal/prerm')
    list.append('%system all\n\n')
    f.write('\n'.join(list))
    mkDirListImpl(TARGET_DIR, f)
    appendInitList(f)
    appendCron(f)
    appendDesktop(f)
    f.close()
    
def mkDirListImpl(path, f):
    if os.path.isdir(path):
        f.write('d 0755 root root '+src2dest(path)+' -\n')
        for entry in os.listdir(path):
            mkDirListImpl(path+'/'+entry, f)
    elif os.path.isfile(path):
        chmod = stat.S_IMODE(os.lstat(path)[stat.ST_MODE])
        f.write('f '+oct(chmod)+' root root '+src2dest(path)+' '+path+'\n')

def src2dest(path):
    return path.replace(TARGET_BASE, "/usr/share")
    
def appendInitList(f):
    f.write('i 755 root sys cacic '+DIR+'/internal/cacic\n')

def appendCron(f):
    f.write('f 755 root sys /etc/cron.hourly/chksis '+DIR+'/internal/chksis\n')

def appendDesktop(f):
     f.write('f 644 root sys /usr/share/applications/pycacic.desktop '+DIR+'/internal/pycacic.desktop\n')
     f.write('f 644 root sys /etc/xdg/autostart/pycacic.desktop '+DIR+'/internal/pycacic.desktop\n')


def clean():
    execute('rm -Rf '+TARGET_BASE)

def cmd_exists(cmd):
    return os.system(cmd+' > /dev/null 2>&1') == 0

if __name__ == '__main__':
    import sys
    try:
        server = sys.argv[1]
    except:
        sys.exit(1)
    print "Unpacking necessary files...",
    TARGET_BASE = tempfile.mkdtemp('pycacic')
    TARGET_DIR = TARGET_BASE+"/pycacic"
    CACIC_CONF = TARGET_DIR+"/config/cacic.conf"
    CACIC_CONF_ENC = TARGET_DIR+"/config/cacic.dat"
    unpack()
    print "[OK]"
    
    print "Generating Version Hash",
    writeMD5()
    print "[OK]"
    
    mkconfig()
    
    mkDistList()
    choice = ''
    output = ''
    types = ['deb', 'rpm', 'portable']
    cmds = ['dpkg', 'rpmbuild', '']
    archs = ['all', 'noarch', 'noarch']
    while choice != '5':
        os.system('clear')
        print '\n'
        print '\t1 - Debian Package (.deb)'
        print '\t2 - RPM Package Manager (.rpm)'
        print '\t3 - Generic Install for others distributions'
        print '\t---'
        print '\t4 - Update Package'
        print '\t---'
        print '\t5 - Exit'
        print '\n\tPackages Destination: '+DIR
        print '\n\t%s' % output
        choice = ''
        while choice not in ('1', '2', '3', '4', '5'):
            choice = '1' #raw_input('\n\tChoice: ').strip()
        if choice != '4' and choice != '5':
            type = types[int(choice) - 1]
            arch = archs[int(choice) - 1]
            print '\n\t-> Generating ('+type+') package ',
            output = mkPackage(type, arch)
            if output:
                output = 'Package successfully created.'
                print '[OK]\n'
            else:
                print '[FAILED]'
                output = 'Packaging failed.'
                command = cmds[int(choice) - 1]
                if command != '' and not cmd_exists(command):
                    output += "\n\t- Command: '"+command+"' is required for building this type of package.\n"
            choice = '5'
        elif choice == '4':
            print "\n\t-> Generating update package for Web auto-update ",
            tarname = "pycacic_"+VERSION+".update.tgz"
            os.system("tar --preserve-permissions -C "+TARGET_DIR+"/.. -czf "+DIR+"/"+tarname+" pycacic/")
            print "[OK]\n"
    clean()
    print "-- Done"
    
    
