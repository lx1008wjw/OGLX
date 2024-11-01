/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBegin.c
 DESCRIPTION       : sglBegin shall begin the vertices 
					of a primitive or a group of primitives.
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
  NAME: sgl_set_line_mode
  DESCRIPTION:
    Function shall disable texturing and set line mode.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_set_line_mode(void)
{
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_LINE;

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    oglxDisable((SGLulong) GL_TEXTURE_2D);
#endif

    oglxEnable((SGLulong) GL_BLEND);

    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_set_fill_mode
  DESCRIPTION:
    Function shall enable texturing if a texture is a filled
    shape and disable line mode.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_set_fill_mode(void)
{
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_FILL;

    if (glob_pr_sglStatemachine->b_halo_level_state) {
        oglxDisable((SGLulong) GL_DEPTH_TEST);
    }

    oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);

    /*  If texture state is enabled and the texture bound has a correct ID */
    if (glob_pr_sglStatemachine->b_texture_state) {
        SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
        if (loc_l_last_texture_bound_index < (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) {

            if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
                oglxBindTexture((GLuint) (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].ui_texture));

                /* The bound texture is not the Simple Font texture */
                if ((glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_RGB_TRANSPARENT)
                    || (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA)) {
                    oglxEnable((SGLulong) GL_BLEND);
                }
                else {
                    oglxDisable((SGLulong) GL_BLEND);
                }

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                oglxEnable((SGLulong) GL_TEXTURE_2D);
#endif
            }
        }
    }
    else {
        if (glob_pr_sglStatemachine->b_enable_gradient) {
            SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_gradient_index;
            if (loc_l_last_texture_bound_index < (SGLlong) glob_pr_sglStatemachine->ul_number_of_gradients) {
                if (glob_pr_sglStatemachine->p_gradient_attrib != SGL_NULL) {
                    oglxBindTexture((GLuint) (glob_pr_sglStatemachine->p_gradient_attrib[loc_l_last_texture_bound_index].ui_texture));

                    oglxEnable((SGLulong) GL_BLEND);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                    oglxEnable((SGLulong) GL_TEXTURE_2D);
#endif
                }
            }
        }
        else {
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
            oglxDisable((SGLulong) GL_TEXTURE_2D);
#endif
            oglxEnable((SGLulong) GL_BLEND);
        }
    }

    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglBegin
  DESCRIPTION:
    Function shall set the type of the primitive.
  PARAMETERS:
    par_b_mode ->  specification of primitives that will
    be created from vertices declared between sglBegin and 
    sglEnd. Values: SGL_LINES,SGL_LINE_STRIP, SGL_LINE_LOOP,
    SGL_TRIANGLE_STRIP, SGL_POLYGON, SGL_MASK1, SGL_MASK2, SGL_MASK4, SGL_MASK8,
	SGL_MASK16, SGL_MASK32, SGL_MASK64, SGL_MASK128
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglBegin(SGLbyte par_b_mode)
{
    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    SGLbool loc_b_texture_alpha = SGL_FALSE;
    glob_pr_sglStatemachine->b_new_vertex = SGL_FALSE;

    if (glob_pr_sglStatemachine->b_texture_state) {
        if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
            if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                loc_b_texture_alpha = SGL_TRUE;
            }
        }
    }

    /* Nothing shall be drawn if a texture of type SGL_BITMAP_ALPHA is defined and if modulate is inactive */
    switch (par_b_mode) {
    case SGL_LINES:
    {
        /* Update OGLX context to prepare a draw in SGL_LINES mode, it shall draw lines */
        sgl_set_line_mode();
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINES;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINES;
        glob_pr_sglStatemachine->b_sgl_lines = SGL_TRUE;
    }
        break;
    case SGL_LINE_STRIP:
    {
        /* Update OGLX context to prepare a draw in SGL_LINE_STRIP mode, it is a line mode */
        sgl_set_line_mode();
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINE_STRIP;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_STRIP;
    }
        break;
    case SGL_LINE_LOOP:
    {
        /* Update OGLX context to prepare a draw in SGL_LINE_LOOP mode, it is a line mode */
        sgl_set_line_mode();
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINE_LOOP;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;
        glob_pr_sglStatemachine->b_close_line_loop = SGL_TRUE;
    }
        break;
    case SGL_TRIANGLE_STRIP:
    {
        if ((glob_pr_sglStatemachine->b_texture_state) && (loc_b_texture_alpha) && (!glob_pr_sglStatemachine->b_modulate)) {
            /* Nothing to do */
        }
        else {
            /* Update OGLX context to prepare a draw in SGL_TRIANGLE_STRIP mode, it is a filled mode */
            sgl_set_fill_mode();
            glob_pr_sglStatemachine->ul_currentAction = SGL_TRIANGLE_STRIP;
            /* Set Geometry type to SGL_LINE_LOOP for surrounding line drawing of polygon smooth */
            glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;
        }
    }
        break;
    case SGL_POLYGON:
    {
        if ((glob_pr_sglStatemachine->b_texture_state) && (loc_b_texture_alpha) && (!glob_pr_sglStatemachine->b_modulate)) {
            /* Nothing to do */
        }
        else {
            /* Update OGLX context to prepare a draw in SGL_POLYGON mode, it is a filled mode */
            sgl_set_fill_mode();
            glob_pr_sglStatemachine->ul_currentAction = SGL_POLYGON;
            /* Set Geometry type to SGL_LINE_LOOP for surrounding line drawing of polygon smooth */
            glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_LOOP;
        }
    }
        break;
        /* Stencils */
    case SGL_MASK1:
    case SGL_MASK2:
    case SGL_MASK4:
    case SGL_MASK8:
    case SGL_MASK16:
    case SGL_MASK32:
    case SGL_MASK64:
    case SGL_MASK128:
    {
        oglx_select_mask((SGLulong) par_b_mode, SGL_VISIBLE_INSIDE);
        glob_pr_sglStatemachine->ul_currentAction = (SGLulong) par_b_mode;
    }
        break;
    default:
    {
        /* In default case, no drawing mode is required */
        glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
        oglxSetError(SGL_ERROR_SGL_BEGIN, (SGLulong) par_b_mode);
    }
        break;
    }
    glob_pr_sglStatemachine->tr_geometry.l_size = 0;

    return;
}

/* End of File ***************************************************************/
