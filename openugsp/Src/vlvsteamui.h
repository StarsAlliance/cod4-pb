#ifndef _VLVSTEAMUI_H_
#define _VLVSTEAMUI_H_

#include "vupshared.h"

#define STEAMUI_MAGIC "`#SteamUI_JoinDialog_KeyRequestFailed_Text` 00 00 00 `#SteamUI_JoinDialog_KeyRequestFailed_Title` 00 00 `#Steam_RequestingLegacyKey` 00 00"

#define STEAMUI_KEYSERVERCHECK_SRC "57 : 8B F9 :  8B 8F FC 01 00 00 : 81 E1 FF FF FF 00 : 39 48 04 : 0F 85 ?? ?? 00 00 : 83 38 01 _: 75 ?? :_ [57] B9 03 00 00 00 : [8B D7] : \
                                    E8 ?? ?? FF FF : 8B 4C 24 0C : 64 89 0D 00 00 00 00"
#define STEAMUI_KEYSERVERCHECK_DST "89 C0"


#define STEAMUI_GAMEUPDATECHECK_SRC "8B 0D ?? ?? ?? ?? : 8B B4 24 ?? 05 00 00 : 8B 11 : 8B 86 ?? 00 00 00 : 8B 52 28 : 50 : FF D2 : \
                                     84 C0 _: { 0F 84 ?? ?? 00 00 ^ 74 ??} :_ 8B 0D ?? ?? ?? ?? : 8B 01 : 8B 50 08 : 68 ?? ?? ?? ?? : FF D2 : 50"
#define STEAMUI_GAMEUPDATECHECK_DST1 "66 89 C0 66 89 C0"
#define STEAMUI_GAMEUPDATECHECK_DST2 "89 C0"

TBOOL IfSteamUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);

void PatchSteamUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200]);



#endif
