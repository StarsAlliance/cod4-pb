#ifndef __OPLOG_H__
#define __OPLOG_H__

#ifdef cplusplus
  extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>


#include "opportability.h"
#include "opptrnbase.h"
#include "opstream.h"

#define TLDB_UseFound 1
#define TLDB_UseErrors 2
#define TLDB_UseLocations 4
#define TLDB_UseALL 7

typedef struct {
    TOPError ErrA[3];
}TERRLDB;

typedef struct {
    uint8_t WhatUsed;
    TBOOL *ifFoundLogDB;
    TERRLDB *ErrorsLogDB;
    TVAADDR *LocationsLogDB;
}TLOGDB;

TOPError LIBFUNC OPCreateLogDataBase(TLOGDB *const LogDB,
                                     const uint64_t AmmountOfFields);
TOPError LIBFUNC OPFreeLogDataBase(TLOGDB *const LogDB);



#ifdef cplusplus
  }
#endif


#endif
