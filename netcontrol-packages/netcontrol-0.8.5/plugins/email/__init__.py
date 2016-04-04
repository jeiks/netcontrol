MODULES = ['main']
 
DEPS =  [ (['any'], [
            ('app', 'Postfix', 'postfix'),
            ('app', 'Courier Imap', 'imapd'),
            ('app', 'Courier POP', 'pop3d'),
            ]) ]

NAME = 'Email'
PLATFORMS = ['any']
DESCRIPTION = 'Configurar servidor de Email'
VERSION = '1.0'
GENERATION = 1
AUTHOR = 'Joao Paulo'
HOMEPAGE = 'http://fluxsoftwares.com'
