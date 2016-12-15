#ifndef __OPPTRNBASE_H__
#define __OPPTRNBASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opportability.h"
#include "operror.h"

#ifdef cplusplus
  extern "C" {
#endif

/* Base Pattern Handling constants */
#define BytePatternEnd 0xFFFF
#define HalfBytePatternEnd 0xFF
#define ByteWildcard 0x100
#define HalfByteWildcard 0x10

/* Extended constants to support optional-subpatterns and logic OR between 2 subpatterns */

#define BytePatternOptionalBlockStart 0x101
#define HalfBytePatternOptionalBlockStart 0x11
#define BytePatternOptionalBlockEnd 0x102
#define HalfBytePatternOptionalBlockEnd 0x12

#define BytePatternLogicalBlockStart 0x103
#define HalfBytePatternLogicalBlockStart 0x13
#define BytePatternLogicalBlockEnd 0x105
#define HalfBytePatternLogicalBlockEnd 0x15
#define BytePatternLogicalXOROperator 0x104
#define HalfBytePatternLogicalXOROperator 0x14


/* Old classic PTRN INTERFACE1 system */

typedef enum
{
  TU_UseBytePatterns = 0,
  TU_UseHalfBytePatterns = 1
} TPTRN_TYPE;

typedef enum
{
  TB_FALSE = 0,
  TB_TRUE = 1,
}TBOOL;
typedef uint16_t *TPTRNB;

typedef uint8_t *TPTRNHB;

typedef struct {
  TPTRN_TYPE PtrnType;

  union {
   TPTRNB PtrnB;
   TPTRNHB PtrnHB;
  };
}TPTRN_10;

#ifndef OP_PTRN_VERSION
 #define OP_PTRN_VERSION 1
#endif

#if (OP_PTRN_VERSION == 2)
 #define OPParseUserPtrn OPParseUserPtrn_20
 #define OPValidatePattern OPValidatePattern_20
 #define OPFreePatternMemory OPFreePatternMemory_20
 #define TPTRN TPTRN_20
#else
 #if (OP_PTRN_VERSION == 1)
   #define OPParseUserPtrn OPParseUserPtrn_10
   #define OPValidatePattern OPValidatePattern_10
   #define OPFreePatternMemory OPFreePatternMemory_10
   #define TPTRN TPTRN_10
 #endif
#endif

TOPError LIBFUNC OPParseUserPtrn_10(const char *InputPtrn,
                                 const TPTRN_TYPE DesiredPattern,
                                 TPTRN_10 *const ParsedPtrn);

TOPError LIBFUNC OPValidatePattern_10(const TPTRN_10 PatternToValidate,
                                   uint64_t *const PatternLength);

TOPError LIBFUNC OPFreePatternMemory_10(TPTRN_10 *const Pattern);

/* New optimized PTRN INTERFACE2 system */

typedef enum
{
  TU_SUBPTRN2_TYPICAL = 0, /* Typical sub pattern with only pat data and special mask */
  TU_SUBPTRN2_OPTIONAL = 1, /* This pat is declared as optional and may not be present in scanned data */
  TU_SUBPTRN2_LOGICAL = 2, /* This is set of mini-pats there only one should be present in the scanned data */
} TSUBPTRN2_TYPE;

typedef struct TPTRN2_TAG
{
  TSUBPTRN2_TYPE op_SubPatType;
  struct op_SimplePatternInfo_TAG {
    uint8_t *op_PatData;
    uint8_t *op_PatWildCrdMask;
   #if (_OP_128BIT_REGISTERS_AVAILABLE_)
    uint64_t op_uAmmountOfDQWordsInPatData;
   #endif
   #if defined (_WIN64) || defined (_OP_64BIT_REGISTERS_AVAILABLE_)
    uint64_t op_uAmmountOfQWordsInPatData;
   #endif
    uint64_t op_uAmmountOfDWordsInPatData;
    uint64_t op_uAmmountOfWordsInPatData;
    uint64_t op_bIsSingleBytePresentInData;
  } op_SimplePatternInfo;
  struct TPTRN2 *op_LogicalXORPatterns;
  uint64_t *op_uAmmountOfLogicalXORPatterns;
  struct TPTRN2 *op_NextSubPattern;
}TPTRN_20;

TOPError LIBFUNC OPParseUserPtrn_20(const char *InputPtrn,
                                 const TPTRN_TYPE DesiredPattern,
                                 TPTRN_20 *const ParsedPtrn);
TOPError LIBFUNC OPValidatePattern_20(const TPTRN_20 PatternToValidate,
                                   uint64_t *const PatternLength);

TOPError LIBFUNC OPFreePatternMemory_20(TPTRN_20 *const Pattern);

TOPError LIBFUNC OPParseUserPtrnInternal_20(const char *PreParsedInputPtrn,
                                            const TPTRN_TYPE DesiredPattern,
                                            TPTRN_20 *const ParsedPtrn);



#ifdef cplusplus
  }
#endif

#endif
