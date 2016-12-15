
#ifndef _VCONFIG_HANDLER_H_
#define _VCONFIG_HANDLER_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <wchar.h>
#include <pthread.h>
#include "SteamCommon.h"


#if (defined (_WIN32) || defined (_WIN64))
  #include <string.h>
  #include <windows.h>
  #include "mvuperror.h"

  typedef TmVUPError (*TmVUPPATCHPROCM)(HMODULE,BOOL);
  typedef TmVUPError (*TmVUPPATCHPROCF)(char *,BOOL);
   #if (!defined(WITH_CLIENT_AUTH_WRAP))
    #include "vkern.h"
   #endif
#else
  #include <dlfcn.h>
#endif

#include "opportability.h"


#define ConfFieldMaxSize 60


#define ESTEMU_VER ESTEMU_MAJOR_VER"."ESTEMU_MINOR_VER" "ESTEMU_TYPE_VER
#define ESTEMU_MAJOR_VER "2"
#define ESTEMU_MINOR_VER "0"
#define ESTEMU_TYPE_VER "Release Candidate 2 TRUNK/SVN"
#define ESTEMU_RDATE "28 December 2009  3:45 AM"
#define ESTEMU_AUTH_PROTOCOL 2




/*

#define EFAllowSteamEmuClients                    0x1LL
#define EFAllowLegacySteamEmuClients              0x2LL
#define EFAlloweSTEAMATiONClients                 0x4LL
#define EFAllowRevEmuClients                      0x8LL
#define EFAllowHookEmuClients                     0x10LL
#define EFAllowLegitClients                       0x20LL
#define EFAllowSettiServerScanner                 0x40LL
#define EFAllowUnknownClients                     0x80LL
#define EFAllowUnknownLegitSimulatingClients      0x100LL
#define EFAllowUnknownNonSteamSimulatingClients   0x200LL
#define EFAAllowLegacyWONHL1Clients               0x400LL
#define EFAAlloweSTEAMATiONHL1WONClients          0x800LL


#define EFUseIPGenerationForLegit                 0x1000LL
#define EFUseIPGenerationForAllNonLegit           0x2000LL
#define EFLogClientTypeOnConnect                  0x4000LL
#define EFLogRejectedClientTypes                  0x8000LL
#define EFForceUseOfSharedID                      0x10000LL
#define EFSteamEmuOverallCompat                   0x20000LL
#define EFRejectAfterInitialValidation            0x40000LL
#define EFForceMineSTSemiSteamVersion             0x80000LL



#define EFForceInetListenServer                   0x100000LL
#define EFSeSAuthenticateViaValveAUTHServer       0x200000LL
#define EFDisableGameFilesAutoUpdating            0x400000LL
#define EFFilterValveTestApps                     0x800000LL

#define EFUseMsgInAdditionToLogForLogging         0x1000000LL

#define EFPrintConfigurationOnLoad 0x1000001LL
#define EFPrintConfigurationOnValidationInit 0x1000002LL
#define EFForceUseOfSharedIDForID2IPCLients 0x1000003LL
#define EFUseSteamWorksNonSteamV35UserIDTicket 0x1000004LL


#define est_getflagvalue(v_conf,v_flag) ( (((v_conf) & v_flag) == 0) ? 0 : 1 )
#define est_setflagvalue(v_conf,v_flag) ( *v_conf = ((*v_conf) | v_flag) )
#define est_unsetflagvalue(v_conf,v_flag) ((((*v_conf) & v_flag) == 0) ? : *v_conf = (*v_conf) - v_flag)
*/

typedef enum
{
   TB_FALSE = 0,
   TB_TRUE = 1,
} TBOOL;


extern "C" {

//typedef uint64_t TESTEAMATiONConfig;

typedef struct TESTEAMATiONConfig_TAG
{
   /*
  Client security configuration definitions
                                            */

uint8_t EFAllowSteamEmuClients:1;
uint8_t EFAllowLegacySteamEmuClients:1;
uint8_t EFAlloweSTEAMATiONClients:1;
uint8_t EFAllowRevEmuClients:1;
uint8_t EFAllowRevEmu2NDGClients:1;
uint8_t EFAllowRevEmu3RDGClients:1;

uint8_t EFAllowRevSteamUpClients:1;
uint8_t EFAllowHookEmuClients:1;
uint8_t EFAllowLegitClients:1;
uint8_t EFAllowSettiServerScanner:1;
uint8_t EFAllowUnknownClients:1;
uint8_t EFAllowUnknownLegitSimulatingClients:1;
uint8_t EFAllowUnknownNonSteamSimulatingClients:1;
uint8_t EFAAllowLegacyWONHL1Clients:1;
uint8_t EFAAlloweSTEAMATiONHL1WONClients:1;

/*
  Additional security configuration for special clients
                                                        */
uint8_t EFUseIPGenerationForLegit:1;
uint8_t EFUseIPGenerationForAllNonLegit:1;
uint8_t EFLogClientTypeOnConnect:1;
uint8_t EFLogRejectedClientTypes:1;
uint8_t EFForceUseOfSharedID:1;
uint8_t EFSteamEmuOverallCompat:1;
uint8_t EFRejectAfterInitialValidation:1;
uint8_t EFForceMineSTSemiSteamVersion:1;

/*
  SeS client configuration parameters
                                       */

uint8_t EFForceInetListenServer:1;
uint8_t EFSeSAuthenticateViaValveAUTHServer:1;
uint8_t EFDisableGameFilesAutoUpdating:1;
uint8_t EFFilterValveTestApps:1;

uint8_t EFUseMsgInAdditionToLogForLogging:1;

uint8_t EFPrintConfigurationOnLoad:1;
uint8_t EFPrintConfigurationOnValidationInit:1;
uint8_t EFForceUseOfSharedIDForID2IPCLients:1;
uint8_t EFUseSteamWorksNonSteamV35UserIDTicket:1;


/*
   eSTEAMATiON   modules
                                        */
uint8_t EFModulemVUPLoad:1;
uint8_t EFModuleslibSCILoad:1;
}
TESTEAMATiONConfig;


int LoadEmulatorConfig(TESTEAMATiONConfig *Config);
int IsLibraryInitialized();
void eST_PrintConfig();


STEAM_API TESTEAMATiONConfig STEAM_CALL Get_eSTConfig();
STEAM_API uint8_t STEAM_CALL PLUGINS_eSTIsInitialized();


}

#endif

