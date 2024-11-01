/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglClipBox.c
 DESCRIPTION       : sglClipBox command shall set a clipbox
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
  NAME: sgl_clip_box_detect_error
  DESCRIPTION: 
	Function shall check parameters of sglClipBox.
  PARAMETERS:          
    par_ul_number -> Clipbox to define (SGL_MASK1, SGL_MASK2, SGL_MASK4, SGL_MASK8, SGL_MASK16, SGL_MASK32, SGL_MASK64, SGL_MASK128)
    par_b_type -> Type of the clipbox (SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE)
    par_f_x_bottom_left -> X-coordinate of the lower left corner
    par_f_y_bottom_left -> Y-coordinate of the lower left corner
    par_f_x_top_right -> X-coordinate of the higher right corner
    par_f_y_top_right -> Y-coordinate of the higher right corner
  RETURN:
    SGLbool -> Error detected (SGL_TRUE) or not (SGL_FALSE)
---------------------------------------------------------------------+*/
SGLbool sgl_clip_box_detect_error(SGLulong par_ul_number, SGLbyte par_b_type, SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left,
                                  SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    if ((par_ul_number != SGL_MASK1) && (par_ul_number != SGL_MASK2) && (par_ul_number != SGL_MASK4) && (par_ul_number != SGL_MASK8)
        && (par_ul_number != SGL_MASK16) && (par_ul_number != SGL_MASK32) && (par_ul_number != SGL_MASK64) && (par_ul_number != SGL_MASK128)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check type is SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE */
    if ((par_b_type != SGL_VISIBLE_INSIDE) && (par_b_type != SGL_VISIBLE_OUTSIDE)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check par_f_x_top_right is different from par_f_x_bottom_left */
    if (((par_f_x_top_right - par_f_x_bottom_left) <= SGL_MIN_F) && ((par_f_x_top_right - par_f_x_bottom_left) >= -SGL_MIN_F)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check par_f_y_top_right is different from par_f_y_bottom_left */
    if (((par_f_y_top_right - par_f_y_bottom_left) <= SGL_MIN_F) && ((par_f_y_top_right - par_f_y_bottom_left) >= -SGL_MIN_F)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglClipBox
  DESCRIPTION: 
	Function shall set a clipbox.
  PARAMETERS:          
    par_ul_number -> Clipbox to define (SGL_MASK1, SGL_MASK2, SGL_MASK4, SGL_MASK8, SGL_MASK16, SGL_MASK32, SGL_MASK64, SGL_MASK128)
    par_b_type -> Type of the clipbox (SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE)
    par_f_x_bottom_left -> X-coordinate of the lower left corner
    par_f_y_bottom_left -> Y-coordinate of the lower left corner
    par_f_x_top_right -> X-coordinate of the higher right corner
    par_f_y_top_right -> Y-coordinate of the higher right corner
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglClipBox(SGLulong par_ul_number, SGLbyte par_b_type, SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right,
                SGLfloat par_f_y_top_right)
{
    sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
    void *loc_p_vertex_array;

    SGLbool loc_b_error_detected =
        sgl_clip_box_detect_error(par_ul_number, par_b_type, par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_top_right);

    if (!loc_b_error_detected) {
        oglx_select_mask(par_ul_number, par_b_type);

        loc_t_vertex_array[0][0] = (GLfloat) par_f_x_bottom_left;
        loc_t_vertex_array[0][1] = (GLfloat) par_f_y_bottom_left;
        loc_t_vertex_array[1][0] = (GLfloat) par_f_x_bottom_left;
        loc_t_vertex_array[1][1] = (GLfloat) par_f_y_top_right;
        loc_t_vertex_array[2][0] = (GLfloat) par_f_x_top_right;
        loc_t_vertex_array[2][1] = (GLfloat) par_f_y_top_right;
        loc_t_vertex_array[3][0] = (GLfloat) par_f_x_top_right;
        loc_t_vertex_array[3][1] = (GLfloat) par_f_y_bottom_left;

        loc_p_vertex_array = loc_t_vertex_array;

        /* Set the ortho for clipbox 2 */
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglxDrawVertexArray((SGLulong) GL_TRIANGLE_FAN, 4, (const SGLfloat *) loc_p_vertex_array);
#else
        oglxDrawVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, 4, (const GLfloat *) loc_p_vertex_array);
#endif

        sglEndMaskDefinition();
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CLIPBOX, par_ul_number);
    }

    return;
}

/* End of File ***************************************************************/
