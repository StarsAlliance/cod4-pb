#ifndef _OPBININFO_H
#define _OPBININFO_H

#include "opstream.h"
#include "opbindefs.h"

#ifdef cplusplus
  extern "C" {
#endif


typedef enum {
  TBINF_MZ_DOS = 0,
  TBINF_LE_OS2 = 1,
  TBINF_NE_WIN16OS2 = 2,
  TBINF_PE_WIN = 3,
  TBINF_PEPLUS_WIN = 4,
  TBINF_ELF_UNIX = 5,
  TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY = 6
} TBINFORMAT;

typedef enum {
  TBINT_EXECUTABLE = 0,
  TBINT_SHAREDLIB = 1,
  TBINT_RELOCELFOBJ = 2,
  TBINT_UNKNOWN = 3
} TBINTYPE;

typedef enum {
  TBINB_16B = 0,
  TBINB_32B = 1,
  TBINB_64B = 2
} TBINBITS;

typedef enum {
  TBINA_i86 = 0,
  TBINA_amd64 = 1,
  TBINA_i64 = 2,
  TBINA_OTHER = 3
} TBINARCH;

typedef struct {
    TBINFORMAT BinaryFormat;
    TBINTYPE BinaryType;
    TBINBITS BitsType;
    TBINARCH BinaryArchitecture;
} TBININFO;


TOPError LIBFUNC OPGetBinaryInfo(const TSTREAMADDR_HDR fStream,
                                 TBININFO *const fBinInfo);


TOPError LIBFUNC OPELFGetAdditionalInfo(const TSTREAMADDR_HDR fStream,
                                        uint64_t *BaseCode,
                                        uint64_t *SectionTable,
                                        uint64_t *ProgramHeaderTable,
                                        uint32_t *StringTable,
                                        uint32_t TextSect[2],
                                        uint32_t DataSect[2],
                                        uint32_t Data1Sect[2],
                                        uint32_t RODataSect[2],
                                        uint32_t ROData1Sect[2],
                                        uint32_t DynamicSect[2],
                                        uint32_t GOTSect[2]);

TOPError LIBFUNC OPELFGetDynamicRelocationTable(const TSTREAMADDR_HDR fStream,uint64_t RelDynSectionAddress[2],uint64_t *puRelDynSectionSize);
TOPError LIBFUNC OPELFFindRelocationInSpecificAddressRange(const TSTREAMADDR_HDR fStream, const uint64_t uStartLookupAddress, const uint64_t uEndLookupAddress,void **pResultingRelocationAddress, TBOOL *bIsRelocationExists);
TOPError LIBFUNC OPPEGetAdditionalInfo(const TSTREAMADDR_HDR fStream,
                                       uint64_t *Base,
                                       uint16_t *MajorOperatingSystemVersion,
                                       uint16_t *MinorOperatingSystemVersion,
                                       uint16_t *MajorImageVersion,
                                       uint16_t *MinorImageVersion,
                                       uint32_t *CheckSum,
                                       uint16_t *Subsystem,
                                       uint16_t *SectionTable,
                                       uint32_t *Text,
                                       uint32_t *Data,
                                       uint32_t *RData,
                                       uint32_t *Reloc,
                                       uint32_t *ExportTable,
                                       uint32_t *sizeOfExportSection,
                                       uint32_t *ImportTable);

TOPError LIBFUNC OPPEGetDLLCharacteristics(const TSTREAMADDR_HDR fStream,
                                           uint16_t *PEDllCharacteristics);
TOPError LIBFUNC OPPESetDLLCharacteristics(const TSTREAMADDR_HDR fStream,
                                           uint16_t PEDllCharacteristics);

TOPError LIBFUNC OPPEGetExportedFunctionInfo(const TSTREAMADDR_HDR fStream,
                                             char *FunctionName,
                                             uint16_t *FunctionOrdinal,
                                             uint32_t *FunctionLocation,
                                             char **NameFromOrdinal,
                                             TBOOL *IfItIsForwarder);

#define opadd(x,y) ( ((uint64_t) ((uint64_t)x + (uint64_t)y) > 0xFFFFFFFF) ? (uint32_t) ((uint64_t)x + (uint64_t)y - (uint64_t)0xFFFFFFFF)  : (uint32_t) (x + y) )
#define opadc(x,y) ( ((uint64_t) ((uint64_t)x + (uint64_t)y) > 0xFFFFFFFF) ? (uint32_t) ((uint64_t)x + (uint64_t)y - (uint64_t)0xFFFFFFFE)  : (uint32_t) (x + y) )
TOPError LIBFUNC OPCalculatePEChecksum(TSTREAMADDR_NOHDR fStream,uint32_t ChecksumCalculationStartingBase,uint64_t AmmountOfWordsInFile,uint16_t *CalculatedChecksum);


#ifdef cplusplus
  }
#endif

#endif
