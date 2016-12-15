#ifndef _VLVGAMEUI_H_
#define _VLVGAMEUI_H_

#include "vupshared.h"

#define GAMEUI_MAGIC1 "`#GameUI_Quit` 00 00 00 00"
#define GAMEUI_MAGIC2 "`#GameUI_QuitConfirmationTitle` 00 00 00"
#define GAMEUI_MAGIC3 "`#GameUI_QuitConfirmationText` 00 00 00 00"

#define GAMEUI_INITSVLANVAL_SRC "`sv_lan `"

#define GAMEUI_INITSVLANVAL_DST "`0`"

TBOOL IfGameUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);

void PatchGameUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200]);



#endif
