#ifndef _OP_PORTABILITY_H_
#define _OP_PORTABILITY_H_

#ifdef cplusplus
  extern "C" {
#endif


#ifndef __NOLIBBUILD__
#ifdef _WIN32 /* If compiled under MS Windows environment for 32bit target*/

  #if BUILD_DLL || BUILDING_DLL
    #define LIBFUNC __declspec (dllexport)
  #else /* Not BUILDING_DLL */
    #define LIBFUNC __declspec (dllimport)
  #endif /* Not BUILDING_DLL */
#else
  #ifdef _WIN64 /* If compiled under MS Windows environment for 64bit target */

     #if BUILD_DLL || BUILDING_DLL
       #define LIBFUNC __declspec (dllexport)
     #else /* Not BUILDING_DLL */
       #define LIBFUNC __declspec (dllimport)
     #endif /* Not BUILDING_DLL */
  #else
     #define LIBFUNC /* If compiled under Unix-like OS */
  #endif
#endif
#else
 #define LIBFUNC /* If compiled streigth into software */
#endif

#if defined(_MSC_VER) && !defined(__PellesC__) /* If Microsoft Visual C compiler is used to compile the library */
/* If we are using Microsoft VS 2005/2008 compiler then we need to define some libstdc types which are not found in msvcrt */
   typedef __int8 int8_t;
   typedef unsigned __int8 uint8_t;
   typedef __int16 int16_t;
   typedef unsigned __int16 uint16_t;
   typedef __int32  int32_t;
   typedef unsigned __int32 uint32_t;
   typedef __int64 int64_t;
   typedef unsigned __int64 uint64_t;
#else
   #include <stdint.h>
   #define _FILE_OFFSET_BITS 64
   #define _LARGE_FILES
   #if !defined(BOOL) && !defined(TDM_GCC)
    #define BOOL int
   #endif
   #ifndef __int8
    #define __int8 int8_t
   #endif
   #ifndef __int16
    #define __int16 int16_t
   #endif
   #ifndef __int32
    #define __int32 int32_t
   #endif
   #ifndef __int64
    #define __int64 int64_t
   #endif
#endif  /* stdint.h defines for stupid MS compilers */

#include <string.h>

#ifdef _FILE_OFFSET_BITS
#undef _FILE_OFFSET_BITS
#endif
#define _FILE_OFFSET_BITS 64

#ifdef cplusplus
  }
#endif

#endif
