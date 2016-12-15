#ifndef _OPBINPE_H_
#define _OPBINPE_H_


#ifdef cplusplus
  extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Portable Executable Format And Other MS Formats /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
#define OPIMAGE_SIZEOF_FILE_HEADER	20
#define OPIMAGE_FILE_RELOCS_STRIPPED	1
#define OPIMAGE_FILE_EXECUTABLE_IMAGE	2
#define OPIMAGE_FILE_LINE_NUMS_STRIPPED	4
#define OPIMAGE_FILE_LOCAL_SYMS_STRIPPED	8
#define OPIMAGE_FILE_AGGRESIVE_WS_TRIM 	16
#define OPIMAGE_FILE_LARGE_ADDRESS_AWARE	32
#define OPIMAGE_FILE_BYTES_REVERSED_LO	128
#define OPIMAGE_FILE_32BIT_MACHINE	256
#define OPIMAGE_FILE_DEBUG_STRIPPED	512
#define OPIMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	1024
#define OPIMAGE_FILE_NET_RUN_FROM_SWAP	2048
#define OPIMAGE_FILE_SYSTEM	4096
#define OPIMAGE_FILE_DLL	8192
#define OPIMAGE_FILE_UP_SYSTEM_ONLY	16384
#define OPIMAGE_FILE_BYTES_REVERSED_HI	32768

#define OPIMAGE_FILE_MACHINE_UNKNOWN	0x0000
#define OPIMAGE_FILE_MACHINE_AM33		0x01d3 /* Matsushita AM33 */
#define OPIMAGE_FILE_MACHINE_AMD64	0x8664 /* x64 */
#define OPIMAGE_FILE_MACHINE_ARM		0x01c0 /* ARM little endian */
#define OPIMAGE_FILE_MACHINE_EBC		0x0ebc /* EFI byte code */
#define OPIMAGE_FILE_MACHINE_I386		0x014c /* Intel 386 or later processors
						  and compatible processors */
#define OPIMAGE_FILE_MACHINE_IA64		0x0200 /* Intel Itanium processor family */
#define OPIMAGE_FILE_MACHINE_M32R		0x9041 /* Mitsubishi M32R little endian */
#define OPIMAGE_FILE_MACHINE_MIPS16	0x0266 /* MIPS16 */
#define OPIMAGE_FILE_MACHINE_MIPSFPU	0x0366 /* MIPS with FPU */
#define OPIMAGE_FILE_MACHINE_MIPSFPU16	0x0466 /* MIPS16 with FPU */
#define OPIMAGE_FILE_MACHINE_POWERPC	0x01f0 /* Power PC little endian */
#define OPIMAGE_FILE_MACHINE_POWERPCFP	0x01f1 /* Power PC with floating point support */
#define OPIMAGE_FILE_MACHINE_R4000	0x0166 /* MIPS little endian */
#define OPIMAGE_FILE_MACHINE_SH3		0x01a2 /* Hitachi SH3 */
#define OPIMAGE_FILE_MACHINE_SH3DSP	0x01a3 /* Hitachi SH3 DSP */
#define OPIMAGE_FILE_MACHINE_SH4		0x01a6 /* Hitachi SH4 */
#define OPIMAGE_FILE_MACHINE_SH5		0x01a8 /* Hitachi SH5 */
#define OPIMAGE_FILE_MACHINE_THUMB	0x01c2 /* Thumb */
#define OPIMAGE_FILE_MACHINE_WCEMIPSV2	0x0169 /* MIPS little-endian WCE v2 */

#define OPIMAGE_DOS_SIGNATURE 0x5A4D
#define OPIMAGE_OS2_SIGNATURE 0x454E
#define OPIMAGE_OS2_SIGNATURE_LE 0x454C
#define OPIMAGE_VXD_SIGNATURE 0x454C
#define OPIMAGE_NT_SIGNATURE 0x00004550
#define OPIMAGE_NT_OPTIONAL_HDR32_MAGIC 0x10b
#define OPIMAGE_NT_OPTIONAL_HDR64_MAGIC 0x20b
#define OPIMAGE_ROM_OPTIONAL_HDR_MAGIC 0x107
#define OPIMAGE_SEPARATE_DEBUG_SIGNATURE 0x4944
#define OPIMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
#define OPIMAGE_SIZEOF_ROM_OPTIONAL_HEADER 56
#define OPIMAGE_SIZEOF_STD_OPTIONAL_HEADER 28
#define OPIMAGE_SIZEOF_NT_OPTIONAL_HEADER 224
#define OPIMAGE_SIZEOF_SHORT_NAME 8
#define OPIMAGE_SIZEOF_SECTION_HEADER 40
#define OPIMAGE_SIZEOF_SYMBOL 18
#define OPIMAGE_SIZEOF_AUX_SYMBOL 18
#define OPIMAGE_SIZEOF_RELOCATION 10
#define OPIMAGE_SIZEOF_BASE_RELOCATION 8
#define OPIMAGE_SIZEOF_LINENUMBER 6
#define OPIMAGE_SIZEOF_ARCHIVE_MEMBER_HDR 60
#define OPSIZEOF_RFPO_DATA 16

#define OPIMAGE_SUBSYSTEM_UNKNOWN	0
#define OPIMAGE_SUBSYSTEM_NATIVE	1
#define OPIMAGE_SUBSYSTEM_WINDOWS_GUI	2
#define OPIMAGE_SUBSYSTEM_WINDOWS_CUI	3
#define OPIMAGE_SUBSYSTEM_OS2_CUI		5
#define OPIMAGE_SUBSYSTEM_POSIX_CUI	7
#define OPIMAGE_SUBSYSTEM_NATIVE_WINDOWS	8
#define OPIMAGE_SUBSYSTEM_WINDOWS_CE_GUI	9
#define OPIMAGE_SUBSYSTEM_EFI_APPLICATION	10
#define OPIMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER	11
#define OPIMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER	12
#define OPIMAGE_SUBSYSTEM_EFI_ROM	13
#define OPIMAGE_SUBSYSTEM_XBOX	14

#define OPIMAGE_DIRECTORY_ENTRY_EXPORT 0
#define OPIMAGE_DIRECTORY_ENTRY_IMPORT 1
#define OPIMAGE_DIRECTORY_ENTRY_RESOURCE 2
#define OPIMAGE_DIRECTORY_ENTRY_EXCEPTION 3
#define OPIMAGE_DIRECTORY_ENTRY_SECURITY 4
#define OPIMAGE_DIRECTORY_ENTRY_BASERELOC 5
#define OPIMAGE_DIRECTORY_ENTRY_DEBUG 6

#define OPIMAGE_DIRECTORY_ENTRY_ARCHITECTURE 7
#define OPIMAGE_DIRECTORY_ENTRY_GLOBALPTR 8
#define OPIMAGE_DIRECTORY_ENTRY_TLS 9
#define OPIMAGE_DIRECTORY_ENTRY_LOAD_CONFIG 10
#define OPIMAGE_DIRECTORY_ENTRY_BOUND_IMPORT 11
#define OPIMAGE_DIRECTORY_ENTRY_IAT 12
#define OPIMAGE_DIRECTORY_ENTRY_DELAY_IMPORT 13
#define OPIMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14




typedef struct {
	uint16_t e_magic;
	uint16_t e_cblp;
	uint16_t e_cp;
	uint16_t e_crlc;
	uint16_t e_cparhdr;
	uint16_t e_minalloc;
	uint16_t e_maxalloc;
	uint16_t e_ss;
	uint16_t e_sp;
	uint16_t e_csum;
	uint16_t e_ip;
	uint16_t e_cs;
	uint16_t e_lfarlc;
	uint16_t e_ovno;
	uint16_t e_res[4];
	uint16_t e_oemid;
	uint16_t e_oeminfo;
	uint16_t e_res2[10];
	uint32_t e_lfanew;
} OP_IMAGE_DOS_HEADER;

typedef struct {
	uint16_t Machine;
	uint16_t NumberOfSections;
	uint32_t TimeDateStamp;
	uint32_t PointerToSymbolTable;
	uint32_t NumberOfSymbols;
	uint16_t SizeOfOptionalHeader;
	uint16_t Characteristics;
} OP_IMAGE_FILE_HEADER;


typedef struct {
    uint16_t    Magic;
    uint8_t   MajorLinkerVersion;
    uint8_t   MinorLinkerVersion;
    uint32_t   SizeOfCode;
    uint32_t   SizeOfInitializedData;
    uint32_t   SizeOfUninitializedData;
    uint32_t   AddressOfEntryPoint;
    uint32_t   BaseOfCode;
} OP_IMAGE_STANDARD_OPTIONAL_HEADER;

typedef struct {
	uint32_t Signature;
	OP_IMAGE_FILE_HEADER FileHeader;
	OP_IMAGE_STANDARD_OPTIONAL_HEADER CutOptHdr;
} OP_STRIPPED_IMAGE_NT_HEADERS;

typedef struct {
    uint32_t   VirtualAddress;
    uint32_t   Size;
} OP_IMAGE_DATA_DIRECTORY;

typedef struct {
    uint8_t Name[OPIMAGE_SIZEOF_SHORT_NAME];
    union {
      uint32_t PhysicalAddress;
      uint32_t VirtualSize;
    } Misc;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
} OP_IMAGE_SECTION_HEADER;

#define OP_IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

typedef struct {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Name;
    uint32_t Base;
    uint32_t NumberOfFunctions;
    uint32_t NumberOfNames;
    uint32_t AddressOfFunctions;
    uint32_t AddressOfNames;
    uint32_t AddressOfNameOrdinals;
} OPIMAGE_EXPORT_DIRECTORY;

typedef struct {
    uint16_t Hint;
    uint8_t Name[1];
} OPIMAGE_IMPORT_BY_NAME;

typedef struct {
    //
    // Standard fields.
    //

    uint16_t    Magic;
    uint8_t   MajorLinkerVersion;
    uint8_t   MinorLinkerVersion;
    uint32_t   SizeOfCode;
    uint32_t   SizeOfInitializedData;
    uint32_t   SizeOfUninitializedData;
    uint32_t   AddressOfEntryPoint;
    uint32_t   BaseOfCode;
    uint32_t   BaseOfData;

    //
    // NT additional fields.
    //

    uint32_t   ImageBase;
    uint32_t   SectionAlignment;
    uint32_t   FileAlignment;
    uint16_t    MajorOperatingSystemVersion;
    uint16_t    MinorOperatingSystemVersion;
    uint16_t    MajorImageVersion;
    uint16_t    MinorImageVersion;
    uint16_t    MajorSubsystemVersion;
    uint16_t    MinorSubsystemVersion;
    uint32_t   Win32VersionValue;
    uint32_t   SizeOfImage;
    uint32_t   SizeOfHeaders;
    uint32_t   CheckSum;
    uint16_t    Subsystem;
    uint16_t    DllCharacteristics;
    uint32_t   SizeOfStackReserve;
    uint32_t   SizeOfStackCommit;
    uint32_t   SizeOfHeapReserve;
    uint32_t   SizeOfHeapCommit;
    uint32_t   LoaderFlags;
    uint32_t   NumberOfRvaAndSizes;
    OP_IMAGE_DATA_DIRECTORY DataDirectory[OP_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} OP_IMAGE_OPTIONAL_HEADER32;


typedef struct {
    //
    // Standard fields.
    //

    uint16_t        Magic;
    uint8_t       MajorLinkerVersion;
    uint8_t       MinorLinkerVersion;
    uint32_t       SizeOfCode;
    uint32_t       SizeOfInitializedData;
    uint32_t       SizeOfUninitializedData;
    uint32_t       AddressOfEntryPoint;
    uint32_t       BaseOfCode;

    //
    // NT additional fields.
    //

    uint64_t   ImageBase;
    uint32_t       SectionAlignment;
    uint32_t       FileAlignment;
    uint16_t        MajorOperatingSystemVersion;
    uint16_t        MinorOperatingSystemVersion;
    uint16_t        MajorImageVersion;
    uint16_t        MinorImageVersion;
    uint16_t        MajorSubsystemVersion;
    uint16_t        MinorSubsystemVersion;
    uint32_t       Win32VersionValue;
    uint32_t       SizeOfImage;
    uint32_t       SizeOfHeaders;
    uint32_t       CheckSum;
    uint16_t        Subsystem;
    uint16_t        DllCharacteristics;
    uint64_t   SizeOfStackReserve;
    uint64_t   SizeOfStackCommit;
    uint64_t   SizeOfHeapReserve;
    uint64_t   SizeOfHeapCommit;
    uint32_t       LoaderFlags;
    uint32_t       NumberOfRvaAndSizes;
    OP_IMAGE_DATA_DIRECTORY DataDirectory[OP_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} OP_IMAGE_OPTIONAL_HEADER64;

#define IMAGE_SIZEOF_STD_OPTIONAL_HEADER      28
#define IMAGE_SIZEOF_NT_OPTIONAL32_HEADER    224
#define IMAGE_SIZEOF_NT_OPTIONAL64_HEADER    240

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC      0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC      0x20b

typedef struct {
    uint32_t Signature;
    OP_IMAGE_FILE_HEADER FileHeader;
    OP_IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} OP_FULL_IMAGE_NT_HEADERS64;

typedef struct {
    uint32_t Signature;
    OP_IMAGE_FILE_HEADER FileHeader;
    OP_IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} OP_FULL_IMAGE_NT_HEADERS32;


#ifdef cplusplus
  }
#endif

#endif
