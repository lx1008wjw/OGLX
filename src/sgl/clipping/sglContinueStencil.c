/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglContinueStencil.c
 DESCRIPTION       : sglContinueStencil command shall end the draw of current stencil area and begin next area
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
  NAME: sglContinueStencil
  DESCRIPTION:
    Function shall shall end the draw of current stencil area and begin next area
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglContinueStencil(void)
{
    if ((glob_pr_sglStatemachine->ul_currentAction == SGL_MASK1) || (glob_pr_sglStatemachine->ul_currentAction == SGL_MASK2)
        || (glob_pr_sglStatemachine->ul_currentAction == SGL_MASK4) || (glob_pr_sglStatemachine->ul_currentAction == SGL_MASK8)
        || (glob_pr_sglStatemachine->ul_currentAction == SGL_MASK16) || (glob_pr_sglStatemachine->ul_currentAction == SGL_MASK32)
        || (glob_pr_sglStatemachine->ul_currentAction == SGL_MASK64) || (glob_pr_sglStatemachine->ul_currentAction == SGL_MASK128)) {
        void *loc_p_vertex_array;

        sgl_finish_shape_arc();

        /* Call the vertex function to give the stencil definition to OpenGL driver */
        loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglxDrawVertexArray((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const SGLfloat *) loc_p_vertex_array);
#else
        oglxDrawVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, glob_pr_sglStatemachine->tr_geometry.l_size, (const SGLfloat *) loc_p_vertex_array);
#endif
        glob_pr_sglStatemachine->tr_geometry.l_size = 0;
    }
    else {
        /* Nothing to do */
    }
    return;
}

/* End of File ***************************************************************/
