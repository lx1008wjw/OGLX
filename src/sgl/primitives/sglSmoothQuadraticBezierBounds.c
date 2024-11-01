/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSmoothQuadraticBezierBounds.c
 DESCRIPTION       : Function shall compute the bounds of a bezier curve.
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
#include "mth.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSmoothQuadraticBezierBounds
  DESCRIPTION:
    Function shall compute the bounds of a bezier curve.
  PARAMETERS:
	par_f_prev_x, par_f_prev_y -> Starting point.
	par_f_prev_control_x, par_f_prev_control_y -> Previous control point.
	par_f_x,par_f_y ->  Ending point. 
	par_pf_x1, par_pf_y1, par_pf_x2, par_pf_y2 -> The resulting bounds.
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSmoothQuadraticBezierBounds(SGLfloat par_f_prev_x, SGLfloat par_f_prev_y,
                                    SGLfloat par_f_prev_control_x, SGLfloat par_f_prev_control_y,
                                    SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_x1, SGLfloat * par_pf_y1, SGLfloat * par_pf_x2, SGLfloat * par_pf_y2)
{
    if ((par_pf_x1 != SGL_NULL) && (par_pf_y1 != SGL_NULL) && (par_pf_x2 != SGL_NULL) && (par_pf_y2 != SGL_NULL)) {
        sglQuadraticBezierBounds(par_f_prev_x, par_f_prev_y,
                                 (2.0F * par_f_prev_x) - par_f_prev_control_x, (2.0F * par_f_prev_y) - par_f_prev_control_y, par_f_x, par_f_y,
                                 par_pf_x1, par_pf_y1, par_pf_x2, par_pf_y2);
    }
    else {
        oglxSetError(SGL_ERROR_SGL_SMOOTHQUADBEZIERBOUNDS, 0U);
    }

    return;
}

/* End of File ***************************************************************/
