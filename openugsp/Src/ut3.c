#include "ut3.h"


TBOOL IfUT3Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   OPParseUserPtrn(UT3_MAGIC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
     strcpy(Version,"                 Unreal Tournament 3");
   return Res;
}

void PatchUT3Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   uint8_t i = 1;
   *PatchingResult = TB_TRUE;
   printf("Performing job...\n\n");
   printf("\n%u) Looking for client CD-Key authentication check ...\n",i);
   i ++;
   OPParseUserPtrn(UT3_CDKEY_AUTH_SRC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPParseUserPtrn(UT3_CDKEY_AUTHS_DST,TU_UseBytePatterns,&Pat);
       if (fileOS == VUP_Win32)
        {
          OPWritePtrn(fileStream,pLoc + 27,Pat);
          OPWritePtrn(fileStream,pLoc + 49,Pat);
        }
       else
        {
          OPWritePtrn(fileStream,pLoc + 19,Pat);
          OPWritePtrn(fileStream,pLoc + 30,Pat);
          OPWritePtrn(fileStream,pLoc + 47,Pat);
          OPWritePtrn(fileStream,pLoc + 58,Pat);
        }
       OPFreePatternMemory(&Pat);
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }

   printf("\n%u) Looking for client CD-Key reauthentication while connection to server is\n   established check ...\n",i);
   i ++;
   OPParseUserPtrn(UT3_CDKEY_REAUTH_SRC,TU_UseHalfBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPParseUserPtrn(UT3_CDKEY_AUTHS_DST,TU_UseBytePatterns,&Pat);
       if (fileOS == VUP_Win32)
        {
          OPWritePtrn(fileStream,pLoc + 22,Pat);
        }
       else
        {
          OPWritePtrn(fileStream,pLoc + 17,Pat);
          OPWritePtrn(fileStream,pLoc + 28,Pat);
          OPWritePtrn(fileStream,pLoc + 42,Pat);
          OPWritePtrn(fileStream,pLoc + 53,Pat);
        }
       OPFreePatternMemory(&Pat);
       if (fileOS == VUP_Win32)
        {
          OPParseUserPtrn(UT3_CDKEY_AUTHS_DST2,TU_UseBytePatterns,&Pat);
          OPWritePtrn(fileStream,pLoc + 44,Pat);
          OPFreePatternMemory(&Pat);
        }
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }

   printf("\n%u) Looking for client CD-Key authentication failure check ...\n",i);
   i ++;
   OPParseUserPtrn(UT3_CDKEY_INVALIDAUTH_SRC,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPParseUserPtrn(UT3_CDKEY_INVALIDAUTH_DST,TU_UseBytePatterns,&Pat);
       if (fileOS == VUP_Win32)
        {
          OPWritePtrn(fileStream,pLoc + 23,Pat);
        }
       else
        {
          OPWritePtrn(fileStream,pLoc + 13,Pat);
        }
       OPFreePatternMemory(&Pat);
       if (fileOS == VUP_Linux)
        {
           OPParseUserPtrn(UT3_CDKEY_AUTHS_DST2,TU_UseBytePatterns,&Pat);
           OPWritePtrn(fileStream,pLoc + 26,Pat);
           OPFreePatternMemory(&Pat);
        }
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }
}
