#ifndef _VLVSHARED_H_
#define _VLVSHARED_H_

#include <string.h>
#include <time.h>
#include <math.h>

#include "vupshared.h"

#define VLVMagicN1 "50 72 6F 74 6F 63 6F 6C 20 76 65 72 73 69 6F \
                    6E 20 25 69 0A 45 78 65 20 76 65 72 73 69 6F \
                    6E 20 25 73 20 28 25 73 29 0A 00"
/* MagicN1 Updated to reflect last changes on L4D engine builds */

#define VLVMagicN2 "45 78 65 20 62 75 69 6C 64 3A 20 ?? ?? 3A ?? \
                    ?? 3A ?? ?? 20 ?? ?? ?? 20 ?? ?? 20 ?? ?? ?? \
                    ?? 20 28 25 69 29"

#define VLVMagicNewVersioningScheme "45 78 65 20  62 75 69 6C 64 3A 20"

#define VLVDateMagicHLDSAndAllUniX "00 [00 00] [73 74 65 61 6D 2E 69 6E 66 00] [72 00] 50 61 74 63 68 56 65 72 73 69 6F 6E 3D 00"

#define VLVProtoMagic_Win "68 ?? ?? ?? {?0 ^ ?2} 68 ?? ?? ?? {?0 ^ ?2} 6A ?? [74 ??] 68 ?? ?? ?? {?0 ^ ?1} {E8 ?? ?? ?? ?? ^ FF ??} {[A1 ?? ?? ?? ?? : 83 C4 10 : 50] E8 ?? ?? ?? ?? 50 \
                           68 ?? ?? ?? ?? {E8 ?? ?? ?? ?? ^ FF ??} 83 C4 {18 ^ 0C} ^ 83 C4 10 : E8 ?? ?? ?? 00 : 83 78 08 00 : 74 ?? : E8 ?? ?? ?? ??}"

/*
#define VLVProtoMagic_UniX "53 E8 ?? ?? ?? FF 81 C3 ?? ?? 1? 00 83 EC 18 \
                            {B9 ?? 00 00 00 8B ?? ?? ?? 00 00 89 4C 24 04 8B 83 ?? 5? 00 00 89 54 24 0C ^ C7 44 24 04 ?? 00 00 00 8B ?? ?? ?? F? FF} \
                            8D 93 ?? ?? F? FF [8B ?? ?? ?? F? FF] 89 14 24 89 4? 24 08 [89 44 24 0C] E8 ?? ?? ?? FF E8 ?? ?? ?? FF 89 4? 24 04 8D 83 ?? ?? F? FF \
                            89 04 24 E8 ?? ?? ?? FF 83 C4 18"
*/

#define VLVProtoMagic_UniX "E8 ?? ?? ?? FF 81 C3 ?? ?? 1? 00 {83 EC ^ 89 ?? 24} 18 \
                            {B9 ?? 00 00 00 8B ?? ?? ?? 00 00 89 4C 24 04 8B 83 ?? 5? 00 00 89 54 24 0C ^ C7 44 24 04 ?? 00 00 00 8B ?? ?? ?? F? FF} \
                            8D 93 ?? ?? F? FF [8B ?? ?? ?? F? FF] 89 14 24 89 4? 24 08 [89 44 24 0C] E8 ?? ?? ?? FF E8 ?? ?? ?? FF 89"

#define VLVProtoMagic_UniX_2 "83 EC ?C : {{B9 ^ 68} ?? ?? ?? 00 : {BA ^ 68} ?? ?? ?? 00 : [89 4C 24 0C] _: {B8 ?? ?? ?? ?? ^ 6A ??} :_ ^ \
                              89 5C 24 14 : E8 ?? ?? ?? FF : 81 C3 ?? ?? ?? ?? : 89 74 24 18 _: C7 44 24 04 ?? 00 00 00 :_ 8B B3 ?? ?? ?? FF : 8D 8B ?? ?? ?? FF : \
                              8B 83 ?? ?? ?? FF : 89 0C 24 : 89 74 24 0C : 89 44 24 08 : E8 ?? ?? ?? FF}"

#define VLVProtoMagic_UniX_3 "{8B 83 ?? ?? 00 00 : 50 ^ FF B3 ?? ?? 00 00} : {8B 83 ?? ?? 00 00 : 50 ^ FF B3 ?? ?? 00 00} _: 6A ?? :_ {8B 85 ?? ?? FF FF ^ 8B 44 24 ??} : 50 : E8 ?? ?? ?? FF : 83 C4 F8 : \
                              E8 ?? ?? ?? FF : 50 : {8B 85 ?? ?? FF FF ^ 8B 44 24 ??} : 50 : E8 ?? ?? ?? FF"

#define VLVProtoMagic_UniX_4 "8B 8B ?? ?? ?? FF : 8D 93 ?? ?? ?? FF : 8B 83 ?? ?? ?? FF : 89 4C 24 08 : 89 14 24 : 89 44 24 0C _: C7 44 24 04 ?? 00 00 00 :_ E8 ?? ?? ?? FF : E8 ?? ?? ?? FF : 89 C6"


#define VLVSteamIDFull "`STEAM_ID_PENDING`"
#define VLVSteamID666Full "?? ?? ?? ?? ?? ?? `666:88:666`"

#define VLVSteamIDDWORD_2 "M_ID"
#define VLVSteamIDDWORD_3 "_PEN"
#define VLVSteamIDDWORD_4 "`DING`"

#define VLVSteamIDDWORD666_2 "M_66"
#define VLVSteamIDDWORD666_3 "6:88"
#define VLVSteamIDDWORD666_4 ":666"


#define VLVSteamID666DWORDs "?? ?? ?? ?? ?? ?? ?? `66` ?? ?? ?? ?? ?? ?? ?? `6:88` ?? `:666`"

#define VLVSteamIDDWORDs2 "`STEA` ?? ?? ?? ?? ?? ?? `M_ID` ?? ?? ?? ?? ?? ?? `_PEN` ?? ?? ?? ?? ?? ?? `DING` "
#define VLVSteamID666DWORDs2 "?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? `66` ?? ?? ?? ?? ?? ?? `6:88` ?? ?? ?? ?? ?? ?? `:666`"

#define VLVPlayerLimitsServerLib "8B 4? 24 0? [C7 ?? 02 00 00 00] 8B ?? 24 0? [8B ?4 24 ??] [C7 ?? 02 00 00 00]  C7 ?? _??_ 00 00 00 [8B ?4 24 ??] C7 ?? _??_ 00 00 00 {C3 ^ C2 0C 00}"

/*
#define VLVSteamClient_Magic "{00 00 00 00 `.?AVISteamClientTest@@` 00 : ?? ?? ?? ?? ?? : 00 00 00 00 ^ \
                              ?? ?? `ISteamClientTest` : 00 ?? ?? ?? ?? ?? ?? ?? ?? ?? : ?? ?? {`ISteamClient` ^ `IClientEngine`} 00 ?? ?? `CSteamClient` 00 : 00 00 00 00 00 00 00 00 00 00}"
*/

#define VLVSteamClient_Magic "{`../clientdll/steamengine.cpp` ^ `.\\steamengine.cpp` ^ `:\\s3_client_rel\\src\\clientdll\\steamengine.cpp`}"
#define VLVSteamClient3_Magic "{`:\\steam3_rel_client\\src\\clientdll\\steamengine.cpp` ^ `P4Clients/steam3_rel_client/src/public/steam` ^ `/valve/steam3_rel_client/src/clientdll/` ^ `/steam_rel_client_linux/build/src/clientdll`}"

#define VLVSteamClientLog_Src "{52 : 50 : FF 15 ?? ?? ?? ?? : 83 C4 10 : 85 C0 : 74 ?? : 83 F8 17 : 74 ?? __: 50 :__ 68 ?? ?? ?? ?? : \
                                FF 15 ?? ?? ?? ?? : 83 C4 08 ^ \
                                89 44 24 08 : 8B 45 10 : 8D 75 F0 : 89 74 24 0C : 89 4C 24 04 : 89 04 24 : FF 15 ?? ?? ?? 00 _: 85 C0 : 0F 95 C2 : \
                                83 F8 17 : 0F 95 C3 : 84 D3 _: 74 ?? :_ __: 89 44 24 04 :__ C7 04 24 68 BE 67 00 : E8 ?? ?? ?? ?? : B8 FF FF FF FF : \
                                EB ?? : ^ \
                                90 90 90 90 90 90 90 90 90 90 90 90 90 : 8B ?? F? : 8B ?? F? : 8B ?? F? : 89 EC : 5D : C3 : 89 ?? ?? ?? : \
                                C7 04 24 ?? ?? ?? 00 :  E8 ?? ?? ?? ?? : 8D 76 00 ^ \
                                83 F8 17 : 0F 84 ?? ?? F? FF : 89 44 24 04 : C7 04 24 24 44 43 00 : E8 ?? ?? F? FF : B8 FF FF FF FF : EB ??}"

#define VLVSteamClientLog_Dst "EB 0E 89 C0"
#define VLVSteamClientLog_Dst_WiN "EB 0D"

#define VLVSteamClient_Steam3TicketRSAVerification "{E8 ?? ?? ?? ?? : 85 C0 : 74 ?? : 8B 00 : 2B F0 : 83 FE 01 : 7C ?? : 85 FF : 8D 0C 18 : 74 ?? : \
                                                     8B 97 00 04 00 00 : 52 : 57 : 56 : 50 : 53 _: E8 ?? ?? ?? ?? :_ 83 C4 14 : 84 C0 : 74 ?? ^ \
                                                     B9 ?? ?? ?? ?? : E8 ?? ?? ?? ?? : 85 C0 : 74 ?? : 8B 88 00 04 00 00 : 8B 55 0C : 51 : 8B 4D 08 : 50 : \
                                                     8B 07 52 : 56 : 50 : 51 _: E8 ?? ?? ?? ?? :_ 83 C4 18 : 84 C0 : 74 ?? ^ \
                                                                                    \
                                                                                    \
                                                     8B 83 ?? ?? ?? ?? : 89 44 24 08 : 8B 83 ?? ?? ?? ?? : 89 04 24 : E8 ?? ?? ?? ?? : 89 34 24 : 89 C7 : \
                                                     8B 45 10 : 89 44 24 08 : 8B 45 0C : 89 44 24 04 : E8 ?? ?? ?? ?? : 89 7C 24 04 : \
                                                     89 34 24 _: E8 ?? ?? ?? ?? :_ ^ \
                                                     [31 ?? : 89 4C 24 04] : 89 44 24 08 : [31 ?? : 89 44 24 04] : 8B 83 ?? ?? ?? ?? : 89 04 24 : \
                                                     E8 ?? ?? ?? ?? : 85 C0 : 89 C2 : 74 ?? : 8B 80 00 04 00 00 : 89 54 24 10 : 89 7C 24 0C : 89 44 24 14 : \
                                                     8B 85 ?? ?? ?? FF : 01 F0 : 89 44 24 08 : 8B 06 : 89 34 24 : 89 44 24 04 _: E8 ?? ?? ?? 00 :_ 0F B6 C0 : \
                                                     EB ??}"

#define VLVSteamClient_Steam3TicketRSAVerification_dst "31 C0 : 40 : 89 C0"


#define VLVSteamClient_Steam2LibraryReference_Win "`steam.dll`"
#define VLVSteamClient_Steam2LibraryReference_NiX "`libsteamvalidateuseridtickets_i486.so`"

#define VLVSteamClient_Steam2LibraryReference_Win_Dst "`eSTS2.dll`"
#define VLVSteamClient_Steam2LibraryReference_NiX_Dst "`libeST_STEAM2.so` 00"


#define VLVServerLibMagic_N1 "{ `ISERVERPLUGINHELPERS001` ^ `VSoundEmitter002` } 00 `ServerUploadGameStats001`"
#define VLVServerLibMagic_N2 "{ `VSERVERENGINETOOLS001` 00 00 ^ `ISERVERPLUGINHELPERS001` } 00 `SceneFileCache002`"


TBOOL GetValveEngineInfo(TSTREAMADDR_HDR const fileStream,
                         int32_t *vlvEngineBuild,
                         uint8_t *vlvProtocol,
                         char vlvVersion[20],
                         char vlvTime[9],
                         char vlvDate[40],
                         const TVUP_OSType fileOS);

TBOOL SetEngineProtocol(TSTREAMADDR_HDR const fileStream, uint8_t NewProtocol, uint8_t *OldProtocol, const TVUP_OSType fileOS);

int32_t GetValveBuildFromDate(char vlvDate[12]);

TBOOL IfValveSteamClient(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120]);
void PatchValveSteamClient(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[120]);

TBOOL IfValveSteamClient3(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120]);
void PatchValveSteamClient3(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[120]);

TBOOL IfValveServerLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120]);
void PatchValveServerLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL *PatchingResult,uint8_t NewPlayersLimit);

TBOOL IfValveDedicatedLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120]);
void PatchValveDedicatedLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,uint8_t NewPlayersLimit);

#endif
