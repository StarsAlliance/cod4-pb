#ifndef __mVUPERROR_H__
#define __mVUPERROR_H__

#include "opportability.h"

#ifdef cplusplus
  extern "C" {
#endif

typedef enum
{
  EmVUPSuccessfullOperation = 0,
  EmVUPVirtualMemoryReadError = 1,
  EmVUPDataIsNotAModule = 2,
  EmVUPUnrecognizedModule = 3,
  EmVUPUnsupportedApplication = 4,
  EmVUPUnableToSaveTheFile = 5,
  EmVUPPatchingFailure = 6
}TmVUPError;

#ifdef cplusplus
  }
#endif

#endif
