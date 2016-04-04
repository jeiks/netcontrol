from distutils.core import setup

setup(
    name='netcontrolldap',
    version='1.0',
    author='Jacson Rodrigues Correia da Silva',
    author_email='jacsonrcsilva@gmail.com',
    packages=['netcontrolldap', 'netcontrolldap.test'],
    scripts=['bin/vector_from_schema'],
    url='http://www.fluxsoftwares.com',
    license='LICENSE.txt',
    description='Script to create and manage openLDAP SAM accounts.',
    long_description=open('README.txt').read(),
    install_requires=[
        "ldap >= 2.3.11",
        "passlib >= 2.3.11-1",
    	"python >= 2.6",
    ],
)
