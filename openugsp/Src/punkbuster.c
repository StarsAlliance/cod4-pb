#include "punkbuster.h"

TBOOL IfPunkBuster(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
  TPTRN Pat;
  TBOOL Res;
  TVAADDR pLoc;
  strcpy(Version,"UNKNOWN");
  OPParseUserPtrn(PunkBuster_Magic1,TU_UseBytePatterns,&Pat);
  OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
  OPFreePatternMemory(&Pat);
  if (!Res)
   {
     OPParseUserPtrn(PunkBuster_Magic2,TU_UseBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (!Res)
       return Res;
   }
  OPParseUserPtrn(PunkBuster_Magic3,TU_UseBytePatterns,&Pat);
  OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
  OPFreePatternMemory(&Pat);
  if (Res)
   strcpy(Version,"Punk Buster AntiCheat Server Library");
  return Res;
}

void PatchPunkBuster(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,char Version[120])
{

}
