/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglLineWidthPointerf.c
 DESCRIPTION       : sgLineWidthPointer command shall define the linewidth map 
					to be used for smooth lines
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
  NAME: sglLineWidthPointerf
  DESCRIPTION:
    Function shall define the linewidth map to be used for lines.
  PARAMETERS:  
    par_ps_line_width -> Pointer to the linewidth map   
	par_ul_nb_line_width -> number of entries in the line width map
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglLineWidthPointerf(const sgl_line_width * par_ps_line_width, SGLulong par_ul_nb_line_width)
{
    if ((par_ps_line_width != SGL_NULL) && (par_ul_nb_line_width > 0UL)) {
        SGLbool loc_b_error_detected = SGL_FALSE;
        SGLulong loc_ul_i = 0UL;

        while ((loc_ul_i < par_ul_nb_line_width) && (!loc_b_error_detected)) {
            if ((par_ps_line_width[loc_ul_i].f_line_width < 0.0F) || (par_ps_line_width[loc_ul_i].f_line_width > 15.0F)
                || (par_ps_line_width[loc_ul_i].f_color_width < 0.0F) || (par_ps_line_width[loc_ul_i].f_color_width > 15.0F)
                || (par_ps_line_width[loc_ul_i].f_halo_width < 0.0F) || (par_ps_line_width[loc_ul_i].f_halo_width > 15.0F)
                ) {
                loc_b_error_detected = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }
            loc_ul_i++;
        }

        if (!loc_b_error_detected) {
            glob_pr_sglStatemachine->tr_line_width = par_ps_line_width;
            glob_pr_sglStatemachine->ul_nb_line_width = par_ul_nb_line_width;
        }
        else {
            oglxSetError(SGL_ERROR_SGL_LINEWIDTHPOINTERF, 0U);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_LINEWIDTHPOINTERF, 0U);
    }

    return;
}

/* End of File ***************************************************************/
