/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxReadBinaryFonts.h
 DESCRIPTION       : oglxReadBinaryFonts provides an example of binary font loader
 COPYRIGHT (C)     : 2016 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- +*/

#ifndef _OGLXREADBINARYFONTS_H
#define _OGLXREADBINARYFONTS_H

#include "sgl.h"
#include <stddef.h>

/* Function to get one byte from input */
typedef int (*oglxReadByteFunc) (void);

/* Function to allocate memory */
typedef void *(*oglxMallocFunc) (size_t);

/* Function to free memory */
typedef void (*oglxFreeFunc) (void *);

/* Keywords for beginning/end of OEM block  */
#define KW_BEG_OEM_BLOCK 0x6A
#define KW_END_OEM_BLOCK 0x6B
#define KW_TYP_OEM_FONT_TABLE 0xC1

/* 
 * Read the content of a binary font table block. 
 * Return 0 in case of success. 
 */
OGLXAPI int oglxReadBinaryFonts(oglxReadByteFunc pf_read_byte, oglxMallocFunc pf_malloc, oglxFreeFunc pf_free);

#endif                          /* _OGLXREADBINARYFONTS_H */
