/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglFormatTexture.c
 DESCRIPTION       : sglFormatTexture shall set the texture attributes
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
#include "mth.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_compute_nx_ny
  DESCRIPTION:
    Function shall compute the horizontal and vertical distribution of current texture.
  PARAMETERS:
    par_f_horiz_pattern -> horizontal distribution of the texture
    par_f_vert_pattern -> vertical distribution of the texture
	par_b_texture -> Defines if texture table shall be used or gradient table
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_compute_nx_ny(SGLfloat par_f_horiz_pattern, SGLfloat par_f_vert_pattern, SGLbool par_b_texture)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_x;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_y;
    SGLbool loc_b_valid_index = SGL_TRUE;
    SGLfloat loc_f_texture_width = 0.0F;
    SGLfloat loc_f_texture_height = 0.0F;

    if (par_b_texture) {
        SGLlong loc_l_last_texture_bound = glob_pr_sglStatemachine->l_last_texture_bound_index;
        if (loc_l_last_texture_bound < (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) {
            loc_f_texture_width = (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound].ul_textures_dimension[0];
            loc_f_texture_height = (SGLfloat) glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound].ul_textures_dimension[1];
        }
        else {
            loc_b_valid_index = SGL_FALSE;
        }
    }
    else {
        SGLlong loc_l_last_texture_bound = glob_pr_sglStatemachine->l_gradient_index;
        if (loc_l_last_texture_bound < (SGLlong) glob_pr_sglStatemachine->ul_number_of_gradients) {
            loc_f_texture_width = (SGLfloat) glob_pr_sglStatemachine->p_gradient_attrib[loc_l_last_texture_bound].ul_textures_dimension[0];
            loc_f_texture_height = (SGLfloat) glob_pr_sglStatemachine->p_gradient_attrib[loc_l_last_texture_bound].ul_textures_dimension[1];
        }
        else {
            loc_b_valid_index = SGL_FALSE;
        }

    }

    if (loc_b_valid_index) {
        SGLfloat loc_f_bbox_width = glob_pr_sglStatemachine->f_current_bbox_width;
        SGLfloat loc_f_bbox_height = glob_pr_sglStatemachine->f_current_bbox_height;

        /* Compute Nx/Ny values of texture coordinates */
        if ((par_f_horiz_pattern <= 0.0F) && (par_f_vert_pattern <= 0.0F)) {
            glob_pr_sglStatemachine->f_current_nx = SGLfloat_div(loc_f_bbox_width, (loc_f_ratio_x * loc_f_texture_width));
            glob_pr_sglStatemachine->f_current_ny = SGLfloat_div(loc_f_bbox_height, (loc_f_ratio_y * loc_f_texture_height));
        }
        else {
            if (par_f_horiz_pattern <= 0.0F) {
                if (!(sgluFloatIsEqual(loc_f_bbox_height, 0.0F))) {
                    glob_pr_sglStatemachine->f_current_nx = par_f_vert_pattern *
                        SGLfloat_div(loc_f_texture_height, loc_f_texture_width) * SGLfloat_div(loc_f_bbox_width, loc_f_bbox_height);
                    glob_pr_sglStatemachine->f_current_ny = par_f_vert_pattern;
                }
                else {
                    /* Robustness case when height is null */
                    glob_pr_sglStatemachine->f_current_nx = 0.0F;
                    glob_pr_sglStatemachine->f_current_ny = 0.0F;
                }
            }
            else {
                if (par_f_vert_pattern <= 0.0F) {
                    if (!(sgluFloatIsEqual(loc_f_bbox_width, 0.0F))) {
                        glob_pr_sglStatemachine->f_current_nx = par_f_horiz_pattern;
                        glob_pr_sglStatemachine->f_current_ny = par_f_horiz_pattern *
                            SGLfloat_div(loc_f_texture_width, loc_f_texture_height) * SGLfloat_div(loc_f_bbox_height, loc_f_bbox_width);
                    }
                    else {
                        /* Robustness case when height is null */
                        glob_pr_sglStatemachine->f_current_nx = 0.0F;
                        glob_pr_sglStatemachine->f_current_ny = 0.0F;
                    }
                }
                else {
                    glob_pr_sglStatemachine->f_current_nx = par_f_horiz_pattern;
                    glob_pr_sglStatemachine->f_current_ny = par_f_vert_pattern;
                }
            }
        }
    }
    else {
        /* Robustness case when no texture is bound */
        glob_pr_sglStatemachine->f_current_nx = 0.0F;
        glob_pr_sglStatemachine->f_current_ny = 0.0F;
    }
    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_compute_dx_dy
  DESCRIPTION:
    Function shall compute the horizontal and vertical alignment of current texture.
  PARAMETERS:
    par_l_horiz_align -> horizontal alignment of the texture
    par_l_vert_align -> vertical alignment of the texture
	par_b_texture -> Defines if texture table shall be used or gradient table
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_compute_dx_dy(SGLlong par_l_horiz_align, SGLlong par_l_vert_align, SGLbool par_b_texture)
{
    SGLbool loc_b_valid_index = SGL_TRUE;

    if (par_b_texture) {
        SGLlong loc_l_last_texture_bound = glob_pr_sglStatemachine->l_last_texture_bound_index;
        if (loc_l_last_texture_bound < (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) {
            /* Valid index */
        }
        else {
            loc_b_valid_index = SGL_FALSE;
        }
    }
    else {
        SGLlong loc_l_last_texture_bound = glob_pr_sglStatemachine->l_gradient_index;
        if (loc_l_last_texture_bound < (SGLlong) glob_pr_sglStatemachine->ul_number_of_gradients) {
            /* Valid index */
        }
        else {
            loc_b_valid_index = SGL_FALSE;
        }

    }

    if (loc_b_valid_index) {
        /* Compute Dx/Dy values of texture coordinates */
        switch (par_l_horiz_align) {
        case SGL_ALIGN_LEFT:
            glob_pr_sglStatemachine->f_current_dx = 0.0F;
            break;
        case SGL_ALIGN_RIGHT:
            glob_pr_sglStatemachine->f_current_dx =
                SGLint32_to_float(mth_f_to_l_ceil(glob_pr_sglStatemachine->f_current_nx)) - glob_pr_sglStatemachine->f_current_nx;
            break;
        default:
            /* case SGL_ALIGN_CENTER: */
            glob_pr_sglStatemachine->f_current_dx = (1.0F - glob_pr_sglStatemachine->f_current_nx) * 0.5F;
            break;
        }

        switch (par_l_vert_align) {
        case SGL_ALIGN_BOTTOM:
            glob_pr_sglStatemachine->f_current_dy = 0.0F;
            break;
        case SGL_ALIGN_TOP:
            glob_pr_sglStatemachine->f_current_dy =
                SGLint32_to_float(mth_f_to_l_ceil(glob_pr_sglStatemachine->f_current_ny)) - glob_pr_sglStatemachine->f_current_ny;
            break;
        default:
            /* case SGL_ALIGN_MIDDLE: */
            glob_pr_sglStatemachine->f_current_dy = (1.0F - glob_pr_sglStatemachine->f_current_ny) * 0.5F;
            break;
        }
    }
    else {
        /* Robustness case when no texture is bound */
        glob_pr_sglStatemachine->f_current_dx = 0.0F;
        glob_pr_sglStatemachine->f_current_dy = 0.0F;
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_format_texture_detect_error
  DESCRIPTION:
    Function shall check parameters of sglFormatTexture.
  PARAMETERS:
    par_ul_nb_vertices -> number of vertices of the figure on which the texture is applied
    par_pf_x -> table of x coordinates of the figure
    par_pf_y -> table of y coordinates of the figure
    par_l_horiz_align -> horizontal alignment of the texture
    par_l_vert_align -> vertical alignment of the texture
  RETURN:
    SGLbool -> Error detected (SGL_TRUE) or not (SGL_FALSE)
---------------------------------------------------------------------+*/
SGLbool sgl_format_texture_detect_error(SGLulong par_ul_nb_vertices, const SGLfloat * par_pf_x, const SGLfloat * par_pf_y,
                                        SGLlong par_l_horiz_align, SGLlong par_l_vert_align)
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

    if ((par_l_horiz_align != SGL_ALIGN_LEFT) && (par_l_horiz_align != SGL_ALIGN_RIGHT) && (par_l_horiz_align != SGL_ALIGN_CENTER)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_l_vert_align != SGL_ALIGN_BOTTOM) && (par_l_vert_align != SGL_ALIGN_TOP) && (par_l_vert_align != SGL_ALIGN_MIDDLE)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture attribute structure */
    if (glob_pr_sglStatemachine->p_texture_attrib == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglFormatTexture
  DESCRIPTION:
    Function shall set the texture attributes.
  PARAMETERS:
    par_ul_nb_vertices -> number of vertices of the figure on which the texture is applied
    par_pf_x -> table of x coordinates of the figure
    par_pf_y -> table of y coordinates of the figure
    par_f_horiz_pattern -> horizontal distribution of the texture
    par_f_vert_pattern -> vertical distribution of the texture
    par_l_horiz_align -> horizontal alignment of the texture
    par_l_vert_align -> vertical alignment of the texture
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglFormatTexture(SGLulong par_ul_nb_vertices, SGLfloat * par_pf_x, SGLfloat * par_pf_y,
                      SGLfloat par_f_horiz_pattern, SGLfloat par_f_vert_pattern, SGLlong par_l_horiz_align, SGLlong par_l_vert_align)
{
    SGLfloat loc_f_xmax;
    SGLfloat loc_f_ymax;

    SGLbool loc_b_error_detected = sgl_format_texture_detect_error(par_ul_nb_vertices, par_pf_x, par_pf_y,
                                                                   par_l_horiz_align, par_l_vert_align);

    if (!loc_b_error_detected) {
        loc_f_xmax = mth_get_max_value(par_ul_nb_vertices, par_pf_x);
        loc_f_ymax = mth_get_max_value(par_ul_nb_vertices, par_pf_y);

        glob_pr_sglStatemachine->f_current_xmin = mth_get_min_value(par_ul_nb_vertices, par_pf_x);
        glob_pr_sglStatemachine->f_current_ymin = mth_get_min_value(par_ul_nb_vertices, par_pf_y);

        glob_pr_sglStatemachine->f_current_bbox_width = loc_f_xmax - glob_pr_sglStatemachine->f_current_xmin;
        glob_pr_sglStatemachine->f_current_bbox_height = loc_f_ymax - glob_pr_sglStatemachine->f_current_ymin;

        sgl_compute_nx_ny(par_f_horiz_pattern, par_f_vert_pattern, SGL_TRUE);

        sgl_compute_dx_dy(par_l_horiz_align, par_l_vert_align, SGL_TRUE);
    }
    else {
        oglxSetError(SGL_ERROR_SGL_FORMATTEXTURE, par_ul_nb_vertices);
    }
    return;
}

/* End of File ***************************************************************/
