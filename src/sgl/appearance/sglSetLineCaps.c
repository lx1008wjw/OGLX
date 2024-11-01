/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetLineCaps.c
 DESCRIPTION       : sglSetLineCaps command shall store the line caps appearance
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSetLineCaps
  DESCRIPTION:
    Function shall set the line caps rendering style.
  PARAMETERS:
    par_l_type -> the type of the line (SGL_STRAIGHT_CAPS or SGL_ROUNDED_CAPS)
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglSetLineCaps(SGLlong par_l_type)
{
    if (par_l_type == (SGLlong) SGL_STRAIGHT_CAPS) {
        glob_pr_sglStatemachine->l_line_caps_appearance = SGL_STRAIGHT_CAPS;
    }
    else {
        glob_pr_sglStatemachine->l_line_caps_appearance = SGL_ROUNDED_CAPS;
    }

    return;
}
