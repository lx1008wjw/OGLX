/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglConvertScreenToPoint.c
 DESCRIPTION       : sglConvertScreenToPoint command shall convert the given screen coordinates to user coordinates
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
  NAME: sglConvertScreenToPoint
  DESCRIPTION:
    Function shall convert the given the given screen coordinates to user coordinates
  PARAMETERS:
    par_f_x -> X-coordinate on screen
    par_f_y -> Y-coordinate on screen
    par_pf_user_x -> X-coordinate in user units
    par_pf_user_y -> Y-coordinate in user units
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglConvertScreenToPoint(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_user_x, SGLfloat * par_pf_user_y)
{
    if ((par_pf_user_x != SGL_NULL) && (par_pf_user_y != SGL_NULL)) {
        SGLfloat loc_f_par_x =
            SGLfloat_div(par_f_x, (SGLfloat) glob_pr_sglStatemachine->ul_screen_w) * (2.0F * glob_pr_sglStatemachine->f_viewport_width_div_2);
        SGLfloat loc_f_par_y =
            SGLfloat_div(par_f_y, (SGLfloat) glob_pr_sglStatemachine->ul_screen_h) * (2.0F * glob_pr_sglStatemachine->f_viewport_height_div_2);

        sglConvertPixelToPoint(loc_f_par_x, loc_f_par_y, par_pf_user_x, par_pf_user_y);
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CONVERTSCREENTOPOINT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
