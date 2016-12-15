#include "cod4.h"

TBOOL IfCOD4Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
  TPTRN Pat;
  TBOOL Res;
  TVAADDR pLoc,tmpLoc;
  uint64_t rems,Base;
  uint8_t tmpptrOff,tmpMCHCodeDistance;
  /*
  unsigned char *COD4VerDate,*COD4VerOS,*COD4VerVer,*COD4VerTxt;
  */
  unsigned char *COD4VerDate,*COD4VerOS,*COD4VerVer,*COD4VerTxt,*COD4VerBuild;
  uint32_t COD4VerBuild2;
  TSTREAMADDR_NOHDR tmpStr;
  strcpy(Version,"UNKNOWN");


  if (fileOS == VUP_Win32)
   {
      OPParseUserPtrn(COD4FullVerMagic_WiN,TU_UseBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);

      if (!Res)
        return TB_FALSE;

      OPPEGetAdditionalInfo(fileStream,&Base,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

      tmpptrOff = 1;
      tmpMCHCodeDistance = 5;
      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerDate,&rems);

      tmpptrOff += tmpMCHCodeDistance;

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerBuild,&rems);

      tmpptrOff += (tmpMCHCodeDistance << 2);

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerOS,&rems);

      tmpptrOff += (tmpMCHCodeDistance << 1);

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerVer,&rems);

      tmpptrOff += tmpMCHCodeDistance;

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerTxt,&rems);
      snprintf(Version,200,"%s %s build %s %s %s",COD4VerTxt,COD4VerVer,COD4VerBuild,COD4VerDate,COD4VerOS);
   }
  else
   {
      OPParseUserPtrn(COD4FullVerMagic_Unix_1,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);

      if (!Res)
        return TB_FALSE;

      OPELFGetAdditionalInfo(fileStream,&Base,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

      tmpptrOff = 14;
      tmpMCHCodeDistance = 8;

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerOS,&rems);

      tmpptrOff += tmpMCHCodeDistance + 4;

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerVer,&rems);

      tmpptrOff += tmpMCHCodeDistance;

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerTxt,&rems);


      OPParseUserPtrn(COD4FullVerMagic_Unix_2,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      tmpptrOff = 10;

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      tmpLoc = (*((uint32_t *) tmpStr)) - Base;
      OPJumpToOffsetInStream(fileStream,tmpLoc,&COD4VerDate,&rems);

      tmpptrOff += tmpMCHCodeDistance;

      OPJumpToOffsetInStream(fileStream,pLoc + tmpptrOff,&tmpStr,&rems);
      COD4VerBuild2 = *((uint32_t *) tmpStr);
      snprintf(Version,200,"%s %s build %d %s %s",COD4VerTxt,COD4VerVer,COD4VerBuild2,COD4VerDate,COD4VerOS);
   }

  return TB_TRUE;
}

void PatchCOD4Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
  TPTRN Pat;
  TBOOL Res;
  TSTREAMADDR_NOHDR tmpStr;
  TVAADDR PreAcceptAddr;
  uint64_t pLoc,rems;
  uint8_t i = 1,ii = 'a';
  *PatchingResult = TB_TRUE;
  printf("Performing job...\n\n");
  if (fileOS == VUP_Win32)
   {
     printf("\n%u) Looking for connection check main DENY junction ...\n",i);
     i ++;
     OPParseUserPtrn(COD4DenyJunction_Win,TU_UseHalfBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
       {
          printf("        Found at 0x%"PRIX64".   ",pLoc);
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\nOperation halted. Contact author to add independent support for this COD4 build...\n\n");
          return;
       }
     printf("\n%u) Detecting the pre-ACCEPT address ...\n",i);
     OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
     PreAcceptAddr = pLoc + 30 + *((uint32_t *)(tmpStr + 26));
     printf("        Located at 0x%"PRIX64".   ",PreAcceptAddr);
     i ++;

     printf("\n%u) Enforcing jump to pre-ACCEPT address from DENY junction ...\n",i);
     *((uint8_t *)(tmpStr + 24)) = 0x90;
     *((uint8_t *)(tmpStr + 25)) = 0xE9;
     printf("        Done ... \n\n\n");
     /*
     printf("\n%u) Patching 2 CD-Key duplication checks inside main DENY junction ...\n",i);
     *((uint32_t *)(tmpStr + 34)) = PreAcceptAddr - (pLoc + 38);
     *((uint32_t *)(tmpStr + 42)) = PreAcceptAddr - (pLoc + 46);
     printf("        Done ... \n\n\n");

     i ++;

     printf("\n%u) Patching additional 3 CD-Key validation checks ...\n",i);
     *((uint8_t *)(tmpStr + 69)) = PreAcceptAddr - (pLoc + 70);
     printf("        Done ... \n\n\n");
     */
     i ++;

     printf("\n%u) Looking for GUID validation check ...\n",i);
     i ++;
     OPParseUserPtrn(COD4GUIDComparision_Win,TU_UseHalfBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
       {
          printf("        Found at 0x%"PRIX64".   ",pLoc);
          OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
          printf("Patching ...  ");
          *(tmpStr + 2) = 0xEB;

          printf("Done\n\n\n");
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\nOperation halted. Contact author to add independent support for this COD4 build...\n\n");
          return;
       }
     printf("\n%u) Looking for DEMO block ...\n",i);
     pLoc += *(tmpStr + 3) + 4;
     printf("        Located at 0x%"PRIX64".   ",pLoc);
     OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
     i ++;

     printf("\n%u) Detecting main ACCEPT junction address ...\n",i);
     pLoc += *(tmpStr + 28) + 32;
     printf("        Located at 0x%"PRIX64".   ",pLoc);
     OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
     i ++;

     printf("\n\n%u) Scanning ACCEPT junction for AntiPirate checks ...\n",i);
     /*
     *(tmpStr + 22) = 0x89;
     *(tmpStr + 23) = 0xC0;

     *(tmpStr + 30) = 0x66;
     *(tmpStr + 31) = 0x8B;
     *(tmpStr + 32) = 0xC0;
     *(tmpStr + 33) = 0x66;
     *(tmpStr + 34) = 0x8B;
     *(tmpStr + 35) = 0xC0;
     */

     OPSetSearchMarkers(fileStream,pLoc,0,OP_MODIFY_START_OFFSET_MARKER);
     OPSetSearchWithMarkersState(fileStream,TB_TRUE);


     OPParseUserPtrn(COD4AcceptAllChecks_Win,TU_UseBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
       {
          printf("        Located at 0x%"PRIX64".   ",pLoc);
          printf("\n%u%c) Patching CD-Key validation check ...  ",i,ii);
          OPParseUserPtrn(COD4Accept_Dst2,TU_UseBytePatterns,&Pat);
          OPWritePtrn(fileStream,pLoc + 2,Pat);
          OPFreePatternMemory(&Pat);
          printf("Done\n");
          ii ++;
          printf("%u%c) Patching silent DROP code ...  ",i,ii);
          OPParseUserPtrn(COD4Accept_Dst,TU_UseBytePatterns,&Pat);
          OPWritePtrn(fileStream,pLoc + 10,Pat);
          OPFreePatternMemory(&Pat);
          printf("Done\n\n\n");
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n");
       }
     OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_START_OFFSET_MARKER);
     OPSetSearchWithMarkersState(fileStream,TB_FALSE);
   }
  else
   {
     printf("\n%u) Looking for connection check main DENY junction ...\n",i);
     i ++;
     OPParseUserPtrn(COD4DenyJunction_Unix,TU_UseHalfBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
       {
          printf("        Found at 0x%"PRIX64".   ",pLoc);
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\nOperation halted. Contact author to add independent support for this COD4 build...\n\n");
          return;
       }
     printf("\n%u) Detecting the GUID validation check address ...\n",i);
     OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
     PreAcceptAddr = pLoc + 30 + *((uint32_t *)(tmpStr + 26));
     printf("        Located at 0x%"PRIX64".   ",PreAcceptAddr);
     i ++;

     printf("\n%u) Enforcing jump to GUID validation check from DENY junction ...\n",i);
     *((uint8_t *)(tmpStr + 24)) = 0x90;
     *((uint8_t *)(tmpStr + 25)) = 0xE9;
     printf("        Done ... \n\n\n");
     i ++;

     pLoc = PreAcceptAddr;
     OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);

     printf("\n%u) Patching GUID validation check ...\n",i);
     *((uint8_t *)(tmpStr + 32)) = 0x90;
     *((uint8_t *)(tmpStr + 33)) = 0xE9;
     i ++;
     printf("        Done ... \n\n\n");

     printf("\n%u) Looking for DEMO block ...\n",i);
     pLoc += 38 + *((uint32_t *)(tmpStr + 34));
     printf("        Located at 0x%"PRIX64".   ",pLoc);
     i ++;

     printf("\n%u) Detecting main ACCEPT junction address ...\n",i);
     pLoc += 30;
     printf("        Located at 0x%"PRIX64".   ",pLoc);
     OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
     i ++;

     printf("\n\n%u) Scanning ACCEPT junction for AntiPirate checks ...\n",i);
     /*
     *(tmpStr + 24) = 0x66;
     *(tmpStr + 25) = 0x8B;
     *(tmpStr + 26) = 0xC0;
     *(tmpStr + 27) = 0x66;
     *(tmpStr + 28) = 0x8B;
     *(tmpStr + 29) = 0xC0;

     *(tmpStr + 40) = 0x66;
     *(tmpStr + 41) = 0x8B;
     *(tmpStr + 42) = 0xC0;
     *(tmpStr + 43) = 0x66;
     *(tmpStr + 44) = 0x8B;
     *(tmpStr + 45) = 0xC0;
     */
     OPSetSearchMarkers(fileStream,pLoc,0,OP_MODIFY_START_OFFSET_MARKER);
     OPSetSearchWithMarkersState(fileStream,TB_TRUE);

     printf("%u%c) Looking for CD-Key validation check ...\n",i,ii);
     OPParseUserPtrn(COD4AcceptCDKeyCheck_Lin,TU_UseBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
       {
          printf("        Found at 0x%"PRIX64".   ",pLoc);
          OPParseUserPtrn(COD4Accept_Dst,TU_UseBytePatterns,&Pat);
           printf("Patching ...  ");
           OPWritePtrn(fileStream,pLoc + 2,Pat);
           OPFreePatternMemory(&Pat);
           printf("Done\n");
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n");
       }
     ii ++;
     printf("%u%c) Looking for silent DROP code ...\n",i,ii);
     OPParseUserPtrn(COD4AcceptSilentDrop_Lin,TU_UseBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
       {
          printf("        Found at 0x%"PRIX64".   ",pLoc);
          OPParseUserPtrn(COD4Accept_Dst,TU_UseBytePatterns,&Pat);
           printf("Patching ...  ");
           OPWritePtrn(fileStream,pLoc + 2,Pat);
           OPFreePatternMemory(&Pat);
           printf("Done\n\n\n");
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n\n\n");
       }
     OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_START_OFFSET_MARKER);
     OPSetSearchWithMarkersState(fileStream,TB_FALSE);
   }
}
