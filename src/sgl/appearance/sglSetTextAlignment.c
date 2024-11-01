/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetTextAlignment.c
 DESCRIPTION       : sglSetTextAlignment command shall store the text alignment properties
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
  NAME: sglSetTextAlignment
  DESCRIPTION:
    Function shall store the text alignment properties.
  PARAMETERS:
    par_l_hor_alignment -> horizontal alignment property (SGL_ALIGN_LEFT or SGL_ALIGN_CENTER or SGL_ALIGN_RIGHT)
    par_l_vert_alignment -> vertical alignment property (SGL_ALIGN_BOTTOM or SGL_ALIGN_MIDDLE or SGL_ALIGN_TOP)
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglSetTextAlignment(SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment)
{
    SGLbool loc_b_error = SGL_FALSE;

    /* Check horizontal alignment */
    if ((par_l_hor_alignment != SGL_ALIGN_LEFT) && (par_l_hor_alignment != SGL_ALIGN_CENTER) && (par_l_hor_alignment != SGL_ALIGN_RIGHT)) {
        loc_b_error = SGL_TRUE;
    }
    else {
        glob_pr_sglStatemachine->f_h_alignment = ((SGLfloat) par_l_hor_alignment) * 0.5F;
        glob_pr_sglStatemachine->b_h_alignment = (SGLbyte) par_l_hor_alignment;
    }

    /* Check vertical alignment */
    if ((par_l_vert_alignment != SGL_ALIGN_BOTTOM) && (par_l_vert_alignment != SGL_ALIGN_MIDDLE) && (par_l_vert_alignment != SGL_ALIGN_TOP)) {
        loc_b_error = SGL_TRUE;
    }
    else {
        glob_pr_sglStatemachine->f_v_alignment = ((SGLfloat) par_l_vert_alignment) * 0.5F;
        glob_pr_sglStatemachine->b_v_alignment = (SGLbyte) par_l_vert_alignment;
    }

    if (loc_b_error) {
        oglxSetError(SGL_ERROR_SGL_SETTEXTALIGNMENT, 0U);
    }
    else {
        /* Nothing to do */
    }

    return;
}
