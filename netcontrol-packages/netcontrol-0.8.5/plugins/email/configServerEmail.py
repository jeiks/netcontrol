#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> e João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
from bind.configfile import ConfigFile
import os
import ConfigParser
from restart import *
class Configurar:    
    
    def getEmail(self):
        email = open('/var/lib/netcontrol/plugins/email/email').read()
        return email
    
    def conf_roundcube_apache(self):
        arquivo = open('/var/lib/netcontrol/plugins/email/templates_confs/apache_roundcube.conf','r').read()
        open('/etc/roundcube/apache.conf','w').write(arquivo)
        arq = open('/var/lib/netcontrol/plugins/email/conf/conf_roundcube_apache','r').read()
        return arq
    
    
    def config_roundcube(self,servidor,mail_dominio):
        arquivo = open('/var/lib/netcontrol/plugins/email/templates_confs/main.inc.php','r').read()
        arquivo = arquivo.replace('[SERVIDOR]',servidor).replace('[DOMINIO]',mail_dominio)
        open('/etc/roundcube/main.inc.php','w').write(arquivo)
        return arquivo
        
    def config_postfix(self,dominio=None,home=None):
        arquivo = open('/var/lib/netcontrol/plugins/email/templates_confs/main.cf','r').read()
        hostname = open('/etc/hostname','r').read()
        arquivo = arquivo.replace('[DOMINIO]',dominio).replace('[HOME]',home).replace('[HOSTNAME]',hostname)
        open('/etc/postfix/main.cf','w').write(arquivo)
        return arquivo
        
    def config_authldaprc(self):
        if os.access('/etc/ldap/netcontrol',4):
	  Config = ConfigParser.ConfigParser()
	  Config.read('/etc/ldap/netcontrol')
	  a = Config.get('base','bindDN')
	  b = Config.get('base','adminPW')
	  temp = open('/var/lib/netcontrol/plugins/email/conf/conf_ldap').read()
	  temp = temp.replace('[DC]',a)
	  temp = temp.replace('[CN]','cn=admin')
	  temp = temp.replace('[SENHA]',b)
	  open('/etc/courier/authldaprc','w').write(temp)
	  
    def config_authdaemon(self):
        a = open('/var/lib/netcontrol/plugins/email/templates_confs/authdaemonrc').read()
        open('/etc/courier/authdaemonrc','w').write(a)
    
    # dominio = nome da zona, ip do servidor
    def config_bind(self,dominio):
        zonas = ConfigFile('/etc/bind/named.conf.local')
        print dir(zonas)
        for i in zonas.getZones():
            if i.getName() == dominio:
                arq = i.getFile()
                arquivo = open(arq,'r').read()
                print arquivo
                if arquivo.find('MX') == -1:
                    mx = '@\tIN\tMX 10 '
                    indice = arquivo.find('192')
                    for j in range(14):
                        mx += arquivo[indice+j]
                    arquivo += mx
                    open(arq,'w').write(arquivo)      
    def restart(self):
        lista = ['netcontrol','apache2','bind9','postfix','courier-authdaemon','courier-imap','courier-imap-ssl','courier-ldap','courier-pop']
        for i in lista:
            restart("/etc/init.d/%s restart"%i,"True")
        return True
