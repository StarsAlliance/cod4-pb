#ifndef _VGOLDSRC_H_
#define _VGOLDSRC_H_

#include <string.h>
#include "vupshared.h"
#include "vlvshared.h"

#define HWRNDR_MAGIC "`hw.dll` 00 3F 3F"
#define SWRNDR_MAGIC "`sw.dll` 00 3F 3F"


#define VLVGoldSSteamValidation_Src "{51 : 52 : E8 ?? ?? 00 00 : 83 C4 0C : 85 C0 _: 0F 85 ?? ?? 00 00 :_ D9 05 ?? ?? ?? ?? : D8 1D ?? ?? ?? ?? : DF E0 : F6 C4 40 \
                                      _: 74 ?? :_ {8D 45 ?? ^ 8D 44 ?? ??} : 68 ?? ?? ?? ?? : 50 : E8 ?? ?? FF FF : 83 C4 08 ^ \
                                      52 : 25 FF FF 00 00 : 51 : 53 : 66 89 43 ?? : E8 ?? ?? ?? 00 : 83 C4 0C : 85 C0 : _: 0F 85 ?? ?? 00 00 :_ ^ \
                                      57 : 51 : {8B 85 ^ FF B5} ?? ?? FF FF : [50] : E8 ?? ?? ?? FF : 83 C4 10 : 85 C0 : _: 0F 85 ?? ?? 00 00 :_ \
                                      8B 83 ?? ?? 00 00 : D9 EE: D9 40 0C : D? E9 : D? ?? : {0F ^ 80} ?? ?? : {0F ^ 80} ?? ??}"


#define VLVGoldSSteamValidation_Dst "90 E9"


//50 51 52
#define VLVGoldSrcLANChk_Src "{5? : 5? : E8 ?? ?? FF FF : 83 C4 08 : 85 C0 _: 75 ?? :_ [{8D 54 ?? ?? ^ 8D 55 ?? ^ 8D 4D ??}] : 68 ?? ?? ?? ?? : {5? : E8 ?? ?? FF FF : 83 C4 08 ^ E9 ?? ?? ?? ?? : 6A 04} ^ \
                               89 E7 : [8B B3 ?? ?? 00 00] : 83 C4 EC : FC : B9 05 00 00 00 : F3 A5 : 89 E7 : 8D 75 ?? : FC : B9 05 00 00 00 : F3 A5 : E8 ?? ?? ?? FF : \
                               83 C4 30 : 85 C0 _: 75 ?? :_ 83 C4 F4 : 83 C4 EC : 89 E7 : 8D 75 ?? : FC : B9 05 00 00 00 : F3 A5 : E8 ?? ?? ?? FF : \
                               83 C4 20 : 85 C0 _: 75 ?? :_}"
#define VLVGoldSrcLANChk_Dst "EB"


#define VLVGoldSrcJMPTabHelper "{{8B 44 24 ?? ^ 8B 45 ??} : 48 : 83 F8 0D _: 0F 87 ?? ?? 00 00 :_ _: FF 24 85 ?? ?? ?? ?? :_ {8B 4C 24 ?? ^ 8B 4D ??} : 68 ?? ?? ?? ?? : \
                                6A 00 : 51 : E8 ?? ?? ?? FF : 83 C4 0C  ^ \
                                81 C3 ?? ?? ?? ?? : {8B 54 24 20 : 8B ?? 24 24 : 8B ?? 24 2C : 8B 44 24 28 ^ 8B 55 08 : 8B ?? 0C : 8B ?? 14 : 8B 45 10} : 48 : \
                                83 F8 0D _: 0F 87 ?? ?? 00 00 :_ 89 DF : 2B BC 83 ?? ?? ?? FF : FF E7}"


#define VLVGoldSrcSafeLabelValidate "{{8B ?5 ?? ^ 8B 44 24 ??} : 68 ?? ?? ?? ?? : 6A 00 : 5? : E8 ?? ?? ?? FF : 83 C4 0C : [5D] : C2 0C 00 ^ \
                                      83 C4 FC : 8D 83 ?? ?? ?? FF : 50 : 6A 00 : 5? : E8 ?? ?? ?? FF : {83 ^ 8D} ?? ?? : 5B : 5E : 5F }"


#define VLVGoldSrcExploitableCode_Src "{83 F? 20 _: 7D ?? :_ 8A 0? : 8B C? : 3C 28 : 75 ?? : 80 7? 02 29 : 75 ?? : 8D 4? 03 : EB ?? ^ \
                                        83 7D DC 1F _: 0F 8F ?? ?? 00 00 :_ 83 7D 0C 00 : 74 ?? : 8B 45 10 : 39 45 DC : 74 ?? : 83 7E 0C 00 : 74 ?? ^ \
                                        83 FD 1F _: 0F 8F ?? ?? 00 00 :_ 83 7C 24 54 00 : 74 ?? : 3B 6C 24 58 : 74 ?? : 83 7E 0C 00 : 74 ??}"
//#define VLVGoldSrcExploitableCode_Dst "EB"

#define VLVGoldSrcSVParseVoiceDataExploitableCode_Src "{8B 15 ?? ?? ?? ?? : 56 : 8B 75 08 : B8 ?? ?? ?? ?? : 8B CE : 2B CA : F7 E9 : C1 FA 0B : 8B C2 : \
                                                        C1 E8 1F : 03 D0 : 89 ?? F4 : E8 ?? ?? ?? ?? : 3D 00 10 00  00 : 89 ?? F8 _: 76 ?? :_ \
                                                        68 ?? ?? ?? ?? : E8 ?? ?? ?? ?? :  83 C4 04 : 5E : 8B E5 : 5D : C3 ^ \
                                                        8B 93 ?? ?? 00 00 : 8B 45 08 : 2B 42 04 : 69 C0 ?? ?? ?? ?? : C1 F8 03 : 89 85 ?? ?? FF FF : \
                                                        E8 ?? ?? ?? ?? : 89 85 ?? ?? FF FF : 3D 00 10 00 00 _: 76 ?? :_ 83 C4 F4 : 8D 83 ?? ?? FF FF : \
                                                        50 : E8 ?? ?? ?? ?? : [83 C4 10] : E9 ?? ?? 00 00}"

#define VLVGoldSrcSVParseVoiceDataErrorString_Src "`SV_ParseVoiceData: invalid incoming packet.` 0A"
#define VLVGoldSrcSVParseVoiceDataErrorString_Dst "`VSERVSEC: HLDS_VCRASH EXPLOIT DETECTED.` 0A 00"

#define VLVGoldSrcLog_PrintfFunc "{81 C1 6C 07 00 00 : 51 : 40 : 52 : 50 : 68 ?? ?? ?? ?? : 68 00 04 00 00 : 68 ?? ?? ?? ?? : E8 ?? ?? ?? ?? ^ \
                                   05 6C 07 00 00 : 50 : {8B 42 0C : 50 ^ FF 72 0C } : 8B 42 10 : 40 : 50 : 8D 83 ?? ?? ?? FF : 50 : 68 00 04 00 00 : \
                                   [8D B3 ?? ?? ?? ??] : 56 : E8 ?? ?? ?? FF : 83 C4 30}"

#define MachCodeFuncStart "{55 : 8B EC ^ 55 : 89 E5 : 83 EC ?? : 57 ^ 83 EC ?? : 55}"

#define VLVGoldSrcCheckDuplicateSteamIDFunc "{D9 05 ?? ?? ?? ?? : D8 1D ?? ?? ?? ?? : C7 45 ?? FF FF FF FF : DF E0 : F6 C4 40 _: 75 ?? :_  83 C8 FF : 8B E5 : 5D : C3 ^ \
                                              C7 45 ?? FF FF FF FF : 31 F6 : 8B 83 ?? ?? 00 00 : D9 EE : D9 40 ?? : DA E9 : DF E0 : 80 E4 45 : 80 FC 40 _: 74 ?? :_ EB ?? ^ \
                                              C7 44 24 ?? FF FF FF FF : 31 F6 : D9 40 ?? : DF E9 : DD D8 : 0F 95 C0 : 0F 9A C4 : 08 C4 _: 74 ?? :_ EB ?? ^ \
                                              C7 44 24 ?? FF FF FF FF : BE 00 00 00 00 : D9 40 ?? : DA E9 : DF E0 : 80 E4 45 : 80 FC 40 _: 74 ?? :_ EB ??}"

#define VLVGoldSrcEmptyCallStatement_Dst "66 89 C0 : 89 C0"


TBOOL IfGoldSrcEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);

void PatchGoldSrcEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200]);


#endif
