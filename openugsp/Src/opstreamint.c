#include "opstreamint.h"

TOPError OPLookupPatternAtStart(TSTREAMADDR_NOHDR StreamStartPtr,
                                uint64_t StreamStartToEndLength,
                                TPTRN DesiredPtrnToFind,
                                volatile TBOOL *const Result)
{
  uint64_t i,j = 0,tmp;
  TSTREAMADDR_NOHDR intStream;
  TOPError tmpErr;
  TBOOL tmpRes;
  if (!StreamStartPtr || !Result)
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  *Result = TB_TRUE;
  intStream = StreamStartPtr;
  if (DesiredPtrnToFind.PtrnType == TU_UseBytePatterns)
   {
     for (i = 0; i < StreamStartToEndLength;i ++)
      {
         switch (DesiredPtrnToFind.PtrnB[j])
          {
             case ByteWildcard:
                               j ++;
                               break;
             case BytePatternEnd:
                                 return EOPSuccessfullOperation;
             case BytePatternOptionalBlockStart:
                                                tmpErr = OPCmpOptionalData(intStream + i,DesiredPtrnToFind,StreamStartToEndLength - i,&j,&tmp,&tmpRes);
                                                if (tmpErr)
                                                  return tmpErr;
                                                if (tmpRes)
                                                 {
                                                   i += tmp - 1;
                                                 }
                                                else
                                                  i --;
                                                break;

             case BytePatternLogicalBlockStart:
                                               tmpErr = OPCmpLogicalData(intStream + i,DesiredPtrnToFind,StreamStartToEndLength - i,&j,&tmp,&tmpRes);
                                               if (tmpErr)
                                                  return tmpErr;
                                               if (tmpRes)
                                                {
                                                  i += tmp - 1;
                                                }
                                               else
                                                {
                                                  *Result = TB_FALSE;
                                                  return EOPSuccessfullOperation;
                                                }
                                               break;

             default:
                     if (DesiredPtrnToFind.PtrnB[j] != intStream[i])
                      {
                        *Result = TB_FALSE;
                        return EOPSuccessfullOperation;
                      }
                     else
                      {
                        j ++;
                      }
          }
      }
     if ((i == StreamStartToEndLength) && (DesiredPtrnToFind.PtrnB[j] != BytePatternEnd))
      *Result = TB_FALSE;
     return EOPSuccessfullOperation;
   }
  else
   {
     uint8_t HByte;
     for (i = 0; i < (StreamStartToEndLength << 1);i ++)
      {
         switch (DesiredPtrnToFind.PtrnHB[j])
          {
             case HalfByteWildcard:
                                    j ++;
                                    break;
             case HalfBytePatternEnd:
                                      return EOPSuccessfullOperation;
             case HalfBytePatternOptionalBlockStart:
                                                tmpErr = OPCmpOptionalData(intStream + (i >> 1),DesiredPtrnToFind,StreamStartToEndLength - (i >> 1),&j,&tmp,&tmpRes);
                                                if (tmpErr)
                                                  return tmpErr;
                                                if (tmpRes)
                                                 {
                                                   i += tmp - 1;
                                                 }
                                                else
                                                  i --;
                                                break;

             case HalfBytePatternLogicalBlockStart:
                                               tmpErr = OPCmpLogicalData(intStream + (i >> 1),DesiredPtrnToFind,StreamStartToEndLength - (i >> 1),&j,&tmp,&tmpRes);
                                               if (tmpErr)
                                                  return tmpErr;
                                               if (tmpRes)
                                                {
                                                  i += tmp - 1;
                                                }
                                               else
                                                {
                                                  *Result = TB_FALSE;
                                                  return EOPSuccessfullOperation;
                                                }
                                               break;
             default:
                      HByte = (i % 2) ? (intStream[i >> 1] & 0x0F) : (intStream[i >> 1] >> 4);
                      if (DesiredPtrnToFind.PtrnHB[j] != HByte)
                       {
                          *Result = TB_FALSE;
                          return EOPSuccessfullOperation;
                       }
                      else
                       {
                           j ++;
                       }
          }
      }
     if ((i == (StreamStartToEndLength << 1)) && (DesiredPtrnToFind.PtrnHB[j] != HalfBytePatternEnd))
      *Result = TB_FALSE;
     return EOPSuccessfullOperation;
   }
}



TOPError OPCmpOptionalData(TSTREAMADDR_NOHDR cutStream,TPTRN Pattern,uint64_t StreamStartToEndLength,TVAADDR *PatternChkStartEndOffset,TVAADDR *EndOfOptData,TBOOL *Result)
{
    uint64_t i = 0,j = 1,tmp;
    TOPError tmpErr;
    TBOOL tmpRes;
    TVAADDR tmpPatOff;
    uint8_t HByte;
    if (!PatternChkStartEndOffset || !EndOfOptData || !Result)
      return EOPAttemptToReferenceMemoryAtNULLAddress;
    *Result = TB_TRUE;
    if (Pattern.PtrnType == TU_UseBytePatterns)
     {
       if (Pattern.PtrnB[*PatternChkStartEndOffset] != BytePatternOptionalBlockStart)
        {
            *Result = TB_FALSE;
            return EOPInvalidDataInOPPattern;
        }
       while (i < StreamStartToEndLength)
        {
          switch (Pattern.PtrnB[(*PatternChkStartEndOffset) + j])
           {
              case ByteWildcard:
                                i ++;
                                j ++;
                                break;

              case BytePatternEnd:
                                  *EndOfOptData = 0;
                                  *Result = TB_FALSE;
                                  return EOPInvalidDataInOPPattern;
              case BytePatternOptionalBlockEnd:
                                               *EndOfOptData = i;
                                               *PatternChkStartEndOffset += j + 1;
                                               return EOPSuccessfullOperation;
              case BytePatternOptionalBlockStart:
                                                 tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                 tmpErr = OPCmpOptionalData(cutStream + i,Pattern,StreamStartToEndLength - i,&tmpPatOff,&tmp,&tmpRes);
                                                 if (tmpErr)
                                                   return tmpErr;
                                                 if (tmpRes)
                                                    i += tmp;
                                                 j = tmpPatOff - (*PatternChkStartEndOffset);
                                                 break;
              case BytePatternLogicalBlockStart:
                                                 tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                 tmpErr = OPCmpLogicalData(cutStream + i,Pattern,StreamStartToEndLength - i,&tmpPatOff,&tmp,&tmpRes);
                                                 if (tmpErr)
                                                   return tmpErr;
                                                 if (tmpRes)
                                                    i += tmp;
                                                 j = tmpPatOff - (*PatternChkStartEndOffset);
                                                 break;

              default:
                      if (Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != cutStream[i])
                       {
                           *Result = TB_FALSE;
                           do {
                              j ++;
                              if (Pattern.PtrnB[(*PatternChkStartEndOffset) + j] == BytePatternEnd)
                                return EOPInvalidDataInOPPattern;
                           }
                           while (Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != BytePatternOptionalBlockEnd);
                           *EndOfOptData = 0;
                           *PatternChkStartEndOffset += j + 1;
                           return EOPSuccessfullOperation;
                       }
                      i ++;
                      j ++;

           }
        }
       if ((i == StreamStartToEndLength) && (Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != BytePatternEnd))
        *Result = TB_FALSE;
     }
    else
     {
        if (Pattern.PtrnHB[*PatternChkStartEndOffset] != HalfBytePatternOptionalBlockStart)
        {
            *Result = TB_FALSE;
            return EOPInvalidDataInOPPattern;
        }
       while (i < (StreamStartToEndLength << 1))
        {
          switch (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j])
           {
              case HalfByteWildcard:
                                    i ++;
                                    j ++;
                                    break;

              case HalfBytePatternEnd:
                                  *EndOfOptData = 0;
                                  *Result = TB_FALSE;
                                  return EOPInvalidDataInOPPattern;
              case HalfBytePatternOptionalBlockEnd:
                                                    *EndOfOptData = i;
                                                    *(PatternChkStartEndOffset) += j + 1;
                                                    return EOPSuccessfullOperation;
              case HalfBytePatternOptionalBlockStart:
                                                     tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                     tmpErr = OPCmpOptionalData(cutStream + (i >> 1),Pattern,StreamStartToEndLength - (i >> 1),&tmpPatOff,&tmp,&tmpRes);
                                                     if (tmpErr)
                                                       return tmpErr;
                                                     if (tmpRes)
                                                       i += tmp;
                                                     j = tmpPatOff - (*PatternChkStartEndOffset);
                                                       break;
              case HalfBytePatternLogicalBlockStart:
                                                     tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                     tmpErr = OPCmpLogicalData(cutStream + (i >> 1),Pattern,StreamStartToEndLength - (i >> 1),&tmpPatOff,&tmp,&tmpRes);
                                                     if (tmpErr)
                                                       return tmpErr;
                                                     if (tmpRes)
                                                       i += tmp;
                                                     j = tmpPatOff - (*PatternChkStartEndOffset);
                                                       break;

              default:
                      HByte = (i % 2) ? (cutStream[i >> 1] & 0x0F) : (cutStream[i >> 1] >> 4);
                      if (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HByte)
                       {
                           *Result = TB_FALSE;
                           do {
                              j ++;
                              if (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] == HalfBytePatternEnd)
                                return EOPInvalidDataInOPPattern;
                           }
                           while (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HalfBytePatternOptionalBlockEnd);
                           *EndOfOptData = 0;
                           *PatternChkStartEndOffset += j + 1;
                           return EOPSuccessfullOperation;
                       }
                      i ++;
                      j ++;

           }
        }
       if ((i == (StreamStartToEndLength << 1)) && (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HalfBytePatternEnd))
        *Result = TB_FALSE;
     }
    return EOPSuccessfullOperation;
}


TOPError OPCmpLogicalData(TSTREAMADDR_NOHDR cutStream,TPTRN Pattern,uint64_t StreamStartToEndLength,TVAADDR *PatternChkStartEndOffset,TVAADDR *EndOfOptData,TBOOL *Result)
{
   uint64_t i = 0,j = 1,tmp;
    TOPError tmpErr;
    TBOOL tmpRes;
    TVAADDR tmpPatOff;
    uint8_t HByte;
    if (!PatternChkStartEndOffset || !EndOfOptData || !Result)
      return EOPAttemptToReferenceMemoryAtNULLAddress;
    *Result = TB_TRUE;
    if (Pattern.PtrnType == TU_UseBytePatterns)
     {
       if (Pattern.PtrnB[*PatternChkStartEndOffset] != BytePatternLogicalBlockStart)
        {
            *Result = TB_FALSE;
            return EOPInvalidDataInOPPattern;
        }
       while (i < StreamStartToEndLength)
        {
          switch (Pattern.PtrnB[(*PatternChkStartEndOffset) + j])
           {
              case ByteWildcard:
                                i ++;
                                j ++;
                                break;

              case BytePatternEnd:
                                  *EndOfOptData = 0;
                                  *Result = TB_FALSE;
                                  return EOPInvalidDataInOPPattern;
              case BytePatternLogicalBlockEnd:
                                               *EndOfOptData = i;
                                               (*PatternChkStartEndOffset) += j + 1;
                                               return EOPSuccessfullOperation;
              case BytePatternLogicalBlockStart:
                                                 tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                 tmpErr = OPCmpLogicalData(cutStream + i,Pattern,StreamStartToEndLength - i,&tmpPatOff,&tmp,&tmpRes);
                                                 if (tmpErr)
                                                  {
                                                    return tmpErr;
                                                  }
                                                 if (tmpRes)
                                                   i += tmp;
                                                 else
                                                   *Result = TB_FALSE;
                                                 j = tmpPatOff - (*PatternChkStartEndOffset);
                                                 break;
              case BytePatternLogicalXOROperator:
                                                  if (*Result == TB_TRUE)
                                                   {
                                                     uint64_t k = 1;
                                                     *EndOfOptData = i;
                                                     do {
                                                      j ++;
                                                      switch (Pattern.PtrnB[(*PatternChkStartEndOffset) + j])
                                                       {
                                                          case BytePatternEnd:
                                                                              *EndOfOptData = 0;
                                                                              *Result = TB_FALSE;
                                                                              return EOPInvalidDataInOPPattern;

                                                          case BytePatternLogicalBlockStart:
                                                                                            k ++;
                                                                                            break;

                                                          case BytePatternLogicalBlockEnd:
                                                                                          k --;
                                                                                          break;
                                                       }
                                                     }
                                                     while (Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != BytePatternLogicalBlockEnd || (k));
                                                     (*PatternChkStartEndOffset) += j + 1;
                                                     return EOPSuccessfullOperation;
                                                   }
                                                  else
                                                   {
                                                     *Result = TB_TRUE;
                                                     i = 0;
                                                     j ++;
                                                     break;
                                                   }
              case BytePatternOptionalBlockStart:
                                                 tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                 tmpErr = OPCmpOptionalData(cutStream + i,Pattern,StreamStartToEndLength - i,&tmpPatOff,&tmp,&tmpRes);
                                                 if (tmpErr)
                                                   return tmpErr;
                                                 if (tmpRes)
                                                    i += tmp;
                                                 j = tmpPatOff - (*PatternChkStartEndOffset);
                                                 break;

              default:
                      if (Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != cutStream[i])
                       {
                           uint64_t k = 1,l = 8;
                           *Result = TB_FALSE;
                           i = 0;
                           // v1.20 - Fast forwarder to speed-up logical pattern lookup
                           do
                            {
                              j ++;
                              switch (Pattern.PtrnB[(*PatternChkStartEndOffset) + j])
                               {
                                 case BytePatternLogicalBlockStart:
                                                                   k ++;
                                                                   break;
                                 case BytePatternLogicalBlockEnd:
                                                                 if (!k)
                                                                  {
                                                                    *EndOfOptData = 0;
                                                                    *Result = TB_FALSE;
                                                                    return EOPInvalidDataInOPPattern;
                                                                  }
                                                                 k --;
                                                                 break;
                                 case BytePatternLogicalXOROperator:
                                                                    if (k != 1)
                                                                     l = 8;
                                                                    else
                                                                     l = 0;
                               }
                            }
                           while ((Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != BytePatternEnd) &&
                                  ((Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != BytePatternLogicalBlockEnd) || (k)) &&
                                  ((Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != BytePatternLogicalXOROperator) || (l)));
                           if ((Pattern.PtrnB[(*PatternChkStartEndOffset) + j] == BytePatternLogicalBlockEnd))
                            {
                              *PatternChkStartEndOffset += j + 1;
                              return EOPSuccessfullOperation;
                            }
                       }
                      else
                       {
                         i ++;
                         j ++;
                       }

           }
        }
       if ((i == StreamStartToEndLength) && (Pattern.PtrnB[(*PatternChkStartEndOffset) + j] != BytePatternEnd))
        {
          *Result = TB_FALSE;
          // v1.20 - This will fix endless loop when logic-nested pattern is lookuped and we are reaching area close to EOF.
          *EndOfOptData = 0;
          (*PatternChkStartEndOffset) += j + 1;
        }
     }
    else
     {
        if (Pattern.PtrnHB[*PatternChkStartEndOffset] != HalfBytePatternLogicalBlockStart)
        {
            *Result = TB_FALSE;
            return EOPInvalidDataInOPPattern;
        }
       while (i < (StreamStartToEndLength << 1))
        {
          switch (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j])
           {
              case HalfByteWildcard:
                                    i ++;
                                    j ++;
                                    break;

              case HalfBytePatternEnd:
                                  *EndOfOptData = 0;
                                  *Result = TB_FALSE;
                                  return EOPInvalidDataInOPPattern;
              case HalfBytePatternLogicalBlockEnd:
                                               *EndOfOptData = i;
                                               *PatternChkStartEndOffset += j + 1;
                                               return EOPSuccessfullOperation;
              case HalfBytePatternLogicalBlockStart:
                                                 tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                 tmpErr = OPCmpLogicalData(cutStream + (i >> 1),Pattern,StreamStartToEndLength - i,&tmpPatOff,&tmp,&tmpRes);
                                                 if (tmpErr)
                                                   return tmpErr;
                                                 if (tmpRes)
                                                    i += tmp;
                                                 else
                                                   *Result = TB_FALSE;
                                                 j = tmpPatOff - (*PatternChkStartEndOffset);
                                                 break;
              case HalfBytePatternLogicalXOROperator:
                                                  if (*Result == TB_TRUE)
                                                   {
                                                      uint64_t k = 1;
                                                     *EndOfOptData = i;
                                                      do {
                                                       j ++;
                                                       switch (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j])
                                                        {
                                                           case HalfBytePatternEnd:
                                                                                   *EndOfOptData = 0;
                                                                                   *Result = TB_FALSE;
                                                                                   return EOPInvalidDataInOPPattern;

                                                           case HalfBytePatternLogicalBlockStart:
                                                                                                 k ++;
                                                                                                 break;

                                                           case HalfBytePatternLogicalBlockEnd:
                                                                                                 k --;
                                                                                                 break;
                                                        }

                                                      }
                                                      while ((Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HalfBytePatternLogicalBlockEnd) || (k));
                                                      (*PatternChkStartEndOffset) += j + 1;
                                                      return EOPSuccessfullOperation;
                                                   }
                                                  else
                                                   {
                                                     *Result = TB_TRUE;
                                                     i = 0;
                                                     j ++;
                                                     break;
                                                   }

              case HalfBytePatternOptionalBlockStart:
                                                      tmpPatOff = (*PatternChkStartEndOffset) + j;
                                                      tmpErr = OPCmpOptionalData(cutStream + (i >> 1),Pattern,StreamStartToEndLength - i,&tmpPatOff,&tmp,&tmpRes);
                                                      if (tmpErr)
                                                        return tmpErr;
                                                      if (tmpRes)
                                                        i += tmp;
                                                      j = tmpPatOff - (*PatternChkStartEndOffset);
                                                      break;


              default:
                      HByte = (i % 2) ? (cutStream[i >> 1] & 0x0F) : (cutStream[i >> 1] >> 4);
                      if (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HByte)
                       {

                           uint64_t k = 1,l = 8;
                           i = 0;
                           *Result = TB_FALSE;
                           // v1.20 - Fast forwarder to speed-up logical pattern lookup
                           do
                            {
                              j ++;
                              switch (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j])
                               {
                                 case HalfBytePatternLogicalBlockStart:
                                                                   k ++;
                                                                   break;
                                 case HalfBytePatternLogicalBlockEnd:
                                                                 if (!k)
                                                                  {
                                                                    *EndOfOptData = 0;
                                                                    *Result = TB_FALSE;
                                                                    return EOPInvalidDataInOPPattern;
                                                                  }
                                                                 k --;
                                                                 break;
                                 case HalfBytePatternLogicalXOROperator:
                                                                        if (k != 1)
                                                                         l = 8;
                                                                        else
                                                                         l = 0;
                                                                        break;
                               }
                            }
                           while ((Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HalfBytePatternEnd) &&
                                  ((Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HalfBytePatternLogicalBlockEnd) || (k)) &&
                                  ((Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HalfBytePatternLogicalXOROperator) || (l)));

                           if ((Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] == HalfBytePatternLogicalBlockEnd))
                            {
                              *EndOfOptData = 0;
                              *PatternChkStartEndOffset += j + 1;
                              return EOPSuccessfullOperation;
                            }


                       }
                      else
                       {

                         i ++;
                         j ++;
                       }

           }
        }
       if ((i == (StreamStartToEndLength << 1)) && (Pattern.PtrnHB[(*PatternChkStartEndOffset) + j] != HalfBytePatternEnd))
        {
          *Result = TB_FALSE;
          // v1.20 - This will fix endless loop when logic-nested pattern is lookuped and we are reaching area close to EOF.
          *EndOfOptData = 0;
          (*PatternChkStartEndOffset) += j + 1;
        }
     }
    return EOPSuccessfullOperation;
}

