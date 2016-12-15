#include "vsource2007.h"

extern TBOOL eSTEAMATiON_Prep_Mode, eSTEAMATiON_Prep_DropCrackedSteam, eSTEAMATiON_Prep_AllowDuplicateSteamIDs,
             PatchMasterServerUpdateNotificationProtection, PatchClientConnectionLostForExtractedSteamProtection,LeavePENDINGIDInPlace;

TBOOL IfSrc2K7Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   int32_t build_nr;
   uint8_t proto;
   char eng_version[40];
   char eng_time[9];
   char eng_date[12];
   strcpy(Version,"UNKNOWN");
   if (!GetValveEngineInfo(fileStream,&build_nr,&proto,eng_version,eng_time,eng_date,fileOS))
     return TB_FALSE;
   if ((proto > 7) && (proto < 36))
    {
      snprintf(Version,200,"Valve engine(Source 2007)\nProtocol version %"PRIu8"\nExe version %s (ModName)\nExe build: %s %s (%"PRIi32")\n",
              proto,eng_version,eng_time,eng_date,build_nr);
      return TB_TRUE;
    }
   else
    return TB_FALSE;
}

void PatchSrc2K7Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
   TBOOL AllowNonSteam = TB_TRUE, AllowCrackedSteam = TB_TRUE, AllowNonClassBInLANServer = TB_TRUE,
         /*UseLANIDInLANMode = TB_TRUE,*/ ClientOptions = TB_FALSE, RemoveMasterWarning = TB_FALSE, RemoveClientConnLost = TB_FALSE, NewSrc2K7Builds1 = TB_FALSE;
   uint8_t UseIDMode = LeavePENDINGIDInPlace; //0 = 666ID, 1 = PENDING_ID
   TPTRN Pat;
   TBOOL Res;
   TSTREAMADDR_NOHDR tmpStr;
   uint64_t pLoc,rems;
   uint32_t *CBCDRHelperJT = NULL,GOT[2];
   uint64_t CBCDRHelperSafeLabelAddr = 0,SteamValidationProtectionAddressLoc = 0;
   int32_t Build;
   uint8_t i = 1;
   char *tmp_ver;
   TVAADDR SteamValidationLoc = 0;
   *PatchingResult = TB_TRUE;
   tmp_ver = Version + strlen(Version);
   while (*(tmp_ver - 1) != '(')
    tmp_ver --;
   sscanf(tmp_ver,"%"SCNi32")",&Build);

   NewSrc2K7Builds1 = ((Build >= 3412) ? TB_TRUE : TB_FALSE);

   if (eSTEAMATiON_Prep_Mode)
    {
      AllowNonSteam = TB_FALSE;
      AllowCrackedSteam = !eSTEAMATiON_Prep_DropCrackedSteam;
      goto eSTEAMATiON_Prep_Advanced_Skip;
    }
   if (PatchMasterServerUpdateNotificationProtection)
     RemoveMasterWarning = TB_TRUE;
   if (PatchClientConnectionLostForExtractedSteamProtection)
     RemoveClientConnLost = TB_TRUE;
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
      if (fileOS == VUP_Win32)
       {
         printf("\nAre you patching a client/listen server and want to remove latest Valve client protections?  ");
         do
          Answer = getchar();
         while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
         if ((Answer == 'y') || (Answer == 'Y'))
          {
            ClientOptions = TB_TRUE;
          }
       }
      if (NewSrc2K7Builds1)
       {
         printf("\nDo you want to remove new \"Client timed out\" which prevents extracted clients with Steam alongside to connect ?  ");
         do
          Answer = getchar();
         while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
         if ((Answer == 'y') || (Answer == 'Y'))
          {
             RemoveClientConnLost = TB_TRUE;
          }
       }
      printf("\nDid you patched players limit on your server and wanna get rid of annoying MasterRequestRestart message ?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'y') || (Answer == 'Y'))
       {
          RemoveMasterWarning = TB_TRUE;
       }
      printf("\nAll required information gatherned...\n");
    }
   eSTEAMATiON_Prep_Advanced_Skip:
   printf("Performing job...\n\n");

   if (AllowNonSteam || AllowNonClassBInLANServer || eSTEAMATiON_Prep_Mode)
    {
      TVAADDR tmpLast;
      if (!eSTEAMATiON_Prep_Mode)
       {
         TBOOL NewBuild_11032008 = TB_FALSE;
         char ii = 'a';
         printf("\n%u) Looking for Steam validation check ...\n",i);
         OPParseUserPtrn(VLV2K7SteamValidation_Src,TU_UseHalfBytePatterns,&Pat);
         if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
          {
            OPGetLastOffsetInStream(fileStream,&tmpLast);
            OPSetSearchMarkers(fileStream,0,tmpLast / 3,OP_MODIFY_END_OFFSET_MARKER);
            OPSetSearchWithMarkersState(fileStream,TB_TRUE);
          }
         OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Linux) ? TB_TRUE : TB_FALSE,0,&pLoc,&Res);
         OPFreePatternMemory(&Pat);
         if (Res)
          {
             if (fileOS == VUP_Win32)
              {
                 OPParseUserPtrn(VLV2K7SteamValidation_Win_Dst,TU_UseBytePatterns,&Pat);
              }
             else
              {
                 OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                 if ((tmpStr[3] == 0x89) && tmpStr[0] == 0x89)
                   NewBuild_11032008 = TB_TRUE;
                 OPParseUserPtrn(VLV2K7SteamValidation_UNiX_Dst,TU_UseBytePatterns,&Pat);
              }
             printf("        Found at 0x%"PRIX64".   ",pLoc);
             printf("Patching ...  ");
             if (!AllowNonSteam/*eSTEAMATiON_Prep_Mode*/)
              {
                 OPFreePatternMemory(&Pat);
                 printf("Not needed in eSTEAMATiON Preparation mode\n\n\n");
              }
             else
              {
                OPWritePtrn(fileStream,pLoc - (NewBuild_11032008 ? 1 : 0),Pat);
                OPFreePatternMemory(&Pat);
                printf("Done\n\n\n");
              }
            SteamValidationLoc = pLoc;

            printf("\n%u%c) Looking for and storing validation code location ...\n",i,ii);
            SteamValidationProtectionAddressLoc = (pLoc - (NewBuild_11032008 ? 1 : 0)) + ((fileOS == VUP_Win32) ? 20 : 21);

            OPParseUserPtrn(VLV2K7SteamValidationProtectionCodeConsistencyCheck,TU_UseHalfBytePatterns,&Pat);
            OPFindPtrn(fileStream,Pat,1,TLOOKUPSTATICOFFSET,TB_FALSE,SteamValidationProtectionAddressLoc,&pLoc,&Res);
            OPFreePatternMemory(&Pat);
            if (Res)
             {
                SteamValidationProtectionAddressLoc += 18;
                printf("        Found at 0x%"PRIX64"\n\n",SteamValidationProtectionAddressLoc);
             }
            else
             {
                *PatchingResult = TB_FALSE;
                printf("        Not Found due to CC failure\n\n\n");
                //printf("        DEVELOPERDBG: Check at 0x%"PRIX64"\n\n",SteamValidationProtectionAddressLoc);
             }
          }
         else
          {
             *PatchingResult = TB_FALSE;
             printf("        Not Found\n\n\n");
          }
         i ++;
       }
      if ((Build >= 3362) && !eSTEAMATiON_Prep_Mode)
       {
         char ii = 'a';
         //printf("\n\nPOST 25 JAN 2008 ENGINE IS LOADED... PATCHING CERTIFICATES RELATED CHECKS... \n\n");
         printf("\n\nSOURCE 2007 ENGINE NEWER THAN 3362 IS LOADED... REMOVING STEAM CERTIFICATE'S RELATED CHECK... \n\n");
         printf("\n%u) Looking for Steam certificate's length check ...\n",i);
         OPParseUserPtrn(VLV2K7SteamCertificateChecking_Src,TU_UseHalfBytePatterns,&Pat);
         OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Linux) ? TB_TRUE : TB_FALSE,0,&pLoc,&Res);
         OPFreePatternMemory(&Pat);
         if (Res)
          {
             uint64_t pLoc3 = pLoc;
             printf("        Found at 0x%"PRIX64".   ",pLoc);
             if (SteamValidationLoc || AllowNonClassBInLANServer)
              {
                 uint32_t PatchData;
                 uint8_t tmpCmds[8],tmpCmdsLinux[4] = {0,0,0,0}, uRegisterType = 0;
                 TSTREAMADDR_NOHDR tmpStr2;
                 TVAADDR NoCertLoc;
                 TVAADDR Location,PreValidationPrepLoc,SteamCertFailedJMP;
                 if (fileOS == VUP_Win32)
                  {
                    Location = pLoc + 17; // Location of offset to jump to "Steam certificate length error (x/2028) code"
                    PatchData = SteamValidationLoc - pLoc - 7; // The new offset to write to go to "Steam validation rejected" instead.
                    OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                    if (tmpStr[2] == 0)
                      uRegisterType = 1; // We have EDI register insted of EBP
                    SteamCertFailedJMP = Location - 2; // Start of machine code with our jmp.
                    NoCertLoc = *((uint32_t *)tmpStr) + Location + 4; // Will point to "Steam cert error" machine code
                  }
                 else
                  {
                    OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                    if (tmpStr[0] == 0xE8)
                     {
                       Location = pLoc + 22; // For Linux Location will point to machine code ,there spec register compared against 0x7FF
                       PatchData = SteamValidationLoc - pLoc - 12; // Offset for Steam validation rejected machine code location.
                       OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                       *tmpStr = 0xE9;
                       tmpStr ++;
                       SteamCertFailedJMP = Location - 1;
                       NoCertLoc = *((uint32_t *)tmpStr) + Location + 4;
                       uRegisterType = 1; // Register for this linux Build is ECX :)
                     }
                    else
                     {
                       /*
                       memcpy(tmpCmdsLinux,tmpStr + 14,4);
                       Location = pLoc + 29;
                       PatchData = SteamValidationLoc - pLoc - 18;
                       OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                       SteamCertFailedJMP = Location - 2;
                       NoCertLoc = *((uint32_t *)tmpStr) + Location + 4;
                       */

                       /*
                         Copy the mov reg,stack command with respective register which has the length of cert value as it has been changed in the code on Linux
                       */
                       memcpy(tmpCmdsLinux,tmpStr,4);
                       Location = pLoc + 15;
                       PatchData = SteamValidationLoc - (pLoc + 4);
                       OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                       SteamCertFailedJMP = Location - 2;
                       NoCertLoc = *((uint32_t *)tmpStr) + Location + 4;
                       uRegisterType = 0; // Register for this Linux build is EDX :)
                     }

                  }
                 printf("\n%u%c) Governing NULL certificate client drop location ...\n        Found at 0x%"PRIX64"\n",i,ii,NoCertLoc);
                 OPParseUserPtrn(VLV2K7SteamCertificateCheckingPreSteamValidationJMP,TU_UseHalfBytePatterns,&Pat);
                 /*
                    The call to OPFindPtrn here should be specially designed for each supported engine.
                    The resulting address pointed by SteamValidationLoc - PatternDependentSpecialOffset should point to machine code
                    there passing of first 2 arguments via stack for CSteam3Server::NotifyClientConnect function occurs.
                 */
                 OPFindPtrn(fileStream,Pat,1,TLOOKUPSTATICOFFSET,TB_FALSE,SteamValidationLoc - ((fileOS == VUP_Win32) ? 17 : 32 ),&PreValidationPrepLoc,&Res);
                 OPFreePatternMemory(&Pat);
                 //printf("DEVELOPERDBG: Check at: 0x%lX\n\n",SteamValidationLoc - ((fileOS == VUP_Win32) ? 17 : 32 ));
                 ii ++;
                 printf("\n%u%c) Reading stack-parameters passed to Steam validation routine ...\n",i,ii);
                 if (Res)
                  {
                     printf("        Found at 0x%"PRIX64"\n",PreValidationPrepLoc);
                     OPJumpToOffsetInStream(fileStream,PreValidationPrepLoc,&tmpStr2,&rems);
                     ii++ ;
                     //printf("        Patching ...  ");
                     /*
                        Copy 2 stack parameters to tmpCmds - Used on both Windows and Linux
                     */
                     memcpy(tmpCmds,tmpStr2,8);
                     if (fileOS == VUP_Win32)
                      {
                        printf("\n%u%c) Adding NonSteam support junctions to code ...\n",i,ii);
                        tmpStr2[0] = 0xE9;
                        *((int32_t *)(tmpStr2 + 1)) = NoCertLoc - (PreValidationPrepLoc + 5);
                        tmpStr2[5] = 0x66;
                        tmpStr2[6] = 0x89;
                        tmpStr2[7] = 0xC0;
                      }
                     else
                      {
                        ii ++;
                        printf("\n%u%c) Checking for certificate length register ...\n",i,ii);
                        if (tmpCmdsLinux[0])
                         {
                           printf("        Found\n");
                           ii ++;
                           printf("\n%u%c) Adding NonSteam support junctions to code ...\n",i,ii);
                           memcpy(tmpStr2,tmpCmdsLinux,4);
                           tmpStr2[4] = 0xEB;
                           *((int8_t *)(tmpStr2 + 5)) = NoCertLoc - (PreValidationPrepLoc + 6);
                           tmpStr2[6] = 0x89;
                           tmpStr2[7] = 0xC0;
                         }
                        else
                         {
                           printf("        Not Found. Skipping ...\n\n\n");
                           goto VUP_SRC2K7_CERT_PTCH_FINITA;
                         }
                      }
                     OPParseUserPtrn(VLV2K7SteamCertificateCheckingPreSteamValidationJMP2_Dst,TU_UseBytePatterns,&Pat);
                     OPWritePtrn(fileStream,NoCertLoc,Pat);
                     OPFreePatternMemory(&Pat);
                     if (fileOS == VUP_Win32)
                      {
                         OPParseUserPtrn(VLV2K7SteamCertificateChecking_Dst3,TU_UseBytePatterns,&Pat);
                         OPWritePtrn(fileStream,NoCertLoc + 14,Pat);
                         OPFreePatternMemory(&Pat);
                      }
                     OPJumpToOffsetInStream(fileStream,NoCertLoc,&tmpStr2,&rems);
                     memcpy(tmpStr2 + 4,tmpCmds,8);
                     if (fileOS == VUP_Linux)
                      {
                        if (uRegisterType)
                         tmpStr2[1] = 0xC9; // ECX
                        else
                         tmpStr2[1] = 0xD2; // EDX
                      }
                     else
                      {
                         // New code - With new pattern for Windows new register has also been introduced.
                         if (uRegisterType)
                           tmpStr2[1] = 0xFF; // EDI
                        // Otherwise default EBP will be used
                      }
                     tmpStr2[3] = (SteamValidationLoc + 12) - (NoCertLoc + 4);
                     tmpStr2[13] = (PreValidationPrepLoc + 8)  - (NoCertLoc + 14);
                     printf("        Done\n");
                     ii ++;
                     printf("\n%u%c) Patching main Steam certificate check(SECURELY) ...\n",i,ii);
                     //OPParseUserPtrn(VLV2K7SteamCertificateChecking_Dst1,TU_UseBytePatterns,&Pat);
                     OPJumpToOffsetInStream(fileStream,pLoc3,&tmpStr2,&rems);
                     OPParseUserPtrn(((fileOS == VUP_Win32) ? ((tmpStr2[3] == 0x8B) ? VLV2K7SteamCertificateChecking_Win2_Dst : VLV2K7SteamCertificateChecking_Win_Dst) : VLV2K7SteamCertificateChecking_UNiX_Dst1),TU_UseBytePatterns,&Pat);
                     //printf("\nDEBUG: 0x%"PRIX64"\n\n",SteamCertFailedJMP);
                     //OPWritePtrn(fileStream,SteamCertFailedJMP,Pat);
                     if (fileOS == VUP_Win32)
                       OPWritePtrn(fileStream,pLoc3 + ((tmpStr2[3] == 0x8B) ? 6 : 0),Pat);
                     else
                       OPWritePtrn(fileStream,pLoc3 - 14,Pat);
                     OPFreePatternMemory(&Pat);
                     *((uint32_t *)(tmpStr2 + ((fileOS == VUP_Win32) ? 17 : /*29*/15))) = SteamValidationProtectionAddressLoc - pLoc3 - ((fileOS == VUP_Win32) ? 21 : /*33*/ 19);
                     //*((uint32_t *)(tmpStr - 1)) = PatchData;
                     printf("        Done\n\n\n");
                  }
                 else
                  {
                    printf("        Not Found. Skipping ...\n\n\n");
                  }
              }
             else
              {
                 printf("Skipping ...\n\n\n");
              }
          }
         else
          {
             *PatchingResult = TB_FALSE;
             printf("        Not Found\n\n\n");
          }
         VUP_SRC2K7_CERT_PTCH_FINITA:
         i ++;
         //printf("\n\nCERTIFICATES RELATED CHECKS PATCHING COMPLETE.\n\n\n");
         printf("\n\nSTEAM CERTIFICATE'S RELATED CHECK PATCHING COMPLETE.\n\n\n");
       }
      OPSetSearchWithMarkersState(fileStream,TB_FALSE);
    }
   if (AllowNonSteam || AllowCrackedSteam || eSTEAMATiON_Prep_Mode || RemoveClientConnLost)
    {
      uint64_t PEBase;
      TVAADDR tmpLast;
      TBOOL NewBuild_11032008 = TB_FALSE;
      printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDenyHelper jump table address ...\n",i);
      i ++;
      OPParseUserPtrn(VLV2K7JMPTabHelper,TU_UseHalfBytePatterns,&Pat);
      if (fileOS == VUP_Win32)
       {
         OPGetLastOffsetInStream(fileStream,&tmpLast);
         OPSetSearchMarkers(fileStream,0,tmpLast >> 1,OP_MODIFY_END_OFFSET_MARKER);
         OPSetSearchWithMarkersState(fileStream,TB_TRUE);
       }
      else
       {
         if (NewSrc2K7Builds1)
          {
            OPGetLastOffsetInStream(fileStream,&tmpLast);
            OPSetSearchMarkers(fileStream,0,((tmpLast >> 2) + 0x30000),OP_MODIFY_END_OFFSET_MARKER);
            OPSetSearchWithMarkersState(fileStream,TB_TRUE);
          }
       }
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPSetSearchWithMarkersState(fileStream,TB_FALSE);
      OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_END_OFFSET_MARKER);
      OPFreePatternMemory(&Pat);

      if (Res)
       {
          if (fileOS == VUP_Win32)
           {
              TVAADDR CBCDRHelperJTAddr;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              CBCDRHelperJTAddr = pLoc + 27 + ((tmpStr[18] == 0x83) ? 3 : 0);
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
              //CBCDRHelperJTAddr = pLoc + 61;
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if (tmpStr[7] == 0xE8)
                NewBuild_11032008 = TB_TRUE;
              if (NewBuild_11032008 == TB_TRUE)
               {
                  if (tmpStr[25] == 0x76)
                   {
                      CBCDRHelperJTAddr = pLoc + 30 + tmpStr[26];
                      OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
                      tmp = *((uint32_t *)tmpStr);
                   }
                  else
                   {
                      *PatchingResult = TB_FALSE;
                      printf("        Not Found\n\nOperation halted.\nContact author to add independent support for this Source 2007 build...\n\n");
                      return;
                   }
               }
              else
               {
                 CBCDRHelperJTAddr = pLoc + 46;
                 OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
                 tmp = *((uint32_t *) tmpStr);
               }

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
          printf("        Not Found\n\nOperation halted.\nContact author to add independent support for this Source 2007 build...\n\n");
          return;
       }
      printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDenyHelper safe label address ...\n",i);
      i ++;
      if (Res)
       {
          if (fileOS == VUP_Win32)
           {
              /*
              uint32_t tmp;
              CBCDRHelperSafeLabelAddr = pLoc + 69;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
              tmp = *((uint32_t *) tmpStr);
              CBCDRHelperSafeLabelAddr += tmp + 4;
              printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
              CBCDRHelperSafeLabelAddr += PEBase;
              */


              OPParseUserPtrn(VLV2K7JMPSafeLabelData_WiN,TU_UseHalfBytePatterns,&Pat);
              OPSetSearchWithMarkersState(fileStream,TB_TRUE);
              OPSetSearchMarkers(fileStream,pLoc,0,OP_MODIFY_START_OFFSET_MARKER);
              OPFindPtrn(fileStream,Pat,5,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
              OPFreePatternMemory(&Pat);
              OPSetSearchWithMarkersState(fileStream,TB_FALSE);
              OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_START_OFFSET_MARKER);
              if (Res)
               {
                  CBCDRHelperSafeLabelAddr = pLoc;
                  printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
                  CBCDRHelperSafeLabelAddr += PEBase;
               }
              else
               {
                  *PatchingResult = TB_FALSE;
                  printf("        Not Found.\n        Skipping\n\n\n");
               }
           }
          else
           {
              if (NewBuild_11032008 == TB_TRUE)
               {
                 CBCDRHelperSafeLabelAddr = pLoc + 45;
               }
              else
               {
                 CBCDRHelperSafeLabelAddr = pLoc + 82;
               }
              printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
              CBCDRHelperSafeLabelAddr = 0xFFFFFFFF - (GOT[0] - CBCDRHelperSafeLabelAddr) + 1;
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
          if (AllowNonSteam || AllowCrackedSteam || eSTEAMATiON_Prep_Mode)
           {
             j = 11;
             if (fileOS == VUP_Linux)
              j ++;
             printf("\n%u%c) Patching Steam VAC Logon check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
             if (eSTEAMATiON_Prep_Mode)
              {
                 uint8_t j;
                 char ii = 'a';
                 j = 10;
                 if (fileOS == VUP_Linux)
                   j ++;
                 printf("\n%u%c) Patching Steam VAC Logon In-Game(With SemiSteam Clients) check ... ",i,ii);
                 CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
                 printf("Done\n\n\n");
                 ii ++;
              }
           }
          if (AllowNonSteam /*&&*/ || eSTEAMATiON_Prep_Mode)
           {
             j = 13;
             if (fileOS == VUP_Linux)
              j ++;
             printf("\n%u%c) Patching Steam UserID ticket verification check ... ",i,ii);
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
          if (/*(AllowNonSteam || AllowCrackedSteam) &&*/ RemoveClientConnLost)
           {
             j = 12;
             if (fileOS == VUP_Linux)
              j ++;
             printf("\n%u%c) Patching new \"Client Timed-Out\" protection ... ",i,ii);
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

           /*
           {
             int k,IsLinux;
             IsLinux = ((fileOS == VUP_Linux) ? 1 : 0);
             printf("JMP TABLE BINARY DUMP FOR AUTHOR AUDIT:\n");
             for (k = IsLinux; k < 14 + IsLinux; k ++)
              printf("         0x%"PRIX32"\n",CBCDRHelperJT[k]);
             printf("END OF JMP TABLE BINARY DUMP FOR AUTHOR AUDIT:\n\n\n");
           }
          */

          i ++;
       }
    }


   if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
    {
       TOPError tmpErr;
       printf("\n%u) Looking for CSteam3::CheckForDuplicateSteamID duplicate SteamID check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLV2K7CheckDuplicateSteamIDFunc,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Win32) ? TB_FALSE : TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if ((fileOS == VUP_Win32))
            {
              tmpStr[10] = 0x89;
              tmpStr[11] = 0xC0;
            }
           else
            {
              tmpStr[7] = 0x90;
              tmpStr[8] = 0xE9;
            }
           printf("Done\n\n\n");
        }
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }

   if ((!UseIDMode) && (AllowNonSteam || AllowCrackedSteam /* || (!UseLANIDInLANMode) */) && (!eSTEAMATiON_Prep_Mode))
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
           TVAADDR DW2_Loc = 0,DW3_Loc = 0,DW4_Loc = 0;
           uint8_t safety_counter = 0;
           OPParseUserPtrn(VLVSteamIDDWORD_4,TU_UseBytePatterns,&Pat);
           OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&DW4_Loc,&Res);
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
                   goto safety_interrupted;
                 DW3_Loc --;
                 safety_counter ++;
               }
              while (strncmp((char *)DW3,VLVSteamIDDWORD_3,4) && (safety_counter < 20));
              if (safety_counter == 20)
                goto safety_interrupted;
              safety_counter = 0;
              DW2 = DW3;
              DW2_Loc = DW3_Loc;
              do
               {
                 DW2 --;
                 if (!DW2_Loc)
                   goto safety_interrupted;
                 DW2_Loc --;
                 safety_counter ++;
               }
              while (strncmp((char *)DW2,VLVSteamIDDWORD_2,4) && (safety_counter < 20));
              if (safety_counter == 20)
                goto safety_interrupted;

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
               safety_interrupted:
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
       tmpErr = OPParseUserPtrn(VLV2K7LANChk_Src,TU_UseHalfBytePatterns,&Pat);
       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
           TVAADDR tmpLast;
           OPGetLastOffsetInStream(fileStream,&tmpLast);
           OPSetSearchMarkers(fileStream,0,((tmpLast >> 2) + 0x40000),OP_MODIFY_END_OFFSET_MARKER);
           OPSetSearchWithMarkersState(fileStream,TB_TRUE);
        }
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Win32) ? TB_FALSE : TB_TRUE,0,&pLoc,&Res);
       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
          OPSetSearchWithMarkersState(fileStream,TB_FALSE);
          OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_END_OFFSET_MARKER);
        }
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           if (fileOS == VUP_Win32)
            {
              OPParseUserPtrn(VLV2K7LANChk_WinDst,TU_UseBytePatterns,&Pat);
            }
           else
            {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if (tmpStr[3] == 0x89)
               OPParseUserPtrn(VLV2K7LANChk_UNiXDst_1,TU_UseBytePatterns,&Pat);
              else
               OPParseUserPtrn(VLV2K7LANChk_UNiXDst_2,TU_UseBytePatterns,&Pat);
            }
           printf("Patching ...  ");
           OPWritePtrn(fileStream,pLoc,Pat);
           OPFreePatternMemory(&Pat);
           printf("Done\n\n\n");
        }
       else
        {
           //*PatchingResult = TB_FALSE;
           //printf("        Not Found\n\n\n");
           printf("        Not Found. Skipping...\n\n\n");
        }
    }
   if (RemoveMasterWarning)
    {
       TOPError tmpErr;
       printf("\n%u) Looking for \"MasterRequestRestart. Please Update Your Server\" msg ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLV2K7MasterRequestRestartNag_Src,TU_UseHalfBytePatterns,&Pat);
       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
           TVAADDR tmpLast;
           OPGetLastOffsetInStream(fileStream,&tmpLast);
           OPSetSearchMarkers(fileStream,tmpLast >> 1,0,OP_MODIFY_START_OFFSET_MARKER);
           OPSetSearchWithMarkersState(fileStream,TB_TRUE);
        }
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
          OPSetSearchWithMarkersState(fileStream,TB_FALSE);
          OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_START_OFFSET_MARKER);
        }
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           if (fileOS == VUP_Win32)
            {
              OPParseUserPtrn(VLV2K7MasterRequestRestartNag_Dst_Win32,TU_UseBytePatterns,&Pat);
            }
           else
            {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if (tmpStr[3] == 0x74)
               OPParseUserPtrn(VLV2K7MasterRequestRestartNag_Dst_UNiX_1,TU_UseBytePatterns,&Pat);
              else
               OPParseUserPtrn(VLV2K7MasterRequestRestartNag_Dst_UNiX_2,TU_UseBytePatterns,&Pat);
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
   if (ClientOptions || ClientChecks)
    {
       TOPError tmpErr;
       if (fileOS == VUP_Win32)
        {
          printf("\n%u) Looking for Steam execution requirement check ...\n",i);
          i ++;
          tmpErr = OPParseUserPtrn(VLV2K7ServerReqSteam_Src,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
              printf("        Found at 0x%"PRIX64".   ",pLoc);
              /*if (fileOS == VUP_Win32)
               {*/
                 OPParseUserPtrn(VLV2K7ServerReqSteam_Dst_Win,TU_UseBytePatterns,&Pat);
               /*}
              else
               {
                 OPParseUserPtrn(VLV2K7ServerReqSteam_Dst_Unix,TU_UseBytePatterns,&Pat);
               }*/
              printf("Patching ...  ");
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              OPWritePtrn(fileStream,pLoc + ((tmpStr[2] == 0x0) ? 1 : 0),Pat);
              OPFreePatternMemory(&Pat);
              printf("Done\n\n\n");
           }
          else
           {
              *PatchingResult = TB_FALSE;
              printf("        Not Found\n\n\n");
           }
          printf("\n%u) Looking for internet server's CD Key requirement check ...\n",i);
          i ++;
          tmpErr = OPParseUserPtrn(VLV2K7CDKeyRequiredForInternerServers ,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
              printf("        Found at 0x%"PRIX64".   ",pLoc);
              OPJumpToOffsetInStream(fileStream,pLoc + 7,&tmpStr,&rems);
              printf("Patching ...  ");
              *tmpStr = 0xEB;
              printf("Done\n\n\n");
           }
          else
           {
              *PatchingResult = TB_FALSE;
              printf("        Not Found\n\n\n");
           }
          printf("\n%u) Looking for Client execution permission check ...\n",i);
          i ++;
          tmpErr = OPParseUserPtrn(VLV2K7ExecutionPermissionChecking_Src,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
              printf("        Found at 0x%"PRIX64".   ",pLoc);
              printf("Patching ...  ");
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if (tmpStr[0] == 0x68)
               {
                 OPParseUserPtrn(VLV2K7ExecutionPermissionChecking_Dst,TU_UseBytePatterns,&Pat);
                 OPJumpToOffsetInStream(fileStream,pLoc + 6,&tmpStr,&rems);
                 if ((tmpStr[0] == 0x8B) && (tmpStr[1] == 0xC8))
                   pLoc += 4;
                 OPWritePtrn(fileStream,pLoc,Pat);
                 OPFreePatternMemory(&Pat);
               }
              else
               {
                 tmpStr[25] = 0xEB;
               }
              printf("Done\n\n\n");
           }
          else
           {
              *PatchingResult = TB_FALSE;
              printf("        Not Found\n\n\n");
           }
        }
    }
}

