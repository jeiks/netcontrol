from ConfigParser import ConfigParser
from netcontrolldap.netcontrolldap import *

cfg = ConfigParser()
cfg.read('/etc/ldap/netcontrol')
server  = cfg.get('base', 'server')
bindDN  = cfg.get('base', 'bindDN')                       
adminPW = cfg.get('base', 'adminPW')
del cfg

con = LDAPConnection( server=server, ssl=True, baseDN=bindDN, admPasswd=adminPW )
print con
print con.initConnection()
