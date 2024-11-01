/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexFontLineWidthi.c
 DESCRIPTION       : sglIndexLineWidthi command shall set the current linewidth 
					index
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
  NAME: sglIndexFontLineWidthi
  DESCRIPTION:
    Function shall set the current font linewidth index.
  PARAMETERS:
    par_l_index -> linewidth index, range: [0,7]
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglIndexFontLineWidthi(SGLlong par_l_index)
{
    SGLlong loc_l_index = par_l_index;
    if (loc_l_index >= 0) {
        /* Report an error and set default value if input parameter is invalid */
        if (loc_l_index >= ((SGLlong) FONT_LINE_WIDTH_SIZE)) {
            oglxSetError(SGL_ERROR_SGL_INDEXFONTLINEWIDTHI, (SGLulong) loc_l_index);
            loc_l_index = (SGLlong) (FONT_LINE_WIDTH_SIZE - 1U);
        }
        else {
            /* Nothing to do */
        }

        /* Set index for Bitmap fonts */
        if (loc_l_index < (SGLlong) 1) {
            glob_pr_sglStatemachine->b_current_font_linewidth = 0U;
        }
        else {
            glob_pr_sglStatemachine->b_current_font_linewidth = SGLint32_to_uint8(loc_l_index - 1L);
        }

        /* Set index for Stroke fonts */
        glob_pr_sglStatemachine->l_current_font_linewidth_index = loc_l_index;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXFONTLINEWIDTHI, (SGLulong) loc_l_index);
    }

    return;
}

/* End of File ***************************************************************/
