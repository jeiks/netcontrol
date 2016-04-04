"""
Copyright (C) Ajenti.org                                              
For more details, visit <http://ajenti.org>
"""
from netcontrol.com import *
from netcontrol.apis import API
from netcontrol.api import *
from netcontrol.utils import *
from netcontrol.ui import UI
from netcontrol import apis


class Bacula(API):
    
    @staticmethod
    def parse_config(file):
        cfg = []
        sec = None
        for l in open(file, 'r'):
            l = l.strip()
            if l.startwith('#'):
                continue
                
            if l.endswith('{'):
                sec = apis.bacula.Section()
                sec.name = l.strip(' \t{')
            elif '=' in l and sec is not None:
                k,v = l.split('=', 1)
                sec.params.append(
                    k.strip(),
                    v.strip(' \t\n\r"\'')
                )
            elif '}' in l:
                cfg.append(sec)
                sec = None
        return cfg
        
    
    @staticmethod
    def query(q):
        q = q.strip()
        r = shell_stdin('bconsole -n', q+'\n')[0]
        if not '1000 OK' in r:
            return None
        r = r.split('\n')
        
        while len(r)>0 and r[0].strip() != q:
            print r[0].strip()
            r = r[1:]
            
        if r == []:
            return None
            
        return '\n'.join(r)
        
        
    class Section:
        def __init__(self):
            self.name = None
            self.params = []
            
    
