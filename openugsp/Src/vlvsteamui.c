#include "vlvsteamui.h"

TBOOL IfSteamUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   OPParseUserPtrn(STEAMUI_MAGIC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   strcpy(Version,"               Valve SteamUI Library");
   return Res;
}


void PatchSteamUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
   TSTREAMADDR_NOHDR tmpStr;
   uint64_t pLoc,rems;
   TBOOL Res;
   TPTRN Pat;
   uint8_t i = 1;
   *PatchingResult = TB_TRUE;
   printf("Performing job...\n\n");
   printf("\n%u) Looking for keyserver contact check ...\n",i);
   i ++;
   OPParseUserPtrn(STEAMUI_KEYSERVERCHECK_SRC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPParseUserPtrn(STEAMUI_KEYSERVERCHECK_DST,TU_UseBytePatterns,&Pat);
       OPWritePtrn(fileStream,pLoc + 27,Pat);
       OPFreePatternMemory(&Pat);
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }
   printf("\n%u) Looking for internal game update check ...\n",i);
   i ++;
   OPParseUserPtrn(STEAMUI_GAMEUPDATECHECK_SRC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
       if (tmpStr[29] == 0x0F)
         OPParseUserPtrn(STEAMUI_GAMEUPDATECHECK_DST1,TU_UseBytePatterns,&Pat);
       else
         OPParseUserPtrn(STEAMUI_GAMEUPDATECHECK_DST2,TU_UseBytePatterns,&Pat);
       OPWritePtrn(fileStream,pLoc + 29,Pat);
       OPFreePatternMemory(&Pat);
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }
}
