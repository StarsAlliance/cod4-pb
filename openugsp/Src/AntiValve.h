#ifndef _ANTIVALVE_H
#define _ANTIVALVE_H

#if !defined(_WIN32) && !defined(_WIN64)

#ifdef HL1EMUBUILD
#include <sys/stat.h>
#endif

extern "C" {

//Emu loading support(Source)
/*
void _ZNK3BSL10CException6FormatEbPKc();
void _ZTIN3BSL10CExceptionE();
void _ZTVN3BSL10CExceptionE();
void _ZNK3BSL13CUnpackedTime5ToStrEPh();
void _ZN3BSL5CTime3NowEv();
void _ZN3BSL13CUnpackedTime7FromStrEPKhj();
void _ZN3BSL5CTime16FromUnpackedTimeERKNS_13CUnpackedTimeE();
void _ZNK3BSL5CTime14ToUnpackedTimeENS_14ETimePrecisionE();
*/

//extern int stat(const char * path, struct stat * buf);

}

#endif

#endif
