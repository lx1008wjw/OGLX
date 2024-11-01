/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglFormatGradient.c
 DESCRIPTION       : sglFormatGradient shall set the gradient attributes
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
  NAME: sgl_format_gradient_detect_error
  DESCRIPTION:
    Function shall check parameters of sglFormatGradient.
  PARAMETERS:
    par_ul_nb_vertices -> number of vertices of the figure on which the texture is applied
    par_pf_x -> table of x coordinates of the figure
    par_pf_y -> table of y coordinates of the figure
  RETURN:
    SGLbool -> Error detected (SGL_TRUE) or not (SGL_FALSE)
---------------------------------------------------------------------+*/
SGLbool sgl_format_gradient_detect_error(SGLulong par_ul_nb_vertices, SGLfloat * par_pf_x, SGLfloat * par_pf_y)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    if (par_ul_nb_vertices <= 1UL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (par_ul_nb_vertices >= SGL_MAX_VERTEX_ARRAY_SIZE) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (par_pf_x == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (par_pf_y == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture attribute structure */
    if (glob_pr_sglStatemachine->p_gradient_attrib == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglFormatGradient
  DESCRIPTION:
    Function shall set the gradient attributes.
  PARAMETERS:
    par_ul_nb_vertices -> number of vertices of the figure on which the texture is applied
    par_pf_x -> table of x coordinates of the figure
    par_pf_y -> table of y coordinates of the figure
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglFormatGradient(SGLulong par_ul_nb_vertices, SGLfloat * par_pf_x, SGLfloat * par_pf_y)
{
    SGLfloat loc_f_xmax;
    SGLfloat loc_f_ymax;

    SGLbool loc_b_error_detected = sgl_format_gradient_detect_error(par_ul_nb_vertices, par_pf_x, par_pf_y);

    if (!loc_b_error_detected) {
        /* Nothing shall be done if textures are active */
        if (!glob_pr_sglStatemachine->b_texture_state) {
            loc_f_xmax = mth_get_max_value(par_ul_nb_vertices, par_pf_x);
            loc_f_ymax = mth_get_max_value(par_ul_nb_vertices, par_pf_y);

            glob_pr_sglStatemachine->f_current_xmin = mth_get_min_value(par_ul_nb_vertices, par_pf_x);
            glob_pr_sglStatemachine->f_current_ymin = mth_get_min_value(par_ul_nb_vertices, par_pf_y);

            glob_pr_sglStatemachine->f_current_bbox_width = loc_f_xmax - glob_pr_sglStatemachine->f_current_xmin;
            glob_pr_sglStatemachine->f_current_bbox_height = loc_f_ymax - glob_pr_sglStatemachine->f_current_ymin;

            sgl_compute_nx_ny(1.0F, 1.0F, SGL_FALSE);

            sgl_compute_dx_dy(SGL_ALIGN_CENTER, SGL_ALIGN_MIDDLE, SGL_FALSE);
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_FORMATGRADIENT, par_ul_nb_vertices);
    }
    return;
}

/* End of File ***************************************************************/
