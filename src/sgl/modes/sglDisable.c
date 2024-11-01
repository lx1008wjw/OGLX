/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglDisable.c
 DESCRIPTION       : sglDisable command shall disable SGL modes
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
  NAME: sglDisable
  DESCRIPTION:
    Function shall disable SGL modes.
  PARAMETERS:
    par_b_cap -> capability to disable (SGL_POLYGON_SMOOTH,
    SGL_LINE_HALOING, SGL_TEXTURE_2D, SGL_HALO_SAME_LEVEL, SGL_MODULATE, SGL_TESSELLATION, SGL_TEXT_POS_ADJUSTMENT, SGL_GRADIENT)            
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglDisable(SGLbyte par_b_cap)
{
    switch (par_b_cap) {
    case SGL_POLYGON_SMOOTH:
    {
        /* Set polygon smooth status to disabled in OGLX context */
        glob_pr_sglStatemachine->b_polygon_smooth = SGL_FALSE;
    }
        break;
    case SGL_LINE_HALOING:
    {
        /* Set haloing status to disabled in OGLX context */
        glob_pr_sglStatemachine->b_haloing_state = (SGLbyte) SGL_FALSE;
    }
        break;
    case SGL_TEXTURE_2D:
    {
        /* Set textures status to disabled in OGLX context */
        glob_pr_sglStatemachine->b_texture_state = SGL_FALSE;
        glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_UNDEFINED;
    }
        break;
    case SGL_MODULATE:
    {
        /* Set modulate status to disabled in OGLX context */
        glob_pr_sglStatemachine->b_modulate = SGL_FALSE;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglxTexEnvi((SGLulong) GL_TEXTURE_ENV, (SGLulong) GL_TEXTURE_ENV_MODE, (SGLlong) GL_REPLACE);
#endif
    }
        break;
    case SGL_TESSELLATION:
    {
        /* Set polygon smooth status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_tessellation = SGL_FALSE;
    }
        break;
    case SGL_HALO_SAME_LEVEL:
    {
        /* Set textures status to enabled in OGLX context */
        glob_pr_sglStatemachine->b_halo_level_state = SGL_FALSE;
        oglxDisable((SGLulong) GL_DEPTH_TEST);
    }
        break;
    case SGL_TEXT_POS_ADJUSTMENT:
    {
        /* Disable text adjustment to improve rendering of textured fonts */
        glob_pr_sglStatemachine->b_enable_text_adjustment = SGL_FALSE;
    }
        break;
    case SGL_GRADIENT:
    {
        /* Enable gradient application */
        glob_pr_sglStatemachine->b_enable_gradient = SGL_FALSE;
    }
        break;
    default:
    {
        oglxSetError(SGL_ERROR_SGL_DISABLE, (SGLulong) par_b_cap);
    }
        break;
    }

    return;
}

/* End of File ***************************************************************/
