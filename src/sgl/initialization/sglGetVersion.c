/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetVersion.c
 DESCRIPTION       : sglGetVersion shall return OGLX version
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"
#include "oglx_version.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetVersion
  DESCRIPTION:
    Function shall return OGLX version
  PARAMETERS:
	None
  RETURN:
    char *-> OGLX version
---------------------------------------------------------------------+*/
char *sglGetVersion(void)
{
    return (char *) OGLX_VERSION;
}

/* End of File ***************************************************************/
