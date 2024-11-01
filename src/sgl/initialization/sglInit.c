/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglInit.c
 DESCRIPTION       : sglInit command shall initialize the graphic machine
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

/* default colormap */
static sgl_color glob_s_default_colormap[1] = { {1.0F, 0.50196F, 0.74117F, 1.0F} };

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_clear_opengl_stencil
  DESCRIPTION:
    Function shall clear the OpenGL stencil.
  PARAMETERS:
    None
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_clear_opengl_stencil(void)
{
    glStencilMask(0xFFU);
    glClearStencil(0x0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x0U);

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_clipping
  DESCRIPTION:
    Function shall initialize data used by clipping functions
  PARAMETERS:
    None                
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_initialize_clipping(void)
{
    SGLulong loc_ul_i;

    glob_pr_sglStatemachine->ul_activated_masks = SGL_NO_MASK;

    /************ Initialize SGL and OpenGL clipplanes structures ************/
    for (loc_ul_i = 0UL; loc_ul_i < SGL_MAX_CLIP_PLANES; loc_ul_i++) {
        glob_pr_sglStatemachine->b_clipplane_state[loc_ul_i] = SGL_CP_OFF;
    }

    /************ Clear the OpenGL Stencil ************/
    sgl_clear_opengl_stencil();

#ifndef OGLX_NO_GLCLIPPLANE
    sgl_initialize_clip_plane_structure();
#endif
    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_resources_tables
  DESCRIPTION:
    Function shall initialize color map, line width map, line stipple map, texture table and font table.
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_initialize_resources_tables(void)
{
    SGLulong loc_ul_i;
    SGLulong loc_ul_number_of_textures = glob_pr_sglStatemachine->ul_number_of_textures;
    SGLulong loc_ul_number_of_gradients = glob_pr_sglStatemachine->ul_number_of_gradients;

    glob_pr_sglStatemachine->b_line_rendering_mode = SGL_SMOOTH_LINES;

    /* Initialize textures */
    for (loc_ul_i = 0UL; loc_ul_i < GLF_MAX_NUMBER_OF_TEXTURE; loc_ul_i++) {
        glob_pr_sglStatemachine->tui_texture_id[loc_ul_i] = 0U;
    }

    /* Initialize texture ids table */
    for (loc_ul_i = 0U; loc_ul_i < loc_ul_number_of_textures; loc_ul_i++) {
        glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ui_texture = 0U;
        glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].b_texture_specified_state = SGL_FALSE;
        glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ul_textures_dimension[0] = 0U;
        glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ul_textures_dimension[1] = 0U;
        glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ul_dimension_power_2[0] = 0U;
        glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ul_dimension_power_2[1] = 0U;
        glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].b_textures_app_mode = SGL_CLAMP;
    }

    /* Initialize gradient table */
    for (loc_ul_i = 0U; loc_ul_i < loc_ul_number_of_gradients; loc_ul_i++) {
        glob_pr_sglStatemachine->p_gradient_attrib[loc_ul_i].ui_texture = 0U;
        glob_pr_sglStatemachine->p_gradient_attrib[loc_ul_i].b_texture_specified_state = SGL_FALSE;
        glob_pr_sglStatemachine->p_gradient_attrib[loc_ul_i].ul_textures_dimension[0] = 0U;
        glob_pr_sglStatemachine->p_gradient_attrib[loc_ul_i].ul_textures_dimension[1] = 0U;
        glob_pr_sglStatemachine->p_gradient_attrib[loc_ul_i].ul_dimension_power_2[0] = 0U;
        glob_pr_sglStatemachine->p_gradient_attrib[loc_ul_i].ul_dimension_power_2[1] = 0U;
    }

    for (loc_ul_i = 0U; loc_ul_i < SGL_MAX_STRING_LINES; loc_ul_i++) {
        ts_lines[loc_ul_i].pb_substring = SGL_NULL;
        ts_lines[loc_ul_i].ul_substring_length = 0U;
    }

    sglColorPointerf((const sgl_color *) glob_s_default_colormap, 1UL);

    sglLineWidthPointerf((const sgl_line_width *) font_line_width, FONT_LINE_WIDTH_SIZE);

    sglLineStipplePointer((const sgl_linestipple *) default_line_stipple_table, DEFAULT_LINE_STIPPLE_NUMBER);

    /* The number of fonts shall be initialized to 0 */
    glob_pr_sglStatemachine->ul_nb_of_fonts = 0UL;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_appearance_attributes
  DESCRIPTION:
    Function shall Initialize the appearance attributes of rich text.
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_initialize_appearance_attributes(void)
{
    /* Initialize Rich text appearance */
    glob_pr_sglStatemachine->l_foreground_color_id = -1L;
    glob_pr_sglStatemachine->l_background_color_id = -1L;
    glob_pr_sglStatemachine->l_font_id = -1L;
    glob_pr_sglStatemachine->l_outline_value = -1L;
    glob_pr_sglStatemachine->b_enable_esc_sequence = SGL_TRUE;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_vertex_data
  DESCRIPTION:
    Function shall Initialize the vertex data.
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_initialize_vertex_data(void)
{
    SGLulong loc_ul_i, loc_ul_j;
    SGLfloat loc_f_alpha_offset;
    SGLfloat loc_f_alpha;
    SGLfloat loc_f_angle;
    SGLfloat loc_f_sin = 0.0F;
    SGLfloat loc_f_cos = 0.0F;
    SGLushort loc_ul_nb_lines;
#ifndef SC2_DEV_ENV
    void *loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
    void *loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#endif
    glob_pr_sglStatemachine->p_current_vertex_array = (const void *) glob_pr_sglStatemachine->tr_geometry.p_vertices;

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glTexCoordPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexPointer(2, GL_FLOAT, 0, (const GLfloat *) loc_p_vertex_array);
#endif

#ifdef SC2_DEV_ENV
    glob_pr_sglStatemachine->b_sc2_shader_created = SGL_FALSE;
#endif


#ifdef ES2_DEV_ENV
    glob_pr_sglStatemachine->l_vertex_array_index = OGLX_ES2_VERTEX_ARRAY;
    glob_pr_sglStatemachine->l_texture_array_index = OGLX_ES2_TEX_COORD_ARRAY;

    glVertexAttribPointer((GLuint) (glob_pr_sglStatemachine->l_texture_array_index), 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_tex_vertex_array);
    glVertexAttribPointer((GLuint) (glob_pr_sglStatemachine->l_vertex_array_index), 2, GL_FLOAT, GL_FALSE, 0, (const GLfloat *) loc_p_vertex_array);
#endif
    /* Initialize global vertex arrays */
    glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
    glob_pr_sglStatemachine->tr_geometry.l_size_array_vertices = 0L;
    glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;
    glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_STRIP;

    for (loc_ul_i = 0UL; loc_ul_i < SGL_MAX_VERTEX_ARRAY_SIZE; loc_ul_i++) {
        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][0] = 0.0F;
        glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][1] = 0.0F;
        glob_pr_sglStatemachine->tr_geometry.p_array_vertices[loc_ul_i][0] = 0.0F;
        glob_pr_sglStatemachine->tr_geometry.p_array_vertices[loc_ul_i][1] = 0.0F;
        glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i][0] = 0.0F;
        glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i][1] = 0.0F;
    }

    glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_FALSE;
    glob_pr_sglStatemachine->l_line_caps_appearance = SGL_ROUNDED_CAPS;
    glob_pr_sglStatemachine->f_line_loop_first_vertex_x = 0.0F;
    glob_pr_sglStatemachine->f_line_loop_first_vertex_y = 0.0F;
    glob_pr_sglStatemachine->b_sgl_lines = SGL_FALSE;

    glob_pr_sglStatemachine->b_arc_clockwise = SGL_TRUE;
    glob_pr_sglStatemachine->f_arc_angle = 90.0F;
    glob_pr_sglStatemachine->b_arc_added = SGL_FALSE;
    glob_pr_sglStatemachine->f_last_point_x = 0.0F;
    glob_pr_sglStatemachine->f_last_point_y = 0.0F;
    glob_pr_sglStatemachine->b_drawing_arc = SGL_FALSE;
    glob_pr_sglStatemachine->b_draw_circle = SGL_FALSE;
    glob_pr_sglStatemachine->b_draw_ellipse = SGL_FALSE;
    glob_pr_sglStatemachine->b_draw_crown = SGL_FALSE;

    glob_pr_sglStatemachine->f_last_stipple_length = 0.0F;
    glob_pr_sglStatemachine->ul_last_stipple_index = 0UL;
    glob_pr_sglStatemachine->b_odd_vertex = SGL_FALSE;

    for (loc_ul_i = 0UL; loc_ul_i < ((SGLulong) OGLX_MAX_LINES); loc_ul_i++) {
        for (loc_ul_j = 0UL; loc_ul_j < ((SGLulong) OGLX_MAX_LINES); loc_ul_j++) {
            glob_pr_sglStatemachine->pf_sin[loc_ul_i][loc_ul_j] = 0.0F;
            glob_pr_sglStatemachine->pf_cos[loc_ul_i][loc_ul_j] = 0.0F;
        }
    }

    loc_f_alpha_offset = 10.0F;

    for (loc_ul_nb_lines = 1U; loc_ul_nb_lines < ((SGLushort) OGLX_MAX_LINES); loc_ul_nb_lines++) {
        loc_f_alpha = SGLfloat_div(180.0F - (2.0F * loc_f_alpha_offset), (SGLfloat) loc_ul_nb_lines + 1.0F);

        for (loc_ul_j = 1UL; loc_ul_j <= loc_ul_nb_lines; loc_ul_j++) {
            loc_f_angle = loc_f_alpha_offset + (loc_f_alpha * (SGLfloat) loc_ul_j);
            mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
            glob_pr_sglStatemachine->pf_sin[loc_ul_nb_lines][loc_ul_j] = loc_f_sin;
            glob_pr_sglStatemachine->pf_cos[loc_ul_nb_lines][loc_ul_j] = loc_f_cos;
        }
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_texture_data
  DESCRIPTION:
    Function shall Initialize the texture data.
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_initialize_texture_data(void)
{
    SGLulong loc_ul_i;
    SGLulong loc_ul_buffer_size = ((4UL * glob_pr_sglStatemachine->ul_max_texture_width) * glob_pr_sglStatemachine->ul_max_texture_height);

    glob_pr_sglStatemachine->i_texture_internal_format = (SGLlong) GL_RGB;
    glob_pr_sglStatemachine->i_alpha_texture_internal_format = (SGLlong) GL_RGBA;

    /* Initialize the texture buffers */
    /* tub_texture_buffer can not be null if ul_max_texture_width or ul_max_texture_height is equal to 0 */
    for (loc_ul_i = 0UL; loc_ul_i < loc_ul_buffer_size; loc_ul_i++) {
        glob_pr_sglStatemachine->tub_texture_buffer[loc_ul_i] = 0U;
    }

    for (loc_ul_i = 0UL; loc_ul_i < SGL_MAX_STATIC_TEXTURE; loc_ul_i++) {
        glob_pr_sglStatemachine->s_static_data[loc_ul_i].b_defined = SGL_FALSE;
    }

    glob_pr_sglStatemachine->ul_static_bitmap_number = 0UL;

#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
    glob_pr_sglStatemachine->b_change_viewport = SGL_FALSE;

    glob_pr_sglStatemachine->f_prev_ortho_left = 0.0F;
    glob_pr_sglStatemachine->f_prev_ortho_right = 0.0F;
    glob_pr_sglStatemachine->f_prev_ortho_bottom = 0.0F;
    glob_pr_sglStatemachine->f_prev_ortho_top = 0.0F;

    glob_pr_sglStatemachine->i_prev_x1 = 0L;
    glob_pr_sglStatemachine->i_prev_y1 = 0L;
    glob_pr_sglStatemachine->i_prev_width = 0L;
    glob_pr_sglStatemachine->i_prev_height = 0L;

#endif

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_transformation
  DESCRIPTION:
    Function shall Initialize the transformation data.
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_initialize_transformation(void)
{
    SGLulong loc_ul_i;
    SGLulong loc_ul_j;

    /* Initialize the matrix stack index */
    glob_pr_sglStatemachine->ul_matrix_stack_index = 0U;

    /* Initialize the pitch, current z and translate priority values */
    glob_pr_sglStatemachine->f_ratio_x = SGL_DEFAULT_RATIO;
    glob_pr_sglStatemachine->f_ratio_y = SGL_DEFAULT_RATIO;
    glob_pr_sglStatemachine->f_scale_x = 1.0F;
    glob_pr_sglStatemachine->f_scale_y = 1.0F;
    glob_pr_sglStatemachine->f_ratio_scale_x = 1.0F;
    glob_pr_sglStatemachine->f_ratio_scale_y = 1.0F;
    glob_pr_sglStatemachine->f_rotation_angle = 0.0F;

    glob_pr_sglStatemachine->f_viewport_width_div_2 = ((SGLfloat) glob_pr_sglStatemachine->ul_screen_w) / 2.0F;
    glob_pr_sglStatemachine->f_viewport_height_div_2 = ((SGLfloat) glob_pr_sglStatemachine->ul_screen_h) / 2.0F;
    glob_pr_sglStatemachine->f_width_factor = 1.0F;
    glob_pr_sglStatemachine->f_height_factor = 1.0F;
    glob_pr_sglStatemachine->f_ratio_scale_width = 1.0F;
    glob_pr_sglStatemachine->f_ratio_scale_height = 1.0F;

    glob_pr_sglStatemachine->f_ortho_left = 0.0F;
    glob_pr_sglStatemachine->f_ortho_right = (SGLfloat) glob_pr_sglStatemachine->ul_screen_w;
    glob_pr_sglStatemachine->f_ortho_bottom = 0.0F;
    glob_pr_sglStatemachine->f_ortho_top = (SGLfloat) glob_pr_sglStatemachine->ul_screen_h;

    /* Initialize the projection matrix for OpenGL */
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
#endif

    /* Set default viewport */
    sglViewport(0L, 0L, (SGLlong) glob_pr_sglStatemachine->ul_screen_w, (SGLlong) glob_pr_sglStatemachine->ul_screen_h);

    /* Set orthographic matrix */
    sglOrtho(0.0F, (SGLfloat) glob_pr_sglStatemachine->ul_screen_w, 0.0F, (SGLfloat) glob_pr_sglStatemachine->ul_screen_h);

    /* Initialise SGL matrix stack */

    for (loc_ul_i = 0UL; loc_ul_i < ((SGLulong) SGL_MAX_STACK); loc_ul_i++) {
        for (loc_ul_j = 0UL; loc_ul_j < 16UL; loc_ul_j++) {
            glob_pr_sglStatemachine->tr_stack[loc_ul_i].tf_modelview_matrix[loc_ul_j]
                = glob_pr_sglStatemachine->f_modelview_matrix[loc_ul_j];
        }
        glob_pr_sglStatemachine->tr_stack[loc_ul_i].f_ratio_x = glob_pr_sglStatemachine->f_ratio_x;
        glob_pr_sglStatemachine->tr_stack[loc_ul_i].f_ratio_y = glob_pr_sglStatemachine->f_ratio_y;
        glob_pr_sglStatemachine->tr_stack[loc_ul_i].f_scale_x = 1.0F;
        glob_pr_sglStatemachine->tr_stack[loc_ul_i].f_scale_y = 1.0F;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_is_power_of_two
  DESCRIPTION:
    Function shall check if given integer is a power of two.
  PARAMETERS:
    par_ul_value -> Integer to check                
  RETURN:
    SGLbool -> SGL_TRUE if given integer is a power of two, SGL_FALSE otherwise  
---------------------------------------------------------------------+*/
SGLbool sgl_is_power_of_two(SGLulong par_ul_value)
{
    SGLbool loc_b_return;
    SGLulong loc_ul_value1 = (GLuint) par_ul_value;
    SGLulong loc_ul_value2 = (GLuint) (par_ul_value - 1UL);
    if ((loc_ul_value1 & loc_ul_value2) == 0U) {
        loc_b_return = SGL_TRUE;
    }
    else {
        loc_b_return = SGL_FALSE;
    }

    return loc_b_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_init_detect_error
  DESCRIPTION:
    Function shall check parameters of sglInit.
  PARAMETERS:
    par_ul_screen_width -> screen width
    par_ul_screen_height -> screen height
    par_pb_texture_buffer -> pointer to the texture buffer
    par_ul_texture_max_width -> required maximum texture width
    par_ul_texture_max_height -> required maximum texture height
  RETURN:
    SGLbool -> Error detected (SGL_TRUE) or not (SGL_FALSE)  
---------------------------------------------------------------------+*/
SGLbool sgl_init_detect_error(const sgl_type_statemachine * par_s_context, SGLulong par_ul_screen_width, SGLulong par_ul_screen_height,
                              const SGLbyte * par_pb_texture_buffer, SGLulong par_ul_texture_max_width, SGLulong par_ul_texture_max_height)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    if (par_s_context == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check if screen is at least one user unit large */
    if (par_ul_screen_width == 0UL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check if screen is at least one user unit high */
    if (par_ul_screen_height == 0UL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture buffer */
    if (par_pb_texture_buffer != SGL_NULL) {
        SGLbyte loc_b_texture_size_valid = 1U;
        SGLbool loc_b_tex_width_is_power_of_two = sgl_is_power_of_two(par_ul_texture_max_width);
        SGLbool loc_b_tex_height_is_power_of_two = sgl_is_power_of_two(par_ul_texture_max_height);

        /* Check texture buffer size */

        if (par_ul_texture_max_width < SGL_TEXTURE_MIN_WIDTH) {
            loc_b_texture_size_valid = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (par_ul_texture_max_height < SGL_TEXTURE_MIN_HEIGHT) {
            loc_b_texture_size_valid = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (loc_b_tex_width_is_power_of_two != SGL_TRUE) {
            loc_b_texture_size_valid = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (loc_b_tex_height_is_power_of_two != SGL_TRUE) {
            loc_b_texture_size_valid = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (loc_b_texture_size_valid == 0U) {
            loc_b_error_detected = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        if (par_ul_texture_max_width != 0UL) {
            loc_b_error_detected = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
        if (par_ul_texture_max_height != 0UL) {
            loc_b_error_detected = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
    }

    return loc_b_error_detected;
}

#ifdef ES2_DEV_ENV
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglInit_es2
  DESCRIPTION:
    Initialization of the OpenGL ES2 specific state machine.
  PARAMETERS:
    None          
  RETURN:			
    SGLbyte -> SGL_TRUE if the initialisation is performed, SGL_FALSE if an error is detected and nothing is done.
---------------------------------------------------------------------+*/
SGLbool sglInit_es2(void)
{
    SGLulong loc_ui_i;
    SGLulong loc_ui_j;

    /* Used to hold the fragment and vertex shader handles */
    glob_pr_sglStatemachine->ui_frag_shader = 0U;
    glob_pr_sglStatemachine->ui_vert_shader = 0U;

    /* Used to hold the program handle (made out of the two previous shaders */
    glob_pr_sglStatemachine->ui_program_object = 0U;
    glob_pr_sglStatemachine->i_matrix_location = 0U;

    for (loc_ui_i = 0U; loc_ui_i < SGL_MAX_STACK; loc_ui_i++) {
        for (loc_ui_j = 0U; loc_ui_j < 16U; loc_ui_j++) {
            glob_pr_sglStatemachine->pf_pushed_matrix[loc_ui_j][loc_ui_i] = 0.0F;
        }
    }

    glob_pr_sglStatemachine->ul_pushed_matrix_number = 0UL;
    return sglReset_es2();
}
#endif

#ifdef SC2_DEV_ENV
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglInit_sc2
  DESCRIPTION:
    Initialization of the OpenGL ES2 specific state machine.
  PARAMETERS:
    None          
  RETURN:			
    None
---------------------------------------------------------------------+*/
SGLbool sglInit_sc2(void)
{
    SGLulong loc_ui_i;
    SGLulong loc_ui_j;

    /* Used to hold the program handle (made out of the two previous shaders */
    glob_pr_sglStatemachine->ui_program_object = 0U;
    glob_pr_sglStatemachine->i_matrix_location = 0U;

    for (loc_ui_i = 0U; loc_ui_i < SGL_MAX_STACK; loc_ui_i++) {
        for (loc_ui_j = 0U; loc_ui_j < 16U; loc_ui_j++) {
            glob_pr_sglStatemachine->pf_pushed_matrix[loc_ui_j][loc_ui_i] = 0.0F;
        }
    }

    glob_pr_sglStatemachine->ul_pushed_matrix_number = 0UL;

    return sglReset_sc2();
}
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglInit
  DESCRIPTION:
    Function shall initialize the graphic machine.
  PARAMETERS:
    par_s_context -> context structure used by OGLX to keep state machine parameters
    par_p_parameters -> Initialization parameters of OGLX
  RETURN:
    SGLbool -> SGL_TRUE if the initialisation is performed, SGL_FALSE 
    if an error is detected and nothing is done.  
---------------------------------------------------------------------+*/
SGLbool sglInit(sgl_type_statemachine * par_s_context, const sgl_parameters * par_p_parameters)
{
    SGLulong loc_ul_i;
    SGLbool loc_b_return = SGL_TRUE;

    SGLulong loc_ul_screen_width = par_p_parameters->ul_screen_width;
    SGLulong loc_ul_screen_height = par_p_parameters->ul_screen_height;
    SGLbyte *loc_pb_texture_buffer = par_p_parameters->pb_texture_buffer;
    SGLulong loc_ul_texture_max_width = par_p_parameters->ul_texture_max_width;
    SGLulong loc_ul_texture_max_height = par_p_parameters->ul_texture_max_height;

    SGLbool loc_b_error_detected = sgl_init_detect_error(par_s_context, loc_ul_screen_width, loc_ul_screen_height,
                                                         loc_pb_texture_buffer, loc_ul_texture_max_width, loc_ul_texture_max_height);

    /* Check parameters of sglInit */
    if (loc_b_error_detected) {
        loc_b_return = SGL_FALSE;
    }
    else {
        glob_pr_sglStatemachine = par_s_context;

        glob_pr_sglStatemachine->b_static_sequence_started = SGL_FALSE;
        /* Set global textures data */
        glob_pr_sglStatemachine->tub_texture_buffer = loc_pb_texture_buffer;
        glob_pr_sglStatemachine->ul_max_texture_width = loc_ul_texture_max_width;
        glob_pr_sglStatemachine->ul_max_texture_height = loc_ul_texture_max_height;

        glob_pr_sglStatemachine->p_texture_attrib = par_p_parameters->p_texture_attrib;
        glob_pr_sglStatemachine->ul_number_of_textures = par_p_parameters->ul_number_of_textures;

        glob_pr_sglStatemachine->p_gradient_attrib = par_p_parameters->p_gradient_attrib;
        glob_pr_sglStatemachine->ul_number_of_gradients = par_p_parameters->ul_number_of_gradients;

        /* Initialise screen width and height */
        glob_pr_sglStatemachine->ul_screen_w = loc_ul_screen_width;
        glob_pr_sglStatemachine->ul_screen_h = loc_ul_screen_height;

#ifdef SC2_DEV_ENV
        /* Binary format for SC2 shaders loading */
        glob_pr_sglStatemachine->ul_binary_format = par_p_parameters->ul_binary_format;
#else
        glob_pr_sglStatemachine->ul_binary_format = 0UL;
#endif

        glob_pr_sglStatemachine->ul_error_counter = 0UL;

        /* Initialize error table */
        for (loc_ul_i = 0UL; loc_ul_i < SGL_ERROR_MAX; loc_ul_i++) {
            glob_pr_sglStatemachine->tul_error_table[loc_ul_i][0] = 0UL;
            glob_pr_sglStatemachine->tul_error_table[loc_ul_i][1] = 0UL;
        }
        glob_pr_sglStatemachine->b_overflow_flag = SGL_FALSE;

        /* Initialize sglBegin/sglEnd mode with SGL_NO_VERTEX */
        glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;

        /* Initialize the charset encoding format */
        glob_pr_sglStatemachine->b_charset_encoding = SGL_FORMAT_ASCII;

        /* Enable text adjustment by default */
        glob_pr_sglStatemachine->b_enable_text_adjustment = SGL_TRUE;

        /* Set ASCII Space as default separator */
        glob_pr_sglStatemachine->tb_word_separators[0] = 0x20U;
        for (loc_ul_i = 1UL; loc_ul_i < SGL_MAX_SEPARATORS; loc_ul_i++) {
            glob_pr_sglStatemachine->tb_word_separators[loc_ul_i] = SGL_K_NULL;
        }
        glob_pr_sglStatemachine->ul_nb_word_separators = 1UL;

        /* Set ASCII Carriage return as default separator */
        glob_pr_sglStatemachine->tb_line_separators[0] = SGL_K_LF;
        glob_pr_sglStatemachine->tb_line_separators[1] = SGL_K_CR;
        for (loc_ul_i = 2UL; loc_ul_i < SGL_MAX_SEPARATORS; loc_ul_i++) {
            glob_pr_sglStatemachine->tb_line_separators[loc_ul_i] = SGL_K_NULL;
        }
        glob_pr_sglStatemachine->ul_nb_line_separators = 2UL;

        sgl_initialize_appearance_attributes();

        sgl_initialize_resources_tables();

        sgl_initialize_texture_data();

        sgl_initialize_vertex_data();

        sgl_initialize_clipping();

        sgl_initialize_transformation();

        /* Call sglReset to initialiase internal states */
        sglReset();

        /* Load default font */
        sgluLoadFonts(SGL_NULL);

    }

#ifdef ES2_DEV_ENV

    /* Specific ES2 Initialization */
    if (loc_b_return) {
        loc_b_return = sglInit_es2();
    }
    else {
        /* Nothing to do */
    }
#endif

#ifdef SC2_DEV_ENV

    /* Specific SC2 Initialization */
    if (loc_b_return) {
        loc_b_return = sglInit_sc2();
    }
    else {
        /* Nothing to do */
    }
#endif

    return loc_b_return;
}

/* End of File ***************************************************************/
