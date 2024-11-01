/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxLoadBinaryFonts.c
 DESCRIPTION       : oglxLoadBinaryFonts provides an example of binary fonts loader.
 COPYRIGHT (C)     : 2016 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

#include "oglxLoadBinaryFonts.h"
#include "oglxReadBinaryFonts.h"
#include "sgl.h"

/******************************************************************************
 **                           Porting
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static FILE *global_file = NULL;

static void oglxOpenFile(const char *path)
{
    global_file = fopen(path, "rb");
}

static void oglxCloseFile(void)
{
    (void) fclose(global_file);
}

static SGLuint32 oglxGetFilePosition(void)
{
    return (SGLuint32) ftell(global_file);
}

static void oglxSetFilePosition(SGLuint32 pos)
{
    (void) fseek(global_file, (long) pos, SEEK_SET);
}

static int oglxReadByte(void)
{
    return fgetc(global_file);
}

static void *oglxMalloc(size_t sz)
{
    void *mem = malloc(sz);
    if (mem != NULL) {
        memset(mem, 0, sz);
    }
    return mem;

}

static void oglxFree(void *mem)
{
    if (mem != NULL) {
        free(mem);
    }
}

/******************************************************************************
 **                           Basic reading
 *****************************************************************************/

static SGLuint8 oglxReadUint8()
{
    return (SGLuint8) oglxReadByte();
}

static SGLuint16 oglxReadUint16()
{
    SGLuint16 u1 = oglxReadUint8();
    SGLuint16 u2 = oglxReadUint8();
    return ((u1 << 8) | u2);
}

static SGLuint32 oglxReadUint32()
{
    SGLuint32 u1 = oglxReadUint16();
    SGLuint32 u2 = oglxReadUint16();
    return ((u1 << 16) | u2);
}

static void oglxSkip(SGLuint32 nb_bytes)
{
    while (nb_bytes > 0U) {
        (void) oglxReadByte();
        nb_bytes--;
    }
}

/******************************************************************************
 **                           Blocks reading
 *****************************************************************************/

static SGLbool oglxReadHeader(void)
{
    SGLuint16 kw = oglxReadUint16();
    if (kw != 0xA661)
        return SGL_FALSE;
    oglxSkip(6U);
    return SGL_TRUE;
}

static SGLbool oglxReadFooter(void)
{
    SGLuint8 kw = oglxReadUint8();
    if (kw != 0xE0)
        return SGL_FALSE;
    oglxSkip(3U);
    return SGL_TRUE;
}

static SGLuint32 oglxReadBeginOemBlock(SGLuint8 kind)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if ((kw1 != KW_BEG_OEM_BLOCK) || (kw2 != kind))
        return 0;
    oglxSkip(2U);
    /* Return the block size */
    return oglxReadUint32();
}

static SGLbool oglxReadEndOemBlock(SGLuint8 kind)
{
    SGLuint8 kw1 = oglxReadUint8();
    SGLuint8 kw2 = oglxReadUint8();
    if ((kw1 != KW_END_OEM_BLOCK) || (kw2 != kind))
        return SGL_FALSE;
    oglxSkip(2U);
    return SGL_TRUE;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxLoadBinaryFonts
  DESCRIPTION:
    Function provides an example of binary fonts loader.
  PARAMETERS:
    path: file path containing binary fonts
  RETURN:
    0 in case of success, otherwise an error code
---------------------------------------------------------------------+*/
int oglxLoadBinaryFonts(const char *path)
{
    int code = 0;

    /* Open binary file */
    oglxOpenFile(path);
    if (global_file == NULL) {
        code = 1;
    }

    /* Read header */
    if ((code == 0) && !oglxReadHeader()) {
        code = 2;
    }

    if (code == 0) {
        /* Begin OEM block Font Table */
        SGLuint32 begin_pos = oglxGetFilePosition();
        SGLuint32 block_size = oglxReadBeginOemBlock(KW_TYP_OEM_FONT_TABLE);
        if (block_size == 0) {
            code = 3;
        }

        /* Read binary font block content */
        if (code == 0) {
            code = oglxReadBinaryFonts(oglxReadByte, oglxMalloc, oglxFree);
        }

        /* Align on 32-bit */
        if (code == 0) {
            SGLuint32 current_pos = oglxGetFilePosition();
            SGLuint32 align32 = (4U - (current_pos % 4U)) % 4U;
            oglxSkip(align32);
        }

        /* End OEM block Font Table */
        if ((code == 0) && !oglxReadEndOemBlock(KW_TYP_OEM_FONT_TABLE)) {
            code = 4;
        }

        if (block_size != 0) {
            oglxSetFilePosition(begin_pos + block_size);
        }

        /* Read header */
        if (!oglxReadFooter()) {
            code = 5;
        }
    }

    /* Close file */
    if (global_file != NULL) {
        oglxCloseFile();
    }

    return code;
}

/* End of File ***************************************************************/
