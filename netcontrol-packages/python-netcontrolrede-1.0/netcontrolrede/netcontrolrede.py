# -*- coding: utf-8 -*-
"""
Copyright (C) 2012 FluxSoftwares <http://www.fluxsoftwares.com>.
(Escrito por Jacson Rodrigues Correia da Silva <jacsonrcsilva@gmail.com> para FluxSoftwares)

Este arquivo é parte do programa NetControl.

O NetControl é um software livre; você pode redistribuí-lo e/ou modificá-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral GNU/GPL em português para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU, sob o título "LICENCA.txt", junto com este programa, se não, acesse o Portal do Software Público Brasileiro no endereço www.softwarepublico.gov.br ou escreva para a Fundação do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA

"""

class MascRede:
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    _masc = None
    def __init__(self, masc=None):
        if masc is not None:
            self.setMasc(masc)

    def setMasc(self, masc):
        try:
            if int(masc) <= 32 and int(masc) >= 0:
                self._masc = masc
        except:
            if masc.find('.') <> -1:
                self._masc = MascConv.strToInt(masc)

    def getMascAsStr(self):
        return MascConv.intToStr(self._masc)

    def getMascAsInt(self):
        return self._masc

    def getMascAsOctets(self):
        return MascConv.intToOctets(self._masc)

class MascConv:
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    @staticmethod
    def intToOctets(num):
        try:
            num = int(num)
            masc = (2**(num)-1) << (32-num)
            m4   = (masc & 0xFF000000) >> 24
            m3   = (masc & 0x00FF0000) >> 16
            m2   = (masc & 0x0000FF00) >> 8
            m1   = (masc & 0x000000FF)
            return [m4,m3,m2,m1]
        except:
            return []
    
    @staticmethod
    def intToStr(num):
        r = MascConv.intToOctets(num)
        if r <> []:
            return "%d.%d.%d.%d" % (r[0],r[1],r[2],r[3])
        return []

    @staticmethod
    def octectsToInt( masc ):
        try:
            masc =  (int(masc[0]) << 24) | \
                    (int(masc[1]) << 16) | \
                    (int(masc[2]) <<  8) | \
                    (int(masc[3]))
            cont = 0
            while (masc & 0x080000000) >> 31 == 1:
                cont+= 1
                masc = masc << 1
            return cont
        except:
            return 0

    @staticmethod
    def strToInt( masc ):
        return MascConv.octectsToInt( masc.split('.') )


class Rede:
    """
    @author: Jacson RC Silva <jacsonrcsilva@gmail.com>
    """
    __ip   = None
    __masc = None
    __rede = None

    def __init__(self, ip, masc):
        self.setIP( ip )
        self.setMasc( masc )

    def __setRede(self):
        if self.__ip is not None and self.__masc is not None:
            self.__rede  = map( lambda x,y: x&y, self.__ip, self.getMasc() )

    def setIP(self, ip):
        self.__ip = map( lambda x: int(x),   ip.split('.') )
        self.__setRede()

    def setMasc(self, masc):
        self.__masc = MascRede( masc )
        self.__setRede()

    def getIP(self):
        return self.__ip[:]

    def getMasc(self):
        if self.__masc is None: return None
        return self.__masc.getMascAsOctets()[:]

    def getRede(self):
        return self.__rede[:]

    def getBroadcast(self):
        r = self.getRede()
        m = self.getMasc()

        return [ int( (255-m[0])|r[0] ),
                 int( (255-m[1])|r[1] ),
                 int( (255-m[2])|r[2] ),
                 int( (255-m[3])|r[3] )]

    def __retStr(self, f):
        return "%d.%d.%d.%d" % (f[0], f[1], f[2], f[3])

    def getIPAsStr(self):
        return self.__retStr( self.getIP() )

    def getMascAsStr(self):
        return self.__retStr( self.getMasc() )

    def getMascAsInt(self):
        return self.__masc.getMascAsInt()

    def getRedeAsStr(self):
        return self.__retStr( self.getRede() )

    def getBroadcastAsStr(self):
        return self.__retStr( self.getBroadcast() )

    def getQntIPs(self):
        m = self.getMasc()
        return (256-m[0])*(256-m[1])*(256-m[2])*(256-m[3])

    def ipIsValid(self, ip):
        tmp = Rede(ip,self.getMascAsStr())

        if self.getRedeAsStr() == tmp.getRedeAsStr():
            return True
        else:	return False

