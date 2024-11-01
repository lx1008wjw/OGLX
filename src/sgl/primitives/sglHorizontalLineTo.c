/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglHorizontalLineTo.c
 DESCRIPTION       : sglHorizontalLineTo shall define ahorizontal line to given x coordinate.
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
  NAME: sglHorizontalLineTo
  DESCRIPTION:
    sglHorizontalLineTo shall define a horizontal line to given x coordinate.
  PARAMETERS:
    par_f_x ->  X coordinate of the point to move to. 
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglHorizontalLineTo(SGLfloat par_f_x)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        sglLineTo(par_f_x, glob_pr_sglStatemachine->f_last_path_point_y);
        /* glob_pr_sglStatemachine attributes remain unchainged as the command is considered as sglLineTo */
    }
    return;
}

/* End of File ***************************************************************/
