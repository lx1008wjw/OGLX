/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglScalef.c
 DESCRIPTION       : sglScalef command shall multiply the current modelview 
					matrix by a general scaling matrix
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
  NAME: sglScalef
  DESCRIPTION:
    Function shall multiply the current modelview matrix.
    by a general scaling matrix.
  PARAMETERS:
    par_f_x -> Scale factor along the x-axis
    par_f_y -> Scale factor along the y-axis
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglScalef(SGLfloat par_f_x, SGLfloat par_f_y)
{
    /*              
       For better readibility the model view matrix glob_pr_sglStatemachine->f_modelview_matrix
       is presented here as fi, i being the index of a value in the matrix.

       The new model view matrix is the matricial product between previous one and scaling matrix:

       |f0          f4              f8              f12|.|par_f_x   0               0               0|
       |f1          f5              f9              f13| |0                 par_f_y 0               0|
       |f2          f6              f10             f14| |0                 0               1               0|
       |f3          f7              f11             f15| |0                 0               0               1|

       =

       |f0*par_f_x          f4*par_f_y              f8              f12|
       |f1*par_f_x          f5*par_f_y              f9              f13|
       |f2*par_f_x          f6*par_f_y              f10             f14|
       |f3*par_f_x          f7*par_f_y              f11             f15|


     */

    glob_pr_sglStatemachine->f_modelview_matrix[0] *= par_f_x;
    glob_pr_sglStatemachine->f_modelview_matrix[1] *= par_f_x;
    glob_pr_sglStatemachine->f_modelview_matrix[2] *= par_f_x;
    glob_pr_sglStatemachine->f_modelview_matrix[3] *= par_f_x;

    if ((par_f_x > SGL_MIN_F) || (par_f_x < -SGL_MIN_F)) {
        glob_pr_sglStatemachine->f_ratio_x = mth_f_abs(SGLfloat_div(glob_pr_sglStatemachine->f_ratio_x, par_f_x));
        glob_pr_sglStatemachine->f_scale_x = mth_f_abs(glob_pr_sglStatemachine->f_scale_x * par_f_x);
        glob_pr_sglStatemachine->f_ratio_scale_x = glob_pr_sglStatemachine->f_ratio_x * glob_pr_sglStatemachine->f_scale_x;
        glob_pr_sglStatemachine->f_ratio_scale_width = glob_pr_sglStatemachine->f_ratio_scale_x * glob_pr_sglStatemachine->f_width_factor;
    }
    else {
        glob_pr_sglStatemachine->f_ratio_x = SGL_DEFAULT_RATIO;
    }

    glob_pr_sglStatemachine->f_modelview_matrix[4] *= par_f_y;
    glob_pr_sglStatemachine->f_modelview_matrix[5] *= par_f_y;
    glob_pr_sglStatemachine->f_modelview_matrix[6] *= par_f_y;
    glob_pr_sglStatemachine->f_modelview_matrix[7] *= par_f_y;

    if ((par_f_y > SGL_MIN_F) || (par_f_y < -SGL_MIN_F)) {
        glob_pr_sglStatemachine->f_ratio_y = mth_f_abs(SGLfloat_div(glob_pr_sglStatemachine->f_ratio_y, par_f_y));
        glob_pr_sglStatemachine->f_scale_y = mth_f_abs(glob_pr_sglStatemachine->f_scale_y * par_f_y);
        glob_pr_sglStatemachine->f_ratio_scale_y = glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_scale_y;
        glob_pr_sglStatemachine->f_ratio_scale_height = glob_pr_sglStatemachine->f_ratio_scale_y * glob_pr_sglStatemachine->f_height_factor;
    }
    else {
        glob_pr_sglStatemachine->f_ratio_y = SGL_DEFAULT_RATIO;
    }

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glScalef(par_f_x, par_f_y, 1.0F);
#else
    oglxLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#endif

    return;
}

/* End of File ***************************************************************/
