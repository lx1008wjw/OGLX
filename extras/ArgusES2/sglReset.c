/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglReset.c
 DESCRIPTION       : sglReset command shall reset the graphic machine
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

#ifdef ES2_DEV_ENV
#define GL_GCCSO_SHADER_BINARY_COREAVI 0x8D65

static unsigned char shaderData[] =
{
#include "oglx_shader.dat"
};


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglReset_es2
  DESCRIPTION:
    Function shall reset the specicic OpenGL ES 2.0 attributes of the graphic machine.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
SGLbool sglReset_es2(void)
{
    GLint loc_b_linked;
    GLuint shaders[2];
    GLint shaderSize = 0;

    SGLbyte loc_us_return = SGL_TRUE;
    /* Create the fragment shader object */
    glob_pr_sglStatemachine->ui_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if (loc_us_return) {
        /* Loads the vertex shader in the same way */

        glob_pr_sglStatemachine->ui_vert_shader = glCreateShader(GL_VERTEX_SHADER);
		
        if (loc_us_return) {
            /* Create the shader program */
            glob_pr_sglStatemachine->ui_program_object = glCreateProgram();


            /* Attach the fragment and vertex shaders to it */
            glAttachShader(glob_pr_sglStatemachine->ui_program_object, glob_pr_sglStatemachine->ui_frag_shader);
            glAttachShader(glob_pr_sglStatemachine->ui_program_object, glob_pr_sglStatemachine->ui_vert_shader);

			shaders[0] = glob_pr_sglStatemachine->ui_vert_shader;
			shaders[1] = glob_pr_sglStatemachine->ui_frag_shader;
			
			shaderSize = sizeof(shaderData);
			glShaderBinary(2, shaders, GL_GCCSO_SHADER_BINARY_COREAVI, shaderData, shaderSize);
            
            /* Bind the custom vertex attribute "pf_vertex_array" to location VERTEX_ARRAY */
            glBindAttribLocation(glob_pr_sglStatemachine->ui_program_object, OGLX_ES2_VERTEX_ARRAY, "pf_vertex_array");
            glBindAttribLocation(glob_pr_sglStatemachine->ui_program_object, OGLX_ES2_TEX_COORD_ARRAY, "pf_tex_coord");

            /* Link the program */
            glLinkProgram(glob_pr_sglStatemachine->ui_program_object);

            /* Check if linking succeeded in the same way we checked for compilation success */

            glGetProgramiv(glob_pr_sglStatemachine->ui_program_object, GL_LINK_STATUS, &loc_b_linked);

            if (!loc_b_linked) {
                /* Error */
                loc_us_return = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }

            if (loc_us_return) {
                /* Actually use the created program */
                glUseProgram(glob_pr_sglStatemachine->ui_program_object);

                glob_pr_sglStatemachine->i_matrix_location = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_transformation_matrix");
                glUniformMatrix4fv(glob_pr_sglStatemachine->i_matrix_location, 1, GL_FALSE, glob_pr_sglStatemachine->f_modelview_matrix);

                glob_pr_sglStatemachine->i_tex_on_location = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "glob_i_texture_active");
                glob_pr_sglStatemachine->i_tex_on_state = -1;
                glUniform1i(glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "sampler2d"), 0);

				glob_pr_sglStatemachine->i_clipPlane_uniform[0] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_clipPlane0");
				glob_pr_sglStatemachine->i_clipPlane_active[0] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "b_clipPlane0");
				glUniform1i(glob_pr_sglStatemachine->i_clipPlane_active[0], 0);

				glob_pr_sglStatemachine->i_clipPlane_uniform[1] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_clipPlane1");
				glob_pr_sglStatemachine->i_clipPlane_active[1] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "b_clipPlane1");
				glUniform1i(glob_pr_sglStatemachine->i_clipPlane_active[1], 0);

				glob_pr_sglStatemachine->i_clipPlane_uniform[2] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_clipPlane2");
				glob_pr_sglStatemachine->i_clipPlane_active[2] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "b_clipPlane2");
				glUniform1i(glob_pr_sglStatemachine->i_clipPlane_active[2], 0);

				glob_pr_sglStatemachine->i_clipPlane_uniform[3] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_clipPlane3");
				glob_pr_sglStatemachine->i_clipPlane_active[3] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "b_clipPlane3");
				glUniform1i(glob_pr_sglStatemachine->i_clipPlane_active[3], 0);

				glob_pr_sglStatemachine->i_clipPlane_uniform[4] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_clipPlane4");
				glob_pr_sglStatemachine->i_clipPlane_active[4] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "b_clipPlane4");
				glUniform1i(glob_pr_sglStatemachine->i_clipPlane_active[4], 0);

				glob_pr_sglStatemachine->i_clipPlane_uniform[5] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "pf_clipPlane5");
				glob_pr_sglStatemachine->i_clipPlane_active[5] = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "b_clipPlane5");
				glUniform1i(glob_pr_sglStatemachine->i_clipPlane_active[5], 0);

                glEnable(GL_SAMPLE_COVERAGE);
                glSampleCoverage(1.0, GL_FALSE);

                glEnableVertexAttribArray(glob_pr_sglStatemachine->l_vertex_array_index);
                glEnableVertexAttribArray(glob_pr_sglStatemachine->l_texture_array_index);
                glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;

                oglxSetTextureState(0);
                glob_pr_sglStatemachine->tf_current_color_es2[0] = 0.0F;
                glob_pr_sglStatemachine->tf_current_color_es2[1] = 0.0F;
                glob_pr_sglStatemachine->tf_current_color_es2[2] = 0.0F;
                glob_pr_sglStatemachine->tf_current_color_es2[3] = 0.0F;

                glob_pr_sglStatemachine->i_color_location = glGetUniformLocation(glob_pr_sglStatemachine->ui_program_object, "v_color");
                glUniform4fv(glob_pr_sglStatemachine->i_color_location, 1, glob_pr_sglStatemachine->tf_current_color_es2);

                oglxUpdateColor_es2(glob_pr_sglStatemachine->tf_current_color_es2);

                glob_pr_sglStatemachine->ul_framebuffer = 0UL;
            }
            else {
                /* Nothing to do */
            }
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    return loc_us_return;
}
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_text_attributes
  DESCRIPTION:
    Function shall reset the text attributes
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_text_attributes(void)
{
    glob_pr_sglStatemachine->ui_current_font_id = INVALID_FONT_ID;
    glob_pr_sglStatemachine->p_current_font = oglx_get_font(INVALID_FONT_ID);

    glob_pr_sglStatemachine->f_h_alignment = 0.0F;
    glob_pr_sglStatemachine->f_v_alignment = 0.0F;
    glob_pr_sglStatemachine->b_h_alignment = (SGLbyte) SGL_ALIGN_LEFT;
    glob_pr_sglStatemachine->b_v_alignment = (SGLbyte) SGL_ALIGN_BOTTOM;

    glob_pr_sglStatemachine->b_current_font_linewidth = 0U;

    glob_pr_sglStatemachine->l_current_font_linewidth_index = 0L;
    glob_pr_sglStatemachine->l_current_font_color_index = ((SGLushort) glob_pr_sglStatemachine->ul_nb_colors - 1U);
    glob_pr_sglStatemachine->ul_font_id = 0UL;
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_appearance_attributes
  DESCRIPTION:
    Function shall reset the appearance attributes
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_appearance_attributes(void)
{
    glob_pr_sglStatemachine->b_reverse_color = SGL_FALSE;

    /* Reset of global variables for OGLX line width */
    glob_pr_sglStatemachine->f_last_ogl_linewidth = 0.0F;

    /* Reset Alpha value */
    sglTransparency8((SGLlong) SGL_MAX_ALPHA_8);

    /* Reset of internal variables */
    sglIndexColori((SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1UL));
    /* Index 1 is the default halo color (usually black) */
    sglIndexHaloColori(-1L);
    oglxUpdateColor((SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1UL));

    glob_pr_sglStatemachine->l_second_font_color_id = 0L;
    glob_pr_sglStatemachine->l_second_font_id = (SGLlong) INVALID_FONT_ID;
    glob_pr_sglStatemachine->l_second_font_line_width = 0L;

    sglIndexLineWidthi((SGLlong) (glob_pr_sglStatemachine->ul_nb_line_width - 1UL));
    sglIndexLineStipplei((SGLlong) (glob_pr_sglStatemachine->ul_nb_line_stipple - 1UL));

    sglDisable(SGL_MODULATE);

    glob_pr_sglStatemachine->ui_ogl_texture_bound = NO_OGL_TEXTURE_BOUND;
    glob_pr_sglStatemachine->f_current_dx = 0.0F;
    glob_pr_sglStatemachine->f_current_dy = 0.0F;
    glob_pr_sglStatemachine->f_current_nx = 0.0F;
    glob_pr_sglStatemachine->f_current_ny = 0.0F;
    glob_pr_sglStatemachine->f_current_xmin = 0.0F;
    glob_pr_sglStatemachine->f_current_ymin = 0.0F;
    glob_pr_sglStatemachine->f_current_bbox_width = 1.0F;
    glob_pr_sglStatemachine->f_current_bbox_height = 1.0F;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_clipping_attributes
  DESCRIPTION:
    Function shall reset the clipping attributes
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_clipping_attributes(void)
{
    SGLulong loc_ul_i;
    SGLbyte loc_b_clipplane;

    /* Set clipplanes as undefined */
    for (loc_b_clipplane = 0U; loc_b_clipplane < SGL_MAX_CLIP_PLANES; loc_b_clipplane++) {
        glob_pr_sglStatemachine->b_clipplane_state[loc_b_clipplane] = SGL_CP_OFF;
    }

    glob_pr_sglStatemachine->ul_allocated_masks = 0x0UL;

    for (loc_ul_i = 0U; loc_ul_i < SGL_MAX_SCISSORS; loc_ul_i++) {
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].i_x = 0L;
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].i_y = 0L;
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].s_width = 0L;
        glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_i].s_height = 0L;
    }

    glob_pr_sglStatemachine->ul_number_of_scissors = 0UL;

    glob_pr_sglStatemachine->ul_mask_definition = SGL_NO_MASK;

#ifndef OGLX_NO_GLCLIPPLANE
    sgl_disable_clip_planes();
#endif
    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_reset_mode_attributes
  DESCRIPTION:
    Function shall reset the mode attributes
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_reset_mode_attributes(void)
{
    glDepthFunc(GL_LEQUAL);

    glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_blend = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_stencil_test = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test = SGL_FALSE;
    glob_pr_sglStatemachine->r_ogl_state.b_state_depth_test = SGL_FALSE;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glob_pr_sglStatemachine->r_ogl_state.ul_texture_target = GL_TEXTURE_ENV;
    glob_pr_sglStatemachine->r_ogl_state.ul_texture_pname = GL_TEXTURE_ENV_MODE;
    glob_pr_sglStatemachine->r_ogl_state.l_texture_param = GL_REPLACE;
#endif
    glob_pr_sglStatemachine->f_current_depth = 0.0F;

    glDisable(GL_BLEND);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable(GL_SCISSOR_TEST);

    glDisable(GL_STENCIL_TEST);

    /* Reset Open GL states */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Disable all SGL features */
    sglDisable(SGL_POLYGON_SMOOTH);
    sglDisable(SGL_TEXTURE_2D);
    sglDisable(SGL_HALO_SAME_LEVEL);
    sglDisable(SGL_LINE_HALOING);
    sglDisable(SGL_TESSELLATION);
    sglDisable(SGL_GRADIENT);

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglReset
  DESCRIPTION:
    Function shall reset the graphic machine.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglReset(void)
{
    void *loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
    void *loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;

    glob_pr_sglStatemachine->p_current_vertex_array = (const void *) glob_pr_sglStatemachine->tr_geometry.p_vertices;

    /* always use modelview matrix */
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
#endif
    glob_pr_sglStatemachine->b_static_sequence_started = SGL_FALSE;

    /*Delete matrix stack */
    glob_pr_sglStatemachine->ul_matrix_stack_index = 0UL;

    /* Loop should not be closed */
    glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
    glob_pr_sglStatemachine->b_close_line_loop = SGL_FALSE;
    glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_STRIP;
    glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
    glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices = 0L;
    glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;

    glob_pr_sglStatemachine->b_path_type = SGL_NO_PATH;
    glob_pr_sglStatemachine->b_last_command = OGLX_NO_COMMAND;
    glob_pr_sglStatemachine->f_first_path_point_x = 0.0F;
    glob_pr_sglStatemachine->f_first_path_point_y = 0.0F;
    glob_pr_sglStatemachine->f_last_path_point_x = 0.0F;
    glob_pr_sglStatemachine->f_last_path_point_y = 0.0F;
    glob_pr_sglStatemachine->f_last_path_control_x = 0.0F;
    glob_pr_sglStatemachine->f_last_path_control_y = 0.0F;

    sgl_reset_appearance_attributes();

    /* It is set to glob_pr_sglStatemachine->ul_number_of_textures, it means that no texture is bound */
    glob_pr_sglStatemachine->l_last_texture_bound_index = (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures;
    /* Same for gradients */
    glob_pr_sglStatemachine->l_gradient_index = (SGLlong) glob_pr_sglStatemachine->ul_number_of_gradients;

    glob_pr_sglStatemachine->ui_map_index = 0UL;

    /* No vertex given */
    glob_pr_sglStatemachine->b_new_vertex = SGL_FALSE;

    /*Set modelview matrix to the ortho values */
    sglLoadIdentity();

    /* Reset all clipping attributes */
    sgl_reset_clipping_attributes();

    /* Reset all text attributes */
    sgl_reset_text_attributes();

    /* Reset all mode attributes */
    sgl_reset_mode_attributes();

    glob_ul_triangle_number = 0UL;

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glAlphaFunc(GL_GREATER, 0.5F);

    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
#ifndef OGLX_NO_VERTEX_ARRAY
    /* Enable Vertex arrays */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_vertex_array);
#endif

    /* Reset texture environment mode */
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_REPLACE);
#endif
#ifdef ES2_DEV_ENV
    glVertexAttribPointer(glob_pr_sglStatemachine->l_texture_array_index, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexAttribPointer(glob_pr_sglStatemachine->l_vertex_array_index, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_vertex_array);
    glEnableVertexAttribArray(glob_pr_sglStatemachine->l_vertex_array_index);
    glEnableVertexAttribArray(glob_pr_sglStatemachine->l_texture_array_index);
    glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;
#endif
#ifdef SC2_DEV_ENV
    if (glob_pr_sglStatemachine->b_sc2_shader_created) {
        glVertexAttribPointer(glob_pr_sglStatemachine->l_texture_array_index, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_tex_vertex_array);
        glVertexAttribPointer(glob_pr_sglStatemachine->l_vertex_array_index, 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_vertex_array);
        glEnableVertexAttribArray(glob_pr_sglStatemachine->l_vertex_array_index);
        glEnableVertexAttribArray(glob_pr_sglStatemachine->l_texture_array_index);
        glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;
    }
#endif
    glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;

    return;
}

/* End of File ***************************************************************/
