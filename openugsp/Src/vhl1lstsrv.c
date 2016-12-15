#include "vhl1lstsrv.h"

extern TBOOL eSTEAMATiON_Prep_Mode, eSTEAMATiON_Prep_DropCrackedSteam, eSTEAMATiON_Prep_AllowDuplicateSteamIDs,LeavePENDINGIDInPlace;

TBOOL IfHL1LstSrvLib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
  int32_t build_nr;
   uint8_t proto;
   char eng_version[40];
   char eng_time[9];
   char eng_date[12];
   strcpy(Version,"UNKNOWN");
   if (!GetValveEngineInfo(fileStream,&build_nr,&proto,eng_version,eng_time,eng_date,fileOS))
     return TB_FALSE;
   if (proto == 47)
    {
      snprintf(Version,200,"Valve engine(Classic GoldSRC Dedicated)\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
              proto,eng_version,eng_time,eng_date,build_nr);
      if (fileOS != VUP_Linux)
       {
          TVAADDR pLoc;
          TBOOL Res;
          TPTRN Pat;
          uint8_t type = 0;
          OPParseUserPtrn(VHL1LSTHWRNDR_MAGIC,TU_UseBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           type = 1;
          else
           {
             OPParseUserPtrn(VHL1LSTSWRNDR_MAGIC,TU_UseBytePatterns,&Pat);
             OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
             OPFreePatternMemory(&Pat);
             if (Res)
              type = 2;
           }
          switch (type)
           {
             case 1:
                     snprintf(Version,200,"Valve engine(Classic GoldSRC Hardware Rendering Listen)\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
                              proto,eng_version,eng_time,eng_date,build_nr);
                     break;
             case 2:
                     snprintf(Version,200,"Valve engine(Classic GoldSRC Software Rendering Listen)\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
                              proto,eng_version,eng_time,eng_date,build_nr);
                     break;
             default:
                     return TB_FALSE;
                     //break;
           }
       }
      else
       return TB_FALSE;
      return TB_TRUE;
    }
   else
    return TB_FALSE;
}

void PatchHL1LstSrvLib(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[200])
{
   TBOOL AllowNonSteam = TB_TRUE, AllowCrackedSteam = TB_TRUE, AllowNonClassBInLANServer = TB_TRUE, AllowHL1WONClients = TB_TRUE, ForceSteamWorksProtocol = TB_FALSE;
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
      printf("\nDo you want to enable Non Steam players with Steam emulator to join your server(sv_lan 0 mode)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          AllowNonSteam = TB_FALSE;
       }

      printf("\nDo you want to enable Non Steam WON players to join your server(sv_lan 0 mode)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          AllowHL1WONClients = TB_FALSE;
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
      printf("\nDo you want to change protocol to new-age SteamWorks(Protocol 48)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'y') || (Answer == 'Y'))
       {
          ForceSteamWorksProtocol = TB_TRUE;
       }
      printf("\nAll required information gatherned...\n");
    }

   eSTEAMATiON_Prep_Advanced_Skip:
   printf("Performing job...\n\n");

   if (AllowNonSteam)
    {
      printf("\n%u) Looking for Steam validation check ...\n",i);
      i ++;
      OPParseUserPtrn(VLVGoldSrcCLListenSteamValidation_Src,TU_UseBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (Res)
       {
          OPParseUserPtrn(VLVGoldSrcCLListenSteamValidation_Dst,TU_UseBytePatterns,&Pat);
          printf("        Found at 0x%"PRIX64".   ",pLoc);
          printf("Patching ...  ");
          OPWritePtrn(fileStream,pLoc + 17,Pat);
          OPFreePatternMemory(&Pat);
          printf("Done\n\n\n");
       }
      else
       {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n\n\n");
       }

      printf("\n%u) Looking for Steam certificate length check(SAFE) ...\n",i);
      i ++;
      OPParseUserPtrn(VLVGoldSrcCLListenSteamCertChk_Src,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (Res)
       {
          OPParseUserPtrn(VLVGoldSrcCLListenSteamCertChk_Dst,TU_UseBytePatterns,&Pat);
          printf("        Found at 0x%"PRIX64".   ",pLoc);
          printf("Patching ...  ");
          OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
          pLoc += 50;
          if (tmpStr[24] == 0x44)
           pLoc ++;
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

   if (AllowNonSteam || AllowCrackedSteam || eSTEAMATiON_Prep_Mode)
    {
      uint64_t PEBase;
      printf("\n%u) Looking for CSteam/CSteamServer3::OnGSClientDenyHelper jump table address ...\n",i);
      i ++;
      OPParseUserPtrn(VLVGoldSrcCLListenJMPTabHelper,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (Res)
       {
         TVAADDR CBCDRHelperJTAddr = pLoc;
         OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
         CBCDRHelperJTAddr += 16;
         if (tmpStr[1] == 0x44)
           CBCDRHelperJTAddr ++;
         OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
         CBCDRHelperJTAddr = *((uint32_t *) tmpStr);
         HL1ClassicListenGetBaseAddr(fileStream,&PEBase);

         CBCDRHelperJTAddr -= (PEBase + HDRS_SIZE);
         OPJumpToOffsetInStream(fileStream,CBCDRHelperJTAddr,&tmpStr,&rems);
         CBCDRHelperJT = (uint32_t *) tmpStr;
         printf("        Found at 0x%"PRIX64".   ",CBCDRHelperJTAddr);
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
         uint32_t tmp;
         CBCDRHelperSafeLabelAddr = pLoc;
         OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
         CBCDRHelperSafeLabelAddr += 9;
         if (tmpStr[1] == 0x44)
           CBCDRHelperSafeLabelAddr ++;
         OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
         tmp = *((uint32_t *) tmpStr);
         CBCDRHelperSafeLabelAddr += tmp + 4;
         OPParseUserPtrn(VLVGoldSrcCLListenSafeLabelValidate,TU_UseBytePatterns,&Pat);
         OPFindPtrn(fileStream,Pat,1,TLOOKUPSTATICOFFSET,TB_TRUE,CBCDRHelperSafeLabelAddr,&pLoc,&Res);
         OPFreePatternMemory(&Pat);
         if (Res)
          {
             OPJumpToOffsetInStream(fileStream,CBCDRHelperSafeLabelAddr,&tmpStr,&rems);
             CBCDRHelperSafeLabelAddr += 19;
             if (tmpStr[1] == 0x44)
              CBCDRHelperSafeLabelAddr ++;
             printf("        Found at 0x%"PRIX64".   \n",CBCDRHelperSafeLabelAddr);
             CBCDRHelperSafeLabelAddr += PEBase + HDRS_SIZE;
          }
         else
          {
             *PatchingResult = TB_FALSE;
             printf("        Not Found\n\n\n");
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
          printf("\n%u%c) Patching Steam VAC logon check ... ",i,ii);
          CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
          printf("Done\n\n\n");
          ii ++;
          if (AllowNonSteam && !eSTEAMATiON_Prep_Mode)
           {
             j = 13;
             printf("\n%u%c) Patching Steam UserID ticket verifying check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (AllowCrackedSteam)
           {
             j = 3;
             printf("\n%u%c) Patching Steam account game ownership check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (AllowCrackedSteam || AllowNonSteam)
           {
             j = 10;
             printf("\n%u%c) Patching \"Steam connection lost\" check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
             ii ++;
           }
          if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs && eSTEAMATiON_Prep_Mode)
           {
             j = 5;
             printf("\n%u%c) Patching SteamID duplication check ... ",i,ii);
             CBCDRHelperJT[j] = (uint32_t)CBCDRHelperSafeLabelAddr;
             printf("Done\n\n\n");
           }
          i ++;
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
           OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&DW4_Loc,&Res);
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
       uint64_t tmpLoc;
       printf("\n%u) Looking for client's network Class check ...\n",i);
       i ++;
       tmpErr = OPParseUserPtrn(VLVGoldSrcCLListenLANChk_Src,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
           tmpLoc = pLoc;
           printf("        Found at 0x%"PRIX64".   ",pLoc);
           OPParseUserPtrn(VLVGoldSrcCLListenLANChk_Dst,TU_UseBytePatterns,&Pat);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           tmpLoc += 34;
           if (tmpStr[17] == 0x44)
             tmpLoc += 2;
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
   if (ForceSteamWorksProtocol)
    {
       uint8_t old_proto,new_proto = 48;
       printf("\n%u) Switching protocol number to SourceWorks Protocol 48 ...\n",i);
       Res = SetEngineProtocol(fileStream,new_proto,&old_proto,fileOS);
       if (Res)
        {
          printf("        Switched from %u to %u\n\n\n",old_proto,new_proto);
        }
       else
         printf("        Failed\n\n\n");
    }
}

void HL1ClassicListenCryptMan(TSTREAMADDR_HDR const fileStream, TBOOL IsWeGonnaToCryptItBack)
{
  TSTREAMADDR_NOHDR tmpStr;
  uint64_t rems,i;
  uint8_t encprc_byte;
  char encpr_chr = 'W';
  TVAADDR f_size;
  OPJumpToOffsetInStream(fileStream,0x44,&tmpStr,&rems);
  OPGetLastOffsetInStream(fileStream,&f_size);
  f_size ++;
  f_size -= 0x44;

  for (i = 0; i < f_size; i ++)
      {
          encprc_byte = tmpStr[i];
          tmpStr[i] ^= encpr_chr;
          if (!IsWeGonnaToCryptItBack)
            encpr_chr += tmpStr[i] + 'W';
          else
            encpr_chr += encprc_byte + 'W';
      }

}

void HL1ClassicListenGetBaseAddr(TSTREAMADDR_HDR const fileStream, uint64_t *HL1LstBase)
{
   TSTREAMADDR_NOHDR tmpStr;
   uint64_t rems;
   if (!HL1LstBase)
    return;
   OPJumpToOffsetInStream(fileStream,0x44,&tmpStr,&rems);
   *HL1LstBase = (((THL1LstHDR *)tmpStr) -> ImageBase) ^ 0x49c042d1;
}
