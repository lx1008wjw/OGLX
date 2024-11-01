/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEnd.c
 DESCRIPTION       : sglEnd shall end the vertices 
					of a primitive or a group of primitives.
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Project specific includes
 *****************************************************************************/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"
#include "mth.h"


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_close_line_loop
  DESCRIPTION:
    Function shall close a line loop if necessary.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_close_line_loop(void)
{
    /* Only finish the draw in case of a line loop */
    if (glob_pr_sglStatemachine->b_close_line_loop) {
        if (glob_pr_sglStatemachine->b_arc_added) {
            glob_pr_sglStatemachine->b_arc_added = SGL_FALSE;
            sgl_add_arc_segment(glob_pr_sglStatemachine->f_last_point_x, glob_pr_sglStatemachine->f_last_point_y,
                                glob_pr_sglStatemachine->f_line_loop_first_vertex_x, glob_pr_sglStatemachine->f_line_loop_first_vertex_y);
            glob_pr_sglStatemachine->b_close_line_loop = SGL_FALSE;
        }
        else {
            glob_pr_sglStatemachine->b_close_line_loop = SGL_FALSE;
            /* Add the ending point in case of a line stipple */
            if (glob_pr_sglStatemachine->b_stipple_ok || (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH)) {
                oglxVertex2f(glob_pr_sglStatemachine->f_line_loop_first_vertex_x, glob_pr_sglStatemachine->f_line_loop_first_vertex_y);
            }
            else {
                /* Nothing to do */
            }
        }

    }
    else {
        /* Nothing to do */
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_finish_shape_arc
  DESCRIPTION:
    Function shall finish the definition of a shape if it is ended by an arc
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_finish_shape_arc(void)
{
    if (glob_pr_sglStatemachine->b_arc_added) {
        glob_pr_sglStatemachine->b_arc_added = SGL_FALSE;
        sgl_add_arc_segment(glob_pr_sglStatemachine->f_last_point_x, glob_pr_sglStatemachine->f_last_point_y,
                            glob_pr_sglStatemachine->tr_geometry.p_vertices[0][0], glob_pr_sglStatemachine->tr_geometry.p_vertices[0][1]);
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_is_ear
  DESCRIPTION:
    Function shall state if the point at given index is an ear for the ear clipping algorithm
  PARAMETERS:
    par_ul_ear_index -> index of point to check
  RETURN:
    SGLbool -> SGL_TRUE if input point is an ear, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_is_ear(SGLulong par_ul_ear_index)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;
    SGLulong loc_ul_end;
    SGLulong loc_ul_i;
    SGLulong loc_ul_ear_index = par_ul_ear_index;
    SGLbool loc_b_ear = SGL_TRUE;
    SGLfloat loc_pf_points[3][2];

    if (par_ul_ear_index == 0UL) {
        loc_pf_points[0][0] = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1L][0];
        loc_pf_points[0][1] = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1L][1];
        loc_ul_i = 2UL;
        loc_ul_end = SGLint32_to_uint32(loc_l_size - 1L);
    }
    else {
        loc_ul_i = 0UL;
        loc_ul_end = (SGLulong) loc_l_size;
        loc_pf_points[0][0] = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1UL][0];
        loc_pf_points[0][1] = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1UL][1];
    }
    loc_pf_points[1][0] = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][0];
    loc_pf_points[1][1] = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][1];
    if (par_ul_ear_index == SGLint32_to_uint32(loc_l_size - 1L)) {
        loc_ul_i = 1UL;
        loc_ul_end = (SGLulong) (loc_l_size - 2L);
        loc_pf_points[2][0] = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][0];
        loc_pf_points[2][1] = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][1];
    }
    else {
        loc_pf_points[2][0] = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1UL][0];
        loc_pf_points[2][1] = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1UL][1];
    }

    /* No other point of the polygon shall be in the triangle defined by the ear candiate and the preceding and following point */
    while ((loc_ul_i < loc_ul_end) && loc_b_ear) {
        if (loc_ul_i != (loc_ul_ear_index - 1UL)) {
            if (mth_point_in_shape
                (glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][0], glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][1], 3UL,
                 loc_pf_points)) {
                loc_b_ear = SGL_FALSE;
            }
            else {
                loc_ul_i++;
            }
        }
        else {
            loc_ul_i += 3UL;
        }
    }

    /* Only a convex vertex can be an ear */
    if (loc_b_ear) {
        SGLfloat loc_f_x = (loc_pf_points[0][0] + loc_pf_points[2][0]) / 2.0F;
        SGLfloat loc_f_y = (loc_pf_points[0][1] + loc_pf_points[2][1]) / 2.0F;

        /* The point is convex if the center of the segment between the two other points of the triangle are inside the polygon */
        if (!mth_point_in_shape(loc_f_x, loc_f_y, (SGLulong) loc_l_size, glob_pr_sglStatemachine->tr_geometry.p_vertices)) {
            loc_b_ear = SGL_FALSE;
        }
    }

    return loc_b_ear;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_get_first_ear
  DESCRIPTION:
    Function shall return the first ear of current polygon
  PARAMETERS:
    None
  RETURN:
    SGLulong -> Index of first detected ear
---------------------------------------------------------------------+*/
SGLulong sgl_get_first_ear(void)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;
    SGLulong loc_ul_ear_index = 0UL;
    SGLbool loc_b_ear_found = SGL_FALSE;

    while ((loc_ul_ear_index < (SGLulong) loc_l_size) && (!loc_b_ear_found)) {
        loc_b_ear_found = sgl_is_ear(loc_ul_ear_index);

        if (!loc_b_ear_found) {
            loc_ul_ear_index++;
        }
    }

    /* If no ear is found */
    if (loc_ul_ear_index == ((SGLulong) loc_l_size)) {
        loc_ul_ear_index = 0UL;
    }

    return loc_ul_ear_index;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_add_triangle_suppress_ear
  DESCRIPTION:
    Function shall add triangle with edge at index par_ul_ear_index to triangle stack and suppress it from current polygon
  PARAMETERS:
    par_ul_ear_index -> Index of the detected ear
	par_b_texture -> Texture active or not
  RETURN:
    SGLulong -> Size of the remaining polygon
---------------------------------------------------------------------+*/
SGLulong sgl_add_triangle_suppress_ear(SGLulong par_ul_ear_index, SGLbool par_b_texture)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;

    if (glob_ul_triangle_number < SGL_MAX_VERTEX_ARRAY_SIZE) {
        SGLulong loc_ul_i;

        if (par_ul_ear_index == 0UL) {
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1][0];
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_l_size - 1][1];
            if (par_b_texture) {
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_l_size - 1][0];
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_l_size - 1][1];
            }
        }
        else {
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1UL][0];
            glob_t_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index - 1UL][1];
            if (par_b_texture) {
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index - 1UL][0];
                glob_t_texture_triangles[glob_ul_triangle_number].s_p1.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index - 1UL][1];
            }
        }

        glob_t_triangles[glob_ul_triangle_number].s_p2.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][0];
        glob_t_triangles[glob_ul_triangle_number].s_p2.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index][1];

        if (par_b_texture) {
            glob_t_texture_triangles[glob_ul_triangle_number].s_p2.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index][0];
            glob_t_texture_triangles[glob_ul_triangle_number].s_p2.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index][1];
        }

        if (par_ul_ear_index < (SGLulong) (loc_l_size - 1L)) {
            glob_t_triangles[glob_ul_triangle_number].s_p3.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1UL][0];
            glob_t_triangles[glob_ul_triangle_number].s_p3.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[par_ul_ear_index + 1UL][1];
            if (par_b_texture) {
                glob_t_texture_triangles[glob_ul_triangle_number].s_p3.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index + 1UL][0];
                glob_t_texture_triangles[glob_ul_triangle_number].s_p3.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[par_ul_ear_index + 1UL][1];
            }
        }
        else {
            glob_t_triangles[glob_ul_triangle_number].s_p3.f_x = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][0];
            glob_t_triangles[glob_ul_triangle_number].s_p3.f_y = glob_pr_sglStatemachine->tr_geometry.p_vertices[0][1];
            if (par_b_texture) {
                glob_t_texture_triangles[glob_ul_triangle_number].s_p3.f_x = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[0][0];
                glob_t_texture_triangles[glob_ul_triangle_number].s_p3.f_y = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[0][1];
            }
        }

        glob_ul_triangle_number++;

        for (loc_ul_i = par_ul_ear_index; loc_ul_i < (SGLulong) (loc_l_size - 1); loc_ul_i++) {
            glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][0] = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i + 1UL][0];
            glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i][1] = glob_pr_sglStatemachine->tr_geometry.p_vertices[loc_ul_i + 1UL][1];
            if (par_b_texture) {
                glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i][0] = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i + 1UL][0];
                glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i][1] = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates[loc_ul_i + 1UL][1];
            }
        }

        glob_pr_sglStatemachine->tr_geometry.l_size = loc_l_size - 1;
        glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = loc_l_size - 1;
    }

    return (SGLulong) (glob_pr_sglStatemachine->tr_geometry.l_size);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_triangles
  DESCRIPTION:
    Function shall draw the triangles of the triangle stack
  PARAMETERS:
	par_b_texture -> Texture active or not
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_triangles(SGLbool par_b_texture)
{
    sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
    sgl_vector2d *loc_t_tex_coord_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
    SGLulong loc_ul_i;
    for (loc_ul_i = 0UL; loc_ul_i < glob_ul_triangle_number; loc_ul_i++) {
        sgl_triangle loc_s_triangle = glob_t_triangles[loc_ul_i];
        loc_t_vertex_array[0][0] = loc_s_triangle.s_p1.f_x;
        loc_t_vertex_array[0][1] = loc_s_triangle.s_p1.f_y;
        loc_t_vertex_array[1][0] = loc_s_triangle.s_p2.f_x;
        loc_t_vertex_array[1][1] = loc_s_triangle.s_p2.f_y;
        loc_t_vertex_array[2][0] = loc_s_triangle.s_p3.f_x;
        loc_t_vertex_array[2][1] = loc_s_triangle.s_p3.f_y;
        if (par_b_texture) {
            sgl_triangle loc_s_texture_triangle = glob_t_texture_triangles[loc_ul_i];
            loc_t_tex_coord_array[0][0] = loc_s_texture_triangle.s_p1.f_x;
            loc_t_tex_coord_array[0][1] = loc_s_texture_triangle.s_p1.f_y;
            loc_t_tex_coord_array[1][0] = loc_s_texture_triangle.s_p2.f_x;
            loc_t_tex_coord_array[1][1] = loc_s_texture_triangle.s_p2.f_y;
            loc_t_tex_coord_array[2][0] = loc_s_texture_triangle.s_p3.f_x;
            loc_t_tex_coord_array[2][1] = loc_s_texture_triangle.s_p3.f_y;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
            oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_FAN, 3L, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
#else
            if (glob_pr_sglStatemachine->b_texture_state) {
                if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
                    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
                    if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                        oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, 3L, &(loc_t_vertex_array[0][0]), SGL_TRUE);
                    }
                    else {
                        oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, 3L, &(loc_t_vertex_array[0][0]), SGL_FALSE);
                    }
                }
            }
            else {
                oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, 3L, &(loc_t_vertex_array[0][0]), SGL_FALSE);
            }
#endif

        }
        else {
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
            oglxDrawVertexArray((SGLulong) GL_TRIANGLE_FAN, 3L, &(loc_t_vertex_array[0][0]));
#else
            oglxDrawVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, 3L, &(loc_t_vertex_array[0][0]));
#endif
        }
    }

    /* Reset triangles number */
    glob_ul_triangle_number = 0UL;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_tessellation
  DESCRIPTION:
    Function shall perform a tessellation of current polygon with ear clipping algorithm
  PARAMETERS:
	None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_tessellation(void)
{
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;
    SGLbool loc_b_texture = (glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord > 0L);

    while (loc_l_size > 3) {
        SGLulong loc_ul_ear_index = sgl_get_first_ear();
        loc_l_size = (SGLlong) sgl_add_triangle_suppress_ear(loc_ul_ear_index, loc_b_texture);
    }

    /* Add the last triangle */
    (void) sgl_add_triangle_suppress_ear(1UL, loc_b_texture);

    sgl_draw_triangles(loc_b_texture);

    glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;
    glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_shape
  DESCRIPTION:
    Function shall draw the defined shape
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_shape(void)
{
    SGLbyte loc_b_close_line = 1U;
    SGLbool loc_b_complex_shape = (((glob_pr_sglStatemachine->b_draw_crown) || (glob_pr_sglStatemachine->b_draw_ellipse)
                                    || (glob_pr_sglStatemachine->b_draw_circle)) ? SGL_TRUE : SGL_FALSE);

    if ((!glob_pr_sglStatemachine->b_tessellation) || loc_b_complex_shape) {
        void *loc_p_vertex_array;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        void *loc_p_tex_vertex_array;
#endif
        /* Call the vertex function to give vertices an textures to OpenGL driver */
        if (glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord == 0L) {
            if ((glob_pr_sglStatemachine->ul_currentAction == SGL_POLYGON) || (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH)) {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                oglxDrawVertexArray((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const SGLfloat *) loc_p_vertex_array);
#else
                oglxDrawVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const SGLfloat *) loc_p_vertex_array);
#endif
            }
            else {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                oglxDrawVertexArray((SGLulong) GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const SGLfloat *) loc_p_vertex_array);
#else
                oglxDrawVertexArray_es2((SGLulong) GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size, (const SGLfloat *) loc_p_vertex_array);
#endif
            }
        }
        else {
#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
            SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
#endif
            if ((glob_pr_sglStatemachine->ul_currentAction == SGL_POLYGON) || (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH)) {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
                oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size,
                                       (const SGLfloat *) loc_p_vertex_array, (const SGLfloat *) loc_p_tex_vertex_array);
#else
                if (glob_pr_sglStatemachine->b_texture_state) {
                    if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
                        if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                            oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size,
                                                       (const SGLfloat *) loc_p_vertex_array, SGL_TRUE);
                        }
                        else {
                            oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size,
                                                       (const SGLfloat *) loc_p_vertex_array, SGL_FALSE);
                        }
                    }
                }
                else {
                    oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const SGLfloat *) loc_p_vertex_array,
                                               SGL_FALSE);
                }
#endif
                glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;
            }
            else {
                loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                loc_p_tex_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
                oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size,
                                       (const SGLfloat *) loc_p_vertex_array, (const SGLfloat *) loc_p_tex_vertex_array);
#else
                if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
                    if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                        oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size,
                                                   (const SGLfloat *) loc_p_vertex_array, SGL_TRUE);
                    }
                    else {
                        oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_STRIP, glob_pr_sglStatemachine->tr_geometry.l_size,
                                                   (const SGLfloat *) loc_p_vertex_array, SGL_FALSE);
                    }
                }
#endif
                glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0L;
            }
        }

        if (glob_pr_sglStatemachine->b_polygon_smooth != SGL_TRUE) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->b_transparency != SGL_MAX_ALPHA_8) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if ((glob_pr_sglStatemachine->b_texture_state) || (glob_pr_sglStatemachine->b_enable_gradient)) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        /* Draw a closed line for polygon smooth when no transparency and no texture */
        if (loc_b_close_line != 0U) {
            sgl_polygon_smooth_backup loc_struct_backup;

            sgl_backup_for_polygon_smooth(&loc_struct_backup);

            sgl_draw_line();

            sgl_restore_after_polygon_smooth(&loc_struct_backup);

        }
        else {
            /* Nothing to do */
        }

    }
    else {
        if (glob_pr_sglStatemachine->b_polygon_smooth != SGL_TRUE) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->b_transparency != SGL_MAX_ALPHA_8) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        if ((glob_pr_sglStatemachine->b_texture_state) || (glob_pr_sglStatemachine->b_enable_gradient)) {
            loc_b_close_line = 0U;
        }
        else {
            /* Nothing to do */
        }

        /* Draw a closed line for polygon smooth when no transparency and no texture */
        if (loc_b_close_line != 0U) {
            sgl_polygon_smooth_backup loc_struct_backup;

            sgl_backup_for_polygon_smooth(&loc_struct_backup);

            sgl_draw_line();

            sgl_restore_after_polygon_smooth(&loc_struct_backup);

        }
        else {
            /* Nothing to do */
        }
        sgl_tessellation();
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglEnd
  DESCRIPTION:
    Function shall terminate a primitive description.
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEnd(void)
{
    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    SGLbool loc_b_texture_alpha = SGL_FALSE;

    if (glob_pr_sglStatemachine->b_texture_state) {
        if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
            if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                loc_b_texture_alpha = SGL_TRUE;
            }
        }
    }

    if ((glob_pr_sglStatemachine->tr_geometry.l_size > 1) && (glob_pr_sglStatemachine->ul_currentAction != SGL_NO_VERTEX)) {
        switch (glob_pr_sglStatemachine->ul_currentAction) {
        case SGL_LINES:
        {
            sgl_end_line();
            glob_pr_sglStatemachine->b_sgl_lines = SGL_FALSE;
        }
            break;
        case SGL_LINE_STRIP:
        case SGL_LINE_LOOP:
        {
            sgl_end_line();
        }
            break;
        case SGL_TRIANGLE_STRIP:
        case SGL_POLYGON:
        {
            /* Nothing shall be drawn if a texture of type SGL_BITMAP_ALPHA is defined and if modulate is inactive */
            if ((glob_pr_sglStatemachine->b_texture_state) && (loc_b_texture_alpha) && (!glob_pr_sglStatemachine->b_modulate)) {
                /* Nothing to do */
            }
            else {
                sgl_end_shape();
                /* Restore Depth test if necessary */
                if (glob_pr_sglStatemachine->b_halo_level_state) {
                    oglxEnable((SGLulong) GL_DEPTH_TEST);
                }
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
            SGLbool loc_b_p_smooth_backup = glob_pr_sglStatemachine->b_polygon_smooth;
            glob_pr_sglStatemachine->b_polygon_smooth = SGL_FALSE;
            glob_pr_sglStatemachine->ul_currentAction = SGL_POLYGON;
            sgl_end_shape();
            sglEndMaskDefinition();
            glob_pr_sglStatemachine->b_polygon_smooth = loc_b_p_smooth_backup;
        }
            break;
        default:
        {
            /* Nothing to do */
        }
            break;
        }
    }
    else {
        /* Nothing to do */
    }

    glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_UNDEFINED;

    /* reset the switch of line mode */
    glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_FALSE;
    glob_pr_sglStatemachine->f_last_stipple_length = 0.0F;
    glob_pr_sglStatemachine->ul_last_stipple_index = 0UL;
    glob_pr_sglStatemachine->b_odd_vertex = SGL_FALSE;
    glob_pr_sglStatemachine->tr_geometry.l_size = 0;

    return;
}

/* End of File ***************************************************************/
