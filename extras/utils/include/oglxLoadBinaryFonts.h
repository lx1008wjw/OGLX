/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxLoadBinaryFonts.h
 DESCRIPTION       : oglxLoadBinaryFonts provides an example of binary font loader
 COPYRIGHT (C)     : 2016 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- +*/

#ifndef _OGLXLOADBINARYFONTS_H
#define _OGLXLOADBINARYFONTS_H

#include "sgl.h"

/* 
 * Load a complete binary font table file. 
 * Return 0 in case of success. 
 */
OGLXAPI int oglxLoadBinaryFonts(const char *path);

#endif                          /* _OGLXLOADBINARYFONTS_H */
