# -*- coding: utf-8 -*-
MODULES = ['api', 'main', 'meter', 'widget', 's_upstart', 's_arch', 's_bsd', 's_centos', 's_gentoo']

DEPS =  [
    (['centos', 'fedora'],
     [
	('app', 'Service manager', 'chkconfig'),
     ])
]

NAME = 'Serviços'
PLATFORMS = ['debian', 'arch', 'freebsd', 'centos', 'fedora', 'gentoo', 'mandriva']
DESCRIPTION = 'Controla serviços do sistema'
VERSION = '1'
GENERATION = 1
AUTHOR = 'Ajenti team'
HOMEPAGE = 'http://ajenti.org'
