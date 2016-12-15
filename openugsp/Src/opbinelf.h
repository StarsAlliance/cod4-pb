#ifndef _OPBINELF_H_
#define _OPBINELF_H_


#ifdef cplusplus
  extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Executable And Linkable File Format /////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

typedef uint32_t	Elf32_Addr;
typedef uint16_t	Elf32_Half;
typedef uint32_t	Elf32_Off;
typedef int32_t	    Elf32_Sword;
typedef uint32_t	Elf32_Word;

/* 64-bit ELF base types. */
typedef uint64_t	Elf64_Addr;
typedef uint16_t	Elf64_Half;
typedef int16_t	    Elf64_SHalf;
typedef uint64_t	Elf64_Off;
typedef int32_t	    Elf64_Sword;
typedef uint32_t	Elf64_Word;
typedef uint64_t	Elf64_Xword;
typedef int64_t	    Elf64_Sxword;


#define OPEI_NIDENT 16

typedef struct {
    unsigned char   e_ident[OPEI_NIDENT];
    Elf32_Half        e_type;
    Elf32_Half        e_machine;
} OP_STRIPPED_ELF_HDR;

typedef struct {
  unsigned char	e_ident[OPEI_NIDENT];
  Elf32_Half	e_type;
  Elf32_Half	e_machine;
  Elf32_Word	e_version;
  Elf32_Addr	e_entry;  /* Entry point */
  Elf32_Off	e_phoff;
  Elf32_Off	e_shoff;
  Elf32_Word	e_flags;
  Elf32_Half	e_ehsize;
  Elf32_Half	e_phentsize;
  Elf32_Half	e_phnum;
  Elf32_Half	e_shentsize;
  Elf32_Half	e_shnum;
  Elf32_Half	e_shstrndx;
} OP_FULL_ELF_HDR32;

typedef struct {
  unsigned char	e_ident[OPEI_NIDENT];		/* ELF "magic number" */
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;		/* Entry point virtual address */
  Elf64_Off e_phoff;		/* Program header table file offset */
  Elf64_Off e_shoff;		/* Section header table file offset */
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} OP_FULL_ELF_HDR64;


/* -------------------------------------------------------Used for sections----------------------------------------------- */

/* sh_type */
#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

/*  p_type */
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7               /* Thread local storage segment */
#define PT_LOOS    0x60000000      /* OS-specific */
#define PT_HIOS    0x6fffffff      /* OS-specific */
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff


/* sh_flags */
#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKPROC	0xf0000000

/* special section indexes */
#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff

typedef struct {
  Elf32_Word	sh_name;
  Elf32_Word	sh_type;
  Elf32_Word	sh_flags;
  Elf32_Addr	sh_addr;
  Elf32_Off	    sh_offset;
  Elf32_Word	sh_size;
  Elf32_Word	sh_link;
  Elf32_Word	sh_info;
  Elf32_Word	sh_addralign;
  Elf32_Word	sh_entsize;
} OP_FULL_ELF_SHDR32;

typedef struct {
  Elf64_Word    sh_name;		/* Section name, index in string tbl */
  Elf64_Word    sh_type;		/* Type of section */
  Elf64_Xword   sh_flags;		/* Miscellaneous section attributes */
  Elf64_Addr    sh_addr;		/* Section virtual addr at execution */
  Elf64_Off     sh_offset;		/* Section file offset */
  Elf64_Xword   sh_size;		/* Size of section in bytes */
  Elf64_Word    sh_link;		/* Index of another section */
  Elf64_Word    sh_info;		/* Additional section information */
  Elf64_Xword   sh_addralign;	/* Section alignment */
  Elf64_Xword   sh_entsize;	/* Entry size if section holds table */
} OP_FULL_ELF_SHDR64;

typedef struct {
  Elf32_Word	p_type;
  Elf32_Off	    p_offset;
  Elf32_Addr	p_vaddr;
  Elf32_Addr	p_paddr;
  Elf32_Word	p_filesz;
  Elf32_Word	p_memsz;
  Elf32_Word	p_flags;
  Elf32_Word	p_align;
} OP_FULL_ELF_PHDR32;

typedef struct {
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;		/* Segment file offset */
  Elf64_Addr p_vaddr;		/* Segment virtual address */
  Elf64_Addr p_paddr;		/* Segment physical address */
  Elf64_Xword p_filesz;		/* Segment size in file */
  Elf64_Xword p_memsz;		/* Segment size in memory */
  Elf64_Xword p_align;		/* Segment alignment, file & memory */
} OP_FULL_ELF_PHDR64;

/// The following are used with relocations
#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)

#define ELF64_R_SYM(i)                  ((i) >> 32)
#define ELF64_R_TYPE(i)                 ((i) & 0xffffffff)

// Relocation types
#define R_386_NONE      0
#define R_386_32        1
#define R_386_PC32      2
#define R_386_GOT32     3
#define R_386_PLT32     4
#define R_386_COPY      5
#define R_386_GLOB_DAT  6
#define R_386_JMP_SLOT  7
#define R_386_RELATIVE  8
#define R_386_GOTOFF    9
#define R_386_GOTPC    10

typedef struct elf32_rel {
  Elf32_Addr    r_offset;
  Elf32_Word    r_info;
} Elf32_Rel;

typedef struct elf64_rel {
  Elf64_Addr r_offset;  ///< Location at which to apply the action
  Elf64_Xword r_info;   ///< index and type of relocation
} Elf64_Rel;

typedef struct elf32_rela{
  Elf32_Addr    r_offset;
  Elf32_Word    r_info;
  Elf32_Sword   r_addend;
} Elf32_Rela;

typedef struct elf64_rela {
  Elf64_Addr r_offset;  ///< Location at which to apply the action
  Elf64_Xword r_info;   ///< index and type of relocation
  Elf64_Sxword r_addend;        ///< Constant addend used to compute value
} Elf64_Rela;



/*                                                               End                                                       */


#define OPEI_MAG0	0
#define OPEI_MAG1	1
#define OPEI_MAG2	2
#define OPEI_MAG3 3

#define OPEI_CLASS 4

#define OPELFMAG0	0x7f
#define OPELFMAG1	'E'
#define OPELFMAG2	'L'
#define OPELFMAG3	'F'

#define OPELFCLASSNONE 0
#define OPELFCLASS32 1
#define OPELFCLASS64 2

#define ET_NONE	0
#define ET_REL	1
#define ET_EXEC	2
#define ET_DYN	3
#define ET_CORE	4
#define ET_LOOS	0xfe00
#define ET_HIOS	0xfeff
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

#define EM_386 3
#define EM_IA_64 50
#define EM_X86_64 62


#ifdef cplusplus
  }
#endif


#endif
