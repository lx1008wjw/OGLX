/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexGradienti.c
 DESCRIPTION       : sglIndexGradienti shall select a texture gradient
 COPYRIGHT (C)     : 2016 Esterel Technologies SAS. All Rights Reserved.
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
  NAME: sglIndexGradienti
  DESCRIPTION:
    Function shall select a texture gradient.
  PARAMETERS:
    par_l_gradient_id -> Index of gradient, range: [0,glob_pr_sglStatemachine->ul_number_of_gradients-1]
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglIndexGradienti(SGLlong par_l_gradient_id)
{
    if (glob_pr_sglStatemachine->p_gradient_attrib != SGL_NULL) {
        SGLbool loc_b_error_detected = SGL_FALSE;
        /* Check texture number is valid */
        if ((par_l_gradient_id >= (SGLlong) glob_pr_sglStatemachine->ul_number_of_gradients) || (par_l_gradient_id < 0L)) {
            loc_b_error_detected = SGL_TRUE;
        }
        else {
            if (!(glob_pr_sglStatemachine->p_gradient_attrib[par_l_gradient_id].b_texture_specified_state)) {
                loc_b_error_detected = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }
        }

        if (!loc_b_error_detected) {
            /* Set texture number as active one */
            glob_pr_sglStatemachine->l_gradient_index = par_l_gradient_id;
        }
        else {
            oglxSetError(SGL_ERROR_SGL_INDEXGRADIENTI, (SGLulong) par_l_gradient_id);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXGRADIENTI, (SGLulong) par_l_gradient_id);
    }

    return;
}

/* End of File ***************************************************************/
