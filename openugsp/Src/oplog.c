#include "oplog.h"

TOPError LIBFUNC OPCreateLogDataBase(TLOGDB *const LogDB,
                                     const uint64_t AmmountOfFields)
{
   uint64_t i;
   if (!LogDB)
    return EOPAttemptToReferenceMemoryAtNULLAddress;
   LogDB -> ifFoundLogDB = NULL;
   LogDB -> ErrorsLogDB = NULL;
   LogDB -> LocationsLogDB = NULL;

   if ((LogDB -> WhatUsed) & TLDB_UseFound)
    {
      LogDB -> ifFoundLogDB = (TBOOL *)malloc (sizeof(TBOOL) * AmmountOfFields);
      if (!(LogDB -> ifFoundLogDB))
       return EOPCannotAllocateMemoryResources;
      for (i = 0;i < AmmountOfFields;i ++)
       (LogDB -> ifFoundLogDB)[i] = TB_FALSE;
    }
   if ((LogDB -> WhatUsed) & TLDB_UseErrors)
    {
      LogDB -> ErrorsLogDB = (TERRLDB *)malloc ((sizeof(TERRLDB) * AmmountOfFields) << 1);
      if (!(LogDB -> ErrorsLogDB))
       return EOPCannotAllocateMemoryResources;
      for (i = 0;i < AmmountOfFields;i ++)
       {
        ((LogDB -> ErrorsLogDB)[i]).ErrA[0] = EOPSuccessfullOperation;
        ((LogDB -> ErrorsLogDB)[i]).ErrA[1] = EOPSuccessfullOperation;
        ((LogDB -> ErrorsLogDB)[i]).ErrA[2] = EOPSuccessfullOperation;
       }
    }
   if ((LogDB -> WhatUsed) & TLDB_UseLocations)
    {
      LogDB -> LocationsLogDB = (TVAADDR *) malloc (sizeof(TVAADDR) * AmmountOfFields);
      if (!(LogDB -> LocationsLogDB))
       return EOPCannotAllocateMemoryResources;
      for (i = 0;i < AmmountOfFields;i ++)
       (LogDB -> LocationsLogDB)[i] = (TVAADDR) 0;
    }
   return EOPSuccessfullOperation;
}
TOPError LIBFUNC OPFreeLogDataBase(TLOGDB *const LogDB)
{
  if (!LogDB)
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  if (LogDB -> ifFoundLogDB)
   {
     free(LogDB -> ifFoundLogDB);
     LogDB -> ifFoundLogDB = NULL;
   }
  if (LogDB -> ErrorsLogDB)
   {
     free(LogDB -> ErrorsLogDB);
     LogDB -> ErrorsLogDB = NULL;
   }
  if (LogDB -> LocationsLogDB)
   {
     free(LogDB -> LocationsLogDB);
     LogDB -> LocationsLogDB = NULL;
   }
  return EOPSuccessfullOperation;
}
