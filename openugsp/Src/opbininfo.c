#include "opbininfo.h"

#define OP_PE_MAGIC
#define OP_ELF_MAGIC

TOPError LIBFUNC OPGetBinaryInfo(const TSTREAMADDR_HDR fStream,
                                 TBININFO *const fBinInfo)
{
  TOPError intError;
  TSTREAMADDR_NOHDR Addr;
  TVAADDR intOff;
  OP_STRIPPED_ELF_HDR *intElfHeader;
  uint64_t intRemSize;
  uint16_t intData,intData2;
  if (!fStream || !fBinInfo)
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  OPJumpToOffsetInStream(fStream,0x0,&Addr,&intRemSize);
  if (intRemSize >= sizeof(OP_IMAGE_DOS_HEADER))
   {
      uint16_t intMagic;
      uint32_t intPEMagic;
      OP_STRIPPED_IMAGE_NT_HEADERS *intPEHdr;
      OP_IMAGE_DOS_HEADER *intDosHdr;
      intDosHdr = (OP_IMAGE_DOS_HEADER *)Addr;
      if (intDosHdr -> e_magic != OPIMAGE_DOS_SIGNATURE)
       goto NOTWINBINARY;
      intOff = intDosHdr -> e_lfanew;
      if (intOff >= intRemSize)
       goto NOTWINBINARY;
      intError = OPJumpToOffsetInStream(fStream,intOff,&Addr,&intRemSize);
      if (intError)
       {
          fBinInfo -> BitsType = TBINB_16B;
          fBinInfo -> BinaryArchitecture = TBINA_OTHER;
          fBinInfo -> BinaryType = TBINT_UNKNOWN;
          fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
          if (intError != EOPAttemptToGetOutsideOfStreamBounds)
           return intError;
          else
           return EOPSuccessfullOperation;
       }
      if (Addr + 1 >= fStream + intRemSize)
       {
          fBinInfo -> BitsType = TBINB_16B;
          fBinInfo -> BinaryArchitecture = TBINA_i86;
          fBinInfo -> BinaryType = TBINA_OTHER;
          fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
          return EOPSuccessfullOperation;
       }

      intMagic = *((uint16_t *) Addr);
      if ((intMagic == OPIMAGE_OS2_SIGNATURE_LE) || (intMagic == OPIMAGE_OS2_SIGNATURE))
       {
         fBinInfo -> BitsType = TBINB_16B;
         fBinInfo -> BinaryArchitecture = TBINA_i86;
         fBinInfo -> BinaryType = TBINT_UNKNOWN;
         fBinInfo -> BinaryFormat = ((intMagic == OPIMAGE_OS2_SIGNATURE_LE) ? TBINF_LE_OS2 : TBINF_NE_WIN16OS2);
         return EOPSuccessfullOperation;
       }
      if (Addr + 3 >= fStream + intRemSize)
       {
          fBinInfo -> BitsType = TBINB_16B;
          fBinInfo -> BinaryArchitecture = TBINA_OTHER;
          fBinInfo -> BinaryType = TBINT_UNKNOWN;
          fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
          return EOPSuccessfullOperation;
       }
      intPEMagic = *((uint32_t *) Addr);
      if (intPEMagic != OPIMAGE_NT_SIGNATURE)
       {
         fBinInfo -> BitsType = TBINB_16B;
         fBinInfo -> BinaryArchitecture = TBINA_i86;
         fBinInfo -> BinaryType = TBINT_UNKNOWN;
         fBinInfo -> BinaryFormat = TBINF_MZ_DOS;
         return EOPSuccessfullOperation;
       }
      if (Addr + sizeof(OP_STRIPPED_IMAGE_NT_HEADERS) - 1 >= fStream + intRemSize)
       {
          fBinInfo -> BitsType = TBINB_16B;
          fBinInfo -> BinaryArchitecture = TBINA_OTHER;
          fBinInfo -> BinaryType = TBINT_UNKNOWN;
          fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
          return EOPSuccessfullOperation;
       }
      intPEHdr = (OP_STRIPPED_IMAGE_NT_HEADERS *)Addr;
      intData = (intPEHdr -> FileHeader).Machine;
      intData2 = (intPEHdr -> CutOptHdr).Magic;
      switch (intData) {
        case OPIMAGE_FILE_MACHINE_I386:  fBinInfo -> BitsType = TBINB_32B;
                                         fBinInfo -> BinaryArchitecture = TBINA_i86;
                                         if (intData2 == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
                                           fBinInfo -> BinaryFormat = TBINF_PE_WIN;
                                         else
                                           if (intData2 == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                                             fBinInfo -> BinaryFormat = TBINF_PEPLUS_WIN;
                                         break;

        case OPIMAGE_FILE_MACHINE_AMD64: fBinInfo -> BitsType = TBINB_64B;
                                         fBinInfo -> BinaryArchitecture = TBINA_amd64;
                                         if (intData2 == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
                                           fBinInfo -> BinaryFormat = TBINF_PE_WIN;
                                         else
                                           if (intData2 == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                                             fBinInfo -> BinaryFormat = TBINF_PEPLUS_WIN;
                                         break;

        case OPIMAGE_FILE_MACHINE_IA64:  fBinInfo -> BitsType = TBINB_64B;
                                         fBinInfo -> BinaryArchitecture = TBINA_i64;
                                         if (intData2 == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
                                           fBinInfo -> BinaryFormat = TBINF_PE_WIN;
                                         else
                                           if (intData2 == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                                             fBinInfo -> BinaryFormat = TBINF_PEPLUS_WIN;
                                         break;

        default:                         fBinInfo -> BitsType = TBINB_16B;
                                         fBinInfo -> BinaryArchitecture = TBINA_OTHER;
                                         fBinInfo -> BinaryType = TBINT_UNKNOWN;
                                         fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
                                         return EOPSuccessfullOperation;
      }
     intData = (intPEHdr -> FileHeader).Characteristics;
     if (!(intData & OPIMAGE_FILE_EXECUTABLE_IMAGE))
      {
         fBinInfo -> BinaryType = TBINT_UNKNOWN;
         fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
         return EOPSuccessfullOperation;
      }
     if (intData & OPIMAGE_FILE_DLL)
       fBinInfo -> BinaryType = TBINT_SHAREDLIB;
     else
       fBinInfo -> BinaryType = TBINT_EXECUTABLE;
     return EOPSuccessfullOperation;
   }
  else
   {
       fBinInfo -> BitsType = TBINB_16B;
       fBinInfo -> BinaryArchitecture = TBINA_OTHER;
       fBinInfo -> BinaryType = TBINT_UNKNOWN;
       fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
       return EOPSuccessfullOperation;
   }

  NOTWINBINARY:
   if (intRemSize >= sizeof(OP_STRIPPED_ELF_HDR))
    {
      uint8_t *intElfIdent;
      uint16_t intData;
      intElfHeader = (OP_STRIPPED_ELF_HDR *) Addr;
      intElfIdent = intElfHeader -> e_ident;
      if ((intElfIdent[OPEI_MAG0] == OPELFMAG0) && (intElfIdent[OPEI_MAG1] == OPELFMAG1) &&
          (intElfIdent[OPEI_MAG2] == OPELFMAG2) && (intElfIdent[OPEI_MAG3] == OPELFMAG3))
       {
         switch (intElfIdent[OPEI_CLASS]) {
             case OPELFCLASS32:
                                fBinInfo -> BitsType = TBINB_32B;
                                break;
             case OPELFCLASS64:
                                fBinInfo -> BitsType = TBINB_64B;
                                break;
             default:
                                fBinInfo -> BitsType = TBINB_16B;
                                fBinInfo -> BinaryArchitecture = TBINA_OTHER;
                                fBinInfo -> BinaryType = TBINT_UNKNOWN;
                                fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
                                return EOPSuccessfullOperation;
         }
         fBinInfo -> BinaryFormat = TBINF_ELF_UNIX;
         intData = intElfHeader -> e_type;
         switch (intData) {
             case ET_EXEC: fBinInfo -> BinaryType = TBINT_EXECUTABLE;
                           break;
             case ET_DYN:  fBinInfo -> BinaryType = TBINT_SHAREDLIB;
                           break;
             case ET_REL:  fBinInfo -> BinaryType = TBINT_RELOCELFOBJ;
                           break;
             default:      fBinInfo -> BinaryType = TBINT_UNKNOWN;
         }
         intData = intElfHeader -> e_machine;
         switch (intData) {
             case EM_386:     fBinInfo -> BinaryArchitecture = TBINA_i86;
                              break;
             case EM_X86_64:  fBinInfo -> BinaryArchitecture = TBINA_amd64;
                              break;
             case EM_IA_64:   fBinInfo -> BinaryArchitecture = TBINA_i64;
                              break;
             default:         fBinInfo -> BinaryArchitecture = TBINA_OTHER;
         }
         return EOPSuccessfullOperation;
       }
      else
       {
         fBinInfo -> BitsType = TBINB_16B;
         fBinInfo -> BinaryArchitecture = TBINA_i86;
         fBinInfo -> BinaryType = TBINT_UNKNOWN;
         fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
         return EOPSuccessfullOperation;
       }
    }
   else
    {
       fBinInfo -> BitsType = TBINB_16B;
       fBinInfo -> BinaryArchitecture = TBINA_i86;
       fBinInfo -> BinaryType = TBINT_UNKNOWN;
       fBinInfo -> BinaryFormat = TBIN_UNKNOWN_FORMAT_OR_DAMAGED_BINARY;
       return EOPSuccessfullOperation;
    }
}

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
                                        uint32_t GOTSect[2])
{
    uint64_t intRemSize,etmpSect,etmpPhdr;
    uint32_t i,etmpStr;
    TSTREAMADDR_NOHDR Addr, SectionTableStartPtr, StringTableStartPtr;
    uint16_t SectionTableFieldsCount,SectionTableFieldSize, SectionTableStringTableLocationIndex,ProgramTableFieldsCount,ProgramTableFieldSize;
    void *tmpCmpStrictResolution;
    TOPError tmpErr;
    if (!fStream)
      return EOPAttemptToReferenceMemoryAtNULLAddress;
    if (SectionTable)
     *SectionTable = 0;
    if (ProgramHeaderTable)
     *ProgramHeaderTable = 0;
    if (BaseCode)
     *BaseCode = 0;
    if (StringTable)
     *StringTable = 0;
    if (TextSect)
     {
       TextSect[0] = 0;
       TextSect[1] = 0;
     }
    if (DataSect)
     {
       DataSect[0] = 0;
       DataSect[1] = 0;
     }
    if (Data1Sect)
     {
       Data1Sect[0] = 0;
       Data1Sect[1] = 0;
     }
    if (RODataSect)
     {
       RODataSect[0] = 0;
       RODataSect[1] = 0;
     }
    if (ROData1Sect)
     {
       ROData1Sect[0] = 0;
       ROData1Sect[1] = 0;
     }
    if (DynamicSect)
     {
       DynamicSect[0] = 0;
       DynamicSect[1] = 0;
     }
    if (GOTSect)
     {
       GOTSect[0] = 0;
       GOTSect[1] = 0;
     }
    OPJumpToOffsetInStream(fStream,0x0,&Addr,&intRemSize);
    if (intRemSize < sizeof(OP_STRIPPED_ELF_HDR))
      return EOPAttemptToGetOutsideOfStreamBounds;
    if ((((OP_STRIPPED_ELF_HDR *)Addr) -> e_ident)[OPEI_CLASS] == OPELFCLASS32)
     {
        TBOOL GotPLTDetected = TB_FALSE;
        OP_FULL_ELF_SHDR32 *tmpSect;
        OP_FULL_ELF_PHDR32 *tmpPhdr;
        if (intRemSize < sizeof(OP_FULL_ELF_HDR32))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpSect = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shoff;
        if (SectionTable)
          *SectionTable = etmpSect;
        etmpPhdr = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phoff;
        if (ProgramHeaderTable)
          *ProgramHeaderTable = etmpPhdr;
        SectionTableFieldsCount = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shnum;
        SectionTableFieldSize = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shentsize;
        SectionTableStringTableLocationIndex = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shstrndx;
        ProgramTableFieldsCount = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phnum;
        ProgramTableFieldSize = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phentsize;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpSect,&SectionTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < sizeof(OP_FULL_ELF_SHDR32))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpStr = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + SectionTableStringTableLocationIndex * SectionTableFieldSize)) -> sh_offset;
        if (StringTable)
         *StringTable = etmpStr;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpStr,&StringTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < SectionTableFieldSize * SectionTableFieldsCount)
          return EOPAttemptToGetOutsideOfStreamBounds;
        for (i = 1; i < SectionTableFieldsCount; i ++) // We gonna skip index-0 section
         {
             tmpSect = (OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize);
             if (DataSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".data"))
                {
                  DataSect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  DataSect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (TextSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".text"))
                {
                  TextSect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  TextSect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (Data1Sect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".data1"))
                {
                  Data1Sect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  Data1Sect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (RODataSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".rodata"))
                {
                  RODataSect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  RODataSect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (ROData1Sect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".rodata1"))
                {
                  ROData1Sect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  ROData1Sect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (DynamicSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".dynamic"))
                {
                  DynamicSect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  DynamicSect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (GOTSect)
              {
                if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".got.plt"))
                 {
                   GOTSect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                   GOTSect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                   GotPLTDetected = TB_TRUE;
                   continue;
                 }
                if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".got") && !GotPLTDetected)
                 {
                   GOTSect[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                   GOTSect[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                   continue;
                 }
              }

         }
        if (BaseCode)
         {
           tmpCmpStrictResolution = (void *)&tmpPhdr;
           tmpErr = OPJumpToOffsetInStream(fStream,etmpPhdr,(TSTREAMADDR_NOHDR *)tmpCmpStrictResolution,&intRemSize);
           if (tmpErr)
             return tmpErr;
           if (intRemSize < ProgramTableFieldSize * ProgramTableFieldsCount)
             return EOPAttemptToGetOutsideOfStreamBounds;
           for (i = 0; i < ProgramTableFieldsCount; i ++)
            {
               if (tmpPhdr[i].p_type == PT_LOAD)
                {
                  *BaseCode = (tmpPhdr[i].p_vaddr) - (tmpPhdr[i].p_offset);
                  break;
                }
            }
         }
     }
    else
     {
        OP_FULL_ELF_SHDR64 *tmpSect;
        OP_FULL_ELF_PHDR64 *tmpPhdr;
        if (intRemSize < sizeof(OP_FULL_ELF_HDR64))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpSect = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shoff;
        if (SectionTable)
          *SectionTable = etmpSect;
        etmpPhdr = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phoff;
        if (ProgramHeaderTable)
          *ProgramHeaderTable = etmpPhdr;
        SectionTableFieldsCount = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shnum;
        SectionTableFieldSize = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shentsize;
        SectionTableStringTableLocationIndex = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shstrndx;
        ProgramTableFieldsCount = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phnum;
        ProgramTableFieldSize = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phentsize;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpSect,&SectionTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < sizeof(OP_FULL_ELF_SHDR64))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpStr = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + SectionTableStringTableLocationIndex * SectionTableFieldSize)) -> sh_offset;
        if (StringTable)
         *StringTable = etmpStr;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpStr,&StringTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < SectionTableFieldSize * SectionTableFieldsCount)
          return EOPAttemptToGetOutsideOfStreamBounds;
        for (i = 1; i < SectionTableFieldsCount; i ++) // We gonna skip index-0 section
         {
             tmpSect = (OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize);
             if (DataSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".data"))
                {
                  DataSect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  DataSect[1] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (TextSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".text"))
                {
                  TextSect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  TextSect[1] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (Data1Sect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".data1"))
                {
                  Data1Sect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  Data1Sect[1] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (RODataSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".rodata"))
                {
                  RODataSect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  RODataSect[1] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (ROData1Sect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".rodata1"))
                {
                  ROData1Sect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  ROData1Sect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (DynamicSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".dynamic"))
                {
                  DynamicSect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  DynamicSect[1] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
             if (GOTSect)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".got"))
                {
                  GOTSect[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  GOTSect[1] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  continue;
                }
         }
        if (BaseCode)
         {
           tmpCmpStrictResolution = (void *)&tmpPhdr;
           tmpErr = OPJumpToOffsetInStream(fStream,etmpPhdr,(TSTREAMADDR_NOHDR *)tmpCmpStrictResolution,&intRemSize);
           if (tmpErr)
             return tmpErr;
           if (intRemSize < ProgramTableFieldSize * ProgramTableFieldsCount)
             return EOPAttemptToGetOutsideOfStreamBounds;
           for (i = 0; i < ProgramTableFieldsCount; i ++)
            {
               if (tmpPhdr[i].p_type == PT_LOAD)
                {
                   *BaseCode = (tmpPhdr[i].p_vaddr) - (tmpPhdr[i].p_offset);
                   break;
                }
            }
         }
    }
  return EOPSuccessfullOperation;
}


TOPError LIBFUNC OPELFGetDynamicRelocationTable(const TSTREAMADDR_HDR fStream,uint64_t RelDynSectionAddress[2],uint64_t *puRelDynSectionSize)
{
   uint64_t intRemSize,etmpSect,etmpPhdr;
    uint32_t i,etmpStr;
    TSTREAMADDR_NOHDR Addr, SectionTableStartPtr, StringTableStartPtr;
    uint16_t SectionTableFieldsCount,SectionTableFieldSize, SectionTableStringTableLocationIndex,ProgramTableFieldsCount,ProgramTableFieldSize;
    TOPError tmpErr;
    if (!fStream)
      return EOPAttemptToReferenceMemoryAtNULLAddress;
    if (RelDynSectionAddress)
     *RelDynSectionAddress = 0;
    else
     return EOPAttemptToReferenceMemoryAtNULLAddress;
    OPJumpToOffsetInStream(fStream,0x0,&Addr,&intRemSize);
    if (intRemSize < sizeof(OP_STRIPPED_ELF_HDR))
      return EOPAttemptToGetOutsideOfStreamBounds;
    if ((((OP_STRIPPED_ELF_HDR *)Addr) -> e_ident)[OPEI_CLASS] == OPELFCLASS32)
     {
        OP_FULL_ELF_SHDR32 *tmpSect;
        if (intRemSize < sizeof(OP_FULL_ELF_HDR32))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpSect = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shoff;
        etmpPhdr = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phoff;
        SectionTableFieldsCount = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shnum;
        SectionTableFieldSize = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shentsize;
        SectionTableStringTableLocationIndex = ((OP_FULL_ELF_HDR32 *)Addr) -> e_shstrndx;
        ProgramTableFieldsCount = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phnum;
        ProgramTableFieldSize = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phentsize;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpSect,&SectionTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < sizeof(OP_FULL_ELF_SHDR32))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpStr = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + SectionTableStringTableLocationIndex * SectionTableFieldSize)) -> sh_offset;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpStr,&StringTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < SectionTableFieldSize * SectionTableFieldsCount)
          return EOPAttemptToGetOutsideOfStreamBounds;
        for (i = 1; i < SectionTableFieldsCount; i ++) // We gonna skip index-0 section
         {
             tmpSect = (OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize);
             if (RelDynSectionAddress)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".rel.dyn"))
                {
                  RelDynSectionAddress[0] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  RelDynSectionAddress[1] = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  if (puRelDynSectionSize)
                   *puRelDynSectionSize = ((OP_FULL_ELF_SHDR32 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_size;
                  break;
                }


         }
     }
    else
     {
        OP_FULL_ELF_SHDR64 *tmpSect;
        if (intRemSize < sizeof(OP_FULL_ELF_HDR64))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpSect = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shoff;
        etmpPhdr = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phoff;
        SectionTableFieldsCount = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shnum;
        SectionTableFieldSize = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shentsize;
        SectionTableStringTableLocationIndex = ((OP_FULL_ELF_HDR64 *)Addr) -> e_shstrndx;
        ProgramTableFieldsCount = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phnum;
        ProgramTableFieldSize = ((OP_FULL_ELF_HDR32 *)Addr) -> e_phentsize;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpSect,&SectionTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < sizeof(OP_FULL_ELF_SHDR64))
          return EOPAttemptToGetOutsideOfStreamBounds;
        etmpStr = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + SectionTableStringTableLocationIndex * SectionTableFieldSize)) -> sh_offset;
        tmpErr = OPJumpToOffsetInStream(fStream,etmpStr,&StringTableStartPtr,&intRemSize);
        if (tmpErr)
          return tmpErr;
        if (intRemSize < SectionTableFieldSize * SectionTableFieldsCount)
          return EOPAttemptToGetOutsideOfStreamBounds;
        for (i = 1; i < SectionTableFieldsCount; i ++) // We gonna skip index-0 section
         {
             tmpSect = (OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize);
             if (RelDynSectionAddress)
               if (!strcmp((char *) (StringTableStartPtr + (tmpSect -> sh_name)),".rel.dyn"))
                {
                  RelDynSectionAddress[0] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_addr;
                  RelDynSectionAddress[1] = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_offset;
                  if (puRelDynSectionSize)
                   *puRelDynSectionSize = ((OP_FULL_ELF_SHDR64 *)(SectionTableStartPtr + i * SectionTableFieldSize)) -> sh_size;
                  break;
                }

         }
    }
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPELFFindRelocationInSpecificAddressRange(const TSTREAMADDR_HDR fStream, const uint64_t uStartLookupAddress, const uint64_t uEndLookupAddress,void **pResultingRelocationAddress, TBOOL *bIsRelocationExists)
{
   uint64_t RDSAddr[2];
   uint64_t RDSSize;
   uint64_t i;
   TOPError Res;
   if (!fStream || !pResultingRelocationAddress || !bIsRelocationExists)
      return EOPAttemptToReferenceMemoryAtNULLAddress;
   *bIsRelocationExists = TB_FALSE;
   Res = OPELFGetDynamicRelocationTable(fStream,RDSAddr,&RDSSize);
   if (Res)
    return Res;
   uint64_t intRemSize;
   TSTREAMADDR_NOHDR Addr;
   OPJumpToOffsetInStream(fStream,0x0,&Addr,&intRemSize);
   if (intRemSize < sizeof(OP_STRIPPED_ELF_HDR))
     return EOPAttemptToGetOutsideOfStreamBounds;
   if ((((OP_STRIPPED_ELF_HDR *)Addr) -> e_ident)[OPEI_CLASS] == OPELFCLASS32)
    {
      OPJumpToOffsetInStream(fStream,RDSAddr[1],&Addr,&intRemSize);
      for (i = 0; i < RDSSize / 8 ; i ++)
       {
          if (   ((((Elf32_Rel *)(Addr + 8*i)) -> r_offset) >= uStartLookupAddress) && ((((Elf32_Rel *)(Addr + 8*i)) -> r_offset) <= uEndLookupAddress)   )
           {
              *bIsRelocationExists = TB_TRUE;
              *pResultingRelocationAddress = (void *)(Addr + 8*i);
           }
       }
    }
   else
    {
      OPJumpToOffsetInStream(fStream,RDSAddr[1],&Addr,&intRemSize);
      for (i = 0; i < RDSSize / 16 ; i ++)
       {
          if (   ((((Elf64_Rel *)(Addr + 16*i)) -> r_offset) >= uStartLookupAddress) && ((((Elf64_Rel *)(Addr + 16*i)) -> r_offset) <= uEndLookupAddress)   )
           {
              *bIsRelocationExists = TB_TRUE;
              *pResultingRelocationAddress = (void *)(Addr + 16*i);
           }
       }
    }
   return EOPSuccessfullOperation;

}

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
                                       uint32_t *ImportTable)
{
  uint64_t intRemSize;
  TOPError tmpErr;
  TSTREAMADDR_NOHDR Addr,Addr2,intSect;
  uint16_t SectionsCount,i;
  uint16_t intSubSystem;
  OP_IMAGE_SECTION_HEADER *SectionTablePtr;
  if (!fStream)
   return EOPAttemptToReferenceMemoryAtNULLAddress;

  if (Base)
    *Base = 0;
  if (MajorOperatingSystemVersion)
    *MajorOperatingSystemVersion = 0;
  if (MinorOperatingSystemVersion)
    *MinorOperatingSystemVersion = 0;
  if (MajorImageVersion)
    *MajorImageVersion = 0;
  if (MinorImageVersion)
    *MinorImageVersion = 0;
  if (CheckSum)
    *CheckSum = 0;
  if (Subsystem)
    *Subsystem = 0;
  if (SectionTable)
    *SectionTable = 0;
  if (Text)
    *Text = 0;
  if (Data)
    *Data = 0;
  if (RData)
    *RData = 0;
  if (Reloc)
    *Reloc = 0;
  if (ExportTable)
    *ExportTable = 0;
  if (sizeOfExportSection)
   {
      *sizeOfExportSection = 0;
   }
  if (ImportTable)
    *ImportTable = 0;
  OPJumpToOffsetInStream(fStream,0x0,&Addr,&intRemSize);
  if (intRemSize < sizeof(OP_IMAGE_DOS_HEADER))
    return EOPAttemptToGetOutsideOfStreamBounds;
  Addr2 = Addr;
  tmpErr = OPJumpToOffsetInStream(fStream,((OP_IMAGE_DOS_HEADER *)Addr2) -> e_lfanew,&Addr,&intRemSize);
  if (tmpErr)
    return EOPAttemptToGetOutsideOfStreamBounds;
  if (intRemSize < sizeof(OP_STRIPPED_IMAGE_NT_HEADERS))
    return EOPAttemptToGetOutsideOfStreamBounds;
  if (*((uint16_t *)(Addr + sizeof(uint32_t) + sizeof(OP_IMAGE_FILE_HEADER))) == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
   {
       OP_FULL_IMAGE_NT_HEADERS32 *intPEHdr;
       if (intRemSize < sizeof(OP_FULL_IMAGE_NT_HEADERS32))
         return EOPAttemptToGetOutsideOfStreamBounds;
       intPEHdr = (OP_FULL_IMAGE_NT_HEADERS32 *) Addr;
       intSubSystem = (intPEHdr -> OptionalHeader).Subsystem;
       if (Base)
        {
          *Base = (intPEHdr -> OptionalHeader).ImageBase;
          if (!(*Base))
           {
             if (intSubSystem != OPIMAGE_SUBSYSTEM_WINDOWS_CE_GUI)
              *Base += ((((intPEHdr -> FileHeader).Characteristics) & OPIMAGE_FILE_DLL) ? 0x10000000 : 0x400000);
             else
              *Base += 0x10000;
           }
        }
       if (MajorOperatingSystemVersion)
        *MajorOperatingSystemVersion = (intPEHdr -> OptionalHeader).MajorOperatingSystemVersion;
       if (MinorOperatingSystemVersion)
        *MinorOperatingSystemVersion = (intPEHdr -> OptionalHeader).MinorOperatingSystemVersion;
       if (MajorImageVersion)
        *MajorImageVersion = (intPEHdr -> OptionalHeader).MajorImageVersion;
       if (MinorImageVersion)
        *MinorImageVersion = (intPEHdr -> OptionalHeader).MinorImageVersion;
       if (CheckSum)
        *CheckSum = (intPEHdr -> OptionalHeader).CheckSum;
       if (Subsystem)
        *Subsystem = intSubSystem;

       if (ExportTable)
          *ExportTable = (intPEHdr -> OptionalHeader).DataDirectory[OPIMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
       if (sizeOfExportSection)
          *sizeOfExportSection = (intPEHdr -> OptionalHeader).DataDirectory[OPIMAGE_DIRECTORY_ENTRY_EXPORT].Size;
       if (ImportTable)
          *ImportTable = (intPEHdr -> OptionalHeader).DataDirectory[OPIMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;


       intSect = Addr + OPIMAGE_SIZEOF_FILE_HEADER + (intPEHdr -> FileHeader).SizeOfOptionalHeader + 4;  // FiX
       if (SectionTable)
         *SectionTable = intSect - Addr;
       SectionsCount = (intPEHdr -> FileHeader).NumberOfSections;
       SectionTablePtr = (OP_IMAGE_SECTION_HEADER *)((void *)intSect);
       for (i = 0; i < SectionsCount; i ++)
        {
           if (Text)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".text"))
                {
                  Text[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  Text[1] = SectionTablePtr[i].PointerToRawData;
                  Text[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
           if (Data)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".data"))
                {
                  Data[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  Data[1] = SectionTablePtr[i].PointerToRawData;
                  Data[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
           if (RData)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".rdata"))
                {
                  RData[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  RData[1] = SectionTablePtr[i].PointerToRawData;
                  RData[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
           if (Reloc)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".reloc"))
                {
                  Reloc[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  Reloc[1] = SectionTablePtr[i].PointerToRawData;
                  Reloc[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
        }
   }
  else
   {
       OP_FULL_IMAGE_NT_HEADERS64 *intPEHdr;
       if (intRemSize < sizeof(OP_FULL_IMAGE_NT_HEADERS64))
         return EOPAttemptToGetOutsideOfStreamBounds;
       intPEHdr = (OP_FULL_IMAGE_NT_HEADERS64 *) Addr;
       intSubSystem = (intPEHdr -> OptionalHeader).Subsystem;
       if (Base)
        {
          *Base = (intPEHdr -> OptionalHeader).ImageBase;
          if (!(*Base))
           {
             if (intSubSystem != OPIMAGE_SUBSYSTEM_WINDOWS_CE_GUI)
              *Base += ((((intPEHdr -> FileHeader).Characteristics) & OPIMAGE_FILE_DLL) ? 0x10000000 : 0x400000);
             else
              *Base += 0x10000;
           }
        }
       if (MajorOperatingSystemVersion)
        *MajorOperatingSystemVersion = (intPEHdr -> OptionalHeader).MajorOperatingSystemVersion;
       if (MinorOperatingSystemVersion)
        *MinorOperatingSystemVersion = (intPEHdr -> OptionalHeader).MinorOperatingSystemVersion;
       if (MajorImageVersion)
        *MajorImageVersion = (intPEHdr -> OptionalHeader).MajorImageVersion;
       if (MinorImageVersion)
        *MinorImageVersion = (intPEHdr -> OptionalHeader).MinorImageVersion;
       if (CheckSum)
        *CheckSum = (intPEHdr -> OptionalHeader).CheckSum;
       if (Subsystem)
        *Subsystem = intSubSystem;

       if (ExportTable)
          *ExportTable = (intPEHdr -> OptionalHeader).DataDirectory[OPIMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
       if (sizeOfExportSection)
          *sizeOfExportSection = (intPEHdr -> OptionalHeader).DataDirectory[OPIMAGE_DIRECTORY_ENTRY_EXPORT].Size;
       if (ImportTable)
          *ImportTable = (intPEHdr -> OptionalHeader).DataDirectory[OPIMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;


       intSect = Addr + OPIMAGE_SIZEOF_FILE_HEADER + (intPEHdr -> FileHeader).SizeOfOptionalHeader + 4; // FiX
       if (SectionTable)
         *SectionTable = intSect - Addr;
       SectionsCount = (intPEHdr -> FileHeader).NumberOfSections;
       SectionTablePtr = (OP_IMAGE_SECTION_HEADER *)((void *)intSect);
       for (i = 0; i < SectionsCount; i ++)
        {
           if (Text)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".text"))
                {
                  Text[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  Text[1] = SectionTablePtr[i].PointerToRawData;
                  Text[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
           if (Data)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".data"))
                {
                  Data[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  Data[1] = SectionTablePtr[i].PointerToRawData;
                  Data[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
           if (RData)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".rdata"))
                {
                  RData[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  RData[1] = SectionTablePtr[i].PointerToRawData;
                  RData[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
           if (Reloc)
            {
               if (!strcmp(((char *)SectionTablePtr[i].Name),".reloc"))
                {
                  Reloc[0] =  SectionTablePtr[i].PointerToRawData + OPIMAGE_SIZEOF_FILE_HEADER + Addr - Addr2;
                  Reloc[1] = SectionTablePtr[i].PointerToRawData;
                  Reloc[2] = SectionTablePtr[i].VirtualAddress;
                }
               continue;
            }
        }
   }
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPPEGetDLLCharacteristics(const TSTREAMADDR_HDR fStream,
                                           uint16_t *PEDllCharacteristics)
{
  uint64_t intRemSize;
  TOPError tmpErr;
  TSTREAMADDR_NOHDR Addr,Addr2;
  if (!fStream || !PEDllCharacteristics)
   return EOPAttemptToReferenceMemoryAtNULLAddress;

  OPJumpToOffsetInStream(fStream,0x0,&Addr,&intRemSize);
  if (intRemSize < sizeof(OP_IMAGE_DOS_HEADER))
    return EOPAttemptToGetOutsideOfStreamBounds;
  Addr2 = Addr;
  tmpErr = OPJumpToOffsetInStream(fStream,((OP_IMAGE_DOS_HEADER *)Addr2) -> e_lfanew,&Addr,&intRemSize);
  if (tmpErr)
    return EOPAttemptToGetOutsideOfStreamBounds;
  if (intRemSize < sizeof(OP_STRIPPED_IMAGE_NT_HEADERS))
    return EOPAttemptToGetOutsideOfStreamBounds;
  if (*((uint16_t *)(Addr + sizeof(uint32_t) + sizeof(OP_IMAGE_FILE_HEADER))) == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
   {
       OP_FULL_IMAGE_NT_HEADERS32 *intPEHdr;
       if (intRemSize < sizeof(OP_FULL_IMAGE_NT_HEADERS32))
         return EOPAttemptToGetOutsideOfStreamBounds;
       intPEHdr = (OP_FULL_IMAGE_NT_HEADERS32 *) Addr;
       *PEDllCharacteristics = (intPEHdr -> OptionalHeader).DllCharacteristics;
   }
  else
   {
       OP_FULL_IMAGE_NT_HEADERS64 *intPEHdr;
       if (intRemSize < sizeof(OP_FULL_IMAGE_NT_HEADERS64))
         return EOPAttemptToGetOutsideOfStreamBounds;
       intPEHdr = (OP_FULL_IMAGE_NT_HEADERS64 *) Addr;
       *PEDllCharacteristics = (intPEHdr -> OptionalHeader).DllCharacteristics;
   }
  return EOPSuccessfullOperation;
}


TOPError LIBFUNC OPPESetDLLCharacteristics(const TSTREAMADDR_HDR fStream,
                                           uint16_t PEDllCharacteristics)
{
   uint64_t intRemSize;
  TOPError tmpErr;
  TSTREAMADDR_NOHDR Addr,Addr2;
  if (!fStream)
   return EOPAttemptToReferenceMemoryAtNULLAddress;

  OPJumpToOffsetInStream(fStream,0x0,&Addr,&intRemSize);
  if (intRemSize < sizeof(OP_IMAGE_DOS_HEADER))
    return EOPAttemptToGetOutsideOfStreamBounds;
  Addr2 = Addr;
  tmpErr = OPJumpToOffsetInStream(fStream,((OP_IMAGE_DOS_HEADER *)Addr2) -> e_lfanew,&Addr,&intRemSize);
  if (tmpErr)
    return EOPAttemptToGetOutsideOfStreamBounds;
  if (intRemSize < sizeof(OP_STRIPPED_IMAGE_NT_HEADERS))
    return EOPAttemptToGetOutsideOfStreamBounds;
  if (*((uint16_t *)(Addr + sizeof(uint32_t) + sizeof(OP_IMAGE_FILE_HEADER))) == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
   {
       OP_FULL_IMAGE_NT_HEADERS32 *intPEHdr;
       if (intRemSize < sizeof(OP_FULL_IMAGE_NT_HEADERS32))
         return EOPAttemptToGetOutsideOfStreamBounds;
       intPEHdr = (OP_FULL_IMAGE_NT_HEADERS32 *) Addr;
       (intPEHdr -> OptionalHeader).DllCharacteristics = PEDllCharacteristics;
   }
  else
   {
       OP_FULL_IMAGE_NT_HEADERS64 *intPEHdr;
       if (intRemSize < sizeof(OP_FULL_IMAGE_NT_HEADERS64))
         return EOPAttemptToGetOutsideOfStreamBounds;
       intPEHdr = (OP_FULL_IMAGE_NT_HEADERS64 *) Addr;
       (intPEHdr -> OptionalHeader).DllCharacteristics = PEDllCharacteristics;
   }
  return EOPSuccessfullOperation;
}



TOPError LIBFUNC OPCalculatePEChecksum(TSTREAMADDR_NOHDR fStream,uint32_t ChecksumCalculationStartingBase,uint64_t AmmountOfWordsInFile,uint16_t *CalculatedChecksum)
{
   uint64_t AmmountOfBytesInFile = AmmountOfWordsInFile << 1,AmmountOfFinishedData,Temp1;
   uint8_t AmmountOfBytesInFileSizePowerOfTwo = 0,AmmountOfRemainedData,k;
   uint32_t ChkSumTmp = ChecksumCalculationStartingBase;
   if (!fStream || !CalculatedChecksum)
     return EOPAttemptToReferenceMemoryAtNULLAddress;
   Temp1 = AmmountOfBytesInFile;
   do
    {
      Temp1 >>= 1;
      AmmountOfBytesInFileSizePowerOfTwo ++;
    }
   while ((Temp1 != 1) && (AmmountOfBytesInFileSizePowerOfTwo < 8));
   if (AmmountOfBytesInFileSizePowerOfTwo < 3)
    {
        /* MS PE Addictive checksumming algorithm does not support files smaller than 8 bytes */
    }
   if (AmmountOfBytesInFileSizePowerOfTwo < 7)
    {
       uint8_t i;
       Temp1 = (1 << (AmmountOfBytesInFileSizePowerOfTwo - 2)) - 1;
       ChkSumTmp = opadd(ChkSumTmp,((uint32_t *)fStream)[0]);
       for (i = 1; i < Temp1; i ++)
        {
            ChkSumTmp = opadc(ChkSumTmp,((uint32_t *)fStream)[i]);
        }
    }
   else
    {
       uint32_t i,j;
       Temp1 = (1 << (AmmountOfBytesInFileSizePowerOfTwo - 2)) - 1;
       for (i = 0; i < (AmmountOfBytesInFile >> 7); i ++)
        {
          ChkSumTmp = opadd(ChkSumTmp,((uint32_t *)(fStream + (i << 7)))[0]);
          for (j = 1; j < Temp1; j ++)
           {
              ChkSumTmp = opadc(ChkSumTmp,((uint32_t *)(fStream + (i << 7)))[j]);
           }
        }
    }
  AmmountOfFinishedData = (AmmountOfBytesInFile >> AmmountOfBytesInFileSizePowerOfTwo) << AmmountOfBytesInFileSizePowerOfTwo;
  AmmountOfRemainedData = AmmountOfBytesInFile - AmmountOfFinishedData;
  for (k = 0; k < AmmountOfRemainedData; k ++)
   {
     uint32_t tmpzer = 0;
     ChkSumTmp = opadd(ChkSumTmp,((uint8_t *)(fStream + AmmountOfFinishedData))[k]);
     ChkSumTmp = opadc(ChkSumTmp,tmpzer);
   }
  ChkSumTmp = (ChkSumTmp >> 16) + (ChkSumTmp & 0xFFFF); //HIWORD + LOWORD
  *CalculatedChecksum = (ChkSumTmp + (ChkSumTmp >> 16)) & 0xFFFF;
  return EOPSuccessfullOperation;
}

TOPError LIBFUNC OPPEGetExportedFunctionInfo(const TSTREAMADDR_HDR fStream,
                                             char *FunctionName,
                                             uint16_t *FunctionOrdinal,
                                             uint32_t *FunctionLocation,
                                             char **NameFromOrdinal,
                                             TBOOL *IfItIsForwarder)
{
  uint32_t expt,expt_size,i;
  uint64_t intRemSize;
  OPIMAGE_EXPORT_DIRECTORY *Addr;
  uint32_t *Names,*Funcs;
  uint16_t *Ordinals;
  TBOOL IfFound = TB_FALSE;
  if (IfItIsForwarder)
   *IfItIsForwarder = TB_FALSE;
  if (!fStream)
   return EOPAttemptToReferenceMemoryAtNULLAddress;
  OPPEGetAdditionalInfo(fStream,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,&expt,&expt_size,NULL);
  OPJumpToOffsetInStream(fStream,expt,((void *)&Addr),&intRemSize);
  OPJumpToOffsetInStream(fStream,Addr -> AddressOfNames,((void *)&Names),&intRemSize);
  OPJumpToOffsetInStream(fStream,Addr -> AddressOfNameOrdinals,((void *)&Ordinals),&intRemSize);
  OPJumpToOffsetInStream(fStream,Addr -> AddressOfFunctions,((void *)&Funcs),&intRemSize);
  if (FunctionName) // We are looking for function by name
   {
     for (i = 0;i < (Addr -> NumberOfNames); i ++)
      {
         char *intFuncName;
         OPJumpToOffsetInStream(fStream,Names[i],(TSTREAMADDR_NOHDR *)((void *)(&intFuncName)),&intRemSize);
         if (!intFuncName)
          {
            continue;
          }
         if (!strcmp(FunctionName,intFuncName))
          {
            uint16_t intFuncOrd;
            IfFound = TB_TRUE;
            intFuncOrd = Ordinals[i];
            if (FunctionOrdinal)
              *FunctionOrdinal = intFuncOrd;
            if (FunctionLocation)
              *FunctionLocation = Funcs[intFuncOrd];
            if (IfItIsForwarder)
             {
               if ((Funcs[intFuncOrd] < expt + expt_size) && (Funcs[intFuncOrd] > expt))
                 *IfItIsForwarder = TB_TRUE;
             }
            break;
          }
         else
          {
            continue;
          }
      }
   }
  else
    if (FunctionOrdinal)
     {
        if (FunctionLocation)
          *FunctionLocation = Funcs[*FunctionOrdinal];
        if (IfItIsForwarder)
         {
            if ((Funcs[*FunctionOrdinal] < expt + expt_size) && (Funcs[*FunctionOrdinal] > expt))
              *IfItIsForwarder = TB_TRUE;
         }
        for (i = 0;i < (Addr -> NumberOfNames); i ++)
         {
           if (Ordinals[i] == (*FunctionOrdinal))
            {
               IfFound = TB_TRUE;
               break;
            }
         }
        if ((i < (Addr -> NumberOfNames)) && NameFromOrdinal)
          OPJumpToOffsetInStream(fStream,Names[i],(TSTREAMADDR_NOHDR *)((void *)NameFromOrdinal),&intRemSize);

     }
  if (IfFound)
    return EOPSuccessfullOperation;
  else
    return EOPRequestedObjectNotFound;
}
