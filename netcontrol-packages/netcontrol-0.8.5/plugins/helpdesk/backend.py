#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Written by João Paulo Contantino <joaopaulo@fluxsoftwares.com> for FluxSoftwares)
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.com import *
import MySQLdb as mydb
from netcontrolldap import netcontrolldap
from pprint import pprint
from ConfigParser import ConfigParser

class Config(object):
	"""
	@author: <joaopauloctga@gmail.com>
	"""
	indice = ["nomeServidor", "enderecoServidor", "porta", "contaLDAP", "senhaContLDAP", "baseDN", "usuario", "nomeUser", "sobrenome", "email", "protSSL"]
	db 	   = None
	cursor = None
	results= None
	host   = "127.0.0.1"
	user   = "netcontrol"
	passwd = "14c75ad602ce79f4b8fd818c7ba8f88a"
    
	def __init__(self):
		self.db = mydb.connect(self.host, self.user, self.passwd, "redmine_default")
		self.cursor = self.db.cursor()
	
	def __del__(self):
		self.db.close()	
	
	def getConfig(self):
		self.cursor.execute("SELECT name, host, port, account, account_password, base_dn, attr_login, attr_firstname, attr_lastname, attr_mail, tls FROM auth_sources")
		self.results = self.cursor.fetchall()
		dic = {}
		if self.results <> ():
			for i in range(0,11):
				dic[ self.indice[ ( i ) ] ] = self.results[0][i]
			dic["fonte_config"] = "redmine"
			return dic
		else:
			for i in self.indice:
				dic[ i ] = "Sem regsitro"
			from ConfigParser import ConfigParser
			conf = ConfigParser()
			conf.read('/etc/ldap/netcontrol')
			dic["nomeServidor"] = conf.get('base','organization')
			dic["enderecoServidor"] = conf.get('base','server')
			dic["contaLDAP"]        = "cn=admin,%s"%conf.get('base','bindDN')
			dic["baseDN"]           = conf.get('base','bindDN')
			dic["senhaContLDAP"]    = conf.get('base','adminPW')
			dic["email"]     		= conf.get('base','email')
			dic["porta"]			= "389"
			dic["usuario"]	    	= "uid"
			dic["nomeUser"]			= "giverName"
			dic["sobrenome"]		= "sN"
			dic["fonte_config"] = "arquivo_ldap"
			del conf
		return dic
	
	def addUserRedmine(self, login, firstname, lastname, mail):
		sql = "INSERT INTO users VALUES(NULL,'%s','','%s','%s','%s','0','0','1','NULL','pt-BR','1','NULL','NULL','User','NULL')"\
									 %(login,firstname,lastname,mail)
		self.cursor.execute(sql)
		self.db.commit()
			
	def getUsers(self):
		self.cursor.execute("SELECT login FROM users")
		return self.cursor.fetchall()
		
	
	def delConfig(self):
		#self.cursor.execute( "SELECT id from auth_sources" )
		#confId = self.cursor.fetchall()
		#if not confId:
	#		return 
	#	print confId
		sql = "DELETE FROM auth_sources WHERE id=1"
		self.cursor.execute( sql )
		self.db.commit()
		
	
	def setConfig(self, name, host, port, account, account_pw, base_dn, attr_login, attr_firstname, attr_lastname, attr_mail, tls):
		self.delConfig()
		#result = self.cursor.execute("select * from auth_sources")
		#idConf = self.cursor.fetchall()
		#if not result:
		sql = "INSERT INTO auth_sources values(1,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',0,'%s')" % ( "AuthSourceLdap", name, host, port, account, account_pw, base_dn, attr_login, attr_firstname, attr_lastname, attr_mail, tls)
		#else:
		#	sql = "UPDATE auth_sources SET type='%s', name='%s', host='%s', port='%s', account='%s', account_password='%s', base_dn='%s', attr_login='%s', attr_firstname='%s', attr_lastname='%s', attr_mail='%s', onthefly_register=0, tls='%s' WHERE id=%s" % ( "AuthSourceLdap", name, host, port, account, account_pw, base_dn, attr_login, attr_firstname, attr_lastname, attr_mail, tls, idConf[0][0])
		self.cursor.execute(sql)
		self.db.commit()


class SyncLdapRedmine(object):
	
	LDAP = None
	conn = None
	server = None
	bindDN = None
	adminPW = None
	
	def __init__(self):
		try:
			cfg = ConfigParser()
			cfg.read('/etc/ldap/netcontrol')
			self.server  = cfg.get('base', 'server')
			self.bindDN  = cfg.get('base', 'bindDN')
			self.adminPW = cfg.get('base', 'adminPW')
			del cfg
			self.LDAP = netcontrolldap.LDAPConnection(self.server, ssl=True, admPasswd=self.adminPW, baseDN=self.bindDN)
			if not self.LDAP.getGroup("helpdesk"):
				self.LDAP.addGroup("helpdesk")
			self.conn = True
		except:
			self.conn = False
	def getUsersLDAP(self):
		try:
			return self.LDAP.getGroup("helpdesk")[0][1]["memberUid"]
		except:
			return []
	
	def getInfoUser(self, user):
		info = {}
		try:
			info["login"] = self.LDAP.getUser(user)[0][1]["uid"][0]
		except:
			info["login"] = "NULL"
		try:
			info["nome"]  = str(self.LDAP.getUser(user)[0][1]["cn"][0].split()[0])
		except:
			info["nome"] = "NULL"
		try:
			ss = ''
			for i in self.LDAP.getUser(user)[0][1]["cn"][0].split()[1:]:
				ss += str(i)+' '
			info["sobrenome"] = ss.strip()
		except:
			info["sobrenome"] = "NULL"
		try:
			info["mail"] = self.LDAP.getUser(user)[0][1]["mail"][0]
		except:
			info["mail"] = "NULL"
		return info
	
	def addUserLdaoToRed(self,login,nome, sobrenome, mail):
		Config().addUserRedmine(login, nome, sobrenome, mail)
	
	def searchUser(self, user):
		for i in self.getUsersRedmine():
			if str(i[0]).strip() == str(user).strip():
				return True
		return False
	
	def getUsersRedmine(self):
		return Config().getUsers()
	
	
	def syncDB(self):
		for i in self.getUsersLDAP():
			if not self.searchUser( i ):
				info = self.getInfoUser( i )
				print info["login"]
				self.addUserLdaoToRed( info["login"], info["nome"], info["sobrenome"], info["mail"] )


#a = Config()
#a.setConfig("name", "host", "389", "admin", "123", "admin_cn", "admin_dn", "nome", "sobrenome", "email", "0")
#a.delConfig()

#a.addUserRedmine("joaopaulo","joao paulo","constantino","joaopauloctga@gmail.com")
#a.syncDB()
