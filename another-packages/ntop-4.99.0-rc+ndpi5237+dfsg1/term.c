/*
 *  Copyright (C) 1998-2012 Luca Deri <deri@ntop.org>
 *
 *		 	    http://www.ntop.org/
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "ntop.h"



/* ******************************* */

void termIPServices(void) {
  int i;
  ProtocolsList *proto = myGlobals.ipProtosList;

  for(i=0; i<myGlobals.numActServices; i++) {
    if(myGlobals.udpSvc[i] != NULL) {
      free(myGlobals.udpSvc[i]->name);
      free(myGlobals.udpSvc[i]);
    }

    if(myGlobals.tcpSvc[i] != NULL) {
      if(myGlobals.tcpSvc[i]->name != NULL) {
	free(myGlobals.tcpSvc[i]->name);
      }
      free(myGlobals.tcpSvc[i]);
    }
  }

  if(myGlobals.udpSvc) free(myGlobals.udpSvc);
  if(myGlobals.tcpSvc) free(myGlobals.tcpSvc);

  /* ****************************** */

  while(proto != NULL) {
    ProtocolsList *nextProto = proto->next;

    free(proto->protocolName);
    free(proto);
    proto = nextProto;
  }
}

/* ******************************* */

void termIPSessions(void) {
  int i, j;

  for(j=0; j<myGlobals.numDevices; j++) {
    if(myGlobals.device[j].sessions == NULL) continue;
  
    for(i=0; i<MAX_TOT_NUM_SESSIONS; i++) {
      IPSession *session = myGlobals.device[j].sessions[i];

      while(session != NULL) {
	IPSession *nextSession;
	nextSession = session->next;
	free(session);
	session = nextSession;
      }
    }

    myGlobals.device[j].numSessions = 0;
    
    while(myGlobals.device[j].fragmentList != NULL)
      deleteFragment(myGlobals.device[j].fragmentList, j);
  }
}

/* ************************************************ */

void termGdbm() {
  if(myGlobals.pwFile)            { gdbm_close(myGlobals.pwFile); myGlobals.pwFile = NULL;                       }
  if(myGlobals.prefsFile)         { gdbm_close(myGlobals.prefsFile); myGlobals.prefsFile = NULL;                 }
  if(myGlobals.macPrefixFile)     { gdbm_close(myGlobals.macPrefixFile); myGlobals.macPrefixFile = NULL;         }
  if(myGlobals.fingerprintFile)   { gdbm_close(myGlobals.fingerprintFile); myGlobals.fingerprintFile = NULL;     }
  if(myGlobals.serialFile)        { gdbm_close(myGlobals.serialFile); myGlobals.serialFile = NULL;               }
  if(myGlobals.resolverCacheFile) { gdbm_close(myGlobals.resolverCacheFile); myGlobals.resolverCacheFile = NULL; }
  if(myGlobals.topTalkersFile)    { gdbm_close(myGlobals.topTalkersFile); myGlobals.topTalkersFile = NULL;       }
}
