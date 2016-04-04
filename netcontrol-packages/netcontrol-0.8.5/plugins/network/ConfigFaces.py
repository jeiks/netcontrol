# -*- coding: utf-8 -*-
#-*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Written by João Paulo Contantino <joaopaulo@fluxsoftwares.com> 
            Jacson Correia Silva  <jacsonrcsilva@gmail.com>
            for FluxSoftwares)
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
from os import system


class ConfigRedes(object):
  
  allRedes = []
  redesArq = []
  facesUP  = []
  ifacesV = {}
  def __init__(self):
    pass
    
  # pega todas as interfaces do /proc/net/dev
  def getRedesProc(self):
    self.allRedes = []
    open('/tmp/catRedesProc','w').write(' ')
    if not system('cat /proc/net/dev > /tmp/catRedesProc'):
      redes = open('/tmp/catRedesProc').readlines()
      for i in redes[2:]:
        i = i.split(':')[0].strip()
        if i <> 'pan0' and i <> 'lo':
          self.allRedes.append( i )
    return self.allRedes
  
  # pega as redes que estao configuradas no arquivo - > /etc/network/interfaces
  def getRedesArquivo(self):
    faceArq = open('/etc/network/interfaces').readlines()
    self.redesArq = []
    for i in faceArq:
      if not i.find('auto') and i.split()[1] <> 'lo':
        self.redesArq.append( i.split()[1] )
    return self.redesArq
  
  # pega interfaces virtuais
  def getRedesVirtuais(self):
    self.ifacesV = {}
    for i in self.getRedesArquivo():
      #try:
        i = i + ':0'
        j = i.split(':')[1]
        i = i.split(':')[0]
        if  not self.ifacesV.has_key(i):
          self.ifacesV[ i ] = 0
        else:
          self.ifacesV[ i ] = self.ifacesV[ i ] + 1
    return self.ifacesV
    
  # pega as interfaces existentes que nao estao de pé
  def getRedesDown(self):
    self.facesUP = []
    for i in self.getRedesProc():
      try:
        self.getRedesArquivo().index( i )
      except:
        self.facesUP.append( i )
    return self.facesUP
    
  # escreve todas as interfaces existentes no arquivo de configuracao
  def setAllRedes(self):
    arq = open('/etc/network/interfaces').read()
    if len(self.getRedesDown()) > 0:
      for i in self.getRedesDown():
        try:
          self.getRedesArquivo.index(i)
          pass
        except:
          arq = arq + str( "\nauto %s\niface %s inet dhcp\n"%(i,i) )
      open('/etc/network/interfaces','w').write(arq)
    return arq
  # adiciona uma interface virtual
  def addIfaceVirtual(self,iface=None,ip=None,masc="255.255.255.0"):
    if not iface:
      pass
    else:
      num = int( self.getRedesVirtuais()[iface] )  + 1
      arq = open('/etc/network/interfaces').read()
      arq += str( "\nauto %s:%s\niface %s:%s inet static\n"%(iface,num,iface,num))
      arq += str( "\t\t address %s\n"%ip)
      arq += str( "\t\t netmask %s"%masc)
      a = open('/etc/network/interfaces','w')
      a.write(arq)
      a.close()
      system("ifconfig %s:%s %s netmask %s"%(iface,num,ip,masc))
      
  # apaga uma interface virtual
  def delIFaceVirtual(self,iface):
    arq = open('/etc/network/interfaces','r')
    tmp = arq.read()
    inicio = tmp.find('auto %s'%iface)
    arq.seek(inicio + 1)
    tmp = arq.read()
    fim = int( tmp.find('auto ') ) + inicio
    arq.seek(0)
    tmp = arq.read()
    tmp = tmp[:inicio]
    tmp += tmp[fim:]
    arq.close()
    open('/etc/network/interfaces','w').write(tmp)
    system("ifconfig %s down"%(iface))
    
  # atualiza o status da rede quando alterada
  def ifconfig_refresh_iface(self,iface,ip,masc,gateway):
    system("ifconfig %s %s netmask %s gateway %s"%(iface,ip,masc,gateway))
    
    
  def _execute(self, command, daemon=False):
        '''
        @return:
            [ (Boolean) Coomand executed with sucess ,
              (tuple) (stdout,stderr)                  ]
        '''
        cmd = command.split()
        print cmd[0], '::' , cmd[1:]
        if daemon:
            from time import sleep
            from os import access
            open( fileSync ,'w').write('1')
            exec_as_daemon(cmd[0], cmd[1:])
            a = open( fileSync ).read()
            while access('/proc/%s'%a, 1):
                sleep(1)
                a = open( fileSync ).read()
        else:
            from os import system
            system(command)
        return [ True, 'Success' ]
    
fileSync = '/tmp/netcontrolNetwork.pid'

# Copyright: (c) 2011 phobie <pydaemon.phobie@safersignup.com>
# License: CC-by-sa 3.0
def exec_as_daemon(s_path_cmd, l_args=[]):
    import os
    i_pid = os.fork()
    if i_pid != 0:
        # Always remember to gobble your zombie children
        os.wait()
        # Back to parent
        return
    # Detach from parent
    os.setsid()
    # Do not block any mounts
    os.chdir('/')
    # Reset file creation rights
    os.umask(0)
    i_pid = os.fork()
    if i_pid != 0:
        open( fileSync, 'w' ).write(str(i_pid))
        # Close the direct child process
        os._exit(0)
    # Get the maximum count of open file handles
    try:
        import resource
        i_fd_max = resource.getrlimit(resource.RLIMIT_NOFILE)[1]
        if i_fd_max == resource.RLIM_INFINITY:
            i_fd_max = 1024
    except ImportError:
        i_fd_max = 1024
    # Try to close all possible file handles
    for i_cur_fd in range(0, i_fd_max):
        try:
            os.close(i_cur_fd)
        except OSError:
            pass
    # Assosiate STDIN with /dev/null
    os.open(os.devnull if hasattr(os, "devnull") else '/dev/null', os.O_RDWR)
    # STDOUT to /dev/null
    os.dup2(0, 1)
    # STDERR to /dev/null
    os.dup2(0, 2)
    l_args.insert(0, s_path_cmd)
    # Let the clone become a other process

    os.execv(s_path_cmd, l_args)