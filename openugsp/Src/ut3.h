#ifndef _UT3_H_
#define _UT3_H_

#include "vupshared.h"

#define UT3_MAGIC "{`Supports Offscreen Render Targets: %s` 00 00 00 `Version: %f` 00 `Name: %s` 00 00 00 00 ^ \
                    00 00 00 00 00 00 00 `U` 00 00 00 `T` 00 00 00 00 00 00 00}"

#define UT3_CDKEY_AUTH_SRC "{D1 E8 : 83 E0 01 : 0F 85 ?? ?? 00 00 : 8B 4D 08 : 81 C1 CC 00 00 00 : E8 ?? ?? ?? FF : 85 C0 _: 0F 8E ?? ?? 00 00 :_ 8B 4D 08 : \
                             81 C1 D8 00 00 00 : E8 ?? ?? ?? FF : 85 C0 _: 0F 8E ?? ?? 00 00 :_ ^ \
                             8B 45 0C : 05 CC 00 00 00 : 89 85 D8 FE FF FF : 8B 40 04 : 85 C0 _: 0F 84 ?? ?? 00 00 :_ 83 E8 01 : 85 C0 _: 0F 8E ?? ?? 00 00 :_ 8B 55 0C : \
                             8B 82 DC 00 00 00 : 85 C0 _: 0F 84 ?? ?? 00 00 :_ 83 E8 01 : 85 C0 _: 0F 8E ?? ?? 00 00 :_ 8B 55 0C : 8B 02 : 89 14 24 : FF 90 ?? 01 00 00}"


#define UT3_CDKEY_REAUTH_SRC "{89 8D 68 FF FF FF : 8B 4D 08 : 81 C1 CC 00 00 00 : E8 ?? ?? ?? FF : 85 C0 _: 0F 8E ?? ?? 00 00 :_ 8B 4D 08 : 81 C1 D8 00 00 00 : \
                               E8 ?? ?? ?? FF : 85 C0 _: 7E ?? :_ ^ \
                               81 EC AC 00 00 00 : 8B 55 0C : 8B 8? D0 00 00 00 : 85 C0 _: 0F 84 ?? ?? 00 00 :_ 83 E8 01 : 85 C0 _: 0F 8E ?? ?? 00 00 :_ 8B 82 DC 00 00 00 :\
                               85 C0 _: 0F 84 ?? ?? 00 00 :_ 83 E8 01 : 85 C0 _: 0F 8E ?? ?? 00 00 :_ 8B 55 0C : 8B ?? DC 00 00 00 : 85 ?? : 0F 85 ?? ?? 00 00 : \
                               C7 85 6C FF FF FF 01 00 00 00}"

#define UT3_CDKEY_AUTHS_DST "66 89 C0 66 89 C0"
#define UT3_CDKEY_AUTHS_DST2 "89 C0"


#define UT3_CDKEY_INVALIDAUTH_SRC "{8B 4D EC : 83 C1 01 : 89 4D EC : 8B 4D F0 : 83 C1 44 : E8 ?? ?? ?? FF : 39 45 EC _: 7D ?? :_ 8B 55 EC : 52 : 8B 4D F0 : 83 C1 44 : \
                                    E8 ?? ?? ?? ?? : 8B 00 : 89 45 E8 : 8B 4D E8 : 8B 91 E4 00 00 00 : 3B 55 0C _: 75 ?? :_ ^ \
                                    8B 50 44 : 8B 1A : 31 C0 : 3B B3 E4 00 00 00 _: 75 ?? _:_ EB ?? :_ 8B 1C 82 : 39 B3 E4 00 00 00 _: 74 ?? :_ 83 C0 01 : 39 C8 : \
                                    75 ?? : [90] : 8D 74 26 00 : {8D BC 27 00 00 00 00 ^ 83 C4 1C}}"
#define UT3_CDKEY_INVALIDAUTH_DST "EB"


TBOOL IfUT3Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);

void PatchUT3Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200]);



#endif
