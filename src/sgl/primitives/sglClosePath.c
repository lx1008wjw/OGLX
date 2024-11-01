/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglClosePath.c
 DESCRIPTION       : sglClosePath shall define a line between current point and first point.
 COPYRIGHT (C)     : 2016 Esterel Technologies SAS. All Rights Reserved.
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
  NAME: sglClosePath
  DESCRIPTION:
    Function shall define a line between current point and first point.
  PARAMETERS:
	None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglClosePath(void)
{
    if ((glob_pr_sglStatemachine->b_path_type == SGL_OUTLINE)
        || ((glob_pr_sglStatemachine->b_path_type == SGL_FILL) && (glob_pr_sglStatemachine->tr_geometry.l_size > 2L))) {
        sglLineTo(glob_pr_sglStatemachine->f_first_path_point_x, glob_pr_sglStatemachine->f_first_path_point_y);
        glob_pr_sglStatemachine->b_last_command = OGLX_CLOSE_PATH;
    }
    return;
}

/* End of File ***************************************************************/
