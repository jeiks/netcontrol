# -*- coding: utf-8 -*-
'''
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
'''

from zone import Zone
from zonefile import ZoneFile

class ConfigFile:

    def __init__(self, filename):
        self.zones = []
        self.filename = filename
        self.contents = self.__getFile()
        self.removeComments()
        self.__includeChildren()
        self.__prepareZones()
        self.includedFiles = []
    
    def __getFile(self):
        try:
            return open(self.filename).read()
        except:
            print 'Error Openning file:', self.filename
    
    def __includeChildren(self):
        
        last = -1
        while 1:
            # getting where is 'include' directive
            pos = self.contents.find('include', last+1)
            if pos == -1: return None
            # marking 'include' position
            last = pos

            # Opening files to add in file.Contents 
            fileTmp = ''
            # 'get' is true when it is the filename position 
            get  = False
            while self.contents[pos] <> ';':
                if self.contents[pos] == '"':
                    # don't get \" as filename  
                    if not get:
                        pos += 1

                    get = not get

                if get: fileTmp = fileTmp+self.contents[pos]
                pos += 1

            # remove include directive from self.contents
            self.contents = self.contents[:last] + self.contents[(pos+1):]
            
            # open file included and add it to self.contents
            try:
                self.includedFiles.append(fileTmp)
                temp = ConfigFile(fileTmp)
                temp.removeComments()
                self.contents += '\n' + temp.getContents()
            except:
                print 'File not found:', fileTmp
                pass

    def __prepareZones(self):
        last = -1
        while 1:
            # getting where is 'include' directive
            pos = self.contents.find('zone ', last+1)
            if pos == -1: return None
            # fix
            if pos > 1:
                char = self.contents[pos-1]
                if char <> '\n' and char <> '\r' and \
                   char <> ' '  and char <> '\t' and \
                   char <> ';':
                       return None
            # marking 'include' position
            last = pos
            
            # creating a Zone instance
            thisZone = Zone()
            try:
                OneZonePart = self.contents[pos:].split('}')[0]
                # Getting Name
                thisZone.setName( self.__getStr( OneZonePart.split('{')[0] ) )
                # Getting Filename
                thisZone.setFile( self.__getStr( OneZonePart.split('file')[1].split(';')[0] ) )
                #Getting type
                if OneZonePart.split('type')[1].split(';')[0].upper().find('SLAVE') <> -1:
                    thisZone.setType('slave')
                else:
                    thisZone.setType('master')
                    
                self.zones.append(thisZone)
            except:
                pass

            # destroying thisZone instance
            thisZone = None


    def __getStr(self, text):
        pos = 0
        get = False
        ret = ""
        while pos < len(text):
            if text[ pos ] == '"':
                get = not get
                if get: pos += 1
                else: return ret

            if get: ret += text[ pos ]
            pos += 1

    
    def getContents(self):
        return self.contents
    
    def removeComments(self):
        import re
        # '#*[newline]' or '//*[newline]' 
        expr = re.compile('(\#|//).*\n')
        temp = expr.sub('', self.contents)
        self.contents = temp
    
    def getZones(self):
        return self.zones

    def setZones(self, zones):
        try:
            if type(zones[0]) <> type(Zone()):
                print 'The argument must be a Zone list'
        except:
            print 'The argument must be a Zone list'
            pass

    def SaveFile(self):
        ConfigFile.SaveFileWithZone(self.filename, self.zones)

    @staticmethod
    def SaveFileWithZone(filename, zones, newZone=False):
        from os import access, path, mkdir,remove
        if not path.exists( path.dirname(filename) ):
            mkdir( path.dirname(filename), 0755 )
        fileDest = open(filename, 'w')
        for i in zones:
            fileDest.write( i.getZoneAsNamedConf()+'\n' )
        fileDest.close()
        try:
            for i in zones:
                iFile = i.getFile()
                if not access( iFile, 6 ):
                    if not path.isdir( path.dirname( iFile ) ):
                        try: remove( path.dirname( iFile) )
                        except: pass
                        mkdir( path.dirname(iFile), 0755 )
                    newContent = ZoneFile( newZone=True ).getZoneFile()
                    fileDest = open(i.getFile(), 'w')
                    fileDest.write(newContent)
                    fileDest.close()
        except:
            pass
