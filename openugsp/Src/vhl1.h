#ifndef _VHL1_H_
#define _VHL1_H_

#include <string.h>
#include "vupshared.h"
#include "vlvshared.h"


#define VLVHL1SteamValidation_Src ""

#define VLVHL1SteamValidation_Win_Dst ""
#define VLVHL1SteamValidation_UNiX_Dst ""
#define VLVHL1SteamValidation_UNiX_Dst_AMD ""
#define VLVHL1SteamValidation_UNiX_Dst_AMD64 ""



#define VLVHL1CDKeyValidation_Src ""

#define VLVHL1CDKeyValidation_Win_Dst ""
#define VLVHL1CDKeyValidation_UNiX_Dst ""
#define VLVHL1CDKeyValidation_UNiX_Dst_AMD ""
#define VLVHL1CDKeyValidation_UNiX_Dst_AMD64 ""



#define VLVHL1ValidationTypeChk_Win_Src ""

#define VLVHL1ValidationTypeChk_Win_Dst ""
#define VLVHL1ValidationTypeChk_UNiX_Dst ""
#define VLVHL1ValidationTypeChk_UNiX_Dst_AMD ""
#define VLVHL1ValidationTypeChk_UNiX_Dst_AMD64 ""



#define VLVHL1LANChk_Src ""


#define VLVHL1LANChk_WinDst ""
#define VLVHL1LANChk_UNiXDst ""

#define VLVHL1LANChk_UNiXDstAMD ""
#define VLVHL1LANChk_UNiXDstAMD64 ""


#define VLVHL1SteamIDJunction ""
#define VLVHL1JMPTabHelper ""

// the following patterns are reserved for HLShield compat additional patches:
#define VLVHL1AuthenticationTypeChk_Win_Src ""

#define VLVHL1AuthenticationTypeChk_Win_Dst ""
#define VLVHL1AuthenticationTypeChk_UNiX_Dst ""
#define VLVHL1AuthenticationTypeChk_UNiX_Dst_AMD ""
#define VLVHL1AuthenticationTypeChk_UNiX_Dst_AMD64 ""



#define VLVHL1SteamUserIDTicket1_Win_Src ""

#define VLVHL1SteamUserIDTicket1_Win_Dst ""
#define VLVHL1SteamUserIDTicket1_UNiX_Dst ""
#define VLVHL1SteamUserIDTicket1_UNiX_Dst_AMD ""
#define VLVHL1SteamUserIDTicket1_UNiX_Dst_AMD64 ""



#define VLVHL1SteamUserIDTicket2_Win_Src ""

#define VLVHL1SteamUserIDTicket2_Win_Dst ""
#define VLVHL1SteamUserIDTicket2_UNiX_Dst ""
#define VLVHL1SteamUserIDTicket2_UNiX_Dst_AMD ""
#define VLVHL1SteamUserIDTicket2_UNiX_Dst_AMD64 ""





TBOOL IfHL1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,char Version[150]);

void PatchHL1Engine(TSTREAMADDR_HDR const fileStream,const TVUP_OSType fileOS,TBOOL PromtUser,TBOOL ClientChecks,TBOOL *PatchingResult,char Version[150]);


#endif

