#include "ietqw.h"
#ifdef _MSC_VER
  #pragma warning( disable : 4996 )
#endif

TBOOL IfETQWEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[200])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   uint64_t rems;
   TSTREAMADDR_NOHDR tmpStr;
   char Date[12],Time[9];
   uint8_t MajorVersion = 0,MinorVersion = 0;
   uint16_t MajorBuild = 0,MinorBuild = 0;
   strcpy(Date,"UNK 00 6666");
   strcpy(Time,"66:66:66");
   strcpy(Version,"UNKNOWN");
   if (fileOS == VUP_Win32)
    {
        OPParseUserPtrn(ETQWWinMagic,TU_UseBytePatterns,&Pat);
        OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
        OPFreePatternMemory(&Pat);
        // +16 Data
        if (Res)
         {
           TBOOL Res2;
           TSTREAMADDR_NOHDR tmpStr2;
           uint64_t Base;
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           tmpStr += 16;
           strcpy(Date,(char *)tmpStr);
           tmpStr += 12;
           strcpy(Time,(char *)tmpStr);


           OPParseUserPtrn(ETQWWinVerMagic,TU_UseHalfBytePatterns,&Pat);
           OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res2);
           OPFreePatternMemory(&Pat);
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           MinorVersion = *((unsigned char *)(tmpStr + 3));
           MajorVersion = *((unsigned char *)(tmpStr + 5));
           OPPEGetAdditionalInfo(fileStream,&Base,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
           tmpStr -= 30;
           pLoc = *((uint32_t *)tmpStr) - Base;
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr2,&rems);
           MinorBuild = *((uint16_t *)tmpStr2);
           tmpStr += 6;
           pLoc = *((uint32_t *)tmpStr) - Base;
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr2,&rems);
           MajorBuild = *((uint16_t *)tmpStr2);
           snprintf(Version,200,"ETQW v%u.%u.%u.%u (win-x86) %s %s",MajorVersion,MinorVersion,MajorBuild,MinorBuild,Date,Time);
         }
        return Res;
    }
   else
    {
        OPParseUserPtrn(ETQWLinMagic,TU_UseBytePatterns,&Pat);
        OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
        if (Res)
         {
           TBOOL Res2;
           TSTREAMADDR_NOHDR tmpStr2;
           TVAADDR pLoc2;
           uint32_t Base[2];
           uint32_t etqwVer;
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           strcpy(Time,(char *)tmpStr);
           tmpStr += 9;
           strcpy(Date,(char *)tmpStr);
           OPFreePatternMemory(&Pat);
           OPParseUserPtrn(ETQWLinVerMagic,TU_UseHalfBytePatterns,&Pat);
           OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res2);
           OPFreePatternMemory(&Pat);
           if (!Res2)
             return Res2;
           OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
           /*
           if (*((unsigned char *)(tmpStr - 2)) == 0x0)
             MinorVersion = *((unsigned char *)(tmpStr - 4));
           MajorVersion = *((unsigned char *)(tmpStr + 5));
           */
           if (tmpStr[4] == 0xB8)
            {
              MinorVersion = *((uint32_t *)(tmpStr + 5));
              MajorVersion = *((uint32_t *)(tmpStr + 14));
            }
           else
            {
              MinorVersion = 0;
              MajorVersion = *((uint32_t *)(tmpStr + 11));
            }
           OPELFGetAdditionalInfo(fileStream,NULL,NULL,NULL,NULL,NULL,Base,NULL,NULL,NULL,NULL,NULL);
           etqwVer = MajorVersion * 10 + MinorVersion;

           OPSetSearchMarkers(fileStream,0,pLoc,OP_MODIFY_END_OFFSET_MARKER);
           OPSetSearchWithMarkersState(fileStream,TB_TRUE);

           OPParseUserPtrn(ETQWLinBuildSubMagic,TU_UseBytePatterns,&Pat);
           OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc2,&Res2);
           OPFreePatternMemory(&Pat);
           if (Res2)
            {
              OPJumpToOffsetInStream(fileStream,pLoc2 + 5,&tmpStr,&rems);

              //tmpStr -= ((etqwVer == 12) ? 27 : 4);
              pLoc = *((uint32_t *)tmpStr) - (Base[0] - Base[1]);
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr2,&rems);
              MajorBuild = *((uint16_t *)tmpStr2);
            }
           else
            MajorBuild = 0;

           OPParseUserPtrn(ETQWLinBuildSubMagic,TU_UseBytePatterns,&Pat);
           OPFindPtrn(fileStream,Pat,2,TLOOKUPSAR,TB_TRUE,0,&pLoc2,&Res2);
           OPFreePatternMemory(&Pat);
           if (Res2)
            {
              OPJumpToOffsetInStream(fileStream,pLoc2 + 5,&tmpStr,&rems);

              //tmpStr -= ((etqwVer == 12) ? 29 : 9);
              pLoc = *((uint32_t *)tmpStr) - (Base[0] - Base[1]);
              OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr2,&rems);
              MinorBuild = *((uint16_t *)tmpStr2);
              snprintf(Version,200,"ETQW v%u.%u.%u.%u (linux-x86) %s %s",MajorVersion,MinorVersion,MajorBuild,MinorBuild,Date,Time);
            }
           else
            MinorBuild = 0;

           OPSetSearchMarkers(fileStream,0,0,OP_MODIFY_END_OFFSET_MARKER);
           OPSetSearchWithMarkersState(fileStream,TB_FALSE);
         }
        else
         OPFreePatternMemory(&Pat);
        return Res;
    }

}

void PatchETQWEngine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS, TBOOL PromtUser, TBOOL ClientChecks,TBOOL *PatchingResult, char Version[200])
{
   TBOOL PatchLAN = TB_TRUE;
   TBOOL PatchINET = TB_TRUE;
   TVAADDR pLoc,NewAndLan1ChkLoc;
   TBOOL Res,NewAndLan1ChkRes;
   TPTRN Pat;
   uint8_t i = 1;
   uint32_t MajorVersion,MinorVersion,etqwVer;
   *PatchingResult = TB_TRUE;

   if (PromtUser)
    {
      char Answer;
      printf("\nVUP is running in ADVANCED mode!!\nA few questions will be asked during patching process.\n");
      printf("You will need to press special answer keys(case insensitive so SHIFT does not matter):\nY = YES           N = NO         D = USE DEFAULT\n");
      printf("\nDo you want to enable non-authenticated players to join your server(Inet or LanAuth modes)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          PatchINET = TB_FALSE;
       }
      printf("\nDo you want to enable players outside your LAN to join your server(Lan mode)?  ");
      do
       Answer = getchar();
      while ((Answer != 'n') && (Answer != 'N') && (Answer != 'y') && (Answer != 'Y') && (Answer != 'd') && (Answer != 'D'));
      if ((Answer == 'n') || (Answer == 'N'))
       {
          PatchLAN = TB_FALSE;
       }
      printf("\nAll required information gatherned...\n");
    }
   printf("Performing job...\n\n");
   /* Lets check if we are running versions of ETQW prior to 1.2 in WiN or any in UNiX */
   sscanf(Version,"ETQW v%u.%u.",&MajorVersion,&MinorVersion);
   etqwVer = MajorVersion * 10 + MinorVersion;
   if (fileOS == VUP_Win32)
    {
      OPParseUserPtrn(ETQW_WiN_netLAN_vPre12_Src_1,TU_UseHalfBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&NewAndLan1ChkLoc,&NewAndLan1ChkRes);
      OPFreePatternMemory(&Pat);
    }
   if (PatchLAN)
    {


       if ((fileOS == VUP_Linux) || (etqwVer != 12))
        {
          printf("\n%u) Looking for \"netLan server\" nag logging check #1...\n",i);
          i ++;
          if (fileOS == VUP_Linux)
           {
             OPParseUserPtrn(ETQW_NiX_netLAN_Src_1,TU_UseHalfBytePatterns,&Pat);
             OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
             OPFreePatternMemory(&Pat);
           }
          else
           {
             OPParseUserPtrn(ETQW_WiN_netLAN_vPre12_Src_1,TU_UseHalfBytePatterns,&Pat);
             Res = NewAndLan1ChkRes;
             pLoc = NewAndLan1ChkLoc;
           }
          if (Res)
           {
             printf("        Found at 0x%"PRIX64".   ",pLoc);
             printf("Patching ...  ");
             if (fileOS == VUP_Linux)
              {
                TSTREAMADDR_NOHDR tmpStr;
                uint64_t rems;
                OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                if (*tmpStr < 0x10)
                 {
                   OPJumpToOffsetInStream(fileStream,pLoc + 19,&tmpStr,&rems);
                   if (*(tmpStr + 1) == 0x84)
                    {
                       OPParseUserPtrn(ETQW_NiX_netLAN_Dst_1_b,TU_UseBytePatterns,&Pat);
                       OPWritePtrn(fileStream,pLoc + 19,Pat);
                    }
                   else
                     if (*(tmpStr + 1) == 0x85)
                      {
                         OPParseUserPtrn(ETQW_NiX_netLAN_Dst_1_a,TU_UseBytePatterns,&Pat);
                         OPWritePtrn(fileStream,pLoc + 19,Pat);
                      }

                 }
                else
                 {
                   OPJumpToOffsetInStream(fileStream,pLoc + 16,&tmpStr,&rems);
                   if (*(tmpStr + 1) == 0x84)
                    {
                       OPParseUserPtrn(ETQW_NiX_netLAN_Dst_1_b,TU_UseBytePatterns,&Pat);
                       OPWritePtrn(fileStream,pLoc + 16,Pat);
                    }
                   else
                     if (*(tmpStr + 1) == 0x85)
                      {
                         OPParseUserPtrn(ETQW_NiX_netLAN_Dst_1_a,TU_UseBytePatterns,&Pat);
                         OPWritePtrn(fileStream,pLoc + 16,Pat);
                      }
                 }
              }
             else
              {
                OPParseUserPtrn(ETQW_WiN_netLAN_vPre12_Dst_1,TU_UseHalfBytePatterns,&Pat);
                OPWritePtrn(fileStream,pLoc,Pat);
              }
             OPFreePatternMemory(&Pat);
             printf("Done\n\n\n");
           }
          else
           {
             *PatchingResult = TB_FALSE;
             printf("        Not Found\n\n\n");
           }
          if (fileOS == VUP_Linux)
            OPParseUserPtrn(ETQW_NiX_netLAN_Src_2,TU_UseHalfBytePatterns,&Pat);
          else
            OPParseUserPtrn(ETQW_WiN_netLAN_vPre12_Src_2,TU_UseHalfBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
             printf("\n%u) Looking for \"netLan server\" nag logging check #2...\n",i);
             i ++;
             if (fileOS == VUP_Linux)
               OPParseUserPtrn(ETQW_NiX_netLAN_Dst_2,TU_UseBytePatterns,&Pat);
             else
               OPParseUserPtrn(ETQW_Win_netLAN_vPre12_Dst_2,TU_UseBytePatterns,&Pat);
             printf("        Found at 0x%"PRIX64".   ",pLoc);
             printf("Patching ...  ");
             OPWritePtrn(fileStream,pLoc,Pat);
             OPFreePatternMemory(&Pat);
             printf("Done\n\n\n");
           }
          else
           {
              if (fileOS == VUP_Linux)
               {
                  OPParseUserPtrn(ETQW_NiX_netLAN_Src_2_b,TU_UseHalfBytePatterns,&Pat);
                  OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
                  OPFreePatternMemory(&Pat);
                  if (Res)
                   {
                     TSTREAMADDR_NOHDR tmpStr;
                     uint64_t rems;
                     OPJumpToOffsetInStream(fileStream,pLoc - 2,&tmpStr,&rems);
                     if ((*tmpStr) == 0x75)
                      {
                         printf("\n%u) Looking for \"netLan server\" nag logging check #2...\n",i);
                         i ++;
                         printf("        Found at 0x%"PRIX64".   ",pLoc - 2);
                         printf("Patching ...  ");
                         *tmpStr = 0xEB;
                         printf("Done\n\n\n");
                      }
                   }
               }
           }

        }
       else
        {
          printf("\n%u) Looking for \"netLan server\" nag logging check...\n",i);
          i ++;
          OPParseUserPtrn(ETQW_WiN_netLAN_v12_Src,TU_UseBytePatterns,&Pat);
          OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
          OPFreePatternMemory(&Pat);
          if (Res)
           {
             OPParseUserPtrn(ETQW_Win_netLAN_v12_Dst,TU_UseBytePatterns,&Pat);
             printf("        Found at 0x%"PRIX64".   ",pLoc);
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
   if (PatchLAN || PatchINET)
    {
       TSTREAMADDR_NOHDR tmpStr;
       uint64_t rems;
       if (fileOS == VUP_Win32)
        {
          if (NewAndLan1ChkRes && (etqwVer >= 14))
           {
             printf("\n%u) Looking for new hidden Authentication check...  \n\n",i);
             i ++;
             OPJumpToOffsetInStream(fileStream,NewAndLan1ChkLoc - 13,&tmpStr,&rems);
             if ((tmpStr[0] == 0x0F) && ((tmpStr[1] >> 4) == 8))
              {
                printf("        Found at 0x%"PRIX64".   ",NewAndLan1ChkLoc - 13);
                printf("Patching ...  ");
                tmpStr[0] = 0x90;
                tmpStr[1] = 0xE9;
                printf("Done\n\n\n");
              }
             else
              {
                *PatchingResult = TB_FALSE;
                printf("        Not Found\n\n\n");
              }
           }
        }
       printf("\n%u) Looking for client connection check...  \n\n",i);
       i ++;
       if (fileOS == VUP_Linux)
         OPParseUserPtrn(ETQW_NiX_Connect_Src,TU_UseHalfBytePatterns,&Pat);
       else
         OPParseUserPtrn(ETQW_WiN_Connect_Src,TU_UseHalfBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
          TBOOL extra5B;
          TVAADDR pLoc2 = pLoc;
          printf("        Found at 0x%"PRIX64".\n",pLoc);
          if (PatchLAN)
           {
             printf("Patching client's IP class check...  ");
             if (fileOS == VUP_Linux)
               OPParseUserPtrn(ETQW_NiX_Connect_Dst_LAN,TU_UseBytePatterns,&Pat);
             else
               {
                   TPTRN Pat2;
                   OPParseUserPtrn(ETQW_WiN_Connect_Dst_Extra5,TU_UseBytePatterns,&Pat2);
                   OPFindPtrn(fileStream,Pat2,0,TLOOKUPSTATICOFFSET,TB_FALSE,pLoc + 10,NULL,&extra5B);
                   OPFreePatternMemory(&Pat2);
                   OPParseUserPtrn(ETQW_WiN_Connect_Dst_LAN,TU_UseBytePatterns,&Pat);
                   if (extra5B)
                     pLoc2 += 5;
               }
             OPWritePtrn(fileStream,pLoc2,Pat);
             OPFreePatternMemory(&Pat);
             printf("Done\n\n");
           }
          if (PatchINET)
           {
             printf("Patching client authentication check...  ");
             if (fileOS == VUP_Linux)
               OPParseUserPtrn(ETQW_NiX_Connect_Dst_INET,TU_UseBytePatterns,&Pat);
             else
               OPParseUserPtrn(ETQW_WiN_Connect_Dst_INET,TU_UseBytePatterns,&Pat);
             OPWritePtrn(fileStream,pLoc,Pat);
             OPFreePatternMemory(&Pat);
             printf("Done\n\n\n");
           }
        }
       else
        {
          *PatchingResult = TB_FALSE;
          printf("Not Found\n\n\n");
        }
    }
   printf("\n\nAll patching jobs completed.\n");
}

