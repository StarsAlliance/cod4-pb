#ifndef _VPLAYERLIMITS_H_
#define _VPLAYERLIMITS_H_

#include "vupshared.h"

TBOOL IfValveServerLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120]);
TBOOL IfValveDedicatedLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120]);

#endif
