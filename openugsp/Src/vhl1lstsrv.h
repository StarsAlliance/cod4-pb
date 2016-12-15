#ifndef _VHL1LSTSRV_H_
#define _VHL1LSTSRV_H_

#include "vupshared.h"
#include "vlvshared.h"

#define VHL1LSTHWRNDR_MAGIC "`hw.dll` 00 3F 3F"
#define VHL1LSTSWRNDR_MAGIC "`sw.dll` 00 3F 3F"

#define HDRS_SIZE 0xF40

typedef struct
{
  uint32_t    Characteristics;
  uint32_t    Sections;
  uint32_t    copywhat;
  uint32_t    ImageBase;
  uint32_t    EntryPoint;
  uint32_t    ImportTable;
} THL1LstHDR;


#define VLVGoldSrcCLListenSteamValidation_Src "D9 05 ?? ?? ?? ?? : D8 1D ?? ?? ?? ?? : DF E0 : F6 C4 40 _: 74 ?? :_ 8D {45 DC ^ 44 24 1C} : 68 ?? ?? ?? ?? : 50 :\
                                               E8 ?? ?? FF FF : 83 C4 08 : 5F : 5E"
#define VLVGoldSrcCLListenSteamValidation_Dst "EB"



#define VLVGoldSrcCLListenSteamCertChk_Src "A1 ?? ?? ?? ?? : 89 9A 10 58 00 00 : 89 82 A4 4B 00 00 : 40 : A3 ?? ?? ?? ?? : 8B {45 F4 ^ 44 24 18}  : 83 F8 03 : 0F 85 ?? ?? 00 00 : \
                                            8B ?D ?? ?? ?? ?? : A1 ?? ?? ?? ?? : 2B ?8 : {85 DB ^ 3B EB} _: 0F 8E ?? ?? 00 00 :_ 81 F? 00 08 00 00 : 0F 8D ?? ?? 00 00"

#define VLVGoldSrcCLListenSteamCertChk_Dst "66 89 C0 66 89 C0"



#define VLVGoldSrcCLListenLANChk_Src "E8 ?? ?? ?? FF : 83 C4 24 : 85 C0 : 0F 84 ?? ?? 00 00 : 8B {45 F4 ^ 44 24 18} : 8D {4D DC ^ 4C 24 1C} : 50 : 51 : E8 ?? ?? ?? FF : 83 C4 08 : \
                                      85 C0 __: 75 ?? :__ 8D {55 DC ^ 54 24 1C} : 68 ?? ?? ?? ?? : 52 : E8 ?? ?? ?? FF : 83 C4 08"
#define VLVGoldSrcCLListenLANChk_Dst "EB"


#define VLVGoldSrcCLListenJMPTabHelper "8B {45 0C ^ 44 24 08} : 48 : 83 F8 0D : 0F 87 ?? ?? 00 00 __: FF 24 85 ?? ?? ?? ?? :__ 8B {4D 08 ^ 4C 24 04} : 68 ?? ?? ?? ?? : 6A 00 : 51 : E8 ?? ?? ?? FF : 83 C4 0C"

#define VLVGoldSrcCLListenSafeLabelValidate "8B {45 08 ^ 44 24 04} : 68 ?? ?? ?? ?? : 6A 00 : 50 : E8 ?? ?? ?? FF : 83 C4 0C : [5D] : C2"


TBOOL IfHL1LstSrvLib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200]);

void PatchHL1LstSrvLib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200]);

void HL1ClassicListenCryptMan(TSTREAMADDR_HDR const fileStream, TBOOL IsWeGonnaToCryptItBack);

void HL1ClassicListenGetBaseAddr(TSTREAMADDR_HDR const fileStream, uint64_t *HL1LstBase);


#endif
