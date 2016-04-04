from netcontrolldap.netcontrolldap import *
from pprint import pprint
from ConfigParser import ConfigParser

cfg = ConfigParser()
cfg.read('/etc/ldap/netcontrol')
server  = cfg.get('base', 'server')
bindDN  = cfg.get('base', 'bindDN')                       
adminPW = cfg.get('base', 'adminPW')
del cfg

c = LDAPConnection( server=server, ssl=False, baseDN='cn=config', admName='admin', admPasswd=adminPW)
print c.initConnection()
from pprint import pprint
pprint( c.search() )

c.closeConnection()
