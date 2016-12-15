#ifndef _UE25_H_
#define _UE25_H_

#include "vupshared.h"

#define UE25_MAGIC "{`FUnrealfileSummary<<` 00 `FGenerationInfo<<` 00 `ULinkerLoad::VerifyImport` 00 ^ `/ROGame.u` 00 00 00 `/ROEngine.u` 00 `/ROEffects.u` 00 00 00}"

#define UE25_STEAM3DENY_AUTH_CHECK_SRC "{0F 8E ?? ?? 00 00 : 8B 40 30 : 55 : 56 : 8B 30 : 8B 9E ?? 14 00 00 : 3B 5C 24 ?? : 75 ?? : 8B 9E ?? 14 00 00 : \
                                         3B 5C 24 ?? _: 74 ?? :_ 41 : 83 C0 04 : 3B CA 7C ?? ^ \
                                         8B 75 ?? : 8B 4D ?? : 8B 46 30 : 8B 34 98 : 8B 96 ?? 14 00 00 : 8B 86 ?? 14 00 00 : 31 D1 : 8B 55 ?? : 31 D0 : \
                                         09 C1 _: 0F 84 ?? ?? 00 00 :_ 43 : 39 5F 04 : 7F ??}"




#define UE25_STEAM3DENY_AUTHS_DST "66 89 C0 66 89 C0"
#define UE25_STEAM3DENY_AUTHS_DST2 "89 C0"



TBOOL IfUE25Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);

void PatchUE25Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200]);



#endif
