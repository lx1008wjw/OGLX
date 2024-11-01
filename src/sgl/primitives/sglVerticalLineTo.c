/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglVerticalLineTo.c
 DESCRIPTION       : sglVerticalLineTo shall define a vertical line to given y coordinate.
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
  NAME: sglVerticalLineTo
  DESCRIPTION:
    sglVerticalLineTo shall define a vertical line to given y coordinate.
  PARAMETERS:
    par_f_y ->  Y coordinate of the point to move to. 
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglVerticalLineTo(SGLfloat par_f_y)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        sglLineTo(glob_pr_sglStatemachine->f_last_path_point_x, par_f_y);
    }
    return;
}

/* End of File ***************************************************************/
