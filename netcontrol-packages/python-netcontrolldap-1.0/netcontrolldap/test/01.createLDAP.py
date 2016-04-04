from netcontrolldap.netcontrolldap import *

a = LDAPControl()
print a.cleanLDAP()
print a.setSSLAttributes( 'Jeiks CTGA', 'Caratinga', 'Minas Gerais', 'Brasil', 'jeiks@fluxsoftwares.com', '99999')
print a.createNewBase( 'Jeiks CTGA', 'flux.intranet', '123456', ssl=True, workgroup='FLUXINTRANET')
