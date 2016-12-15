#include "vsource2007U1.h"

uint8_t Src2K7U1NewProtocolCalcaulatingSystem = 0;

extern TBOOL eSTEAMATiON_Prep_Mode, eSTEAMATiON_Prep_DropCrackedSteam, eSTEAMATiON_Prep_AllowDuplicateSteamIDs,
             PatchMasterServerUpdateNotificationProtection, PatchClientConnectionLostForExtractedSteamProtection,LeavePENDINGIDInPlace,Source2007U2NoSteamLogonDisable;

TBOOL IfSrc2K7U1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   int32_t build_nr;
   uint8_t proto;
   char eng_version[40];
   char eng_time[9];
   char eng_date[12];
   strcpy(Version,"UNKNOWN");
   if (!GetValveEngineInfo(fileStream,&build_nr,&proto,eng_version,eng_time,eng_date,fileOS))
     return TB_FALSE;
   if (Src2K7U1NewProtocolCalcaulatingSystem)
    {
       snprintf(Version,200,"Valve engine(Source 2007 U1)\nProtocol version EXTERNAL(Exe version without dots)\nExe version %s (ModName)\nExe build: %s %s (%"PRIi32")\n",eng_version,eng_time,eng_date,build_nr);
      return TB_TRUE;
    }
   else
    if ((proto > 35) && (proto < 47))
     {
       snprintf(Version,200,"Valve engine(Source 2007 U1)\nProtocol version %"PRIu8"\nExe version %s (ModName)\nExe build: %s %s (%"PRIi32")\n",
               proto,eng_version,eng_time,eng_date,build_nr);
       return TB_TRUE;
     }
    else
      return TB_FALSE;
}

void PatchSrc2K7U1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
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

   NewSrc2K7Builds1 = TB_TRUE;

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

   if (AllowNonSteam || AllowNonClassBInLANServer || eSTEAMATiON_Prep_Mode || AllowCrackedSteam)
    {
      TVAADDR tmpLast;
      if (!eSTEAMATiON_Prep_Mode)
       {
         TBOOL NewBuild_11032008 = TB_FALSE,NewBuildP36 = TB_FALSE;
         char ii = 'a';
         printf("\n%u) Looking for Steam validation check ...\n",i);
         OPParseUserPtrn(VLV2K7U1SteamValidation_Src,TU_UseHalfBytePatterns,&Pat);

         if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
          {
            OPGetLastOffsetInStream(fileStream,&tmpLast);
            OPSetSearchMarkers(fileStream,0,tmpLast / 3,OP_MODIFY_END_OFFSET_MARKER);
            OPSetSearchWithMarkersState(fileStream,TB_TRUE);
          }

         OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,/*(fileOS == VUP_Linux) ? TB_TRUE : */TB_FALSE,0,&pLoc,&Res);
         OPFreePatternMemory(&Pat);
         if (Res)
          {
             if (fileOS == VUP_Win32)
              {
                 OPParseUserPtrn(VLV2K7U1SteamValidation_Win_Dst,TU_UseBytePatterns,&Pat);
              }
             else
              {
                 OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                 if ((tmpStr[3] == 0x89) && (tmpStr[0] ^ 0x80 < 0x10))
                   NewBuild_11032008 = TB_TRUE;
                 if (tmpStr[8 - (NewBuild_11032008 ? 1: 0)] == 0x89)
                  {
                    NewBuildP36 = TB_TRUE;
                  }
                 OPParseUserPtrn(VLV2K7U1SteamValidation_UNiX_Dst,TU_UseBytePatterns,&Pat);
              }
             printf("        Found at 0x%"PRIX64".   ",pLoc);
             printf("Patching ...  ");
             if (eSTEAMATiON_Prep_Mode)
              {
                 OPFreePatternMemory(&Pat);
                 printf("Not needed in eSTEAMATiON Preparation mode\n\n\n");
              }
             else
              {
                OPWritePtrn(fileStream,pLoc - (NewBuild_11032008 ? 1 : 0) + (NewBuildP36 ? 3 : 0),Pat);
                OPFreePatternMemory(&Pat);
                printf("Done\n\n\n");
              }

            if (AllowNonSteam)
             {
               SteamValidationLoc = pLoc;
               printf("\n%u%c) Looking for and storing validation code location ...\n",i,ii);
               SteamValidationProtectionAddressLoc = (pLoc - (NewBuild_11032008 ? 1 : 0)) + (NewBuildP36 ? 3 : 0) + ((fileOS == VUP_Win32) ? 20 : 21);

               OPParseUserPtrn(VLV2K7U1SteamValidationProtectionCodeConsistencyCheck,TU_UseHalfBytePatterns,&Pat);
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
                }
             }
          }
         else
          {
             *PatchingResult = TB_FALSE;
             printf("        Not Found\n\n\n");
          }
         i ++;
       }
      if (!eSTEAMATiON_Prep_Mode && AllowNonSteam)
       {
         char ii = 'a';
         printf("\n\nSOURCE 2007 U1 ENGINE IS LOADED... REMOVING STEAM CERTIFICATE'S RELATED CHECK... \n\n");
         printf("\n%u) Looking for Steam certificate's length check ...\n",i);

         /*
         */
         OPParseUserPtrn(VLV2K7U1SteamCertificateChecking_Src,TU_UseHalfBytePatterns,&Pat);
         OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Linux) ? TB_TRUE : TB_FALSE,0,&pLoc,&Res);
         OPFreePatternMemory(&Pat);
         if (Res)
          {
             uint64_t pLoc3 = pLoc; // ploc3 will store the "Steam ceritificate checking pattern start location"
             printf("        Found at 0x%"PRIX64".   ",pLoc);
             if (SteamValidationLoc || AllowNonClassBInLANServer)
              {
                 uint32_t uRelOffFromSCLERlJmpToSVRRlJmp;
                 uint8_t pmcArgumentsForNotifyClientConnect[8],pmcLinuxRegisterWithCertLenToTmpRegLEA[4] = {0,0,0,0};
                 TSTREAMADDR_NOHDR tmpStr2;
                 TVAADDR NoCertLoc = 0;
                 TVAADDR Location,PreValidationPrepLoc,SteamCertFailedJMP;
                 uint8_t uRegisterType = 0;
                 /*if (fileOS == VUP_Linux)
                  {

                  }*/
                 OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                 if (fileOS == VUP_Win32)
                  {
                     // Depend on the pattern - lets make it selective
                    switch (tmpStr[1])
                     {
                        case 0x55: // EBP register
                                 uRegisterType = 0;
                                 Location = pLoc + 17; // Needs to point to "Steam cert length error" in patterns conditional jump machine command
                                 uRelOffFromSCLERlJmpToSVRRlJmp = SteamValidationLoc - (Location + 4); // Will store the "relative jmp" offset to get from the above jump to "Steam validation rejected" code place.
                                 OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems); // Gonna read the respective relative-offset of "Steam certificate length error
                                 SteamCertFailedJMP = Location - 2; // Address of the relative jump of our relative offset.
                                 NoCertLoc = *((uint32_t *)(tmpStr)) + Location + 4; // 4 is a size in bytes of our jump offset
                                 break;
                        case 0x47: // LEA EAX,[EDI - 1]   Used in L4D1 <- EDI Register
                                 uRegisterType = 1;
                                 goto LBL_ALGO;

                        case 0x40: // Unknown - Yet to be determined(Possibly for L4D2)
                                 uRegisterType = 2;
                                 goto LBL_ALGO;

                        case 0x43: // L4D2 Engine Build 4121 and above <- EBX Register
                                   // LEA  EAX, [EBX-1]
                                 uRegisterType = 3;
                                 goto LBL_ALGO;

                        default:
                                 printf("\nFATAL ERROR: Unsupported register used to store certificate length\nPatching halted\nContact author for an updated version of the program\n");
                                 *PatchingResult = TB_FALSE;
                                 break;

                                LBL_ALGO:
                                 Location = pLoc + 10; // Needs to point to "Steam cert length error" in patterns conditional jump machine command
                                 uRelOffFromSCLERlJmpToSVRRlJmp = SteamValidationLoc - (Location + 4); // Will store the "relative jmp" offset to get from the above jump to "Steam validation rejected" code place.
                                 OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems); // Gonna read the respective relative-offset of "Steam certificate length error
                                 SteamCertFailedJMP = Location - 2; // Address of the relative jump of our relative offset.
                                 NoCertLoc = *((uint32_t *)(tmpStr)) + Location + 4; // 4 is a size in bytes of our jump offset
                                 break;
                     }
                  }
                 else
                  {
                    // Depend on the pattern - lets make it selective
                    switch (tmpStr[1])
                     {
                       case 0x4C: // ECX Register
                                 uRegisterType = 0;
                                 memcpy(pmcLinuxRegisterWithCertLenToTmpRegLEA,tmpStr,4);
                                 switch (tmpStr[11])
                                  {
                                    case 0x0F: // Long jump
                                            Location = pLoc + 13; //Needs to point to "Steam cert length error" in patterns conditional jump machine command
                                            uRelOffFromSCLERlJmpToSVRRlJmp = SteamValidationLoc - (Location + 4); // Offset for Steam validation rejected machine code location.
                                            OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                                            SteamCertFailedJMP = Location - 2;
                                            NoCertLoc = *((uint32_t *)(tmpStr)) + Location + 4;
                                            break;
                                    case 0x76: // Short jump
                                            Location = pLoc + 12; //Needs to point to "Steam cert length error" in patterns conditional jump machine command
                                            uRelOffFromSCLERlJmpToSVRRlJmp = SteamValidationLoc - (Location + 1); // Offset for Steam validation rejected machine code location.
                                            OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                                            SteamCertFailedJMP = Location - 1;
                                            NoCertLoc = *((uint8_t *)(tmpStr)) + Location + 1;
                                            break;
                                  }
                                 break;
                       case 0x54: // EDX Register
                                 uRegisterType = 1;
                                 memcpy(pmcLinuxRegisterWithCertLenToTmpRegLEA,tmpStr,4);
                                 Location = pLoc + 15;
                                 uRelOffFromSCLERlJmpToSVRRlJmp = SteamValidationLoc - (Location + 4);
                                 OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                                 SteamCertFailedJMP = Location - 2;
                                 NoCertLoc = *((uint32_t *)(tmpStr)) + Location + 4;
                                 break;
                       case 0x5C: // EBX Register
                                  uRegisterType = 2;
                                  memcpy(pmcLinuxRegisterWithCertLenToTmpRegLEA,tmpStr,4);
                                  Location = pLoc + 15;
                                  uRelOffFromSCLERlJmpToSVRRlJmp = SteamValidationLoc - (Location + 4);
                                  OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                                  SteamCertFailedJMP = Location - 2;
                                  NoCertLoc = *((uint32_t *)(tmpStr)) + Location + 4;
                                  break;

                       case 0x45: // EAX Register - L4D2 23.04.2010 update
                                  uRegisterType = 3;
                                  pmcLinuxRegisterWithCertLenToTmpRegLEA[0] = 0x90;
                                  memcpy(pmcLinuxRegisterWithCertLenToTmpRegLEA + 1,tmpStr,3);
                                  Location = pLoc + 13;
                                  uRelOffFromSCLERlJmpToSVRRlJmp = SteamValidationLoc - (Location + 4);
                                  OPJumpToOffsetInStream(fileStream,Location,&tmpStr,&rems);
                                  SteamCertFailedJMP = Location - 2;
                                  NoCertLoc = *((uint32_t *)(tmpStr)) + Location + 4;
                                  break;
                       default:
                                 printf("\nFATAL ERROR: Unsupported register used to store certificate length\nPatching halted\nContact author for an updated version of the program\n");
                                 *PatchingResult = TB_FALSE;
                                 break;
                     }

                  }
                 printf("\n%u%c) Governing NULL certificate client drop location ...\n        Found at 0x%"PRIX64"\n",i,ii,NoCertLoc);

                 if (fileOS == VUP_Linux)
                  {
                    OPParseUserPtrn(VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP_VALiDATiON_NiX,TU_UseHalfBytePatterns,&Pat);
                    OPFindPtrn(fileStream,Pat,1,TLOOKUPSTATICOFFSET,TB_FALSE,NoCertLoc,&PreValidationPrepLoc,&Res);
                    OPFreePatternMemory(&Pat);
                    printf("\n%u%c) Verifying NULL certificate client drop location code consistency ...\n        %s\n",i,ii,Res ? "SUCCEED" : "FAILED");
                    ii ++;
                    if (!Res)
                     {
                       printf("        Skipping certificate-related patching(SAFETY MEASURE).\nSome ancient Non-Steam clients will NOT be able to join the server...\n\n");
                       goto VUP_SRC2K7_CERT_PTCH_FINITA;
                     }
                    OPJumpToOffsetInStream(fileStream,PreValidationPrepLoc,&tmpStr2,&rems);
                    if (tmpStr2[1] == 0x0F)
                      NoCertLoc += 8;
                  }


                 OPParseUserPtrn(VLV2K7U1NotifyClientConnectCallPrep,TU_UseHalfBytePatterns,&Pat);
                 OPGetLastOffsetInStream(fileStream,&tmpLast);
                 OPSetSearchMarkers(fileStream,SteamValidationProtectionAddressLoc - 150,SteamValidationProtectionAddressLoc,OP_MODIFY_BOTH_MARKERS);
                 OPSetSearchWithMarkersState(fileStream,TB_TRUE);
                 OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&PreValidationPrepLoc,&Res);
                 OPSetSearchWithMarkersState(fileStream,TB_FALSE);
                 OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_BOTH_MARKERS);
                 OPFreePatternMemory(&Pat);
                 ii ++;
                 printf("\n%u%c) Reading stack-parameters passed to Steam validation routine ...\n",i,ii);
                 if (Res)
                  {
                     printf("        Found at 0x%"PRIX64"\n",PreValidationPrepLoc);
                     OPJumpToOffsetInStream(fileStream,PreValidationPrepLoc,&tmpStr2,&rems);
                     ii++ ;
                     if ((uRegisterType == 3) && (fileOS == VUP_Linux)) // L4D2 23.04.2010 update
                      {
                         pmcArgumentsForNotifyClientConnect[0] = 0x90;
                         memcpy(pmcArgumentsForNotifyClientConnect + 1,tmpStr2,7);
                      }
                     else
                       memcpy(pmcArgumentsForNotifyClientConnect,tmpStr2,8);
                     if (fileOS == VUP_Win32)
                      {
                        /*
                          Adding a jump to our new code for cert validation with SteamClientLess Non-Steam support
                        */
                        printf("\n%u%c) Adding NonSteam support junctions to code ...\n",i,ii);
                        tmpStr2[0] = 0xE9;
                        *((int32_t *)(tmpStr2 + 1)) = NoCertLoc - (PreValidationPrepLoc + 5);
                        tmpStr2[5] = 0x66;
                        tmpStr2[6] = 0x89;
                        tmpStr2[7] = 0xC0;
                      }
                     else
                      {
                        printf("\n%u%c) Checking for certificate length register ...\n",i,ii);
                        ii ++;
                        if (pmcLinuxRegisterWithCertLenToTmpRegLEA[0])
                         {
                           printf("        Found\n");
                           ii ++;
                           printf("\n%u%c) Adding NonSteam support junctions to code ...\n",i,ii);
                           /*
                           memcpy(tmpStr2,pmcLinuxRegisterWithCertLenToTmpRegLEA,4);
                           tmpStr2[4] = 0xEB;
                           *((int32_t *)(tmpStr2 + 5)) = NoCertLoc - (PreValidationPrepLoc + 6);
                           tmpStr2[6] = 0x89;
                           tmpStr2[7] = 0xC0;
                           */
                           tmpStr2[0] = 0xE9;
                           *((int32_t *)(tmpStr2 + 1)) = NoCertLoc - (PreValidationPrepLoc + 5);
                           tmpStr2[5] = 0x66;
                           tmpStr2[6] = 0x89;
                           tmpStr2[7] = 0xC0;
                         }
                        else
                         if (pmcArgumentsForNotifyClientConnect[0])
                          {
                             printf("        Found\n");
                             ii ++;
                             printf("\n%u%c) Adding NonSteam support junctions to code ...\n",i,ii);
                             /*
                             tmpStr2[0] = 0xEB;
                             *((int8_t *)(tmpStr2 + 1)) = NoCertLoc - (PreValidationPrepLoc + 2);
                             tmpStr2[2] = 0x89;
                             tmpStr2[3] = 0xC0;
                             */
                             tmpStr2[0] = 0xE9;
                             *((int32_t *)(tmpStr2 + 1)) = NoCertLoc - (PreValidationPrepLoc + 5);
                             tmpStr2[5] = 0x66;
                             tmpStr2[6] = 0x89;
                             tmpStr2[7] = 0xC0;
                          }
                         else
                           {
                             printf("        Not Found. Skipping ...\n\n\n");
                             goto VUP_SRC2K7_CERT_PTCH_FINITA;
                           }
                      }
                     if (fileOS == VUP_Win32)
                       OPParseUserPtrn(VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP2_Dst,TU_UseBytePatterns,&Pat);
                     else
                       OPParseUserPtrn(VLV2K7U1SteamCertificateCheckingPreSteamValidationJMP_UNIX_Dst,TU_UseBytePatterns,&Pat);
                     OPWritePtrn(fileStream,NoCertLoc,Pat);
                     OPFreePatternMemory(&Pat);

                     /* Switched to Jmp3 - This fill-up only needed with Jmp2 with short jump in the end.
                     if (fileOS == VUP_Win32)
                      {
                         OPParseUserPtrn(VLV2K7U1SteamCertificateChecking_Dst3,TU_UseBytePatterns,&Pat);
                         OPWritePtrn(fileStream,NoCertLoc + 14,Pat);
                         OPFreePatternMemory(&Pat);
                      }
                     */

                     /*
                       Final editing of our new code with proper jump addresses.
                     */
                     OPJumpToOffsetInStream(fileStream,NoCertLoc,&tmpStr2,&rems);
                     if (fileOS == VUP_Linux)
                      {
                        memcpy(tmpStr2 + 1,pmcLinuxRegisterWithCertLenToTmpRegLEA,4);
                        memcpy(tmpStr2 + 10,pmcArgumentsForNotifyClientConnect,8);
                      }
                     else
                       memcpy(tmpStr2 + 4,pmcArgumentsForNotifyClientConnect,8);
                     if (fileOS == VUP_Linux)
                      {
                        switch (uRegisterType)
                         {
                           case 0: tmpStr2[6] = 0xC9; //ECX
                                   tmpStr2[0] = 0x51; // Must push the register as it possibly will be used by NotifyClientConnect function
                                   tmpStr2[7] = 0x59; // Pop then finished our stuff
                                   break;
                           case 1:
                                   tmpStr2[6] = 0xD2; //EDX
                                   tmpStr2[0] = 0x52;
                                   tmpStr2[7] = 0x5A;
                                   break;
                           case 2:
                                   tmpStr2[6] = 0xDB; //EBX
                                   tmpStr2[0] = 0x53;
                                   tmpStr2[7] = 0x5B;
                                   break;
                           case 3:
                                   tmpStr2[6] = 0xC0; //EAX L4D2 update 23.04.2010
                                   tmpStr2[0] = 0x50;
                                   tmpStr2[7] = 0x58;
                                   break;
                         }
                      }
                     else
                      {
                        switch (uRegisterType)
                         {
                           case 0:
                                   tmpStr2[1] = 0xED;
                                   break;
                           case 1:
                                   tmpStr2[1] = 0xFF;
                                   break;
                           case 2:
                                   tmpStr2[1] = 0x0; // To be determined
                                   break;
                           case 3:
                                   tmpStr2[1] = 0xDB;
                                   break;
                         }
                      }
                     if (fileOS == VUP_Linux)
                      {
                         uint8_t SteamValidationJumpingCheck = 3;
                         OPJumpToOffsetInStream(fileStream,pLoc3,&tmpStr,&rems);
                         if (tmpStr[3] == 0x0)
                           SteamValidationJumpingCheck ++;
                         if (tmpStr[SteamValidationJumpingCheck + 4] == 0x89)
                           SteamValidationJumpingCheck += 3;
                         SteamValidationJumpingCheck += (4 + 7);

                         tmpStr2[9] = (SteamValidationLoc + SteamValidationJumpingCheck) - (NoCertLoc + 10);
                         ((uint32_t *)(tmpStr2 + 19))[0] = (PreValidationPrepLoc + 8)  - (NoCertLoc + 23);
                         printf("        Done\n");

                         ii ++;
                         printf("\n%u%c) Looking for relocations within new code area ...\n",i,ii);
                         TBOOL bIsOffendingRelocFound = TB_FALSE;
                         Elf32_Rel *psOffendingReloc;
                         OPELFFindRelocationInSpecificAddressRange(fileStream,NoCertLoc,NoCertLoc + 22,(void **)&psOffendingReloc,&bIsOffendingRelocFound);
                         if (bIsOffendingRelocFound)
                          {
                              printf("        Found at 0x%"PRIX64".   ",(uint64_t)(psOffendingReloc -> r_offset));
                              printf("Patching ...  ");
                              psOffendingReloc -> r_offset = NoCertLoc + 24;
                              //psOffendingReloc -> r_info = R_386_NONE; // Dynamic Linker doesn't like this type of reloc in denamic reloc table
                              printf("Done\n\n\n");
                          }
                         else
                          printf("        Not Found(GOOD) ...\n");

                      }
                     else
                      {
                         tmpStr2[3] = (SteamValidationLoc + 12) - (NoCertLoc + 4);
                         ((uint32_t *)(tmpStr2 + 13))[0] = (PreValidationPrepLoc + 8)  - (NoCertLoc + 17);
                         printf("        Done\n");
                      }
                     //printf("\nDEBUG: Relative offsets in NS junction are 0x%"PRIi8" and 0x%"PRIi8"\n\n",(SteamValidationLoc + 12) - (NoCertLoc + 4),(PreValidationPrepLoc + 8)  - (NoCertLoc + 14));

                     ii ++;
                     printf("\n%u%c) Patching main Steam certificate check(SECURELY) ...\n",i,ii);
                     if (fileOS == VUP_Win32)
                      {
                        OPParseUserPtrn(VLV2K7U1SteamCertificateChecking_Win_Dst,TU_UseBytePatterns,&Pat);
                      }
                     else
                      {
                        if (uRegisterType)
                          OPParseUserPtrn(VLV2K7U1SteamCertificateChecking_UNiX_Dst2,TU_UseBytePatterns,&Pat);
                        else
                          OPParseUserPtrn(VLV2K7U1SteamCertificateChecking_UNiX_Dst1,TU_UseBytePatterns,&Pat);

                      }
                     OPWritePtrn(fileStream,pLoc3,Pat);
                     OPFreePatternMemory(&Pat);
                     OPJumpToOffsetInStream(fileStream,pLoc3,&tmpStr2,&rems);
                     /*
                        Here we put a jump to Steam validation rejected stuff for Non-Steam clients.
                     */
                     if (fileOS == VUP_Linux)
                      {
                        switch (uRegisterType)
                         {
                           case 0:
                                   switch (tmpStr2[11])
                                    {
                                     case 0x0F: // Long jump
                                               *((uint32_t *)(tmpStr2 + 13)) = SteamValidationProtectionAddressLoc - (pLoc3 +  13 + 4);
                                               break;
                                     case 0x76: // Short jump
                                               *((uint8_t *)(tmpStr2 + 12)) = SteamValidationProtectionAddressLoc - (pLoc3 +  12 + 1);
                                               break;
                                    }
                                   break;
                           case 1:
                                   *((uint32_t *)(tmpStr2 + 15)) = SteamValidationProtectionAddressLoc - (pLoc3 + 15 + 4);

                                   break;
                           case 2:
                                   *((uint32_t *)(tmpStr2 + 15)) = SteamValidationProtectionAddressLoc - (pLoc3 + 15 + 4);
                                   break;
                         }
                      }
                     else
                      {
                        switch (uRegisterType)
                         {
                           case 0:
                                   *((uint32_t *)(tmpStr2 + 17)) = SteamValidationProtectionAddressLoc - (pLoc3 + 17 + 4);
                                   break;
                           case 1:
                           case 2:
                                   *((uint32_t *)(tmpStr2 + 10)) = SteamValidationProtectionAddressLoc - (pLoc3 + 10 + 4);
                                   break;
                         }
                      }

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
      uint64_t PEBase,ELFBase;
      TBOOL fPIC_Build = TB_TRUE;
      TVAADDR tmpLast;
      //TBOOL NewBuild_11032008 = TB_FALSE;
      printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDenyHelper jump table address ...\n",i);
      i ++;
      OPParseUserPtrn(VLV2K7U1JMPTabHelper,TU_UseHalfBytePatterns,&Pat);
      if (fileOS == VUP_Win32)
       {
         OPGetLastOffsetInStream(fileStream,&tmpLast);
         OPSetSearchMarkers(fileStream,0,tmpLast >> 1,OP_MODIFY_END_OFFSET_MARKER);
         OPSetSearchWithMarkersState(fileStream,TB_TRUE);
       }
      /*
      else
       {
         if (NewSrc2K7Builds1)
          {
            OPGetLastOffsetInStream(fileStream,&tmpLast);
            OPSetSearchMarkers(fileStream,0,((tmpLast >> 2) + 0x30000),OP_MODIFY_END_OFFSET_MARKER);
            OPSetSearchWithMarkersState(fileStream,TB_TRUE);
          }
       }
      */
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPSetSearchWithMarkersState(fileStream,TB_FALSE);
      OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_END_OFFSET_MARKER);
      OPFreePatternMemory(&Pat);

      if (Res)
       {
          if (fileOS == VUP_Win32)
           {
              TVAADDR CBCDRHelperJTAddr;
              CBCDRHelperJTAddr = pLoc + 19;
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
              switch (tmpStr[0])
               {
                  case 0x83:
                            if (tmpStr[6] == 0xC3)
                              CBCDRHelperJTAddr = pLoc + 13;
                            else
                              CBCDRHelperJTAddr = pLoc + 11;
                            OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
                            tmp = *((uint32_t *) tmpStr);
                            OPELFGetAdditionalInfo(fileStream,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,GOT);
                            CBCDRHelperJTAddr = GOT[0] - (0xFFFFFFFF - tmp + 1); // The error was here as GOT[1] was incorrect and now replaced with GOT[0]
                            OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
                            CBCDRHelperJT = (uint32_t *) tmpStr;
                            printf("        Found at 0x%"PRIX64".   ",CBCDRHelperJTAddr);
                            break;
                  case 0x8B:
                            CBCDRHelperJTAddr = pLoc + 68;
                            OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
                            OPELFGetAdditionalInfo(fileStream,&ELFBase,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
                            CBCDRHelperJTAddr = *((uint32_t *) tmpStr) - ELFBase;
                            OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
                            CBCDRHelperJT = (uint32_t *) tmpStr;
                            printf("        Found at 0x%"PRIX64".   ",CBCDRHelperJTAddr);
                            fPIC_Build = TB_FALSE;
                            break;
               }
           }
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n\nOperation halted.\nContact author to add independent support for this Source 2007 U1 build...\n\n");
          return;
       }
      printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDenyHelper safe label address ...\n",i);
      i ++;
      if (Res)
       {
          if (fileOS == VUP_Win32)
           {
              uint32_t tmp;
              CBCDRHelperSafeLabelAddr = pLoc + 42;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
              tmp = *((uint32_t *) tmpStr);
              CBCDRHelperSafeLabelAddr += tmp + 4;
              printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
              CBCDRHelperSafeLabelAddr += PEBase;
           }
          else
           {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              switch (tmpStr[0])
               {
                  case 0x83:
                            CBCDRHelperSafeLabelAddr = pLoc;
                            printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
                            //CBCDRHelperSafeLabelAddr = 0xFFFFFFFF - (GOT[0] - CBCDRHelperSafeLabelAddr) + 1;
                            CBCDRHelperSafeLabelAddr = GOT[0] - CBCDRHelperSafeLabelAddr;
                            break;
                  case 0x8B:
                            CBCDRHelperSafeLabelAddr = pLoc + 49;
                            printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
                            //CBCDRHelperSafeLabelAddr = 0xFFFFFFFF - (GOT[0] - CBCDRHelperSafeLabelAddr) + 1;
                            if (fPIC_Build)
                              CBCDRHelperSafeLabelAddr = GOT[0] - CBCDRHelperSafeLabelAddr;
                            else
                              CBCDRHelperSafeLabelAddr += ELFBase;
                            break;
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
          /*
          printf("DEBUG: Original jump table dump:\n");
          for (j = ((fileOS == VUP_Linux) ? 1 : 0) ; j < (14 + ((fileOS == VUP_Linux) ? 1 : 0)) ; j ++)
           printf("j = %u  ADDR = 0x%lX\n",j,CBCDRHelperJT[j]);
          printf ("CBCDRHelperSafeLabelAddr = 0x%lX\n", CBCDRHelperSafeLabelAddr);
          printf("\nEnd of JMP Table dump\n\n");
          */
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
             if (Source2007U2NoSteamLogonDisable)
              {
                 uint8_t j;
                 char ii = 'a';
                 j = 2;
                 if (fileOS == VUP_Linux)
                   j ++;
                 printf("\n%u%c) Patching Steam VAC Logon In-Game(Source 2007 U2 - L4D2) check ... ",i,ii);
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

          i ++;
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
       }

      /*
         This code is added for new L4D2 update(Linux version) there OnGSClientKick and OnGSClientDeny drop on their own without
         calling traditional OnGSClientDenyHelper
      */
      if (fileOS == VUP_Linux)
       {
          OPParseUserPtrn(VLV2K7U2JMPTabExtraHelper_UNiX,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
              uint8_t bIs66PresentInCode;
              TVAADDR CBCDRHelperJTAddr;
              printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDeny jump table address ...\n",i);
              i ++;

              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              bIs66PresentInCode = ((tmpStr[26] == 0x66) ? 1 : 0);

              CBCDRHelperJTAddr = pLoc + 38 + bIs66PresentInCode;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
              OPELFGetAdditionalInfo(fileStream,&ELFBase,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
              CBCDRHelperJTAddr = *((uint32_t *) tmpStr) - ELFBase;
              OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
              CBCDRHelperJT = (uint32_t *) tmpStr;
              printf("        Found at 0x%"PRIX64".   ",CBCDRHelperJTAddr);

              printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDeny safe label address ...\n",i);
              i ++;

              CBCDRHelperSafeLabelAddr = pLoc + 27 + bIs66PresentInCode;
              printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
              CBCDRHelperSafeLabelAddr += ELFBase;
              //printf("DBG     Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
              //printf("DBG DUMP CBCDRHelperJT 0x%"PRIX32",0x%"PRIX32", 0x%"PRIX32"\n",CBCDRHelperJT[0],CBCDRHelperJT[1],CBCDRHelperJT[2]);



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
                if (Source2007U2NoSteamLogonDisable)
                 {
                    uint8_t j;
                    char ii = 'a';
                    j = 2;
                    if (fileOS == VUP_Linux)
                      j ++;
                    printf("\n%u%c) Patching Steam VAC Logon In-Game(Source 2007 U2 - L4D2) check ... ",i,ii);
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

             i ++;



              OPParseUserPtrn(VLV2K7U2JMPTabExtraHelper_UNiX,TU_UseHalfBytePatterns,&Pat);
              OPFindPtrn(fileStream,Pat,2,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
              OPFreePatternMemory(&Pat);
              if (Res)
               {
                  printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientKick jump table address ...\n",i);
                  i ++;

                  OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                  bIs66PresentInCode = ((tmpStr[26] == 0x66) ? TB_TRUE : TB_FALSE);
                  CBCDRHelperJTAddr = pLoc + 38 + bIs66PresentInCode;
                  OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
                  OPELFGetAdditionalInfo(fileStream,&ELFBase,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
                  CBCDRHelperJTAddr = *((uint32_t *) tmpStr) - ELFBase;
                  CBCDRHelperJT = (uint32_t *) tmpStr;
                  printf("        Found at 0x%"PRIX64".   ",CBCDRHelperJTAddr);

                  printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientKick safe label address ...\n",i);
                  i ++;

                  CBCDRHelperSafeLabelAddr = pLoc + 27 + bIs66PresentInCode;
                  printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
                  CBCDRHelperSafeLabelAddr += ELFBase;

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
                    if (Source2007U2NoSteamLogonDisable)
                     {
                        uint8_t j;
                        char ii = 'a';
                        j = 2;
                        if (fileOS == VUP_Linux)
                          j ++;
                        printf("\n%u%c) Patching Steam VAC Logon In-Game(Source 2007 U2 - L4D2) check ... ",i,ii);
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

                 i ++;

               }
           }
       }
    }

   /*
   if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
    {
       TOPError tmpErr;
       printf("\n%u) Looking for CSteam3Server::OnGSClientApprove duplicate SteamID check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLV2K7U1ClientAcceptExistingSteamIDCheck,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Win32) ? TB_FALSE : TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if ((fileOS == VUP_Win32))
            {
              tmpStr[12] = 0xEB;
            }
           else
            {
              tmpStr[9] = 0x90;
              tmpStr[10] = 0xE9;
            }
           printf("Done\n\n\n");
        }
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }
   */

   if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
    {
       TOPError tmpErr;
       printf("\n%u) Looking for CSteam3::CheckForDuplicateSteamID duplicate SteamID check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLV2K7U1CheckDuplicateSteamIDFunc,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,(fileOS == VUP_Win32) ? TB_FALSE : TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if (fileOS == VUP_Win32)
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
           if ((fileOS == VUP_Linux) && (tmpStr[14] == 0x15)) // Designed for L4D2-Linux there Our function compiled as inline on CSteam3Server::OnGSClientApprove.
            {
                if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
                 {
                   TOPError tmpErr;
                   i --;
                   printf("\n%ua) Looking for inlined CSteam3::CheckForDuplicateSteamID in \n    CSteam3Server::OnGSClientApprove duplicate SteamID check ...\n",i);
                   i ++;
                   tmpErr = OPParseUserPtrn(VLV2K7U2ClientAcceptExistingSteamIDCheck_UNiX,TU_UseHalfBytePatterns,&Pat);
                   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
                   OPFreePatternMemory(&Pat);
                   if (Res)
                    {
                      printf("        Found at 0x%"PRIX64".   ",pLoc);
                      OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                      printf("Patching ...  ");
                      tmpStr[9] = 0x90;
                      tmpStr[10] = 0xE9;
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
       else
        {
           *PatchingResult = TB_FALSE;
           printf("        Not Found\n\n\n");
        }
    }

   if (AllowNonSteam)
    {
      char ii = 'a';
      printf("\n%u) Patching Valve Lobby related protections(Server-side) ...\n",i);
      ii = 'a';
      printf("\n%u%c) Looking for requirement of server for client to be in Lobby ... \n",i,ii);
      ii ++;
      OPParseUserPtrn(VLV2K7U1LobbyRequirement_Src,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if (fileOS == VUP_Win32)
            {
              tmpStr[9] = 0xEB;
            }
           else
            {
              switch (tmpStr[4])
               {
                 case 0x0D:
                            tmpStr[33] = 0x90;
                            tmpStr[34] = 0xE9;
                            break;
                 case 0x00:
                            if (tmpStr[6] == 0x8B)
                             {
                               tmpStr[16] = 0x90;
                               tmpStr[17] = 0xE9;
                             }
                            else
                             {
                               tmpStr[20] = 0x90;
                               tmpStr[21] = 0xE9;
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

      printf("\n%u%c) Looking for requirement of server for client to be Lobby member ... \n",i,ii);
      ii ++;
      OPParseUserPtrn(VLV2K7U1LobbyMembershipRequirement_Src,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if (fileOS == VUP_Win32)
            {
              tmpStr[20] = 0xEB;
            }
           else
            {
              switch (tmpStr[0])
               {
                   case 0x8B:
                           tmpStr[12] = 0x90;
                           tmpStr[13] = 0xE9;
                           break;
                   case 0xFF:
                             if (tmpStr[1] == 0x56)
                               tmpStr[17] = 0xEB;
                             else
                               tmpStr[18] = 0xEB;
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

      i ++;
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
       tmpErr = OPParseUserPtrn(VLV2K7U1LANChk_Src,TU_UseHalfBytePatterns,&Pat);

       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
           TVAADDR tmpLast;
           OPGetLastOffsetInStream(fileStream,&tmpLast);
           OPSetSearchMarkers(fileStream,0,((tmpLast >> 2) + 0x60000),OP_MODIFY_END_OFFSET_MARKER);
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
              OPParseUserPtrn(VLV2K7U1LANChk_WinDst,TU_UseBytePatterns,&Pat);
            }
           else
            {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if (tmpStr[2] == 0x89)
               {
                 OPParseUserPtrn(VLV2K7U1LANChk_UNiXDst_1,TU_UseBytePatterns,&Pat);
               }
              else
               {
                if (tmpStr[2] == 0x0)
                 {
                   pLoc += 7;
                 }
                else
                 {
                   /* Latest L4D2 versions */
                   if ((tmpStr[2] == 0x8B) && tmpStr[2] == 0x06)
                     pLoc -= 2;
                 }
                OPParseUserPtrn(VLV2K7U1LANChk_UNiXDst_2,TU_UseBytePatterns,&Pat);
               }
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


       printf("\n%u) Looking for SourceTV client's network Class check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLV2K7U2SourceTVLANChk_Src,TU_UseHalfBytePatterns,&Pat);

       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
           TVAADDR tmpLast;
           OPGetLastOffsetInStream(fileStream,&tmpLast);
           OPSetSearchMarkers(fileStream,0,((tmpLast >> 1) + 0x60000),OP_MODIFY_END_OFFSET_MARKER);
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
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           printf("Patching ...  ");
           if (fileOS == VUP_Win32)
            {
               tmpStr[47] = 0xEB;
            }
           else
            {
               tmpStr[49] = 0x90;
               tmpStr[49 + 1] = 0xE9;
            }
           printf("Done\n\n\n");
        }
       else
        {
           printf("        Not Found. Skipping...\n\n\n");
        }
    }
   if (RemoveMasterWarning)
    {
       TOPError tmpErr;
       printf("\n%u) Looking for \"MasterRequestRestart. Please Update Your Server\" msg ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLV2K7U1MasterRequestRestartNag_Src,TU_UseHalfBytePatterns,&Pat);

       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
           TVAADDR tmpLast;
           OPGetLastOffsetInStream(fileStream,&tmpLast);
           OPSetSearchMarkers(fileStream,(tmpLast >> 2),(tmpLast >> 1),OP_MODIFY_BOTH_MARKERS);
           OPSetSearchWithMarkersState(fileStream,TB_TRUE);
        }

       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);

       if ((fileOS == VUP_Linux) && NewSrc2K7Builds1)
        {
          OPSetSearchWithMarkersState(fileStream,TB_FALSE);
          OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_BOTH_MARKERS);
        }

       OPFreePatternMemory(&Pat);
       if (Res)
        {
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           if (fileOS == VUP_Win32)
            {
              OPParseUserPtrn(VLV2K7U1MasterRequestRestartNag_Dst_Win32,TU_UseBytePatterns,&Pat);
            }
           else
            {
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              if (tmpStr[3] == 0x74)
               OPParseUserPtrn(VLV2K7U1MasterRequestRestartNag_Dst_UNiX_1,TU_UseBytePatterns,&Pat);
              else
               if (tmpStr[3] == 0x75)
                 OPParseUserPtrn(VLV2K7U1MasterRequestRestartNag_Dst_UNiX_3,TU_UseBytePatterns,&Pat);
               else
                 OPParseUserPtrn(VLV2K7U1MasterRequestRestartNag_Dst_UNiX_2,TU_UseBytePatterns,&Pat);
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
          tmpErr = OPParseUserPtrn(VLV2K7U1ServerReqSteam_Src,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
              printf("        Found at 0x%"PRIX64".   ",pLoc);
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              printf("Patching ...  ");
              tmpStr[11] = 0xEB;
              printf("Done\n\n\n");
           }
          else
           {
              *PatchingResult = TB_FALSE;
              printf("        Not Found\n\n\n");
           }


          printf("\n%u) Looking for Client execution permission check ...\n",i);
          i ++;
          tmpErr = OPParseUserPtrn(VLV2K7U1ExecutionPermissionChecking_Src,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
              printf("        Found at 0x%"PRIX64".   ",pLoc);
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              printf("Patching ...  ");
              tmpStr[25] = 0xEB;
              printf("Done\n\n\n");
           }
          else
           {
              *PatchingResult = TB_FALSE;
              printf("        Not Found\n\n\n");
           }


          printf("\n%u) Looking for Steam ownership check ...\n",i);
          i ++;
          tmpErr = OPParseUserPtrn(VLV2K7U1SteamOwnershipChecking_Src ,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
              printf("        Found at 0x%"PRIX64".   ",pLoc);
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
              printf("Patching ...  ");
              tmpStr[16] = 0xEB;
              printf("Done\n\n\n");
           }
          else
           {
              *PatchingResult = TB_FALSE;
              printf("        Not Found\n\n\n");
           }



          if (Build >= 3663)
           {
             printf("\n%u) Looking for Lobby enablement check on the server(performed on client) ...\n",i);
             i ++;
             tmpErr = OPParseUserPtrn(VLV2K7U1LobbyClientSideCheckIfServerRequiresIt_Src,TU_UseHalfBytePatterns,&Pat);
             OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
             OPFreePatternMemory(&Pat);
             if (Res)
              {
                printf("        Found at 0x%"PRIX64".   ",pLoc);
                OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                printf("Patching ...  ");
                tmpStr[26] = 0x90;
                tmpStr[27] = 0xE9;
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

}

