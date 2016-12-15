#ifndef __OPPTRNBASEINT_H__
#define __OPPTRNBASEINT_H__

#include "opportability.h"
#include "opptrnbase.h"
#include "operror.h"
#include "opstream.h"

#ifdef cplusplus
  extern "C" {
#endif

TOPError OPLookupPatternAtStart(TSTREAMADDR_NOHDR StreamStartPtr,
                                uint64_t StreamStartToEndLength,
                                TPTRN DesiredPtrnToFind,
                                volatile TBOOL *const Result);


TOPError OPCmpOptionalData(TSTREAMADDR_NOHDR cutStream,TPTRN Pattern,uint64_t StreamStartToEndLength,TVAADDR *PatternChkStartEndOffset,TVAADDR *EndOfOptData,TBOOL *Result);
TOPError OPCmpLogicalData(TSTREAMADDR_NOHDR cutStream,TPTRN Pattern,uint64_t StreamStartToEndLength,TVAADDR *PatternChkStartEndOffset,TVAADDR *EndOfOptData,TBOOL *Result);

#ifdef cplusplus
  }
#endif

#endif
