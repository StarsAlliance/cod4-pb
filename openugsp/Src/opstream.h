#ifndef __OPSTREAM_H__
#define __OPSTREAM_H__


#include <stdio.h>
#include <stdlib.h>


#include "opportability.h"
#include "opptrnbase.h"
#include "operror.h"

#ifdef cplusplus
  extern "C" {
#endif

typedef uint8_t * TSTREAMADDR_HDR, *TSTREAMADDR_NOHDR;
typedef uint64_t TVAADDR;
typedef enum
{
  TLOOKUPSTATICOFFSET = 0,
  TLOOKUPSAR = 1
}TLOOKUPTYPE;

TOPError LIBFUNC OPLoadFile(const char *f_name,
                            TSTREAMADDR_HDR *const FileStream);
TOPError LIBFUNC OPSaveFile(const char *f_name,
                            TSTREAMADDR_HDR *const FileStream,
                            const TBOOL LeaveStreamIntact);
TOPError LIBFUNC OPFindPtrn(const TSTREAMADDR_HDR SrcStream,
                            const TPTRN DesiredPtrnToFind,
                            const uint64_t Desired_Occurence,
                            const TLOOKUPTYPE DesiredLkupType,
                            const TBOOL EnableReverseLookup,
                            const TVAADDR DesiredStaticOffset,
                            TVAADDR *const FoundAt,
                            TBOOL *const IfFound);
TOPError LIBFUNC OPJumpToOffsetInStream(const TSTREAMADDR_HDR fStream,
                                        const TVAADDR DesiredOffset,
                                        TSTREAMADDR_NOHDR *const ResultedExtStream,
                                        uint64_t *const AmmoundOfBytesToEndOfStream);
TOPError LIBFUNC OPWritePtrn(const TSTREAMADDR_HDR SrcStream,
                             const TVAADDR DesiredLocation,
                             const TPTRN DesiredPtrnToWrite);

#define OP_MODIFY_START_OFFSET_MARKER 1
#define OP_MODIFY_END_OFFSET_MARKER 2
#define OP_MODIFY_BOTH_MARKERS 3

TOPError LIBFUNC OPGetLastOffsetInStream(const TSTREAMADDR_HDR fStream,
                                         TVAADDR *const LastOffset);

TOPError LIBFUNC OPSetSearchMarkers(const TSTREAMADDR_HDR fStream,
                                    const TVAADDR StartOff,
                                    const TVAADDR EndOff,
                                    const uint8_t ModFlag);

TOPError LIBFUNC OPGetSearchMarkers(const TSTREAMADDR_HDR fStream,
                                    TVAADDR *const StartOff,
                                    TVAADDR *const EndOff);

TOPError LIBFUNC OPSetSearchWithMarkersState(const TSTREAMADDR_HDR fStream,
                                             const TBOOL EnableSearchWithMarkers);

TOPError LIBFUNC OPGetSearchWithMarkersState(const TSTREAMADDR_HDR fStream,
                                             uint8_t *const MarkerState);

#ifdef cplusplus
  }
#endif

#include "opstreamint.h"


#endif
