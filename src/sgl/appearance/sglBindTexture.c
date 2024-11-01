/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBindTexture.c
 DESCRIPTION       : sglBindTexture shall set the specified texture as active
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
  NAME: sglBindTexture
  DESCRIPTION:
    Function shall set the specified texture as active.
  PARAMETERS:
    par_b_target -> Texture type, equal to SGL_TEXTURE_2D, any other value is an error.
    par_l_texture_number -> Number of the active texture, range: [0,glob_pr_sglStatemachine->ul_number_of_textures[
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglBindTexture(SGLbyte par_b_target, SGLlong par_l_texture_number)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    /* Check target is the good one */
    if (par_b_target != SGL_TEXTURE_2D) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check texture attribute buffer is not NULL */
    if (glob_pr_sglStatemachine->p_texture_attrib == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        /* Check texture number is valid */
        if ((par_l_texture_number >= (SGLlong) glob_pr_sglStatemachine->ul_number_of_textures) || (par_l_texture_number < 0L)) {
            loc_b_error_detected = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }

        if (!loc_b_error_detected) {
            /* Check given texture id is not already loaded */
            if (!(glob_pr_sglStatemachine->p_texture_attrib[par_l_texture_number].b_texture_specified_state)) {
                oglxSetError(SGL_ERROR_SGL_BINDTEXTURE, (SGLulong) par_l_texture_number);
            }
            else {
                /* Set texture number as active one */
                glob_pr_sglStatemachine->l_last_texture_bound_index = par_l_texture_number;
            }
        }
        else {
            oglxSetError(SGL_ERROR_SGL_BINDTEXTURE, (SGLulong) par_l_texture_number);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_BINDTEXTURE, (SGLulong) par_b_target);
    }

    return;
}

/* End of File ***************************************************************/
