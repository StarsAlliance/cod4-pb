
#include "ue25.h"

extern TBOOL eSTEAMATiON_Prep_Mode, eSTEAMATiON_Prep_DropCrackedSteam, eSTEAMATiON_Prep_AllowDuplicateSteamIDs,
             PatchMasterServerUpdateNotificationProtection, PatchClientConnectionLostForExtractedSteamProtection,LeavePENDINGIDInPlace;


TBOOL IfUE25Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   OPParseUserPtrn(UE25_MAGIC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
     strcpy(Version,"                 Unreal Engine 2.5");
   return Res;
}

void PatchUE25Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   uint8_t i = 1;
   *PatchingResult = TB_TRUE;
   printf("Performing job...\n\n");
   if (!eSTEAMATiON_Prep_Mode)
    {
       printf("This engine is only supported in eSTEAMATiON preparation mode.\nIf you wanna run the server without eSTEAMATiON consider using another patch, overwise re-run with -esteamation-prep switch\n");
       *PatchingResult = TB_FALSE;
       return;
    }
   printf("\nLooking for STEAM3 Deny handler check ...\n");
   i ++;
   OPParseUserPtrn(UE25_STEAM3DENY_AUTH_CHECK_SRC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,(fileOS == VUP_Win32) ? 1 : 2,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       if (fileOS == VUP_Win32)
        {
          OPParseUserPtrn(UE25_STEAM3DENY_AUTHS_DST2,TU_UseBytePatterns,&Pat);
          OPWritePtrn(fileStream,pLoc + 35,Pat);
        }
       else
        {
          OPParseUserPtrn(UE25_STEAM3DENY_AUTHS_DST,TU_UseBytePatterns,&Pat);
          OPWritePtrn(fileStream,pLoc + 33,Pat);
        }
       OPFreePatternMemory(&Pat);
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }
}
