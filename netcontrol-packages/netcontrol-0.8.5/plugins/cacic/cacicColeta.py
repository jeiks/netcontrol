#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
import MySQLdb

import MySQLdb as my

class ConfigCacic(object):
    """
    @author: <joaopauloctga@gmail.com>
    """
    def __init__(self):
        print 'to-do'
		
    def setConfigAgentes(self,inicio_coleta,senha,intervalo_coleta):
        try:    exibi = exibi is None and 'NULL' or str(exibi)
        except: pass
        try:    inicio_coleta = inicio_coleta is None and 'NULL' or str(inicio_coleta)
        except: pass
        try:    senha = senha is None and 'NULL' or str(senha)
        except: pass
        try:    intervalo_coleta = intervalo_coleta is None and 'NULL' or str(intervalo_coleta)
        except: pass
        
        db = my.connect('localhost','cacic_db_user','123456','cacic')
        cursor = db.cursor()
        sql = ('UPDATE configuracoes_locais SET nu_exec_apos="%s", te_senha_adm_agente="%s", nu_intervalo_exec="%s" ') % (inicio_coleta, senha, intervalo_coleta)
        cursor.execute(sql)
        db.commit()
        db.close()
		
    def getConfigAgentes(self):
        db = my.connect('localhost','cacic_db_user','123456','cacic')
        cursor = db.cursor()
        sql = 'select nu_exec_apos,te_senha_adm_agente,nu_intervalo_exec from configuracoes_locais'
        cursor.execute(sql)
        a = cursor.fetchall()
        open('/tmp/getAgentes','w').write(str(a))
        return a




class Redes:

    def __init__(self,ip='localhost',user='cacic_db_user',senha='123456'):
        self.ip = ip
        self.user = user
        self.senha = senha


    def getRede(self,ip, masc):
        ip = str(ip)
        ip = ip.split('.')
        rede = ''
        masc = str(masc)
        if masc == "None":
          masc = '255.255.255.0'
          masc = masc.split('.')
        else:
          masc = masc.split('.')
        
        for i in range(4):
          rede += str( int(ip[i]) & int(masc[i]) ) + '.'
        
        return rede
        
        
    def getComputadores(self):
        try:
			con = MySQLdb.connect(self.ip,self.user,self.senha,'cacic')
        except:
			return False
			pass
        cursor = con.cursor()
        cursor.execute('select te_ip,te_mascara from computadores')
        pcs =  cursor.fetchall()
        a = {}
        for i in pcs:
            if not a.has_key( self.getRede(i[0], i[1]) ):
                a[self.getRede(i[0], i[1])] = []
                a[self.getRede(i[0], i[1])].append(i[0])
            else:
                a[self.getRede(i[0], i[1])].append(i[0])
        return a


class Coleta:
    indice = ["MAC Address","ID - SO","Sistema Operacional","Nome do Computador","ID - IP Rede","Dominio Windows","Dominio - DNS","Descrição Placa de Video","IP - Computador","Máscara","Host","Descrição da Placa de Rede","Data de Inclusão","Gateway","Wins Primário","Descrição da CPU","Wins Secundário","DNS - Primário","Memória da Placa de Video(MB)","DNS - Secundário","Descrição da Placa Mãe","Servidor DCHP","Memória RAM","Serial da CPU","Fabricante - CPU","Frenquência - CPU","Descrição - Memória RAM","Descrição da BIOS","Data - BIOS","Ultimo Acesso","Versão Cacic","Versão Gercols","Fabricante - BIOS","Palavra Chave","Fabricante Placa Mãe","Quantidade de Cores da Placa de Video","Resolução Placa de Video","Descrição - Placa de Som","Descrição - CD-ROM","Descrição - Teclado","Descrição - Mouse","Descrição - Modem","Workgrup","Ultima Coleta Forçada do Cacic","Modulos","Origem MAC","ID - Conta","","","","","","","","",]
    # lista todos os computadores e suas informações

    def __init__(self,ip='localhost',user='cacic_db_user',senha='123456'):
        self.ip = ip
        self.user = user
        self.senha = senha
    
    def getServer(self):
      return open('/var/lib/netcontrol/plugins/cacic/gerenteCacic/server').read().split('\n')
    
    def getComputador(self):
        try:
            con = MySQLdb.connect(self.ip,self.user,self.senha,'cacic')
        except:
            return False
        
        cursor = con.cursor()
        cursor.execute("SELECT * FROM computadores")
        pcs = cursor.fetchall()
        pc = []
        for i in pcs:
            pc.append(i)
        return pc
