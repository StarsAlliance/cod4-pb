#include "vhl1.h"

extern TBOOL eSTEAMATiON_Prep_Mode;

TBOOL IfHL1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[150])
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
      snprintf(Version,150,"Valve engine\nProtocol version %i\nExe version %s (ModName)\nExe build: %s %s (%i)\n",
              proto,eng_version,eng_time,eng_date,build_nr);
      return TB_TRUE;
    }
   else
    return TB_FALSE;
}

void PatchHL1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[150])
{
}


