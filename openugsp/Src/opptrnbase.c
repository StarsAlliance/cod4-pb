#include "opptrnbase.h"

TOPError LIBFUNC OPParseUserPtrn_10(const char *InputPtrn,
                                 const TPTRN_TYPE DesiredPattern,
                                 TPTRN_10 *const ParsedPtrn)
{
  char *FiltratedInputPtrn;
  TBOOL StringMode = TB_FALSE,StrOperatorEncountered = TB_FALSE;
  uint64_t i = 0,j = 0,ext_operators_count = 0;
  if (!InputPtrn || !ParsedPtrn)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  ParsedPtrn -> PtrnB = NULL;
  while (InputPtrn[i])
   {
      char ptrn_chr;
      ptrn_chr = InputPtrn[i];
      if ((ptrn_chr == '[' ) || (ptrn_chr == ']' ) || (ptrn_chr == '{' ) || (ptrn_chr == '}' ) || (ptrn_chr == '^' ) || (ptrn_chr == '`'))
       ext_operators_count ++;
      else
       if (StringMode)
        ext_operators_count ++;
      if (ptrn_chr == '`')
       {
         if (StringMode)
           StringMode = TB_FALSE;
         else
           StringMode = TB_TRUE;
       }
      i ++;
   }
  FiltratedInputPtrn = (char *) malloc(sizeof(char)*(strlen(InputPtrn) + 1));
  if (!FiltratedInputPtrn)
   return EOPCannotAllocateMemoryResources;
  i = 0;
  StringMode = TB_FALSE;
  while (InputPtrn[i])
   {
      char ptrn_chr;
      ptrn_chr = InputPtrn[i];
      if (ptrn_chr == '`')
       {
          if (StringMode)
           StringMode = TB_FALSE;
          else
           StringMode = TB_TRUE;
          FiltratedInputPtrn[j] = ptrn_chr;
          i ++;
          j ++;
          continue;
       }
      if (StringMode)
       {
          FiltratedInputPtrn[j] = ptrn_chr;
          i ++;
          j ++;
          continue;
       }

      if (((ptrn_chr >= '0') && (ptrn_chr <= '9')) || ((ptrn_chr >= 'A') &&
         (ptrn_chr <= 'F')) || (ptrn_chr == '?') || (ptrn_chr == '*') || (ptrn_chr == '[') ||
         (ptrn_chr == ']') || (ptrn_chr == '{') || (ptrn_chr == '}') || (ptrn_chr == '^'))
       {
          if (ptrn_chr != '*')
            FiltratedInputPtrn[j] = ptrn_chr;
          else
            FiltratedInputPtrn[j] = '?';
          i ++;
          j ++;
          continue;
       }
      if ((ptrn_chr >= 'a') && (ptrn_chr <= 'f'))
       {
          FiltratedInputPtrn[j] = ptrn_chr - 'a' + 'A';
          i ++;
          j ++;
          continue;
       }
      if ((ptrn_chr == ' ') || (ptrn_chr == '.') || (ptrn_chr == ',') ||
         (ptrn_chr == ':') || (ptrn_chr == ';') || (ptrn_chr == '-') || (ptrn_chr == '_'))
       {
         i ++;
         continue;
       }
      free(FiltratedInputPtrn);
      return EOPInvalidCharactersInUserPassedPattern;
   }
  FiltratedInputPtrn[j] = '\0';
  StringMode = TB_FALSE;
  if ((strlen(FiltratedInputPtrn) - ext_operators_count) % 2)
   {
     free(FiltratedInputPtrn);
     return EOPPatternMustConsistOfEvenNumberOfElements;
   }
  if (DesiredPattern == TU_UseBytePatterns)
   {
     i = 0;
     j = 0;
     ParsedPtrn -> PtrnType = TU_UseBytePatterns;
     ParsedPtrn -> PtrnB = (TPTRNB) malloc(sizeof(uint16_t) * (((strlen(FiltratedInputPtrn) - ext_operators_count) >> 1) + ext_operators_count + 1));
     if (!(ParsedPtrn -> PtrnB))
      {
        free(FiltratedInputPtrn);
        return EOPCannotAllocateMemoryResources;
      }
     while (FiltratedInputPtrn[i] != '\0')
      {

         switch (FiltratedInputPtrn[i])
          {
             case '`':
                      if (StringMode)
                        StringMode = TB_FALSE;
                      else
                        StringMode = TB_TRUE;
                      StrOperatorEncountered = TB_TRUE;
                      break;
             case '?':
                      if (!StringMode)
                       {
                         if (FiltratedInputPtrn[i + 1] != '?')
                          {
                            free(FiltratedInputPtrn);
                            free(ParsedPtrn -> PtrnB);
                            return EOPUseOfOddSequenceOfWildcardsInBytePatternsIsProhibited;
                          }
                         else
                          ParsedPtrn -> PtrnB[j >> 1] = ByteWildcard;
                         i ++;
                         break;
                       }
             case '[':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnB[j >> 1] = BytePatternOptionalBlockStart;
                         break;
                       }
             case ']':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnB[j >> 1] = BytePatternOptionalBlockEnd;
                         break;
                       }
             case '{':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnB[j >> 1] = BytePatternLogicalBlockStart;
                         break;
                       }
             case '}':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnB[j >> 1] = BytePatternLogicalBlockEnd;
                         break;
                       }
             case '^':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnB[j >> 1] = BytePatternLogicalXOROperator;
                         break;
                       }
             default:
                     if (StringMode)
                      {
                        ParsedPtrn -> PtrnB[j >> 1] = FiltratedInputPtrn[i];
                      }
                     else
                      {
                         ParsedPtrn -> PtrnB[j >> 1] = (((FiltratedInputPtrn[i] >= 'A') ? ((FiltratedInputPtrn[i] - 'A' + 10)) : (FiltratedInputPtrn[i] - '0') ) << 4) +
                                                       ((FiltratedInputPtrn[i + 1] >= 'A') ? (FiltratedInputPtrn[i + 1] - 'A' + 10) : (FiltratedInputPtrn[i + 1] - '0') );
                         i ++;
                      }
          }
         i ++;
         if (StrOperatorEncountered)
          StrOperatorEncountered = TB_FALSE;
         else
          j += 2;

      }
     ParsedPtrn -> PtrnB[j >> 1] =  BytePatternEnd;
   }
  else
   {
     i = 0;
     j = 0;
     ParsedPtrn -> PtrnType = TU_UseHalfBytePatterns;
     ParsedPtrn -> PtrnHB = (TPTRNHB) malloc(sizeof(uint8_t) * (strlen(FiltratedInputPtrn) + ext_operators_count + 1));
     if (!ParsedPtrn -> PtrnHB)
      {
        free(FiltratedInputPtrn);
        return EOPCannotAllocateMemoryResources;
      }
     while (FiltratedInputPtrn[i] != '\0')
      {
         switch (FiltratedInputPtrn[i])
          {
             case '`':
                      if (StringMode)
                        StringMode = TB_FALSE;
                      else
                        StringMode = TB_TRUE;
                      StrOperatorEncountered = TB_TRUE;
                      break;
             case '?':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnHB[j] = HalfByteWildcard;
                         break;
                       }
             case '[':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnHB[j] = HalfBytePatternOptionalBlockStart;
                         break;
                       }
             case ']':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnHB[j] = HalfBytePatternOptionalBlockEnd;
                         break;
                       }
             case '{':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnHB[j] = HalfBytePatternLogicalBlockStart;
                         break;
                       }
             case '}':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnHB[j] = HalfBytePatternLogicalBlockEnd;
                         break;
                       }
             case '^':
                      if (!StringMode)
                       {
                         ParsedPtrn -> PtrnHB[j] = HalfBytePatternLogicalXOROperator;
                         break;
                       }

             default:
                     if (StringMode)
                      {
                        ParsedPtrn -> PtrnHB[j] = (FiltratedInputPtrn[i] >> 4);
                        ParsedPtrn -> PtrnHB[j + 1] = (FiltratedInputPtrn[i] && 0x0F);
                        j ++;
                      }
                     else
                       ParsedPtrn -> PtrnHB[j] = ((FiltratedInputPtrn[i] >= 'A') ? (FiltratedInputPtrn[i] - 'A' + 10) : (FiltratedInputPtrn[i] - '0') );
          }
         i ++;
         if (StrOperatorEncountered)
          StrOperatorEncountered = TB_FALSE;
         else
          j ++;
      }
     ParsedPtrn -> PtrnHB[j] = HalfBytePatternEnd;
   }
  free(FiltratedInputPtrn);
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPValidatePattern_10(const TPTRN_10 PatternToValidate,
                                   uint64_t *const PatternLength)
{
  uint64_t i;
  switch (PatternToValidate.PtrnType)
   {
      case TU_UseBytePatterns:
           i = 0;
           while (PatternToValidate.PtrnB[i] != BytePatternEnd)
            {
               if ((PatternToValidate.PtrnB[i] > 0xFF) && (PatternToValidate.PtrnB[i] != ByteWildcard))
                {
                   if (PatternLength)
                     *PatternLength = 0LL;
                   return EOPInvalidDataInOPPattern;
                }
               i ++;
            }
           if (PatternLength)
             *PatternLength = i;
           break;
      case TU_UseHalfBytePatterns:
           i = 0;
           while (PatternToValidate.PtrnHB[i] != HalfBytePatternEnd)
            {
               if ((PatternToValidate.PtrnHB[i] > 0xF) && (PatternToValidate.PtrnHB[i] != HalfByteWildcard))
                {
                   if (PatternLength)
                     *PatternLength = 0LL;
                   return EOPInvalidDataInOPPattern;
                }
               i ++;
            }
           if (i % 2)
            {
              if (PatternLength)
               *PatternLength = 0LL;
              return EOPLengthOfOPPatternMustBeEven;
            }
           if (PatternLength)
             *PatternLength = i;
           break;
      default:
          if (PatternLength)
            *PatternLength = 0LL;
          return EOPInvalidTypeValueInOPPattern;
   }
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPFreePatternMemory_10(TPTRN_10 *const Pattern)
{
  if (!Pattern)
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  if (Pattern -> PtrnType == TU_UseBytePatterns)
   {
      if (Pattern -> PtrnB)
       {
         free(Pattern -> PtrnB);
         Pattern -> PtrnB = NULL;
       }
   }
  else
   if (Pattern -> PtrnType == TU_UseHalfBytePatterns)
   {
      if (Pattern -> PtrnHB)
       {
         free(Pattern -> PtrnHB);
         Pattern -> PtrnHB = NULL;
       }
   }
  return EOPSuccessfullOperation;
}


TOPError LIBFUNC OPInternalParseSimpleUserPtrn2(const char *InputPtrn,TPTRN_20 *const ParsedPtrn,char **UserPtrnNextToScan)
{
   uint64_t utmp_Size = 0,i = 0;
   char *p_tmpPtr;
   return 0;
   //while (InputPtrn[i] &&
}

TOPError LIBFUNC OPParseUserPtrn_20(const char *InputPtrn,
                                    const TPTRN_TYPE DesiredPattern,
                                    TPTRN_20 *const ParsedPtrn) /* DesiredPattern is provided only for backward compatability and not used in new implementation */
{
  char *FiltratedInputPtrn;
  TBOOL StringMode = TB_FALSE,StrOperatorEncountered = TB_FALSE;
  uint64_t i = 0,j = 0,ext_operators_count = 0;
  if (!InputPtrn || !ParsedPtrn)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
  (ParsedPtrn -> op_SimplePatternInfo).op_PatData = NULL;
  (ParsedPtrn -> op_SimplePatternInfo).op_PatWildCrdMask = NULL;
  ParsedPtrn -> op_NextSubPattern = NULL;
  while (InputPtrn[i])
   {
      char ptrn_chr;
      ptrn_chr = InputPtrn[i];
      if ((ptrn_chr == '[' ) || (ptrn_chr == ']' ) || (ptrn_chr == '{' ) || (ptrn_chr == '}' ) || (ptrn_chr == '^' ) || (ptrn_chr == '`'))
       ext_operators_count ++;
      else
       if (StringMode)
        ext_operators_count ++;
      if (ptrn_chr == '`')
       {
         if (StringMode)
           StringMode = TB_FALSE;
         else
           StringMode = TB_TRUE;
       }
      i ++;
   }
  FiltratedInputPtrn = (char *) malloc(sizeof(char)*(strlen(InputPtrn) + 1));
  if (!FiltratedInputPtrn)
   return EOPCannotAllocateMemoryResources;
  i = 0;
  StringMode = TB_FALSE;
  while (InputPtrn[i])
   {
      char ptrn_chr;
      ptrn_chr = InputPtrn[i];
      if (ptrn_chr == '`')
       {
          if (StringMode)
           StringMode = TB_FALSE;
          else
           StringMode = TB_TRUE;
          FiltratedInputPtrn[j] = ptrn_chr;
          i ++;
          j ++;
          continue;
       }
      if (StringMode)
       {
          FiltratedInputPtrn[j] = ptrn_chr;
          i ++;
          j ++;
          continue;
       }

      if (((ptrn_chr >= '0') && (ptrn_chr <= '9')) || ((ptrn_chr >= 'A') &&
         (ptrn_chr <= 'F')) || (ptrn_chr == '?') || (ptrn_chr == '*') || (ptrn_chr == '[') ||
         (ptrn_chr == ']') || (ptrn_chr == '{') || (ptrn_chr == '}') || (ptrn_chr == '^'))
       {
          if (ptrn_chr != '*')
            FiltratedInputPtrn[j] = ptrn_chr;
          else
            FiltratedInputPtrn[j] = '?';
          i ++;
          j ++;
          continue;
       }
      if ((ptrn_chr >= 'a') && (ptrn_chr <= 'f'))
       {
          FiltratedInputPtrn[j] = ptrn_chr - 'a' + 'A';
          i ++;
          j ++;
          continue;
       }
      if ((ptrn_chr == ' ') || (ptrn_chr == '.') || (ptrn_chr == ',') ||
         (ptrn_chr == ':') || (ptrn_chr == ';') || (ptrn_chr == '-') || (ptrn_chr == '_'))
       {
         i ++;
         continue;
       }
      free(FiltratedInputPtrn);
      return EOPInvalidCharactersInUserPassedPattern;
   }
  FiltratedInputPtrn[j] = '\0';
  StringMode = TB_FALSE;
  if ((strlen(FiltratedInputPtrn) - ext_operators_count) % 2)
   {
     free(FiltratedInputPtrn);
     return EOPPatternMustConsistOfEvenNumberOfElements;
   }

  OPParseUserPtrnInternal_20(FiltratedInputPtrn,DesiredPattern,ParsedPtrn);
  free(FiltratedInputPtrn);
  return EOPSuccessfullOperation;

}

TOPError LIBFUNC OPParseUserPtrnInternal_20(const char *PreParsedInputPtrn,
                                            const TPTRN_TYPE DesiredPattern,
                                            TPTRN_20 *const ParsedPtrn) /* DesiredPattern is provided only for backward compatability and not used in new implementation */
{
  uint64_t i = 0,j = 0;
  TBOOL StringMode = TB_FALSE,StrOperatorEncountered = TB_FALSE;
  /*
  ParsedPtrn -> PtrnB = (TPTRNB) malloc(sizeof(uint16_t) * (((strlen(PreParsedInputPtrn) - ext_operators_count) >> 1) + ext_operators_count + 1));
  if (!(ParsedPtrn -> PtrnB))
    {
      free(FiltratedInputPtrn);
      return EOPCannotAllocateMemoryResources;
    }
  */
  while (PreParsedInputPtrn[i] != '\0')
    {

         switch (PreParsedInputPtrn[i])
          {
             case '`':
                      if (StringMode)
                        StringMode = TB_FALSE;
                      else
                        StringMode = TB_TRUE;
                      StrOperatorEncountered = TB_TRUE;
                      break;
             case '?':
                      if (!StringMode)
                       {
                         if (PreParsedInputPtrn[i + 1] != '?')
                          {
                            free((void *)PreParsedInputPtrn);
                            //free(ParsedPtrn -> PtrnB);
                            return EOPUseOfOddSequenceOfWildcardsInBytePatternsIsProhibited;
                          }
                         else
                          //ParsedPtrn -> PtrnB[j >> 1] = ByteWildcard;
                         i ++;
                         break;
                       }
             case '[':
                      if (!StringMode)
                       {
                         //ParsedPtrn -> PtrnB[j >> 1] = BytePatternOptionalBlockStart;
                         break;
                       }
             case ']':
                      if (!StringMode)
                       {
                         //ParsedPtrn -> PtrnB[j >> 1] = BytePatternOptionalBlockEnd;
                         break;
                       }
             case '{':
                      if (!StringMode)
                       {
                         //ParsedPtrn -> PtrnB[j >> 1] = BytePatternLogicalBlockStart;
                         break;
                       }
             case '}':
                      if (!StringMode)
                       {
                         //ParsedPtrn -> PtrnB[j >> 1] = BytePatternLogicalBlockEnd;
                         break;
                       }
             case '^':
                      if (!StringMode)
                       {
                         //ParsedPtrn -> PtrnB[j >> 1] = BytePatternLogicalXOROperator;
                         break;
                       }
             default:
                     if (StringMode)
                      {
                        //ParsedPtrn -> PtrnB[j >> 1] = FiltratedInputPtrn[i];
                      }
                     else
                      {
                         //ParsedPtrn -> PtrnB[j >> 1] = (((FiltratedInputPtrn[i] >= 'A') ? ((FiltratedInputPtrn[i] - 'A' + 10)) : (FiltratedInputPtrn[i] - '0') ) << 4) +
                         //                              ((FiltratedInputPtrn[i + 1] >= 'A') ? (FiltratedInputPtrn[i + 1] - 'A' + 10) : (FiltratedInputPtrn[i + 1] - '0') );
                         i ++;
                      }
          }
         i ++;
         if (StrOperatorEncountered)
          StrOperatorEncountered = TB_FALSE;
         else
          j += 2;

    }
  //ParsedPtrn -> PtrnB[j >> 1] =  BytePatternEnd;
  return EOPSuccessfullOperation;

}

TOPError LIBFUNC OPValidatePattern_20(const TPTRN_20 PatternToValidate,
                                      uint64_t *const PatternLength)
{
}

TOPError LIBFUNC OPFreePatternMemory_20(TPTRN_20 *const Pattern)
{
  return 0;
}
