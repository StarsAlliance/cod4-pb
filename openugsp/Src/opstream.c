#include "opstream.h"

#ifdef _MSC_VER
  #pragma warning( disable : 4996 )
#endif

TOPError LIBFUNC OPLoadFile(const char *f_name,
                            TSTREAMADDR_HDR *const FileStream)
{
  FILE *f_handle;
  uint64_t f_size;
  TSTREAMADDR_NOHDR intStream;
  if ((!f_name) || (!FileStream))
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  f_handle = fopen(f_name,"rb");
  if (!f_handle)
   return EOPErrorOpeningSpecifiedFile;
  fseek(f_handle, 0L, SEEK_END);     /* Position to end of file */
  f_size = ftell(f_handle);        /* Get file length */
  rewind(f_handle);
  *FileStream = (TSTREAMADDR_HDR) malloc(f_size + 3*sizeof(uint64_t) + sizeof(uint8_t));
  if (!*FileStream)
   {
     fclose(f_handle);
     return EOPCannotAllocateMemoryResources;
   }
  intStream = (*FileStream) + 3*sizeof(uint64_t) + sizeof(uint8_t);
  if (fread((void *)(intStream),(size_t)f_size,(size_t)1,f_handle) != 1)
   {
     fclose(f_handle);
     free(*FileStream);
     *FileStream = NULL;
     return EOPCannotLoadTheWholeFileIntoMemory;
   }
  fclose(f_handle);
  *((uint64_t *)(*FileStream)) = f_size;
  *((uint64_t *)((*FileStream) + sizeof(uint64_t))) = 0;
  *((uint64_t *)((*FileStream) + 2*sizeof(uint64_t))) = 0;
  *((uint8_t *)((*FileStream) + 3*sizeof(uint64_t))) = TB_FALSE;
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPSaveFile(const char *f_name,
                            TSTREAMADDR_HDR *const FileStream,
                            const TBOOL LeaveStreamIntact)
{
  TSTREAMADDR_NOHDR intStream;
  FILE *f_handle;
  if (!f_name || !FileStream)
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  if (!*FileStream)
   {
     return EOPAttemptToReferenceMemoryAtNULLAddress;
   }
  f_handle = fopen(f_name,"wb+");
  if (!f_handle)
   return EOPErrorOpeningSpecifiedFile;
  intStream = (*FileStream) + 3*sizeof(uint64_t) + sizeof(uint8_t);
  if (fwrite((void *)intStream,*((uint64_t *)(*FileStream)),(size_t)1,f_handle) != 1)
   {
     fclose(f_handle);
     if (!LeaveStreamIntact)
      {
        free(*FileStream);
        *FileStream = NULL;
      }
     return EOPCannotWriteMemoryStreamToFile;
   }
  fclose(f_handle);
  if (!LeaveStreamIntact)
   {
     free(*FileStream);
     *FileStream = NULL;
   }
  return EOPSuccessfullOperation;
}


TOPError LIBFUNC OPFindPtrn(const TSTREAMADDR_HDR SrcStream,
                            const TPTRN DesiredPtrnToFind,
                            const uint64_t Desired_Occurence,
                            const TLOOKUPTYPE DesiredLkupType,
                            const TBOOL EnableReverseLookup,
                            const TVAADDR DesiredStaticOffset,
                            TVAADDR *const FoundAt,
                            TBOOL *const IfFound)
{
  TSTREAMADDR_NOHDR BasePtr;
  uint64_t counter = 0;
  uint8_t mark_state;
  TVAADDR mark_start = 0,mark_end = 0;
  TBOOL Result;
  TOPError intError;
  if (!IfFound || !SrcStream || !(DesiredPtrnToFind.PtrnB))
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  BasePtr = SrcStream + 3*sizeof(uint64_t) + sizeof(uint8_t);
  if (FoundAt)
    *FoundAt = (TVAADDR)0;
  *IfFound = TB_FALSE;
  OPGetSearchWithMarkersState(SrcStream,&mark_state);
  if (mark_state)
   {
     OPGetSearchMarkers(SrcStream,&mark_start,&mark_end);
   }
  if (!mark_end || !mark_state)
      mark_end = (*((uint64_t *)SrcStream)) - 1;
  if (DesiredLkupType == TLOOKUPSAR)
    {
      if (!EnableReverseLookup)
       {
          if (mark_state)
            BasePtr += mark_start;
          while (BasePtr <= SrcStream + 3*sizeof(uint64_t) + sizeof(uint8_t) + mark_end)
           {
             intError = OPLookupPatternAtStart(BasePtr,mark_end + 1 - (BasePtr - SrcStream - 3*sizeof(uint64_t) - sizeof(uint8_t)),DesiredPtrnToFind,&Result);
             if (intError)
               return intError;
             if (Result)
              {
                counter ++;
                if (counter == Desired_Occurence)
                  {
                    if (FoundAt)
                      *FoundAt = (TVAADDR)(BasePtr - SrcStream - 3*sizeof(uint64_t) - sizeof(uint8_t));
                    *IfFound = TB_TRUE;
                    break;
                  }
              }
             BasePtr ++;
           }
       }
      else
       {
          if (mark_state)
            BasePtr += mark_end;
          else
            BasePtr += (*((uint64_t *)SrcStream)) - 1;
          while (BasePtr >= SrcStream + 3*sizeof(uint64_t) + sizeof(uint8_t) + mark_start)
           {
             intError = OPLookupPatternAtStart(BasePtr,mark_end + 1 - (BasePtr - SrcStream - 3*sizeof(uint64_t) - sizeof(uint8_t)),DesiredPtrnToFind,&Result);
             if (intError)
               return intError;
             if (Result)
              {
                counter ++;
                if (counter == Desired_Occurence)
                  {
                    if (FoundAt)
                      *FoundAt = (TVAADDR)(BasePtr - SrcStream - 3*sizeof(uint64_t) - sizeof(uint8_t));
                    *IfFound = TB_TRUE;
                    break;
                  }
              }
             BasePtr --;
           }
       }
   }
  else
    if (DesiredLkupType == TLOOKUPSTATICOFFSET)
     {
       intError = OPLookupPatternAtStart(BasePtr + DesiredStaticOffset,mark_end + 1 - DesiredStaticOffset,DesiredPtrnToFind,&Result);
       if (intError)
         return intError;
       if (Result)
        {
           if (FoundAt)
             *FoundAt = DesiredStaticOffset;
           *IfFound = TB_TRUE;
        }
     }
    else
     return EOPInvalidLookUpType;
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPJumpToOffsetInStream(const TSTREAMADDR_HDR fStream,
                                        const TVAADDR DesiredOffset,
                                        TSTREAMADDR_NOHDR *const ResultedExtStream,
                                        uint64_t *const AmmoundOfBytesToEndOfStream)
{
  if (!ResultedExtStream || !AmmoundOfBytesToEndOfStream)
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  if ( *((uint64_t *)fStream) <= DesiredOffset)
   {
      *ResultedExtStream = NULL;
      *AmmoundOfBytesToEndOfStream = 0;
      return EOPAttemptToGetOutsideOfStreamBounds;
   }
  *ResultedExtStream = fStream + 3*sizeof(uint64_t) + sizeof(uint8_t) + DesiredOffset;
  *AmmoundOfBytesToEndOfStream = *((uint64_t *)fStream) - DesiredOffset;
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPWritePtrn(const TSTREAMADDR_HDR SrcStream,
                             const TVAADDR DesiredLocation,
                             const TPTRN DesiredPtrnToWrite)
{
  TSTREAMADDR_NOHDR BaseAddr;
  uint64_t i = 0;
  uint8_t HBYTE;
  if (!SrcStream)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  BaseAddr = SrcStream + 3*sizeof(uint64_t) + sizeof(uint8_t) + DesiredLocation;
  if (DesiredPtrnToWrite.PtrnType == TU_UseBytePatterns)
   {
     if (!DesiredPtrnToWrite.PtrnB)
       return EOPAttemtToWorkWithNULLedPattern;
     while (DesiredPtrnToWrite.PtrnB[i] != BytePatternEnd)
      {
        if ((i + DesiredLocation) >= *((uint64_t *)(SrcStream)))
         {
            return EOPCannotWritePatternData;
         }
        if (DesiredPtrnToWrite.PtrnB[i] == ByteWildcard)
         {
           i ++;
           continue;
         }
        BaseAddr[i] = (uint8_t) DesiredPtrnToWrite.PtrnB[i];
        i ++;
      }
   }
  else
   {
     if (!DesiredPtrnToWrite.PtrnHB)
       return  EOPAttemtToWorkWithNULLedPattern;
     while (DesiredPtrnToWrite.PtrnHB[i] != HalfBytePatternEnd)
      {
        if ((i + DesiredLocation) >= *((uint64_t *)(SrcStream)))
         {
            return EOPCannotWritePatternData;
         }
        if (DesiredPtrnToWrite.PtrnHB[i] == HalfByteWildcard)
         {
           i ++;
           continue;
         }
        HBYTE = (i % 2) ? (BaseAddr[i >> 1] & 0xF0) + DesiredPtrnToWrite.PtrnHB[i] : (BaseAddr[i >> 1] & 0x0F) + (DesiredPtrnToWrite.PtrnHB[i] << 4);
        BaseAddr[i >> 1] = HBYTE;
        i ++;
      }
   }
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPGetLastOffsetInStream(const TSTREAMADDR_HDR fStream,
                                         TVAADDR *const LastOffset)
{
  if (!fStream || !LastOffset)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  *LastOffset = *((uint64_t *)(fStream)) - 1;
  return EOPSuccessfullOperation;
}


TOPError LIBFUNC OPSetSearchMarkers(const TSTREAMADDR_HDR fStream,
                                    const TVAADDR StartOff,
                                    const TVAADDR EndOff,
                                    const uint8_t ModFlag)
{
  if (!fStream)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  if ((ModFlag == OP_MODIFY_BOTH_MARKERS) && (StartOff > EndOff) && (EndOff > 0))
    return EOPMarkersSetToIncorrectValues;
  if (ModFlag & OP_MODIFY_START_OFFSET_MARKER)
   {
     if (StartOff >= (*((uint64_t *) fStream)))
       return EOPMarkersSetToIncorrectValues;
     *((uint64_t *)(fStream + sizeof(uint64_t))) = StartOff;
   }

  if (ModFlag & OP_MODIFY_END_OFFSET_MARKER)
   {
     if (EndOff >= (*((uint64_t *) fStream)))
       return EOPMarkersSetToIncorrectValues;
     *((uint64_t *)(fStream + 2*sizeof(uint64_t))) = EndOff;
   }
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPGetSearchMarkers(const TSTREAMADDR_HDR fStream,
                                    TVAADDR *const StartOff,
                                    TVAADDR *const EndOff)
{
  if (!fStream || !StartOff || !EndOff)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  *StartOff = *((uint64_t *)(fStream + sizeof(uint64_t)));
  *EndOff = *((uint64_t *)(fStream + 2*sizeof(uint64_t)));
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPSetSearchWithMarkersState(const TSTREAMADDR_HDR fStream,
                                             const TBOOL EnableSearchWithMarkers)
{
  if (!fStream)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  if (EnableSearchWithMarkers)
    *((uint8_t *)(fStream + 3*sizeof(uint64_t))) = 1;
  else
    *((uint8_t *)(fStream + 3*sizeof(uint64_t))) = 0;
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPGetSearchWithMarkersState(const TSTREAMADDR_HDR fStream,
                                             uint8_t *const MarkerState)
{
  if (!fStream || !MarkerState)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  *MarkerState = *((uint8_t *)(fStream + 3*sizeof(uint64_t)));
  return EOPSuccessfullOperation;
}
