#include "confighandler.h"
#include "vlvmsg.h"

#if !defined(fopen64)
#define fopen64 fopen
#endif

#if (!defined(strcasecmp) && defined(_WIN32))
#define strcasecmp _stricmp
#endif


pthread_mutex_t eST_InitSafeMut = PTHREAD_MUTEX_INITIALIZER;


char EncKey[164];
volatile uint8_t ConfigLoaded = 0,ConfigInLoadingState = 0,RedirectedFncs = 0;
TESTEAMATiONConfig EsteamationCFG;
uint32_t eSharedID = 0;
extern void
#if (defined (_WIN32) || defined (_WIN64))
_cdecl
#endif
(*eDYNF_Log)( tchar const *pMsg, ... );

#if (!defined (_WIN32) && !defined (_WIN64))
void *LegitTicketValidationLibraryHandle = NULL;
void *SCILibraryHandle = NULL;
void * MiniVUPEngineLibraryHandle = NULL;
#else
extern char eST_CPA_Bkup[5],eST_CPW_Bkup[5],eST_LLA_Bkup[5],eST_LLExA_Bkup[5],eST_LLW_Bkup[5],eST_LLExW_Bkup[5];
HMODULE LegitTicketValidationLibraryHandle = NULL;
HMODULE ClientEmuForAUTHLibraryHandle = NULL;
HMODULE MiniVUPEngineLibraryHandle = NULL;
HMODULE OriginalKernelLibraryHandle = NULL;
TmVUPPATCHPROCM MiniVUPEnginePatchProcM = NULL;
TmVUPPATCHPROCF MiniVUPEnginePatchProcF = NULL;
HMODULE SCILibraryHandle = NULL;
#endif
int eSTCfgMode = 0;


uint32_t EFMinimum_eSTEAMATiON_Version_Major = 0;
uint32_t EFMinimum_eSTEAMATiON_Version_Minor = 0;

/*
#ifdef HL1EMUBUILD
 #ifdef HL1AMD64
   char LegitTicketValidationLibrary[51] = "vlvticket_amd64.so";
 #else
   char LegitTicketValidationLibrary[51] = "vlvticket_i386.so";
 #endif
#else
#ifndef eClientEMUAddon
char LegitTicketValidationLibrary[51] = "vlvticket_i486.so";
#else
char LegitTicketValidationLibrary[51] = ".\\vlvticket.dll";
#endif
*/

#if (!defined (_WIN32) && !defined (_WIN64))
  #ifdef HL1EMUBUILD
   #ifdef HL1AMD64
     char LegitTicketValidationLibrary[51] = "vlvticket_amd64.so";
   #else
     char LegitTicketValidationLibrary[51] = "vlvticket_i386.so";
   #endif
  #else
    #ifndef eST_SOURCE_2007_U1_L4D2
     char LegitTicketValidationLibrary[51] = "vlvticket_i486.so";
    #else
     char LegitTicketValidationLibrary[51] = "vlvticket.so";
    #endif
  #endif
  char MiniVUPEngineLibrary[51] = "./mVupEngine_i686.so";
#else
   #ifndef HL1SWLNX
   char LegitTicketValidationLibrary[256] = ".\\vlvticket.dll";
   #endif
   char ClientEmuForAUTHLibrary[51] = ".\\nsemu.dll";
   char MiniVUPEngineLibrary[51] = ".\\VupEngine_i686.dll";
#endif

int LoadEmulatorConfig(TESTEAMATiONConfig *Config)
{
   FILE *cfgFile;
   int Res = 0;
   char ConfField[ConfFieldMaxSize + 1];
   char FieldName[ConfFieldMaxSize + 1];
   char FieldValue[ConfFieldMaxSize + 1];

   if (!Config)
     return (-1);
   memset(Config,0,sizeof(TESTEAMATiONConfig));
   Config -> EFAllowSteamEmuClients = 1;
   Config -> EFAllowLegacySteamEmuClients = 0;
   Config -> EFAllowRevEmuClients = 1;
   Config -> EFAllowRevEmu2NDGClients = 1;
   Config -> EFAllowRevEmu3RDGClients = 1;
   Config -> EFAllowRevSteamUpClients = 1;
   Config -> EFAllowHookEmuClients = 1;
   Config -> EFAllowLegitClients = 1;
   Config -> EFSteamEmuOverallCompat = 1;
   Config -> EFAllowSettiServerScanner = 1;
   Config -> EFAlloweSTEAMATiONClients = 1;
   Config -> EFLogClientTypeOnConnect = 1;
   Config -> EFLogRejectedClientTypes = 1;
   Config -> EFUseMsgInAdditionToLogForLogging = 1;
   Config -> EFRejectAfterInitialValidation = 1;
   Config -> EFAAlloweSTEAMATiONHL1WONClients = 1;
   Config -> EFAAllowLegacyWONHL1Clients = 1;
   #if ((defined (_WIN32) || defined (_WIN64)))
   //Config -> EFForceInetListenServer = 1;
   Config -> EFFilterValveTestApps = 1;
   #endif



   #if (defined (_WIN32) || defined (_WIN64))
     cfgFile = fopen64(".\\cfg\\esteamation.cfg","r");
   #else
     cfgFile = fopen64("./cfg/esteamation.conf","r");
   #endif
   if (!cfgFile)
    {
       #if (defined (_WIN32) || defined (_WIN64))
         char tmpCfg[MAX_PATH + 1];
         int16_t i;
         GetModuleFileName(GetModuleHandle("eSTEAMATiON.dll"),tmpCfg,MAX_PATH);
         i = strlen(tmpCfg) - 1;
         while ((i >= 0) && (tmpCfg[i] != '\\'))
         i --;
         if (i >= 0)
          {
            tmpCfg[i + 1] = '\0';
            strcat(tmpCfg,"\\cfg\\esteamation.cfg");
            cfgFile = fopen64(tmpCfg,"r");
          }

         if (!cfgFile)
          {
		    char vWinLoc[MAX_PATH + 1];
		    GetWindowsDirectoryA(vWinLoc,MAX_PATH);
		    if (MAX_PATH - strlen(vWinLoc) >= 17)
		     {
               if (vWinLoc[strlen(vWinLoc) - 1] != '\\')
                 strcat(vWinLoc,"\\");
			   strcat(vWinLoc,"esteamation.cfg");
               cfgFile = fopen64(vWinLoc,"r");
		     }
		    else
		     {
			   cfgFile = NULL;
		     }
          }
         else
          {
             Res = 3;
          }

       #else
         cfgFile = fopen64("/etc/esteamation/esteamation.conf","r");
       #endif

       if (!cfgFile)
        {
          return 0;
        }
       #if (defined (_WIN32) || defined (_WIN64))
       if (Res != 3)
       #endif
         Res = 2;
    }
   else
    {
      Res = 1;
    }
   while (fgets(ConfField,ConfFieldMaxSize,cfgFile))
    {
       uint8_t i = 0,j = 0,ecfgfield_s;
       ecfgfield_s = strlen(ConfField);
       if (!ecfgfield_s)
         continue;


       while ((ConfField[i] == ' ') && (i < ecfgfield_s))
        i ++;
       if (i >= ecfgfield_s)
         continue;
       if ((ConfField[i] == ';') || (ConfField[i] == '#'))
         continue;
       j = i;
       while((ConfField[i] != ' ') && (i < ecfgfield_s))
        {
          FieldName[i - j] = ConfField[i];
          i ++;
        }
       if (i >= ecfgfield_s)
         continue;
       FieldName[i - j] = '\0';
       while ((ConfField[i] == ' ') && (i < ecfgfield_s))
        i ++;
       if (i >= ecfgfield_s)
         continue;
       if (ConfField[i] != '=')
         continue;
       i ++;
       if (i >= ecfgfield_s)
         continue;
       while ((ConfField[i] == ' ') && (i < ecfgfield_s))
        i ++;
       if (i >= ecfgfield_s)
         continue;
       j = i;
       while((ConfField[i] != ' ') && (ConfField[i] != 0x0A) && (ConfField[i] != 0x0D) && (ConfField[i] != ';') && (ConfField[i] != '#') && (i < ecfgfield_s))
        {
          FieldValue[i - j] = ConfField[i];
          i ++;
        }
       FieldValue[i - j] = '\0';
       if (!strcasecmp(FieldName,"AcceptSteamEmuClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowSteamEmuClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowSteamEmuClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptLegacySteamEmuClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowLegacySteamEmuClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowLegacySteamEmuClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AccepteSTEAMATiONSemiSteamClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAlloweSTEAMATiONClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAlloweSTEAMATiONClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AccepteSTEAMATiONHL1WONClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAAlloweSTEAMATiONHL1WONClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAAlloweSTEAMATiONHL1WONClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptRevEmuClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowRevEmuClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowRevEmuClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptRevEmu2NDGenerationClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowRevEmu2NDGClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowRevEmu2NDGClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptRevEmu3RDGenerationClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowRevEmu3RDGClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowRevEmu3RDGClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptHookEmuClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowHookEmuClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowHookEmuClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptSteamClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowLegitClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowLegitClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptSettiServerScanner"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowSettiServerScanner = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowSettiServerScanner = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptHL1WONClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAAllowLegacyWONHL1Clients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAAllowLegacyWONHL1Clients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptUnknownClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowUnknownClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowUnknownClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptUnknownLegitSimulatingClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowUnknownLegitSimulatingClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowUnknownLegitSimulatingClients = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"AcceptUnknownNonSteamSimulatingClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFAllowUnknownNonSteamSimulatingClients = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFAllowUnknownNonSteamSimulatingClients = 0;
           continue;
        }

       if (!strcasecmp(FieldName,"ForceIPDerivedIDsForLegitClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFUseIPGenerationForLegit = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFUseIPGenerationForLegit = 0;
           continue;
        }

       if (!strcasecmp(FieldName,"ForceIPDerivedIDsForAllNonLegitClients"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFUseIPGenerationForAllNonLegit = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFUseIPGenerationForAllNonLegit = 0;
           continue;
        }

       if (!strcasecmp(FieldName,"ForceMinimumeSTEAMATiONSemiSteamVersionOnClient"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFForceMineSTSemiSteamVersion = 1; //EFForceMinimumeSTEAMATiONSemiSteamVersion
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFForceMineSTSemiSteamVersion = 0;
           continue;
        }

       if (!strcasecmp(FieldName,"MinimumeSTEAMATiONSemiSteamVersionOnClientToEnforce"))
        {
           if (Config -> EFForceMineSTSemiSteamVersion)
            {
              sscanf(FieldValue,"%i.%i",&EFMinimum_eSTEAMATiON_Version_Major,&EFMinimum_eSTEAMATiON_Version_Minor);
              if (EFMinimum_eSTEAMATiON_Version_Minor < 10)
               EFMinimum_eSTEAMATiON_Version_Minor *= 10;
            }
           continue;
        }

       if (!strcasecmp(FieldName,"LogClientTypeOnConnect"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFLogClientTypeOnConnect = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFLogClientTypeOnConnect = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"LogRejectedClientTypeOnConnect"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFLogRejectedClientTypes = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFLogRejectedClientTypes = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"LogUsingMsgInAdditionToLogFn"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFUseMsgInAdditionToLogForLogging = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFUseMsgInAdditionToLogForLogging = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"EnforceSteamEmuCompatIDMode"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFSteamEmuOverallCompat = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFSteamEmuOverallCompat = 0;
           continue;
        }

       if (!strcasecmp(FieldName,"ForceUseOfSharedNonSteamID"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFForceUseOfSharedID = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFForceUseOfSharedID = 0;
           continue;
        }

       if (!strcasecmp(FieldName,"SharedNonSteamIDToUse"))
        {
           eSharedID = strtoul(FieldValue, NULL, 0);
           continue;
        }

       if (!strcasecmp(FieldName,"ForceClientsRejectAfterInitialValidation"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFRejectAfterInitialValidation = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFRejectAfterInitialValidation = 0;
           continue;
        }
    #if (defined (_WIN32) || defined (_WIN64))
       if (!strcasecmp(FieldName,"MSWindowsLegitHelperLibName"))
        {
           strncpy(LegitTicketValidationLibrary,FieldValue,50);
           LegitTicketValidationLibrary[50] = '\0';
           continue;
        }
       if (!strcasecmp(FieldName,"ForceListenServerToStartInInetMode"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFForceInetListenServer = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFForceInetListenServer = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"SteamEmulatorLibNameForClientAUTH"))
        {
           strncpy(ClientEmuForAUTHLibrary,FieldValue,50);
           ClientEmuForAUTHLibrary[50] = '\0';
           continue;
        }
       if (!strcasecmp(FieldName,"mVUPEngineLibName"))
        {
           strncpy(MiniVUPEngineLibrary,FieldValue,50);
           ClientEmuForAUTHLibrary[50] = '\0';
           continue;
        }
       if (!strcasecmp(FieldName,"SESAuthenticateViaValvesServersAsTypicalSteam"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFSeSAuthenticateViaValveAUTHServer = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFSeSAuthenticateViaValveAUTHServer = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"SESDisableAutomaticUpdateOfExtractedGameFiles"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFDisableGameFilesAutoUpdating = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFDisableGameFilesAutoUpdating = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"SESFilterValveTestAppsOutOfAppsView"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFFilterValveTestApps = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFFilterValveTestApps = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"EXPUseSpecialSteamWorksTokenForAUTH"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFUseSteamWorksNonSteamV35UserIDTicket = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFUseSteamWorksNonSteamV35UserIDTicket = 0;
           continue;
        }
    #endif
       if (!strcasecmp(FieldName,"Module_Load_mVUP"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFModulemVUPLoad = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFModulemVUPLoad = 0;
           continue;
        }
       if (!strcasecmp(FieldName,"Module_Load_libSCI"))
        {
           if (!strcasecmp(FieldValue,"True"))
             Config -> EFModuleslibSCILoad = 1;
           else
            if (!strcasecmp(FieldValue,"False"))
              Config -> EFModuleslibSCILoad = 0;
           continue;
        }

    }
   fclose(cfgFile);
   return Res;

}

STEAM_API TESTEAMATiONConfig STEAM_CALL Get_eSTConfig()
{
   return EsteamationCFG;
}

STEAM_API uint8_t STEAM_CALL PLUGINS_eSTIsInitialized()
{
   pthread_mutex_lock(&eST_InitSafeMut);
   return (!ConfigInLoadingState && ConfigLoaded);
   pthread_mutex_unlock(&eST_InitSafeMut);
}


int IsLibraryInitialized()
{
   //MessageBox(NULL,"IsLibraryInitialized is now EXECUTING :D","eSTEAMATiON Startup TEST",MB_OK | MB_ICONINFORMATION);
   pthread_mutex_lock(&eST_InitSafeMut);
   if (!ConfigLoaded && !ConfigInLoadingState)
    {
      //char Module[61];
      int i;
      ConfigInLoadingState = 1;
      if (!InitLoggingFacility((void **)(&eDYNF_Log)))
       {
         ConfigInLoadingState = 0;
         eDBGLOG("estlogf.log","eSTEAMATiON FATAL ERROR: eST general dynamic logging facility failed to initialize. Switching to rescue-mode logging facility ...\n");
         pthread_mutex_unlock(&eST_InitSafeMut);
         return false;
       }
      else
        eDBGLOG("estlogf.log","eSTEAMATiON INFO: eST dynamic logging initialize is in process\n");
      eSTCfgMode = LoadEmulatorConfig(&EsteamationCFG);
      //strcpy(Module,"./bin/");
      //strcat(Module,LegitTicketValidationLibrary);
      //LegitTicketValidationLibraryHandle = dlopen(Module,RTLD_LAZY | RTLD_GLOBAL);
      *((uint32_t *)EncKey) = 164;
      for (i = 0;i < 160; i ++)
        EncKey[i + 4] = 6;


      LogDisableTimeStamp();
      eDYNF_Log("\neSTEAMATiON is starting-up.\nVersion : %s.%s\nType: %s\nRelease Date: %s\nAuthor:ViTYAN\n",ESTEMU_MAJOR_VER,ESTEMU_MINOR_VER,ESTEMU_TYPE_VER,ESTEMU_RDATE);
      eDYNF_Log("\neSTEAMATiON initialization and preconfiguring process has been started...\n");
    #if !defined(_WIN32) && !defined (_WIN64)
      #if !defined(HL1SWLNX) && !defined(__STEAM_3_ONLY_STARTUP__)
      LegitTicketValidationLibraryHandle = dlopen(LegitTicketValidationLibrary,RTLD_LAZY | RTLD_GLOBAL);
      if (LegitTicketValidationLibraryHandle)
       {
          eDYNF_Log("eSTEAMATiON legit helper library(%s) has been loaded\n",LegitTicketValidationLibrary);
       }
      else
       {
          eDYNF_Log("\neSTEAMATiON ERROR: Failed to load legit helper library(%s)\n",LegitTicketValidationLibrary);
          pthread_mutex_lock(&eST_InitSafeMut);
          ConfigInLoadingState = 0;
          pthread_mutex_unlock(&eST_InitSafeMut);
          return false;
       }
      #endif

      if (EsteamationCFG.EFModulemVUPLoad)
         {
           MiniVUPEngineLibraryHandle = dlopen(MiniVUPEngineLibrary,RTLD_NOW | RTLD_GLOBAL);
           if (MiniVUPEngineLibraryHandle)
            {
              eDYNF_Log("eSTEAMATiON miniVUP Engine (%s) has been loaded\n",MiniVUPEngineLibrary);
            }
           else
            {
              eDYNF_Log("\neSTEAMATiON Info: miniVUP Engine (%s) is not installed.\n",MiniVUPEngineLibrary);
            }
        }

      if (EsteamationCFG.EFModuleslibSCILoad)
       {
         SCILibraryHandle = dlopen("libSCI.so",RTLD_NOW | RTLD_GLOBAL);
         if (SCILibraryHandle)
          {
             TBOOL (*pSCI_Init_Proc)(TESTEAMATiONConfig);
             eDYNF_Log("eSTEAMATiON libSCI library has been loaded\n");
             eDYNF_Log("Governing libSCI initialization function address ... ");
             pSCI_Init_Proc = (TBOOL (*)(TESTEAMATiONConfig)) dlsym(SCILibraryHandle,"SCI_InitializeLibraryAndAttachToSC");
             eDYNF_Log("0x%lX\n",pSCI_Init_Proc);
             if (pSCI_Init_Proc)
              {
                 BOOL Res;
                 eDYNF_Log("Calling libSCI initialization function and receiving it's HELLO message:\n");
                 Res = pSCI_Init_Proc(EsteamationCFG);
                 eDYNF_Log("\nlibSCI initialization function returned the following:%s\n",(Res == TB_TRUE) ? "SUCCESS" : "FAILURE");
              }
          }
         else
          {
             eDYNF_Log("\neSTEAMATiON ERROR: Failed to load libSCI\n");
          }
       }
    #else
            char tmpLegitTicketValidationLibrary[256],OriginalKernelLibrary[256],tmpOriginalKernelLibrary[256],tmpSCIModuleName[256];
            WIN32_FILE_ATTRIBUTE_DATA vSteamLibInfo;
            uint64_t vSteamLibSize;
            BOOL IsLegitHelperLibraryLoaded = false;
            GetModuleFileName(GetModuleHandle("eSTEAMATiON.dll"),LegitTicketValidationLibrary,256);
            i = strlen(LegitTicketValidationLibrary) - 1;
            while ((LegitTicketValidationLibrary[i] != '\\'))
             {
               if (i == 0)
                 break;
               i --;
             }
            LegitTicketValidationLibrary[i + 1] = '\0';
            strcpy(tmpLegitTicketValidationLibrary,LegitTicketValidationLibrary);
            strcpy(tmpOriginalKernelLibrary,LegitTicketValidationLibrary);
            strcpy(tmpSCIModuleName,LegitTicketValidationLibrary);
            strcat(LegitTicketValidationLibrary,"vlvticket.dll");
            strcat(tmpLegitTicketValidationLibrary,"Steam.dll");
            strcat(tmpOriginalKernelLibrary,"vokrn.dll");
            strcat(tmpSCIModuleName,"libSCI.dll");
    #if !defined(HL1SWLNX) && !defined(__STEAM_3_ONLY_STARTUP__)
            GetFileAttributesEx(tmpLegitTicketValidationLibrary,GetFileExInfoStandard,&vSteamLibInfo);
            vSteamLibSize = (((uint64_t)vSteamLibInfo.nFileSizeHigh) << 32) + (vSteamLibInfo.nFileSizeLow);
            //eDBGLOG("estrev.log","eSTEAMATiON prepares legit helper library(%s)(library size=%llu bytes) ... ",LegitTicketValidationLibrary,vSteamLibSize);
            eDYNF_Log("eSTEAMATiON prepares legit helper library(%s)(library size=%llu bytes) ... ",LegitTicketValidationLibrary,vSteamLibSize);
            if (vSteamLibSize > 2097152)
             {
               if (CopyFile(tmpLegitTicketValidationLibrary,LegitTicketValidationLibrary,FALSE))
                {
                   eDYNF_Log("FINISHED\n");
                }
               else
                {
                   if (GetModuleHandle("Steam.exe"))
                    {
                      eDYNF_Log("FAILED\n");
                      ConfigInLoadingState = 0;
                      //pthread_mutex_unlock(&eST_InitSafeMut);
                      return false;
                    }
                   else
                     eDYNF_Log("SKIPPED(DONE BY SeS ENGINE)\n");
                }
             }
            else
             {
                eDYNF_Log("SKIPPED(NON-STEAM EMULATOR DETECTED)\n");
             }

        LegitTicketValidationLibraryHandle = LoadLibrary(LegitTicketValidationLibrary);
        if (LegitTicketValidationLibraryHandle)
         {
           IsLegitHelperLibraryLoaded = true;
           eDYNF_Log("eSTEAMATiON legit helper library(%s) has been loaded\n",LegitTicketValidationLibrary);
         }
        else
         {
           eDYNF_Log("\neSTEAMATiON ERROR: Failed to load legit helper library(%s)(WINERR=%i)\n",LegitTicketValidationLibrary,GetLastError());
           ConfigInLoadingState = 0;
           pthread_mutex_unlock(&eST_InitSafeMut);
           return false;
           //return true;
         }
   #endif
        ClientEmuForAUTHLibraryHandle = LoadLibrary(ClientEmuForAUTHLibrary);
        if (ClientEmuForAUTHLibraryHandle)
         {
           eDYNF_Log("eSTEAMATiON clientside Non-Steam AUTH helper library(%s) has been loaded\n",ClientEmuForAUTHLibrary);
         }
        else
         {
           eDYNF_Log("\neSTEAMATiON Info: External Non-Steam AUTH helper library (%s) not installed. Using BUILT-IN eSTEAMATiON SemiSteam Authentication\n",ClientEmuForAUTHLibrary);
         }
        if (EsteamationCFG.EFModulemVUPLoad)
         {
           MiniVUPEngineLibraryHandle = LoadLibrary(MiniVUPEngineLibrary);
           if (MiniVUPEngineLibraryHandle)
            {
              eDYNF_Log("eSTEAMATiON miniVUP Engine (%s) has been loaded\n",MiniVUPEngineLibrary);
              eDYNF_Log("eSTEAMATiON prepares miniVUP Engine's memory autodetect-patch routine for use ... ");
              MiniVUPEnginePatchProcM = (TmVUPPATCHPROCM) GetProcAddress(MiniVUPEngineLibraryHandle,"mVUPModulePatchModule");
              if (MiniVUPEnginePatchProcM)
               {
                 eDYNF_Log("SUCCEEDED\n");
               }
              else
               {
                 eDYNF_Log("FAILED\neSTEAMATiON unloads miniVUP Engine due to fatal errors\n");
                 FreeLibrary(MiniVUPEngineLibraryHandle);
                 MiniVUPEngineLibraryHandle = NULL;
               }

              eDYNF_Log("eSTEAMATiON prepares miniVUP Engine's file autodetect-patch routine for use ... ");
              MiniVUPEnginePatchProcF = (TmVUPPATCHPROCF) GetProcAddress(MiniVUPEngineLibraryHandle,"mVUPModulePatchFileA");
              if (MiniVUPEnginePatchProcF)
               {
                 eDYNF_Log("SUCCEEDED\n");
               }
              else
               {
                 eDYNF_Log("FAILED\neSTEAMATiON unloads miniVUP Engine due to fatal errors\n");
                 FreeLibrary(MiniVUPEngineLibraryHandle);
                 MiniVUPEngineLibraryHandle = NULL;
               }
            }
           else
            {
              eDYNF_Log("\neSTEAMATiON Info: miniVUP Engine (%s) is not installed.\n",MiniVUPEngineLibrary);
            }
        }

        if (EsteamationCFG.EFModuleslibSCILoad)
         {
           SCILibraryHandle = LoadLibrary(tmpSCIModuleName);
           if (SCILibraryHandle)
            {
               TBOOL (*pSCI_Init_Proc)(TESTEAMATiONConfig);
               eDYNF_Log("eSTEAMATiON libSCI library has been loaded\n");
               eDYNF_Log("Governing libSCI initialization function address ... ");
               pSCI_Init_Proc = (TBOOL (*)(TESTEAMATiONConfig)) GetProcAddress(SCILibraryHandle,"SCI_InitializeLibraryAndAttachToSC");
               eDYNF_Log("0x%lX\n",pSCI_Init_Proc);
               if (pSCI_Init_Proc)
                {
                   BOOL Res;
                   eDYNF_Log("Calling libSCI initialization function and receiving it's HELLO message:\n");
                   Res = pSCI_Init_Proc(EsteamationCFG);
                   eDYNF_Log("\nlibSCI initialization function returned the following:%s\n",(Res == TB_TRUE) ? "SUCCESS" : "FAILURE");
                }
            }
           else
            {
               eDYNF_Log("\neSTEAMATiON ERROR: Failed to load libSCI\n");
            }
         }
        srand(time(NULL));

   #if !defined(HL1SWLNX) && !defined(__STEAM_3_ONLY_STARTUP__)
        GetModuleFileName(GetModuleHandle("kernel32.dll"),OriginalKernelLibrary,255);
        eDYNF_Log("eSTEAMATiON prepares kernel helper library(%s) ... ",tmpOriginalKernelLibrary);
        OriginalKernelLibraryHandle = GetModuleHandle("vokrn.dll");
        if (CopyFile(OriginalKernelLibrary,tmpOriginalKernelLibrary,FALSE) || OriginalKernelLibraryHandle)
         {
            eDYNF_Log("FINISHED\n");
         }
        else
         {
            if (GetModuleHandle("Steam.exe"))
             {
                eDYNF_Log("FAILED\n");
                ConfigInLoadingState = 0;
                pthread_mutex_unlock(&eST_InitSafeMut);
                return false;
             }
            else
             eDYNF_Log("SKIPPED(DONE BY SeS ENGINE)\n");
         }
        if (!OriginalKernelLibraryHandle)
          OriginalKernelLibraryHandle = LoadLibrary(tmpOriginalKernelLibrary);
        if (OriginalKernelLibraryHandle)
         {
            eDYNF_Log("eSTEAMATiON kernel helper library(%s) has been loaded\n",tmpOriginalKernelLibrary);
         }
        else
         {
           eDYNF_Log("eSTEAMATiON failed to load kernel helper library(%s)(WINERR=%i)\n",tmpOriginalKernelLibrary,GetLastError());
           ConfigInLoadingState = 0;
           pthread_mutex_unlock(&eST_InitSafeMut);
           return false;
         }
   #endif



        /*
        if (!RedirectedFncs)
         {
           HMODULE VNTKrn,eSTLibModuleHandle = NULL;
           VNTKrn = GetModuleHandle("kernel32.dll");
           eSTLibModuleHandle = GetModuleHandle("eSTEAMATiON.dll");
           if (!eSTLibModuleHandle || !VNTKrn)
             return false;
           eDBGLOG(".\\kerntrap5.log","NOTICE:Redirecting specific kernel functions\n");
           VLDRRedirectFunctionEx(VNTKrn,eSTLibModuleHandle,"LoadLibraryW","eSTVLoadLibraryW",eST_LLA_Bkup);
           if (GetModuleHandle("Steam.exe"))
             VLDRRedirectFunctionEx(VNTKrn,eSTLibModuleHandle,"CreateProcessW","eSTVCreateProcessW",eST_CPA_Bkup);
           else
             VLDRRedirectFunctionEx(VNTKrn,eSTLibModuleHandle,"LoadLibraryExW","eSTVLoadLibraryExW",eST_LLExA_Bkup);
           RedirectedFncs = 1;
         }
        */

    #endif
      eDYNF_Log("\neSTEAMATiON v"ESTEMU_VER"\nRelease Date:"ESTEMU_RDATE"\nAuthor:ViTYAN\nhas been successfully loaded.\n");
      if (eSTCfgMode == (-1))
       {
         eDYNF_Log("\neSTEAMATiON configuration failed(Memory access problem). Check your hardware configuration and restart your server\n");
       }
      else
       {
         eDYNF_Log("\neSTEAMATiON configuration has been successfully determined(%s).\n",(eSTCfgMode == 0) ? "Default config" : ((eSTCfgMode == 1) ? "Local config" : ((eSTCfgMode == 2) ? "Systemwide config" : "SeS Engine config")));
         eDYNF_Log("eSTEAMATiON configuration is as following:\n\n");
         eDYNF_Log("[eSTEAMATiON SECURITY POLICY]\n");
         eDYNF_Log("REJECT Clients after initial validation process compleation: %s\n",EsteamationCFG.EFRejectAfterInitialValidation ? "ENABLED" : "DISABLED");
         eDYNF_Log("SteamEmu Clients: %s\n",EsteamationCFG.EFAllowSteamEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("LEGACY SteamEmu Clients: %s\n",EsteamationCFG.EFAllowLegacySteamEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("eSTEAMATiON SemiSteam Native Clients: %s\n",EsteamationCFG.EFAlloweSTEAMATiONClients ? "ACCEPT" : "DROP");
         eDYNF_Log("eSTEAMATiON HL1-WON Clients: %s\n",EsteamationCFG.EFAAlloweSTEAMATiONHL1WONClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Minimal Version Restriction for eSTEAMATiON SemiSteam Native Clients : %s\n",EsteamationCFG.EFForceMineSTSemiSteamVersion ? "ENABLED" : "DISABLED");
         if (EsteamationCFG.EFForceMineSTSemiSteamVersion)
          eDYNF_Log("Minimal Version required for eSTEAMATiON SemiSteam Native Client's is: %i.%i\n",EFMinimum_eSTEAMATiON_Version_Major,EFMinimum_eSTEAMATiON_Version_Minor);
         eDYNF_Log("RevEmu Clients: %s\n",EsteamationCFG.EFAllowRevEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("RevEmu 3-Rd Generation Clients: %s\n",EsteamationCFG.EFAllowRevEmu3RDGClients ? "ACCEPT" : "DROP");
         eDYNF_Log("HookEmu Clients: %s\n",EsteamationCFG.EFAllowHookEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Unknown Clients: %s\n",EsteamationCFG.EFAllowUnknownClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Unknown Steam simulating Clients: %s\n",EsteamationCFG.EFAllowUnknownLegitSimulatingClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Unknown NonSteam simulating Clients: %s\n",EsteamationCFG.EFAllowUnknownNonSteamSimulatingClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Legit Valve and Cracked Steam Clients: %s\n",EsteamationCFG.EFAllowLegitClients ? "ACCEPT" : "DROP");
         eDYNF_Log("SETTI server scanner: %s\n",EsteamationCFG.EFAllowSettiServerScanner ? "ACCEPT" : "DROP");
         eDYNF_Log("LEGACY HL1 WON(-nosteam) Clients: %s\n",EsteamationCFG.EFAAllowLegacyWONHL1Clients ? "ACCEPT" : "DROP");
         eDYNF_Log("\n[eSTEAMATiON GLOBAL ID-GENERATION MODE]\n");
         eDYNF_Log("GLOBAL SteamID mode for Supported(Known) NonSteam Clients: %s\n",EsteamationCFG.EFForceUseOfSharedID ? "Shared ID"  : (EsteamationCFG.EFUseIPGenerationForAllNonLegit ? "by IP" : "Unique ID's"));
         eDYNF_Log("GLOBAL SteamID mode for Legit Valve and Cracked Steam Clients: %s\n",EsteamationCFG.EFUseIPGenerationForLegit ? "by IP" : "Unique ID's");
         if (EsteamationCFG.EFForceUseOfSharedID)
           eDYNF_Log("Shared SteamID For NonSteam clients which was configured: STEAM_0:%u:%u\n", eSharedID % 2, eSharedID / 2);
         else
           eDYNF_Log("SteamEmu ID compatability mode: %s\n",EsteamationCFG.EFSteamEmuOverallCompat ? "ENABLED" : "DISABLED");
         eDYNF_Log("\n[eSTEAMATiON LOGGING]\n");
         eDYNF_Log("Enable logging of Client type to console on connect: %s\n",EsteamationCFG.EFLogClientTypeOnConnect ? "ENABLED" : "DISABLED");
         eDYNF_Log("Enable logging of REJECTED Client type to console on connect: %s\n",EsteamationCFG.EFLogRejectedClientTypes ? "ENABLED" : "DISABLED");
         #if (defined (_WIN32) || defined (_WIN64))
         eDYNF_Log("\n[SemiSteam[SeS] CONFIG]\n");
         //Log("SemiSteam's Internal AUTH mechanism : %s\n",est_getflagvalue(EsteamationCFG,EFSeSAuthenticateViaValveAUTHServer) ? "VALVE" : (ClientEmuForAUTHLibraryHandle ? "3rd Party Non-Steam" : "eSTEAMATiON SemiSteam AUTHv2"));
         eDYNF_Log("SemiSteam's Internal AUTH mechanism : %s\n",EsteamationCFG.EFSeSAuthenticateViaValveAUTHServer ? "VALVE" :  (ClientEmuForAUTHLibraryHandle ? "3rd Party Non-Steam" : ( EsteamationCFG.EFUseSteamWorksNonSteamV35UserIDTicket ? "eSTEAMATiON Experimental GoldSrc SteamWorks AUTH For V35 NS" : "eSTEAMATiON SemiSteam AUTHv2")));
         eDYNF_Log("Enforce SV_LAN 0 mode for Listen Servers : %s\n",EsteamationCFG.EFForceInetListenServer ? "ENABLED" : "DISABLED");
         eDYNF_Log("Automatic update of extracted game files(Minimal footprints) : %s\n",EsteamationCFG.EFDisableGameFilesAutoUpdating ? "DISABLED" : "ENABLED");
         eDYNF_Log("Don't show Valve TesApps in the apps list : %s\n",EsteamationCFG.EFFilterValveTestApps ? "ENABLED" : "DISABLED");
         #endif
         eDYNF_Log("\n[eSTEAMATiON EXTENSION MODULES]\n");
         eDYNF_Log("MiniVUP Engine (Library:%s) : %s (STATUS:%s)\n",MiniVUPEngineLibrary,EsteamationCFG.EFModulemVUPLoad ? "ENABLED" : "DISABLED",MiniVUPEngineLibraryHandle ? "ACTIVE" : "INACTIVE");
         eDYNF_Log("eSTEAMATiON libSCI STEAM3 Extension : %s (STATUS:%s)\n\n\n\n",EsteamationCFG.EFModuleslibSCILoad ? "ENABLED" : "DISABLED",SCILibraryHandle ? "ACTIVE" : "INACTIVE");

       }
      LogEnableTimeStamp();
      #if (defined (_WIN32) || defined (_WIN64))
      if (IsLegitHelperLibraryLoaded && OriginalKernelLibraryHandle)
      #endif
        ConfigLoaded = 1;
      ConfigInLoadingState = 0;
    }
   else
    {

      do
       {
       }
      while (ConfigInLoadingState && !ConfigLoaded);


    }
   pthread_mutex_unlock(&eST_InitSafeMut);
   return true;
}

void eST_PrintConfig()
{
         LogDisableTimeStamp();
         eDYNF_Log("\neSTEAMATiON Steam ParaEmulator.\nVersion : %s.%s\nType: %s\nRelease Date: %s\nAuthor:ViTYAN\n",ESTEMU_MAJOR_VER,ESTEMU_MINOR_VER,ESTEMU_TYPE_VER,ESTEMU_RDATE);
         if (eSTCfgMode == (-1))
          {
            eDYNF_Log("\n\n\neSTEAMATiON configuration failed(Memory access problem). Check your hardware configuration and restart your server\n");
            return;
          }
         eDYNF_Log("\n\n\neSTEAMATiON configuration has been successfully determined(%s).\n",(eSTCfgMode == 0) ? "Default config" : ((eSTCfgMode == 1) ? "Local config" : ((eSTCfgMode == 2) ? "Systemwide config" : "SeS Engine config")));
         eDYNF_Log("eSTEAMATiON configuration is as following:\n\n");
         eDYNF_Log("[eSTEAMATiON SECURITY POLICY]\n");
         eDYNF_Log("REJECT Clients after initial validation process compleation: %s\n",EsteamationCFG.EFRejectAfterInitialValidation ? "ENABLED" : "DISABLED");
         eDYNF_Log("SteamEmu Clients: %s\n",EsteamationCFG.EFAllowSteamEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("LEGACY SteamEmu Clients: %s\n",EsteamationCFG.EFAllowLegacySteamEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("eSTEAMATiON SemiSteam Native Clients: %s\n",EsteamationCFG.EFAlloweSTEAMATiONClients ? "ACCEPT" : "DROP");
         eDYNF_Log("eSTEAMATiON HL1-WON Clients: %s\n",EsteamationCFG.EFAAlloweSTEAMATiONHL1WONClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Minimal Version Restriction for eSTEAMATiON SemiSteam Native Clients : %s\n",EsteamationCFG.EFForceMineSTSemiSteamVersion ? "ENABLED" : "DISABLED");
         if (EsteamationCFG.EFForceMineSTSemiSteamVersion)
          eDYNF_Log("Minimal Version required for eSTEAMATiON SemiSteam Native Client's is: %i.%i\n",EFMinimum_eSTEAMATiON_Version_Major,EFMinimum_eSTEAMATiON_Version_Minor);
         eDYNF_Log("RevEmu Clients: %s\n",EsteamationCFG.EFAllowRevEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("RevEmu 3-Rd Generation Clients: %s\n",EsteamationCFG.EFAllowRevEmu3RDGClients ? "ACCEPT" : "DROP");
         eDYNF_Log("HookEmu Clients: %s\n",EsteamationCFG.EFAllowHookEmuClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Unknown Clients: %s\n",EsteamationCFG.EFAllowUnknownClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Unknown Steam simulating Clients: %s\n",EsteamationCFG.EFAllowUnknownLegitSimulatingClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Unknown NonSteam simulating Clients: %s\n",EsteamationCFG.EFAllowUnknownNonSteamSimulatingClients ? "ACCEPT" : "DROP");
         eDYNF_Log("Legit Valve and Cracked Steam Clients: %s\n",EsteamationCFG.EFAllowLegitClients ? "ACCEPT" : "DROP");
         eDYNF_Log("SETTI server scanner: %s\n",EsteamationCFG.EFAllowSettiServerScanner ? "ACCEPT" : "DROP");
         eDYNF_Log("LEGACY HL1 WON(-nosteam) Clients: %s\n",EsteamationCFG.EFAAllowLegacyWONHL1Clients ? "ACCEPT" : "DROP");
         eDYNF_Log("\n[eSTEAMATiON GLOBAL ID-GENERATION MODE]\n");
         eDYNF_Log("GLOBAL SteamID mode for Supported(Known) NonSteam Clients: %s\n",EsteamationCFG.EFForceUseOfSharedID ? "Shared ID"  : (EsteamationCFG.EFUseIPGenerationForAllNonLegit ? "by IP" : "Unique ID's"));
         eDYNF_Log("GLOBAL SteamID mode for Legit Valve and Cracked Steam Clients: %s\n",EsteamationCFG.EFUseIPGenerationForLegit ? "by IP" : "Unique ID's");
         if (EsteamationCFG.EFForceUseOfSharedID)
           eDYNF_Log("Shared SteamID For NonSteam clients which was configured: STEAM_0:%u:%u\n", eSharedID % 2, eSharedID / 2);
         else
           eDYNF_Log("SteamEmu ID compatability mode: %s\n",EsteamationCFG.EFSteamEmuOverallCompat ? "ENABLED" : "DISABLED");
         eDYNF_Log("\n[eSTEAMATiON LOGGING]\n");
         eDYNF_Log("Enable logging of Client type to console on connect: %s\n",EsteamationCFG.EFLogClientTypeOnConnect ? "ENABLED" : "DISABLED");
         eDYNF_Log("Enable logging of REJECTED Client type to console on connect: %s\n",EsteamationCFG.EFLogRejectedClientTypes ? "ENABLED" : "DISABLED");
         #if (defined (_WIN32) || defined (_WIN64))
         eDYNF_Log("\n[SemiSteam[SeS] CONFIG]\n");
         eDYNF_Log("SemiSteam's Internal AUTH mechanism : %s\n",EsteamationCFG.EFSeSAuthenticateViaValveAUTHServer ? "VALVE" :  (ClientEmuForAUTHLibraryHandle ? "3rd Party Non-Steam" : ( EsteamationCFG.EFUseSteamWorksNonSteamV35UserIDTicket ? "eSTEAMATiON Experimental GoldSrc SteamWorks AUTH For V35 NS" : "eSTEAMATiON SemiSteam AUTHv2")));
         eDYNF_Log("Enforce SV_LAN 0 mode for Listen Servers : %s\n",EsteamationCFG.EFForceInetListenServer ? "ENABLED" : "DISABLED");
         eDYNF_Log("Automatic update of extracted game files(Minimal footprints) : %s\n",EsteamationCFG.EFDisableGameFilesAutoUpdating ? "DISABLED" : "ENABLED");
         eDYNF_Log("Don't show Valve TesApps in the apps list : %s\n",EsteamationCFG.EFFilterValveTestApps ? "ENABLED" : "DISABLED");
         #endif
         eDYNF_Log("\n[eSTEAMATiON EXTENSION MODULES]\n");
         eDYNF_Log("MiniVUP Engine (Library:%s) : %s (STATUS:%s)\n",MiniVUPEngineLibrary,EsteamationCFG.EFModulemVUPLoad ? "ENABLED" : "DISABLED",MiniVUPEngineLibraryHandle ? "ACTIVE" : "INACTIVE");
         eDYNF_Log("eSTEAMATiON libSCI STEAM3 Extension : %s (STATUS:%s)\n\n\n\n",EsteamationCFG.EFModuleslibSCILoad ? "ENABLED" : "DISABLED",SCILibraryHandle ? "ACTIVE" : "INACTIVE");

         LogEnableTimeStamp();
}


