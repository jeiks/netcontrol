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
    

    Modulo globals
    
    Modulo contendo variáveis globais do sistema
    
    @author: Dataprev - ES

"""

import os
import re
import sys
import commands


class Globals:
        
    PATH = ""
    INSTALLED = 0 # False
    PC_XML = ""
    
    def __init__(self):
        pass
    
    def getSocketAttr():
        """Retorna as informacoes do Socket"""
        from config.io import Reader
        sock = Reader.getSocket()
        host = sock['host']
        port = int(sock['port'])
        buf  = int(sock['buffer'])
        addr = (host, port)
        return host, port, buf, addr
    
    def install():
        """Abre console para configuracao do PyCacic"""
        from io import Writer
        from lang.language import Language
        print "\n\t--- Bem-Vindo a Configuracao do PyCacic ---"
        print "\n\tapos preencher as informacoes abaixo o programa ira iniciar\n"
        addr = raw_input("End. do  Servidor ('ex: http://10.0.0.1'): ")
        print "Testando conexao..."
        p = re.compile('[0-9]{1,3}(?:\.[0-9]{1,3}){3}')
        if len(p.findall(addr)) == 0:
            print "Endereco invalido"
            Globals.install()
            return 
        ip = p.findall(addr)[0]            
        if commands.getoutput('ping %s -c 1; echo $?' % ip)[-1:] != '0':
            print "Erro ao tentar conectar ao servidor"
            Globals.install()
            return
        user = raw_input("Usuario do Servidor: ")
        pwd = raw_input("Senha: ")
        if raw_input("\n\t*** Os dados estao corretos? [y|n]").lower() != 'y':
            Globals.install()
            return
        Writer.setPycacicStatus('installed', 1)
        if addr[len(addr)-1] == '/': addr = addr[:-1]
        Writer.setServer('address', addr)
        Writer.setServer('username', user)
        Writer.setServer('password', pwd)
        # salva idioma padrao
        Writer.setPycacic('locale', Language().getSOLang())
        print "\t--- Configuracao concluida com sucesso ---\n\n"
        
    getSocketAttr = staticmethod(getSocketAttr)
    install = staticmethod(install)
# fim classe

# staticos
def getDir():
    va = sys.argv[0]
    if va[0] == "/":
        return os.path.dirname(va)
    else:
        return os.path.dirname(os.getcwd()+"/"+va)
        
def getArgs():
    for arg in sys.argv:
        if arg[0:4] == "-xml":
            Globals.PC_XML = arg[5:]

def isInstalled():
    from config.io import Reader
    return (Reader.getPycacicStatus('installed')['value'] == 'yes')


    
Globals.PATH = getDir()

getArgs()