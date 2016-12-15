#ifndef __OPERROR_H__
#define __OPERROR_H__

#include "opportability.h"

#ifdef cplusplus
  extern "C" {
#endif

typedef enum
{
  EOPSuccessfullOperation = 0,
  EOPPatternDataMismatchesItsType = 1,
  EOPInvalidCharactersInUserPassedPattern = 2,
  EOPPatternMustConsistOfEvenNumberOfElements = 3,
  EOPInvalidDataInOPPattern = 4,
  EOPInvalidTypeValueInOPPattern = 5,
  EOPInvalidLookUpType = 6,
  EOPUseOfOddSequenceOfWildcardsInBytePatternsIsProhibited = 7,
  EOPLengthOfOPPatternMustBeEven = 8,
  EOPPatternTypeConversionFromHBP_To_BP_CannotBePerformed = 9,
  EOPPatternsOfDifferentTypesFoundInPair = 10,
  EOPPatternsOfDifferentLengthsFoundInPair = 11,
  EOPErrorInSourcePatternOfPairHasBeenDetected = 12,
  EOPErrorInDestinationPatternOfPairHasBeenDetected = 13,
  EOPCannotWritePatternData = 14,
  EOPCannotWriteMemoryStreamToFile = 15,
  EOPErrorOpeningSpecifiedFile = 16,
  EOPCannotLoadTheWholeFileIntoMemory = 17,
  EOPAttemptToReferenceMemoryAtNULLAddress = 18,
  EOPCannotAllocateMemoryResources = 19,
  EOPAttemptToGetOutsideOfStreamBounds = 20,
  EOPAttemtToWorkWithNULLedPattern = 21,
  EOPRequestedObjectNotFound = 22,
  EOPMarkersSetToIncorrectValues = 23
}TOPError;

#ifdef cplusplus
  }
#endif

#endif
