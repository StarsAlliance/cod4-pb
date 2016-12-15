#include "vsource.h"

extern TBOOL eSTEAMATiON_Prep_Mode, eSTEAMATiON_Prep_DropCrackedSteam, eSTEAMATiON_Prep_AllowDuplicateSteamIDs,LeavePENDINGIDInPlace;

TBOOL IfSrcEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   int32_t build_nr;
   uint8_t proto;
   char eng_version[40];
   char eng_time[9];
   char eng_date[12];
   strcpy(Version,"UNKNOWN");
   if (!GetValveEngineInfo(fileStream,&build_nr,&proto,eng_version,eng_time,eng_date,fileOS))
     return TB_FALSE;
   if ((proto < 8) && (proto < 47) && (proto != 0))
    {
      snprintf(Version,200,"Valve engine(Source)\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
              proto,eng_version,eng_time,eng_date,build_nr);
      return TB_TRUE;
    }
   else
    return TB_FALSE;
}

void PatchSrcEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
   TBOOL AllowNonSteam = TB_TRUE, AllowCrackedSteam = TB_TRUE, AllowNonClassBInLANServer = TB_TRUE;
   uint8_t UseIDMode = LeavePENDINGIDInPlace; //0 = 666ID, 1 = PENDING_ID
   TPTRN Pat;
   TBOOL Res;
   TSTREAMADDR_NOHDR tmpStr;
   uint64_t pLoc,rems;
   uint32_t *CBCDRHelperJT = NULL;
   uint64_t CBCDRHelperSafeLabelAddr = 0;
   uint8_t i = 1;
   *PatchingResult = TB_TRUE;
   if (eSTEAMATiON_Prep_Mode)
    {
      AllowNonSteam = TB_FALSE;
      AllowCrackedSteam = !eSTEAMATiON_Prep_DropCrackedSteam;
      goto eSTEAMATiON_Prep_Advanced_Skip;
    }
   if (PromtUser)
    {
      char Answer;
      printf("\nVUP is running in ADVANCED mode!!\nA few questions will be asked during patching process.\n");
      printf("You will need to press special answer keys(case insensitive so SHIFT does not matter):\nY = YES           N = NO         D = USE DEFAULT\n");
      printf("\nDo you want to enable Non Steam players to join your server(sv_lan 0 mode)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          AllowNonSteam = TB_FALSE;
       }
      printf("\nDo you want to enable Cracked Steam players to join your server(sv_lan 0 mode)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          AllowCrackedSteam = TB_FALSE;
       }
      printf("\nDo you want to enable players from different subnets to join your server(sv_lan 1 mode)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          AllowNonClassBInLANServer = TB_FALSE;
       }
      /*
      printf("\nDo you want players to get ONLINE/PIRATE ID's even if server is in sv_lan 1 mode?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          UseLANIDInLANMode = TB_FALSE;
       }
      printf("\nDo you want to use code patching so Non Steam will get STEAM_0:0:0 instead of STEAM_666:88:666?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          UseVID = TB_FALSE;
          UseZID = TB_TRUE;
       }
      */
      printf("\nDo you want to leave STEAM_ID_PENDING intact(for compatability with sourcemod even if this is dangerous)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'y') || (Answer == 'Y'))
       {
          UseIDMode = 1;
       }
      printf("\nAll required information gatherned...\n");
    }

   eSTEAMATiON_Prep_Advanced_Skip:
   printf("Performing job...\n\n");
   if (AllowNonSteam && !eSTEAMATiON_Prep_Mode)
    {
      TBOOL IsAncient = TB_FALSE;
      printf("\n%u) Looking for Steam validation check ...\n",i);
      i ++;
      OPParseUserPtrn(VLVSSteamValidation_Src,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (Res)
       {
          if (fileOS == VUP_Win32)
           {
              OPParseUserPtrn(VLVSSteamValidation_Win_Dst,TU_UseBytePatterns,&Pat);
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if (tmpStr[18] == '\0')
               IsAncient = TB_TRUE;
           }
          else
           {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if ((*(tmpStr) == 0x50))  // We are working with AMD optimized build
                OPParseUserPtrn(VLVSSteamValidation_UNiX_AMD_Dst,TU_UseBytePatterns,&Pat);
              else
                OPParseUserPtrn(VLVSSteamValidation_UNiX_Dst,TU_UseBytePatterns,&Pat);
           }
          printf("        Found at 0x%"PRIX64".   ",pLoc);
          printf("Patching ...  ");
          OPWritePtrn(fileStream,pLoc + (IsAncient ? 5 : 0),Pat);
          OPFreePatternMemory(&Pat);
          printf("Done\n\n\n");
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n\n\n");
       }
    }
   if (AllowNonSteam || AllowCrackedSteam || eSTEAMATiON_Prep_Mode)
    {
      uint64_t PEBase;
      printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDenyHelper jump table address ...\n",i);
      i ++;
      OPParseUserPtrn(VLVSJMPTabHelper,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (Res)
       {
          if (fileOS == VUP_Win32)
           {

              TVAADDR CBCDRHelperJTAddr;
              CBCDRHelperJTAddr = pLoc + 22;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
              CBCDRHelperJTAddr = *((uint32_t *) tmpStr);
              OPPEGetAdditionalInfo(fileStream,&PEBase,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
              CBCDRHelperJTAddr -= PEBase;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
              CBCDRHelperJT = (uint32_t *) tmpStr;
              printf("        Found at 0x%"PRIX64".   ",CBCDRHelperJTAddr);
           }
          else
           {
              uint64_t CBCDRHelperJTAddr;
              uint32_t tmp;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if ((*(tmpStr) == 0x90)) // We are working with i486/i686 optimized build
                CBCDRHelperJTAddr = pLoc + 38;
              else
                CBCDRHelperJTAddr = pLoc + 20;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
              tmp = *((uint32_t *) tmpStr);
              CBCDRHelperJTAddr = tmp;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
              CBCDRHelperJT = (uint32_t *) tmpStr;
              printf("        Found at 0x%"PRIX64".   ",CBCDRHelperJTAddr);
           }
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\nOperation halted. Contact author to add independent support for this Source build...\n\n");
          return;
       }
      printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDenyHelper safe label address ...\n",i);
      i ++;
      if (Res)
       {
          if (fileOS == VUP_Win32)
           {
              uint32_t tmp;
              CBCDRHelperSafeLabelAddr = pLoc + 63;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
              tmp = *((uint32_t *) tmpStr);
              CBCDRHelperSafeLabelAddr += tmp + 4;
              printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
              CBCDRHelperSafeLabelAddr += PEBase;
           }
          else
           {
              CBCDRHelperSafeLabelAddr = pLoc + 64;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if ((*(tmpStr) == 0x8B))  // We are working with AMD optimized build
                CBCDRHelperSafeLabelAddr += 12;
              printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
           }
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n\n\n");
       }

      if (Res)
       {
          uint8_t j;
          char ii = 'a';
          j = 11;
          if (fileOS == VUP_Linux)
           j ++;
          printf("\n%u%c) Patching Steam VAC logon check ... ",i,ii);
          CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
          printf("Done\n\n\n");
          ii ++;

          ii = 'a';
          j = 10;
          if (fileOS == VUP_Linux)
          j ++;
          printf("\n%u%c) Patching Steam VAC Logon In-Game(With SemiSteam Clients) check ... ",i,ii);
          CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
          printf("Done\n\n\n");
          ii ++;


          if (AllowNonSteam || eSTEAMATiON_Prep_Mode)
           {
             j = 13;
             if (fileOS == VUP_Linux)
              j ++;
             printf("\n%u%c) Patching Steam UserID ticket verifying check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (AllowCrackedSteam)
           {
             j = 3;
             if (fileOS == VUP_Linux)
              j ++;
             printf("\n%u%c) Patching Steam account game ownership check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
           {
             j = 5;
             if (fileOS == VUP_Linux)
              j ++;
             printf("\n%u%c) Patching SteamID duplication check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
           }
          i ++;
       }
    }


   if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
    {
       TOPError tmpErr;
       printf("\n%u) Looking for CSteam3::CheckForDuplicateSteamID duplicate SteamID check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLVSCheckDuplicateSteamIDFunc,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Win32) ? TB_FALSE : TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if ((fileOS == VUP_Win32))
            {
              tmpStr[18] = 0x89;
              tmpStr[19] = 0xC0;
            }
           else
            {
              tmpStr[17] = 0xEB;
            }
           printf("Done\n\n\n");
        }
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }

   if ((!UseIDMode) && (AllowNonSteam || AllowCrackedSteam) && (!eSTEAMATiON_Prep_Mode))
    {
       printf("\n%u) Looking for STEAM_ID_PENDING hardcode ...\n",i);
       i ++;
       OPParseUserPtrn(VLVSteamIDFull,TU_UseBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64" (AS FULL STRING).   ",pLoc);
           OPParseUserPtrn(VLVSteamID666Full,TU_UseBytePatterns,&Pat);
           printf("Patching ...  ");
           OPWritePtrn(fileStream,pLoc,Pat);
           OPFreePatternMemory(&Pat);
           printf("Done\n\n\n");
        }
       else
        {
           TSTREAMADDR_NOHDR DW2,DW3,DW4;
           TVAADDR DW2_Loc,DW3_Loc,DW4_Loc;
           uint8_t safety_counter = 0;
           OPParseUserPtrn(VLVSteamIDDWORD_4,TU_UseBytePatterns,&Pat);
           OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Win32) ? TB_TRUE : TB_FALSE,0,&DW4_Loc,&Res);
           OPFreePatternMemory(&Pat);
           if (Res)
            {
              OPJumpToOffsetInStream(fileStream,DW4_Loc,&DW4,&rems);
              DW3 = DW4;
              DW3_Loc = DW4_Loc;
              do
               {
                 DW3 --;
                 if (!DW3_Loc)
                   goto safety_interruptedsrc;
                 DW3_Loc --;
                 safety_counter ++;
               }
              while (strncmp((char *)DW3,VLVSteamIDDWORD_3,4) && (safety_counter < 20));
              if (safety_counter == 20)
                goto safety_interruptedsrc;
              safety_counter = 0;
              DW2 = DW3;
              DW2_Loc = DW3_Loc;
              do
               {
                 DW2 --;
                 if (!DW2_Loc)
                   goto safety_interruptedsrc;
                 DW2_Loc --;
                 safety_counter ++;
               }
              while (strncmp((char *)DW2,VLVSteamIDDWORD_2,4) && (safety_counter < 20));
              if (safety_counter == 20)
                goto safety_interruptedsrc;

              printf("        Found at [0x%"PRIX64" + ",DW2_Loc);
              printf("0x%"PRIX64" + ",DW3_Loc);
              printf("0x%"PRIX64"] (DWORDS-SEPARATED).   \n",DW4_Loc);
              printf("        Patching ...  ");
              strncpy((char *)DW2,VLVSteamIDDWORD666_2,4);
              strncpy((char *)DW3,VLVSteamIDDWORD666_3,4);
              strncpy((char *)DW4,VLVSteamIDDWORD666_4,4);
              printf("Done\n\n\n");
            }
           else
            {
               safety_interruptedsrc:
               *PatchingResult = TB_FALSE;
               printf("        Not Found\n\n\n");
            }
        }
    }
   if (AllowNonClassBInLANServer)
    {
       TOPError tmpErr;
       printf("\n%u) Looking for client's network Class check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLVSLANChk_Src,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           if (fileOS == VUP_Win32)
            {
              OPParseUserPtrn(VLVSLANChk_WinDst,TU_UseBytePatterns,&Pat);
            }
           else
            {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if ((*(tmpStr) == 0x83))  // We are working with AMD optimized build
               OPParseUserPtrn(VLVSLANChk_UNiXDstAMD,TU_UseBytePatterns,&Pat);
              else
               OPParseUserPtrn(VLVSLANChk_UNiXDst,TU_UseBytePatterns,&Pat);
            }
           printf("Patching ...  ");
           OPWritePtrn(fileStream,pLoc,Pat);
           OPFreePatternMemory(&Pat);
           printf("Done\n\n\n");
        }
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }
}
