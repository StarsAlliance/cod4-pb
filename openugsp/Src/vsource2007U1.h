
#ifndef _VSOURCE2007U1_H_
#define _VSOURCE2007U1_H_

#include <string.h>
#include "vupshared.h"
#include "vlvshared.h"



#define VLV2K7U1SteamValidation_Src "{E8 ?? ?? ?? 00 _ 8B C8 ^ {89 ?? 24 ^ 8B ?? 0C } [0?] _ 89 ?? 24 0? _ [89 04 24]} E8 {?? ?? ?? 00 ^ ?? ?? F? FF} \
                                  84 C0 __: 0F 85 {9? 00 00 00 ^ ?? F? FF FF} :__ E8 {?? ?? ?? 00 ^ ?? ?? F? FF} : 83 B8 ?? \
                                  00 00 00 01 : 0F 84 {8? 00 00 00 ^ ?? F? FF FF} [8B 44 24 40] 8B ?? [00] {{8B ?? ?? 00 00 00 ^ 8D ?3 ?? ?? F? FF} \
                                  {68 ?? ?? ?? ?? ^ E9 ?? ?? FF FF} ^ C7 44 24 ??}"
#define VLV2K7U1SteamValidation_Win_Dst "?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 90 E9 ?? ?? ?? ??"
#define VLV2K7U1SteamValidation_UNiX_Dst "?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 90 E9 ?? ?? ?? ??"

/*
#define VLV2K7SteamValidationProtectionCodeConsistencyCheck "{E8 ?? ?? ?? ?? : 83 B8 C8 00 00 00 01 : 0F 84 ?? ?? 00 00 : 8B 03 : 8B 88 B8 00 00 00 ^ \
                                                              E8 ?? ?? ?? ?? : 83 B8 9C 00 00 00 01 : 0F 84 ?? ?? 00 00 : 8B 4D 00 : 8B 91 B8 00 00 00 ^ \
                                                              E8 ?? ?? ?? ?? : 83 B8 F0 00 00 00 01 : 0F 84 ?? ?? ?? ?? : 8B 44 24 40 : 8B 10}"
*/

#define VLV2K7U1SteamValidationProtectionCodeConsistencyCheck "E8 ?? ?? ?? ?? : 83 B8 ?? 00 00 00 01 : 0F 84 ?? ?? {00 00 ^ FF FF} : 8B"


#define VLV2K7U1LANChk_Src "{8B ?6 : 8B 9? ?? 00 00 00 : 5? : 55 : 8B CE : FF D2 : 84 C0 _: 75 ?? :_ 8B 06 : 8B 88 ?? 00 00 00 : 68 ?? ?? ?? ?? : 55 : 56 \
                         FF D1 83 C4 0C 5B 5D 5F 33 C0 5E ^ \
                         8B 06 89 34 24 89 4C 24 04 FF 90 EC 00 00 00 84 C0 0F 84 17 01 00 00 8B 44 24 ?0 8B 54 24 ?8 8B 2E 89 44 24 08 \
                         89 54 24 04 89 34 24 FF 95 C0 00 00 00 84 C0 0F 84 01 01 00 00 8B 2E 89 7C 24 08 8B 4C 24 ?4 89 34 24 8B 7C 24 ?8 \
                         89 4C 24 0C 89 7C 24 04 FF 95 E8 00 00 00 84 C0 0F 85 ?7 FD FF FF 8B 44 24 ?8 31 ED ^ \
                         {8B 06 : 8B 7C 24 50 ^ 8? 24 ?? 00 00 00} : 8B 4C 24 ?4 : 89 ?? 24 : 89 ?? 24 08 : 89 4C 24 04 : FF 9? C? 00 00 00 : 84 C0 _: 0F 84 ?? ?? 00 00 :_ ^\
                         8B 2E : 8B 4C 24 54 : 8B 44 24 58 : 8B 54 24 44 : 89 34 24 : 89 4C 24 0C : 89 44 24 08 : 89 54 24 04 _: FF 95 E8 00 00 00 :_ 84 C0 : 0F 84 ?? ?? 00 00 ^ \
                         8B 75 00 : 8B 8C 24 A0 00 00 00 : 8B 94 24 94 00 00 00 : 89 4C 24 08 : 89 54 24 04 : 89 2C 24 : FF 96 C0 00 00 00 : 84 C0 : 0F 84 ?? ?? 00 00 ^ \
                         8B 06 : 8B 55 18 : 89 54 24 08 : 8B 4D 0C : 89 4C 24 04 : 89 34 24 : FF 90 C8 00 00 00 : 84 C0 _: 0F 84 ?? ?? 00 00 :_ 8B 06 : 8B 4D 1C : 89 4C 24 0C : 89 7C 24 08 : 8B 55 0C}"


#define VLV2K7U1LANChk_WinDst "?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? EB ?? ?? ?? ?? ?? ?? ?? ?? ??"
#define VLV2K7U1LANChk_UNiXDst_1 "?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? \
                                ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 66 89 C0 66 89 C0 ?? ??"
#define VLV2K7U1LANChk_UNiXDst_2 "?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 66 89 C0 66 89 C0 ?? ??"

#define VLV2K7U2SourceTVLANChk_Src "{8B 8E ?? ?? ?? 00 : 8B 79 08 : 8B 8E DC 00 00 00 : 8B 11 : 8B 82 C4 00 00 00 : 6A 02 : FF D0 : 8B 8E ?? ?? ?? 00 : \
                                     8B 9? ?? 00 00 00 : 50 : 83 C1 08 : FF D2 : 84 C0 _: 75 ?? :_ 8B 4E FC : 8B 51 30 : 8D 46 FC ^ \
                                     8B 9? ?? ?? ?? 00 : {83 C2 0? : C7 44 24 08 02 00 00 00 ^ C7 44 24 08 02 00 00 00 : 83 C2 0?} : \
                                     {89 44 24 04 : 89 14 24 ^ 89 14 24 : 89 44 24 04} : FF D? : 84 C0 : _: 0F 85 ?? ?? F? FF :_ 8B 0? }"

#define VLV2K7U1IDJunction "83 EC 10 : 56 : 8B 74 24 18 : C6 05 ?? ?? ?? ?? 00 : 8B 06 : 83 E8 01 : 74 ?? : 83 E8 02 : 74 20 :\
                          A1 ?? ?? ?? ?? : 8B 0D ?? ?? ?? ?? : A3 ?? ?? ?? ?? : 89 0D ?? ?? ?? ??"


/*
#define VLV2K7ServerReqSteam_Src "{8? C0 _: 75 ?? :_ 68 ?? ?? 2A 10 : 6A 01 : E8 ?? ?? F7 FF : 8B 06 : 8B 50 34 83 : C4 08 ^ \
                                  89 5C 24 10 : E8 ?? ?? FE FF : 81 C3 ?? ?? ?? 00 : 89 74 24 14 : 8B 8B ?? ?? 00 00 : 8B 74 24 20 : \
                                  89 7C 24 18 : 8B 01 : 8B 7C 24 24 : 8B 10 : 89 04 24 : FF 52 14 : 84 C0 : 0F 84 86 00 00 00 : 8D 57 FF : \
                                  81 FA FE 07 00 00 : 77 3B : 8D 8B ?? ?? F6 FF : 89 0C 24 : E8 ?? ?? FE FF}"
*/
#define VLV2K7U1ServerReqSteam_Src "56 : 8B F1 : E8 ?? ?? ?? ?? : 83 38 00 _: 75 ?? :_ 68 ?? ?? ?? ?? : 6A 01 : E8 ?? ?? F? FF : 8B 06 : 8B 50 3? : \
                                    83 C4 08 : 6A 01 : 8B CE : FF D2"

#define VLV2K7U1CDKeyRequiredForInternerServers "E8 ?? ?? ?? {00 ^ FF} : 84 C0 _: 75 ?? :_ 8B 0D ?? ?? ?? ?? : 8B 01 : 8B 50 14 : FF D2 : 84 C0 \
                                               74 ?? : E8 ?? ?? ?? FF : 84 C0 : 74 ??"

/*
#define VLV2K7U1JMPTabHelper "{CC CC CC CC CC CC CC CC CC CC : 8B 44 24 08 : 83 C0 FF : 83 F8 0D : 0F ?? ?? ?? 00 00 : FF 24 85 ?8 ?? ?? ?? : 8B 44 24 04 :\
                            8B 48 04 : 8B 51 30 : 83 C0 04 : 68 ?? ?? ?? ?? : 50 : FF D2 : 83 C4 08 : C2 0C 00 : 83 B9 C8 00 00 00 01 : 0F 84 ?? ?? 00 00 :\
                            8B 44 24 04 : 8B 48 04 : 8B 51 30 ^ \
                            83 EC ?? : 8B 44 24 ?8 : [89 5C 24 14 : 8B 4C 24 20] : E8 ?? ?? FA FF : 81 C3 ?? ?? ?? 00 : [89 74 24 18] : 8B ?? 24 ?? :\
                            83 F8 0E : [8B 74 24 24] _: 7? ?? :_}"
*/
#define VLV2K7U1JMPTabHelper "{8B 44 24 20 : 83 C0 FF : 83 F8 0D : 0F 87 ?? ?? 00 00 :_ FF 24 85 ?? ?? ?? ?? :_ 68 ?? ?? ?? ?? : E9 ?? ?? 00 00 :\
                               83 BE ?? 00 00 00 01 _: 0F 84 ?? ?? 00 00 :_ 68 ?? ?? ?? ?? : E9 ?? ?? 00 00 ^ \
                               83 C4 20 : 5B : 5E : 5F : C3 ____: 90 :____ 89 D? : 8B 8? B3 ?? ?? ?? ?? : 29 C? : FF ^ \
                               8B 74 24 ?? : 89 7C 24 ?? : 8B 7C 24 ?? : E8 ?? ?? F? FF : 83 F8 02 : 0F 94 C3 : 74 ?? : 83 FE 0E \
                               _: 76 ?? :_ 8B 37 : C7 44 24 ?? ?? ?? ?? 00 : 89 3C 24 : FF 56 3C : 8D 76 00 ___: 8B 5C 24 ?? : 8B 74 24 ?? : 8B 7C 24 ?? : 83 C4 2C : C3 _: FF 24 B5 ?? ?? ?? ?? ^ \
                               83 C4 ?? : 5B : 5E : 5F : 5D : C3 _: 8B 8? B3 ?? ?? ?? ?? : 01 ?? : FF}"
                               /* Last one is for L4D2 23.04.2010 for Linux update */

#define VLV2K7U2JMPTabExtraHelper_UNiX "E8 ?? ?? ?? ?? : 83 F? 0E : 76 ?? : 8B ?? : C7 44 24 04 ?? ?? ?? 00 : 89 ?? 24 : FF 5? 3C : [66] : 90 _: \
                                         83 C4 ?? : 5B : 5E : 5F : 5D : C3 _: FF 24 ?? ?? ?? ?? ??"

/*
#define VLV2K7U1SteamCertificateChecking_Src "{__: 8D 55 FF : 81 FA FE 07 00 00 :__ 89 46 70 : 89 46 74 _: 0F 87 ?? 00 00 00 :_ 8B 7C 24 28 : 8B 4F 04 : 8B 07 : \
                                             8B 57 08 : 89 4C 24 14 : 8B CF : 89 44 24 10 : 89 54 24 18 : E8 ?? ?? ?? 00 : 83 F8 01 : 74 ??  ^ \
                                             E8 ?? ?? F? FF : 8B 4C 24 68 __: 49 : 81 F9 FE 07 00 00 : 76 ??  :_ 8B 54 24 68 : B9 00 08 00 00 : 8D 83 ?? ?? F? FF : \
                                             8B 37 : 89 4C 24 10 : 89 54 24 0C : 89 44 24 08 : 89 6C 24 04 : 89 3C 24 : FF 96 ?? ?? 00 00 : 31 D2 ^ \
                                             8D 47 FF : 3D FE 07 00 00 : 0F 87 ?? ?? 00 00 : 8B 74 24 30 : 8B 0E : 8B 56 04 : 8B 46 08 : 89 4C 24 18 : 8B CE : 89 54 24 1C : 89 44 24 20 : E8 ?? ?? ?? ?? ^ \
                                             C7 47 64 00 00 00 00 : C7 47 68 00 00 00 00 : 8B 54 24 58 ___: 83 EA 01 : 81 FA FE 07 00 00 ___: 0F 87 ?? ?? 00 00 :_ 8B 4C 24 4C : \
                                             8D 7C 24 28 : 8B 01 : 89 44 24 28 : 8B 51 04 : 89 54 24 2C : 0F B7 41 08 : 89 0C 24 : 66 89 44 24 30 : E8 ?? ?? F? FF : \
                                             83 E8 01 : 74 10 : 8B 4C 24 4C : 89 0C}"
*/


/*

   We have 6 patterns for L4D1 and L4D2(Win/NiX)
   8D 55 FF                  lea EDX,[EBP - 1]
   8B 4C 24 68 : 49          mov ECX,[ESP + 0x68]  : dec ECX
   8D 4? FF     <- PROBLEM   lea REG1,[REG2 - 1]
   8D 43 FF                  lea EAX,[EBX - 1]
   8B 54 24 58 : 83 EA 01    mov EDX,[ESP + 0x58] :  sub EDX,1
   8B 5C 24 ?? : 83 EB 01    mov EBX,[ESP + ??] : sub EBX,1

   L4D2(NiX) 23.04.2010 update:
   8B 45 20 : 83 E8 01 mov EAX,[ESP+0x20] : sub EAX,1

*/
#define VLV2K7U1SteamCertificateChecking_Src "{__: 8D 55 FF : 81 FA FE 07 00 00 :__ 89 46 70 : 89 46 74 _: 0F 87 ?? 00 00 00 :_ 8B 7C 24 28 : 8B 4F 04 : 8B 07 : \
                                             8B 57 08 : 89 4C 24 14 : 8B CF : 89 44 24 10 : 89 54 24 18 : E8 ?? ?? ?? 00 : 83 F8 01 : 74 ??  ^ \
                                             8D 4? FF : 3D FE 07 00 00 : 0F 87 ?? ?? 00 00 : 8B 74 24 30 : 8B 0E : 8B 56 04 : 8B 46 08 : 89 4C 24 18 : 8B CE : 89 54 24 1C : 89 44 24 20 : E8 ?? ?? ?? ?? ^ \
                                             8B 4C 24 68 __: 49 : 81 F9 FE 07 00 00 : {_76 ??_ ^ 0F 87 ?? ?? 00 00} ^ \
                                             8B 54 24 58 ___: 83 EA 01 : 81 FA FE 07 00 00 ___: 0F 87 ?? ?? 00 00 :_ 8B 4C 24 4C : \
                                             8D 7C 24 28 : 8B 01 : 89 44 24 28 : 8B 51 04 : 89 54 24 2C : 0F B7 41 08 : 89 0C 24 : 66 89 44 24 30 : E8 ?? ?? F? FF : \
                                             83 E8 01 : 74 10 : 8B 4C 24 4C : 89 0C ^ \
                                             8B 5C 24 ?? : 83 EB 01 : 81 FB FE 07 00 00 _: 0F 87 ?? ?? 00 00 :_ 8B 54 24 ?? : 8B 02 ^ \
                                             8B 45 20 : 83 E8 01 : 3D FE 07 00 00 : 0F 87 ?? ?? 00 00 : 8B 07 : 89 45 D4}"

#define VLV2K7U1NotifyClientConnectCallPrep "{8B ?? ?? ?? : 8B ?? ?? ?? : 5? : 5? : 8D ?? ?? ?? : 5? : 5? : 5? : E8 ?? ?? ?? 00 ^ \
                                              8B ?? ?? ?? : 8B ?? ?? ?? : 89 ?? ?? ?? : 8? ?? ?? ?? : 8? ?? ?? ?? : 8B ?? ?? ?? : 89 ?? ?? ?? ^ \
                                              8B 55 ?? : 89 ?? ?? ?? : 8B 55 ?? : 89 ?? ?? ?? : 8B 55 ?? : 89 ?? ?? ?? : 8B 55 ?? : 89 ?? ?? ?? : 8B 55 ?? : 89 ?? ?? ??}"

#define VLV2K7U1SteamCertificateChecking_Dst1 "66 89 C0 66 89 C0"
#define VLV2K7U1SteamCertificateChecking_Dst2 "89 C0"
#define VLV2K7U1SteamCertificateChecking_Dst3 "66 89 C0"

#define VLV2K7U1SteamCertificateChecking_Win_Dst "?? ?? 00 : ?? FF ?? ?? ??"
#define VLV2K7U1SteamCertificateChecking_UNiX_Dst1 "?? ?? ?? ?? : 90 : ?? ?? FF ??"
#define VLV2K7U1SteamCertificateChecking_UNiX_Dst2 "?? ?? ?? ?? : 66 89 C0 : ?? ?? FF ?? ?? ??"

/*
#define VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP "{8B 44 24 ?? : 8B 54 24 ?? : 55 : 50 : 8D 4C 24 ?? : 51 : 52 : 56 ^ \
                                                                8B 4C 24 ?? : 8B 44 24 ?? : 57 : 51 : 8D 54 24 ?? : 52 : 50 : 53 ^ \
                                                                8B 4C 24 ?? : 8B 54 24 ?? : 89 7C 24 ?? : 8B 7C 24 ?? : 89 4C 24 ?? : 8B 4C 24 ?? : 89 54 24 ?? : \
                                                                89 7C 24 ?? : 89 04 24 : 89 4C 24 ??}"
*/

#define VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP "{8B ^ 89} ?? 24 ?? : {8B ^ 89} ?? 24 ??"


/*
85 ED      test EBP,EBP
85 FF      test EDI,EDI
85 D2      test EDX,EDX
85 C9      test ECX,ECX
85 DB      test EBX,EBX

*/

#define VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP3_Dst "85 FF __: 74 00 __:_ 90 90 90 90 90 90 90 90 _:__ EB 00 __"
#define VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP2_Dst "85 FF __: 74 00 __:_ 90 90 90 90 90 90 90 90 _:__ E9 00 00 00 00 __"
//#define VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP_UNIX_Dst "90 90 90 90 : 85 FF __: 74 00 __:_ 90 90 90 90 90 90 90 90 _:__ E9 00 00 00 00 __"
#define VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP_UNIX_Dst "50 : 90 90 90 90 : 85 FF : 50 __ : 74 00 __ : 90 90 90 90 90 90 90 90 _:__ E9 00 00 00 00 __"


#define VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP_VALiDATiON_NiX "{8B 0F : 8D 83 ?? ?? F9 FF ^ 8B 0E : 8B ?? 24 ?? : 8B ?? 24 ?? : C7 ?? 24 ?? ?? ?? ?? ?? ^ \
                                                                               8B 06: C7 ?? 24 ?? ?? ?? ?? ?? : 8B 55 20 : 89 ?? 24 ?? : 8D ?3 ?? ?? F? FF}"



#define VLV2K7U1ExecutionPermissionChecking_Src "E8 ?? ?? F? FF : 8B 48 14 : 8B ?1 : 8B ?4 F5 ?? ?? ?? ?? : 8B ?? 18 : 5? : FF D? : 84 C0 _: 75 ?? :_ E8 ?? ?? E? FF : \
                                                 50 : 68 ?? ?? ?? ?? : FF 15 ?? ?? ?? ?? : 83 C4 08"

#define VLV2K7U1SteamOwnershipChecking_Src "E8 ?? ?? F? FF : 8B 48 14 : 8B 01 : 8B 10 : FF D2 : 84 C0 _: 75 ?? :_ 68 ?? ?? ?? ?? : E8 ?? ?? F? FF"


#define VLV2K7U1MasterRequestRestartNag_Src "{8B CD : FF D2 : 84 C0 __: 0F 84 ?? ?? 00 00 :__ 8B 35 ?? ?? ?? ?? : 6A 03 : 68 ?? ?? ?? ?? : FF D6 ^ \
                                             84 C0 _: {74 ?? ^ 0F 84 ?? ?? F? FF}  :_ 8D 83 ?? ?? F? FF : [BF 03 00 00 00] : 89 04 24 : {89 7C 24 04 ^ C7 44 24 04 ?? 00 00 00} : \
                                             E8 ?? ?? E? FF : E8 ?? ?? E? FF : [8B 28] : 8D 93 ?? ?? F? FF ^ \
                                             84 C0 : 75 ?? : 83 C4 14 : 5B : 5E : C3 ____: 90 :____ 8D 83 ?? ?? ?? FF : C7 44 24 04 03 00 00 00 : 89 04 24 _: E8 ?? ?? ?? FF : \
                                             E8 ?? ?? ?? FF  : 8D 93 ?? ?? ?? FF}"
#define VLV2K7U1MasterRequestRestartNag_Dst_Win32 "?? ?? : ?? ?? : ?? ??   : 90 E9"
#define VLV2K7U1MasterRequestRestartNag_Dst_UNiX_1 "?? ?? : EB"
#define VLV2K7U1MasterRequestRestartNag_Dst_UNiX_2 "?? ?? : 90 E9"
#define VLV2K7U1MasterRequestRestartNag_Dst_UNiX_3 "?? ?? : 89 C0"

#define VLV2K7U1LobbyRequirement_Src "{8B CF : E8 ?? ?? F? FF : 84 C0 _: 74 ?? :_ 8B 87 70 01 00 00 : 3B C6 : 8B 8F 74 01 00 00 : 75 ?? : 3B CD : 74 ?? ^ \
                                      8B 8C 24 E0 0D 00 00 : 8B 54 24 60 : 8B 7C 24 64 : 8B B1 68 01 00 00 : 8B A9 6C 01 00 00 : 31 EA : 31 F7 : 09 FA _: 0F 84 ?? ?? F? FF :_ ^ \
                                      8B ?? ?? 01 00 00 : 8B A8 ?? 01 00 00 : 89 E? : 09 ?? _: 0F 84 ?? ?? 00 00 :_ 8B 54 24 ?0 : 8B 4? 24 ?4 : 31 EA : 31 ?? : 09 C? : 0F 84 ?? ?? 00 00 ^ \
                                      8B ?? ?? 01 00 00 : 89 8D ?? ?? F? FF : 89 C8 : 0B 85 ?? ?? F? FF _: 0F 84 ?? ?? F? FF : 8B 95 ?? ?? F? FF : 31 F? : 8B 85 ?? ?? F? FF : 31 F? : 09 C? : 0F 84 ?? ?? F? FF}"

#define VLV2K7U1LobbyMembershipRequirement_Src "{8B CE : E8 ?? ?? F? FF : 84 C0 : 74 ?? : 8B CE : E8 ?? ?? F? FF : 84 C0 _: 75 ?? :_ 84 DB : 75 ?? ^ \
                                                8B 8D 6C 01 00 00 : 0B 8D 68 01 00 00 _: 0F 85 ?? ?? F? FF :_ 89 F0 : 84 C0 : 0F 85 ?? ?? F? FF : 8B 55 00 : 8B 84 24 94 00 00 00 : 8D B3 ?? ?? ?? FF ^ \
                                                FF 56 60 : 84 C0 : 74 ?? : A1 ?? ?? 40 00 : 8B 78 30 : 85 FF _: 74 ?? :_ 8B 0D ?? ?? ?? 00 : 8B 51 30 : 85 D2 : 75 ?? : 8B 8D ?? 01 00 00 : \
                                                0B 8D ?? 01 00 00 : 75 ?? : 84 DB : 75 ?? ^ \
                                                FF 50 60 : 84 C0 : 74 ?? : 8B 83 ?? ?? ?? 00 : 8B 40 30 : 85 C0 _: 74 ?? :_ 8B 83 ?? ?? F? ?? : 8B 40 1C : 8B 48 30 : 85 C9 : 75 ?? : 8B 86 ?? ?? 00 00 : \
                                                0B 86 ?? ?? 00 00 : 75 ??}"


#define VLV2K7U1LobbyClientSideCheckIfServerRequiresIt_Src "83 3D ?? ?? ?? ?? 02 : 0F 8D ?? ?? 00 00 : 8B 8E ?? 0? 00 00 : 0B 8E ?? 0? 00 00 : 57 _: 0F 84 ?? ?? 00 00 :_ 8B 0D ?? ?? ?? ??"


#define VLV2K7U2ClientAcceptExistingSteamIDCheck_UNiX "83 C6 01 : 3B 35 ?? ?? ?? ?? _: 0F 8D ?? ?? 00 00 :_ 8B 1D ?? ?? ?? ?? : 8B 0C B3 : 31 DB : 8D 51 04 : 85 C9 : 0F 45 DA : 8B 03 : 89 1C 24 : \
                                                       FF 50 ?? : 84 C0 : 74 ?? : 8B 03 : 89 1C 24 : FF 90 ?? ?? 00 00 : 84 C0 : 75 ?? : 8B 0B"

#define VLV2K7U1CheckDuplicateSteamIDFunc "{83 B9 ?? 00 00 00 01 : 89 0C 24 _: 75 08 :_ 32 C0 : 83 C4 ?? : C2 04 00 : 53 : 55 : 33 ED : 39 2D ?? ?? ?? ?? : 56 : 57 : 0F 8E ?? ?? 00 00 ^ \
                                           83 B8 ?? 00 00 00 01 _: 0F 84 ?? ?? 00 00 :_ 8B BB ?? ?? FF FF : 8B 8F ?? ?? 00 00 : 85 C9 : 0F 8E ?? ?? 00 00 : 8B 74 24 ?? : 8B 54 24 ?? : \
                                           83 C6 04 : 31 ED : 89 74 24 ?? : 85 D2 : 0F 84 ?? ?? 00 00 : 8D 74 24 ?? : 89 74 24 ?? : EB ?? ^ \
                                           83 B8 ?? 00 00 00 01 _: 0F 84 ?? ?? 00 00 :_ 8B 15 ?? ?? ?? ?? : 85 D2 : 0F 8E ?? ?? 00 00 : 8D 4D 04 : 31 FF : 89 4C 24 ?? : EB ??}"




TBOOL IfSrc2K7U1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);


void PatchSrc2K7U1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200]);



#endif
