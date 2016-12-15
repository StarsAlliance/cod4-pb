#include "vlvshared.h"
#ifdef _MSC_VER
  #pragma warning( disable : 4996 )
#endif

extern uint8_t Force_Game_Type;
extern uint8_t Src2K7U1NewProtocolCalcaulatingSystem;

extern TBOOL eSTEAMATiON_Prep_Mode, eSTEAMATiON_Prep_DropCrackedSteam, eSTEAMATiON_Prep_AllowDuplicateSteamIDs,
             PatchMasterServerUpdateNotificationProtection, PatchClientConnectionLostForExtractedSteamProtection,LeavePENDINGIDInPlace;

TBOOL GetValveEngineInfo(TSTREAMADDR_HDR const fileStream,
                            int32_t *vlvEngineBuild,
                            uint8_t *vlvProtocol,
                            char vlvVersion[20],
                            char vlvTime[9],
                            char vlvDate[40],
                            const TVUP_OSType fileOS)
{
  TVAADDR pLoc;
  TBOOL Res,Res2;
  TPTRN Pat;
  uint64_t rems;
  TOPError tmpErr;
  TSTREAMADDR_NOHDR tmpStr;
  if (!Force_Game_Type)
   {
     OPParseUserPtrn(VLVMagicN1,TU_UseBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);

     OPParseUserPtrn(VLVMagicNewVersioningScheme,TU_UseBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res2);
     if (Res2 && !Res)
       Src2K7U1NewProtocolCalcaulatingSystem = 1;
     OPFreePatternMemory(&Pat);
     if (!Res && !Res2)
      {
        return TB_FALSE;
      }
   }
  //printf("DEBUG: VALVE's Magic 1 found\n");
  OPParseUserPtrn(VLVMagicN2,TU_UseBytePatterns,&Pat);
  OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
  OPFreePatternMemory(&Pat);
  if (!Res)
   {
     return TB_FALSE;
   }
  //printf("DEBUG: VALVE's Magic 2 found\n");
  OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
  tmpStr += 11;
  strncpy(vlvTime,(char *)tmpStr,8);
  vlvTime[8] = '\0';
  tmpStr += 9;
  strncpy(vlvDate,(char *)tmpStr,11);
  vlvDate[11] = '\0';

  strcpy(vlvVersion,"EXTERNAL(Look in ModName\\steam.inf)");
  /*
  tmpStr -= 21;
  while (*((char *)tmpStr - 1) != '\0')
   tmpStr --;
  if (*((char *)tmpStr) != '\0')
    strcpy(vlvVersion,(char *)tmpStr);
  else
   {
      tmpErr = OPParseUserPtrn(VLVDateMagicHLDSAndAllUniX,TU_UseBytePatterns,&Pat);
      OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
      OPFreePatternMemory(&Pat);
      if (!Res)
       strcpy(vlvVersion,"Unknown Version");
      else
       {
          OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
          while (*((char *)tmpStr - 1) != '\0')
           tmpStr --;
          tmpStr -= 2;
          while (*((char *)tmpStr - 1) != '\0')
           tmpStr --;
          if ((tmpStr[0] >= '0') && (tmpStr[0] <= '9'))
            strcpy(vlvVersion,(char *)tmpStr);
          else
           strcpy(vlvVersion,"EXTERNAL(Look in ModName\steam.inf)");
       }
   }
  */
  *vlvEngineBuild = GetValveBuildFromDate(vlvDate);
  /* Got it */

  if (fileOS != VUP_Linux)
   {
     tmpErr = OPParseUserPtrn(VLVProtoMagic_Win,TU_UseHalfBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
      {
        OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
        *vlvProtocol = *((uint8_t *)(tmpStr + 11));
      }
   }
  else
   {
     tmpErr = OPParseUserPtrn(VLVProtoMagic_UniX,TU_UseHalfBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
      {
        uint8_t AdditionalASMSubtyping = 0;
        OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
        if(tmpStr[11] == 0x89)
          AdditionalASMSubtyping = 1;

        if (tmpStr[14] == 0xB9)
         *vlvProtocol = *((uint8_t *)tmpStr + 15 + AdditionalASMSubtyping);
        else
         *vlvProtocol = *((uint8_t *)tmpStr + 18 + AdditionalASMSubtyping);
      }
     else
      {
        OPParseUserPtrn(VLVProtoMagic_UniX_2,TU_UseHalfBytePatterns,&Pat);
        OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
        OPFreePatternMemory(&Pat);
        if (Res)
         {
            OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
            switch (*(tmpStr + 3))
             {
               case 0xB9:
                          *vlvProtocol = (uint8_t)(*((uint32_t *)(tmpStr + 18)));
                          break;
               case 0x68:
                          *vlvProtocol = *((uint8_t *)(tmpStr + 14));
                          break;
               case 0x89:
                          *vlvProtocol = *((uint8_t *)(tmpStr + 26));
                          break;
             }
            /*
            if (*(tmpStr + 3) == 0xB9)
              *vlvProtocol = (uint8_t)(*((uint32_t *)(tmpStr + 18)));
            else
              *vlvProtocol = *((uint8_t *)(tmpStr + 14));
            */
         }
        else
         {
            OPParseUserPtrn(VLVProtoMagic_UniX_3,TU_UseHalfBytePatterns,&Pat);
            OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
            OPFreePatternMemory(&Pat);
            if (Res)
             {
                OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                switch (*(tmpStr + 6))
                 {
                   case 0x50:
                              *vlvProtocol = *((uint8_t *)(tmpStr + 15));
                              break;
                   case 0xFF:
                              *vlvProtocol = *((uint8_t *)(tmpStr + 13));
                              break;
                 }
             }
            else
             {
                OPParseUserPtrn(VLVProtoMagic_UniX_4,TU_UseHalfBytePatterns,&Pat);
                OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
                OPFreePatternMemory(&Pat);
                if (Res)
                 {
                    OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                    *vlvProtocol = (uint8_t)(*((uint32_t *)(tmpStr + 33)));
                 }
             }
         }
      }
   }
  //printf("DBG:Res = %s , Protocol = %i , Engine %i\n",Res ? "TRUE" : "FALSE",vlvProtocol,*vlvEngineBuild);
  if (!Res)
    *vlvProtocol = 0;
  return TB_TRUE;
}

TBOOL SetEngineProtocol(TSTREAMADDR_HDR const fileStream, uint8_t NewProtocol, uint8_t *OldProtocol, const TVUP_OSType fileOS)
{
  TVAADDR pLoc;
  TBOOL Res;
  TPTRN Pat;
  uint64_t rems;
  TOPError tmpErr;
  TSTREAMADDR_NOHDR tmpStr;
  if (fileOS != VUP_Linux)
   {
     tmpErr = OPParseUserPtrn(VLVProtoMagic_Win,TU_UseHalfBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
      {
        OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
        if (OldProtocol)
         *OldProtocol = *((uint8_t *)(tmpStr + 11));
        *((uint8_t *)(tmpStr + 11)) = NewProtocol;
      }
   }
  else
   {
     tmpErr = OPParseUserPtrn(VLVProtoMagic_UniX,TU_UseHalfBytePatterns,&Pat);
     OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
     OPFreePatternMemory(&Pat);
     if (Res)
      {
        OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
        if (tmpStr[15] == 0xB9)
         {
           if (OldProtocol)
            *OldProtocol = *((uint8_t *)tmpStr + 16);
           *((uint8_t *)tmpStr + 16) = NewProtocol;
         }
        else
         {
           if (OldProtocol)
             *OldProtocol = *((uint8_t *)tmpStr + 19);
           *((uint8_t *)tmpStr + 19) = NewProtocol;
         }
      }
     else
      {
        OPParseUserPtrn(VLVProtoMagic_UniX_2,TU_UseHalfBytePatterns,&Pat);
        OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
        OPFreePatternMemory(&Pat);
        if (Res)
         {
            OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
            switch (*(tmpStr + 3))
             {
               case 0xB9:
                          if (OldProtocol)
                            *OldProtocol = (uint8_t)(*((uint32_t *)(tmpStr + 18)));
                          *((uint32_t *)(tmpStr + 18)) = (uint32_t)NewProtocol;
                          break;
               case 0x68:
                          if (OldProtocol)
                            *OldProtocol = *((uint8_t *)(tmpStr + 14));
                          *((uint8_t *)(tmpStr + 14)) = NewProtocol;
                          break;
               case 0x89:
                          if (OldProtocol)
                            *OldProtocol = *((uint8_t *)(tmpStr + 26));
                          *((uint8_t *)(tmpStr + 26)) = NewProtocol;
                          break;
             }
            /*
            if (*(tmpStr + 3) == 0xB9)
              *vlvProtocol = (uint8_t)(*((uint32_t *)(tmpStr + 18)));
            else
              *vlvProtocol = *((uint8_t *)(tmpStr + 14));
            */
         }
        else
         {
            OPParseUserPtrn(VLVProtoMagic_UniX_3,TU_UseHalfBytePatterns,&Pat);
            OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
            OPFreePatternMemory(&Pat);
            if (Res)
             {
                OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
                switch (*(tmpStr + 6))
                 {
                   case 0x50:
                              if (OldProtocol)
                                *OldProtocol = *((uint8_t *)(tmpStr + 15));
                              *((uint8_t *)(tmpStr + 15)) = NewProtocol;
                              break;
                   case 0xFF:
                              if (OldProtocol)
                                *OldProtocol = *((uint8_t *)(tmpStr + 13));
                              *((uint8_t *)(tmpStr + 13)) = NewProtocol;
                              break;
                 }
             }
         }
      }
   }
  if (!Res)
    return TB_FALSE;
  return TB_TRUE;
}

int32_t GetValveBuildFromDate(char vlvDate[12])
{
    uint32_t DaysCounter = 0;
    char Months[][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char DaysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    char vlvMonth[4];
    uint8_t vlvMonthDay;
    uint16_t vlvYear;
    int16_t tmpYear1;
    int32_t Res;
    float tmpYear2;
    char tmp[10];
    uint8_t i = 0;
    strncpy(tmp,vlvDate,3);
    *(tmp + 3) = '\0';
    strncpy(vlvMonth,tmp,3);
    strncpy(tmp,vlvDate + 4,2);
    *(tmp + 2) = '\0';
    vlvMonthDay = strtoul(tmp,NULL,0);
    strncpy(tmp,vlvDate + 7,4);
    *(tmp + 4) = '\0';
    vlvYear = strtoul(tmp,NULL,0);
    do
     {
       if (!strncmp(Months[i],vlvMonth,3))
        {
          DaysCounter += vlvMonthDay - 1;
          break;
        }
       DaysCounter += DaysInMonth[i];
       i ++;
     }
    while (i < 12);
    tmpYear1 = vlvYear - 1900;
    tmpYear2 = DaysCounter - ((uint32_t)((tmpYear1 - 1) * (-365.25))) ;
    Res = tmpYear2;
    tmpYear1 &= 0x80000003;
    if (tmpYear2 < 0)
     {
       tmpYear1 --;
       tmpYear1 |= 0xFFFFFFFC;
       tmpYear1 ++;
     }
    if ((tmpYear1 == 0) && (i > 1))
     {
       Res ++;
     }
    Res += 0xFFFF7465;
    return Res;
}


TBOOL IfValveSteamClient(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   strcpy(Version,"UNKNOWN");
   OPParseUserPtrn(VLVSteamClient_Magic,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
     sprintf(Version,"Valve SteamClient Library (%s Build)",(fileOS == VUP_Win32) ? "WIN32" : "LINUX");
   return Res;
}

void PatchValveSteamClient(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[120])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   *PatchingResult = TB_TRUE;
   TSTREAMADDR_NOHDR tmpStr;
   uint8_t i = 1;
   uint64_t rems;


eSTEAMATiON_Prep_Advanced_Skip:
   printf("Performing job...\n\n");
   if (eSTEAMATiON_Prep_Mode)
    {
      printf("\n%u) ",i);
      i ++;
    }
   else
     printf("\n");
   printf("Looking for SteamStartValidatingUserIDTicket validation errors logging ...\n");
   OPParseUserPtrn(VLVSteamClientLog_Src,TU_UseHalfBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       if (fileOS == VUP_Linux)
         OPParseUserPtrn(VLVSteamClientLog_Dst,TU_UseBytePatterns,&Pat);
       else
         OPParseUserPtrn(VLVSteamClientLog_Dst_WiN,TU_UseBytePatterns,&Pat);
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
       switch (*tmpStr)
        {
           case 0x52:
                      if (fileOS == VUP_Win32)
                       {
                          OPWritePtrn(fileStream,pLoc + 20,Pat);
                       }
                      else
                       *PatchingResult = TB_FALSE;
                      break;
           case 0x89:
                      OPWritePtrn(fileStream,pLoc + 42,Pat);
                      break;
           case 0x90:
                      OPWritePtrn(fileStream,pLoc + 26,Pat);
                      break;
           case 0x83:
                      OPWritePtrn(fileStream,pLoc + 9,Pat);
                      break;
        }
       OPFreePatternMemory(&Pat);
       printf("Done\n\n\n");
    }
   else
    {
       *PatchingResult = TB_FALSE;
       printf("        Not Found\n\n\n");
    }
   if (eSTEAMATiON_Prep_Mode)
    {
       printf("\n%u) Looking for Valve's Steam2 AUTH module name reference ...\n",i);
       if (fileOS == VUP_Linux)
         OPParseUserPtrn(VLVSteamClient_Steam2LibraryReference_NiX,TU_UseBytePatterns,&Pat);
       else
         OPParseUserPtrn(VLVSteamClient_Steam2LibraryReference_Win,TU_UseBytePatterns,&Pat);
       OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
       OPFreePatternMemory(&Pat);
       if (Res)
        {
            if (fileOS == VUP_Linux)
              OPParseUserPtrn(VLVSteamClient_Steam2LibraryReference_NiX_Dst,TU_UseBytePatterns,&Pat);
            else
              OPParseUserPtrn(VLVSteamClient_Steam2LibraryReference_Win_Dst,TU_UseBytePatterns,&Pat);
            printf("        Found at 0x%"PRIX64".   ",pLoc);
            printf("Patching ...  ");
            OPWritePtrn(fileStream,pLoc,Pat);
        }
       else
        {
          *PatchingResult = TB_FALSE;
          printf("        Not Found\n\n\n");
        }
    }
}

TBOOL IfValveSteamClient3(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   strcpy(Version,"UNKNOWN");
   OPParseUserPtrn(VLVSteamClient3_Magic,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
     sprintf(Version,"Valve SteamClient3 Library (%s Build)",(fileOS == VUP_Win32) ? "WIN32" : "LINUX");
   return Res;
}

void PatchValveSteamClient3(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[120])
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;
   *PatchingResult = TB_TRUE;
   TSTREAMADDR_NOHDR tmpStr;
   uint64_t rems;
eSTEAMATiON_Prep_Advanced_Skip:
   printf("Performing job...\n\n");
   printf("\n\n\nLooking for CSteamEngine::BIsTicketSignatureValid vulnerable code chunk ...\n");
   OPParseUserPtrn(VLVSteamClient_Steam3TicketRSAVerification,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
       printf("        Found at 0x%"PRIX64".   ",pLoc);
       printf("Patching ...  ");
       OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
       OPParseUserPtrn(VLVSteamClient_Steam3TicketRSAVerification_dst,TU_UseBytePatterns,&Pat);
       if (fileOS == VUP_Win32)
        {
           switch(tmpStr[0])
            {
               case 0xE8:
                         OPWritePtrn(fileStream,pLoc + 36,Pat);
                         break;
               case 0xB9:
                         OPWritePtrn(fileStream,pLoc + 34,Pat);
                         break;
            }
        }
       else
        {
           switch(tmpStr[0])
            {
               case 0x8B:
                         OPWritePtrn(fileStream,pLoc + 55,Pat);
                         break;
               case 0x89:
               case 0x31:
                         OPWritePtrn(fileStream,pLoc + 69,Pat);
                         break;
            }
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

TBOOL IfValveServerLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[120])
{
   TVAADDR pLoc;
   TBOOL Res1,Res2;
   TPTRN Pat;
   strcpy(Version,"UNKNOWN");
   OPParseUserPtrn(VLVServerLibMagic_N1,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res1);
   OPFreePatternMemory(&Pat);
   OPParseUserPtrn(VLVServerLibMagic_N2,TU_UseBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_TRUE,0,&pLoc,&Res2);
   OPFreePatternMemory(&Pat);
   if (Res1 && Res2)
     sprintf(Version,"     Valve Server Library (%s Build)",(fileOS == VUP_Win32) ? "WIN32" : "LINUX");
   return (Res1 && Res2);
}

void PatchValveServerLibrary(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL *PatchingResult,uint8_t NewPlayersLimit)
{
   TVAADDR pLoc;
   TBOOL Res;
   TPTRN Pat;

   TSTREAMADDR_NOHDR tmpStr;
   uint64_t rems;
   *PatchingResult = TB_TRUE;
eSTEAMATiON_Prep_Advanced_Skip:
   printf("Performing job...\n\n");
   printf("\n\nLooking for players limit in CServerGameClients::GetPlayerLimits function ...\n");
   OPParseUserPtrn(VLVPlayerLimitsServerLib,TU_UseHalfBytePatterns,&Pat);
   OPFindPtrn(fileStream,Pat,1,TLOOKUPSAR,TB_FALSE,0,&pLoc,&Res);
   OPFreePatternMemory(&Pat);
   if (Res)
    {
      printf("        Found at 0x%"PRIX64".\n",pLoc);
      OPJumpToOffsetInStream(fileStream,pLoc,&tmpStr,&rems);
      if (tmpStr[4] == 0x8B)
       {
         printf("Current max players limits are: %u\\%u\n",tmpStr[20],tmpStr[26]);
         printf("Patching max players limits to %i ...  ",NewPlayersLimit);
         tmpStr[20] = NewPlayersLimit;
         tmpStr[26] = NewPlayersLimit;
       }
      else
       {
         printf("Current max players limits are: %u\\%u\n",tmpStr[16],tmpStr[26]);
         printf("Patching max players limits to %i ...  ",NewPlayersLimit);
         tmpStr[16] = NewPlayersLimit;
         tmpStr[26] = NewPlayersLimit;
       }
      printf("Done\n\n\n");
    }
   else
    {
      *PatchingResult = TB_FALSE;
      printf("        Not Found\n\n\n");
    }
}

