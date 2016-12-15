#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if  (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "vhl1.h"
#include "vhl1lstsrv.h"
#include "vgoldsrc.h"
#include "vsource.h"
#include "vsource2007.h"
#include "vsource2007U1.h"
#include "ietqw.h"
#include "cod4.h"
#include "ut3.h"
#include "ue25.h"
#include "vlvsteamui.h"
#include "vlvgameui.h"
#include "vupshared.h"


#ifdef _MSC_VER
  #pragma warning( disable : 4996 )
#endif

#define VUP_VERSION "v2.8"
#define VUP_RDATE "26.04.2010 08:27 PM"

typedef TBOOL (*TVUPGAMEDETECTPROC)(TSTREAMADDR_HDR const,const TVUP_OSType,char [200]);
typedef void (*TVUPGAMEPATCHPROC)(TSTREAMADDR_HDR const,const TVUP_OSType,TBOOL,TBOOL,TBOOL *,char [200]);
typedef void (*TVUPVALVEPLLIMITSPATCHPROC)(TSTREAMADDR_HDR const,const TVUP_OSType,TBOOL,TBOOL *,uint8_t);



#define VUP_SUPPORTED_GAMES_COUNT 14

#define VUP_SUPPORTED_VALVE_PLAYER_LIMIT_FILES_COUNT 1

#define VUP_MAXFILES_TO_PATCH_AT_ONCE 5

extern uint8_t Src2K7U1NewProtocolCalcaulatingSystem;


TVUPGAMEDETECTPROC GameDetectorsList[VUP_SUPPORTED_GAMES_COUNT] = {IfETQWEngine,
                                                                   IfCOD4Engine,
                                                                   IfSrc2K7Engine,
                                                                   IfSrc2K7U1Engine,
                                                                   IfSrcEngine,
                                                                   IfHL1LstSrvLib,
                                                                   IfGoldSrcEngine,
                                                                   IfUT3Engine,
                                                                   IfUE25Engine,
                                                                   IfValveSteamClient,
                                                                   IfValveSteamClient3,
                                                                   IfSteamUILib,
                                                                   IfGameUILib,
                                                                   IfValveServerLibrary};

TVUPGAMEPATCHPROC  GamePatchersList[VUP_SUPPORTED_GAMES_COUNT - VUP_SUPPORTED_VALVE_PLAYER_LIMIT_FILES_COUNT] = {PatchETQWEngine,
                                                                                                                 PatchCOD4Engine,
                                                                                                                 PatchSrc2K7Engine,
                                                                                                                 PatchSrc2K7U1Engine,
                                                                                                                 PatchSrcEngine,
                                                                                                                 PatchHL1LstSrvLib,
                                                                                                                 PatchGoldSrcEngine,
                                                                                                                 PatchUT3Engine,
                                                                                                                 PatchUE25Engine,
                                                                                                                 PatchValveSteamClient,
                                                                                                                 PatchValveSteamClient3,
                                                                                                                 PatchSteamUILib,
                                                                                                                 PatchGameUILib};

TVUPVALVEPLLIMITSPATCHPROC ValvePlayersLimitsPatchersList[VUP_SUPPORTED_VALVE_PLAYER_LIMIT_FILES_COUNT] = {PatchValveServerLibrary};


TBOOL PromtUser = TB_FALSE, PatchClientProtections = TB_FALSE, UnpackersLoadersSwitch = TB_FALSE, PatchMasterServerUpdateNotificationProtection = TB_FALSE,
      PatchClientConnectionLostForExtractedSteamProtection = TB_FALSE, LeavePENDINGIDInPlace = TB_FALSE,
      eSTEAMATiON_Prep_Mode = TB_FALSE,eSTEAMATiON_Prep_DropCrackedSteam = TB_FALSE, eSTEAMATiON_Prep_AllowDuplicateSteamIDs = TB_FALSE, Source2007U2NoSteamLogonDisable = TB_FALSE;
char *srcF[VUP_MAXFILES_TO_PATCH_AT_ONCE],*dstF[VUP_MAXFILES_TO_PATCH_AT_ONCE];
uint8_t FilesCounterSrc = 0,FilesCounterDst = 0,SuccessfullyPatched = 0,FailedOnPatching = 0,Force_Game_Type = 0;
TSTREAMADDR_HDR fStr;

void VUPSleep(uint16_t Secs);

TBOOL ParseArguments(int argc,char ** argv);

int main(int argc,char **argv)
{
    TBININFO fInfo;
    TOPError intError;
    TVUP_OSType fOS;
    TBOOL DetectionHelper = TB_FALSE;
    uint8_t i,j;
    char ver[200];
    printf("Welcome to ViTYAN\'s Open Universal GameServer Patch(OpenUGSP) "VUP_VERSION"\n\n");
    printf("Currently supported game types: id Software Enemy Territory - Quake Wars,\
                                    \n                                Valve Source 2007 Original (Protocols 9-14)(TF2/DOD:S),\
                                    \n                                Valve Source 2007 U1 (Protocol 36)(L4D/L4D2),\
                                    \n                                Valve Source(HL2:DM/CSS),\
                                    \n                                Valve GoldSrc Classic Listen Engines,\
                                    \n                                Valve SteamWorks GoldSrc(CS 1.6/TFC/OP/DOD1/HL1/HL:DM),\
                                    \n                                Valve SteamClient,SteamUI,GameUI and Server Libs,\
                                    \n                                Infinity Ward Call Of Duty 4,\
                                    \n                                Epic Games Unreal Tournament 3,\
                                    \n                                Unreal Engine 2.5 Based Games[LIMITED],\
                                    \n                                eSTEAMATiON Preparation mode.\n\n");
    printf("Release date: "VUP_RDATE"\n");
    printf("Know who holds the power...\n\n");
    for (i = 0;i < VUP_MAXFILES_TO_PATCH_AT_ONCE; i ++)
     {
        srcF[i] = NULL;
        dstF[i] = NULL;
     }
    if (!ParseArguments(argc,argv))
     {
        printf("ERROR:Invalid/Insufficient/Too Many arguments passed...\n");
        printf("Here is a list of currently supported options and correct cmd format:\n");
        printf("ugsp[_i686 | _amd | [86 | 64].exe] [-advanced] [-patch-client-checks] [-force-game-type=GAME_TYPE_TO_ENFORCE]   [-autoquit] [-o output_file_1] ..\n   [-o output_file_n] file_to_patch_1 .. file_to_patch_n");
        printf("\n\nOptions:\n\n-advanced: Enables advanced mode which will enable asking advanced questions during patching process\n\n");
        printf("-patch-client-checks: Enables patching specific client-side protections(Valve Source 2007 Engines Only)\n\n");
        printf("-patch-mastersrv-updnotify: Enables patching of \"Master request restart.Update your server\" messages\n\n");
        printf("-patch-client-timed-out: Enables patching of \"Client timed out\" protection which get activated with Unpacked client+Steam clients on Source 2007 engine\n\n");
        printf("-leave-pendingid-intact: If Enabled clients will join with initial STEAM_ID_PENDING ID, otherwise they will  join with initial STEAM_666:88:666 ID.\n\n");
        printf("-esteamation-prep: Enables eSTEAMATiON Preparation operation mode for Valve's engines. Ovverides even -advanced switch\n\n");
        printf("-esteamation-no-cracked-steam: Will dont patch protection of Cracked Steam clients in eSTEAMATiON prep mode coz they circuvment the id system\n\n");
        printf("-esteamation-allow-duplicate-ids: Will patch additional check to allow clients with same SteamID's to joint to the server at the same time\n\n");
        printf("-enable-additional-no-steam-logon-patch: Will apply a third \"No Steam logon\" patch. Should be used in latest versions of L4D2\n\n");
        printf("-autoquit: UGSP will not wait for users keystroke in end of operations and will close aumatically\n\n");
        printf("-o FNAME: Instructs UGSP to store patched data in file different from source file\n\n");
        printf("-force-game-type=GAME_TYPE_TO_ENFORCE: Disables UGSP's autodetection mechanism\n                             and enforces UGSP to ASSUME specific game type\n\n");
        printf("Game types for force-game operation mode:\n");
        printf("etqw - id Software Enemy Territory - Quake Wars\n\ncod4 - Infinity Ward Call Of Duty 4\n\nvlvsrc2k7 - Valve Source 2007(TF2)\n\nvlvsrc - Valve Source(HL2:DM/CSS)\n\nvlvhl1classic - Valve GoldSrc Classic Listen server\n\nvlvhl1sw - Valve SteamWorks GoldSrc(All)\n\nut3 - Epic Games Unreal Tournament 3\n\nvlvsteamclient - Valve SteamClient(Linux)\n\nvlvserverlib - Valve Server Library\n\n\n\n");
        if (!UnpackersLoadersSwitch)
          getchar();
        return 1;
     }

    printf("Preparing to handle %u file%s... Output to different destinations(-o) is %s\n\n",FilesCounterSrc,(FilesCounterSrc > 1) ? "s" : "",(FilesCounterDst > 0) ? "ENABLED" : "DISABLED");

    for (j = 0;j < FilesCounterSrc;j ++)
     {
       if (FilesCounterSrc > 1)
        {
          int timer;
          printf("PREPARING TO LOAD FILE #%u From %u FILES TOTAL...",j + 1,FilesCounterSrc);
          for (timer = 1; timer <= 5; timer ++)
           {
             VUPSleep(1);
             printf(" %i-sec",timer);
           }
          VUPSleep(1);
          printf("\n\n");
          Src2K7U1NewProtocolCalcaulatingSystem = 0;
        }
         //////////////////////////////////////////////////////////
         ////////////// Let's load our file into memory ///////////
         //////////////////////////////////////////////////////////
       printf("Loading file %s into memory...  ",srcF[j]);
       intError = OPLoadFile(srcF[j],&fStr);
       if (intError)
        {
          printf("FAILED (Error:%u)\n",intError);
          return 1;
        }
       printf("SUCCEEDED\n");
       printf("Analyzing file format ... ");
       intError = OPGetBinaryInfo(fStr,&fInfo);
       if (intError)
        {
            // Lets check first if we load HL1 Classic encrypted Listen server engine
            TSTREAMADDR_NOHDR tmpStr;
            uint64_t rems;
            OPJumpToOffsetInStream(fStr,0x40,&tmpStr,&rems);
            if (((uint32_t *)tmpStr)[0] == 0x12345678)
             {
               fOS = VUP_Win32_HL1Classic;
               printf("SUCCEEDED\n");
             }
            else
             {
               printf("FAILED(Error:%u)\n",intError);
               return 1;
             }
        }
       else
        {
          printf("SUCCEEDED\n");
          switch (fInfo.BinaryFormat) {
             case TBINF_PE_WIN: fOS = VUP_Win32;
                                break;
             case TBINF_ELF_UNIX: fOS = VUP_Linux;
                                  break;
             default:
                     {
                       TSTREAMADDR_NOHDR tmpStr;
                       uint64_t rems;
                       OPJumpToOffsetInStream(fStr,0x40,&tmpStr,&rems);
                       if (((uint32_t *)tmpStr)[0] == 0x12345678)
                        {
                          fOS = VUP_Win32_HL1Classic;
                        }
                       else
                        {
                          printf("Unknown file format. Operation halted\n");
                          return 1;
                        }
                     }
          }
        }
       if (fOS == VUP_Win32)
         printf("Destination Operating System: Microsoft Windows\n\n\n");
       else
        {
          if (fOS == VUP_Win32_HL1Classic)
           {
             printf("Destination Operating System: Microsoft Windows(HL1 Classic Listen Encryption Challenge FOUND)\n\n\n");
             printf("Please be patient. Decryption in process ... ");
             HL1ClassicListenCryptMan(fStr,TB_FALSE);
             printf("FINISHED\n\n");
           }
          else
           printf("Destination Operating System: Unix-Like Operating System\n\n\n");
        }
       printf("Trying to detect game type from binary:\n\n");

       //////////////////////////////////////////////////////////
       //////////////// Recognize game type ... /////////////////
       //////////////////////////////////////////////////////////

       if (Force_Game_Type)
        {
           DetectionHelper = TB_TRUE;
           i = Force_Game_Type - 1;
           TBOOL Res;
           printf("\n===================================== UGSP ===================================\n\n");
               switch (i)
                {
                   case 1:
                          printf("       %s",ver);
                          break;

                   case 2:
                   case 3:
                   case 4:
                   case 5:
                   case 6:
                          printf("%s",ver);
                          break;

                   default:
                           printf("              %s",ver);
                           break;
                }

            printf("\n\n===================================== ViTYAN =================================\n\n\n");
            if (i < VUP_SUPPORTED_GAMES_COUNT - VUP_SUPPORTED_VALVE_PLAYER_LIMIT_FILES_COUNT)
             GamePatchersList[i](fStr,fOS,PromtUser,PatchClientProtections,&Res,ver);
            else
              ValvePlayersLimitsPatchersList[i + VUP_SUPPORTED_VALVE_PLAYER_LIMIT_FILES_COUNT - VUP_SUPPORTED_GAMES_COUNT](fStr,fOS,PromtUser,&Res,32);
            if (fOS == VUP_Win32_HL1Classic)
             {
                printf("Please be patient. Encryption in process ... ");
                HL1ClassicListenCryptMan(fStr,TB_TRUE);
                printf("FINISHED\n\n");
             }
            printf("Saving changes to file... ");
            if (Res)
             {
               intError = OPSaveFile(dstF[j],&fStr,TB_FALSE);
               if (intError)
                printf("Failed\n");
               else
                printf("Done\n\n");
             }
            else
             printf("DISABLED(Patching Failed)\n\n");

            if (Res)
             {
                SuccessfullyPatched ++;
                printf("\n\nFILE #%u/%u HAS BEEN PATCHED SUCCESSFULLY\n\n",j + 1,FilesCounterSrc);
             }
            else
             {
                FailedOnPatching ++;
                printf("\n\nPATCHING OF FILE #%u/%u HAS BEEN FAILED. CONTACT THE AUTHOR FOR FURTHER SUPPORT\n\n",j + 1,FilesCounterSrc);
             }
        }
       else
       for (i = 0; i < VUP_SUPPORTED_GAMES_COUNT; i ++)
        {
           DetectionHelper = GameDetectorsList[i](fStr,fOS,ver);
           if (DetectionHelper)
            {
               TBOOL Res;
               printf("\n===================================== UGSP ===================================\n\n");
               switch (i)
                {
                   case 1:
                          printf("       %s",ver);
                          break;

                   case 2:
                   case 3:
                   case 4:
                   case 5:
                   case 6:
                          printf("%s",ver);
                          break;

                   default:
                           printf("              %s",ver);
                           break;
                }

               printf("\n\n===================================== ViTYAN =================================\n\n\n");
               if (i < VUP_SUPPORTED_GAMES_COUNT - VUP_SUPPORTED_VALVE_PLAYER_LIMIT_FILES_COUNT)
                GamePatchersList[i](fStr,fOS,PromtUser,PatchClientProtections,&Res,ver);
               else
                ValvePlayersLimitsPatchersList[i + VUP_SUPPORTED_VALVE_PLAYER_LIMIT_FILES_COUNT - VUP_SUPPORTED_GAMES_COUNT](fStr,fOS,PromtUser,&Res,32);
               if (fOS == VUP_Win32_HL1Classic)
                {
                  printf("Please be patient. Encryption in process ... ");
                  HL1ClassicListenCryptMan(fStr,TB_TRUE);
                  printf("FINISHED\n\n");
                }
               printf("Saving changes to file... ");
               if (Res)
                {
                  intError = OPSaveFile(dstF[j],&fStr,TB_FALSE);
                  if (intError)
                   {
                     printf("Failed\n");
                     printf("===============================================================================\n");
                     printf("===============================================================================\n");
                     printf("============================= WRITE/SAVE ERROR !!! ============================\n");
                     printf("========================= ACCESS DENIED OR FILE IN USE ========================\n");
                     printf("===============================================================================\n");
                     printf("===============================================================================\n");
                   }
                  else
                   printf("Done\n\n");
                }
               else
                printf("DISABLED(Patching Failed)\n\n");

               if (Res)
                {
                  SuccessfullyPatched ++;
                  printf("\n\nFILE #%u/%u HAS BEEN PATCHED SUCCESSFULLY\n\n",j + 1,FilesCounterSrc);
                }
               else
                {
                  FailedOnPatching ++;
                  printf("\n\nPATCHING OF FILE #%u/%u HAS BEEN FAILED. CONTACT THE AUTHOR FOR FURTHER SUPPORT\n\n",j + 1,FilesCounterSrc);
                }

               break;
            }
        }

       if (!DetectionHelper)
        {
          FailedOnPatching ++;
          printf("This binary file is not one of supported files\nIf you sure this file should be patched but it didn't for some reason,\nthen fill bug report to author at csmania.ru or cs.rin.ru forums.\nOperation halted ...\n\n");
          free(fStr);
        }
     }
    if (!FailedOnPatching)
      printf("\n\nALL FILES HAVE BEEN SUCCESSFULLY PATCHED. HAVE FUN!!!\n");
    else
      if (!SuccessfullyPatched)
        printf("\n\nPATCHING OF ALL FILES HAS BEEN FAILED. CONTACT THE AUTHOR FOR FURTHER SUPPORT\n\n");
      else
        printf("\n\nPATCHING OF %i FILES HAS BEEN SUCCESSED WHILE PATCHING OF %i FILES HAS BEEN FAILED\nCONTACT THE AUTHOR FOR FURTHER SUPPORT\n\n",SuccessfullyPatched,FailedOnPatching);
    if (!UnpackersLoadersSwitch)
     getchar();
    return 0;
}

TBOOL ParseArguments(int argc,char ** argv)
{
  uint8_t i;
  for (i = 1; i < argc; i ++)
   {
       if (!strcmp(argv[i],"-advanced"))
        {
          if (PromtUser)
           return TB_FALSE;
          PromtUser = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-patch-client-checks"))
        {
          if (PatchClientProtections)
           return TB_FALSE;
          PatchClientProtections = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-autoquit"))
        {
          if (UnpackersLoadersSwitch)
           return TB_FALSE;
          UnpackersLoadersSwitch = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-patch-mastersrv-updnotify"))
        {
          if (PatchMasterServerUpdateNotificationProtection)
           return TB_FALSE;
          PatchMasterServerUpdateNotificationProtection = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-patch-client-timed-out"))
        {
          if (PatchClientConnectionLostForExtractedSteamProtection)
           return TB_FALSE;
          PatchClientConnectionLostForExtractedSteamProtection = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-leave-pendingid-intact"))
        {
          if (LeavePENDINGIDInPlace)
           return TB_FALSE;
          LeavePENDINGIDInPlace = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-esteamation-prep"))
        {
          if (eSTEAMATiON_Prep_Mode)
           return TB_FALSE;
          eSTEAMATiON_Prep_Mode  = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-esteamation-no-cracked-steam"))
        {
          if (eSTEAMATiON_Prep_DropCrackedSteam)
           return TB_FALSE;
          eSTEAMATiON_Prep_DropCrackedSteam  = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-esteamation-allow-duplicate-ids"))
        {
          if (eSTEAMATiON_Prep_AllowDuplicateSteamIDs)
           return TB_FALSE;
          eSTEAMATiON_Prep_AllowDuplicateSteamIDs  = TB_TRUE;
          continue;
        }
       if (!strcmp(argv[i],"-enable-additional-no-steam-logon-patch"))
        {
          if (Source2007U2NoSteamLogonDisable)
           return TB_FALSE;
          Source2007U2NoSteamLogonDisable  = TB_TRUE;
          continue;
        }
       if (!strncmp(argv[i],"-force-game-type=",17))
        {
          if (Force_Game_Type)
           return TB_FALSE;
          if (!strcmp(argv[i] + 17,"etqw"))
            Force_Game_Type = 1;
          else
            if (!strcmp(argv[i] + 17,"cod4"))
              Force_Game_Type = 2;
            else
              if (!strcmp(argv[i] + 17,"vlvsrc2k7"))
                Force_Game_Type = 3;
              else
                if (!strcmp(argv[i] + 17,"vlvsrc2k7u1"))
                  Force_Game_Type = 4;
                else
                  if (!strcmp(argv[i] + 17,"vlvsrc"))
                    Force_Game_Type = 5;
                  else
                    if (!strcmp(argv[i] + 17,"vlvhl1classic"))
                      Force_Game_Type = 6;
                    else
                      if (!strcmp(argv[i] + 17,"vlvhl1sw"))
                        Force_Game_Type = 7;
                      else
                        if (!strcmp(argv[i] + 17,"ut3"))
                          Force_Game_Type = 8;
                        else
                          if (!strcmp(argv[i] + 17,"vlvsteamclient"))
                            Force_Game_Type = 9;
                          else
                           if (!strcmp(argv[i] + 17,"vlvserverlib"))
                             Force_Game_Type = 10;
                           else
                             return TB_FALSE;
          continue;
        }
       if (!strcmp(argv[i],"-o"))
        {
           if ((i + 1) == argc)
            return TB_FALSE;
           FilesCounterDst ++;
           if (FilesCounterDst > VUP_MAXFILES_TO_PATCH_AT_ONCE)
            return TB_FALSE;
           dstF[FilesCounterDst - 1] = argv[i + 1];
           i ++;
           continue;
        }
       FilesCounterSrc ++;
       if (FilesCounterSrc > VUP_MAXFILES_TO_PATCH_AT_ONCE)
            return TB_FALSE;
       srcF[FilesCounterSrc - 1] = argv[i];

   }

  if (!FilesCounterSrc)
    return TB_FALSE;
  if (FilesCounterDst && (FilesCounterSrc != FilesCounterDst))
    return TB_FALSE;
  if (!FilesCounterDst)
    for (i = 0;i < FilesCounterSrc;i ++)
      dstF[i] = srcF[i];

  return TB_TRUE;
}


void VUPSleep(uint16_t Secs)
{
  #if  (defined(_WIN32) || defined(_WIN64))
   Sleep(Secs * 1000);
  #else
   //usleep(Secs * 1000000);
  #endif
}
