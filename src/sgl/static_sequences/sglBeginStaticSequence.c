/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBeginStaticSequence.c
 DESCRIPTION       : sglBeginStaticSequence shall begin a sequence of static objects 
 COPYRIGHT (C)     : 2014 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

#ifndef ES11_DEV_ENV
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglBeginStaticSequence
  DESCRIPTION:
    Function shall begin a sequence of static objects 
  PARAMETERS:
    par_f_min_x, par_f_min_y -> lower left corner of the static sequence bounding box relatively to (par_f_origin_x, par_f_origin_y)
	par_f_max_x, par_f_max_y -> upper right corner of the static sequence bounding box relatively to (par_f_origin_x, par_f_origin_y)
  RETURN:
    SGLulong -> the index of the sequence (to use when calling sglCallStaticSequence).
---------------------------------------------------------------------+*/
SGLulong sglBeginStaticSequence(SGLfloat par_f_min_x, SGLfloat par_f_min_y, SGLfloat par_f_max_x, SGLfloat par_f_max_y)
{
    SGLulong loc_ul_return = 0UL;

    if ((par_f_min_x <= par_f_max_x) && (par_f_min_y <= par_f_max_y)) {
        if (!glob_pr_sglStatemachine->b_static_sequence_started) {
#ifdef OGLX_DISPLAY_LISTS
            loc_ul_return = (SGLulong) glGenLists(1);
            glNewList((GLuint) loc_ul_return, GL_COMPILE);

#endif

#ifdef OGLX_FBO
            if (glob_pr_sglStatemachine->ul_static_bitmap_number < SGL_MAX_STATIC_TEXTURE) {
                GLuint loc_ui_framebuffer;
                GLuint loc_ui_texture;
                GLenum loc_e_status;
                GLint loc_i_texture_width = (GLint) (par_f_max_x - par_f_min_x);
                GLint loc_i_texture_height = (GLint) (par_f_max_y - par_f_min_y);
                SGLulong loc_ul_index = glob_pr_sglStatemachine->ul_static_bitmap_number;

                glGenFramebuffers(1, &loc_ui_framebuffer);
                glGenTextures(1, &loc_ui_texture);

                loc_ul_return = (SGLulong) loc_ui_texture;

                glBindTexture(GL_TEXTURE_2D, loc_ui_texture);
#ifndef SC2_DEV_ENV
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                             (GLsizei) (SGLfloat_div
                                        ((SGLfloat) loc_i_texture_width, (glob_pr_sglStatemachine->f_ratio_x * glob_pr_sglStatemachine->f_width_factor)))
                             ,
                             (GLsizei) (SGLfloat_div
                                        ((SGLfloat) loc_i_texture_height, (glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_height_factor))), 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, SGL_NULL);
#else
                {
                    GLsizei loc_l_width = (GLsizei) (SGLfloat_div((SGLfloat) loc_i_texture_width,
                                                                  (glob_pr_sglStatemachine->f_ratio_x * glob_pr_sglStatemachine->f_width_factor)));
                    GLsizei loc_l_height =
                        (GLsizei) (SGLfloat_div
                                   ((SGLfloat) loc_i_texture_height, (glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_height_factor)));

                    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, loc_l_width, loc_l_height);
                }
#endif
                glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_width = (SGLulong) loc_i_texture_width;
                glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_height = (SGLulong) loc_i_texture_height;
                glob_pr_sglStatemachine->s_static_data[loc_ul_index].f_x = par_f_min_x;
                glob_pr_sglStatemachine->s_static_data[loc_ul_index].f_y = par_f_min_y;
                glob_pr_sglStatemachine->s_static_data[loc_ul_index].ul_texture_id = loc_ul_return;
                glob_pr_sglStatemachine->s_static_data[loc_ul_index].b_defined = SGL_TRUE;

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) GL_LINEAR);

                glBindFramebuffer(GL_FRAMEBUFFER, loc_ui_framebuffer);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, loc_ui_texture, 0);

                loc_e_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

                if (loc_e_status == (GLenum) GL_FRAMEBUFFER_COMPLETE) {
                    SGLfloat loc_f_tx = -par_f_min_x;
                    SGLfloat loc_f_ty = -par_f_min_y;

                    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glBlendColor(0.0F, 0.0F, 0.0F, 0.0F);
                    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

                    if (((SGLfloat) loc_i_texture_width > glob_pr_sglStatemachine->f_ortho_right)
                        || ((SGLfloat) loc_i_texture_height > glob_pr_sglStatemachine->f_ortho_top)) {
                        SGLlong lov_l_v_width =
                            SGLint32_div(loc_i_texture_width, SGLfloat_to_int32(glob_pr_sglStatemachine->f_ratio_x * glob_pr_sglStatemachine->f_width_factor));
                        SGLlong loc_l_v_height = SGLint32_div(loc_i_texture_height,
                                                              SGLfloat_to_int32(glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_height_factor));

                        glob_pr_sglStatemachine->b_change_viewport = SGL_TRUE;

                        glob_pr_sglStatemachine->i_prev_x1 = SGLfloat_to_int32(glob_pr_sglStatemachine->f_viewport_x1);
                        glob_pr_sglStatemachine->i_prev_y1 = SGLfloat_to_int32(glob_pr_sglStatemachine->f_viewport_y1);
                        glob_pr_sglStatemachine->i_prev_width = SGLfloat_to_int32(glob_pr_sglStatemachine->f_viewport_width_div_2 * 2.0F);
                        glob_pr_sglStatemachine->i_prev_height = SGLfloat_to_int32(glob_pr_sglStatemachine->f_viewport_height_div_2 * 2.0F);
                        glob_pr_sglStatemachine->f_prev_ortho_left = glob_pr_sglStatemachine->f_ortho_left;
                        glob_pr_sglStatemachine->f_prev_ortho_right = glob_pr_sglStatemachine->f_ortho_right;
                        glob_pr_sglStatemachine->f_prev_ortho_bottom = glob_pr_sglStatemachine->f_ortho_bottom;
                        glob_pr_sglStatemachine->f_prev_ortho_top = glob_pr_sglStatemachine->f_ortho_top;
                        sglViewport(0L, 0L, lov_l_v_width, loc_l_v_height);
                        sglOrtho(0.0F, SGLint32_to_float(loc_i_texture_width), 0.0F, SGLint32_to_float(loc_i_texture_height));
                    }
                    oglxPushMatrix();

                    glob_pr_sglStatemachine->f_modelview_matrix[12] += ((glob_pr_sglStatemachine->f_modelview_matrix[0] * loc_f_tx)
                                                                        + (glob_pr_sglStatemachine->f_modelview_matrix[4] * loc_f_ty));
                    glob_pr_sglStatemachine->f_modelview_matrix[13] += ((glob_pr_sglStatemachine->f_modelview_matrix[1] * loc_f_tx)
                                                                        + (glob_pr_sglStatemachine->f_modelview_matrix[5] * loc_f_ty));

                    oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);

                    glob_pr_sglStatemachine->ul_framebuffer = (SGLulong) loc_ui_framebuffer;

                    loc_ul_return = loc_ul_index;
                    glob_pr_sglStatemachine->ul_static_bitmap_number = loc_ul_index + 1UL;
                }
            }
            else {
                /* Error the maximum number of fbo is reached */
            }
#endif
            glob_pr_sglStatemachine->b_static_sequence_started = SGL_TRUE;
        }
    }
    else {
        /* Error the bounding box is not a valid one */
    }

    return loc_ul_return;
}
#endif
/* End of File ***************************************************************/
