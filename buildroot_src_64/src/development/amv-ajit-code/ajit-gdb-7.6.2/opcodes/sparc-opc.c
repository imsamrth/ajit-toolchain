/* Table of opcodes for the sparc.
   Copyright 1989, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
   2000, 2002, 2004, 2005, 2006, 2007, 2008, 2011, 2012
   Free Software Foundation, Inc.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.  If not, write to the
   Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */


/* FIXME-someday: perhaps the ,a's and such should be embedded in the
   instruction's name rather than the args.  This would make gas faster, pinsn
   slower, but would mess up some macros a bit.  xoxorich. */

#include "sysdep.h"
#include <stdio.h>
#include "opcode/sparc.h"

/* Some defines to make life easy.  */
#define MASK_V6		SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_V6)
#define MASK_V7		SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_V7)
#define MASK_V8		SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_V8)
#define MASK_SPARCLET	SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_SPARCLET)
#define MASK_SPARCLITE	SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_SPARCLITE)
#define MASK_V9		SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_V9)
#define MASK_V9A	SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_V9A)
#define MASK_V9B	SPARC_OPCODE_ARCH_MASK (SPARC_OPCODE_ARCH_V9B)

/* Bit masks of architectures supporting the insn.  */

#define v6		(MASK_V6 | MASK_V7 | MASK_V8 | MASK_SPARCLET \
			 | MASK_SPARCLITE | MASK_V9 | MASK_V9A | MASK_V9B)
/* v6 insns not supported on the sparclet.  */
#define v6notlet	(MASK_V6 | MASK_V7 | MASK_V8 \
			 | MASK_SPARCLITE | MASK_V9 | MASK_V9A | MASK_V9B)
#define v7		(MASK_V7 | MASK_V8 | MASK_SPARCLET \
			 | MASK_SPARCLITE | MASK_V9 | MASK_V9A | MASK_V9B)
/* Although not all insns are implemented in hardware, sparclite is defined
   to be a superset of v8.  Unimplemented insns trap and are then theoretically
   implemented in software.
   It's not clear that the same is true for sparclet, although the docs
   suggest it is.  Rather than complicating things, the sparclet assembler
   recognizes all v8 insns.  */
#define v8		(MASK_V8 | MASK_SPARCLET | MASK_SPARCLITE \
			 | MASK_V9 | MASK_V9A | MASK_V9B)
#define sparclet	(MASK_SPARCLET)
#define sparclite	(MASK_SPARCLITE)
#define v9		(MASK_V9 | MASK_V9A | MASK_V9B)
#define v9a		(MASK_V9A | MASK_V9B)
#define v9b		(MASK_V9B)
/* v6 insns not supported by v9.  */
#define v6notv9		(MASK_V6 | MASK_V7 | MASK_V8 \
			 | MASK_SPARCLET | MASK_SPARCLITE)
/* v9a instructions which would appear to be aliases to v9's impdep's
   otherwise.  */
#define v9notv9a	(MASK_V9)

/* Table of opcode architectures.
   The order is defined in opcode/sparc.h.  */

const struct sparc_opcode_arch sparc_opcode_archs[] =
{
  { "v6", MASK_V6 },
  { "v7", MASK_V6 | MASK_V7 },
  { "v8", MASK_V6 | MASK_V7 | MASK_V8 },
  { "sparclet", MASK_V6 | MASK_V7 | MASK_V8 | MASK_SPARCLET },
  { "sparclite", MASK_V6 | MASK_V7 | MASK_V8 | MASK_SPARCLITE },
  /* ??? Don't some v8 priviledged insns conflict with v9?  */
  { "v9", MASK_V6 | MASK_V7 | MASK_V8 | MASK_V9 },
  /* v9 with ultrasparc additions */
  { "v9a", MASK_V6 | MASK_V7 | MASK_V8 | MASK_V9 | MASK_V9A },
  /* v9 with cheetah additions */
  { "v9b", MASK_V6 | MASK_V7 | MASK_V8 | MASK_V9 | MASK_V9A | MASK_V9B },
  { NULL, 0 }
};

/* Given NAME, return it's architecture entry.  */

enum sparc_opcode_arch_val
sparc_opcode_lookup_arch (const char *name)
{
  const struct sparc_opcode_arch *p;

  for (p = &sparc_opcode_archs[0]; p->name; ++p)
    if (strcmp (name, p->name) == 0)
      return (enum sparc_opcode_arch_val) (p - &sparc_opcode_archs[0]);

  return SPARC_OPCODE_ARCH_BAD;
}

/* Branch condition field.  */
#define COND(x)		(((x) & 0xf) << 25)

/* Compare And Branch condition field.  */
#define CBCOND(x)	(((x) & 0x1f) << 25)

/* v9: Move (MOVcc and FMOVcc) condition field.  */
#define MCOND(x,i_or_f)	((((i_or_f) & 1) << 18) | (((x) >> 11) & (0xf << 14))) /* v9 */

/* v9: Move register (MOVRcc and FMOVRcc) condition field.  */
#define RCOND(x)	(((x) & 0x7) << 10)	/* v9 */

#define CONDA	(COND (0x8))
#define CONDCC	(COND (0xd))
#define CONDCS	(COND (0x5))
#define CONDE	(COND (0x1))
#define CONDG	(COND (0xa))
#define CONDGE	(COND (0xb))
#define CONDGU	(COND (0xc))
#define CONDL	(COND (0x3))
#define CONDLE	(COND (0x2))
#define CONDLEU	(COND (0x4))
#define CONDN	(COND (0x0))
#define CONDNE	(COND (0x9))
#define CONDNEG	(COND (0x6))
#define CONDPOS	(COND (0xe))
#define CONDVC	(COND (0xf))
#define CONDVS	(COND (0x7))

#define CONDNZ	CONDNE
#define CONDZ	CONDE
#define CONDGEU	CONDCC
#define CONDLU	CONDCS

#define FCONDA		(COND (0x8))
#define FCONDE		(COND (0x9))
#define FCONDG		(COND (0x6))
#define FCONDGE		(COND (0xb))
#define FCONDL		(COND (0x4))
#define FCONDLE		(COND (0xd))
#define FCONDLG		(COND (0x2))
#define FCONDN		(COND (0x0))
#define FCONDNE		(COND (0x1))
#define FCONDO		(COND (0xf))
#define FCONDU		(COND (0x7))
#define FCONDUE		(COND (0xa))
#define FCONDUG		(COND (0x5))
#define FCONDUGE	(COND (0xc))
#define FCONDUL		(COND (0x3))
#define FCONDULE	(COND (0xe))

#define FCONDNZ	FCONDNE
#define FCONDZ	FCONDE

#define ICC 		(0)	/* v9 */
#define XCC 		(1 << 12) /* v9 */
#define CBCOND_XCC	(1 << 21)
#define FCC(x)		(((x) & 0x3) << 11) /* v9 */
#define FBFCC(x)	(((x) & 0x3) << 20)	/* v9 */

/* The order of the opcodes in the table is significant:

	* The assembler requires that all instances of the same mnemonic must
	be consecutive.	If they aren't, the assembler will bomb at runtime.

	* The disassembler should not care about the order of the opcodes.  */

/* Entries for commutative arithmetic operations.  */
/* ??? More entries can make use of this.  */
#define COMMUTEOP(opcode, op3, arch_mask) \
{ opcode,	F3(2, op3, 0), F3(~2, ~op3, ~0)|ASI(~0),	"1,2,d", 0, 0, arch_mask }, \
{ opcode,	F3(2, op3, 1), F3(~2, ~op3, ~1),		"1,i,d", 0, 0, arch_mask }, \
{ opcode,	F3(2, op3, 1), F3(~2, ~op3, ~1),		"i,1,d", 0, 0, arch_mask }

const struct sparc_opcode sparc_opcodes[] = {

{ "ld",	F3(3, 0x00, 0), F3(~3, ~0x00, ~0),		"[1+2],d", 0, 0, v6 },
{ "ld",	F3(3, 0x00, 0), F3(~3, ~0x00, ~0)|RS2_G0,	"[1],d", 0, 0, v6 }, /* ld [rs1+%g0],d */
{ "ld",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1),		"[1+i],d", 0, 0, v6 },
{ "ld",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1),		"[i+1],d", 0, 0, v6 },
{ "ld",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1)|RS1_G0,	"[i],d", 0, 0, v6 },
{ "ld",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1)|SIMM13(~0),	"[1],d", 0, 0, v6 }, /* ld [rs1+0],d */
{ "ld",	F3(3, 0x20, 0), F3(~3, ~0x20, ~0),		"[1+2],g", 0, 0, v6 },
{ "ld",	F3(3, 0x20, 0), F3(~3, ~0x20, ~0)|RS2_G0,	"[1],g", 0, 0, v6 }, /* ld [rs1+%g0],d */
{ "ld",	F3(3, 0x20, 1), F3(~3, ~0x20, ~1),		"[1+i],g", 0, 0, v6 },
{ "ld",	F3(3, 0x20, 1), F3(~3, ~0x20, ~1),		"[i+1],g", 0, 0, v6 },
{ "ld",	F3(3, 0x20, 1), F3(~3, ~0x20, ~1)|RS1_G0,	"[i],g", 0, 0, v6 },
{ "ld",	F3(3, 0x20, 1), F3(~3, ~0x20, ~1)|SIMM13(~0),	"[1],g", 0, 0, v6 }, /* ld [rs1+0],d */

{ "ld",	F3(3, 0x21, 0), F3(~3, ~0x21, ~0)|RD(~0),	"[1+2],F", 0, 0, v6 },
{ "ld",	F3(3, 0x21, 0), F3(~3, ~0x21, ~0)|RS2_G0|RD(~0),"[1],F", 0, 0, v6 }, /* ld [rs1+%g0],d */
{ "ld",	F3(3, 0x21, 1), F3(~3, ~0x21, ~1)|RD(~0),	"[1+i],F", 0, 0, v6 },
{ "ld",	F3(3, 0x21, 1), F3(~3, ~0x21, ~1)|RD(~0),	"[i+1],F", 0, 0, v6 },
{ "ld",	F3(3, 0x21, 1), F3(~3, ~0x21, ~1)|RS1_G0|RD(~0),"[i],F", 0, 0, v6 },
{ "ld",	F3(3, 0x21, 1), F3(~3, ~0x21, ~1)|SIMM13(~0)|RD(~0),"[1],F", 0, 0, v6 }, /* ld [rs1+0],d */

{ "ld",	F3(3, 0x30, 0), F3(~3, ~0x30, ~0),		"[1+2],D", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x30, 0), F3(~3, ~0x30, ~0)|RS2_G0,	"[1],D", 0, 0, v6notv9 }, /* ld [rs1+%g0],d */
{ "ld",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1),		"[1+i],D", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1),		"[i+1],D", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1)|RS1_G0,	"[i],D", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1)|SIMM13(~0),	"[1],D", 0, 0, v6notv9 }, /* ld [rs1+0],d */
{ "ld",	F3(3, 0x31, 0), F3(~3, ~0x31, ~0),		"[1+2],C", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x31, 0), F3(~3, ~0x31, ~0)|RS2_G0,	"[1],C", 0, 0, v6notv9 }, /* ld [rs1+%g0],d */
{ "ld",	F3(3, 0x31, 1), F3(~3, ~0x31, ~1),		"[1+i],C", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x31, 1), F3(~3, ~0x31, ~1),		"[i+1],C", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x31, 1), F3(~3, ~0x31, ~1)|RS1_G0,	"[i],C", 0, 0, v6notv9 },
{ "ld",	F3(3, 0x31, 1), F3(~3, ~0x31, ~1)|SIMM13(~0),	"[1],C", 0, 0, v6notv9 }, /* ld [rs1+0],d */

/* The v9 LDUW is the same as the old 'ld' opcode, it is not the same as the
   'ld' pseudo-op in v9.  */
{ "lduw",	F3(3, 0x00, 0), F3(~3, ~0x00, ~0),		"[1+2],d", F_ALIAS, 0, v9 },
{ "lduw",	F3(3, 0x00, 0), F3(~3, ~0x00, ~0)|RS2_G0,	"[1],d", F_ALIAS, 0, v9 }, /* ld [rs1+%g0],d */
{ "lduw",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1),		"[1+i],d", F_ALIAS, 0, v9 },
{ "lduw",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1),		"[i+1],d", F_ALIAS, 0, v9 },
{ "lduw",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1)|RS1_G0,	"[i],d", F_ALIAS, 0, v9 },
{ "lduw",	F3(3, 0x00, 1), F3(~3, ~0x00, ~1)|SIMM13(~0),	"[1],d", F_ALIAS, 0, v9 }, /* ld [rs1+0],d */

{ "ldd",	F3(3, 0x03, 0), F3(~3, ~0x03, ~0)|ASI(~0),	"[1+2],d", 0, 0, v6 },
{ "ldd",	F3(3, 0x03, 0), F3(~3, ~0x03, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v6 }, /* ldd [rs1+%g0],d */
{ "ldd",	F3(3, 0x03, 1), F3(~3, ~0x03, ~1),		"[1+i],d", 0, 0, v6 },
{ "ldd",	F3(3, 0x03, 1), F3(~3, ~0x03, ~1),		"[i+1],d", 0, 0, v6 },
{ "ldd",	F3(3, 0x03, 1), F3(~3, ~0x03, ~1)|RS1_G0,	"[i],d", 0, 0, v6 },
{ "ldd",	F3(3, 0x03, 1), F3(~3, ~0x03, ~1)|SIMM13(~0),	"[1],d", 0, 0, v6 }, /* ldd [rs1+0],d */
{ "ldd",	F3(3, 0x23, 0), F3(~3, ~0x23, ~0)|ASI(~0),	"[1+2],H", 0, 0, v6 },
{ "ldd",	F3(3, 0x23, 0), F3(~3, ~0x23, ~0)|ASI_RS2(~0),	"[1],H", 0, 0, v6 }, /* ldd [rs1+%g0],d */
{ "ldd",	F3(3, 0x23, 1), F3(~3, ~0x23, ~1),		"[1+i],H", 0, 0, v6 },
{ "ldd",	F3(3, 0x23, 1), F3(~3, ~0x23, ~1),		"[i+1],H", 0, 0, v6 },
{ "ldd",	F3(3, 0x23, 1), F3(~3, ~0x23, ~1)|RS1_G0,	"[i],H", 0, 0, v6 },
{ "ldd",	F3(3, 0x23, 1), F3(~3, ~0x23, ~1)|SIMM13(~0),	"[1],H", 0, 0, v6 }, /* ldd [rs1+0],d */

{ "ldd",	F3(3, 0x33, 0), F3(~3, ~0x33, ~0)|ASI(~0),	"[1+2],D", 0, 0, v6notv9 },
{ "ldd",	F3(3, 0x33, 0), F3(~3, ~0x33, ~0)|ASI_RS2(~0),	"[1],D", 0, 0, v6notv9 }, /* ldd [rs1+%g0],d */
{ "ldd",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1),		"[1+i],D", 0, 0, v6notv9 },
{ "ldd",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1),		"[i+1],D", 0, 0, v6notv9 },
{ "ldd",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1)|RS1_G0,	"[i],D", 0, 0, v6notv9 },
{ "ldd",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1)|SIMM13(~0),	"[1],D", 0, 0, v6notv9 }, /* ldd [rs1+0],d */

{ "ldq",	F3(3, 0x22, 0), F3(~3, ~0x22, ~0)|ASI(~0),	"[1+2],J", 0, 0, v9 },
{ "ldq",	F3(3, 0x22, 0), F3(~3, ~0x22, ~0)|ASI_RS2(~0),	"[1],J", 0, 0, v9 }, /* ldd [rs1+%g0],d */
{ "ldq",	F3(3, 0x22, 1), F3(~3, ~0x22, ~1),		"[1+i],J", 0, 0, v9 },
{ "ldq",	F3(3, 0x22, 1), F3(~3, ~0x22, ~1),		"[i+1],J", 0, 0, v9 },
{ "ldq",	F3(3, 0x22, 1), F3(~3, ~0x22, ~1)|RS1_G0,	"[i],J", 0, 0, v9 },
{ "ldq",	F3(3, 0x22, 1), F3(~3, ~0x22, ~1)|SIMM13(~0),	"[1],J", 0, 0, v9 }, /* ldd [rs1+0],d */

{ "ldsb",	F3(3, 0x09, 0), F3(~3, ~0x09, ~0)|ASI(~0),	"[1+2],d", 0, 0, v6 },
{ "ldsb",	F3(3, 0x09, 0), F3(~3, ~0x09, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v6 }, /* ldsb [rs1+%g0],d */
{ "ldsb",	F3(3, 0x09, 1), F3(~3, ~0x09, ~1),		"[1+i],d", 0, 0, v6 },
{ "ldsb",	F3(3, 0x09, 1), F3(~3, ~0x09, ~1),		"[i+1],d", 0, 0, v6 },
{ "ldsb",	F3(3, 0x09, 1), F3(~3, ~0x09, ~1)|RS1_G0,	"[i],d", 0, 0, v6 },
{ "ldsb",	F3(3, 0x09, 1), F3(~3, ~0x09, ~1)|SIMM13(~0),	"[1],d", 0, 0, v6 }, /* ldsb [rs1+0],d */

{ "ldsh",	F3(3, 0x0a, 0), F3(~3, ~0x0a, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v6 }, /* ldsh [rs1+%g0],d */
{ "ldsh",	F3(3, 0x0a, 0), F3(~3, ~0x0a, ~0)|ASI(~0),	"[1+2],d", 0, 0, v6 },
{ "ldsh",	F3(3, 0x0a, 1), F3(~3, ~0x0a, ~1),		"[1+i],d", 0, 0, v6 },
{ "ldsh",	F3(3, 0x0a, 1), F3(~3, ~0x0a, ~1),		"[i+1],d", 0, 0, v6 },
{ "ldsh",	F3(3, 0x0a, 1), F3(~3, ~0x0a, ~1)|RS1_G0,	"[i],d", 0, 0, v6 },
{ "ldsh",	F3(3, 0x0a, 1), F3(~3, ~0x0a, ~1)|SIMM13(~0),	"[1],d", 0, 0, v6 }, /* ldsh [rs1+0],d */

{ "ldstub",	F3(3, 0x0d, 0), F3(~3, ~0x0d, ~0)|ASI(~0),	"[1+2],d", 0, 0, v6 },
{ "ldstub",	F3(3, 0x0d, 0), F3(~3, ~0x0d, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v6 }, /* ldstub [rs1+%g0],d */
{ "ldstub",	F3(3, 0x0d, 1), F3(~3, ~0x0d, ~1),		"[1+i],d", 0, 0, v6 },
{ "ldstub",	F3(3, 0x0d, 1), F3(~3, ~0x0d, ~1),		"[i+1],d", 0, 0, v6 },
{ "ldstub",	F3(3, 0x0d, 1), F3(~3, ~0x0d, ~1)|RS1_G0,	"[i],d", 0, 0, v6 },
{ "ldstub",	F3(3, 0x0d, 1), F3(~3, ~0x0d, ~1)|SIMM13(~0),	"[1],d", 0, 0, v6 }, /* ldstub [rs1+0],d */

{ "ldsw",	F3(3, 0x08, 0), F3(~3, ~0x08, ~0)|ASI(~0),	"[1+2],d", 0, 0, v9 },
{ "ldsw",	F3(3, 0x08, 0), F3(~3, ~0x08, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v9 }, /* ldsw [rs1+%g0],d */
{ "ldsw",	F3(3, 0x08, 1), F3(~3, ~0x08, ~1),		"[1+i],d", 0, 0, v9 },
{ "ldsw",	F3(3, 0x08, 1), F3(~3, ~0x08, ~1),		"[i+1],d", 0, 0, v9 },
{ "ldsw",	F3(3, 0x08, 1), F3(~3, ~0x08, ~1)|RS1_G0,	"[i],d", 0, 0, v9 },
{ "ldsw",	F3(3, 0x08, 1), F3(~3, ~0x08, ~1)|SIMM13(~0),	"[1],d", 0, 0, v9 }, /* ldsw [rs1+0],d */

{ "ldub",	F3(3, 0x01, 0), F3(~3, ~0x01, ~0)|ASI(~0),	"[1+2],d", 0, 0, v6 },
{ "ldub",	F3(3, 0x01, 0), F3(~3, ~0x01, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v6 }, /* ldub [rs1+%g0],d */
{ "ldub",	F3(3, 0x01, 1), F3(~3, ~0x01, ~1),		"[1+i],d", 0, 0, v6 },
{ "ldub",	F3(3, 0x01, 1), F3(~3, ~0x01, ~1),		"[i+1],d", 0, 0, v6 },
{ "ldub",	F3(3, 0x01, 1), F3(~3, ~0x01, ~1)|RS1_G0,	"[i],d", 0, 0, v6 },
{ "ldub",	F3(3, 0x01, 1), F3(~3, ~0x01, ~1)|SIMM13(~0),	"[1],d", 0, 0, v6 }, /* ldub [rs1+0],d */

{ "lduh",	F3(3, 0x02, 0), F3(~3, ~0x02, ~0)|ASI(~0),	"[1+2],d", 0, 0, v6 },
{ "lduh",	F3(3, 0x02, 0), F3(~3, ~0x02, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v6 }, /* lduh [rs1+%g0],d */
{ "lduh",	F3(3, 0x02, 1), F3(~3, ~0x02, ~1),		"[1+i],d", 0, 0, v6 },
{ "lduh",	F3(3, 0x02, 1), F3(~3, ~0x02, ~1),		"[i+1],d", 0, 0, v6 },
{ "lduh",	F3(3, 0x02, 1), F3(~3, ~0x02, ~1)|RS1_G0,	"[i],d", 0, 0, v6 },
{ "lduh",	F3(3, 0x02, 1), F3(~3, ~0x02, ~1)|SIMM13(~0),	"[1],d", 0, 0, v6 }, /* lduh [rs1+0],d */

{ "ldx",	F3(3, 0x0b, 0), F3(~3, ~0x0b, ~0)|ASI(~0),	"[1+2],d", 0, 0, v9 },
{ "ldx",	F3(3, 0x0b, 0), F3(~3, ~0x0b, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v9 }, /* ldx [rs1+%g0],d */
{ "ldx",	F3(3, 0x0b, 1), F3(~3, ~0x0b, ~1),		"[1+i],d", 0, 0, v9 },
{ "ldx",	F3(3, 0x0b, 1), F3(~3, ~0x0b, ~1),		"[i+1],d", 0, 0, v9 },
{ "ldx",	F3(3, 0x0b, 1), F3(~3, ~0x0b, ~1)|RS1_G0,	"[i],d", 0, 0, v9 },
{ "ldx",	F3(3, 0x0b, 1), F3(~3, ~0x0b, ~1)|SIMM13(~0),	"[1],d", 0, 0, v9 }, /* ldx [rs1+0],d */

{ "ldx",	F3(3, 0x21, 0)|RD(1), F3(~3, ~0x21, ~0)|RD(~1),	"[1+2],F", 0, 0, v9 },
{ "ldx",	F3(3, 0x21, 0)|RD(1), F3(~3, ~0x21, ~0)|RS2_G0|RD(~1),	"[1],F", 0, 0, v9 }, /* ld [rs1+%g0],d */
{ "ldx",	F3(3, 0x21, 1)|RD(1), F3(~3, ~0x21, ~1)|RD(~1),	"[1+i],F", 0, 0, v9 },
{ "ldx",	F3(3, 0x21, 1)|RD(1), F3(~3, ~0x21, ~1)|RD(~1),	"[i+1],F", 0, 0, v9 },
{ "ldx",	F3(3, 0x21, 1)|RD(1), F3(~3, ~0x21, ~1)|RS1_G0|RD(~1),	"[i],F", 0, 0, v9 },
{ "ldx",	F3(3, 0x21, 1)|RD(1), F3(~3, ~0x21, ~1)|SIMM13(~0)|RD(~1),"[1],F", 0, 0, v9 }, /* ld [rs1+0],d */

{ "ldx", F3(3, 0x21, 0)|RD(3), F3(~3, ~0x21, ~0)|RD(~3), "[1+2],(", 0, 0, v9b },
{ "ldx", F3(3, 0x21, 0)|RD(3), F3(~3, ~0x21, ~0)|RS2_G0|RD(~3),"[1],(", 0, 0, v9b },
{ "ldx", F3(3, 0x21, 1)|RD(3), F3(~3, ~0x21, ~1)|RD(~3), "[1+i],(", 0, 0, v9b },
{ "ldx", F3(3, 0x21, 1)|RD(3), F3(~3, ~0x21, ~1)|RD(~3), "[i+1],(", 0, 0, v9b },
{ "ldx", F3(3, 0x21, 1)|RD(3), F3(~3, ~0x21, ~1)|RS1_G0|RD(~3),"[i],(", 0, 0, v9b },
{ "ldx", F3(3, 0x21, 1)|RD(3), F3(~3, ~0x21, ~1)|SIMM13(~0)|RD(~3),"[1],(", 0, 0, v9b },

{ "lda",	F3(3, 0x10, 0), F3(~3, ~0x10, ~0),		"[1+2]A,d", 0, 0, v6 },
{ "lda",	F3(3, 0x10, 0), F3(~3, ~0x10, ~0)|RS2_G0,	"[1]A,d", 0, 0, v6 }, /* lda [rs1+%g0],d */
{ "lda",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "lda",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "lda",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "lda",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */
{ "lda",	F3(3, 0x30, 0), F3(~3, ~0x30, ~0),		"[1+2]A,g", 0, 0, v9 },
{ "lda",	F3(3, 0x30, 0), F3(~3, ~0x30, ~0)|RS2_G0,	"[1]A,g", 0, 0, v9 }, /* lda [rs1+%g0],d */
{ "lda",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1),		"[1+i]o,g", 0, 0, v9 },
{ "lda",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1),		"[i+1]o,g", 0, 0, v9 },
{ "lda",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1)|RS1_G0,	"[i]o,g", 0, 0, v9 },
{ "lda",	F3(3, 0x30, 1), F3(~3, ~0x30, ~1)|SIMM13(~0),	"[1]o,g", 0, 0, v9 }, /* ld [rs1+0],d */

{ "ldda",	F3(3, 0x13, 0), F3(~3, ~0x13, ~0),		"[1+2]A,d", 0, 0, v6 },
{ "ldda",	F3(3, 0x13, 0), F3(~3, ~0x13, ~0)|RS2_G0,	"[1]A,d", 0, 0, v6 }, /* ldda [rs1+%g0],d */
{ "ldda",	F3(3, 0x13, 1), F3(~3, ~0x13, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "ldda",	F3(3, 0x13, 1), F3(~3, ~0x13, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "ldda",	F3(3, 0x13, 1), F3(~3, ~0x13, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "ldda",	F3(3, 0x13, 1), F3(~3, ~0x13, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "ldda",	F3(3, 0x33, 0), F3(~3, ~0x33, ~0),		"[1+2]A,H", 0, 0, v9 },
{ "ldda",	F3(3, 0x33, 0), F3(~3, ~0x33, ~0)|RS2_G0,	"[1]A,H", 0, 0, v9 }, /* ldda [rs1+%g0],d */
{ "ldda",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1),		"[1+i]o,H", 0, 0, v9 },
{ "ldda",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1),		"[i+1]o,H", 0, 0, v9 },
{ "ldda",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1)|RS1_G0,	"[i]o,H", 0, 0, v9 },
{ "ldda",	F3(3, 0x33, 1), F3(~3, ~0x33, ~1)|SIMM13(~0),	"[1]o,H", 0, 0, v9 }, /* ld [rs1+0],d */

{ "ldqa",	F3(3, 0x32, 0), F3(~3, ~0x32, ~0),		"[1+2]A,J", 0, 0, v9 },
{ "ldqa",	F3(3, 0x32, 0), F3(~3, ~0x32, ~0)|RS2_G0,	"[1]A,J", 0, 0, v9 }, /* ldd [rs1+%g0],d */
{ "ldqa",	F3(3, 0x32, 1), F3(~3, ~0x32, ~1),		"[1+i]o,J", 0, 0, v9 },
{ "ldqa",	F3(3, 0x32, 1), F3(~3, ~0x32, ~1),		"[i+1]o,J", 0, 0, v9 },
{ "ldqa",	F3(3, 0x32, 1), F3(~3, ~0x32, ~1)|RS1_G0,	"[i]o,J", 0, 0, v9 },
{ "ldqa",	F3(3, 0x32, 1), F3(~3, ~0x32, ~1)|SIMM13(~0),	"[1]o,J", 0, 0, v9 }, /* ldd [rs1+0],d */

{ "ldsba",	F3(3, 0x19, 0), F3(~3, ~0x19, ~0),		"[1+2]A,d", 0, 0, v6 },
{ "ldsba",	F3(3, 0x19, 0), F3(~3, ~0x19, ~0)|RS2_G0,	"[1]A,d", 0, 0, v6 }, /* ldsba [rs1+%g0],d */
{ "ldsba",	F3(3, 0x19, 1), F3(~3, ~0x19, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "ldsba",	F3(3, 0x19, 1), F3(~3, ~0x19, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "ldsba",	F3(3, 0x19, 1), F3(~3, ~0x19, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "ldsba",	F3(3, 0x19, 1), F3(~3, ~0x19, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "ldsha",	F3(3, 0x1a, 0), F3(~3, ~0x1a, ~0),		"[1+2]A,d", 0, 0, v6 },
{ "ldsha",	F3(3, 0x1a, 0), F3(~3, ~0x1a, ~0)|RS2_G0,	"[1]A,d", 0, 0, v6 }, /* ldsha [rs1+%g0],d */
{ "ldsha",	F3(3, 0x1a, 1), F3(~3, ~0x1a, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "ldsha",	F3(3, 0x1a, 1), F3(~3, ~0x1a, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "ldsha",	F3(3, 0x1a, 1), F3(~3, ~0x1a, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "ldsha",	F3(3, 0x1a, 1), F3(~3, ~0x1a, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "ldstuba",	F3(3, 0x1d, 0), F3(~3, ~0x1d, ~0),		"[1+2]A,d", 0, 0, v6 },
{ "ldstuba",	F3(3, 0x1d, 0), F3(~3, ~0x1d, ~0)|RS2_G0,	"[1]A,d", 0, 0, v6 }, /* ldstuba [rs1+%g0],d */
{ "ldstuba",	F3(3, 0x1d, 1), F3(~3, ~0x1d, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "ldstuba",	F3(3, 0x1d, 1), F3(~3, ~0x1d, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "ldstuba",	F3(3, 0x1d, 1), F3(~3, ~0x1d, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "ldstuba",	F3(3, 0x1d, 1), F3(~3, ~0x1d, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "ldswa",	F3(3, 0x18, 0), F3(~3, ~0x18, ~0),		"[1+2]A,d", 0, 0, v9 },
{ "ldswa",	F3(3, 0x18, 0), F3(~3, ~0x18, ~0)|RS2_G0,	"[1]A,d", 0, 0, v9 }, /* lda [rs1+%g0],d */
{ "ldswa",	F3(3, 0x18, 1), F3(~3, ~0x18, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "ldswa",	F3(3, 0x18, 1), F3(~3, ~0x18, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "ldswa",	F3(3, 0x18, 1), F3(~3, ~0x18, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "ldswa",	F3(3, 0x18, 1), F3(~3, ~0x18, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "lduba",	F3(3, 0x11, 0), F3(~3, ~0x11, ~0),		"[1+2]A,d", 0, 0, v6 },
{ "lduba",	F3(3, 0x11, 0), F3(~3, ~0x11, ~0)|RS2_G0,	"[1]A,d", 0, 0, v6 }, /* lduba [rs1+%g0],d */
{ "lduba",	F3(3, 0x11, 1), F3(~3, ~0x11, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "lduba",	F3(3, 0x11, 1), F3(~3, ~0x11, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "lduba",	F3(3, 0x11, 1), F3(~3, ~0x11, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "lduba",	F3(3, 0x11, 1), F3(~3, ~0x11, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "lduha",	F3(3, 0x12, 0), F3(~3, ~0x12, ~0),		"[1+2]A,d", 0, 0, v6 },
{ "lduha",	F3(3, 0x12, 0), F3(~3, ~0x12, ~0)|RS2_G0,	"[1]A,d", 0, 0, v6 }, /* lduha [rs1+%g0],d */
{ "lduha",	F3(3, 0x12, 1), F3(~3, ~0x12, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "lduha",	F3(3, 0x12, 1), F3(~3, ~0x12, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "lduha",	F3(3, 0x12, 1), F3(~3, ~0x12, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "lduha",	F3(3, 0x12, 1), F3(~3, ~0x12, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "lduwa",	F3(3, 0x10, 0), F3(~3, ~0x10, ~0),		"[1+2]A,d", F_ALIAS, 0, v9 }, /* lduwa === lda */
{ "lduwa",	F3(3, 0x10, 0), F3(~3, ~0x10, ~0)|RS2_G0,	"[1]A,d", F_ALIAS, 0, v9 }, /* lda [rs1+%g0],d */
{ "lduwa",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1),		"[1+i]o,d", F_ALIAS, 0, v9 },
{ "lduwa",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1),		"[i+1]o,d", F_ALIAS, 0, v9 },
{ "lduwa",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1)|RS1_G0,	"[i]o,d", F_ALIAS, 0, v9 },
{ "lduwa",	F3(3, 0x10, 1), F3(~3, ~0x10, ~1)|SIMM13(~0),	"[1]o,d", F_ALIAS, 0, v9 }, /* ld [rs1+0],d */

{ "ldxa",	F3(3, 0x1b, 0), F3(~3, ~0x1b, ~0),		"[1+2]A,d", 0, 0, v9 },
{ "ldxa",	F3(3, 0x1b, 0), F3(~3, ~0x1b, ~0)|RS2_G0,	"[1]A,d", 0, 0, v9 }, /* lda [rs1+%g0],d */
{ "ldxa",	F3(3, 0x1b, 1), F3(~3, ~0x1b, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "ldxa",	F3(3, 0x1b, 1), F3(~3, ~0x1b, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "ldxa",	F3(3, 0x1b, 1), F3(~3, ~0x1b, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "ldxa",	F3(3, 0x1b, 1), F3(~3, ~0x1b, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* ld [rs1+0],d */

{ "st",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI(~0),		"d,[1+2]", 0, 0, v6 },
{ "st",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI_RS2(~0),		"d,[1]", 0, 0, v6 }, /* st d,[rs1+%g0] */
{ "st",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),			"d,[1+i]", 0, 0, v6 },
{ "st",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),			"d,[i+1]", 0, 0, v6 },
{ "st",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RS1_G0,		"d,[i]", 0, 0, v6 },
{ "st",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|SIMM13(~0),		"d,[1]", 0, 0, v6 }, /* st d,[rs1+0] */
{ "st",	F3(3, 0x24, 0), F3(~3, ~0x24, ~0)|ASI(~0),		"g,[1+2]", 0, 0, v6 },
{ "st",	F3(3, 0x24, 0), F3(~3, ~0x24, ~0)|ASI_RS2(~0),		"g,[1]", 0, 0, v6 }, /* st d[rs1+%g0] */
{ "st",	F3(3, 0x24, 1), F3(~3, ~0x24, ~1),			"g,[1+i]", 0, 0, v6 },
{ "st",	F3(3, 0x24, 1), F3(~3, ~0x24, ~1),			"g,[i+1]", 0, 0, v6 },
{ "st",	F3(3, 0x24, 1), F3(~3, ~0x24, ~1)|RS1_G0,		"g,[i]", 0, 0, v6 },
{ "st",	F3(3, 0x24, 1), F3(~3, ~0x24, ~1)|SIMM13(~0),		"g,[1]", 0, 0, v6 }, /* st d,[rs1+0] */

{ "st",	F3(3, 0x34, 0), F3(~3, ~0x34, ~0)|ASI(~0),		"D,[1+2]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x34, 0), F3(~3, ~0x34, ~0)|ASI_RS2(~0),		"D,[1]", 0, 0, v6notv9 }, /* st d,[rs1+%g0] */
{ "st",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1),			"D,[1+i]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1),			"D,[i+1]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1)|RS1_G0,		"D,[i]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1)|SIMM13(~0),		"D,[1]", 0, 0, v6notv9 }, /* st d,[rs1+0] */
{ "st",	F3(3, 0x35, 0), F3(~3, ~0x35, ~0)|ASI(~0),		"C,[1+2]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x35, 0), F3(~3, ~0x35, ~0)|ASI_RS2(~0),		"C,[1]", 0, 0, v6notv9 }, /* st d,[rs1+%g0] */
{ "st",	F3(3, 0x35, 1), F3(~3, ~0x35, ~1),			"C,[1+i]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x35, 1), F3(~3, ~0x35, ~1),			"C,[i+1]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x35, 1), F3(~3, ~0x35, ~1)|RS1_G0,		"C,[i]", 0, 0, v6notv9 },
{ "st",	F3(3, 0x35, 1), F3(~3, ~0x35, ~1)|SIMM13(~0),		"C,[1]", 0, 0, v6notv9 }, /* st d,[rs1+0] */

{ "st",	F3(3, 0x25, 0), F3(~3, ~0x25, ~0)|RD_G0|ASI(~0),	"F,[1+2]", 0, 0, v6 },
{ "st",	F3(3, 0x25, 0), F3(~3, ~0x25, ~0)|RD_G0|ASI_RS2(~0),	"F,[1]", 0, 0, v6 }, /* st d,[rs1+%g0] */
{ "st",	F3(3, 0x25, 1), F3(~3, ~0x25, ~1)|RD_G0,		"F,[1+i]", 0, 0, v6 },
{ "st",	F3(3, 0x25, 1), F3(~3, ~0x25, ~1)|RD_G0,		"F,[i+1]", 0, 0, v6 },
{ "st",	F3(3, 0x25, 1), F3(~3, ~0x25, ~1)|RD_G0|RS1_G0,		"F,[i]", 0, 0, v6 },
{ "st",	F3(3, 0x25, 1), F3(~3, ~0x25, ~1)|RD_G0|SIMM13(~0),	"F,[1]", 0, 0, v6 }, /* st d,[rs1+0] */

{ "stw",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v9 },
{ "stw",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v9 }, /* st d,[rs1+%g0] */
{ "stw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[1+i]", F_ALIAS, 0, v9 },
{ "stw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[i+1]", F_ALIAS, 0, v9 },
{ "stw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v9 },
{ "stw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v9 }, /* st d,[rs1+0] */
{ "stsw",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v9 },
{ "stsw",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v9 }, /* st d,[rs1+%g0] */
{ "stsw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[1+i]", F_ALIAS, 0, v9 },
{ "stsw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[i+1]", F_ALIAS, 0, v9 },
{ "stsw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v9 },
{ "stsw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v9 }, /* st d,[rs1+0] */
{ "stuw",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v9 },
{ "stuw",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v9 }, /* st d,[rs1+%g0] */
{ "stuw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[1+i]", F_ALIAS, 0, v9 },
{ "stuw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[i+1]", F_ALIAS, 0, v9 },
{ "stuw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v9 },
{ "stuw",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v9 }, /* st d,[rs1+0] */

{ "spill",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v6 },
{ "spill",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* st d,[rs1+%g0] */
{ "spill",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[1+i]", F_ALIAS, 0, v6 },
{ "spill",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1),		"d,[i+1]", F_ALIAS, 0, v6 },
{ "spill",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v6 },
{ "spill",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* st d,[rs1+0] */

{ "sta",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0),		"d,[1+2]A", 0, 0, v6 },
{ "sta",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0)|RS2(~0),	"d,[1]A", 0, 0, v6 }, /* sta d,[rs1+%g0] */
{ "sta",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[1+i]o", 0, 0, v9 },
{ "sta",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[i+1]o", 0, 0, v9 },
{ "sta",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|RS1_G0,	"d,[i]o", 0, 0, v9 },
{ "sta",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|SIMM13(~0),	"d,[1]o", 0, 0, v9 }, /* st d,[rs1+0] */

{ "sta",	F3(3, 0x34, 0), F3(~3, ~0x34, ~0),		"g,[1+2]A", 0, 0, v9 },
{ "sta",	F3(3, 0x34, 0), F3(~3, ~0x34, ~0)|RS2(~0),	"g,[1]A", 0, 0, v9 }, /* sta d,[rs1+%g0] */
{ "sta",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1),		"g,[1+i]o", 0, 0, v9 },
{ "sta",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1),		"g,[i+1]o", 0, 0, v9 },
{ "sta",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1)|RS1_G0,	"g,[i]o", 0, 0, v9 },
{ "sta",	F3(3, 0x34, 1), F3(~3, ~0x34, ~1)|SIMM13(~0),	"g,[1]o", 0, 0, v9 }, /* st d,[rs1+0] */

{ "stwa",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0),		"d,[1+2]A", F_ALIAS, 0, v9 },
{ "stwa",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0)|RS2(~0),	"d,[1]A", F_ALIAS, 0, v9 }, /* sta d,[rs1+%g0] */
{ "stwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[1+i]o", F_ALIAS, 0, v9 },
{ "stwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[i+1]o", F_ALIAS, 0, v9 },
{ "stwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|RS1_G0,	"d,[i]o", F_ALIAS, 0, v9 },
{ "stwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|SIMM13(~0),	"d,[1]o", F_ALIAS, 0, v9 }, /* st d,[rs1+0] */
{ "stswa",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0),		"d,[1+2]A", F_ALIAS, 0, v9 },
{ "stswa",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0)|RS2(~0),	"d,[1]A", F_ALIAS, 0, v9 }, /* sta d,[rs1+%g0] */
{ "stswa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[1+i]o", F_ALIAS, 0, v9 },
{ "stswa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[i+1]o", F_ALIAS, 0, v9 },
{ "stswa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|RS1_G0,	"d,[i]o", F_ALIAS, 0, v9 },
{ "stswa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|SIMM13(~0),	"d,[1]o", F_ALIAS, 0, v9 }, /* st d,[rs1+0] */
{ "stuwa",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0),		"d,[1+2]A", F_ALIAS, 0, v9 },
{ "stuwa",	F3(3, 0x14, 0), F3(~3, ~0x14, ~0)|RS2(~0),	"d,[1]A", F_ALIAS, 0, v9 }, /* sta d,[rs1+%g0] */
{ "stuwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[1+i]o", F_ALIAS, 0, v9 },
{ "stuwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1),		"d,[i+1]o", F_ALIAS, 0, v9 },
{ "stuwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|RS1_G0,	"d,[i]o", F_ALIAS, 0, v9 },
{ "stuwa",	F3(3, 0x14, 1), F3(~3, ~0x14, ~1)|SIMM13(~0),	"d,[1]o", F_ALIAS, 0, v9 }, /* st d,[rs1+0] */

{ "stb",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|ASI(~0),	"d,[1+2]", 0, 0, v6 },
{ "stb",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|ASI_RS2(~0),	"d,[1]", 0, 0, v6 }, /* stb d,[rs1+%g0] */
{ "stb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1),		"d,[1+i]", 0, 0, v6 },
{ "stb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1),		"d,[i+1]", 0, 0, v6 },
{ "stb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|RS1_G0,	"d,[i]", 0, 0, v6 },
{ "stb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|SIMM13(~0),	"d,[1]", 0, 0, v6 }, /* stb d,[rs1+0] */

{ "stsb",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v6 },
{ "stsb",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* stb d,[rs1+%g0] */
{ "stsb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1),		"d,[1+i]", F_ALIAS, 0, v6 },
{ "stsb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1),		"d,[i+1]", F_ALIAS, 0, v6 },
{ "stsb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v6 },
{ "stsb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* stb d,[rs1+0] */
{ "stub",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v6 },
{ "stub",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* stb d,[rs1+%g0] */
{ "stub",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1),		"d,[1+i]", F_ALIAS, 0, v6 },
{ "stub",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1),		"d,[i+1]", F_ALIAS, 0, v6 },
{ "stub",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v6 },
{ "stub",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* stb d,[rs1+0] */

{ "stba",	F3(3, 0x15, 0), F3(~3, ~0x15, ~0),		"d,[1+2]A", 0, 0, v6 },
{ "stba",	F3(3, 0x15, 0), F3(~3, ~0x15, ~0)|RS2(~0),	"d,[1]A", 0, 0, v6 }, /* stba d,[rs1+%g0] */
{ "stba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1),		"d,[1+i]o", 0, 0, v9 },
{ "stba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1),		"d,[i+1]o", 0, 0, v9 },
{ "stba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1)|RS1_G0,	"d,[i]o", 0, 0, v9 },
{ "stba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1)|SIMM13(~0),	"d,[1]o", 0, 0, v9 }, /* stb d,[rs1+0] */

{ "stsba",	F3(3, 0x15, 0), F3(~3, ~0x15, ~0),		"d,[1+2]A", F_ALIAS, 0, v6 },
{ "stsba",	F3(3, 0x15, 0), F3(~3, ~0x15, ~0)|RS2(~0),	"d,[1]A", F_ALIAS, 0, v6 }, /* stba d,[rs1+%g0] */
{ "stsba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1),		"d,[1+i]o", F_ALIAS, 0, v9 },
{ "stsba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1),		"d,[i+1]o", F_ALIAS, 0, v9 },
{ "stsba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1)|RS1_G0,	"d,[i]o", F_ALIAS, 0, v9 },
{ "stsba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1)|SIMM13(~0),	"d,[1]o", F_ALIAS, 0, v9 }, /* stb d,[rs1+0] */
{ "stuba",	F3(3, 0x15, 0), F3(~3, ~0x15, ~0),		"d,[1+2]A", F_ALIAS, 0, v6 },
{ "stuba",	F3(3, 0x15, 0), F3(~3, ~0x15, ~0)|RS2(~0),	"d,[1]A", F_ALIAS, 0, v6 }, /* stba d,[rs1+%g0] */
{ "stuba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1),		"d,[1+i]o", F_ALIAS, 0, v9 },
{ "stuba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1),		"d,[i+1]o", F_ALIAS, 0, v9 },
{ "stuba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1)|RS1_G0,	"d,[i]o", F_ALIAS, 0, v9 },
{ "stuba",	F3(3, 0x15, 1), F3(~3, ~0x15, ~1)|SIMM13(~0),	"d,[1]o", F_ALIAS, 0, v9 }, /* stb d,[rs1+0] */

{ "std",	F3(3, 0x07, 0), F3(~3, ~0x07, ~0)|ASI(~0),	"d,[1+2]", 0, 0, v6 },
{ "std",	F3(3, 0x07, 0), F3(~3, ~0x07, ~0)|ASI_RS2(~0),	"d,[1]", 0, 0, v6 }, /* std d,[rs1+%g0] */
{ "std",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1),		"d,[1+i]", 0, 0, v6 },
{ "std",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1),		"d,[i+1]", 0, 0, v6 },
{ "std",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1)|RS1_G0,	"d,[i]", 0, 0, v6 },
{ "std",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1)|SIMM13(~0),	"d,[1]", 0, 0, v6 }, /* std d,[rs1+0] */

{ "std",	F3(3, 0x26, 0), F3(~3, ~0x26, ~0)|ASI(~0),	"q,[1+2]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x26, 0), F3(~3, ~0x26, ~0)|ASI_RS2(~0),	"q,[1]", 0, 0, v6notv9 }, /* std d,[rs1+%g0] */
{ "std",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1),		"q,[1+i]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1),		"q,[i+1]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1)|RS1_G0,	"q,[i]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1)|SIMM13(~0),	"q,[1]", 0, 0, v6notv9 }, /* std d,[rs1+0] */
{ "std",	F3(3, 0x27, 0), F3(~3, ~0x27, ~0)|ASI(~0),	"H,[1+2]", 0, 0, v6 },
{ "std",	F3(3, 0x27, 0), F3(~3, ~0x27, ~0)|ASI_RS2(~0),	"H,[1]", 0, 0, v6 }, /* std d,[rs1+%g0] */
{ "std",	F3(3, 0x27, 1), F3(~3, ~0x27, ~1),		"H,[1+i]", 0, 0, v6 },
{ "std",	F3(3, 0x27, 1), F3(~3, ~0x27, ~1),		"H,[i+1]", 0, 0, v6 },
{ "std",	F3(3, 0x27, 1), F3(~3, ~0x27, ~1)|RS1_G0,	"H,[i]", 0, 0, v6 },
{ "std",	F3(3, 0x27, 1), F3(~3, ~0x27, ~1)|SIMM13(~0),	"H,[1]", 0, 0, v6 }, /* std d,[rs1+0] */

{ "std",	F3(3, 0x36, 0), F3(~3, ~0x36, ~0)|ASI(~0),	"Q,[1+2]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x36, 0), F3(~3, ~0x36, ~0)|ASI_RS2(~0),	"Q,[1]", 0, 0, v6notv9 }, /* std d,[rs1+%g0] */
{ "std",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1),		"Q,[1+i]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1),		"Q,[i+1]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1)|RS1_G0,	"Q,[i]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1)|SIMM13(~0),	"Q,[1]", 0, 0, v6notv9 }, /* std d,[rs1+0] */
{ "std",	F3(3, 0x37, 0), F3(~3, ~0x37, ~0)|ASI(~0),	"D,[1+2]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x37, 0), F3(~3, ~0x37, ~0)|ASI_RS2(~0),	"D,[1]", 0, 0, v6notv9 }, /* std d,[rs1+%g0] */
{ "std",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1),		"D,[1+i]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1),		"D,[i+1]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1)|RS1_G0,	"D,[i]", 0, 0, v6notv9 },
{ "std",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1)|SIMM13(~0),	"D,[1]", 0, 0, v6notv9 }, /* std d,[rs1+0] */

{ "spilld",	F3(3, 0x07, 0), F3(~3, ~0x07, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v6 },
{ "spilld",	F3(3, 0x07, 0), F3(~3, ~0x07, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* std d,[rs1+%g0] */
{ "spilld",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1),		"d,[1+i]", F_ALIAS, 0, v6 },
{ "spilld",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1),		"d,[i+1]", F_ALIAS, 0, v6 },
{ "spilld",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v6 },
{ "spilld",	F3(3, 0x07, 1), F3(~3, ~0x07, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* std d,[rs1+0] */

{ "stda",	F3(3, 0x17, 0), F3(~3, ~0x17, ~0),		"d,[1+2]A", 0, 0, v6 },
{ "stda",	F3(3, 0x17, 0), F3(~3, ~0x17, ~0)|RS2(~0),	"d,[1]A", 0, 0, v6 }, /* stda d,[rs1+%g0] */
{ "stda",	F3(3, 0x17, 1), F3(~3, ~0x17, ~1),		"d,[1+i]o", 0, 0, v9 },
{ "stda",	F3(3, 0x17, 1), F3(~3, ~0x17, ~1),		"d,[i+1]o", 0, 0, v9 },
{ "stda",	F3(3, 0x17, 1), F3(~3, ~0x17, ~1)|RS1_G0,	"d,[i]o", 0, 0, v9 },
{ "stda",	F3(3, 0x17, 1), F3(~3, ~0x17, ~1)|SIMM13(~0),	"d,[1]o", 0, 0, v9 }, /* std d,[rs1+0] */
{ "stda",	F3(3, 0x37, 0), F3(~3, ~0x37, ~0),		"H,[1+2]A", 0, 0, v9 },
{ "stda",	F3(3, 0x37, 0), F3(~3, ~0x37, ~0)|RS2(~0),	"H,[1]A", 0, 0, v9 }, /* stda d,[rs1+%g0] */
{ "stda",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1),		"H,[1+i]o", 0, 0, v9 },
{ "stda",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1),		"H,[i+1]o", 0, 0, v9 },
{ "stda",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1)|RS1_G0,	"H,[i]o", 0, 0, v9 },
{ "stda",	F3(3, 0x37, 1), F3(~3, ~0x37, ~1)|SIMM13(~0),	"H,[1]o", 0, 0, v9 }, /* std d,[rs1+0] */

{ "sth",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|ASI(~0),	"d,[1+2]", 0, 0, v6 },
{ "sth",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|ASI_RS2(~0),	"d,[1]", 0, 0, v6 }, /* sth d,[rs1+%g0] */
{ "sth",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1),		"d,[1+i]", 0, 0, v6 },
{ "sth",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1),		"d,[i+1]", 0, 0, v6 },
{ "sth",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|RS1_G0,	"d,[i]", 0, 0, v6 },
{ "sth",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|SIMM13(~0),	"d,[1]", 0, 0, v6 }, /* sth d,[rs1+0] */

{ "stsh",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v6 },
{ "stsh",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* sth d,[rs1+%g0] */
{ "stsh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1),		"d,[1+i]", F_ALIAS, 0, v6 },
{ "stsh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1),		"d,[i+1]", F_ALIAS, 0, v6 },
{ "stsh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v6 },
{ "stsh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* sth d,[rs1+0] */
{ "stuh",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|ASI(~0),	"d,[1+2]", F_ALIAS, 0, v6 },
{ "stuh",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|ASI_RS2(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* sth d,[rs1+%g0] */
{ "stuh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1),		"d,[1+i]", F_ALIAS, 0, v6 },
{ "stuh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1),		"d,[i+1]", F_ALIAS, 0, v6 },
{ "stuh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|RS1_G0,	"d,[i]", F_ALIAS, 0, v6 },
{ "stuh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|SIMM13(~0),	"d,[1]", F_ALIAS, 0, v6 }, /* sth d,[rs1+0] */

{ "stha",	F3(3, 0x16, 0), F3(~3, ~0x16, ~0),		"d,[1+2]A", 0, 0, v6 },
{ "stha",	F3(3, 0x16, 0), F3(~3, ~0x16, ~0)|RS2(~0),	"d,[1]A", 0, 0, v6 }, /* stha ,[rs1+%g0] */
{ "stha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1),		"d,[1+i]o", 0, 0, v9 },
{ "stha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1),		"d,[i+1]o", 0, 0, v9 },
{ "stha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1)|RS1_G0,	"d,[i]o", 0, 0, v9 },
{ "stha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1)|SIMM13(~0),	"d,[1]o", 0, 0, v9 }, /* sth d,[rs1+0] */

{ "stsha",	F3(3, 0x16, 0), F3(~3, ~0x16, ~0),		"d,[1+2]A", F_ALIAS, 0, v6 },
{ "stsha",	F3(3, 0x16, 0), F3(~3, ~0x16, ~0)|RS2(~0),	"d,[1]A", F_ALIAS, 0, v6 }, /* stha ,[rs1+%g0] */
{ "stsha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1),		"d,[1+i]o", F_ALIAS, 0, v9 },
{ "stsha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1),		"d,[i+1]o", F_ALIAS, 0, v9 },
{ "stsha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1)|RS1_G0,	"d,[i]o", F_ALIAS, 0, v9 },
{ "stsha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1)|SIMM13(~0),	"d,[1]o", F_ALIAS, 0, v9 }, /* sth d,[rs1+0] */
{ "stuha",	F3(3, 0x16, 0), F3(~3, ~0x16, ~0),		"d,[1+2]A", F_ALIAS, 0, v6 },
{ "stuha",	F3(3, 0x16, 0), F3(~3, ~0x16, ~0)|RS2(~0),	"d,[1]A", F_ALIAS, 0, v6 }, /* stha ,[rs1+%g0] */
{ "stuha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1),		"d,[1+i]o", F_ALIAS, 0, v9 },
{ "stuha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1),		"d,[i+1]o", F_ALIAS, 0, v9 },
{ "stuha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1)|RS1_G0,	"d,[i]o", F_ALIAS, 0, v9 },
{ "stuha",	F3(3, 0x16, 1), F3(~3, ~0x16, ~1)|SIMM13(~0),	"d,[1]o", F_ALIAS, 0, v9 }, /* sth d,[rs1+0] */

{ "stx",	F3(3, 0x0e, 0), F3(~3, ~0x0e, ~0)|ASI(~0),	"d,[1+2]", 0, 0, v9 },
{ "stx",	F3(3, 0x0e, 0), F3(~3, ~0x0e, ~0)|ASI_RS2(~0),	"d,[1]", 0, 0, v9 }, /* stx d,[rs1+%g0] */
{ "stx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1),		"d,[1+i]", 0, 0, v9 },
{ "stx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1),		"d,[i+1]", 0, 0, v9 },
{ "stx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1)|RS1_G0,	"d,[i]", 0, 0, v9 },
{ "stx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1)|SIMM13(~0),	"d,[1]", 0, 0, v9 }, /* stx d,[rs1+0] */

{ "stx",	F3(3, 0x25, 0)|RD(1), F3(~3, ~0x25, ~0)|ASI(~0)|RD(~1),	"F,[1+2]", 0, 0, v9 },
{ "stx",	F3(3, 0x25, 0)|RD(1), F3(~3, ~0x25, ~0)|ASI_RS2(~0)|RD(~1),"F,[1]", 0, 0, v9 }, /* stx d,[rs1+%g0] */
{ "stx",	F3(3, 0x25, 1)|RD(1), F3(~3, ~0x25, ~1)|RD(~1),		"F,[1+i]", 0, 0, v9 },
{ "stx",	F3(3, 0x25, 1)|RD(1), F3(~3, ~0x25, ~1)|RD(~1),		"F,[i+1]", 0, 0, v9 },
{ "stx",	F3(3, 0x25, 1)|RD(1), F3(~3, ~0x25, ~1)|RS1_G0|RD(~1),	"F,[i]", 0, 0, v9 },
{ "stx",	F3(3, 0x25, 1)|RD(1), F3(~3, ~0x25, ~1)|SIMM13(~0)|RD(~1),"F,[1]", 0, 0, v9 }, /* stx d,[rs1+0] */

{ "stxa",	F3(3, 0x1e, 0), F3(~3, ~0x1e, ~0),		"d,[1+2]A", 0, 0, v9 },
{ "stxa",	F3(3, 0x1e, 0), F3(~3, ~0x1e, ~0)|RS2(~0),	"d,[1]A", 0, 0, v9 }, /* stxa d,[rs1+%g0] */
{ "stxa",	F3(3, 0x1e, 1), F3(~3, ~0x1e, ~1),		"d,[1+i]o", 0, 0, v9 },
{ "stxa",	F3(3, 0x1e, 1), F3(~3, ~0x1e, ~1),		"d,[i+1]o", 0, 0, v9 },
{ "stxa",	F3(3, 0x1e, 1), F3(~3, ~0x1e, ~1)|RS1_G0,	"d,[i]o", 0, 0, v9 },
{ "stxa",	F3(3, 0x1e, 1), F3(~3, ~0x1e, ~1)|SIMM13(~0),	"d,[1]o", 0, 0, v9 }, /* stx d,[rs1+0] */

{ "stq",	F3(3, 0x26, 0), F3(~3, ~0x26, ~0)|ASI(~0),	"J,[1+2]", 0, 0, v9 },
{ "stq",	F3(3, 0x26, 0), F3(~3, ~0x26, ~0)|ASI_RS2(~0),	"J,[1]", 0, 0, v9 }, /* stq [rs1+%g0] */
{ "stq",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1),		"J,[1+i]", 0, 0, v9 },
{ "stq",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1),		"J,[i+1]", 0, 0, v9 },
{ "stq",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1)|RS1_G0,	"J,[i]", 0, 0, v9 },
{ "stq",	F3(3, 0x26, 1), F3(~3, ~0x26, ~1)|SIMM13(~0),	"J,[1]", 0, 0, v9 }, /* stq [rs1+0] */

{ "stqa",	F3(3, 0x36, 0), F3(~3, ~0x36, ~0)|ASI(~0),	"J,[1+2]A", 0, 0, v9 },
{ "stqa",	F3(3, 0x36, 0), F3(~3, ~0x36, ~0)|ASI_RS2(~0),	"J,[1]A", 0, 0, v9 }, /* stqa [rs1+%g0] */
{ "stqa",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1),		"J,[1+i]o", 0, 0, v9 },
{ "stqa",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1),		"J,[i+1]o", 0, 0, v9 },
{ "stqa",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1)|RS1_G0,	"J,[i]o", 0, 0, v9 },
{ "stqa",	F3(3, 0x36, 1), F3(~3, ~0x36, ~1)|SIMM13(~0),	"J,[1]o", 0, 0, v9 }, /* stqa [rs1+0] */

{ "swap",	F3(3, 0x0f, 0), F3(~3, ~0x0f, ~0)|ASI(~0),	"[1+2],d", 0, 0, v7 },
{ "swap",	F3(3, 0x0f, 0), F3(~3, ~0x0f, ~0)|ASI_RS2(~0),	"[1],d", 0, 0, v7 }, /* swap [rs1+%g0],d */
{ "swap",	F3(3, 0x0f, 1), F3(~3, ~0x0f, ~1),		"[1+i],d", 0, 0, v7 },
{ "swap",	F3(3, 0x0f, 1), F3(~3, ~0x0f, ~1),		"[i+1],d", 0, 0, v7 },
{ "swap",	F3(3, 0x0f, 1), F3(~3, ~0x0f, ~1)|RS1_G0,	"[i],d", 0, 0, v7 },
{ "swap",	F3(3, 0x0f, 1), F3(~3, ~0x0f, ~1)|SIMM13(~0),	"[1],d", 0, 0, v7 }, /* swap [rs1+0],d */

{ "swapa",	F3(3, 0x1f, 0), F3(~3, ~0x1f, ~0),		"[1+2]A,d", 0, 0, v7 },
{ "swapa",	F3(3, 0x1f, 0), F3(~3, ~0x1f, ~0)|RS2(~0),	"[1]A,d", 0, 0, v7 }, /* swapa [rs1+%g0],d */
{ "swapa",	F3(3, 0x1f, 1), F3(~3, ~0x1f, ~1),		"[1+i]o,d", 0, 0, v9 },
{ "swapa",	F3(3, 0x1f, 1), F3(~3, ~0x1f, ~1),		"[i+1]o,d", 0, 0, v9 },
{ "swapa",	F3(3, 0x1f, 1), F3(~3, ~0x1f, ~1)|RS1_G0,	"[i]o,d", 0, 0, v9 },
{ "swapa",	F3(3, 0x1f, 1), F3(~3, ~0x1f, ~1)|SIMM13(~0),	"[1]o,d", 0, 0, v9 }, /* swap [rs1+0],d */

{ "restore",	F3(2, 0x3d, 0), F3(~2, ~0x3d, ~0)|ASI(~0),			"1,2,d", 0, 0, v6 },
{ "restore",	F3(2, 0x3d, 0), F3(~2, ~0x3d, ~0)|RD_G0|RS1_G0|ASI_RS2(~0),	"", 0, 0, v6 }, /* restore %g0,%g0,%g0 */
{ "restore",	F3(2, 0x3d, 1), F3(~2, ~0x3d, ~1),				"1,i,d", 0, 0, v6 },
{ "restore",	F3(2, 0x3d, 1), F3(~2, ~0x3d, ~1)|RD_G0|RS1_G0|SIMM13(~0),	"", 0, 0, v6 }, /* restore %g0,0,%g0 */

{ "rett",	F3(2, 0x39, 0), F3(~2, ~0x39, ~0)|RD_G0|ASI(~0),	"1+2", F_UNBR|F_DELAYED, 0, v6 }, /* rett rs1+rs2 */
{ "rett",	F3(2, 0x39, 0), F3(~2, ~0x39, ~0)|RD_G0|ASI_RS2(~0),	"1", F_UNBR|F_DELAYED, 0, v6 },	/* rett rs1,%g0 */
{ "rett",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1)|RD_G0,		"1+i", F_UNBR|F_DELAYED, 0, v6 }, /* rett rs1+X */
{ "rett",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1)|RD_G0,		"i+1", F_UNBR|F_DELAYED, 0, v6 }, /* rett X+rs1 */
{ "rett",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1)|RD_G0|RS1_G0,		"i", F_UNBR|F_DELAYED, 0, v6 }, /* rett X+rs1 */
{ "rett",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1)|RD_G0|RS1_G0,		"i", F_UNBR|F_DELAYED, 0, v6 },	/* rett X */
{ "rett",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1)|RD_G0|SIMM13(~0),	"1", F_UNBR|F_DELAYED, 0, v6 },	/* rett rs1+0 */

{ "save",	F3(2, 0x3c, 0), F3(~2, ~0x3c, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "save",	F3(2, 0x3c, 1), F3(~2, ~0x3c, ~1),		"1,i,d", 0, 0, v6 },
{ "save",	F3(2, 0x3c, 1), F3(~2, ~0x3c, ~1),		"i,1,d", 0, 0, v6 }, /* Sun assembler compatibility */
{ "save",	0x81e00000,	~0x81e00000,			"", F_ALIAS, 0, v6 },

{ "ret",  F3(2, 0x38, 1)|RS1(0x1f)|SIMM13(8), F3(~2, ~0x38, ~1)|SIMM13(~8),	       "", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl %i7+8,%g0 */
{ "retl", F3(2, 0x38, 1)|RS1(0x0f)|SIMM13(8), F3(~2, ~0x38, ~1)|RS1(~0x0f)|SIMM13(~8), "", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl %o7+8,%g0 */

{ "jmpl",	F3(2, 0x38, 0), F3(~2, ~0x38, ~0)|ASI(~0),	"1+2,d", F_JSR|F_DELAYED, 0, v6 },
{ "jmpl",	F3(2, 0x38, 0), F3(~2, ~0x38, ~0)|ASI_RS2(~0),	"1,d", F_JSR|F_DELAYED, 0, v6 }, /* jmpl rs1+%g0,d */
{ "jmpl",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1)|SIMM13(~0),	"1,d", F_JSR|F_DELAYED, 0, v6 }, /* jmpl rs1+0,d */
{ "jmpl",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1)|RS1_G0,	"i,d", F_JSR|F_DELAYED, 0, v6 }, /* jmpl %g0+i,d */
{ "jmpl",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1),		"1+i,d", F_JSR|F_DELAYED, 0, v6 },
{ "jmpl",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1),		"i+1,d", F_JSR|F_DELAYED, 0, v6 },

{ "done",	F3(2, 0x3e, 0)|RD(0), F3(~2, ~0x3e, ~0)|RD(~0)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "retry",	F3(2, 0x3e, 0)|RD(1), F3(~2, ~0x3e, ~0)|RD(~1)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "saved",	F3(2, 0x31, 0)|RD(0), F3(~2, ~0x31, ~0)|RD(~0)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "restored",	F3(2, 0x31, 0)|RD(1), F3(~2, ~0x31, ~0)|RD(~1)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "allclean",	F3(2, 0x31, 0)|RD(2), F3(~2, ~0x31, ~0)|RD(~2)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "otherw",	F3(2, 0x31, 0)|RD(3), F3(~2, ~0x31, ~0)|RD(~3)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "normalw",	F3(2, 0x31, 0)|RD(4), F3(~2, ~0x31, ~0)|RD(~4)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "invalw",	F3(2, 0x31, 0)|RD(5), F3(~2, ~0x31, ~0)|RD(~5)|RS1_G0|SIMM13(~0),	"", 0, 0, v9 },
{ "sir",	F3(2, 0x30, 1)|RD(0xf), F3(~2, ~0x30, ~1)|RD(~0xf)|RS1_G0,		"i", 0, 0, v9 },

{ "flush",	F3(2, 0x3b, 0), F3(~2, ~0x3b, ~0)|ASI(~0),	"1+2", 0, 0, v8 },
{ "flush",	F3(2, 0x3b, 0), F3(~2, ~0x3b, ~0)|ASI_RS2(~0),	"1", 0, 0, v8 }, /* flush rs1+%g0 */
{ "flush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1)|SIMM13(~0),	"1", 0, 0, v8 }, /* flush rs1+0 */
{ "flush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1)|RS1_G0,	"i", 0, 0, v8 }, /* flush %g0+i */
{ "flush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1),		"1+i", 0, 0, v8 },
{ "flush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1),		"i+1", 0, 0, v8 },

/* IFLUSH was renamed to FLUSH in v8.  */
{ "iflush",	F3(2, 0x3b, 0), F3(~2, ~0x3b, ~0)|ASI(~0),	"1+2", F_ALIAS, 0, v6 },
{ "iflush",	F3(2, 0x3b, 0), F3(~2, ~0x3b, ~0)|ASI_RS2(~0),	"1", F_ALIAS, 0, v6 }, /* flush rs1+%g0 */
{ "iflush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1)|SIMM13(~0),	"1", F_ALIAS, 0, v6 }, /* flush rs1+0 */
{ "iflush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1)|RS1_G0,	"i", F_ALIAS, 0, v6 },
{ "iflush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1),		"1+i", F_ALIAS, 0, v6 },
{ "iflush",	F3(2, 0x3b, 1), F3(~2, ~0x3b, ~1),		"i+1", F_ALIAS, 0, v6 },

{ "return",	F3(2, 0x39, 0), F3(~2, ~0x39, ~0)|ASI(~0),	"1+2", 0, 0, v9 },
{ "return",	F3(2, 0x39, 0), F3(~2, ~0x39, ~0)|ASI_RS2(~0),	"1", 0, 0, v9 }, /* return rs1+%g0 */
{ "return",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1)|SIMM13(~0),	"1", 0, 0, v9 }, /* return rs1+0 */
{ "return",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1)|RS1_G0,	"i", 0, 0, v9 }, /* return %g0+i */
{ "return",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1),		"1+i", 0, 0, v9 },
{ "return",	F3(2, 0x39, 1), F3(~2, ~0x39, ~1),		"i+1", 0, 0, v9 },

{ "flushw",	F3(2, 0x2b, 0), F3(~2, ~0x2b, ~0)|RD_G0|RS1_G0|ASI_RS2(~0),	"", 0, 0, v9 },

{ "membar",	F3(2, 0x28, 1)|RS1(0xf), F3(~2, ~0x28, ~1)|RD_G0|RS1(~0xf)|SIMM13(~127), "K", 0, 0, v9 },
{ "stbar",	F3(2, 0x28, 0)|RS1(0xf), F3(~2, ~0x28, ~0)|RD_G0|RS1(~0xf)|SIMM13(~0), "", 0, 0, v8 },

{ "prefetch",	F3(3, 0x2d, 0), F3(~3, ~0x2d, ~0),		"[1+2],*", 0, 0, v9 },
{ "prefetch",	F3(3, 0x2d, 0), F3(~3, ~0x2d, ~0)|RS2_G0,	"[1],*", 0, 0, v9 }, /* prefetch [rs1+%g0],prefetch_fcn */
{ "prefetch",	F3(3, 0x2d, 1), F3(~3, ~0x2d, ~1),		"[1+i],*", 0, 0, v9 },
{ "prefetch",	F3(3, 0x2d, 1), F3(~3, ~0x2d, ~1),		"[i+1],*", 0, 0, v9 },
{ "prefetch",	F3(3, 0x2d, 1), F3(~3, ~0x2d, ~1)|RS1_G0,	"[i],*", 0, 0, v9 },
{ "prefetch",	F3(3, 0x2d, 1), F3(~3, ~0x2d, ~1)|SIMM13(~0),	"[1],*", 0, 0, v9 }, /* prefetch [rs1+0],prefetch_fcn */
{ "prefetcha",	F3(3, 0x3d, 0), F3(~3, ~0x3d, ~0),		"[1+2]A,*", 0, 0, v9 },
{ "prefetcha",	F3(3, 0x3d, 0), F3(~3, ~0x3d, ~0)|RS2_G0,	"[1]A,*", 0, 0, v9 }, /* prefetcha [rs1+%g0],prefetch_fcn */
{ "prefetcha",	F3(3, 0x3d, 1), F3(~3, ~0x3d, ~1),		"[1+i]o,*", 0, 0, v9 },
{ "prefetcha",	F3(3, 0x3d, 1), F3(~3, ~0x3d, ~1),		"[i+1]o,*", 0, 0, v9 },
{ "prefetcha",	F3(3, 0x3d, 1), F3(~3, ~0x3d, ~1)|RS1_G0,	"[i]o,*", 0, 0, v9 },
{ "prefetcha",	F3(3, 0x3d, 1), F3(~3, ~0x3d, ~1)|SIMM13(~0),	"[1]o,*", 0, 0, v9 }, /* prefetcha [rs1+0],d */

{ "sll",	F3(2, 0x25, 0),      F3(~2, ~0x25, ~0)|(1<<12)|(0x7f<<5),	"1,2,d", 0, 0, v6 },
{ "sll",	F3(2, 0x25, 1),      F3(~2, ~0x25, ~1)|(1<<12)|(0x7f<<5),	"1,X,d", 0, 0, v6 },
{ "slld",	F5(2, 0x25, 1, 0x2), F5(~2, ~0x25, ~1, ~0x2),	                "1,Y,d", 0, 0, v6 }, /* AJIT */
{ "slld",	F5(2, 0x25, 0, 0x2), F5(~2, ~0x25, ~0, ~0x2),	                "1,2,d", 0, 0, v6 }, /* AJIT */
{ "sra",	F3(2, 0x27, 0),      F3(~2, ~0x27, ~0)|(1<<12)|(0x7f<<5),	"1,2,d", 0, 0, v6 },
{ "sra",	F3(2, 0x27, 1),      F3(~2, ~0x27, ~1)|(1<<12)|(0x7f<<5),	"1,X,d", 0, 0, v6 },
{ "srad",	F5(2, 0x27, 1, 0x2), F5(~2, ~0x27, ~1, ~0x2),	                "1,Y,d", 0, 0, v6 }, /* AJIT */
{ "srad",	F5(2, 0x27, 0, 0x2), F5(~2, ~0x27, ~0, ~0x2),	                "1,2,d", 0, 0, v6 }, /* AJIT */
{ "srl",	F3(2, 0x26, 0),      F3(~2, ~0x26, ~0)|(1<<12)|(0x7f<<5),	"1,2,d", 0, 0, v6 },
{ "srl",	F3(2, 0x26, 1),      F3(~2, ~0x26, ~1)|(1<<12)|(0x7f<<5),	"1,X,d", 0, 0, v6 },
{ "srld",	F5(2, 0x26, 1, 0x2), F5(~2, ~0x26, ~1, ~0x2),	                "1,Y,d", 0, 0, v6 }, /* AJIT */
{ "srld",	F5(2, 0x26, 0, 0x2), F5(~2, ~0x26, ~0, ~0x2),	                "1,2,d", 0, 0, v6 }, /* AJIT */

{ "sllx",	F3(2, 0x25, 0)|(1<<12), F3(~2, ~0x25, ~0)|(0x7f<<5),	"1,2,d", 0, 0, v9 },
{ "sllx",	F3(2, 0x25, 1)|(1<<12), F3(~2, ~0x25, ~1)|(0x3f<<6),	"1,Y,d", 0, 0, v9 },
{ "srax",	F3(2, 0x27, 0)|(1<<12), F3(~2, ~0x27, ~0)|(0x7f<<5),	"1,2,d", 0, 0, v9 },
{ "srax",	F3(2, 0x27, 1)|(1<<12), F3(~2, ~0x27, ~1)|(0x3f<<6),	"1,Y,d", 0, 0, v9 },
{ "srlx",	F3(2, 0x26, 0)|(1<<12), F3(~2, ~0x26, ~0)|(0x7f<<5),	"1,2,d", 0, 0, v9 },
{ "srlx",	F3(2, 0x26, 1)|(1<<12), F3(~2, ~0x26, ~1)|(0x3f<<6),	"1,Y,d", 0, 0, v9 },

{ "mulscc",	F3(2, 0x24, 0), F3(~2, ~0x24, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "mulscc",	F3(2, 0x24, 1), F3(~2, ~0x24, ~1),		"1,i,d", 0, 0, v6 },

{ "divscc",	F3(2, 0x1d, 0), F3(~2, ~0x1d, ~0)|ASI(~0),	"1,2,d", 0, 0, sparclite },
{ "divscc",	F3(2, 0x1d, 1), F3(~2, ~0x1d, ~1),		"1,i,d", 0, 0, sparclite },

{ "scan",	F3(2, 0x2c, 0), F3(~2, ~0x2c, ~0)|ASI(~0),	"1,2,d", 0, 0, sparclet|sparclite },
{ "scan",	F3(2, 0x2c, 1), F3(~2, ~0x2c, ~1),		"1,i,d", 0, 0, sparclet|sparclite },

{ "popc",	F3(2, 0x2e, 0), F3(~2, ~0x2e, ~0)|RS1_G0|ASI(~0),"2,d", 0, HWCAP_POPC, v9 },
{ "popc",	F3(2, 0x2e, 1), F3(~2, ~0x2e, ~1)|RS1_G0,	"i,d", 0, HWCAP_POPC, v9 },

{ "clr",	F3(2, 0x02, 0), F3(~2, ~0x02, ~0)|RD_G0|RS1_G0|ASI_RS2(~0),	"d", F_ALIAS, 0, v6 }, /* or %g0,%g0,d */
{ "clr",	F3(2, 0x02, 1), F3(~2, ~0x02, ~1)|RS1_G0|SIMM13(~0),		"d", F_ALIAS, 0, v6 }, /* or %g0,0,d	*/
{ "clr",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|RD_G0|ASI(~0),		"[1+2]", F_ALIAS, 0, v6 },
{ "clr",	F3(3, 0x04, 0), F3(~3, ~0x04, ~0)|RD_G0|ASI_RS2(~0),		"[1]", F_ALIAS, 0, v6 }, /* st %g0,[rs1+%g0] */
{ "clr",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RD_G0,			"[1+i]", F_ALIAS, 0, v6 },
{ "clr",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RD_G0,			"[i+1]", F_ALIAS, 0, v6 },
{ "clr",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RD_G0|RS1_G0,			"[i]", F_ALIAS, 0, v6 },
{ "clr",	F3(3, 0x04, 1), F3(~3, ~0x04, ~1)|RD_G0|SIMM13(~0),		"[1]", F_ALIAS, 0, v6 }, /* st %g0,[rs1+0] */

{ "clrb",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|RD_G0|ASI(~0),	"[1+2]", F_ALIAS, 0, v6 },
{ "clrb",	F3(3, 0x05, 0), F3(~3, ~0x05, ~0)|RD_G0|ASI_RS2(~0),	"[1]", F_ALIAS, 0, v6 }, /* stb %g0,[rs1+%g0] */
{ "clrb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|RD_G0,		"[1+i]", F_ALIAS, 0, v6 },
{ "clrb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|RD_G0,		"[i+1]", F_ALIAS, 0, v6 },
{ "clrb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|RD_G0|RS1_G0,		"[i]", F_ALIAS, 0, v6 },
{ "clrb",	F3(3, 0x05, 1), F3(~3, ~0x05, ~1)|RD_G0|SIMM13(~0),	"[1]", F_ALIAS, 0, v6 }, /* stb %g0,[rs1+0] */

{ "clrh",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|RD_G0|ASI(~0),	"[1+2]", F_ALIAS, 0, v6 },
{ "clrh",	F3(3, 0x06, 0), F3(~3, ~0x06, ~0)|RD_G0|ASI_RS2(~0),	"[1]", F_ALIAS, 0, v6 }, /* sth %g0,[rs1+%g0] */
{ "clrh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|RD_G0,		"[1+i]", F_ALIAS, 0, v6 },
{ "clrh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|RD_G0,		"[i+1]", F_ALIAS, 0, v6 },
{ "clrh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|RD_G0|RS1_G0,		"[i]", F_ALIAS, 0, v6 },
{ "clrh",	F3(3, 0x06, 1), F3(~3, ~0x06, ~1)|RD_G0|SIMM13(~0),	"[1]", F_ALIAS, 0, v6 }, /* sth %g0,[rs1+0] */

{ "clrx",	F3(3, 0x0e, 0), F3(~3, ~0x0e, ~0)|RD_G0|ASI(~0),	"[1+2]", F_ALIAS, 0, v9 },
{ "clrx",	F3(3, 0x0e, 0), F3(~3, ~0x0e, ~0)|RD_G0|ASI_RS2(~0),	"[1]", F_ALIAS, 0, v9 }, /* stx %g0,[rs1+%g0] */
{ "clrx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1)|RD_G0,		"[1+i]", F_ALIAS, 0, v9 },
{ "clrx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1)|RD_G0,		"[i+1]", F_ALIAS, 0, v9 },
{ "clrx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1)|RD_G0|RS1_G0,		"[i]", F_ALIAS, 0, v9 },
{ "clrx",	F3(3, 0x0e, 1), F3(~3, ~0x0e, ~1)|RD_G0|SIMM13(~0),	"[1]", F_ALIAS, 0, v9 }, /* stx %g0,[rs1+0] */

{ "orcc",	F3(2, 0x12, 0), F3(~2, ~0x12, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "orcc",	F3(2, 0x12, 1), F3(~2, ~0x12, ~1),		"1,i,d", 0, 0, v6 },
{ "orcc",	F3(2, 0x12, 1), F3(~2, ~0x12, ~1),		"i,1,d", 0, 0, v6 },

/* This is not a commutative instruction.  */
{ "orncc",	F3(2, 0x16, 0), F3(~2, ~0x16, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "orncc",	F3(2, 0x16, 1), F3(~2, ~0x16, ~1),		"1,i,d", 0, 0, v6 },

/* This is not a commutative instruction.  */
{ "orn",	F3(2, 0x06, 0), F3(~2, ~0x06, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "orn",	F3(2, 0x06, 1), F3(~2, ~0x06, ~1),		"1,i,d", 0, 0, v6 },

{ "tst",	F3(2, 0x12, 0), F3(~2, ~0x12, ~0)|RD_G0|ASI_RS2(~0),	"1", 0, 0, v6 }, /* orcc rs1, %g0, %g0 */
{ "tst",	F3(2, 0x12, 0), F3(~2, ~0x12, ~0)|RD_G0|RS1_G0|ASI(~0),	"2", 0, 0, v6 }, /* orcc %g0, rs2, %g0 */
{ "tst",	F3(2, 0x12, 1), F3(~2, ~0x12, ~1)|RD_G0|SIMM13(~0),	"1", 0, 0, v6 }, /* orcc rs1, 0, %g0 */

{ "wr",	F3(2, 0x30, 0),		F3(~2, ~0x30, ~0)|ASI(~0),		"1,2,m", 0, 0, v8 }, /* wr r,r,%asrX */
{ "wr",	F3(2, 0x30, 1),		F3(~2, ~0x30, ~1),			"1,i,m", 0, 0, v8 }, /* wr r,i,%asrX */
{ "wr",	F3(2, 0x30, 0),		F3(~2, ~0x30, ~0)|RS1_G0|ASI(~0),	"2,m", F_ALIAS, 0, v8 }, /* wr %g0,rs2,%asrX */
{ "wr",	F3(2, 0x30, 1),		F3(~2, ~0x30, ~1)|RS1_G0,		"i,m", F_ALIAS, 0, v8 }, /* wr %g0,i,%asrX */
{ "wr",	F3(2, 0x30, 0),		F3(~2, ~0x30, ~0)|ASI_RS2(~0),		"1,m", F_ALIAS, 0, v8 }, /* wr rs1,%g0,%asrX */
{ "wr",	F3(2, 0x30, 0),		F3(~2, ~0x30, ~0)|RD_G0|ASI(~0),	"1,2,y", 0, 0, v6 }, /* wr r,r,%y */
{ "wr",	F3(2, 0x30, 1),		F3(~2, ~0x30, ~1)|RD_G0,		"1,i,y", 0, 0, v6 }, /* wr r,i,%y */
{ "wr",	F3(2, 0x30, 0),		F3(~2, ~0x30, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,y", F_ALIAS, 0, v6 }, /* wr %g0,rs2,%y */
{ "wr",	F3(2, 0x30, 1),		F3(~2, ~0x30, ~1)|RD_G0|RS1_G0,		"i,y", F_ALIAS, 0, v6 }, /* wr %g0,i,%y */
{ "wr",	F3(2, 0x30, 0),		F3(~2, ~0x30, ~0)|RD_G0|ASI_RS2(~0),	"1,y", F_ALIAS, 0, v6 }, /* wr rs1,%g0,%y */
{ "wr",	F3(2, 0x31, 0),		F3(~2, ~0x31, ~0)|RD_G0|ASI(~0),	"1,2,p", 0, 0, v6notv9 }, /* wr r,r,%psr */
{ "wr",	F3(2, 0x31, 1),		F3(~2, ~0x31, ~1)|RD_G0,		"1,i,p", 0, 0, v6notv9 }, /* wr r,i,%psr */
{ "wr",	F3(2, 0x31, 0),		F3(~2, ~0x31, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,p", F_ALIAS, 0, v6notv9 }, /* wr %g0,rs2,%psr */
{ "wr",	F3(2, 0x31, 1),		F3(~2, ~0x31, ~1)|RD_G0|RS1_G0,		"i,p", F_ALIAS, 0, v6notv9 }, /* wr %g0,i,%psr */
{ "wr",	F3(2, 0x31, 0),		F3(~2, ~0x31, ~0)|RD_G0|ASI_RS2(~0),	"1,p", F_ALIAS, 0, v6notv9 }, /* wr rs1,%g0,%psr */
{ "wr",	F3(2, 0x32, 0),		F3(~2, ~0x32, ~0)|RD_G0|ASI(~0),	"1,2,w", 0, 0, v6notv9 }, /* wr r,r,%wim */
{ "wr",	F3(2, 0x32, 1),		F3(~2, ~0x32, ~1)|RD_G0,		"1,i,w", 0, 0, v6notv9 }, /* wr r,i,%wim */
{ "wr",	F3(2, 0x32, 0),		F3(~2, ~0x32, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,w", F_ALIAS, 0, v6notv9 }, /* wr %g0,rs2,%wim */
{ "wr",	F3(2, 0x32, 1),		F3(~2, ~0x32, ~1)|RD_G0|RS1_G0,		"i,w", F_ALIAS, 0, v6notv9 }, /* wr %g0,i,%wim */
{ "wr",	F3(2, 0x32, 0),		F3(~2, ~0x32, ~0)|RD_G0|ASI_RS2(~0),	"1,w", F_ALIAS, 0, v6notv9 }, /* wr rs1,%g0,%wim */
{ "wr",	F3(2, 0x33, 0),		F3(~2, ~0x33, ~0)|RD_G0|ASI(~0),	"1,2,t", 0, 0, v6notv9 }, /* wr r,r,%tbr */
{ "wr",	F3(2, 0x33, 1),		F3(~2, ~0x33, ~1)|RD_G0,		"1,i,t", 0, 0, v6notv9 }, /* wr r,i,%tbr */
{ "wr",	F3(2, 0x33, 0),		F3(~2, ~0x33, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,t", F_ALIAS, 0, v6notv9 }, /* wr %g0,rs2,%tbr */
{ "wr",	F3(2, 0x33, 1),		F3(~2, ~0x33, ~1)|RD_G0|RS1_G0,		"i,t", F_ALIAS, 0, v6notv9 }, /* wr %g0,i,%tbr */
{ "wr",	F3(2, 0x33, 0),		F3(~2, ~0x33, ~0)|RD_G0|ASI_RS2(~0),	"1,t", F_ALIAS, 0, v6notv9 }, /* wr rs1,%g0,%tbr */

{ "wr", F3(2, 0x30, 0)|RD(2),	F3(~2, ~0x30, ~0)|RD(~2)|ASI(~0),	"1,2,E", 0, 0, v9 }, /* wr r,r,%ccr */
{ "wr", F3(2, 0x30, 1)|RD(2),	F3(~2, ~0x30, ~1)|RD(~2),		"1,i,E", 0, 0, v9 }, /* wr r,i,%ccr */
{ "wr", F3(2, 0x30, 0)|RD(3),	F3(~2, ~0x30, ~0)|RD(~3)|ASI(~0),	"1,2,o", 0, 0, v9 }, /* wr r,r,%asi */
{ "wr", F3(2, 0x30, 1)|RD(3),	F3(~2, ~0x30, ~1)|RD(~3),		"1,i,o", 0, 0, v9 }, /* wr r,i,%asi */
{ "wr", F3(2, 0x30, 0)|RD(6),	F3(~2, ~0x30, ~0)|RD(~6)|ASI(~0),	"1,2,s", 0, 0, v9 }, /* wr r,r,%fprs */
{ "wr", F3(2, 0x30, 1)|RD(6),	F3(~2, ~0x30, ~1)|RD(~6),		"1,i,s", 0, 0, v9 }, /* wr r,i,%fprs */

{ "wr", F3(2, 0x30, 0)|RD(16),	F3(~2, ~0x30, ~0)|RD(~16)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%pcr */
{ "wr", F3(2, 0x30, 1)|RD(16),	F3(~2, ~0x30, ~1)|RD(~16),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%pcr */
{ "wr", F3(2, 0x30, 0)|RD(17),	F3(~2, ~0x30, ~0)|RD(~17)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%pic */
{ "wr", F3(2, 0x30, 1)|RD(17),	F3(~2, ~0x30, ~1)|RD(~17),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%pic */
{ "wr", F3(2, 0x30, 0)|RD(18),	F3(~2, ~0x30, ~0)|RD(~18)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%dcr */
{ "wr", F3(2, 0x30, 1)|RD(18),	F3(~2, ~0x30, ~1)|RD(~18),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%dcr */
{ "wr", F3(2, 0x30, 0)|RD(19),	F3(~2, ~0x30, ~0)|RD(~19)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%gsr */
{ "wr", F3(2, 0x30, 1)|RD(19),	F3(~2, ~0x30, ~1)|RD(~19),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%gsr */
{ "wr", F3(2, 0x30, 0)|RD(20),	F3(~2, ~0x30, ~0)|RD(~20)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%set_softint */
{ "wr", F3(2, 0x30, 1)|RD(20),	F3(~2, ~0x30, ~1)|RD(~20),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%set_softint */
{ "wr", F3(2, 0x30, 0)|RD(21),	F3(~2, ~0x30, ~0)|RD(~21)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%clear_softint */
{ "wr", F3(2, 0x30, 1)|RD(21),	F3(~2, ~0x30, ~1)|RD(~21),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%clear_softint */
{ "wr", F3(2, 0x30, 0)|RD(22),	F3(~2, ~0x30, ~0)|RD(~22)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%softint */
{ "wr", F3(2, 0x30, 1)|RD(22),	F3(~2, ~0x30, ~1)|RD(~22),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%softint */
{ "wr", F3(2, 0x30, 0)|RD(23),	F3(~2, ~0x30, ~0)|RD(~23)|ASI(~0),	"1,2,_", 0, HWCAP_VIS, v9a }, /* wr r,r,%tick_cmpr */
{ "wr", F3(2, 0x30, 1)|RD(23),	F3(~2, ~0x30, ~1)|RD(~23),		"1,i,_", 0, HWCAP_VIS, v9a }, /* wr r,i,%tick_cmpr */
{ "wr", F3(2, 0x30, 0)|RD(24),	F3(~2, ~0x30, ~0)|RD(~24)|ASI(~0),	"1,2,_", HWCAP_VIS2, 0, v9b }, /* wr r,r,%sys_tick */
{ "wr", F3(2, 0x30, 1)|RD(24),	F3(~2, ~0x30, ~1)|RD(~24),		"1,i,_", HWCAP_VIS2, 0, v9b }, /* wr r,i,%sys_tick */
{ "wr", F3(2, 0x30, 0)|RD(25),	F3(~2, ~0x30, ~0)|RD(~25)|ASI(~0),	"1,2,_", HWCAP_VIS2, 0, v9b }, /* wr r,r,%sys_tick_cmpr */
{ "wr", F3(2, 0x30, 1)|RD(25),	F3(~2, ~0x30, ~1)|RD(~25),		"1,i,_", HWCAP_VIS2, 0, v9b }, /* wr r,i,%sys_tick_cmpr */
{ "wr", F3(2, 0x30, 0)|RD(26),	F3(~2, ~0x30, ~0)|RD(~26)|ASI(~0),	"1,2,_", 0, HWCAP_CBCOND, v9b }, /* wr r,r,%cfr */
{ "wr", F3(2, 0x30, 1)|RD(26),	F3(~2, ~0x30, ~1)|RD(~26),		"1,i,_", 0, HWCAP_CBCOND, v9b }, /* wr r,i,%cfr */
{ "wr", F3(2, 0x30, 0)|RD(27),	F3(~2, ~0x30, ~0)|RD(~27)|ASI(~0),	"1,2,_", 0, HWCAP_PAUSE, v9b }, /* wr r,r,%pause */
{ "wr", F3(2, 0x30, 1)|RD(27),	F3(~2, ~0x30, ~1)|RD(~27),		"1,i,_", 0, HWCAP_PAUSE, v9b }, /* wr r,i,%pause */
{ "wr", F3(2, 0x30, 0)|RD(28),	F3(~2, ~0x30, ~0)|RD(~28)|ASI(~0),	"1,2,_", 0, HWCAP_VIS3, v9b }, /* wr r,r,%cps */
{ "wr", F3(2, 0x30, 1)|RD(28),	F3(~2, ~0x30, ~1)|RD(~28),		"1,i,_", 0, HWCAP_VIS3, v9b }, /* wr r,i,%cps */

{ "pause", F3(2, 0x30, 1)|RD(27)|RS1(0), F3(~2, ~0x30, ~1)|RD(~27)|RS1(~0), "i", 0, HWCAP_PAUSE, v9b }, /* wr %g0,i,%pause */

{ "rd",	F3(2, 0x28, 0),			F3(~2, ~0x28, ~0)|SIMM13(~0),		"M,d", 0, 0, v8 }, /* rd %asrX,r */
{ "rd",	F3(2, 0x28, 0),			F3(~2, ~0x28, ~0)|RS1_G0|SIMM13(~0),	"y,d", 0, 0, v6 }, /* rd %y,r */
{ "rd",	F3(2, 0x29, 0),			F3(~2, ~0x29, ~0)|RS1_G0|SIMM13(~0),	"p,d", 0, 0, v6notv9 }, /* rd %psr,r */
{ "rd",	F3(2, 0x2a, 0),			F3(~2, ~0x2a, ~0)|RS1_G0|SIMM13(~0),	"w,d", 0, 0, v6notv9 }, /* rd %wim,r */
{ "rd",	F3(2, 0x2b, 0),			F3(~2, ~0x2b, ~0)|RS1_G0|SIMM13(~0),	"t,d", 0, 0, v6notv9 }, /* rd %tbr,r */

{ "rd",	F3(2, 0x28, 0)|RS1(2),		F3(~2, ~0x28, ~0)|RS1(~2)|SIMM13(~0),	"E,d", 0, 0, v9 }, /* rd %ccr,r */
{ "rd",	F3(2, 0x28, 0)|RS1(3),		F3(~2, ~0x28, ~0)|RS1(~3)|SIMM13(~0),	"o,d", 0, 0, v9 }, /* rd %asi,r */
{ "rd",	F3(2, 0x28, 0)|RS1(4),		F3(~2, ~0x28, ~0)|RS1(~4)|SIMM13(~0),	"W,d", 0, 0, v9 }, /* rd %tick,r */
{ "rd",	F3(2, 0x28, 0)|RS1(5),		F3(~2, ~0x28, ~0)|RS1(~5)|SIMM13(~0),	"P,d", 0, 0, v9 }, /* rd %pc,r */
{ "rd",	F3(2, 0x28, 0)|RS1(6),		F3(~2, ~0x28, ~0)|RS1(~6)|SIMM13(~0),	"s,d", 0, 0, v9 }, /* rd %fprs,r */

{ "rd",	F3(2, 0x28, 0)|RS1(16),		F3(~2, ~0x28, ~0)|RS1(~16)|SIMM13(~0),	"/,d", 0, HWCAP_VIS, v9a }, /* rd %pcr,r */
{ "rd",	F3(2, 0x28, 0)|RS1(17),		F3(~2, ~0x28, ~0)|RS1(~17)|SIMM13(~0),	"/,d", 0, HWCAP_VIS, v9a }, /* rd %pic,r */
{ "rd",	F3(2, 0x28, 0)|RS1(18),		F3(~2, ~0x28, ~0)|RS1(~18)|SIMM13(~0),	"/,d", 0, HWCAP_VIS, v9a }, /* rd %dcr,r */
{ "rd",	F3(2, 0x28, 0)|RS1(19),		F3(~2, ~0x28, ~0)|RS1(~19)|SIMM13(~0),	"/,d", 0, HWCAP_VIS, v9a }, /* rd %gsr,r */
{ "rd",	F3(2, 0x28, 0)|RS1(22),		F3(~2, ~0x28, ~0)|RS1(~22)|SIMM13(~0),	"/,d", 0, HWCAP_VIS, v9a }, /* rd %softint,r */
{ "rd",	F3(2, 0x28, 0)|RS1(23),		F3(~2, ~0x28, ~0)|RS1(~23)|SIMM13(~0),	"/,d", 0, HWCAP_VIS, v9a }, /* rd %tick_cmpr,r */
{ "rd",	F3(2, 0x28, 0)|RS1(24),		F3(~2, ~0x28, ~0)|RS1(~24)|SIMM13(~0),	"/,d", HWCAP_VIS2, 0, v9b }, /* rd %sys_tick,r */
{ "rd",	F3(2, 0x28, 0)|RS1(25),		F3(~2, ~0x28, ~0)|RS1(~25)|SIMM13(~0),	"/,d", HWCAP_VIS2, 0, v9b }, /* rd %sys_tick_cmpr,r */
{ "rd",	F3(2, 0x28, 0)|RS1(26),		F3(~2, ~0x28, ~0)|RS1(~26)|SIMM13(~0),	"/,d", HWCAP_CBCOND, 0, v9b }, /* rd %cfr,r */
{ "rd",	F3(2, 0x28, 0)|RS1(28),		F3(~2, ~0x28, ~0)|RS1(~28)|SIMM13(~0),	"/,d", 0, HWCAP_VIS3, v9b }, /* rd %cps,r */

{ "rdpr",	F3(2, 0x2a, 0),		F3(~2, ~0x2a, ~0)|SIMM13(~0),	"?,d", 0, 0, v9 },   /* rdpr %priv,r */
{ "wrpr",	F3(2, 0x32, 0),		F3(~2, ~0x32, ~0),		"1,2,!", 0, 0, v9 }, /* wrpr r1,r2,%priv */
{ "wrpr",	F3(2, 0x32, 0),		F3(~2, ~0x32, ~0)|SIMM13(~0),	"1,!", 0, 0, v9 },   /* wrpr r1,%priv */
{ "wrpr",	F3(2, 0x32, 1),		F3(~2, ~0x32, ~1),		"1,i,!", 0, 0, v9 }, /* wrpr r1,i,%priv */
{ "wrpr",	F3(2, 0x32, 1),		F3(~2, ~0x32, ~1),		"i,1,!", F_ALIAS, 0, v9 }, /* wrpr i,r1,%priv */
{ "wrpr",	F3(2, 0x32, 1),		F3(~2, ~0x32, ~1)|RS1(~0),	"i,!", 0, 0, v9 },   /* wrpr i,%priv */

{ "rdhpr",	F3(2, 0x29, 0),		F3(~2, ~0x29, ~0)|SIMM13(~0),	"$,d", 0, 0, v9 },   /* rdhpr %hpriv,r */
{ "wrhpr",	F3(2, 0x33, 0),		F3(~2, ~0x33, ~0),		"1,2,%", 0, 0, v9 }, /* wrhpr r1,r2,%hpriv */
{ "wrhpr",	F3(2, 0x33, 0),		F3(~2, ~0x33, ~0)|SIMM13(~0),	"1,%", 0, 0, v9 },   /* wrhpr r1,%hpriv */
{ "wrhpr",	F3(2, 0x33, 1),		F3(~2, ~0x33, ~1),		"1,i,%", 0, 0, v9 }, /* wrhpr r1,i,%hpriv */
{ "wrhpr",	F3(2, 0x33, 1),		F3(~2, ~0x33, ~1),		"i,1,%", F_ALIAS, 0, v9 }, /* wrhpr i,r1,%hpriv */
{ "wrhpr",	F3(2, 0x33, 1),		F3(~2, ~0x33, ~1)|RS1(~0),	"i,%", 0, 0, v9 },   /* wrhpr i,%hpriv */

/* ??? This group seems wrong.  A three operand move?  */
{ "mov",	F3(2, 0x30, 0), F3(~2, ~0x30, ~0)|ASI(~0),		"1,2,m", F_ALIAS, 0, v8 }, /* wr r,r,%asrX */
{ "mov",	F3(2, 0x30, 1), F3(~2, ~0x30, ~1),			"1,i,m", F_ALIAS, 0, v8 }, /* wr r,i,%asrX */
{ "mov",	F3(2, 0x30, 0), F3(~2, ~0x30, ~0)|RD_G0|ASI(~0),	"1,2,y", F_ALIAS, 0, v6 }, /* wr r,r,%y */
{ "mov",	F3(2, 0x30, 1), F3(~2, ~0x30, ~1)|RD_G0,		"1,i,y", F_ALIAS, 0, v6 }, /* wr r,i,%y */
{ "mov",	F3(2, 0x31, 0), F3(~2, ~0x31, ~0)|RD_G0|ASI(~0),	"1,2,p", F_ALIAS, 0, v6notv9 }, /* wr r,r,%psr */
{ "mov",	F3(2, 0x31, 1), F3(~2, ~0x31, ~1)|RD_G0,		"1,i,p", F_ALIAS, 0, v6notv9 }, /* wr r,i,%psr */
{ "mov",	F3(2, 0x32, 0), F3(~2, ~0x32, ~0)|RD_G0|ASI(~0),	"1,2,w", F_ALIAS, 0, v6notv9 }, /* wr r,r,%wim */
{ "mov",	F3(2, 0x32, 1), F3(~2, ~0x32, ~1)|RD_G0,		"1,i,w", F_ALIAS, 0, v6notv9 }, /* wr r,i,%wim */
{ "mov",	F3(2, 0x33, 0), F3(~2, ~0x33, ~0)|RD_G0|ASI(~0),	"1,2,t", F_ALIAS, 0, v6notv9 }, /* wr r,r,%tbr */
{ "mov",	F3(2, 0x33, 1), F3(~2, ~0x33, ~1)|RD_G0,		"1,i,t", F_ALIAS, 0, v6notv9 }, /* wr r,i,%tbr */

{ "mov",	F3(2, 0x28, 0), F3(~2, ~0x28, ~0)|SIMM13(~0),		"M,d", F_ALIAS, 0, v8 }, /* rd %asr1,r */
{ "mov",	F3(2, 0x28, 0), F3(~2, ~0x28, ~0)|RS1_G0|SIMM13(~0),	"y,d", F_ALIAS, 0, v6 }, /* rd %y,r */
{ "mov",	F3(2, 0x29, 0), F3(~2, ~0x29, ~0)|RS1_G0|SIMM13(~0),	"p,d", F_ALIAS, 0, v6notv9 }, /* rd %psr,r */
{ "mov",	F3(2, 0x2a, 0), F3(~2, ~0x2a, ~0)|RS1_G0|SIMM13(~0),	"w,d", F_ALIAS, 0, v6notv9 }, /* rd %wim,r */
{ "mov",	F3(2, 0x2b, 0), F3(~2, ~0x2b, ~0)|RS1_G0|SIMM13(~0),	"t,d", F_ALIAS, 0, v6notv9 }, /* rd %tbr,r */

{ "mov",	F3(2, 0x30, 0), F3(~2, ~0x30, ~0)|RS1_G0|ASI(~0),	"2,m", F_ALIAS, 0, v8 }, /* wr %g0,rs2,%asrX */
{ "mov",	F3(2, 0x30, 1), F3(~2, ~0x30, ~1)|RS1_G0,		"i,m", F_ALIAS, 0, v8 }, /* wr %g0,i,%asrX */
{ "mov",	F3(2, 0x30, 0), F3(~2, ~0x30, ~0)|ASI_RS2(~0),		"1,m", F_ALIAS, 0, v8 }, /* wr rs1,%g0,%asrX */
{ "mov",	F3(2, 0x30, 1), F3(~2, ~0x30, ~1)|SIMM13(~0),		"1,m", F_ALIAS, 0, v8 }, /* wr rs1,0,%asrX */
{ "mov",	F3(2, 0x30, 0), F3(~2, ~0x30, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,y", F_ALIAS, 0, v6 }, /* wr %g0,rs2,%y */
{ "mov",	F3(2, 0x30, 1), F3(~2, ~0x30, ~1)|RD_G0|RS1_G0,		"i,y", F_ALIAS, 0, v6 }, /* wr %g0,i,%y */
{ "mov",	F3(2, 0x30, 0), F3(~2, ~0x30, ~0)|RD_G0|ASI_RS2(~0),	"1,y", F_ALIAS, 0, v6 }, /* wr rs1,%g0,%y */
{ "mov",	F3(2, 0x30, 1), F3(~2, ~0x30, ~1)|RD_G0|SIMM13(~0),	"1,y", F_ALIAS, 0, v6 }, /* wr rs1,0,%y */
{ "mov",	F3(2, 0x31, 0), F3(~2, ~0x31, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,p", F_ALIAS, 0, v6notv9 }, /* wr %g0,rs2,%psr */
{ "mov",	F3(2, 0x31, 1), F3(~2, ~0x31, ~1)|RD_G0|RS1_G0,		"i,p", F_ALIAS, 0, v6notv9 }, /* wr %g0,i,%psr */
{ "mov",	F3(2, 0x31, 0), F3(~2, ~0x31, ~0)|RD_G0|ASI_RS2(~0),	"1,p", F_ALIAS, 0, v6notv9 }, /* wr rs1,%g0,%psr */
{ "mov",	F3(2, 0x31, 1), F3(~2, ~0x31, ~1)|RD_G0|SIMM13(~0),	"1,p", F_ALIAS, 0, v6notv9 }, /* wr rs1,0,%psr */
{ "mov",	F3(2, 0x32, 0), F3(~2, ~0x32, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,w", F_ALIAS, 0, v6notv9 }, /* wr %g0,rs2,%wim */
{ "mov",	F3(2, 0x32, 1), F3(~2, ~0x32, ~1)|RD_G0|RS1_G0,		"i,w", F_ALIAS, 0, v6notv9 }, /* wr %g0,i,%wim */
{ "mov",	F3(2, 0x32, 0), F3(~2, ~0x32, ~0)|RD_G0|ASI_RS2(~0),	"1,w", F_ALIAS, 0, v6notv9 }, /* wr rs1,%g0,%wim */
{ "mov",	F3(2, 0x32, 1), F3(~2, ~0x32, ~1)|RD_G0|SIMM13(~0),	"1,w", F_ALIAS, 0, v6notv9 }, /* wr rs1,0,%wim */
{ "mov",	F3(2, 0x33, 0), F3(~2, ~0x33, ~0)|RD_G0|RS1_G0|ASI(~0),	"2,t", F_ALIAS, 0, v6notv9 }, /* wr %g0,rs2,%tbr */
{ "mov",	F3(2, 0x33, 1), F3(~2, ~0x33, ~1)|RD_G0|RS1_G0,		"i,t", F_ALIAS, 0, v6notv9 }, /* wr %g0,i,%tbr */
{ "mov",	F3(2, 0x33, 0), F3(~2, ~0x33, ~0)|RD_G0|ASI_RS2(~0),	"1,t", F_ALIAS, 0, v6notv9 }, /* wr rs1,%g0,%tbr */
{ "mov",	F3(2, 0x33, 1), F3(~2, ~0x33, ~1)|RD_G0|SIMM13(~0),	"1,t", F_ALIAS, 0, v6notv9 }, /* wr rs1,0,%tbr */

{ "mov",	F3(2, 0x02, 0), F3(~2, ~0x02, ~0)|RS1_G0|ASI(~0),	"2,d", 0, 0, v6 }, /* or %g0,rs2,d */
{ "mov",	F3(2, 0x02, 1), F3(~2, ~0x02, ~1)|RS1_G0,		"i,d", 0, 0, v6 }, /* or %g0,i,d	*/
{ "mov",        F3(2, 0x02, 0), F3(~2, ~0x02, ~0)|ASI_RS2(~0),		"1,d", 0, 0, v6 }, /* or rs1,%g0,d   */
{ "mov",        F3(2, 0x02, 1), F3(~2, ~0x02, ~1)|SIMM13(~0),		"1,d", 0, 0, v6 }, /* or rs1,0,d */

{ "or",	F3(2, 0x02, 0), F3(~2, ~0x02, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "or",	F3(2, 0x02, 1), F3(~2, ~0x02, ~1),		"1,i,d", 0, 0, v6 },
{ "or",	F3(2, 0x02, 1), F3(~2, ~0x02, ~1),		"i,1,d", 0, 0, v6 },

{ "ord",	F4(2, 0x02, 0, 1), F4(~2, ~0x02, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */
{ "ordcc",	F4(2, 0x12, 0, 1), F4(~2, ~0x12, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */
{ "ordn",	F4(2, 0x06, 0, 1), F4(~2, ~0x06, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */
{ "ordncc",	F4(2, 0x16, 0, 1), F4(~2, ~0x16, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */

{ "bset",	F3(2, 0x02, 0), F3(~2, ~0x02, ~0)|ASI(~0),	"2,r", F_ALIAS, 0, v6 },	/* or rd,rs2,rd */
{ "bset",	F3(2, 0x02, 1), F3(~2, ~0x02, ~1),		"i,r", F_ALIAS, 0, v6 },	/* or rd,i,rd */

/* This is not a commutative instruction.  */
{ "andn",	F3(2, 0x05, 0), F3(~2, ~0x05, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "andn",	F3(2, 0x05, 1), F3(~2, ~0x05, ~1),		"1,i,d", 0, 0, v6 },
{ "anddn",	F4(2, 0x05, 0, 1), F4(~2, ~0x05, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */

/* This is not a commutative instruction.  */
{ "andncc",	F3(2, 0x15, 0), F3(~2, ~0x15, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "andncc",	F3(2, 0x15, 1), F3(~2, ~0x15, ~1),		"1,i,d", 0, 0, v6 },
{ "anddncc",	F4(2, 0x15, 0, 1), F4(~2, ~0x15, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */

{ "bclr",	F3(2, 0x05, 0), F3(~2, ~0x05, ~0)|ASI(~0),	"2,r", F_ALIAS, 0, v6 },	/* andn rd,rs2,rd */
{ "bclr",	F3(2, 0x05, 1), F3(~2, ~0x05, ~1),		"i,r", F_ALIAS, 0, v6 },	/* andn rd,i,rd */

{ "cmp",	F3(2, 0x14, 0), F3(~2, ~0x14, ~0)|RD_G0|ASI(~0),	"1,2", 0, 0, v6 },	/* subcc rs1,rs2,%g0 */
{ "cmp",	F3(2, 0x14, 1), F3(~2, ~0x14, ~1)|RD_G0,		"1,i", 0, 0, v6 },	/* subcc rs1,i,%g0 */

{ "sub",	F3(2, 0x04, 0), F3(~2, ~0x04, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "sub",	F3(2, 0x04, 1), F3(~2, ~0x04, ~1),		"1,i,d", 0, 0, v6 },
{ "subd",	F4(2, 0x04, 0, 1), F4(~2, ~0x04, ~0, ~1),	"1,2,d", 0, 0, v8 }, /* AJIT */

{ "subcc",	F3(2, 0x14, 0), F3(~2, ~0x14, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "subcc",	F3(2, 0x14, 1), F3(~2, ~0x14, ~1),		"1,i,d", 0, 0, v6 },
{ "subdcc",	F4(2, 0x14, 0, 1), F4(~2, ~0x14, ~0, ~1),	"1,2,d", 0, 0, v8 }, /* AJIT */

{ "subx",	F3(2, 0x0c, 0), F3(~2, ~0x0c, ~0)|ASI(~0),	"1,2,d", 0, 0, v6notv9 },
{ "subx",	F3(2, 0x0c, 1), F3(~2, ~0x0c, ~1),		"1,i,d", 0, 0, v6notv9 },
{ "subc",	F3(2, 0x0c, 0), F3(~2, ~0x0c, ~0)|ASI(~0),	"1,2,d", 0, 0, v9 },
{ "subc",	F3(2, 0x0c, 1), F3(~2, ~0x0c, ~1),		"1,i,d", 0, 0, v9 },

{ "vsubd8",	F6(2, 0x04, 0, 2, 1), F6(~2, ~0x04, ~0, ~2, ~1),	"1,2,d", 0, 0, v8 }, /* AJIT */
{ "vsubd16",	F6(2, 0x04, 0, 2, 2), F6(~2, ~0x04, ~0, ~2, ~3),	"1,2,d", 0, 0, v8 }, /* AJIT */
{ "vsubd32",	F6(2, 0x04, 0, 2, 4), F6(~2, ~0x04, ~0, ~2, ~4),	"1,2,d", 0, 0, v8 }, /* AJIT */

{ "subxcc",	F3(2, 0x1c, 0), F3(~2, ~0x1c, ~0)|ASI(~0),	"1,2,d", 0, 0, v6notv9 },
{ "subxcc",	F3(2, 0x1c, 1), F3(~2, ~0x1c, ~1),		"1,i,d", 0, 0, v6notv9 },
{ "subccc",	F3(2, 0x1c, 0), F3(~2, ~0x1c, ~0)|ASI(~0),	"1,2,d", 0, 0, v9 },
{ "subccc",	F3(2, 0x1c, 1), F3(~2, ~0x1c, ~1),		"1,i,d", 0, 0, v9 },

{ "and",	F3(2, 0x01, 0), F3(~2, ~0x01, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "and",	F3(2, 0x01, 1), F3(~2, ~0x01, ~1),		"1,i,d", 0, 0, v6 },
{ "and",	F3(2, 0x01, 1), F3(~2, ~0x01, ~1),		"i,1,d", 0, 0, v6 },
{ "andd",	F4(2, 0x01, 0, 1), F4(~2, ~0x01, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */

{ "andcc",	F3(2, 0x11, 0), F3(~2, ~0x11, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "andcc",	F3(2, 0x11, 1), F3(~2, ~0x11, ~1),		"1,i,d", 0, 0, v6 },
{ "andcc",	F3(2, 0x11, 1), F3(~2, ~0x11, ~1),		"i,1,d", 0, 0, v6 },
{ "anddcc",	F4(2, 0x11, 0, 1), F4(~2, ~0x11, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */

{ "dec",	F3(2, 0x04, 1)|SIMM13(0x1), F3(~2, ~0x04, ~1)|SIMM13(~0x0001), "r", F_ALIAS, 0, v6 },	/* sub rd,1,rd */
{ "dec",	F3(2, 0x04, 1),		    F3(~2, ~0x04, ~1),		       "i,r", F_ALIAS, 0, v8 },	/* sub rd,imm,rd */
{ "deccc",	F3(2, 0x14, 1)|SIMM13(0x1), F3(~2, ~0x14, ~1)|SIMM13(~0x0001), "r", F_ALIAS, 0, v6 },	/* subcc rd,1,rd */
{ "deccc",	F3(2, 0x14, 1),		    F3(~2, ~0x14, ~1),		       "i,r", F_ALIAS, 0, v8 },	/* subcc rd,imm,rd */
{ "inc",	F3(2, 0x00, 1)|SIMM13(0x1), F3(~2, ~0x00, ~1)|SIMM13(~0x0001), "r", F_ALIAS, 0, v6 },	/* add rd,1,rd */
{ "inc",	F3(2, 0x00, 1),		    F3(~2, ~0x00, ~1),		       "i,r", F_ALIAS, 0, v8 },	/* add rd,imm,rd */
{ "inccc",	F3(2, 0x10, 1)|SIMM13(0x1), F3(~2, ~0x10, ~1)|SIMM13(~0x0001), "r", F_ALIAS, 0, v6 },	/* addcc rd,1,rd */
{ "inccc",	F3(2, 0x10, 1),		    F3(~2, ~0x10, ~1),		       "i,r", F_ALIAS, 0, v8 },	/* addcc rd,imm,rd */

{ "btst",	F3(2, 0x11, 0), F3(~2, ~0x11, ~0)|RD_G0|ASI(~0), "1,2", F_ALIAS, 0, v6 },	/* andcc rs1,rs2,%g0 */
{ "btst",	F3(2, 0x11, 1), F3(~2, ~0x11, ~1)|RD_G0, "i,1", F_ALIAS, 0, v6 },	/* andcc rs1,i,%g0 */

{ "neg",	F3(2, 0x04, 0), F3(~2, ~0x04, ~0)|RS1_G0|ASI(~0), "2,d", F_ALIAS, 0, v6 }, /* sub %g0,rs2,rd */
{ "neg",	F3(2, 0x04, 0), F3(~2, ~0x04, ~0)|RS1_G0|ASI(~0), "O", F_ALIAS, 0, v6 }, /* sub %g0,rd,rd */

{ "add",	F3(2, 0x00, 0), F3(~2, ~0x00, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "add",	F3(2, 0x00, 1), F3(~2, ~0x00, ~1),		"1,i,d", 0, 0, v6 },
{ "add",	F3(2, 0x00, 1), F3(~2, ~0x00, ~1),		"i,1,d", 0, 0, v6 },
{ "addd",	F4(2, 0x00, 0, 1), F4(~2, ~0x00, ~0, ~1),	"1,2,d", 0, 0, v8 }, /* AJIT */
{ "addcc",	F3(2, 0x10, 0), F3(~2, ~0x10, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "addcc",	F3(2, 0x10, 1), F3(~2, ~0x10, ~1),		"1,i,d", 0, 0, v6 },
{ "addcc",	F3(2, 0x10, 1), F3(~2, ~0x10, ~1),		"i,1,d", 0, 0, v6 },
{ "adddcc",	F4(2, 0x10, 0,1), F4(~2, ~0x10, ~0, ~1),	"1,2,d", 0, 0, v8 }, /* AJIT */

{ "addx",	F3(2, 0x08, 0), F3(~2, ~0x08, ~0)|ASI(~0),	"1,2,d", 0, 0, v6notv9 },
{ "addx",	F3(2, 0x08, 1), F3(~2, ~0x08, ~1),		"1,i,d", 0, 0, v6notv9 },
{ "addx",	F3(2, 0x08, 1), F3(~2, ~0x08, ~1),		"i,1,d", 0, 0, v6notv9 },
{ "addc",	F3(2, 0x08, 0), F3(~2, ~0x08, ~0)|ASI(~0),	"1,2,d", 0, 0, v9 },
{ "addc",	F3(2, 0x08, 1), F3(~2, ~0x08, ~1),		"1,i,d", 0, 0, v9 },
{ "addc",	F3(2, 0x08, 1), F3(~2, ~0x08, ~1),		"i,1,d", 0, 0, v9 },

{ "addxcc",	F3(2, 0x18, 0), F3(~2, ~0x18, ~0)|ASI(~0),	"1,2,d", 0, 0, v6notv9 },
{ "addxcc",	F3(2, 0x18, 1), F3(~2, ~0x18, ~1),		"1,i,d", 0, 0, v6notv9 },
{ "addxcc",	F3(2, 0x18, 1), F3(~2, ~0x18, ~1),		"i,1,d", 0, 0, v6notv9 },
{ "addccc",	F3(2, 0x18, 0), F3(~2, ~0x18, ~0)|ASI(~0),	"1,2,d", 0, 0, v9 },
{ "addccc",	F3(2, 0x18, 1), F3(~2, ~0x18, ~1),		"1,i,d", 0, 0, v9 },
{ "addccc",	F3(2, 0x18, 1), F3(~2, ~0x18, ~1),		"i,1,d", 0, 0, v9 },

{ "vaddd8",	F6(2, 0x00, 0, 2, 1), F6(~2, ~0x00, ~0, ~2, ~1),	"1,2,d", 0, 0, v8 }, /* AJIT */
{ "vaddd16",	F6(2, 0x00, 0, 2, 2), F6(~2, ~0x00, ~0, ~2, ~2),	"1,2,d", 0, 0, v8 }, /* AJIT */
{ "vaddd32",	F6(2, 0x00, 0, 2, 4), F6(~2, ~0x00, ~0, ~2, ~4),	"1,2,d", 0, 0, v8 }, /* AJIT */

{ "smul",	F3(2, 0x0b, 0), F3(~2, ~0x0b, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_MUL32, v8 },
{ "smul",	F3(2, 0x0b, 1), F3(~2, ~0x0b, ~1),		"1,i,d", 0, HWCAP_MUL32, v8 },
{ "smul",	F3(2, 0x0b, 1), F3(~2, ~0x0b, ~1),		"i,1,d", 0, HWCAP_MUL32, v8 },
{ "smuld",	F4(2, 0x0b, 0, 1), F4(~2, ~0x0b, ~0, ~1),	"1,2,d", 0, HWCAP_MUL32, v8 }, /* AJIT */
{ "smulcc",	F3(2, 0x1b, 0), F3(~2, ~0x1b, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_MUL32, v8 },
{ "smulcc",	F3(2, 0x1b, 1), F3(~2, ~0x1b, ~1),		"1,i,d", 0, HWCAP_MUL32, v8 },
{ "smulcc",	F3(2, 0x1b, 1), F3(~2, ~0x1b, ~1),		"i,1,d", 0, HWCAP_MUL32, v8 },
{ "smuldcc",	F4(2, 0x1b, 0, 1), F4(~2, ~0x1b, ~0, ~1),	"1,2,d", 0, HWCAP_MUL32, v8 }, /* AJIT */
{ "umul",	F3(2, 0x0a, 0), F3(~2, ~0x0a, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_MUL32, v8 },
{ "umul",	F3(2, 0x0a, 1), F3(~2, ~0x0a, ~1),		"1,i,d", 0, HWCAP_MUL32, v8 },
{ "umul",	F3(2, 0x0a, 1), F3(~2, ~0x0a, ~1),		"i,1,d", 0, HWCAP_MUL32, v8 },
{ "umulcc",	F3(2, 0x1a, 0), F3(~2, ~0x1a, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_MUL32, v8 },
{ "umulcc",	F3(2, 0x1a, 1), F3(~2, ~0x1a, ~1),		"1,i,d", 0, HWCAP_MUL32, v8 },
{ "umulcc",	F3(2, 0x1a, 1), F3(~2, ~0x1a, ~1),		"i,1,d", 0, HWCAP_MUL32, v8 },
{ "umuld",	F4(2, 0x0a, 0, 1), F4(~2, ~0x0a, ~0, ~1),	"1,2,d", 0, HWCAP_MUL32, v8 }, /* AJIT */
{ "umuldcc",	F4(2, 0x1a, 0, 1), F4(~2, ~0x1a, ~0, ~1),	"1,2,d", 0, HWCAP_MUL32, v8 }, /* AJIT */

{ "vumuld8",	F6(2, 0x0a, 0, 2, 1), F6(~2, ~0x0a, ~0, ~2, ~1), "1,2,d", 0, 0, v8 }, /* AJIT */
{ "vumuld16",	F6(2, 0x0a, 0, 2, 2), F6(~2, ~0x0a, ~0, ~2, ~3), "1,2,d", 0, 0, v8 }, /* AJIT */
{ "vumuld32",	F6(2, 0x0a, 0, 2, 4), F6(~2, ~0x0a, ~0, ~2, ~4), "1,2,d", 0, 0, v8 }, /* AJIT */
{ "vsmuld8",	F6(2, 0x0b, 0, 2, 1), F6(~2, ~0x0b, ~0, ~2, ~1), "1,2,d", 0, 0, v8 }, /* AJIT */
{ "vsmuld16",	F6(2, 0x0b, 0, 2, 2), F6(~2, ~0x0b, ~0, ~2, ~3), "1,2,d", 0, 0, v8 }, /* AJIT */
{ "vsmuld32",	F6(2, 0x0b, 0, 2, 4), F6(~2, ~0x0b, ~0, ~2, ~4), "1,2,d", 0, 0, v8 }, /* AJIT */

{ "sdiv",	F3(2, 0x0f, 0), F3(~2, ~0x0f, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_DIV32, v8 },
{ "sdiv",	F3(2, 0x0f, 1), F3(~2, ~0x0f, ~1),		"1,i,d", 0, HWCAP_DIV32, v8 },
{ "sdiv",	F3(2, 0x0f, 1), F3(~2, ~0x0f, ~1),		"i,1,d", 0, HWCAP_DIV32, v8 },
{ "sdivcc",	F3(2, 0x1f, 0), F3(~2, ~0x1f, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_DIV32, v8 },
{ "sdivcc",	F3(2, 0x1f, 1), F3(~2, ~0x1f, ~1),		"1,i,d", 0, HWCAP_DIV32, v8 },
{ "sdivcc",	F3(2, 0x1f, 1), F3(~2, ~0x1f, ~1),		"i,1,d", 0, HWCAP_DIV32, v8 },
{ "sdivd",	F4(2, 0x0f, 0, 1), F4(~2, ~0x0f, ~0, ~1),	"1,2,d", 0, HWCAP_DIV32, v8 }, /* AJIT */
{ "sdivdcc",	F4(2, 0x1f, 0, 1), F4(~2, ~0x1f, ~0, ~1),	"1,2,d", 0, HWCAP_DIV32, v8 }, /* AJIT */
{ "udiv",	F3(2, 0x0e, 0), F3(~2, ~0x0e, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_DIV32, v8 },
{ "udiv",	F3(2, 0x0e, 1), F3(~2, ~0x0e, ~1),		"1,i,d", 0, HWCAP_DIV32, v8 },
{ "udiv",	F3(2, 0x0e, 1), F3(~2, ~0x0e, ~1),		"i,1,d", 0, HWCAP_DIV32, v8 },
{ "udivcc",	F3(2, 0x1e, 0), F3(~2, ~0x1e, ~0)|ASI(~0),	"1,2,d", 0, HWCAP_DIV32, v8 },
{ "udivcc",	F3(2, 0x1e, 1), F3(~2, ~0x1e, ~1),		"1,i,d", 0, HWCAP_DIV32, v8 },
{ "udivcc",	F3(2, 0x1e, 1), F3(~2, ~0x1e, ~1),		"i,1,d", 0, HWCAP_DIV32, v8 },
{ "udivdcc",	F4(2, 0x1e, 0, 1), F4(~2, ~0x1e, ~0, ~1),	"1,2,d", 0, HWCAP_DIV32, v8 }, /* AJIT */
{ "udivd",	F4(2, 0x0e, 0, 1), F4(~2, ~0x0e, ~0, ~1),	"1,2,d", 0, HWCAP_DIV32, v8 }, /* AJIT */

{ "mulx",	F3(2, 0x09, 0), F3(~2, ~0x09, ~0)|ASI(~0),	"1,2,d", 0, 0, v9 },
{ "mulx",	F3(2, 0x09, 1), F3(~2, ~0x09, ~1),		"1,i,d", 0, 0, v9 },
{ "sdivx",	F3(2, 0x2d, 0), F3(~2, ~0x2d, ~0)|ASI(~0),	"1,2,d", 0, 0, v9 },
{ "sdivx",	F3(2, 0x2d, 1), F3(~2, ~0x2d, ~1),		"1,i,d", 0, 0, v9 },
{ "udivx",	F3(2, 0x0d, 0), F3(~2, ~0x0d, ~0)|ASI(~0),	"1,2,d", 0, 0, v9 },
{ "udivx",	F3(2, 0x0d, 1), F3(~2, ~0x0d, ~1),		"1,i,d", 0, 0, v9 },

/* AJIT Additions */
{ "cswap",     F3(3, 0x2f, 0), F3(~3, ~0x2f, ~0), "[1+2]A,d", 0, 0, v8}, /* AJIT */
{ "cswap",     F3(3, 0x2f, 1), F3(~3, ~0x2f, ~1), "[1+i],d",  0, 0, v8}, /* AJIT */
{ "cswapa",    F3(3, 0x3f, 0), F3(~3, ~0x3f, ~0), "[1+2]A,d", 0, 0, v8}, /* AJIT */
{ "cswapa",    F3(3, 0x3f, 1), F3(~3, ~0x3f, ~1), "[1+i],d",  0, 0, v8}, /* AJIT */

{ "call",	F1(0x1), F1(~0x1), "L", F_JSR|F_DELAYED, 0, v6 },
{ "call",	F1(0x1), F1(~0x1), "L,#", F_JSR|F_DELAYED, 0, v6 },

{ "call",	F3(2, 0x38, 0)|RD(0xf), F3(~2, ~0x38, ~0)|RD(~0xf)|ASI(~0),	"1+2", F_JSR|F_DELAYED, 0, v6 }, /* jmpl rs1+rs2,%o7 */
{ "call",	F3(2, 0x38, 0)|RD(0xf), F3(~2, ~0x38, ~0)|RD(~0xf)|ASI(~0),	"1+2,#", F_JSR|F_DELAYED, 0, v6 },
{ "call",	F3(2, 0x38, 0)|RD(0xf), F3(~2, ~0x38, ~0)|RD(~0xf)|ASI_RS2(~0),	"1", F_JSR|F_DELAYED, 0, v6 }, /* jmpl rs1+%g0,%o7 */
{ "call",	F3(2, 0x38, 0)|RD(0xf), F3(~2, ~0x38, ~0)|RD(~0xf)|ASI_RS2(~0),	"1,#", F_JSR|F_DELAYED, 0, v6 },
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf),		"1+i", F_JSR|F_DELAYED, 0, v6 }, /* jmpl rs1+i,%o7 */
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf),		"1+i,#", F_JSR|F_DELAYED, 0, v6 },
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf),		"i+1", F_JSR|F_DELAYED, 0, v6 }, /* jmpl i+rs1,%o7 */
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf),		"i+1,#", F_JSR|F_DELAYED, 0, v6 },
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf)|RS1_G0,	"i", F_JSR|F_DELAYED, 0, v6 }, /* jmpl %g0+i,%o7 */
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf)|RS1_G0,	"i,#", F_JSR|F_DELAYED, 0, v6 },
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf)|SIMM13(~0),	"1", F_JSR|F_DELAYED, 0, v6 }, /* jmpl rs1+0,%o7 */
{ "call",	F3(2, 0x38, 1)|RD(0xf), F3(~2, ~0x38, ~1)|RD(~0xf)|SIMM13(~0),	"1,#", F_JSR|F_DELAYED, 0, v6 },

{ "chkpt",	F2(0, 1)|CONDA|ANNUL|(1<<20), F2(~0, ~1)|((~CONDA)&COND(~0)), "G", 0, HWCAP_TRANS, v9b },

/* Conditional instructions.

   Because this part of the table was such a mess earlier, I have
   macrofied it so that all the branches and traps are generated from
   a single-line description of each condition value.  John Gilmore. */

/* Define branches -- one annulled, one without, etc. */
#define br(opcode, mask, lose, flags) \
 { opcode, (mask)|ANNUL, (lose),       ",a l",   (flags), 0, v6 }, \
 { opcode, (mask)      , (lose)|ANNUL, "l",     (flags), 0, v6 }

#define brx(opcode, mask, lose, flags) /* v9 */ \
 { opcode, (mask)|(2<<20)|BPRED, ANNUL|(lose), "Z,G",      (flags), 0, v9 }, \
 { opcode, (mask)|(2<<20)|BPRED, ANNUL|(lose), ",T Z,G",   (flags), 0, v9 }, \
 { opcode, (mask)|(2<<20)|BPRED|ANNUL, (lose), ",a Z,G",   (flags), 0, v9 }, \
 { opcode, (mask)|(2<<20)|BPRED|ANNUL, (lose), ",a,T Z,G", (flags), 0, v9 }, \
 { opcode, (mask)|(2<<20), ANNUL|BPRED|(lose), ",N Z,G",   (flags), 0, v9 }, \
 { opcode, (mask)|(2<<20)|ANNUL, BPRED|(lose), ",a,N Z,G", (flags), 0, v9 }, \
 { opcode, (mask)|BPRED, ANNUL|(lose)|(2<<20), "z,G",      (flags), 0, v9 }, \
 { opcode, (mask)|BPRED, ANNUL|(lose)|(2<<20), ",T z,G",   (flags), 0, v9 }, \
 { opcode, (mask)|BPRED|ANNUL, (lose)|(2<<20), ",a z,G",   (flags), 0, v9 }, \
 { opcode, (mask)|BPRED|ANNUL, (lose)|(2<<20), ",a,T z,G", (flags), 0, v9 }, \
 { opcode, (mask), ANNUL|BPRED|(lose)|(2<<20), ",N z,G",   (flags), 0, v9 }, \
 { opcode, (mask)|ANNUL, BPRED|(lose)|(2<<20), ",a,N z,G", (flags), 0, v9 }

/* Define four traps: reg+reg, reg + immediate, immediate alone, reg alone. */
#define tr(opcode, mask, lose, flags) \
 { opcode, (mask)|(2<<11)|IMMED, (lose)|RS1_G0,	"Z,i",   (flags), 0, v9 }, /* %g0 + imm */ \
 { opcode, (mask)|(2<<11)|IMMED, (lose),	"Z,1+i", (flags), 0, v9 }, /* rs1 + imm */ \
 { opcode, (mask)|(2<<11), IMMED|(lose),	"Z,1+2", (flags), 0, v9 }, /* rs1 + rs2 */ \
 { opcode, (mask)|(2<<11), IMMED|(lose)|RS2_G0,	"Z,1",   (flags), 0, v9 }, /* rs1 + %g0 */ \
 { opcode, (mask)|IMMED, (lose)|RS1_G0,	"z,i",   (flags)|F_ALIAS, 0, v9 }, /* %g0 + imm */ \
 { opcode, (mask)|IMMED, (lose),	"z,1+i", (flags)|F_ALIAS, 0, v9 }, /* rs1 + imm */ \
 { opcode, (mask), IMMED|(lose),	"z,1+2", (flags)|F_ALIAS, 0, v9 }, /* rs1 + rs2 */ \
 { opcode, (mask), IMMED|(lose)|RS2_G0,	"z,1",   (flags)|F_ALIAS, 0, v9 }, /* rs1 + %g0 */ \
 { opcode, (mask)|IMMED, (lose)|RS1_G0,		"i",     (flags), 0, v6 }, /* %g0 + imm */ \
 { opcode, (mask)|IMMED, (lose),		"1+i",   (flags), 0, v6 }, /* rs1 + imm */ \
 { opcode, (mask)|IMMED, (lose),		"i+1",   (flags), 0, v6 }, /* imm + rs1 */ \
 { opcode, (mask), IMMED|(lose),		"1+2",   (flags), 0, v6 }, /* rs1 + rs2 */ \
 { opcode, (mask), IMMED|(lose)|RS2_G0,		"1",     (flags), 0, v6 } /* rs1 + %g0 */

/* v9: We must put `brx' before `br', to ensure that we never match something
   v9: against an expression unless it is an expression.  Otherwise, we end
   v9: up with undefined symbol tables entries, because they get added, but
   v9: are not deleted if the pattern fails to match.  */

/* Define both branches and traps based on condition mask */
#define cond(bop, top, mask, flags) \
  brx(bop, F2(0, 1)|(mask), F2(~0, ~1)|((~mask)&COND(~0)), F_DELAYED|(flags)), /* v9 */ \
  br(bop,  F2(0, 2)|(mask), F2(~0, ~2)|((~mask)&COND(~0)), F_DELAYED|(flags)), \
  tr(top,  F3(2, 0x3a, 0)|(mask), F3(~2, ~0x3a, 0)|((~mask)&COND(~0)), ((flags) & ~(F_UNBR|F_CONDBR)))

/* Define all the conditions, all the branches, all the traps.  */

/* Standard branch, trap mnemonics */
cond ("b",	"ta",   CONDA, F_UNBR),
/* Alternative form (just for assembly, not for disassembly) */
cond ("ba",	"t",    CONDA, F_UNBR|F_ALIAS),

cond ("bcc",	"tcc",  CONDCC, F_CONDBR),
cond ("bcs",	"tcs",  CONDCS, F_CONDBR),
cond ("be",	"te",   CONDE, F_CONDBR),
cond ("beq",	"teq",  CONDE, F_CONDBR|F_ALIAS),
cond ("bg",	"tg",   CONDG, F_CONDBR),
cond ("bgt",	"tgt",  CONDG, F_CONDBR|F_ALIAS),
cond ("bge",	"tge",  CONDGE, F_CONDBR),
cond ("bgeu",	"tgeu", CONDGEU, F_CONDBR|F_ALIAS), /* for cc */
cond ("bgu",	"tgu",  CONDGU, F_CONDBR),
cond ("bl",	"tl",   CONDL, F_CONDBR),
cond ("blt",	"tlt",  CONDL, F_CONDBR|F_ALIAS),
cond ("ble",	"tle",  CONDLE, F_CONDBR),
cond ("bleu",	"tleu", CONDLEU, F_CONDBR),
cond ("blu",	"tlu",  CONDLU, F_CONDBR|F_ALIAS), /* for cs */
cond ("bn",	"tn",   CONDN, F_CONDBR),
cond ("bne",	"tne",  CONDNE, F_CONDBR),
cond ("bneg",	"tneg", CONDNEG, F_CONDBR),
cond ("bnz",	"tnz",  CONDNZ, F_CONDBR|F_ALIAS), /* for ne */
cond ("bpos",	"tpos", CONDPOS, F_CONDBR),
cond ("bvc",	"tvc",  CONDVC, F_CONDBR),
cond ("bvs",	"tvs",  CONDVS, F_CONDBR),
cond ("bz",	"tz",   CONDZ, F_CONDBR|F_ALIAS), /* for e */

#undef cond
#undef br
#undef brr /* v9 */
#undef tr

#define brr(opcode, mask, lose, flags) /* v9 */ \
 { opcode, (mask)|BPRED, ANNUL|(lose), "1,k",      F_DELAYED|(flags), 0, v9 }, \
 { opcode, (mask)|BPRED, ANNUL|(lose), ",T 1,k",   F_DELAYED|(flags), 0, v9 }, \
 { opcode, (mask)|BPRED|ANNUL, (lose), ",a 1,k",   F_DELAYED|(flags), 0, v9 }, \
 { opcode, (mask)|BPRED|ANNUL, (lose), ",a,T 1,k", F_DELAYED|(flags), 0, v9 }, \
 { opcode, (mask), ANNUL|BPRED|(lose), ",N 1,k",   F_DELAYED|(flags), 0, v9 }, \
 { opcode, (mask)|ANNUL, BPRED|(lose), ",a,N 1,k", F_DELAYED|(flags), 0, v9 }

#define condr(bop, mask, flags) /* v9 */ \
  brr(bop, F2(0, 3)|COND(mask), F2(~0, ~3)|COND(~(mask)), (flags)) /* v9 */

/* v9 */ condr("brnz", 0x5, F_CONDBR),
/* v9 */ condr("brz", 0x1, F_CONDBR),
/* v9 */ condr("brgez", 0x7, F_CONDBR),
/* v9 */ condr("brlz", 0x3, F_CONDBR),
/* v9 */ condr("brlez", 0x2, F_CONDBR),
/* v9 */ condr("brgz", 0x6, F_CONDBR),

#define cbcond(cop, cmask) \
  { "cw" cop, F2(0, 3)|CBCOND(cmask)|F3I(0),F2(~0,~3)|CBCOND(~(cmask))|F3I(~0)|CBCOND_XCC, \
    "1,2,=", F_CONDBR, HWCAP_CBCOND, v9}, \
  { "cw" cop, F2(0, 3)|CBCOND(cmask)|F3I(1),F2(~0,~3)|CBCOND(~(cmask))|F3I(~1)|CBCOND_XCC, \
    "1,X,=", F_CONDBR, HWCAP_CBCOND, v9}, \
  { "cx" cop, F2(0, 3)|CBCOND(cmask)|F3I(0)|CBCOND_XCC,F2(~0,~3)|CBCOND(~(cmask))|F3I(~0), \
    "1,2,=", F_CONDBR, HWCAP_CBCOND, v9}, \
  { "cx" cop, F2(0, 3)|CBCOND(cmask)|F3I(1)|CBCOND_XCC,F2(~0,~3)|CBCOND(~(cmask))|F3I(~1), \
    "1,X,=", F_CONDBR, HWCAP_CBCOND, v9},

cbcond("be",   0x09)
cbcond("ble",  0x0a)
cbcond("bl",   0x0b)
cbcond("bleu", 0x0c)
cbcond("bcs",  0x0d)
cbcond("bneg", 0x0e)
cbcond("bvs",  0x0f)
cbcond("bne",  0x19)
cbcond("bg",   0x1a)
cbcond("bge",  0x1b)
cbcond("bgu",  0x1c)
cbcond("bcc",  0x1d)
cbcond("bpos", 0x1e)
cbcond("bvc",  0x1f)

#undef cbcond
#undef condr /* v9 */
#undef brr /* v9 */

#define movr(opcode, mask, flags) /* v9 */ \
 { opcode, F3(2, 0x2f, 0)|RCOND(mask), F3(~2, ~0x2f, ~0)|RCOND(~(mask)), "1,2,d", (flags), 0, v9 }, \
 { opcode, F3(2, 0x2f, 1)|RCOND(mask), F3(~2, ~0x2f, ~1)|RCOND(~(mask)), "1,j,d", (flags), 0, v9 }

#define fmrrs(opcode, mask, lose, flags) /* v9 */ \
 { opcode, (mask), (lose), "1,f,g", (flags) | F_FLOAT, 0, v9 }
#define fmrrd(opcode, mask, lose, flags) /* v9 */ \
 { opcode, (mask), (lose), "1,B,H", (flags) | F_FLOAT, 0, v9 }
#define fmrrq(opcode, mask, lose, flags) /* v9 */ \
 { opcode, (mask), (lose), "1,R,J", (flags) | F_FLOAT, 0, v9 }

#define fmovrs(mop, mask, flags) /* v9 */ \
  fmrrs(mop, F3(2, 0x35, 0)|OPF_LOW5(5)|RCOND(mask), F3(~2, ~0x35, 0)|OPF_LOW5(~5)|RCOND(~(mask)), (flags)) /* v9 */
#define fmovrd(mop, mask, flags) /* v9 */ \
  fmrrd(mop, F3(2, 0x35, 0)|OPF_LOW5(6)|RCOND(mask), F3(~2, ~0x35, 0)|OPF_LOW5(~6)|RCOND(~(mask)), (flags)) /* v9 */
#define fmovrq(mop, mask, flags) /* v9 */ \
  fmrrq(mop, F3(2, 0x35, 0)|OPF_LOW5(7)|RCOND(mask), F3(~2, ~0x35, 0)|OPF_LOW5(~7)|RCOND(~(mask)), (flags)) /* v9 */

/* v9 */ movr("movrne", 0x5, 0),
/* v9 */ movr("movre", 0x1, 0),
/* v9 */ movr("movrgez", 0x7, 0),
/* v9 */ movr("movrlz", 0x3, 0),
/* v9 */ movr("movrlez", 0x2, 0),
/* v9 */ movr("movrgz", 0x6, 0),
/* v9 */ movr("movrnz", 0x5, F_ALIAS),
/* v9 */ movr("movrz", 0x1, F_ALIAS),

/* v9 */ fmovrs("fmovrsne", 0x5, 0),
/* v9 */ fmovrs("fmovrse", 0x1, 0),
/* v9 */ fmovrs("fmovrsgez", 0x7, 0),
/* v9 */ fmovrs("fmovrslz", 0x3, 0),
/* v9 */ fmovrs("fmovrslez", 0x2, 0),
/* v9 */ fmovrs("fmovrsgz", 0x6, 0),
/* v9 */ fmovrs("fmovrsnz", 0x5, F_ALIAS),
/* v9 */ fmovrs("fmovrsz", 0x1, F_ALIAS),

/* v9 */ fmovrd("fmovrdne", 0x5, 0),
/* v9 */ fmovrd("fmovrde", 0x1, 0),
/* v9 */ fmovrd("fmovrdgez", 0x7, 0),
/* v9 */ fmovrd("fmovrdlz", 0x3, 0),
/* v9 */ fmovrd("fmovrdlez", 0x2, 0),
/* v9 */ fmovrd("fmovrdgz", 0x6, 0),
/* v9 */ fmovrd("fmovrdnz", 0x5, F_ALIAS),
/* v9 */ fmovrd("fmovrdz", 0x1, F_ALIAS),

/* v9 */ fmovrq("fmovrqne", 0x5, 0),
/* v9 */ fmovrq("fmovrqe", 0x1, 0),
/* v9 */ fmovrq("fmovrqgez", 0x7, 0),
/* v9 */ fmovrq("fmovrqlz", 0x3, 0),
/* v9 */ fmovrq("fmovrqlez", 0x2, 0),
/* v9 */ fmovrq("fmovrqgz", 0x6, 0),
/* v9 */ fmovrq("fmovrqnz", 0x5, F_ALIAS),
/* v9 */ fmovrq("fmovrqz", 0x1, F_ALIAS),

#undef movr /* v9 */
#undef fmovr /* v9 */
#undef fmrr /* v9 */

#define movicc(opcode, cond, flags) /* v9 */ \
  { opcode, F3(2, 0x2c, 0)|MCOND(cond,1)|ICC, F3(~2, ~0x2c, ~0)|MCOND(~cond,~1)|XCC|(1<<11), "z,2,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 1)|MCOND(cond,1)|ICC, F3(~2, ~0x2c, ~1)|MCOND(~cond,~1)|XCC|(1<<11), "z,I,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 0)|MCOND(cond,1)|XCC, F3(~2, ~0x2c, ~0)|MCOND(~cond,~1)|(1<<11),     "Z,2,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 1)|MCOND(cond,1)|XCC, F3(~2, ~0x2c, ~1)|MCOND(~cond,~1)|(1<<11),     "Z,I,d", flags, 0, v9 }

#define movfcc(opcode, fcond, flags) /* v9 */ \
  { opcode, F3(2, 0x2c, 0)|FCC(0)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~0)|F3(~2, ~0x2c, ~0), "6,2,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 1)|FCC(0)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~0)|F3(~2, ~0x2c, ~1), "6,I,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 0)|FCC(1)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~1)|F3(~2, ~0x2c, ~0), "7,2,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 1)|FCC(1)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~1)|F3(~2, ~0x2c, ~1), "7,I,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 0)|FCC(2)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~2)|F3(~2, ~0x2c, ~0), "8,2,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 1)|FCC(2)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~2)|F3(~2, ~0x2c, ~1), "8,I,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 0)|FCC(3)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~3)|F3(~2, ~0x2c, ~0), "9,2,d", flags, 0, v9 }, \
  { opcode, F3(2, 0x2c, 1)|FCC(3)|MCOND(fcond,0), MCOND(~fcond,~0)|FCC(~3)|F3(~2, ~0x2c, ~1), "9,I,d", flags, 0, v9 }

#define movcc(opcode, cond, fcond, flags) /* v9 */ \
  movfcc (opcode, fcond, flags), /* v9 */ \
  movicc (opcode, cond, flags) /* v9 */

/* v9 */ movcc  ("mova",	CONDA, FCONDA, 0),
/* v9 */ movicc ("movcc",	CONDCC, 0),
/* v9 */ movicc ("movgeu",	CONDGEU, F_ALIAS),
/* v9 */ movicc ("movcs",	CONDCS, 0),
/* v9 */ movicc ("movlu",	CONDLU, F_ALIAS),
/* v9 */ movcc  ("move",	CONDE, FCONDE, 0),
/* v9 */ movcc  ("movg",	CONDG, FCONDG, 0),
/* v9 */ movcc  ("movge",	CONDGE, FCONDGE, 0),
/* v9 */ movicc ("movgu",	CONDGU, 0),
/* v9 */ movcc  ("movl",	CONDL, FCONDL, 0),
/* v9 */ movcc  ("movle",	CONDLE, FCONDLE, 0),
/* v9 */ movicc ("movleu",	CONDLEU, 0),
/* v9 */ movfcc ("movlg",	FCONDLG, 0),
/* v9 */ movcc  ("movn",	CONDN, FCONDN, 0),
/* v9 */ movcc  ("movne",	CONDNE, FCONDNE, 0),
/* v9 */ movicc ("movneg",	CONDNEG, 0),
/* v9 */ movcc  ("movnz",	CONDNZ, FCONDNZ, F_ALIAS),
/* v9 */ movfcc ("movo",	FCONDO, 0),
/* v9 */ movicc ("movpos",	CONDPOS, 0),
/* v9 */ movfcc ("movu",	FCONDU, 0),
/* v9 */ movfcc ("movue",	FCONDUE, 0),
/* v9 */ movfcc ("movug",	FCONDUG, 0),
/* v9 */ movfcc ("movuge",	FCONDUGE, 0),
/* v9 */ movfcc ("movul",	FCONDUL, 0),
/* v9 */ movfcc ("movule",	FCONDULE, 0),
/* v9 */ movicc ("movvc",	CONDVC, 0),
/* v9 */ movicc ("movvs",	CONDVS, 0),
/* v9 */ movcc  ("movz",	CONDZ, FCONDZ, F_ALIAS),

#undef movicc /* v9 */
#undef movfcc /* v9 */
#undef movcc /* v9 */

#define FM_SF 1		/* v9 - values for fpsize */
#define FM_DF 2		/* v9 */
#define FM_QF 3		/* v9 */

#define fmoviccx(opcode, fpsize, args, cond, flags) /* v9 */ \
{ opcode, F3F(2, 0x35, 0x100+fpsize)|MCOND(cond,0),  F3F(~2, ~0x35, ~(0x100+fpsize))|MCOND(~cond,~0),  "z," args, flags, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x180+fpsize)|MCOND(cond,0),  F3F(~2, ~0x35, ~(0x180+fpsize))|MCOND(~cond,~0),  "Z," args, flags, 0, v9 }

#define fmovfccx(opcode, fpsize, args, fcond, flags) /* v9 */ \
{ opcode, F3F(2, 0x35, 0x000+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x000+fpsize))|MCOND(~fcond,~0), "6," args, flags, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x040+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x040+fpsize))|MCOND(~fcond,~0), "7," args, flags, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x080+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x080+fpsize))|MCOND(~fcond,~0), "8," args, flags, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x0c0+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x0c0+fpsize))|MCOND(~fcond,~0), "9," args, flags, 0, v9 }

/* FIXME: use fmovicc/fmovfcc? */ /* v9 */
#define fmovccx(opcode, fpsize, args, cond, fcond, flags) /* v9 */ \
{ opcode, F3F(2, 0x35, 0x100+fpsize)|MCOND(cond,0),  F3F(~2, ~0x35, ~(0x100+fpsize))|MCOND(~cond,~0),  "z," args, flags | F_FLOAT, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x000+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x000+fpsize))|MCOND(~fcond,~0), "6," args, flags | F_FLOAT, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x180+fpsize)|MCOND(cond,0),  F3F(~2, ~0x35, ~(0x180+fpsize))|MCOND(~cond,~0),  "Z," args, flags | F_FLOAT, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x040+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x040+fpsize))|MCOND(~fcond,~0), "7," args, flags | F_FLOAT, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x080+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x080+fpsize))|MCOND(~fcond,~0), "8," args, flags | F_FLOAT, 0, v9 }, \
{ opcode, F3F(2, 0x35, 0x0c0+fpsize)|MCOND(fcond,0), F3F(~2, ~0x35, ~(0x0c0+fpsize))|MCOND(~fcond,~0), "9," args, flags | F_FLOAT, 0, v9 }

#define fmovicc(suffix, cond, flags) /* v9 */ \
fmoviccx("fmovd" suffix, FM_DF, "B,H", cond, flags),		\
fmoviccx("fmovq" suffix, FM_QF, "R,J", cond, flags),		\
fmoviccx("fmovs" suffix, FM_SF, "f,g", cond, flags)

#define fmovfcc(suffix, fcond, flags) /* v9 */ \
fmovfccx("fmovd" suffix, FM_DF, "B,H", fcond, flags),		\
fmovfccx("fmovq" suffix, FM_QF, "R,J", fcond, flags),		\
fmovfccx("fmovs" suffix, FM_SF, "f,g", fcond, flags)

#define fmovcc(suffix, cond, fcond, flags) /* v9 */ \
fmovccx("fmovd" suffix, FM_DF, "B,H", cond, fcond, flags),	\
fmovccx("fmovq" suffix, FM_QF, "R,J", cond, fcond, flags),	\
fmovccx("fmovs" suffix, FM_SF, "f,g", cond, fcond, flags)

/* v9 */ fmovcc  ("a", CONDA, FCONDA, 0),
/* v9 */ fmovicc ("cc", CONDCC, 0),
/* v9 */ fmovicc ("cs", CONDCS, 0),
/* v9 */ fmovcc  ("e", CONDE, FCONDE, 0),
/* v9 */ fmovcc  ("g", CONDG, FCONDG, 0),
/* v9 */ fmovcc  ("ge", CONDGE, FCONDGE, 0),
/* v9 */ fmovicc ("geu", CONDGEU, F_ALIAS),
/* v9 */ fmovicc ("gu", CONDGU, 0),
/* v9 */ fmovcc  ("l", CONDL, FCONDL, 0),
/* v9 */ fmovcc  ("le", CONDLE, FCONDLE, 0),
/* v9 */ fmovicc ("leu", CONDLEU, 0),
/* v9 */ fmovfcc ("lg", FCONDLG, 0),
/* v9 */ fmovicc ("lu", CONDLU, F_ALIAS),
/* v9 */ fmovcc  ("n", CONDN, FCONDN, 0),
/* v9 */ fmovcc  ("ne", CONDNE, FCONDNE, 0),
/* v9 */ fmovicc ("neg", CONDNEG, 0),
/* v9 */ fmovcc  ("nz", CONDNZ, FCONDNZ, F_ALIAS),
/* v9 */ fmovfcc ("o", FCONDO, 0),
/* v9 */ fmovicc ("pos", CONDPOS, 0),
/* v9 */ fmovfcc ("u", FCONDU, 0),
/* v9 */ fmovfcc ("ue", FCONDUE, 0),
/* v9 */ fmovfcc ("ug", FCONDUG, 0),
/* v9 */ fmovfcc ("uge", FCONDUGE, 0),
/* v9 */ fmovfcc ("ul", FCONDUL, 0),
/* v9 */ fmovfcc ("ule", FCONDULE, 0),
/* v9 */ fmovicc ("vc", CONDVC, 0),
/* v9 */ fmovicc ("vs", CONDVS, 0),
/* v9 */ fmovcc  ("z", CONDZ, FCONDZ, F_ALIAS),

#undef fmoviccx /* v9 */
#undef fmovfccx /* v9 */
#undef fmovccx /* v9 */
#undef fmovicc /* v9 */
#undef fmovfcc /* v9 */
#undef fmovcc /* v9 */
#undef FM_DF /* v9 */
#undef FM_QF /* v9 */
#undef FM_SF /* v9 */

/* Coprocessor branches.  */
#define CBR(opcode, mask, lose, flags, arch) \
 { opcode, (mask), ANNUL | (lose), "l",    flags | F_DELAYED, 0, arch }, \
 { opcode, (mask) | ANNUL, (lose), ",a l", flags | F_DELAYED, 0, arch }

/* Floating point branches.  */
#define FBR(opcode, mask, lose, flags) \
 { opcode, (mask), ANNUL | (lose), "l",    flags | F_DELAYED | F_FBR, 0, v6 }, \
 { opcode, (mask) | ANNUL, (lose), ",a l", flags | F_DELAYED | F_FBR, 0, v6 }

/* V9 extended floating point branches.  */
#define FBRX(opcode, mask, lose, flags) /* v9 */ \
 { opcode, FBFCC(0)|(mask)|BPRED, ANNUL|FBFCC(~0)|(lose), "6,G",      flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(0)|(mask)|BPRED, ANNUL|FBFCC(~0)|(lose), ",T 6,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(0)|(mask)|BPRED|ANNUL, FBFCC(~0)|(lose), ",a 6,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(0)|(mask)|BPRED|ANNUL, FBFCC(~0)|(lose), ",a,T 6,G", flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(0)|(mask), ANNUL|BPRED|FBFCC(~0)|(lose), ",N 6,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(0)|(mask)|ANNUL, BPRED|FBFCC(~0)|(lose), ",a,N 6,G", flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(1)|(mask)|BPRED, ANNUL|FBFCC(~1)|(lose), "7,G",      flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(1)|(mask)|BPRED, ANNUL|FBFCC(~1)|(lose), ",T 7,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(1)|(mask)|BPRED|ANNUL, FBFCC(~1)|(lose), ",a 7,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(1)|(mask)|BPRED|ANNUL, FBFCC(~1)|(lose), ",a,T 7,G", flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(1)|(mask), ANNUL|BPRED|FBFCC(~1)|(lose), ",N 7,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(1)|(mask)|ANNUL, BPRED|FBFCC(~1)|(lose), ",a,N 7,G", flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(2)|(mask)|BPRED, ANNUL|FBFCC(~2)|(lose), "8,G",      flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(2)|(mask)|BPRED, ANNUL|FBFCC(~2)|(lose), ",T 8,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(2)|(mask)|BPRED|ANNUL, FBFCC(~2)|(lose), ",a 8,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(2)|(mask)|BPRED|ANNUL, FBFCC(~2)|(lose), ",a,T 8,G", flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(2)|(mask), ANNUL|BPRED|FBFCC(~2)|(lose), ",N 8,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(2)|(mask)|ANNUL, BPRED|FBFCC(~2)|(lose), ",a,N 8,G", flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(3)|(mask)|BPRED, ANNUL|FBFCC(~3)|(lose), "9,G",      flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(3)|(mask)|BPRED, ANNUL|FBFCC(~3)|(lose), ",T 9,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(3)|(mask)|BPRED|ANNUL, FBFCC(~3)|(lose), ",a 9,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(3)|(mask)|BPRED|ANNUL, FBFCC(~3)|(lose), ",a,T 9,G", flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(3)|(mask), ANNUL|BPRED|FBFCC(~3)|(lose), ",N 9,G",   flags|F_DELAYED|F_FBR, 0, v9 }, \
 { opcode, FBFCC(3)|(mask)|ANNUL, BPRED|FBFCC(~3)|(lose), ",a,N 9,G", flags|F_DELAYED|F_FBR, 0, v9 }

/* v9: We must put `FBRX' before `FBR', to ensure that we never match
   v9: something against an expression unless it is an expression.  Otherwise,
   v9: we end up with undefined symbol tables entries, because they get added,
   v9: but are not deleted if the pattern fails to match.  */

#define CONDFC(fop, cop, mask, flags) \
  FBRX(fop, F2(0, 5)|COND(mask), F2(~0, ~5)|COND(~(mask)), flags), /* v9 */ \
  FBR(fop, F2(0, 6)|COND(mask), F2(~0, ~6)|COND(~(mask)), flags), \
  CBR(cop, F2(0, 7)|COND(mask), F2(~0, ~7)|COND(~(mask)), flags, v6notlet)

#define CONDFCL(fop, cop, mask, flags) \
  FBRX(fop, F2(0, 5)|COND(mask), F2(~0, ~5)|COND(~(mask)), flags), /* v9 */ \
  FBR(fop, F2(0, 6)|COND(mask), F2(~0, ~6)|COND(~(mask)), flags), \
  CBR(cop, F2(0, 7)|COND(mask), F2(~0, ~7)|COND(~(mask)), flags, v6)

#define CONDF(fop, mask, flags) \
  FBRX(fop, F2(0, 5)|COND(mask), F2(~0, ~5)|COND(~(mask)), flags), /* v9 */ \
  FBR(fop, F2(0, 6)|COND(mask), F2(~0, ~6)|COND(~(mask)), flags)

CONDFC  ("fb",    "cb",    0x8, F_UNBR),
CONDFCL ("fba",	  "cba",   0x8, F_UNBR|F_ALIAS),
CONDFC  ("fbe",	  "cb0",   0x9, F_CONDBR),
CONDF   ("fbz",            0x9, F_CONDBR|F_ALIAS),
CONDFC  ("fbg",	  "cb2",   0x6, F_CONDBR),
CONDFC  ("fbge",  "cb02",  0xb, F_CONDBR),
CONDFC  ("fbl",	  "cb1",   0x4, F_CONDBR),
CONDFC  ("fble",  "cb01",  0xd, F_CONDBR),
CONDFC  ("fblg",  "cb12",  0x2, F_CONDBR),
CONDFCL ("fbn",	  "cbn",   0x0, F_UNBR),
CONDFC  ("fbne",  "cb123", 0x1, F_CONDBR),
CONDF   ("fbnz",           0x1, F_CONDBR|F_ALIAS),
CONDFC  ("fbo",	  "cb012", 0xf, F_CONDBR),
CONDFC  ("fbu",	  "cb3",   0x7, F_CONDBR),
CONDFC  ("fbue",  "cb03",  0xa, F_CONDBR),
CONDFC  ("fbug",  "cb23",  0x5, F_CONDBR),
CONDFC  ("fbuge", "cb023", 0xc, F_CONDBR),
CONDFC  ("fbul",  "cb13",  0x3, F_CONDBR),
CONDFC  ("fbule", "cb013", 0xe, F_CONDBR),

#undef CONDFC
#undef CONDFCL
#undef CONDF
#undef CBR
#undef FBR
#undef FBRX	/* v9 */

{ "jmp",	F3(2, 0x38, 0), F3(~2, ~0x38, ~0)|RD_G0|ASI(~0),	"1+2", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl rs1+rs2,%g0 */
{ "jmp",	F3(2, 0x38, 0), F3(~2, ~0x38, ~0)|RD_G0|ASI_RS2(~0),	"1", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl rs1+%g0,%g0 */
{ "jmp",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1)|RD_G0,		"1+i", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl rs1+i,%g0 */
{ "jmp",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1)|RD_G0,		"i+1", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl i+rs1,%g0 */
{ "jmp",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1)|RD_G0|RS1_G0,		"i", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl %g0+i,%g0 */
{ "jmp",	F3(2, 0x38, 1), F3(~2, ~0x38, ~1)|RD_G0|SIMM13(~0),	"1", F_UNBR|F_DELAYED, 0, v6 }, /* jmpl rs1+0,%g0 */

{ "nop",	F2(0, 4), 0xfeffffff, "", 0, 0, v6 }, /* sethi 0, %g0 */

{ "set",	F2(0x0, 0x4), F2(~0x0, ~0x4), "S0,d", F_ALIAS, 0, v6 },
{ "setuw",	F2(0x0, 0x4), F2(~0x0, ~0x4), "S0,d", F_ALIAS, 0, v9 },
{ "setsw",	F2(0x0, 0x4), F2(~0x0, ~0x4), "S0,d", F_ALIAS, 0, v9 },
{ "setx",	F2(0x0, 0x4), F2(~0x0, ~0x4), "S0,1,d", F_ALIAS, 0, v9 },

{ "sethi",	F2(0x0, 0x4), F2(~0x0, ~0x4), "h,d", 0, 0, v6 },

{ "taddcc",	F3(2, 0x20, 0), F3(~2, ~0x20, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "taddcc",	F3(2, 0x20, 1), F3(~2, ~0x20, ~1),		"1,i,d", 0, 0, v6 },
{ "taddcc",	F3(2, 0x20, 1), F3(~2, ~0x20, ~1),		"i,1,d", 0, 0, v6 },
{ "taddcctv",	F3(2, 0x22, 0), F3(~2, ~0x22, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "taddcctv",	F3(2, 0x22, 1), F3(~2, ~0x22, ~1),		"1,i,d", 0, 0, v6 },
{ "taddcctv",	F3(2, 0x22, 1), F3(~2, ~0x22, ~1),		"i,1,d", 0, 0, v6 },

{ "tsubcc",	F3(2, 0x21, 0), F3(~2, ~0x21, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "tsubcc",	F3(2, 0x21, 1), F3(~2, ~0x21, ~1),		"1,i,d", 0, 0, v6 },
{ "tsubcctv",	F3(2, 0x23, 0), F3(~2, ~0x23, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "tsubcctv",	F3(2, 0x23, 1), F3(~2, ~0x23, ~1),		"1,i,d", 0, 0, v6 },

{ "unimp",	F2(0x0, 0x0), 0xffc00000, "n", 0, 0, v6notv9 },
{ "illtrap",	F2(0, 0), F2(~0, ~0)|RD_G0, "n", 0, 0, v9 },

/* This *is* a commutative instruction.  */
{ "xnor",	F3(2, 0x07, 0), F3(~2, ~0x07, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "xnor",	F3(2, 0x07, 1), F3(~2, ~0x07, ~1),		"1,i,d", 0, 0, v6 },
{ "xnor",	F3(2, 0x07, 1), F3(~2, ~0x07, ~1),		"i,1,d", 0, 0, v6 },
{ "xnord",	F4(2, 0x07, 0, 1), F4(~2, ~0x07, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */
/* This *is* a commutative instruction.  */
{ "xnorcc",	F3(2, 0x17, 0), F3(~2, ~0x17, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "xnorcc",	F3(2, 0x17, 1), F3(~2, ~0x17, ~1),		"1,i,d", 0, 0, v6 },
{ "xnorcc",	F3(2, 0x17, 1), F3(~2, ~0x17, ~1),		"i,1,d", 0, 0, v6 },
{ "xnordcc",	F4(2, 0x17, 0, 1), F4(~2, ~0x17, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */
{ "xor",	F3(2, 0x03, 0), F3(~2, ~0x03, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "xor",	F3(2, 0x03, 1), F3(~2, ~0x03, ~1),		"1,i,d", 0, 0, v6 },
{ "xor",	F3(2, 0x03, 1), F3(~2, ~0x03, ~1),		"i,1,d", 0, 0, v6 },
{ "xord",	F4(2, 0X03, 0, 1), F4(~2, ~0X03, ~0, ~1),	"1,2,d", 0, 0, v6 },
{ "xorcc",	F3(2, 0x13, 0), F3(~2, ~0x13, ~0)|ASI(~0),	"1,2,d", 0, 0, v6 },
{ "xorcc",	F3(2, 0x13, 1), F3(~2, ~0x13, ~1),		"1,i,d", 0, 0, v6 },
{ "xorcc",	F3(2, 0x13, 1), F3(~2, ~0x13, ~1),		"i,1,d", 0, 0, v6 },
{ "xordcc",	F4(2, 0x13, 0, 1), F4(~2, ~0x13, ~0, ~1),	"1,2,d", 0, 0, v6 }, /* AJIT */

{ "not",	F3(2, 0x07, 0), F3(~2, ~0x07, ~0)|ASI(~0), "1,d", F_ALIAS, 0, v6 }, /* xnor rs1,%0,rd */
{ "not",	F3(2, 0x07, 0), F3(~2, ~0x07, ~0)|ASI(~0), "r", F_ALIAS, 0, v6 }, /* xnor rd,%0,rd */

{ "btog",	F3(2, 0x03, 0), F3(~2, ~0x03, ~0)|ASI(~0),	"2,r", F_ALIAS, 0, v6 }, /* xor rd,rs2,rd */
{ "btog",	F3(2, 0x03, 1), F3(~2, ~0x03, ~1),		"i,r", F_ALIAS, 0, v6 }, /* xor rd,i,rd */

/* AJIT Additions */
/* SIMD II instructions */
/* AJIT ISA V2: Aug 27, 2020. Changed success on: Sep 27, 2020. */
{ "adddreduce8",  F8(2, 0x2d, 0x0, 0x1), F8(~2, ~0x2d, ~0x0, ~0x1), "1,2,d", 0, 0, v8}, /* AJIT */
{ "ordreduce8",   F8(2, 0x2e, 0x0, 0x1), F8(~2, ~0x2e, ~0x0, ~0x1), "1,2,d", 0, 0, v8}, /* AJIT */
{ "anddreduce8",  F8(2, 0x2f, 0x0, 0x1), F8(~2, ~0x2f, ~0x0, ~0x1), "1,2,d", 0, 0, v8}, /* AJIT */
{ "xordreduce8",  F8(2, 0x3e, 0x0, 0x1), F8(~2, ~0x3e, ~0x0, ~0x1), "1,2,d", 0, 0, v8}, /* AJIT */
{ "adddreduce16", F8(2, 0x2d, 0x0, 0x2), F8(~2, ~0x2d, ~0x0, ~0x2), "1,2,d", 0, 0, v8}, /* AJIT */
{ "ordreduce16",  F8(2, 0x2e, 0x0, 0x2), F8(~2, ~0x2e, ~0x0, ~0x2), "1,2,d", 0, 0, v8}, /* AJIT */
{ "anddreduce16", F8(2, 0x2f, 0x0, 0x2), F8(~2, ~0x2f, ~0x0, ~0x2), "1,2,d", 0, 0, v8}, /* AJIT */
{ "xordreduce16", F8(2, 0x3e, 0x0, 0x2), F8(~2, ~0x3e, ~0x0, ~0x2), "1,2,d", 0, 0, v8}, /* AJIT */
{ "zbytedpos",    F8(2, 0x3f, 0x0, 0x0), F8(~2, ~0x3f, ~0x0, ~0x0), "1,2,d", 0, 0, v8}, /* AJIT */
{ "zbytedpos",    F8I(2, 0x3f, 0x1, 0x0), F8I(~2, ~0x3f, ~0x1, ~0x0), "1,i,d", 0, 0, v8}, /* AJIT */

/* FPop1 and FPop2 are not instructions.  Don't accept them.  */

{ "fdtoi",	F3F(2, 0x34, 0x0d2), F3F(~2, ~0x34, ~0x0d2)|RS1_G0, "B,g", F_FLOAT, 0, v6 },
{ "fstoi",	F3F(2, 0x34, 0x0d1), F3F(~2, ~0x34, ~0x0d1)|RS1_G0, "f,g", F_FLOAT, 0, v6 },
{ "fqtoi",	F3F(2, 0x34, 0x0d3), F3F(~2, ~0x34, ~0x0d3)|RS1_G0, "R,g", F_FLOAT, 0, v8 },

{ "fdtox",	F3F(2, 0x34, 0x082), F3F(~2, ~0x34, ~0x082)|RS1_G0, "B,H", F_FLOAT, 0, v9 },
{ "fstox",	F3F(2, 0x34, 0x081), F3F(~2, ~0x34, ~0x081)|RS1_G0, "f,H", F_FLOAT, 0, v9 },
{ "fqtox",	F3F(2, 0x34, 0x083), F3F(~2, ~0x34, ~0x083)|RS1_G0, "R,H", F_FLOAT, 0, v9 },

{ "fitod",	F3F(2, 0x34, 0x0c8), F3F(~2, ~0x34, ~0x0c8)|RS1_G0, "f,H", F_FLOAT, 0, v6 },
{ "fitos",	F3F(2, 0x34, 0x0c4), F3F(~2, ~0x34, ~0x0c4)|RS1_G0, "f,g", F_FLOAT, 0, v6 },
{ "fitoq",	F3F(2, 0x34, 0x0cc), F3F(~2, ~0x34, ~0x0cc)|RS1_G0, "f,J", F_FLOAT, 0, v8 },

{ "fxtod",	F3F(2, 0x34, 0x088), F3F(~2, ~0x34, ~0x088)|RS1_G0, "B,H", F_FLOAT, 0, v9 },
{ "fxtos",	F3F(2, 0x34, 0x084), F3F(~2, ~0x34, ~0x084)|RS1_G0, "B,g", F_FLOAT, 0, v9 },
{ "fxtoq",	F3F(2, 0x34, 0x08c), F3F(~2, ~0x34, ~0x08c)|RS1_G0, "B,J", F_FLOAT, 0, v9 },

{ "fdtoq",	F3F(2, 0x34, 0x0ce), F3F(~2, ~0x34, ~0x0ce)|RS1_G0, "B,J", F_FLOAT, 0, v8 },
{ "fdtos",	F3F(2, 0x34, 0x0c6), F3F(~2, ~0x34, ~0x0c6)|RS1_G0, "B,g", F_FLOAT, 0, v6 },
{ "fqtod",	F3F(2, 0x34, 0x0cb), F3F(~2, ~0x34, ~0x0cb)|RS1_G0, "R,H", F_FLOAT, 0, v8 },
{ "fqtos",	F3F(2, 0x34, 0x0c7), F3F(~2, ~0x34, ~0x0c7)|RS1_G0, "R,g", F_FLOAT, 0, v8 },
{ "fstod",	F3F(2, 0x34, 0x0c9), F3F(~2, ~0x34, ~0x0c9)|RS1_G0, "f,H", F_FLOAT, 0, v6 },
{ "fstoq",	F3F(2, 0x34, 0x0cd), F3F(~2, ~0x34, ~0x0cd)|RS1_G0, "f,J", F_FLOAT, 0, v8 },

{ "fdivd",	F3F(2, 0x34, 0x04e), F3F(~2, ~0x34, ~0x04e), "v,B,H", F_FLOAT, 0, v6 },
{ "fdivq",	F3F(2, 0x34, 0x04f), F3F(~2, ~0x34, ~0x04f), "V,R,J", F_FLOAT, 0, v8 },
{ "fdivx",	F3F(2, 0x34, 0x04f), F3F(~2, ~0x34, ~0x04f), "V,R,J", F_FLOAT|F_ALIAS, 0, v8 },
{ "fdivs",	F3F(2, 0x34, 0x04d), F3F(~2, ~0x34, ~0x04d), "e,f,g", F_FLOAT, 0, v6 },
{ "fmuld",	F3F(2, 0x34, 0x04a), F3F(~2, ~0x34, ~0x04a), "v,B,H", F_FLOAT, 0, v6 },
{ "fmulq",	F3F(2, 0x34, 0x04b), F3F(~2, ~0x34, ~0x04b), "V,R,J", F_FLOAT, 0, v8 },
{ "fmulx",	F3F(2, 0x34, 0x04b), F3F(~2, ~0x34, ~0x04b), "V,R,J", F_FLOAT|F_ALIAS, 0, v8 },
{ "fmuls",	F3F(2, 0x34, 0x049), F3F(~2, ~0x34, ~0x049), "e,f,g", F_FLOAT, 0, v6 },

{ "fdmulq",	F3F(2, 0x34, 0x06e), F3F(~2, ~0x34, ~0x06e), "v,B,J", F_FLOAT, 0, v8 },
{ "fdmulx",	F3F(2, 0x34, 0x06e), F3F(~2, ~0x34, ~0x06e), "v,B,J", F_FLOAT|F_ALIAS, 0, v8 },
{ "fsmuld",	F3F(2, 0x34, 0x069), F3F(~2, ~0x34, ~0x069), "e,f,H", F_FLOAT, HWCAP_FSMULD, v8 },

{ "fsqrtd",	F3F(2, 0x34, 0x02a), F3F(~2, ~0x34, ~0x02a)|RS1_G0, "B,H", F_FLOAT, 0, v7 },
{ "fsqrtq",	F3F(2, 0x34, 0x02b), F3F(~2, ~0x34, ~0x02b)|RS1_G0, "R,J", F_FLOAT, 0, v8 },
{ "fsqrtx",	F3F(2, 0x34, 0x02b), F3F(~2, ~0x34, ~0x02b)|RS1_G0, "R,J", F_FLOAT|F_ALIAS, 0, v8 },
{ "fsqrts",	F3F(2, 0x34, 0x029), F3F(~2, ~0x34, ~0x029)|RS1_G0, "f,g", F_FLOAT, 0, v7 },

{ "fabsd",	F3F(2, 0x34, 0x00a), F3F(~2, ~0x34, ~0x00a)|RS1_G0, "B,H", F_FLOAT, 0, v9 },
{ "fabsq",	F3F(2, 0x34, 0x00b), F3F(~2, ~0x34, ~0x00b)|RS1_G0, "R,J", F_FLOAT, 0, v9 },
{ "fabsx",	F3F(2, 0x34, 0x00b), F3F(~2, ~0x34, ~0x00b)|RS1_G0, "R,J", F_FLOAT|F_ALIAS, 0, v9 },
{ "fabss",	F3F(2, 0x34, 0x009), F3F(~2, ~0x34, ~0x009)|RS1_G0, "f,g", F_FLOAT, 0, v6 },
{ "fmovd",	F3F(2, 0x34, 0x002), F3F(~2, ~0x34, ~0x002)|RS1_G0, "B,H", F_FLOAT, 0, v9 },
{ "fmovq",	F3F(2, 0x34, 0x003), F3F(~2, ~0x34, ~0x003)|RS1_G0, "R,J", F_FLOAT, 0, v9 },
{ "fmovx",	F3F(2, 0x34, 0x003), F3F(~2, ~0x34, ~0x003)|RS1_G0, "R,J", F_FLOAT|F_ALIAS, 0, v9 },
{ "fmovs",	F3F(2, 0x34, 0x001), F3F(~2, ~0x34, ~0x001)|RS1_G0, "f,g", F_FLOAT, 0, v6 },
{ "fnegd",	F3F(2, 0x34, 0x006), F3F(~2, ~0x34, ~0x006)|RS1_G0, "B,H", F_FLOAT, 0, v9 },
{ "fnegq",	F3F(2, 0x34, 0x007), F3F(~2, ~0x34, ~0x007)|RS1_G0, "R,J", F_FLOAT, 0, v9 },
{ "fnegx",	F3F(2, 0x34, 0x007), F3F(~2, ~0x34, ~0x007)|RS1_G0, "R,J", F_FLOAT|F_ALIAS, 0, v9 },
{ "fnegs",	F3F(2, 0x34, 0x005), F3F(~2, ~0x34, ~0x005)|RS1_G0, "f,g", F_FLOAT, 0, v6 },

{ "faddd",	F3F(2, 0x34, 0x042), F3F(~2, ~0x34, ~0x042), "v,B,H", F_FLOAT, 0, v6 },
{ "faddq",	F3F(2, 0x34, 0x043), F3F(~2, ~0x34, ~0x043), "V,R,J", F_FLOAT, 0, v8 },
{ "faddx",	F3F(2, 0x34, 0x043), F3F(~2, ~0x34, ~0x043), "V,R,J", F_FLOAT|F_ALIAS, 0, v8 },
{ "fadds",	F3F(2, 0x34, 0x041), F3F(~2, ~0x34, ~0x041), "e,f,g", F_FLOAT, 0, v6 },
{ "fsubd",	F3F(2, 0x34, 0x046), F3F(~2, ~0x34, ~0x046), "v,B,H", F_FLOAT, 0, v6 },
{ "fsubq",	F3F(2, 0x34, 0x047), F3F(~2, ~0x34, ~0x047), "V,R,J", F_FLOAT, 0, v8 },
{ "fsubx",	F3F(2, 0x34, 0x047), F3F(~2, ~0x34, ~0x047), "V,R,J", F_FLOAT|F_ALIAS, 0, v8 },
{ "fsubs",	F3F(2, 0x34, 0x045), F3F(~2, ~0x34, ~0x045), "e,f,g", F_FLOAT, 0, v6 },

/* AJIT Additions */
/* SIMD Floating point ops */
/* AJIT ISA V2: Aug 27, 2020. Changed success on: Sep 27, 2020. */
{ "vfadd32",    F3F(2, 0x34, 0x142), F3F(~2, ~0x34, ~0x142), "v,B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "vfadd16",    F3F(2, 0x34, 0x143), F3F(~2, ~0x34, ~0x143), "v,B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "vfsub32",    F3F(2, 0x34, 0x144), F3F(~2, ~0x34, ~0x144), "v,B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "vfsub16",    F3F(2, 0x34, 0x145), F3F(~2, ~0x34, ~0x145), "v,B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "vfmul32",    F3F(2, 0x34, 0x146), F3F(~2, ~0x34, ~0x146), "v,B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "vfmul16",    F3F(2, 0x34, 0x147), F3F(~2, ~0x34, ~0x147), "v,B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
/* AJIT ISA V2: Aug 27, 2020. Changed success on: Sep 27, 2020. */
/* AJIT ISA V2: New additions */
{ "vfi16toh",   F3F(2, 0x34, 0x148), F3F(~2, ~0x34, ~0x148), "B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "vfhtoi16",   F3F(2, 0x34, 0x149), F3F(~2, ~0x34, ~0x149), "B,H", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "faddreduce16", F3F(2, 0x34, 0x150), F3F(~2, ~0x34, ~0x150), "B,g", F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "fstoh",      F3F(2, 0x34, 0x151), F3F(~2, ~0x34, ~0x151), "f,g",   F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */
{ "fhtos",      F3F(2, 0x34, 0x152), F3F(~2, ~0x34, ~0x152), "f,g",   F_FLOAT, 0, v8}, /* rs1,rs2,rd are even numbered! */ /* AJIT */

#define CMPFCC(x)	(((x)&0x3)<<25)

{ "fcmpd",	          F3F(2, 0x35, 0x052),            F3F(~2, ~0x35, ~0x052)|RD_G0,  "v,B",   F_FLOAT, 0, v6 },
{ "fcmpd",	CMPFCC(0)|F3F(2, 0x35, 0x052), CMPFCC(~0)|F3F(~2, ~0x35, ~0x052),	 "6,v,B", F_FLOAT, 0, v9 },
{ "fcmpd",	CMPFCC(1)|F3F(2, 0x35, 0x052), CMPFCC(~1)|F3F(~2, ~0x35, ~0x052),	 "7,v,B", F_FLOAT, 0, v9 },
{ "fcmpd",	CMPFCC(2)|F3F(2, 0x35, 0x052), CMPFCC(~2)|F3F(~2, ~0x35, ~0x052),	 "8,v,B", F_FLOAT, 0, v9 },
{ "fcmpd",	CMPFCC(3)|F3F(2, 0x35, 0x052), CMPFCC(~3)|F3F(~2, ~0x35, ~0x052),	 "9,v,B", F_FLOAT, 0, v9 },
{ "fcmped",	          F3F(2, 0x35, 0x056),            F3F(~2, ~0x35, ~0x056)|RD_G0,  "v,B",   F_FLOAT, 0, v6 },
{ "fcmped",	CMPFCC(0)|F3F(2, 0x35, 0x056), CMPFCC(~0)|F3F(~2, ~0x35, ~0x056),	 "6,v,B", F_FLOAT, 0, v9 },
{ "fcmped",	CMPFCC(1)|F3F(2, 0x35, 0x056), CMPFCC(~1)|F3F(~2, ~0x35, ~0x056),	 "7,v,B", F_FLOAT, 0, v9 },
{ "fcmped",	CMPFCC(2)|F3F(2, 0x35, 0x056), CMPFCC(~2)|F3F(~2, ~0x35, ~0x056),	 "8,v,B", F_FLOAT, 0, v9 },
{ "fcmped",	CMPFCC(3)|F3F(2, 0x35, 0x056), CMPFCC(~3)|F3F(~2, ~0x35, ~0x056),	 "9,v,B", F_FLOAT, 0, v9 },
{ "fcmpq",	          F3F(2, 0x35, 0x053),            F3F(~2, ~0x35, ~0x053)|RD_G0,	 "V,R", F_FLOAT, 0, v8 },
{ "fcmpq",	CMPFCC(0)|F3F(2, 0x35, 0x053), CMPFCC(~0)|F3F(~2, ~0x35, ~0x053),	 "6,V,R", F_FLOAT, 0, v9 },
{ "fcmpq",	CMPFCC(1)|F3F(2, 0x35, 0x053), CMPFCC(~1)|F3F(~2, ~0x35, ~0x053),	 "7,V,R", F_FLOAT, 0, v9 },
{ "fcmpq",	CMPFCC(2)|F3F(2, 0x35, 0x053), CMPFCC(~2)|F3F(~2, ~0x35, ~0x053),	 "8,V,R", F_FLOAT, 0, v9 },
{ "fcmpq",	CMPFCC(3)|F3F(2, 0x35, 0x053), CMPFCC(~3)|F3F(~2, ~0x35, ~0x053),	 "9,V,R", F_FLOAT, 0, v9 },
{ "fcmpeq",	          F3F(2, 0x35, 0x057),            F3F(~2, ~0x35, ~0x057)|RD_G0,	 "V,R", F_FLOAT, 0, v8 },
{ "fcmpeq",	CMPFCC(0)|F3F(2, 0x35, 0x057), CMPFCC(~0)|F3F(~2, ~0x35, ~0x057),	 "6,V,R", F_FLOAT, 0, v9 },
{ "fcmpeq",	CMPFCC(1)|F3F(2, 0x35, 0x057), CMPFCC(~1)|F3F(~2, ~0x35, ~0x057),	 "7,V,R", F_FLOAT, 0, v9 },
{ "fcmpeq",	CMPFCC(2)|F3F(2, 0x35, 0x057), CMPFCC(~2)|F3F(~2, ~0x35, ~0x057),	 "8,V,R", F_FLOAT, 0, v9 },
{ "fcmpeq",	CMPFCC(3)|F3F(2, 0x35, 0x057), CMPFCC(~3)|F3F(~2, ~0x35, ~0x057),	 "9,V,R", F_FLOAT, 0, v9 },
{ "fcmpx",	          F3F(2, 0x35, 0x053),            F3F(~2, ~0x35, ~0x053)|RD_G0,	 "V,R", F_FLOAT|F_ALIAS, 0, v8 },
{ "fcmpx",	CMPFCC(0)|F3F(2, 0x35, 0x053), CMPFCC(~0)|F3F(~2, ~0x35, ~0x053),	 "6,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmpx",	CMPFCC(1)|F3F(2, 0x35, 0x053), CMPFCC(~1)|F3F(~2, ~0x35, ~0x053),	 "7,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmpx",	CMPFCC(2)|F3F(2, 0x35, 0x053), CMPFCC(~2)|F3F(~2, ~0x35, ~0x053),	 "8,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmpx",	CMPFCC(3)|F3F(2, 0x35, 0x053), CMPFCC(~3)|F3F(~2, ~0x35, ~0x053),	 "9,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmpex",	          F3F(2, 0x35, 0x057),            F3F(~2, ~0x35, ~0x057)|RD_G0,	 "V,R", F_FLOAT|F_ALIAS, 0, v8 },
{ "fcmpex",	CMPFCC(0)|F3F(2, 0x35, 0x057), CMPFCC(~0)|F3F(~2, ~0x35, ~0x057),	 "6,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmpex",	CMPFCC(1)|F3F(2, 0x35, 0x057), CMPFCC(~1)|F3F(~2, ~0x35, ~0x057),	 "7,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmpex",	CMPFCC(2)|F3F(2, 0x35, 0x057), CMPFCC(~2)|F3F(~2, ~0x35, ~0x057),	 "8,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmpex",	CMPFCC(3)|F3F(2, 0x35, 0x057), CMPFCC(~3)|F3F(~2, ~0x35, ~0x057),	 "9,V,R", F_FLOAT|F_ALIAS, 0, v9 },
{ "fcmps",	          F3F(2, 0x35, 0x051),            F3F(~2, ~0x35, ~0x051)|RD_G0, "e,f",   F_FLOAT, 0, v6 },
{ "fcmps",	CMPFCC(0)|F3F(2, 0x35, 0x051), CMPFCC(~0)|F3F(~2, ~0x35, ~0x051),	 "6,e,f", F_FLOAT, 0, v9 },
{ "fcmps",	CMPFCC(1)|F3F(2, 0x35, 0x051), CMPFCC(~1)|F3F(~2, ~0x35, ~0x051),	 "7,e,f", F_FLOAT, 0, v9 },
{ "fcmps",	CMPFCC(2)|F3F(2, 0x35, 0x051), CMPFCC(~2)|F3F(~2, ~0x35, ~0x051),	 "8,e,f", F_FLOAT, 0, v9 },
{ "fcmps",	CMPFCC(3)|F3F(2, 0x35, 0x051), CMPFCC(~3)|F3F(~2, ~0x35, ~0x051),	 "9,e,f", F_FLOAT, 0, v9 },
{ "fcmpes",	          F3F(2, 0x35, 0x055),            F3F(~2, ~0x35, ~0x055)|RD_G0, "e,f",   F_FLOAT, 0, v6 },
{ "fcmpes",	CMPFCC(0)|F3F(2, 0x35, 0x055), CMPFCC(~0)|F3F(~2, ~0x35, ~0x055),	 "6,e,f", F_FLOAT, 0, v9 },
{ "fcmpes",	CMPFCC(1)|F3F(2, 0x35, 0x055), CMPFCC(~1)|F3F(~2, ~0x35, ~0x055),	 "7,e,f", F_FLOAT, 0, v9 },
{ "fcmpes",	CMPFCC(2)|F3F(2, 0x35, 0x055), CMPFCC(~2)|F3F(~2, ~0x35, ~0x055),	 "8,e,f", F_FLOAT, 0, v9 },
{ "fcmpes",	CMPFCC(3)|F3F(2, 0x35, 0x055), CMPFCC(~3)|F3F(~2, ~0x35, ~0x055),	 "9,e,f", F_FLOAT, 0, v9 },

/* These Extended FPop (FIFO) instructions are new in the Fujitsu
   MB86934, replacing the CPop instructions from v6 and later
   processors.  */

#define EFPOP1_2(name, op, args) { name, F3F(2, 0x36, op), F3F(~2, ~0x36, ~op)|RS1_G0, args, 0, 0, sparclite }
#define EFPOP1_3(name, op, args) { name, F3F(2, 0x36, op), F3F(~2, ~0x36, ~op),        args, 0, 0, sparclite }
#define EFPOP2_2(name, op, args) { name, F3F(2, 0x37, op), F3F(~2, ~0x37, ~op)|RD_G0,  args, 0, 0, sparclite }

EFPOP1_2 ("efitod",	0x0c8, "f,H"),
EFPOP1_2 ("efitos",	0x0c4, "f,g"),
EFPOP1_2 ("efdtoi",	0x0d2, "B,g"),
EFPOP1_2 ("efstoi",	0x0d1, "f,g"),
EFPOP1_2 ("efstod",	0x0c9, "f,H"),
EFPOP1_2 ("efdtos",	0x0c6, "B,g"),
EFPOP1_2 ("efmovs",	0x001, "f,g"),
EFPOP1_2 ("efnegs",	0x005, "f,g"),
EFPOP1_2 ("efabss",	0x009, "f,g"),
EFPOP1_2 ("efsqrtd",	0x02a, "B,H"),
EFPOP1_2 ("efsqrts",	0x029, "f,g"),
EFPOP1_3 ("efaddd",	0x042, "v,B,H"),
EFPOP1_3 ("efadds",	0x041, "e,f,g"),
EFPOP1_3 ("efsubd",	0x046, "v,B,H"),
EFPOP1_3 ("efsubs",	0x045, "e,f,g"),
EFPOP1_3 ("efdivd",	0x04e, "v,B,H"),
EFPOP1_3 ("efdivs",	0x04d, "e,f,g"),
EFPOP1_3 ("efmuld",	0x04a, "v,B,H"),
EFPOP1_3 ("efmuls",	0x049, "e,f,g"),
EFPOP1_3 ("efsmuld",	0x069, "e,f,H"),
EFPOP2_2 ("efcmpd",	0x052, "v,B"),
EFPOP2_2 ("efcmped",	0x056, "v,B"),
EFPOP2_2 ("efcmps",	0x051, "e,f"),
EFPOP2_2 ("efcmpes",	0x055, "e,f"),

#undef EFPOP1_2
#undef EFPOP1_3
#undef EFPOP2_2

/* These are marked F_ALIAS, so that they won't conflict with sparclite insns
   present.  Otherwise, the F_ALIAS flag is ignored.  */
{ "cpop1",	F3(2, 0x36, 0), F3(~2, ~0x36, ~1), "[1+2],d", F_ALIAS, 0, v6notv9 },
{ "cpop2",	F3(2, 0x37, 0), F3(~2, ~0x37, ~1), "[1+2],d", F_ALIAS, 0, v6notv9 },

/* sparclet specific insns */

COMMUTEOP ("umac", 0x3e, sparclet),
COMMUTEOP ("smac", 0x3f, sparclet),
COMMUTEOP ("umacd", 0x2e, sparclet),
COMMUTEOP ("smacd", 0x2f, sparclet),
COMMUTEOP ("umuld", 0x09, sparclet),
COMMUTEOP ("smuld", 0x0d, sparclet),

{ "shuffle",	F3(2, 0x2d, 0), F3(~2, ~0x2d, ~0)|ASI(~0),	"1,2,d", 0, 0, sparclet },
{ "shuffle",	F3(2, 0x2d, 1), F3(~2, ~0x2d, ~1),		"1,i,d", 0, 0, sparclet },

/* The manual isn't completely accurate on these insns.  The `rs2' field is
   treated as being 6 bits to account for 6 bit immediates to cpush.  It is
   assumed that it is intended that bit 5 is 0 when rs2 contains a reg.  */
#define BIT5 (1<<5)
{ "crdcxt",	F3(2, 0x36, 0)|SLCPOP(4), F3(~2, ~0x36, ~0)|SLCPOP(~4)|BIT5|RS2(~0),	"U,d", 0, 0, sparclet },
{ "cwrcxt",	F3(2, 0x36, 0)|SLCPOP(3), F3(~2, ~0x36, ~0)|SLCPOP(~3)|BIT5|RS2(~0),	"1,u", 0, 0, sparclet },
{ "cpush",	F3(2, 0x36, 0)|SLCPOP(0), F3(~2, ~0x36, ~0)|SLCPOP(~0)|BIT5|RD(~0),	"1,2", 0, 0, sparclet },
{ "cpush",	F3(2, 0x36, 1)|SLCPOP(0), F3(~2, ~0x36, ~1)|SLCPOP(~0)|RD(~0),		"1,Y", 0, 0, sparclet },
{ "cpusha",	F3(2, 0x36, 0)|SLCPOP(1), F3(~2, ~0x36, ~0)|SLCPOP(~1)|BIT5|RD(~0),	"1,2", 0, 0, sparclet },
{ "cpusha",	F3(2, 0x36, 1)|SLCPOP(1), F3(~2, ~0x36, ~1)|SLCPOP(~1)|RD(~0),		"1,Y", 0, 0, sparclet },
{ "cpull",	F3(2, 0x36, 0)|SLCPOP(2), F3(~2, ~0x36, ~0)|SLCPOP(~2)|BIT5|RS1(~0)|RS2(~0), "d", 0, 0, sparclet },
#undef BIT5

/* sparclet coprocessor branch insns */
#define SLCBCC2(opcode, mask, lose) \
 { opcode, (mask), ANNUL|(lose), "l",    F_DELAYED|F_CONDBR, 0, sparclet }, \
 { opcode, (mask)|ANNUL, (lose), ",a l", F_DELAYED|F_CONDBR, 0, sparclet }
#define SLCBCC(opcode, mask) \
  SLCBCC2(opcode, F2(0, 7)|COND(mask), F2(~0, ~7)|COND(~(mask)))

/* cbn,cba can't be defined here because they're defined elsewhere and GAS
   requires all mnemonics of the same name to be consecutive.  */
/*SLCBCC("cbn", 0), - already defined */
SLCBCC("cbe", 1),
SLCBCC("cbf", 2),
SLCBCC("cbef", 3),
SLCBCC("cbr", 4),
SLCBCC("cber", 5),
SLCBCC("cbfr", 6),
SLCBCC("cbefr", 7),
/*SLCBCC("cba", 8), - already defined */
SLCBCC("cbne", 9),
SLCBCC("cbnf", 10),
SLCBCC("cbnef", 11),
SLCBCC("cbnr", 12),
SLCBCC("cbner", 13),
SLCBCC("cbnfr", 14),
SLCBCC("cbnefr", 15),

#undef SLCBCC2
#undef SLCBCC

{ "casa",	F3(3, 0x3c, 0), F3(~3, ~0x3c, ~0), "[1]A,2,d", 0, 0, v9 },
{ "casa",	F3(3, 0x3c, 1), F3(~3, ~0x3c, ~1), "[1]o,2,d", 0, 0, v9 },
{ "casxa",	F3(3, 0x3e, 0), F3(~3, ~0x3e, ~0), "[1]A,2,d", 0, 0, v9 },
{ "casxa",	F3(3, 0x3e, 1), F3(~3, ~0x3e, ~1), "[1]o,2,d", 0, 0, v9 },

/* v9 synthetic insns */
{ "iprefetch",	F2(0, 1)|(2<<20)|BPRED, F2(~0, ~1)|(1<<20)|ANNUL|COND(~0), "G", 0, 0, v9 }, /* bn,a,pt %xcc,label */
{ "signx",	F3(2, 0x27, 0), F3(~2, ~0x27, ~0)|(1<<12)|ASI(~0)|RS2_G0, "1,d", F_ALIAS, 0, v9 }, /* sra rs1,%g0,rd */
{ "signx",	F3(2, 0x27, 0), F3(~2, ~0x27, ~0)|(1<<12)|ASI(~0)|RS2_G0, "r", F_ALIAS, 0, v9 }, /* sra rd,%g0,rd */
{ "clruw",	F3(2, 0x26, 0), F3(~2, ~0x26, ~0)|(1<<12)|ASI(~0)|RS2_G0, "1,d", F_ALIAS, 0, v9 }, /* srl rs1,%g0,rd */
{ "clruw",	F3(2, 0x26, 0), F3(~2, ~0x26, ~0)|(1<<12)|ASI(~0)|RS2_G0, "r", F_ALIAS, 0, v9 }, /* srl rd,%g0,rd */
{ "cas",	F3(3, 0x3c, 0)|ASI(0x80), F3(~3, ~0x3c, ~0)|ASI(~0x80), "[1],2,d", F_ALIAS, 0, v9 }, /* casa [rs1]ASI_P,rs2,rd */
{ "casl",	F3(3, 0x3c, 0)|ASI(0x88), F3(~3, ~0x3c, ~0)|ASI(~0x88), "[1],2,d", F_ALIAS, 0, v9 }, /* casa [rs1]ASI_P_L,rs2,rd */
{ "casx",	F3(3, 0x3e, 0)|ASI(0x80), F3(~3, ~0x3e, ~0)|ASI(~0x80), "[1],2,d", F_ALIAS, 0, v9 }, /* casxa [rs1]ASI_P,rs2,rd */
{ "casxl",	F3(3, 0x3e, 0)|ASI(0x88), F3(~3, ~0x3e, ~0)|ASI(~0x88), "[1],2,d", F_ALIAS, 0, v9 }, /* casxa [rs1]ASI_P_L,rs2,rd */

/* Ultrasparc extensions */
{ "shutdown",	F3F(2, 0x36, 0x080), F3F(~2, ~0x36, ~0x080)|RD_G0|RS1_G0|RS2_G0, "", 0, HWCAP_VIS, v9a },

/* FIXME: Do we want to mark these as F_FLOAT, or something similar?  */
{ "fpadd16",	F3F(2, 0x36, 0x050), F3F(~2, ~0x36, ~0x050), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fpadd16s",	F3F(2, 0x36, 0x051), F3F(~2, ~0x36, ~0x051), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fpadd32",	F3F(2, 0x36, 0x052), F3F(~2, ~0x36, ~0x052), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fpadd32s",	F3F(2, 0x36, 0x053), F3F(~2, ~0x36, ~0x053), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fpsub16",	F3F(2, 0x36, 0x054), F3F(~2, ~0x36, ~0x054), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fpsub16s",	F3F(2, 0x36, 0x055), F3F(~2, ~0x36, ~0x055), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fpsub32",	F3F(2, 0x36, 0x056), F3F(~2, ~0x36, ~0x056), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fpsub32s",	F3F(2, 0x36, 0x057), F3F(~2, ~0x36, ~0x057), "e,f,g", 0, HWCAP_VIS, v9a },

{ "fpack32",	F3F(2, 0x36, 0x03a), F3F(~2, ~0x36, ~0x03a), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fpack16",	F3F(2, 0x36, 0x03b), F3F(~2, ~0x36, ~0x03b)|RS1_G0, "B,g", 0, HWCAP_VIS, v9a },
{ "fpackfix",	F3F(2, 0x36, 0x03d), F3F(~2, ~0x36, ~0x03d)|RS1_G0, "B,g", 0, HWCAP_VIS, v9a },
{ "fexpand",	F3F(2, 0x36, 0x04d), F3F(~2, ~0x36, ~0x04d)|RS1_G0, "f,H", 0, HWCAP_VIS, v9a },
{ "fpmerge",	F3F(2, 0x36, 0x04b), F3F(~2, ~0x36, ~0x04b), "e,f,H", 0, HWCAP_VIS, v9a },

/* Note that the mixing of 32/64 bit regs is intentional.  */
{ "fmul8x16",		F3F(2, 0x36, 0x031), F3F(~2, ~0x36, ~0x031), "e,B,H", 0, HWCAP_VIS, v9a },
{ "fmul8x16au",		F3F(2, 0x36, 0x033), F3F(~2, ~0x36, ~0x033), "e,f,H", 0, HWCAP_VIS, v9a },
{ "fmul8x16al",		F3F(2, 0x36, 0x035), F3F(~2, ~0x36, ~0x035), "e,f,H", 0, HWCAP_VIS, v9a },
{ "fmul8sux16",		F3F(2, 0x36, 0x036), F3F(~2, ~0x36, ~0x036), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fmul8ulx16",		F3F(2, 0x36, 0x037), F3F(~2, ~0x36, ~0x037), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fmuld8sux16",	F3F(2, 0x36, 0x038), F3F(~2, ~0x36, ~0x038), "e,f,H", 0, HWCAP_VIS, v9a },
{ "fmuld8ulx16",	F3F(2, 0x36, 0x039), F3F(~2, ~0x36, ~0x039), "e,f,H", 0, HWCAP_VIS, v9a },

{ "alignaddr",	F3F(2, 0x36, 0x018), F3F(~2, ~0x36, ~0x018), "1,2,d", 0, HWCAP_VIS, v9a },
{ "alignaddrl",	F3F(2, 0x36, 0x01a), F3F(~2, ~0x36, ~0x01a), "1,2,d", 0, HWCAP_VIS, v9a },
{ "faligndata",	F3F(2, 0x36, 0x048), F3F(~2, ~0x36, ~0x048), "v,B,H", 0, HWCAP_VIS, v9a },

{ "fzero",	F3F(2, 0x36, 0x060), F3F(~2, ~0x36, ~0x060), "H", 0, HWCAP_VIS, v9a },
{ "fzeros",	F3F(2, 0x36, 0x061), F3F(~2, ~0x36, ~0x061), "g", 0, HWCAP_VIS, v9a },
{ "fone",	F3F(2, 0x36, 0x07e), F3F(~2, ~0x36, ~0x07e), "H", 0, HWCAP_VIS, v9a },
{ "fones",	F3F(2, 0x36, 0x07f), F3F(~2, ~0x36, ~0x07f), "g", 0, HWCAP_VIS, v9a },
{ "fsrc1",	F3F(2, 0x36, 0x074), F3F(~2, ~0x36, ~0x074), "v,H", 0, HWCAP_VIS, v9a },
{ "fsrc1s",	F3F(2, 0x36, 0x075), F3F(~2, ~0x36, ~0x075), "e,g", 0, HWCAP_VIS, v9a },
{ "fsrc2",	F3F(2, 0x36, 0x078), F3F(~2, ~0x36, ~0x078), "B,H", 0, HWCAP_VIS, v9a },
{ "fsrc2s",	F3F(2, 0x36, 0x079), F3F(~2, ~0x36, ~0x079), "f,g", 0, HWCAP_VIS, v9a },
{ "fnot1",	F3F(2, 0x36, 0x06a), F3F(~2, ~0x36, ~0x06a), "v,H", 0, HWCAP_VIS, v9a },
{ "fnot1s",	F3F(2, 0x36, 0x06b), F3F(~2, ~0x36, ~0x06b), "e,g", 0, HWCAP_VIS, v9a },
{ "fnot2",	F3F(2, 0x36, 0x066), F3F(~2, ~0x36, ~0x066), "B,H", 0, HWCAP_VIS, v9a },
{ "fnot2s",	F3F(2, 0x36, 0x067), F3F(~2, ~0x36, ~0x067), "f,g", 0, HWCAP_VIS, v9a },
{ "for",	F3F(2, 0x36, 0x07c), F3F(~2, ~0x36, ~0x07c), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fors",	F3F(2, 0x36, 0x07d), F3F(~2, ~0x36, ~0x07d), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fnor",	F3F(2, 0x36, 0x062), F3F(~2, ~0x36, ~0x062), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fnors",	F3F(2, 0x36, 0x063), F3F(~2, ~0x36, ~0x063), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fand",	F3F(2, 0x36, 0x070), F3F(~2, ~0x36, ~0x070), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fands",	F3F(2, 0x36, 0x071), F3F(~2, ~0x36, ~0x071), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fnand",	F3F(2, 0x36, 0x06e), F3F(~2, ~0x36, ~0x06e), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fnands",	F3F(2, 0x36, 0x06f), F3F(~2, ~0x36, ~0x06f), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fxor",	F3F(2, 0x36, 0x06c), F3F(~2, ~0x36, ~0x06c), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fxors",	F3F(2, 0x36, 0x06d), F3F(~2, ~0x36, ~0x06d), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fxnor",	F3F(2, 0x36, 0x072), F3F(~2, ~0x36, ~0x072), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fxnors",	F3F(2, 0x36, 0x073), F3F(~2, ~0x36, ~0x073), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fornot1",	F3F(2, 0x36, 0x07a), F3F(~2, ~0x36, ~0x07a), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fornot1s",	F3F(2, 0x36, 0x07b), F3F(~2, ~0x36, ~0x07b), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fornot2",	F3F(2, 0x36, 0x076), F3F(~2, ~0x36, ~0x076), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fornot2s",	F3F(2, 0x36, 0x077), F3F(~2, ~0x36, ~0x077), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fandnot1",	F3F(2, 0x36, 0x068), F3F(~2, ~0x36, ~0x068), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fandnot1s",	F3F(2, 0x36, 0x069), F3F(~2, ~0x36, ~0x069), "e,f,g", 0, HWCAP_VIS, v9a },
{ "fandnot2",	F3F(2, 0x36, 0x064), F3F(~2, ~0x36, ~0x064), "v,B,H", 0, HWCAP_VIS, v9a },
{ "fandnot2s",	F3F(2, 0x36, 0x065), F3F(~2, ~0x36, ~0x065), "e,f,g", 0, HWCAP_VIS, v9a },

{ "fcmpgt16",	F3F(2, 0x36, 0x028), F3F(~2, ~0x36, ~0x028), "v,B,d", 0, HWCAP_VIS, v9a },
{ "fcmpgt32",	F3F(2, 0x36, 0x02c), F3F(~2, ~0x36, ~0x02c), "v,B,d", 0, HWCAP_VIS, v9a },
{ "fcmple16",	F3F(2, 0x36, 0x020), F3F(~2, ~0x36, ~0x020), "v,B,d", 0, HWCAP_VIS, v9a },
{ "fcmple32",	F3F(2, 0x36, 0x024), F3F(~2, ~0x36, ~0x024), "v,B,d", 0, HWCAP_VIS, v9a },
{ "fcmpne16",	F3F(2, 0x36, 0x022), F3F(~2, ~0x36, ~0x022), "v,B,d", 0, HWCAP_VIS, v9a },
{ "fcmpne32",	F3F(2, 0x36, 0x026), F3F(~2, ~0x36, ~0x026), "v,B,d", 0, HWCAP_VIS, v9a },
{ "fcmpeq16",	F3F(2, 0x36, 0x02a), F3F(~2, ~0x36, ~0x02a), "v,B,d", 0, HWCAP_VIS, v9a },
{ "fcmpeq32",	F3F(2, 0x36, 0x02e), F3F(~2, ~0x36, ~0x02e), "v,B,d", 0, HWCAP_VIS, v9a },

{ "edge8",	F3F(2, 0x36, 0x000), F3F(~2, ~0x36, ~0x000), "1,2,d", 0, HWCAP_VIS, v9a },
{ "edge8l",	F3F(2, 0x36, 0x002), F3F(~2, ~0x36, ~0x002), "1,2,d", 0, HWCAP_VIS, v9a },
{ "edge16",	F3F(2, 0x36, 0x004), F3F(~2, ~0x36, ~0x004), "1,2,d", 0, HWCAP_VIS, v9a },
{ "edge16l",	F3F(2, 0x36, 0x006), F3F(~2, ~0x36, ~0x006), "1,2,d", 0, HWCAP_VIS, v9a },
{ "edge32",	F3F(2, 0x36, 0x008), F3F(~2, ~0x36, ~0x008), "1,2,d", 0, HWCAP_VIS, v9a },
{ "edge32l",	F3F(2, 0x36, 0x00a), F3F(~2, ~0x36, ~0x00a), "1,2,d", 0, HWCAP_VIS, v9a },

{ "pdist",	F3F(2, 0x36, 0x03e), F3F(~2, ~0x36, ~0x03e), "v,B,H", 0, HWCAP_VIS, v9a },

{ "array8",	F3F(2, 0x36, 0x010), F3F(~2, ~0x36, ~0x010), "1,2,d", 0, HWCAP_VIS, v9a },
{ "array16",	F3F(2, 0x36, 0x012), F3F(~2, ~0x36, ~0x012), "1,2,d", 0, HWCAP_VIS, v9a },
{ "array32",	F3F(2, 0x36, 0x014), F3F(~2, ~0x36, ~0x014), "1,2,d", 0, HWCAP_VIS, v9a },

/* Cheetah instructions */
{ "edge8n",    F3F(2, 0x36, 0x001), F3F(~2, ~0x36, ~0x001), "1,2,d", HWCAP_VIS2, 0, v9b },
{ "edge8ln",   F3F(2, 0x36, 0x003), F3F(~2, ~0x36, ~0x003), "1,2,d", HWCAP_VIS2, 0, v9b },
{ "edge16n",   F3F(2, 0x36, 0x005), F3F(~2, ~0x36, ~0x005), "1,2,d", HWCAP_VIS2, 0, v9b },
{ "edge16ln",  F3F(2, 0x36, 0x007), F3F(~2, ~0x36, ~0x007), "1,2,d", HWCAP_VIS2, 0, v9b },
{ "edge32n",   F3F(2, 0x36, 0x009), F3F(~2, ~0x36, ~0x009), "1,2,d", HWCAP_VIS2, 0, v9b },
{ "edge32ln",  F3F(2, 0x36, 0x00b), F3F(~2, ~0x36, ~0x00b), "1,2,d", HWCAP_VIS2, 0, v9b },

{ "bmask",     F3F(2, 0x36, 0x019), F3F(~2, ~0x36, ~0x019), "1,2,d", HWCAP_VIS2, 0, v9b },
{ "bshuffle",  F3F(2, 0x36, 0x04c), F3F(~2, ~0x36, ~0x04c), "v,B,H", HWCAP_VIS2, 0, v9b },

{ "siam",      F3F(2, 0x36, 0x081), F3F(~2, ~0x36, ~0x081)|RD_G0|RS1_G0|RS2(~7), "3", HWCAP_VIS2, 0, v9b },

{ "commit",	F3(2, 0x3e, 0)|RD(30), F3(~2, ~0x3e, ~0)|RD(~30)|RS1_G0|SIMM13(~0), "", 0, HWCAP_TRANS, v9b },
{ "fnadds",	F3F(2, 0x34, 0x051), F3F(~2, ~0x34, ~0x051), "e,f,g", F_FLOAT, HWCAP_HPC, v9b },
{ "fnaddd",	F3F(2, 0x34, 0x052), F3F(~2, ~0x34, ~0x052), "v,B,H", F_FLOAT, HWCAP_HPC, v9b },
{ "fnmuls",	F3F(2, 0x34, 0x059), F3F(~2, ~0x34, ~0x059), "e,f,g", F_FLOAT, HWCAP_HPC, v9b },
{ "fnmuld",	F3F(2, 0x34, 0x05a), F3F(~2, ~0x34, ~0x05a), "v,B,H", F_FLOAT, HWCAP_HPC, v9b },
{ "fhadds",	F3F(2, 0x34, 0x061), F3F(~2, ~0x34, ~0x061), "e,f,g", F_FLOAT, HWCAP_HPC, v9b },
{ "fhaddd",	F3F(2, 0x34, 0x062), F3F(~2, ~0x34, ~0x062), "v,B,H", F_FLOAT, HWCAP_HPC, v9b },
{ "fhsubs",	F3F(2, 0x34, 0x065), F3F(~2, ~0x34, ~0x065), "e,f,g", F_FLOAT, HWCAP_HPC, v9b },
{ "fhsubd",	F3F(2, 0x34, 0x066), F3F(~2, ~0x34, ~0x066), "v,B,H", F_FLOAT, HWCAP_HPC, v9b },
{ "fnhadds",	F3F(2, 0x34, 0x071), F3F(~2, ~0x34, ~0x071), "e,f,g", F_FLOAT, HWCAP_HPC, v9b },
{ "fnhaddd",	F3F(2, 0x34, 0x072), F3F(~2, ~0x34, ~0x072), "v,B,H", F_FLOAT, HWCAP_HPC, v9b },
{ "fnsmuld",	F3F(2, 0x34, 0x079), F3F(~2, ~0x34, ~0x079), "e,f,H", F_FLOAT, HWCAP_HPC, v9b },
{ "fpmaddx",	F3(2, 0x37, 0)|OPF_LOW4(0), F3(~2, ~0x37, 0)|OPF_LOW4(~0), "v,B,5,H", F_FLOAT, HWCAP_IMA, v9b },
{ "fmadds",	F3(2, 0x37, 0)|OPF_LOW4(1), F3(~2, ~0x37, 0)|OPF_LOW4(~1), "e,f,4,g", F_FLOAT, HWCAP_FMAF, v9b },
{ "fmaddd",	F3(2, 0x37, 0)|OPF_LOW4(2), F3(~2, ~0x37, 0)|OPF_LOW4(~2), "v,B,5,H", F_FLOAT, HWCAP_FMAF, v9b },
{ "fpmaddxhi",	F3(2, 0x37, 0)|OPF_LOW4(4), F3(~2, ~0x37, 0)|OPF_LOW4(~4), "v,B,5,H", F_FLOAT, HWCAP_IMA, v9b },
{ "fmsubs",	F3(2, 0x37, 0)|OPF_LOW4(5), F3(~2, ~0x37, 0)|OPF_LOW4(~5), "e,f,4,g", F_FLOAT, HWCAP_FMAF, v9b },
{ "fmsubd",	F3(2, 0x37, 0)|OPF_LOW4(6), F3(~2, ~0x37, 0)|OPF_LOW4(~6), "v,B,5,H", F_FLOAT, HWCAP_FMAF, v9b },
{ "fnmsubs",	F3(2, 0x37, 0)|OPF_LOW4(9), F3(~2, ~0x37, 0)|OPF_LOW4(~9), "e,f,4,g", F_FLOAT, HWCAP_FMAF, v9b },
{ "fnmsubd",	F3(2, 0x37, 0)|OPF_LOW4(10), F3(~2, ~0x37, 0)|OPF_LOW4(~10), "v,B,5,H", F_FLOAT, HWCAP_FMAF, v9b },
{ "fnmadds",	F3(2, 0x37, 0)|OPF_LOW4(13), F3(~2, ~0x37, 0)|OPF_LOW4(~13), "e,f,4,g", F_FLOAT, HWCAP_FMAF, v9b },
{ "fnmaddd",	F3(2, 0x37, 0)|OPF_LOW4(14), F3(~2, ~0x37, 0)|OPF_LOW4(~14), "v,B,5,H", F_FLOAT, HWCAP_FMAF, v9b },
{ "fumadds",	F3(2, 0x3f, 0)|OPF_LOW4(1), F3(~2, ~0x3f, 0)|OPF_LOW4(~1), "e,f,4,g", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "fumaddd",	F3(2, 0x3f, 0)|OPF_LOW4(2), F3(~2, ~0x3f, 0)|OPF_LOW4(~2), "v,B,5,H", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "fumsubs",	F3(2, 0x3f, 0)|OPF_LOW4(5), F3(~2, ~0x3f, 0)|OPF_LOW4(~5), "e,f,4,g", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "fumsubd",	F3(2, 0x3f, 0)|OPF_LOW4(6), F3(~2, ~0x3f, 0)|OPF_LOW4(~6), "v,B,5,H", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "fnumsubs",	F3(2, 0x3f, 0)|OPF_LOW4(9), F3(~2, ~0x3f, 0)|OPF_LOW4(~9), "e,f,4,g", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "fnumsubd",	F3(2, 0x3f, 0)|OPF_LOW4(10), F3(~2, ~0x3f, 0)|OPF_LOW4(~10), "v,B,5,H", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "fnumadds",	F3(2, 0x3f, 0)|OPF_LOW4(13), F3(~2, ~0x3f, 0)|OPF_LOW4(~13), "e,f,4,g", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "fnumaddd",	F3(2, 0x3f, 0)|OPF_LOW4(14), F3(~2, ~0x3f, 0)|OPF_LOW4(~14), "v,B,5,H", F_FLOAT, HWCAP_FJFMAU, v9b },
{ "addxc",	F3F(2, 0x36, 0x011), F3F(~2, ~0x36, ~0x011), "1,2,d", 0, HWCAP_VIS3, v9b },
{ "addxccc",	F3F(2, 0x36, 0x013), F3F(~2, ~0x36, ~0x013), "1,2,d", 0, HWCAP_VIS3, v9b },
{ "random",	F3F(2, 0x36, 0x015), F3F(~2, ~0x36, ~0x015), "H", F_FLOAT, HWCAP_RANDOM, v9b },
{ "umulxhi",	F3F(2, 0x36, 0x016), F3F(~2, ~0x36, ~0x016), "1,2,d", 0, HWCAP_VIS3, v9b },
{ "lzd",	F3F(2, 0x36, 0x017), F3F(~2, ~0x36, ~0x017), "2,d", 0, HWCAP_VIS3, v9b },
{ "cmask8",	F3F(2, 0x36, 0x01b), F3F(~2, ~0x36, ~0x01b), "2", 0, HWCAP_VIS3, v9b },
{ "cmask16",	F3F(2, 0x36, 0x01d), F3F(~2, ~0x36, ~0x01d), "2", 0, HWCAP_VIS3, v9b },
{ "cmask32",	F3F(2, 0x36, 0x01f), F3F(~2, ~0x36, ~0x01f), "2", 0, HWCAP_VIS3, v9b },
{ "fsll16",	F3F(2, 0x36, 0x021), F3F(~2, ~0x36, ~0x021), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fsrl16",	F3F(2, 0x36, 0x023), F3F(~2, ~0x36, ~0x023), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fsll32",	F3F(2, 0x36, 0x025), F3F(~2, ~0x36, ~0x025), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fsrl32",	F3F(2, 0x36, 0x027), F3F(~2, ~0x36, ~0x027), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fslas16",	F3F(2, 0x36, 0x029), F3F(~2, ~0x36, ~0x029), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fsra16",	F3F(2, 0x36, 0x02b), F3F(~2, ~0x36, ~0x02b), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fslas32",	F3F(2, 0x36, 0x02d), F3F(~2, ~0x36, ~0x02d), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fsra32",	F3F(2, 0x36, 0x02f), F3F(~2, ~0x36, ~0x02f), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "pdistn",	F3F(2, 0x36, 0x03f), F3F(~2, ~0x36, ~0x03f), "v,B,d", 0, HWCAP_VIS3, v9b },
{ "fmean16",	F3F(2, 0x36, 0x040), F3F(~2, ~0x36, ~0x040), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fpadd64",	F3F(2, 0x36, 0x042), F3F(~2, ~0x36, ~0x042), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fchksm16",	F3F(2, 0x36, 0x044), F3F(~2, ~0x36, ~0x044), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fpsub64",	F3F(2, 0x36, 0x046), F3F(~2, ~0x36, ~0x046), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fpadds16",	F3F(2, 0x36, 0x058), F3F(~2, ~0x36, ~0x058), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fpadds16s",	F3F(2, 0x36, 0x059), F3F(~2, ~0x36, ~0x059), "e,f,g", 0, HWCAP_VIS3, v9b },
{ "fpadds32",	F3F(2, 0x36, 0x05a), F3F(~2, ~0x36, ~0x05a), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fpadds32s",	F3F(2, 0x36, 0x05b), F3F(~2, ~0x36, ~0x05b), "e,f,g", 0, HWCAP_VIS3, v9b },
{ "fpsubs16",	F3F(2, 0x36, 0x05c), F3F(~2, ~0x36, ~0x05c), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fpsubs16s",	F3F(2, 0x36, 0x05d), F3F(~2, ~0x36, ~0x05d), "e,f,g", 0, HWCAP_VIS3, v9b },
{ "fpsubs32",	F3F(2, 0x36, 0x05e), F3F(~2, ~0x36, ~0x05e), "v,B,H", 0, HWCAP_VIS3, v9b },
{ "fpsubs32s",	F3F(2, 0x36, 0x05f), F3F(~2, ~0x36, ~0x05f), "e,f,g", 0, HWCAP_VIS3, v9b },
{ "movdtox",	F3F(2, 0x36, 0x110), F3F(~2, ~0x36, ~0x110), "B,d", F_FLOAT, HWCAP_VIS3, v9b },
{ "movstouw",	F3F(2, 0x36, 0x111), F3F(~2, ~0x36, ~0x111), "f,d", F_FLOAT, HWCAP_VIS3, v9b },
{ "movstosw",	F3F(2, 0x36, 0x113), F3F(~2, ~0x36, ~0x113), "f,d", F_FLOAT, HWCAP_VIS3, v9b },
{ "movxtod",	F3F(2, 0x36, 0x118), F3F(~2, ~0x36, ~0x118), "2,H", F_FLOAT, HWCAP_VIS3, v9b },
{ "movwtos",	F3F(2, 0x36, 0x119), F3F(~2, ~0x36, ~0x119), "2,g", F_FLOAT, HWCAP_VIS3, v9b },
{ "xmulx",	F3F(2, 0x36, 0x115), F3F(~2, ~0x36, ~0x115), "1,2,d", 0, HWCAP_VIS3, v9b },
{ "xmulxhi",	F3F(2, 0x36, 0x116), F3F(~2, ~0x36, ~0x116), "1,2,d", 0, HWCAP_VIS3, v9b },
{ "fucmple8",	F3F(2, 0x36, 0x120), F3F(~2, ~0x36, ~0x120), "v,B,d", 0, HWCAP_VIS3, v9b },
{ "fucmpne8",	F3F(2, 0x36, 0x122), F3F(~2, ~0x36, ~0x122), "v,B,d", 0, HWCAP_VIS3, v9b },
{ "fucmpgt8",	F3F(2, 0x36, 0x128), F3F(~2, ~0x36, ~0x128), "v,B,d", 0, HWCAP_VIS3, v9b },
{ "fucmpeq8",	F3F(2, 0x36, 0x12a), F3F(~2, ~0x36, ~0x12a), "v,B,d", 0, HWCAP_VIS3, v9b },
{"aes_kexpand0",F3F(2, 0x36, 0x130), F3F(~2, ~0x36, ~0x130), "v,B,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_kexpand2",F3F(2, 0x36, 0x131), F3F(~2, ~0x36, ~0x131), "v,B,H", F_FLOAT, HWCAP_AES, v9b },
{ "des_ip",     F3F(2, 0x36, 0x134), F3F(~2, ~0x36, ~0x134), "v,H", F_FLOAT, HWCAP_DES, v9b },
{ "des_iip",    F3F(2, 0x36, 0x135), F3F(~2, ~0x36, ~0x135), "v,H", F_FLOAT, HWCAP_DES, v9b },
{ "des_kexpand",F3F(2, 0x36, 0x136), F3F(~2, ~0x36, ~0x136), "v,X,H", F_FLOAT, HWCAP_DES, v9b },
{"kasumi_fi_fi",F3F(2, 0x36, 0x138), F3F(~2, ~0x36, ~0x138), "v,B,H", F_FLOAT, HWCAP_KASUMI, v9b },
{ "camellia_fi",F3F(2, 0x36, 0x13c), F3F(~2, ~0x36, ~0x13c), "v,B,H", F_FLOAT, HWCAP_CAMELLIA, v9b },
{"camellia_fli",F3F(2, 0x36, 0x13d), F3F(~2, ~0x36, ~0x13d), "v,B,H", F_FLOAT, HWCAP_CAMELLIA, v9b },
{ "md5",        F3F(2, 0x36, 0x140), F3F(~2, ~0x36, ~0x140), "", F_FLOAT, HWCAP_MD5, v9b },
{ "sha1",       F3F(2, 0x36, 0x141), F3F(~2, ~0x36, ~0x141), "", F_FLOAT, HWCAP_SHA1, v9b },
{ "sha256",     F3F(2, 0x36, 0x142), F3F(~2, ~0x36, ~0x142), "", F_FLOAT, HWCAP_SHA256, v9b },
{ "sha512",     F3F(2, 0x36, 0x143), F3F(~2, ~0x36, ~0x143), "", F_FLOAT, HWCAP_SHA512, v9b },
{ "crc32c",     F3F(2, 0x36, 0x147), F3F(~2, ~0x36, ~0x147), "v,B,H", F_FLOAT, HWCAP_CRC32C, v9b },
{ "mpmul",      F3F(2, 0x36, 0x148), F3F(~2, ~0x36, ~0x148), "X", F_FLOAT, HWCAP_MPMUL, v9b },
{ "montmul",    F3F(2, 0x36, 0x149), F3F(~2, ~0x36, ~0x149), "X", F_FLOAT, HWCAP_MONT, v9b },
{ "montsqr",    F3F(2, 0x36, 0x14a), F3F(~2, ~0x36, ~0x14a), "X", F_FLOAT, HWCAP_MONT, v9b },
{"aes_eround01",  F3F4(2, 0x19, 0), F3F4(~2, ~0x19, ~0), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_eround23",  F3F4(2, 0x19, 1), F3F4(~2, ~0x19, ~1), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_dround01",  F3F4(2, 0x19, 2), F3F4(~2, ~0x19, ~2), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_dround23",  F3F4(2, 0x19, 3), F3F4(~2, ~0x19, ~3), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_eround01_l",F3F4(2, 0x19, 4), F3F4(~2, ~0x19, ~4), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_eround23_l",F3F4(2, 0x19, 5), F3F4(~2, ~0x19, ~5), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_dround01_l",F3F4(2, 0x19, 6), F3F4(~2, ~0x19, ~6), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_dround23_l",F3F4(2, 0x19, 7), F3F4(~2, ~0x19, ~7), "v,B,5,H", F_FLOAT, HWCAP_AES, v9b },
{"aes_kexpand1",  F3F4(2, 0x19, 8), F3F4(~2, ~0x19, ~8), "v,B,),H", F_FLOAT, HWCAP_AES, v9b },
{"des_round",     F3F4(2, 0x19, 9), F3F4(~2, ~0x19, ~9), "v,B,5,H", F_FLOAT, HWCAP_DES, v9b },
{"kasumi_fl_xor", F3F4(2, 0x19, 10), F3F4(~2, ~0x19, ~10), "v,B,5,H", F_FLOAT, HWCAP_KASUMI, v9b },
{"kasumi_fi_xor", F3F4(2, 0x19, 11), F3F4(~2, ~0x19, ~11), "v,B,5,H", F_FLOAT, HWCAP_KASUMI, v9b },
{"camellia_f",    F3F4(2, 0x19, 12), F3F4(~2, ~0x19, ~12), "v,B,5,H", F_FLOAT, HWCAP_CAMELLIA, v9b },
{ "flcmps",	CMPFCC(0)|F3F(2, 0x36, 0x151), CMPFCC(~0)|F3F(~2, ~0x36, ~0x151), "6,e,f", F_FLOAT, HWCAP_HPC, v9b },
{ "flcmps",	CMPFCC(1)|F3F(2, 0x36, 0x151), CMPFCC(~1)|F3F(~2, ~0x36, ~0x151), "7,e,f", F_FLOAT, HWCAP_HPC, v9b },
{ "flcmps",	CMPFCC(2)|F3F(2, 0x36, 0x151), CMPFCC(~2)|F3F(~2, ~0x36, ~0x151), "8,e,f", F_FLOAT, HWCAP_HPC, v9b },
{ "flcmps",	CMPFCC(3)|F3F(2, 0x36, 0x151), CMPFCC(~3)|F3F(~2, ~0x36, ~0x151), "9,e,f", F_FLOAT, HWCAP_HPC, v9b },
{ "flcmpd",	CMPFCC(0)|F3F(2, 0x36, 0x152), CMPFCC(~0)|F3F(~2, ~0x36, ~0x152), "6,v,B", F_FLOAT, HWCAP_HPC, v9b },
{ "flcmpd",	CMPFCC(1)|F3F(2, 0x36, 0x152), CMPFCC(~1)|F3F(~2, ~0x36, ~0x152), "7,v,B", F_FLOAT, HWCAP_HPC, v9b },
{ "flcmpd",	CMPFCC(2)|F3F(2, 0x36, 0x152), CMPFCC(~2)|F3F(~2, ~0x36, ~0x152), "8,v,B", F_FLOAT, HWCAP_HPC, v9b },
{ "flcmpd",	CMPFCC(3)|F3F(2, 0x36, 0x152), CMPFCC(~3)|F3F(~2, ~0x36, ~0x152), "9,v,B", F_FLOAT, HWCAP_HPC, v9b },

/* More v9 specific insns, these need to come last so they do not clash
   with v9a instructions such as "edge8" which looks like impdep1. */

#define IMPDEP(name, code) \
{ name,	F3(2, code, 0), F3(~2, ~code, ~0)|ASI(~0), "1,2,d", 0, 0, v9notv9a }, \
{ name,	F3(2, code, 1), F3(~2, ~code, ~1),	   "1,i,d", 0, 0, v9notv9a }, \
{ name, F3(2, code, 0), F3(~2, ~code, ~0),         "x,1,2,d", 0, 0, v9notv9a }, \
{ name, F3(2, code, 0), F3(~2, ~code, ~0),         "x,e,f,g", 0, 0, v9notv9a }

IMPDEP ("impdep1", 0x36),
IMPDEP ("impdep2", 0x37),

#undef IMPDEP

};

const int sparc_num_opcodes = ((sizeof sparc_opcodes)/(sizeof sparc_opcodes[0]));

/* Utilities for argument parsing.  */

typedef struct
{
  int value;
  const char *name;
} arg;

/* Look up NAME in TABLE.  */

static int
lookup_name (const arg *table, const char *name)
{
  const arg *p;

  for (p = table; p->name; ++p)
    if (strcmp (name, p->name) == 0)
      return p->value;

  return -1;
}

/* Look up VALUE in TABLE.  */

static const char *
lookup_value (const arg *table, int value)
{
  const arg *p;

  for (p = table; p->name; ++p)
    if (value == p->value)
      return p->name;

  return NULL;
}

/* Handle ASI's.  */

static arg asi_table[] =
{
  /* These are in the v9 architecture manual.  */
  /* The shorter versions appear first, they're here because Sun's as has them.
     Sun's as uses #ASI_P_L instead of #ASI_PL (which appears in the
     UltraSPARC architecture manual).  */
  { 0x04, "#ASI_N" },
  { 0x0c, "#ASI_N_L" },
  { 0x10, "#ASI_AIUP" },
  { 0x11, "#ASI_AIUS" },
  { 0x18, "#ASI_AIUP_L" },
  { 0x19, "#ASI_AIUS_L" },
  { 0x80, "#ASI_P" },
  { 0x81, "#ASI_S" },
  { 0x82, "#ASI_PNF" },
  { 0x83, "#ASI_SNF" },
  { 0x88, "#ASI_P_L" },
  { 0x89, "#ASI_S_L" },
  { 0x8a, "#ASI_PNF_L" },
  { 0x8b, "#ASI_SNF_L" },
  { 0x04, "#ASI_NUCLEUS" },
  { 0x0c, "#ASI_NUCLEUS_LITTLE" },
  { 0x10, "#ASI_AS_IF_USER_PRIMARY" },
  { 0x11, "#ASI_AS_IF_USER_SECONDARY" },
  { 0x18, "#ASI_AS_IF_USER_PRIMARY_LITTLE" },
  { 0x19, "#ASI_AS_IF_USER_SECONDARY_LITTLE" },
  { 0x80, "#ASI_PRIMARY" },
  { 0x81, "#ASI_SECONDARY" },
  { 0x82, "#ASI_PRIMARY_NOFAULT" },
  { 0x83, "#ASI_SECONDARY_NOFAULT" },
  { 0x88, "#ASI_PRIMARY_LITTLE" },
  { 0x89, "#ASI_SECONDARY_LITTLE" },
  { 0x8a, "#ASI_PRIMARY_NOFAULT_LITTLE" },
  { 0x8b, "#ASI_SECONDARY_NOFAULT_LITTLE" },
  /* These are UltraSPARC and Niagara extensions.  */
  { 0x14, "#ASI_PHYS_USE_EC" },
  { 0x15, "#ASI_PHYS_BYPASS_EC_E" },
  { 0x16, "#ASI_BLK_AIUP_4V" },
  { 0x17, "#ASI_BLK_AIUS_4V" },
  { 0x1c, "#ASI_PHYS_USE_EC_L" },
  { 0x1d, "#ASI_PHYS_BYPASS_EC_E_L" },
  { 0x1e, "#ASI_BLK_AIUP_L_4V" },
  { 0x1f, "#ASI_BLK_AIUS_L_4V" },
  { 0x20, "#ASI_SCRATCHPAD" },
  { 0x21, "#ASI_MMU" },
  { 0x23, "#ASI_BLK_INIT_QUAD_LDD_AIUS" },
  { 0x24, "#ASI_NUCLEUS_QUAD_LDD" },
  { 0x25, "#ASI_QUEUE" },
  { 0x26, "#ASI_QUAD_LDD_PHYS_4V" },
  { 0x2c, "#ASI_NUCLEUS_QUAD_LDD_L" },
  { 0x30, "#ASI_PCACHE_DATA_STATUS" },
  { 0x31, "#ASI_PCACHE_DATA" },
  { 0x32, "#ASI_PCACHE_TAG" },
  { 0x33, "#ASI_PCACHE_SNOOP_TAG" },
  { 0x34, "#ASI_QUAD_LDD_PHYS" },
  { 0x38, "#ASI_WCACHE_VALID_BITS" },
  { 0x39, "#ASI_WCACHE_DATA" },
  { 0x3a, "#ASI_WCACHE_TAG" },
  { 0x3b, "#ASI_WCACHE_SNOOP_TAG" },
  { 0x3c, "#ASI_QUAD_LDD_PHYS_L" },
  { 0x40, "#ASI_SRAM_FAST_INIT" },
  { 0x41, "#ASI_CORE_AVAILABLE" },
  { 0x41, "#ASI_CORE_ENABLE_STAT" },
  { 0x41, "#ASI_CORE_ENABLE" },
  { 0x41, "#ASI_XIR_STEERING" },
  { 0x41, "#ASI_CORE_RUNNING_RW" },
  { 0x41, "#ASI_CORE_RUNNING_W1S" },
  { 0x41, "#ASI_CORE_RUNNING_W1C" },
  { 0x41, "#ASI_CORE_RUNNING_STAT" },
  { 0x41, "#ASI_CMT_ERROR_STEERING" },
  { 0x41, "#ASI_DCACHE_INVALIDATE" },
  { 0x41, "#ASI_DCACHE_UTAG" },
  { 0x41, "#ASI_DCACHE_SNOOP_TAG" },
  { 0x42, "#ASI_DCACHE_INVALIDATE" },
  { 0x43, "#ASI_DCACHE_UTAG" },
  { 0x44, "#ASI_DCACHE_SNOOP_TAG" },
  { 0x45, "#ASI_LSU_CONTROL_REG" },
  { 0x45, "#ASI_DCU_CONTROL_REG" },
  { 0x46, "#ASI_DCACHE_DATA" },
  { 0x47, "#ASI_DCACHE_TAG" },
  { 0x48, "#ASI_INTR_DISPATCH_STAT" },
  { 0x49, "#ASI_INTR_RECEIVE" },
  { 0x4a, "#ASI_UPA_CONFIG" },
  { 0x4a, "#ASI_JBUS_CONFIG" },
  { 0x4a, "#ASI_SAFARI_CONFIG" },
  { 0x4a, "#ASI_SAFARI_ADDRESS" },
  { 0x4b, "#ASI_ESTATE_ERROR_EN" },
  { 0x4c, "#ASI_AFSR" },
  { 0x4d, "#ASI_AFAR" },
  { 0x4e, "#ASI_EC_TAG_DATA" },
  { 0x50, "#ASI_IMMU" },
  { 0x51, "#ASI_IMMU_TSB_8KB_PTR" },
  { 0x52, "#ASI_IMMU_TSB_16KB_PTR" },
  { 0x54, "#ASI_ITLB_DATA_IN" },
  { 0x55, "#ASI_ITLB_DATA_ACCESS" },
  { 0x56, "#ASI_ITLB_TAG_READ" },
  { 0x57, "#ASI_IMMU_DEMAP" },
  { 0x58, "#ASI_DMMU" },
  { 0x59, "#ASI_DMMU_TSB_8KB_PTR" },
  { 0x5a, "#ASI_DMMU_TSB_64KB_PTR" },
  { 0x5b, "#ASI_DMMU_TSB_DIRECT_PTR" },
  { 0x5c, "#ASI_DTLB_DATA_IN" },
  { 0x5d, "#ASI_DTLB_DATA_ACCESS" },
  { 0x5e, "#ASI_DTLB_TAG_READ" },
  { 0x5f, "#ASI_DMMU_DEMAP" },
  { 0x60, "#ASI_IIU_INST_TRAP" },
  { 0x63, "#ASI_INTR_ID" },
  { 0x63, "#ASI_CORE_ID" },
  { 0x63, "#ASI_CESR_ID" },
  { 0x66, "#ASI_IC_INSTR" },
  { 0x67, "#ASI_IC_TAG" },
  { 0x68, "#ASI_IC_STAG" },
  { 0x6e, "#ASI_IC_PRE_DECODE" },
  { 0x6f, "#ASI_IC_NEXT_FIELD" },
  { 0x6f, "#ASI_BRPRED_ARRAY" },
  { 0x70, "#ASI_BLK_AIUP" },
  { 0x71, "#ASI_BLK_AIUS" },
  { 0x72, "#ASI_MCU_CTRL_REG" },
  { 0x74, "#ASI_EC_DATA" },
  { 0x75, "#ASI_EC_CTRL" },
  { 0x76, "#ASI_EC_W" },
  { 0x77, "#ASI_UDB_ERROR_W" },
  { 0x77, "#ASI_UDB_CONTROL_W" },
  { 0x77, "#ASI_INTR_W" },
  { 0x77, "#ASI_INTR_DATAN_W" },
  { 0x77, "#ASI_INTR_DISPATCH_W" },
  { 0x78, "#ASI_BLK_AIUPL" },
  { 0x79, "#ASI_BLK_AIUSL" },
  { 0x7e, "#ASI_EC_R" },
  { 0x7f, "#ASI_UDBH_ERROR_R" },
  { 0x7f, "#ASI_UDBL_ERROR_R" },
  { 0x7f, "#ASI_UDBH_CONTROL_R" },
  { 0x7f, "#ASI_UDBL_CONTROL_R" },
  { 0x7f, "#ASI_INTR_R" },
  { 0x7f, "#ASI_INTR_DATAN_R" },
  { 0xc0, "#ASI_PST8_P" },
  { 0xc1, "#ASI_PST8_S" },
  { 0xc2, "#ASI_PST16_P" },
  { 0xc3, "#ASI_PST16_S" },
  { 0xc4, "#ASI_PST32_P" },
  { 0xc5, "#ASI_PST32_S" },
  { 0xc8, "#ASI_PST8_PL" },
  { 0xc9, "#ASI_PST8_SL" },
  { 0xca, "#ASI_PST16_PL" },
  { 0xcb, "#ASI_PST16_SL" },
  { 0xcc, "#ASI_PST32_PL" },
  { 0xcd, "#ASI_PST32_SL" },
  { 0xd0, "#ASI_FL8_P" },
  { 0xd1, "#ASI_FL8_S" },
  { 0xd2, "#ASI_FL16_P" },
  { 0xd3, "#ASI_FL16_S" },
  { 0xd8, "#ASI_FL8_PL" },
  { 0xd9, "#ASI_FL8_SL" },
  { 0xda, "#ASI_FL16_PL" },
  { 0xdb, "#ASI_FL16_SL" },
  { 0xe0, "#ASI_BLK_COMMIT_P", },
  { 0xe1, "#ASI_BLK_COMMIT_S", },
  { 0xe2, "#ASI_BLK_INIT_QUAD_LDD_P" },
  { 0xf0, "#ASI_BLK_P", },
  { 0xf1, "#ASI_BLK_S", },
  { 0xf8, "#ASI_BLK_PL", },
  { 0xf9, "#ASI_BLK_SL", },
  { 0, 0 }
};

/* Return the value for ASI NAME, or -1 if not found.  */

int
sparc_encode_asi (const char *name)
{
  return lookup_name (asi_table, name);
}

/* Return the name for ASI value VALUE or NULL if not found.  */

const char *
sparc_decode_asi (int value)
{
  return lookup_value (asi_table, value);
}

/* Handle membar masks.  */

static arg membar_table[] =
{
  { 0x40, "#Sync" },
  { 0x20, "#MemIssue" },
  { 0x10, "#Lookaside" },
  { 0x08, "#StoreStore" },
  { 0x04, "#LoadStore" },
  { 0x02, "#StoreLoad" },
  { 0x01, "#LoadLoad" },
  { 0, 0 }
};

/* Return the value for membar arg NAME, or -1 if not found.  */

int
sparc_encode_membar (const char *name)
{
  return lookup_name (membar_table, name);
}

/* Return the name for membar value VALUE or NULL if not found.  */

const char *
sparc_decode_membar (int value)
{
  return lookup_value (membar_table, value);
}

/* Handle prefetch args.  */

static arg prefetch_table[] =
{
  { 0, "#n_reads" },
  { 1, "#one_read" },
  { 2, "#n_writes" },
  { 3, "#one_write" },
  { 4, "#page" },
  { 16, "#invalidate" },
  { 17, "#unified", },
  { 20, "#n_reads_strong", },
  { 21, "#one_read_strong", },
  { 22, "#n_writes_strong", },
  { 23, "#one_write_strong", },
  { 0, 0 }
};

/* Return the value for prefetch arg NAME, or -1 if not found.  */

int
sparc_encode_prefetch (const char *name)
{
  return lookup_name (prefetch_table, name);
}

/* Return the name for prefetch value VALUE or NULL if not found.  */

const char *
sparc_decode_prefetch (int value)
{
  return lookup_value (prefetch_table, value);
}

/* Handle sparclet coprocessor registers.  */

static arg sparclet_cpreg_table[] =
{
  { 0, "%ccsr" },
  { 1, "%ccfr" },
  { 2, "%cccrcr" },
  { 3, "%ccpr" },
  { 4, "%ccsr2" },
  { 5, "%cccrr" },
  { 6, "%ccrstr" },
  { 0, 0 }
};

/* Return the value for sparclet cpreg arg NAME, or -1 if not found.  */

int
sparc_encode_sparclet_cpreg (const char *name)
{
  return lookup_name (sparclet_cpreg_table, name);
}

/* Return the name for sparclet cpreg value VALUE or NULL if not found.  */

const char *
sparc_decode_sparclet_cpreg (int value)
{
  return lookup_value (sparclet_cpreg_table, value);
}
