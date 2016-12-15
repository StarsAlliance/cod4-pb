#include "vgoldsrc.h"

extern TBOOL eSTEAMATiON_Prep_Mode, eSTEAMATiON_Prep_DropCrackedSteam, eSTEAMATiON_Prep_AllowDuplicateSteamIDs,LeavePENDINGIDInPlace;

TBOOL IfGoldSrcEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   int32_t build_nr;
   uint8_t proto;
   char eng_version[40];
   char eng_time[9];
   char eng_date[12];
   strcpy(Version,"UNKNOWN");
   if (!GetValveEngineInfo(fileStream,&build_nr,&proto,eng_version,eng_time,eng_date,fileOS))
     return TB_FALSE;
   if (proto == 48)
    {
      snprintf(Version,200,"Valve engine(SteamWorks GoldSRC Dedicated)\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
              proto,eng_version,eng_time,eng_date,build_nr);
      if (fileOS == VUP_Win32)
       {
          TVAADDR pLoc;
          TBOOL Res;
          TPTRN Pat;
          uint8_t type = 0;
          OPParseUserPtrn(HWRNDR_MAGIC,TU_UseBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           type = 1;
          else
           {
             OPParseUserPtrn(SWRNDR_MAGIC,TU_UseBytePatterns,&Pat);
             OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
             OPFreePatternMemory(&Pat);
             if (Res)
              type = 2;
           }
          switch (type)
           {
             case 1:
                     snprintf(Version,200,"Valve engine(SteamWorks GoldSRC Hardware Rendering Listen)\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
                              proto,eng_version,eng_time,eng_date,build_nr);
                     break;
             case 2:
                     snprintf(Version,200,"Valve engine(SteamWorks GoldSRC Software Rendering Listen/Dedicated)\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
                              proto,eng_version,eng_time,eng_date,build_nr);
                     break;
             default:
                     break;
           }
       }
      return TB_TRUE;
    }
   else
    return TB_FALSE;
}

void PatchGoldSrcEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
  TBOOL AllowNonSteam = TB_TRUE, AllowCrackedSteam = TB_TRUE, AllowNonClassBInLANServer = TB_TRUE, AddFixForClassicExploits = TB_TRUE , AddFixForVCrashExploit = TB_TRUE;
   uint8_t UseIDMode = LeavePENDINGIDInPlace; //0 = 666ID, 1 = PENDING_ID
   TPTRN Pat;
   TBOOL Res;
   TSTREAMADDR_NOHDR tmpStr;
   uint64_t pLoc,rems;
   uint32_t *CBCDRHelperJT = NULL, GOT[2];
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
      printf("\nDo you want to add security fix against classic exploits(FuF's Burn to be Pig and Luigys FakeClients CSDOS)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          AddFixForClassicExploits = TB_FALSE;
       }
      printf("\nDo you want to add security fix against new HLDS_VCRASH exploit?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          AddFixForVCrashExploit = TB_FALSE;
       }
      printf("\nAll required information gatherned...\n");
    }

   eSTEAMATiON_Prep_Advanced_Skip:
   printf("Performing job...\n\n");
   if (AllowNonSteam)
    {
      TBOOL LinuxAMD = TB_FALSE;
      uint64_t tmpLoc;
      printf("\n%u) Looking for Steam validation check ...\n",i);
      i ++;
      OPParseUserPtrn(VLVGoldSSteamValidation_Src,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (Res)
       {
          uint8_t pgsrc_StValidationPtc;
          OPParseUserPtrn(VLVGoldSSteamValidation_Dst,TU_UseBytePatterns,&Pat);
          OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
          if (fileOS == VUP_Linux)
           {
              pgsrc_StValidationPtc = 18;
              if (tmpStr[8] != 0x50)  // We are working with AMD optimized build
                LinuxAMD = TB_TRUE;
           }
          else
           {
              switch (tmpStr[0])
               {
                  case 0x51:
                             pgsrc_StValidationPtc = 12;
                             break;
                  case 0x52:
                             pgsrc_StValidationPtc = 22;
                             break;
               }
           }
          tmpLoc = pLoc + ((fileOS == VUP_Win32) ? pgsrc_StValidationPtc : pgsrc_StValidationPtc + (LinuxAMD ? 0 : 1));
          printf("        Found at 0x%"PRIX64".   ",pLoc);
          printf("Patching ...  ");
          OPWritePtrn(fileStream,tmpLoc,Pat);
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
      OPParseUserPtrn(VLVGoldSrcJMPTabHelper,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (Res)
       {
          if (fileOS == VUP_Win32)
           {

              TVAADDR CBCDRHelperJTAddr = pLoc;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              switch (tmpStr[1])
               {
                  case 0x44: // hw
                            CBCDRHelperJTAddr += 17;
                            break;
                  case 0x45: // sw
                            CBCDRHelperJTAddr += 16;
                            break;
               }
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
              uint64_t CBCDRHelperJTAddr = pLoc;
              uint32_t tmp;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              switch (tmpStr[7])
               {
                 case 0x54: // i686 and amd builds
                           CBCDRHelperJTAddr += 37;
                           break;
                 case 0x55: // i486 build
                           CBCDRHelperJTAddr += 33;
                           break;
               }
              OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
              tmp = *((uint32_t *) tmpStr);

              OPELFGetAdditionalInfo(fileStream,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,GOT);
              CBCDRHelperJTAddr = GOT[0] - (0xFFFFFFFF - tmp + 1); // The error was here as GOT[1] was incorrect and now replaced with GOT[0]
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
              CBCDRHelperSafeLabelAddr = pLoc;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              switch (tmpStr[1])
               {
                  case 0x44: // hw
                            CBCDRHelperSafeLabelAddr += 10;
                            break;
                  case 0x45: // sw
                            CBCDRHelperSafeLabelAddr += 9;
                            break;
               }
              OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
              tmp = *((uint32_t *) tmpStr);
              CBCDRHelperSafeLabelAddr += tmp + 4;

              OPParseUserPtrn(VLVGoldSrcSafeLabelValidate,TU_UseHalfBytePatterns,&Pat);
              OPFindPtrn(fileStream,Pat,1,TLOOKUPSTATICOFFSET,TB_TRUE,CBCDRHelperSafeLabelAddr,&pLoc,&Res);
              OPFreePatternMemory(&Pat);
              if (Res)
               {
                 OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
                 if (tmpStr[1] == 0x44)
                   CBCDRHelperSafeLabelAddr += 20;
                 else
                   if ((tmpStr[1] & 0x0F) == 5)
                     CBCDRHelperSafeLabelAddr += 19;
                 printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
                 CBCDRHelperSafeLabelAddr += PEBase;
               }
              else
               {
                 *PatchingResult = TB_FALSE;
                 printf("        Not Found\n\n\n");
               }
           }
          else
           {
              uint32_t tmp;
              CBCDRHelperSafeLabelAddr = pLoc;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              switch (tmpStr[7])
               {
                 case 0x54: // i686 and amd builds
                           CBCDRHelperSafeLabelAddr += 28;
                           break;
                 case 0x55: // i486 build
                           CBCDRHelperSafeLabelAddr += 24;
                           break;
               }
              OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
              tmp = *((uint32_t *) tmpStr);
              CBCDRHelperSafeLabelAddr += tmp + 4;
              OPParseUserPtrn(VLVGoldSrcSafeLabelValidate,TU_UseHalfBytePatterns,&Pat);
              OPFindPtrn(fileStream,Pat,1,TLOOKUPSTATICOFFSET,TB_FALSE,CBCDRHelperSafeLabelAddr,&pLoc,&Res);
              OPFreePatternMemory(&Pat);
              if (Res)
               {
                 OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
                 switch (tmpStr[18])
                  {
                    case 0x83: // i686 and amd builds
                              CBCDRHelperSafeLabelAddr += 21;
                              break;
                    case 0x8D: // i486 build
                              CBCDRHelperSafeLabelAddr += 18;
                              break;
                  }
                 printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
                 CBCDRHelperSafeLabelAddr = GOT[0] - CBCDRHelperSafeLabelAddr;
               }
              else
               {
                 *PatchingResult = TB_FALSE;
                 printf("        Not Found\n\n\n");
               }
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
          //if (fileOS == VUP_Linux)
           //j ++;
          printf("\n%u%c) Patching Steam VAC logon check ... ",i,ii);
          CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
          printf("Done\n\n\n");
          ii ++;
          if (AllowNonSteam && !eSTEAMATiON_Prep_Mode)
           {
             j = 13;
             //if (fileOS == VUP_Linux)
              //j ++;
             printf("\n%u%c) Patching Steam UserID ticket verifying check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (AllowCrackedSteam)
           {
             j = 3;
             //if (fileOS == VUP_Linux)
              //j ++;
             printf("\n%u%c) Patching Steam account game ownership check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (AllowCrackedSteam || AllowNonSteam)
           {
             j = 10;
             //if (fileOS == VUP_Linux)
              //j ++;
             printf("\n%u%c) Patching \"Steam connection lost\" check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
           {
             j = 5;
             //if (fileOS == VUP_Linux)
              //j ++;
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
       printf("\n%u) Looking for SV_CheckForDuplicateSteamID duplicate SteamID check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLVGoldSrcCheckDuplicateSteamIDFunc,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Win32) ? TB_FALSE : TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if ((fileOS == VUP_Win32))
            {
              tmpStr[24] = 0x89;
              tmpStr[25] = 0xC0;
            }
           else
            {
              switch (tmpStr[1])
               {
                  case 0x45: // i486 linux build
                        tmpStr[30] = 0x89;
                        tmpStr[31] = 0xC0;
                        break;
                  case 0x44: // i686 and amd linux build
                              switch (tmpStr[8])
                               {
                                 case 0x31: // i686
                                       tmpStr[25] = 0x89;
                                       tmpStr[26] = 0xC0;
                                       break;
                                 case 0xBE: //amd
                                       tmpStr[26] = 0x89;
                                       tmpStr[27] = 0xC0;
                                       break;
                               }
                        break;
               }
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
       uint64_t tmpLoc;
       printf("\n%u) Looking for client's network Class check ...\n",i);
       i ++;
       OPParseUserPtrn(VLVGoldSrcLANChk_Src,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           tmpLoc = pLoc;
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPParseUserPtrn(VLVGoldSrcLANChk_Dst,TU_UseBytePatterns,&Pat);
           if (fileOS == VUP_Win32)
            {
              tmpLoc += 12;
            }
           else
            {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              switch (tmpStr[2])
               {
                  case 0x83: // i686 and amd libs
                             tmpLoc += 67;
                             break;

                  case 0x8B: // i486 lib
                             tmpLoc += 73;
                             break;
               }
               //OPParseUserPtrn(VLVSLANChk_UNiXDstAMD,TU_UseBytePatterns,&Pat);
               //OPParseUserPtrn(VLVSLANChk_UNiXDst,TU_UseBytePatterns,&Pat);
            }
           printf("Patching ...  ");
           OPWritePtrn(fileStream,tmpLoc,Pat);
           OPFreePatternMemory(&Pat);
           printf("Done\n\n\n");
        }
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }
   if (AddFixForClassicExploits)
    {
       printf("\n%u) Looking for code exploitable to classic vulnerabilities ...\n",i);
       i ++;
       OPParseUserPtrn(VLVGoldSrcExploitableCode_Src,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           printf("Patching ...  ");
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           if (fileOS == VUP_Win32)
            {
               tmpStr[3] = 0xEB;
            }
           else
            {
              uint8_t i = 3;
              switch (tmpStr[1])
               {
                  case 0x7D: //i486
                             i ++;
                  case 0x8D: //i686 and amd
                             tmpStr[i] = 0x90;
                             tmpStr[i + 1] = 0xE9;
                             break;
               }
            }
           printf("Done\n\n\n");
        }
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }
   if (AddFixForVCrashExploit)
    {
       printf("\n%u) Looking for code exploitable to SV_ParseVoiceData vulnerability ...\n",i);
       char *gsrceng_Ver = strstr(Version,"Exe build:");
       uint16_t gsrceng_build;
       gsrceng_Ver = strchr(gsrceng_Ver,'(');
       sscanf(gsrceng_Ver,"(%hu)",&gsrceng_build);
       if (gsrceng_build >= 4139)
        {
           printf("        Build %u is SAFE(FIXED by Valve since build 4139)\n\n\n",gsrceng_build);
           goto VUP_VCRASH_ALREADY_RESOLVED_VALVE_LBL;
        }
       OPParseUserPtrn(VLVGoldSrcSVParseVoiceDataExploitableCode_Src,TU_UseBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           uint64_t SV_ParseChunckLoc = pLoc,pLogPrintfFunc = 0;
           printf("        Found at 0x%"PRIX64".   ",pLoc);

           char ii = 'a';
           printf("\n%u%c) Looking up for Log_Printf function ... \n",i,ii);
           OPParseUserPtrn(VLVGoldSrcLog_PrintfFunc,TU_UseBytePatterns,&Pat);
           OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
           OPFreePatternMemory(&Pat);
           if (pLoc)
            {
               OPSetSearchMarkers(fileStream,0,pLoc,OP_MODIFY_END_OFFSET_MARKER);
               OPSetSearchWithMarkersState(fileStream,TB_TRUE);
               OPParseUserPtrn(MachCodeFuncStart,TU_UseBytePatterns,&Pat);
               OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
               OPFreePatternMemory(&Pat);
               OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_END_OFFSET_MARKER);
               OPSetSearchWithMarkersState(fileStream,TB_FALSE);
               if (Res)
                {
                   printf("        Found at 0x%"PRIX64". MODE: LOGFIX\n",pLoc);
                   pLogPrintfFunc = pLoc;
                }
               else
                printf("        Not Found. MODE: SILENTFIX\n");
            }
           else
            {
               printf("        Not Found. MODE: SILENTFIX\n");
            }

           if (pLogPrintfFunc)
            {
              ii ++;
              printf("\n%u%c) Looking up for vulnerability error string ... \n",i,ii);
              OPParseUserPtrn(VLVGoldSrcSVParseVoiceDataErrorString_Src,TU_UseBytePatterns,&Pat);
              OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
              OPFreePatternMemory(&Pat);
              if (Res)
               {
                   printf("        Found at 0x%"PRIX64".   ",pLoc);
                   printf("Updating ...  ");
                   OPParseUserPtrn(VLVGoldSrcSVParseVoiceDataErrorString_Dst,TU_UseBytePatterns,&Pat);
                   OPWritePtrn(fileStream,pLoc,Pat);
                   OPFreePatternMemory(&Pat);
                   printf("Done\n");
               }
              else
               printf("        Not Found(Default will be logged)\n");

            }

           ii ++;
           printf("\n%u%c) Securing vulnerable code chunk  ... ",i,ii);
           OPJumpToOffsetInStream(fileStream,SV_ParseChunckLoc,&tmpStr,&rems);
           uint32_t uHostErrorCallRelOff = 54;
           if (fileOS == VUP_Linux)
             uHostErrorCallRelOff ++;
           if (pLogPrintfFunc)
            {
                int32_t dRelativeJmpToLogFunc;
                OPJumpToOffsetInStream(fileStream,SV_ParseChunckLoc + uHostErrorCallRelOff + 1,&tmpStr,&rems);
                dRelativeJmpToLogFunc = pLogPrintfFunc - (SV_ParseChunckLoc + uHostErrorCallRelOff + 5);
                ((int32_t *)tmpStr)[0] = dRelativeJmpToLogFunc;
                printf("Done(LOGFIX)\n\n\n");
            }
           else
            {
                OPParseUserPtrn(VLVGoldSrcEmptyCallStatement_Dst,TU_UseBytePatterns,&Pat);
                OPWritePtrn(fileStream,SV_ParseChunckLoc + uHostErrorCallRelOff,Pat);
                OPFreePatternMemory(&Pat);
                printf("Done(SILENTFIX)\n\n\n");
            }

        }
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }
  VUP_VCRASH_ALREADY_RESOLVED_VALVE_LBL:
   return;
}
