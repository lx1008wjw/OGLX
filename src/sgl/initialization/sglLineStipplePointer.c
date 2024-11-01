/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglLineStipplePointer.c
 DESCRIPTION       : sglLineStipplePointer command shall define the linestipple 
					map to be used
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

const sgl_linestipple default_line_stipple_table[DEFAULT_LINE_STIPPLE_NUMBER] = {
    {{0U, 0U, 0U, 0U, 0U, 0U}, {0U, 0U, 0U, 0U, 0U, 0U}, 1U, 0U},       /* 0 solid pattern :  */
    {{12U, 12U, 0U, 0U, 0U, 0U}, {1U, 0U, 0U, 0U, 0U, 0U}, 0U, 2U},     /* 1 dashed pattern :  */
    {{8U, 8U, 0U, 0U, 0U, 0U}, {1U, 0U, 0U, 0U, 0U, 0U}, 0U, 2U},       /* 2 dashed pattern :  */
    {{1U, 6U, 0U, 0U, 0U, 0U}, {1U, 0U, 0U, 0U, 0U, 0U}, 0U, 2U},       /* 3 dashed pattern :  */
    {{1U, 8U, 8U, 8U, 0U, 0U}, {1U, 0U, 1U, 0U, 0U, 0U}, 0U, 4U}        /* 4 dashed pattern :  */
};

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglLineStipplePointer
  DESCRIPTION:
    Function shall define the linestipple map to be used.
  PARAMETERS:  
    par_ps_linestipple -> Pointer to the linestipple map  
	par_ul_nb_line_stipple -> Number of entries in par_ps_linestipple
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglLineStipplePointer(const sgl_linestipple * par_ps_linestipple, SGLulong par_ul_nb_line_stipple)
{

    if ((par_ps_linestipple != SGL_NULL) && (par_ul_nb_line_stipple > 0UL)) {
        SGLbool loc_b_error_detected = SGL_FALSE;

        SGLulong loc_ul_i;
        for (loc_ul_i = 0UL; loc_ul_i < par_ul_nb_line_stipple; loc_ul_i++) {
            /* Robustness: check the number of segments is not greater than the number of entries in the table */
            if (par_ps_linestipple[loc_ul_i].b_number > SGL_MAX_STIPPLES_LENGTH) {
                loc_b_error_detected = SGL_TRUE;
            }
        }

        if (!loc_b_error_detected) {
            glob_pr_sglStatemachine->tr_linestipple = par_ps_linestipple;
            glob_pr_sglStatemachine->ul_nb_line_stipple = par_ul_nb_line_stipple;
        }
        else {
            oglxSetError(SGL_ERROR_SGL_LINESTIPPLEPOINTER, 0U);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_LINESTIPPLEPOINTER, 0U);
    }

    return;
}

/* End of File ***************************************************************/
