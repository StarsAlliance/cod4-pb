#include "vlvgameui.h"

TBOOL IfGameUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res1,Res2,Res3;
   TPTRN Pat;
   OPParseUserPtrn(GAMEUI_MAGIC1,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res1);
   OPFreePatternMemory(&Pat);

   OPParseUserPtrn(GAMEUI_MAGIC2,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res2);
   OPFreePatternMemory(&Pat);

   OPParseUserPtrn(GAMEUI_MAGIC3,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res3);
   OPFreePatternMemory(&Pat);

   strcpy(Version,"                Valve GameUI Library");
   return (Res1 && Res2 && Res3);
}


void PatchGameUILib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat,Pat2;
   uint8_t i = 1;
   *PatchingResult = TB_TRUE;
   printf("Performing job...\n\n");
   printf("\n%u) Looking for initial sv_lan value enforce #1...\n",i);
   i ++;
   OPParseUserPtrn(GAMEUI_INITSVLANVAL_SRC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPParseUserPtrn(GAMEUI_INITSVLANVAL_DST,TU_UseBytePatterns,&Pat2);
       OPWritePtrn(fileStream,pLoc + 7,Pat2);
       OPFreePatternMemory(&Pat2);
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }
   printf("\n%u) Looking for initial sv_lan value enforce #2...\n",i);
   i ++;
   OPFindPtrn(fileStream,Pat,2,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPParseUserPtrn(GAMEUI_INITSVLANVAL_DST,TU_UseBytePatterns,&Pat);
       OPWritePtrn(fileStream,pLoc + 7,Pat);
       OPFreePatternMemory(&Pat);
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }
}
