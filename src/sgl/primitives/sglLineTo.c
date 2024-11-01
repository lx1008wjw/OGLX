/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglLineTo.c
 DESCRIPTION       : sglLineTo shall define a line to given point.
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
  NAME: sglLineTo
  DESCRIPTION:
    sglLineTo shall define a line to given point.
  PARAMETERS:
    par_f_x ->  X coordinate of the point to move to. 
	par_f_y ->  Y coordinate of the point to move to. 
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglLineTo(SGLfloat par_f_x, SGLfloat par_f_y)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        oglxVertex2f(par_f_x, par_f_y);
        glob_pr_sglStatemachine->f_last_path_point_x = par_f_x;
        glob_pr_sglStatemachine->f_last_path_point_y = par_f_y;
        glob_pr_sglStatemachine->f_last_path_control_x = par_f_x;
        glob_pr_sglStatemachine->f_last_path_control_y = par_f_y;


        glob_pr_sglStatemachine->b_last_command = OGLX_LINE_TO;
    }
    return;
}

/* End of File ***************************************************************/
