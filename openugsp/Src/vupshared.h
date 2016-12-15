#ifndef _VUPSHARED_H_
#define _VUPSHARED_H_


#include <inttypes.h>
#include "opbininfo.h"


typedef enum {
    VUP_UNRECOGNIZED = 0,
    VUP_VVOLDMODE = 1,
    VUP_VVSOURCE = 2,
    VUP_VVSOURCE2K7 = 3,
    VUP_ETQW = 4,
    VUP_UT3 = 5
}TVOP_OPERMODE;

typedef enum
{
    VUP_Win32 = 0,
    VUP_Linux = 1,
    VUP_Win32_HL1Classic = 2
} TVUP_OSType;

typedef enum
{
    VUP_DedicatedServer = 0,
    VUP_ListenServer = 1,
    VUP_Client = 2
} TVUP_DestType;



#endif
