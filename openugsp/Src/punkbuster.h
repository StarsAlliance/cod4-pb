#ifndef _PBUSTER_H_
#define _PBUSTER_H_

#include "vupshared.h"



#define PunkBuster_Magic1 "`PunkBuster Server` 00 00 00 `GuidAuth` 00 00 00 00"
#define PunkBuster_Magic2 "`GuidAuth` 00 `PunkBuster Server` 00"
#define PunkBuster_Magic3 "`PunkBuster Server (v%d.%03d | A%d C%d.%03d) %s<br>`"

#define PunkBuster_EmptyCDKey_Src "A1 5C 2F 07 10 : 69 C0 E8 03 00 00 : 99 : 3B CA _: 7C 18 :_ 7F 04 : 3B F0 : 76 12 : 8B 44 24 10 : 68 44 66 04 10 : \
                                   6A 77 : 55 : 50 : E9 67 04 00 00 ^ \
                                   8D 83 ?? 20 2D 00 : 50 : 68 ?? ?? 2A 00 : E8 ?? 06 10 00 : 83 C4 10 : 85 C0 _: 75 4F :_ 8B 4D 0C : 2B 8B ?? ?? 2D 00 : \
                                   89 C8 : 79 0F : 31 C9 : 3D 78 EC FF FF : 7F 06 : 8D 88 00 00 00 80"

#define PunkBuster_DuplicateGUID_1_Src "         ^ \
                                        0F 84 80 01 00 00 : 8B 87 ?? ?? 2D 00 : 85 C0 : 0F 8E 72 01 00 00 : 83 F8 03 : 0F 8F 69 01 00 00 : 8D 04 B6 : 83 C4 F8 : \
                                        89 C3 : 8B 8D 50 CD FF FF : C1 E3 04 : 51 : 29 C3 : C1 E3 04 : 8D 83  ?? 20 2D 00 : 50 : E8 ?? ?? 10 00 : 83 C4 10 : 85 C0 : \
                                        0F 85 3B 01 00 00 : BE ?? 0E 33 00 : 81 C3 ?? ?? 2D 00 : 80 3D 69 0E 33 00 00 _: 74 59 :_ 89 F0 : 89 F1 : 83 C4 FC : 83 E1 03"
#define PunkBuster_DuplicateGUID_2_Src ""


TBOOL IfPunkBuster(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);

void PatchPunkBuster(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,char Version[200]);

#endif
