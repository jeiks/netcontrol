#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por João Paulo Constantino <joaopauloctga@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
"""
import MySQLdb as mysql
import tarfile
import os

class InstallCacic(object):
  """
  @author: <joaopauloctga@gmail.com>
  """
  servidor = None
  user = None
  senha = None
  dirGerente = '/var/lib/netcontrol/plugins/cacic/gerenteCacic/gerente_cacic.tar.gz'
  extractGerente = '/var/www/'
  dirr = '/var/lib/netcontrol/plugins/cacic/gerenteCacic/'
  
  def __init__(self,servidor='localhost',user='cacic_db_user',senha='123456'):
    self.servidor = servidor
    self.user = user
    self.senha = senha
    
    
  def setFilesCacic(self):
    # funcao para descompactar os arquivos na pasta /var/www/<caicic>
    tar = tarfile
    a = tar.open(self.dirGerente)
    if a.extractall(path=self.extractGerente) == None:
      return True
  
  def setAcesso(self):
    # funcao para modificar acessos da pasta
    a = os.system('chown -R www-data.www-data /var/www/cacic2/')
    os.system('chmod -R 755 /var/www/cacic2/')
    if a == 0:
      return True
    else:
      return False
  
  def installDB(self):
    # funcao para instalar o banco de dados
    a = os.system('/usr/bin/mysql --defaults-file=/etc/mysql/debian.cnf < /var/lib/netcontrol/plugins/cacic/gerenteCacic/localhost.sql')
    if a == 0:
      return True
    else:
      return False
    
    
  def setConfig(self):
    # funcao para relizar configuracoes necessarias em arquivos
    try:
      a = open('%sphp.ini'%self.dirr).read()
      open('/etc/php5/apache2/php.ini','w').write(a)
      open('%sserver'%self.dirr,'w').write(str(True))
      return True
    except:
     return False
    
  def delCacic(self):
    # funcao para apagar todos os dados do servidor cacic
    # apagando arquivos da pasta /var/www/cacic2
      os.system('rm -rf /var/www/cacic2/')
      os.system( "echo 'drop user cacic_db_user;' | /usr/bin/mysql --defaults-file=/etc/mysql/debian.cnf "  )
      os.system( "echo 'drop user cacic_db_user@localhost;' | /usr/bin/mysql --defaults-file=/etc/mysql/debian.cnf "  )
      a = os.system("echo 'drop database cacic;' | /usr/bin/mysql --defaults-file=/etc/mysql/debian.cnf ")
      if a == None:
        return True
      else:
        return False
    
