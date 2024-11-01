/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglClipPlane.c
 DESCRIPTION       : sglClipPlane command shall define a plane for clipping
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

#ifndef OGLX_NO_GLCLIPPLANE

/*+ DEFINITION OF GLOBAL VARIABLES -------------------------------------------------------
 DESCRIPTION       : Definition of global variables of sglClipPlane.c
 LLR: [OGLX-DDD-sglClipPlane_globals]
---------------------------------------------------------------------------- +*/
/* Clip planes global structure */
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
static SGLulong glob_ul_gl_clipplane_code[6];
#endif
#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
static sgl_clip_equation pf_clipPlane[6];
#endif
/*+ END OF DEFINITION OF GLOBAL VARIABLES -------------------------------------------------------*/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_disable_clip_planes
  DESCRIPTION:
    Function shall disable all clip planes.
  PARAMETERS:
    void
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_disable_clip_planes(void)
{
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glDisable(GL_CLIP_PLANE5);
#endif
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_enable_clip_plane
  DESCRIPTION:
    Function shall enable given clip plane.
  PARAMETERS:
    par_ul_cap -> clip plane id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_enable_clip_plane(SGLulong par_ul_cap)
{
    SGLbyte loc_b_number;
    loc_b_number = sgl_get_clip_plane_number(par_ul_cap);

    if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] != SGL_CP_OFF) {
        if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] == SGL_CP_AVAILABLE) {
            glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] = SGL_CP_ENABLE;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
            glEnable((GLenum) (glob_ul_gl_clipplane_code[loc_b_number]));
#else
            glUniform1i((GLint) (glob_pr_sglStatemachine->i_clipPlane_active[loc_b_number]), 1);
#endif
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) par_ul_cap);
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_disable_clip_plane
  DESCRIPTION:
    Function shall disable given clip plane.
  PARAMETERS:
    par_ul_cap -> clip plane id
  RETURN:
    void
---------------------------------------------------------------------+*/
void sgl_disable_clip_plane(SGLulong par_ul_cap)
{
    /* Set clip plane status to disabled in OGLX context */
    SGLbyte loc_b_number;
    loc_b_number = sgl_get_clip_plane_number(par_ul_cap);
    if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] != SGL_CP_OFF) {
        if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] == SGL_CP_ENABLE) {
            glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] = SGL_CP_AVAILABLE;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
            glDisable((GLenum) (glob_ul_gl_clipplane_code[loc_b_number]));
#else
            glUniform1i((GLint) (glob_pr_sglStatemachine->i_clipPlane_active[loc_b_number]), 0);
#endif
        }
        else {
            /* Nothing to do */
        }
    }
    else {
#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
        glUniform1i((GLint) (glob_pr_sglStatemachine->i_clipPlane_active[loc_b_number]), 0);
#endif
    }

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_initialize_clip_plane_structure
  DESCRIPTION:
    Function shall initialize clip planes structure.
  PARAMETERS:
    None
  RETURN:
    void 
---------------------------------------------------------------------+*/
void sgl_initialize_clip_plane_structure(void)
{
    /* Initialize Clip plane table */
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glob_ul_gl_clipplane_code[0] = (SGLulong) GL_CLIP_PLANE0;
    glob_ul_gl_clipplane_code[1] = (SGLulong) GL_CLIP_PLANE1;
    glob_ul_gl_clipplane_code[2] = (SGLulong) GL_CLIP_PLANE2;
    glob_ul_gl_clipplane_code[3] = (SGLulong) GL_CLIP_PLANE3;
    glob_ul_gl_clipplane_code[4] = (SGLulong) GL_CLIP_PLANE4;
    glob_ul_gl_clipplane_code[5] = (SGLulong) GL_CLIP_PLANE5;
#endif
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_get_clip_plane_number
  DESCRIPTION:
    Function shall define a plane for clipping.
  PARAMETERS:
    par_l_number -> Clipplane to define
  RETURN:
    SGLbyte -> the index of clip plane in global table  
---------------------------------------------------------------------+*/
SGLbyte sgl_get_clip_plane_number(SGLulong par_ul_number)
{
    SGLbyte loc_b_result;

    switch (par_ul_number) {
    case SGL_CLIP_PLANE0:
        loc_b_result = 0U;
        break;
    case SGL_CLIP_PLANE1:
        loc_b_result = 1U;
        break;
    case SGL_CLIP_PLANE2:
        loc_b_result = 2U;
        break;
    case SGL_CLIP_PLANE3:
        loc_b_result = 3U;
        break;
    case SGL_CLIP_PLANE4:
        loc_b_result = 4U;
        break;
    default:
        /*case SGL_CLIP_PLANE5: */
        loc_b_result = 5U;
        break;
    }

    return loc_b_result;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_clip_plane
  DESCRIPTION:
    Function shall define a plane for clipping.
  PARAMETERS:
    par_ul_number -> Clipplane to define, range: [0,5]
    par_pf_data -> Address of the array saving the clipplane equation
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sgl_clip_plane(SGLulong par_ul_number, const SGLfloat * par_pf_data)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    if ((par_ul_number != SGL_CLIP_PLANE0) && (par_ul_number != SGL_CLIP_PLANE1)
        && (par_ul_number != SGL_CLIP_PLANE2) && (par_ul_number != SGL_CLIP_PLANE3)
        && (par_ul_number != SGL_CLIP_PLANE4) && (par_ul_number != SGL_CLIP_PLANE5)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLbyte loc_b_number;

        loc_b_number = sgl_get_clip_plane_number(par_ul_number);

        /* There are SGL_MAX_CLIP_PLANES maximum */
        if (glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] != SGL_CP_ENABLE) {
            /* Store the plane equation */

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV) && !defined(ES11_DEV_ENV)
            GLdouble loc_td_equation[4];
            loc_td_equation[0] = (GLdouble) par_pf_data[0];
            loc_td_equation[1] = (GLdouble) par_pf_data[1];
            loc_td_equation[2] = (GLdouble) 0.0;
            loc_td_equation[3] = (GLdouble) par_pf_data[3];
            glClipPlane((GLenum) (glob_ul_gl_clipplane_code[loc_b_number]), loc_td_equation);
#endif

#ifdef ES11_DEV_ENV
            GLfloat loc_tf_equation[4];
            loc_tf_equation[0] = par_pf_data[0];
            loc_tf_equation[1] = par_pf_data[1];
            loc_tf_equation[2] = 0.0F;
            loc_tf_equation[3] = par_pf_data[3];
            glClipPlanef((GLenum) (glob_ul_gl_clipplane_code[loc_b_number]), loc_tf_equation);
#endif

#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)

            pf_clipPlane[loc_b_number][0] = par_pf_data[0];
            pf_clipPlane[loc_b_number][1] = par_pf_data[1];
            pf_clipPlane[loc_b_number][2] = 0.0F;
            pf_clipPlane[loc_b_number][3] = par_pf_data[3];

            glUniform4fv((GLint) (glob_pr_sglStatemachine->i_clipPlane_uniform[loc_b_number]), 1, pf_clipPlane[loc_b_number]);
#endif
            glob_pr_sglStatemachine->b_clipplane_state[loc_b_number] = SGL_CP_AVAILABLE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CLIPPLANE, par_ul_number);
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglClipPlane
  DESCRIPTION:
    Function shall define a plane for clipping.
  PARAMETERS:
    par_ul_number -> Clipplane to define, range: [0,5]
    par_f_start_point_x -> X coordinate of start point
    par_f_start_point_y -> Y coordinate of start point
    par_f_angle -> Angle of the clip plane
    par_b_clockwise -> Orientation of the angle (counter clocwise if SGL_FALSE, clockwise otherwise)
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglClipPlane(SGLulong par_ul_number, SGLfloat par_f_start_point_x, SGLfloat par_f_start_point_y, SGLfloat par_f_angle, SGLbool par_b_clockwise)
{
    SGLfloat loc_pf_equation[4];
    SGLfloat loc_f_side;
    SGLfloat loc_f_startx = par_f_start_point_x;
    SGLfloat loc_f_starty = par_f_start_point_y;
    SGLfloat loc_f_angle = par_f_angle;

    if (par_b_clockwise != SGL_TRUE) {
        loc_f_side = 1.0F;
    }
    else {
        loc_f_side = -1.0F;
    }

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    if (sgluFloatIsEqual(loc_f_angle, 0.0F)) {
        loc_pf_equation[0] = 0.0F;
        loc_pf_equation[1] = -loc_f_side;
        loc_pf_equation[2] = 0.0F;
        loc_pf_equation[3] = loc_f_starty * loc_f_side;
    }
    else {
        if (sgluFloatIsEqual(loc_f_angle, 180.0F)) {
            loc_pf_equation[0] = 0.0F;
            loc_pf_equation[1] = loc_f_side;
            loc_pf_equation[2] = 0.0F;
            loc_pf_equation[3] = -loc_f_starty * loc_f_side;
        }
        else {
            if (sgluFloatIsEqual(loc_f_angle, 90.0F)) {
                loc_pf_equation[0] = loc_f_side;
                loc_pf_equation[1] = 0.0F;
                loc_pf_equation[2] = 0.0F;
                loc_pf_equation[3] = -loc_f_startx * loc_f_side;
            }
            else {
                if (sgluFloatIsEqual(loc_f_angle, 270.0F)) {
                    loc_pf_equation[0] = -loc_f_side;
                    loc_pf_equation[1] = 0.0F;
                    loc_pf_equation[2] = 0.0F;
                    loc_pf_equation[3] = loc_f_startx * loc_f_side;
                }
                else {
                    SGLfloat loc_f_sin = 0.0F;
                    SGLfloat loc_f_cos = 0.0F;
                    mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);

                    loc_pf_equation[0] = loc_f_sin * loc_f_side;
                    loc_pf_equation[1] = -loc_f_cos * loc_f_side;
                    loc_pf_equation[2] = 0.0F;
                    loc_pf_equation[3] = ((loc_f_cos * loc_f_starty) - (loc_f_sin * loc_f_startx)) * loc_f_side;
                }
            }
        }
    }
#endif

#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
    {
        SGLfloat loc_f_sin = 0.0F;
        SGLfloat loc_f_cos = 0.0F;
        SGLfloat loc_f_x, loc_f_y, loc_f_pix_x, loc_f_pix_y;
        SGLfloat loc_f_a, loc_f_b, loc_f_c;
        SGLfloat loc_f_f0 = glob_pr_sglStatemachine->f_modelview_matrix[0];
        SGLfloat loc_f_f1 = glob_pr_sglStatemachine->f_modelview_matrix[1];
        SGLfloat loc_f_f4 = glob_pr_sglStatemachine->f_modelview_matrix[4];
        SGLfloat loc_f_f5 = glob_pr_sglStatemachine->f_modelview_matrix[5];
        SGLfloat loc_f_f12 = glob_pr_sglStatemachine->f_modelview_matrix[12];
        SGLfloat loc_f_f13 = glob_pr_sglStatemachine->f_modelview_matrix[13];
        loc_f_a = par_f_start_point_x;
        loc_f_b = par_f_start_point_y;

        /* Convert clip plane starting point into world coordinates */
        loc_f_startx = (loc_f_a * loc_f_f0) + (loc_f_b * loc_f_f4) + loc_f_f12;
        loc_f_starty = (loc_f_a * loc_f_f1) + (loc_f_b * loc_f_f5) + loc_f_f13;
        /* Take into account of rotation angle if necessary */
        /*loc_f_angle = loc_f_angle + glob_pr_sglStatemachine->f_rotation_angle; */

        mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);

        if (sgluFloatIsEqual(loc_f_angle, 0.0F)) {
            loc_f_a = 0.0F;
            loc_f_b = -loc_f_side;
            loc_f_c = par_f_start_point_y * loc_f_side;
        }
        else {
            if (sgluFloatIsEqual(loc_f_angle, 180.0F)) {
                loc_f_a = 0.0F;
                loc_f_b = loc_f_side;
                loc_f_c = -par_f_start_point_y * loc_f_side;
            }
            else {
                if (sgluFloatIsEqual(loc_f_angle, 90.0F)) {
                    loc_f_a = loc_f_side;
                    loc_f_b = 0.0F;
                    loc_f_c = -par_f_start_point_x * loc_f_side;
                }
                else {
                    if (sgluFloatIsEqual(loc_f_angle, 270.0F)) {
                        loc_f_a = -loc_f_side;
                        loc_f_b = 0.0F;
                        loc_f_c = par_f_start_point_x * loc_f_side;

                    }
                    else {
                        loc_f_a = loc_f_sin * loc_f_side;
                        loc_f_b = -loc_f_cos * loc_f_side;
                        loc_f_c = ((loc_f_cos * par_f_start_point_y) - (loc_f_sin * par_f_start_point_x)) * loc_f_side;
                    }
                }
            }
        }

        /* Compute a second point */
        if (sgluFloatIsEqual(0.0F, loc_f_b)) {
            loc_f_x = par_f_start_point_x;
            loc_f_y = par_f_start_point_y + 10.0F;
        }
        else {
            loc_f_x = par_f_start_point_x + 10.0F;
            loc_f_y = SGLfloat_div(-((loc_f_a * loc_f_x) + loc_f_c), loc_f_b);
        }

        loc_f_pix_x = (loc_f_x * loc_f_f0) + (loc_f_y * loc_f_f4) + loc_f_f12;
        loc_f_pix_y = (loc_f_x * loc_f_f1) + (loc_f_y * loc_f_f5) + loc_f_f13;

        if (sgluFloatIsEqual(0.0F, loc_f_pix_x - loc_f_startx)) {
            loc_pf_equation[0] = 1.0F;
            loc_pf_equation[1] = 0.0F;
            loc_pf_equation[2] = 0.0F;
            loc_pf_equation[3] = -loc_f_startx;
        }
        else {
            loc_pf_equation[0] = SGLfloat_div(loc_f_pix_y - loc_f_starty, loc_f_pix_x - loc_f_startx);
            loc_pf_equation[1] = -1.0F;
            loc_pf_equation[2] = 0.0F;
            loc_pf_equation[3] = SGLfloat_div((loc_f_starty * loc_f_pix_x) - (loc_f_pix_y * loc_f_startx), loc_f_pix_x - loc_f_startx);
        }

        /* Check the direction of the vector */
        if (((loc_f_a * loc_pf_equation[0]) < 0.0F) || ((loc_f_b * loc_pf_equation[1]) < 0.0F)) {
            loc_pf_equation[0] = -(loc_pf_equation[0]);
            loc_pf_equation[1] = -(loc_pf_equation[1]);
            loc_pf_equation[3] = -(loc_pf_equation[3]);
        }
    }
#endif

    sgl_clip_plane(par_ul_number, loc_pf_equation);

    return;
}
#endif
/* End of File ***************************************************************/
