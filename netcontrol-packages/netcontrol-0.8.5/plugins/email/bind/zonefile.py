# -*- coding: utf-8 -*-
'''
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
'''

class Record:
    _record = None
    
    def __init__(self, r=None):
        self._record = { 'Nome':'', 'Classe':'', 'Tipo':'',
                         'Preferência':'', 'IP':'', 'HostName':'',
                         'Texto':'' }
        if r is not None:
            self.setRecord(r)
    
    def setRecord(self, rec):
        TYPE = ''
        if rec.split()[0].upper() == 'IN':
            rec = '@ '+rec

        FIELDS    = rec.split()
        FIELDS[1] = FIELDS[1].upper()
        FIELDS[2] = FIELDS[2].upper()
        TYPE      = FIELDS[2]        

        self._record['Nome']   = FIELDS[0]
        self._record['Classe'] = FIELDS[1]
        self._record['Tipo']   = FIELDS[2]

        try:
            if TYPE == 'A' or TYPE == 'AAAA':
                self._record['IP'] = FIELDS[3]

            elif TYPE == 'CNAME' or TYPE == 'NS' or TYPE == 'PTR':
                try:
                    if FIELDS[3][-1] <> '.': FIELDS[3] += '.'
                except: pass
                self._record['HostName'] = FIELDS[3]

            elif TYPE == 'TXT':
                self._record['Texto'] = "'"+" ".join(map(str,FIELDS[3:])).replace("'",'')+"'"

            elif TYPE == 'MX':
                self._record['Preferência'] = FIELDS[3]
                self._record['HostName'] = FIELDS[4]
            
            return True
        except:
            return False
            pass
                    
    def getRecord(self):
        return self._record
    
class ZoneFile:
    _SOA        = None
    _directives = None
    
    def __init__(self, newZone=False):
        self._SOA = {'Nome da Zona': '', 'Dominio do Servidor': '', 'E-mail do Responsável':'',
            'Número Serial': '', 'Tempo de Atualização': ''  ,
            'Retry' : '', 'Expiração': '' , 'Tempo de cache negativa': ''}

        self._directives = { 'ORIGIN':'', 'INCLUDE': '', 'TTL':'' }
        self._records = []
        self._filename = None
        
        if newZone:
            self.setSOA('')
            self.setDirective('TTL    86400')
        
    def setDirective(self, r):
        for i in self._directives.keys():
            if r.upper().find(i) <> -1:
                self._directives[i] = r.split()[-1]
    
    def getSOAField(self, field):
        recFields = { 'Nome da Zona': 'zoneName',
                      'Dominio do Servidor': 'serverDomain',
                      'E-mail do Responsável': 'emailResp',
                      'Número Serial': 'serial',
                      'Tempo de Atualização': 'refresh',
                      'Retry' : 'retry',
                      'Expiração': 'expire',
                      'Tempo de cache negativa': 'negative' }
        for i in recFields.keys():
            if field == i or field == recFields[i]:
                return self._SOA[i]
        return ''
    
    def setSOAbyFields( self, zoneName='', serverDomain='', emailResp='',
                        serial='', refresh='', retry='', expire='', negative='' ):
            from time import strftime
            # default configuration
            default   = { 'Nome da Zona': '@',
                          'Dominio do Servidor': 'ns.localhost.',
                          'E-mail do Responsável':'admin@localhost.',
                          'Número Serial': strftime("%Y%m%d01"),
                          'Tempo de Atualização': '172800',
                          'Retry' : '900',
                          'Expiração': '1209600',
                          'Tempo de cache negativa': '3600' }
            
            try:
                # fixing serverDomain and emailResp
                if serverDomain[-1]  <> '.': serverDomain = serverDomain+'.'
                if emailResp[-1]     <> '.': emailResp = emailResp+'.'
            except: pass
            
            try:
                if serverDomain[-1] <> '.': serverDomain += '.'
            except:pass
            try:
                if emailResp[-1] <> '.': emailResp += '.'
            except:pass
            # received fields
            recFields = { 'Nome da Zona': zoneName,
                          'Dominio do Servidor': serverDomain,
                          'E-mail do Responsável': emailResp,
                          'Número Serial': serial,
                          'Tempo de Atualização': refresh,
                          'Retry' : retry,
                          'Expiração': expire,
                          'Tempo de cache negativa': negative }
            
            # para cada um dos campos de SOA
            for i in default.keys():
                # Se recebeu o campo em branco
                if recFields[i] == '':
                    # Mantem o valor de SOA
                    # mas se SOA for em branco, define default
                    if self._SOA[i] == '':
                        self._SOA[i] = default[i]
                # se recebeu novo campo, substitui em SOA
                else:
                    self._SOA[i] = recFields[i]

    def setSOA(self, soa):
        try:
            # First part
            F   = soa.split('(')[0].split()
            # Seconf Part
            S   = soa.split('(')[1].split(')')[0].split()
            
            # se não possui nome da zona, adiciona-a como ''
            if F[0] == 'IN': F = [''] + F
            # se não possui o ponto final, corrige-o
            if F[3][-1] <> '.': F[3] = F[3]+'.'
            if F[4][-1] <> '.': F[4] = F[4]+'.'
            self.setSOAbyFields( F[0], F[3], F[4], S[0],
                                 S[1], S[2], S[3], S[4] )
        except:
            self.setSOAbyFields( self._SOA['Nome da Zona'],
                                 self._SOA['Dominio do Servidor'],
                                 self._SOA['E-mail do Responsável'],
                                 self._SOA['Número Serial'],
                                 self._SOA['Tempo de Atualização'],
                                 self._SOA['Retry'],
                                 self._SOA['Expiração'],
                                 self._SOA['Tempo de cache negativa'] )
            pass

    def insertMX(self, pref, host):
        '''
        @param pre:  preferência do servidor
        @param host: local onde se encontra o servidor de email
        '''
        self.insertRecord( "@ IN MX %s %s" % (str(pref), str(host)) )

    def insertIP(self, ip, name, ipv6=False):
        if ipv6:
            self.insertRecord( "%s IN AAAA %s" % (str(ip), str(name)) )
        else:
            self.insertRecord( "%s IN A %s" % (str(ip), str(name)) )

    def insertA(self, ip,name):
        self.insertIPv4(ip,name)

    def insertAAAA(self, ip,name):
        self.insertIPv6(ip,name)

    def insertIPv4(self, ip, name):
        self.insertIP( ip, name )

    def insertIPv6(self, ip, name):
        self.insertIP( ip, name, ipv6=True )

    def insertCNAME(self, name, host):
        self.insertRecord( "%s IN CNAME %s" % (str(name), str(host)) )

    def insertPTR(self, ip, host):
        self.insertRecord( "%s IN PTR %s" % (str(ip), str(host)) )

    def insertTXT(self, name, host):
        self.insertRecord( "%s IN TXT '%s'" % (str(name), str(host)) )

    def insertRecord(self, r):
        t = Record( r )
        self._records.append( t )
        t = None

    def removeRecord(self, idx):
        idx = int(idx)
        del self._records[ idx ]

    def openZoneFile(self, filename):
        self._filename = filename
        try:
            # abrindo arquivo da zona
            fileCont = open(filename).readlines()
        except:
            return False
        
        # obtendo quantas linhas
        qntLines  = len( fileCont )
        # marcador de linha
        cont      = 0
        # marcador de seção SOA
        markSOA   = 0
        # linhas que correspondem ao SOA
        linesSOA  = ''
        self._records = []
        # percorrendo todas as linhas do arquivo
        while cont < qntLines:
            ### descartando comentários, espaços em
            ### branco inúteis e linhas vazias
            line = fileCont[ cont ].split(';')[0].lstrip().rstrip()
            if line == '':
                cont+=1
                continue
            ###
            
            #_ defininado o início da seção SOA
            if line.find('IN') <> -1 and line.find('SOA') <> -1:
                if line.find('IN') < line.find('SOA'):
                    markSOA = 1

            # Se é a seção SOA, adiciona as linhas em linesSOA
            if markSOA == 1:
                linesSOA += line+' '
                # quando terminar a seção, marca que terminou
                # e adiciona no objeto
                if line.find(')') <> -1:
                    markSOA = 0
                    SOA = self.setSOA( linesSOA )

            # se é diretiva, adiciona no objeto
            elif line.startswith('$'):
                self.setDirective( line )
            
            # se é domínio, aidiona no objeto
            else:
                self.insertRecord( line )

            # incrementa a linha
            cont+=1
        
        return True

    def getRecordByIdx(self, idx):
        t = self.getRecords()
        try:    return t[idx]
        except: return ''
    
    def getRecords(self):
        ret = {}
        idx = 0
        for i in self._records:
            t = i.getRecord()
            ret[idx] = t
            idx+=1

        return ret
    
    def getSOA(self):
        return self._SOA
    
    def getDirectiveField(self, field):
        t = self.getDirectives()
        for i in t.keys():
            if field == i:
                return t[i]
        return ''
    
    def getDirectives(self):
        ret = {}
        for i in self._directives.keys():
            if self._directives[i] <> '':
                ret[i] = self._directives[i]
        return ret
    
    def getZoneFile(self, zoneFileObject=None):
        def fieldExp(msg, tam=12):
            if msg == '': return ''
            return msg+(' '*(tam-len(msg)))

        if zoneFileObject is None:
            d = self.getDirectives()
            s = self.getSOA()
            r = self.getRecords()
        else:
            d = zoneFileObject.getDirectives()
            s = zoneFileObject.getSOA()
            r = zoneFileObject.getRecords()
        
        ret = ''
        for i in d.keys():
            ret += "$%s\t%s\n" % (i,d[i])
        
        #ret += "%s\tIN\tSOA\t%s %s (\n%40s\n%40s\n%40s\n%40s\n%40s\n)\n;\n;Records:\n" %\
        ret  += ("%s\tIN\tSOA\t%s %s (\n%40s ;Serial\n%40s ;Refresh\n%40s ;Retry\n%40s " + \
                ";Expire\n%40s ;Negative Cache TTL\n)\n;\n; Records:\n") % \
               (s['Nome da Zona'] , s['Dominio do Servidor'] , s['E-mail do Responsável'], 
                s['Número Serial'], s['Tempo de Atualização'], s['Retry'],s['Expiração'], 
                s['Tempo de cache negativa'])

        for i in r.keys():
            if len( r[i]['HostName'] ) > 0:
                if r[i]['HostName'][-1] <> '.':
                    r[i]['HostName'] = r[i]['HostName']+'.'
                    
            ret +=  ("%s %s %s %s %s %s %s\n") % \
                    ( fieldExp( r[i]['Nome'] ), fieldExp( r[i]['Classe'], 10 ),
                      fieldExp( r[i]['Tipo'] ), fieldExp( r[i]['Preferência'] ),
                      fieldExp( r[i]['IP'] ) , fieldExp( r[i]['HostName'] ),
                      fieldExp( r[i]['Texto'] ) )
        return ret

    def saveZoneFile(self):
        try:
            open(self._filename, 'w').write( self.getZoneFile() )
        except:
            pass

    @staticmethod
    def getZoneFileByZone(zoneFileObject):
        tmp = ZoneFile()
        ret = tmp.getZoneFile(zoneFileObject)
        del tmp
        return ret
