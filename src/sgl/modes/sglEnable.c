/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEnable.c
 DESCRIPTION       : sglEnable command shall enable SGL modes
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
  NAME: sglEnable
  DESCRIPTION:
    API call to enable SGL states.
  PARAMETERS:
    par_b_cap -> capability to enable (SGL_POLYGON_SMOOTH,
    SGL_LINE_HALOING, SGL_TEXTURE_2D, SGL_HALO_SAME_LEVEL, SGL_MODULATE, SGL_TESSELLATION, SGL_TEXT_POS_ADJUSTMENT, SGL_GRADIENT)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEnable(SGLbyte par_b_cap)
{
    switch (par_b_cap) {
    case SGL_POLYGON_SMOOTH:
    {
        /* Set polygon smooth status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_polygon_smooth = SGL_TRUE;
    }
        break;
    case SGL_LINE_HALOING:
    {
        /* Set haloing status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_haloing_state = (SGLbyte) SGL_TRUE;
    }
        break;
    case SGL_TEXTURE_2D:
    {
        /* Set textures status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_texture_state = SGL_TRUE;
        glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_UNDEFINED;
    }
        break;
    case SGL_MODULATE:
    {
        /* Set modulate status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_modulate = SGL_TRUE;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglxTexEnvi((SGLulong) GL_TEXTURE_ENV, (SGLulong) GL_TEXTURE_ENV_MODE, (SGLlong) GL_MODULATE);
#endif
    }
        break;
    case SGL_TESSELLATION:
    {
        /* Set polygon smooth status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_tessellation = SGL_TRUE;
    }
        break;
    case SGL_HALO_SAME_LEVEL:
    {
        /* Set textures status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_halo_level_state = SGL_TRUE;
        oglxEnable((SGLulong) GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
    }
        break;
    case SGL_TEXT_POS_ADJUSTMENT:
    {
        /* Enable text adjustment to improve rendering of textured fonts */
        glob_pr_sglStatemachine->b_enable_text_adjustment = SGL_TRUE;
    }
        break;
    case SGL_GRADIENT:
    {
        /* Enable gradient application */
        glob_pr_sglStatemachine->b_enable_gradient = SGL_TRUE;
    }
        break;
    default:
    {
        oglxSetError(SGL_ERROR_SGL_ENABLE, (SGLulong) par_b_cap);
    }
        break;
    }

    return;
}

/* End of File ***************************************************************/
