/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgl_intern_data.c
 DESCRIPTION       : Declaration of global structures
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

/*+ DEFINITION OF GLOBAL VARIABLES -------------------------------------------------------
 DESCRIPTION       : Definition of OGLX global variables
 LLR: [OGLX-DDD-sgl_intern_data]
---------------------------------------------------------------------------- +*/

sgl_type_statemachine *glob_pr_sglStatemachine = SGL_NULL;

sgl_line_string ts_lines[SGL_MAX_STRING_LINES];

sgl_triangle glob_t_triangles[SGL_MAX_VERTEX_ARRAY_SIZE];

sgl_triangle glob_t_texture_triangles[SGL_MAX_VERTEX_ARRAY_SIZE];

SGLulong glob_ul_triangle_number;

SGLushort glob_us_string[SGLU_MAX_TEXT_AREA_STRING_LENGTH + 1U];

SGLulong glob_ul_text_error;

SGLulong glob_ul_textarea_error;
/*+ END OF DEFINITION OF GLOBAL VARIABLES -------------------------------------------------------*/


/* End of File ***************************************************************/
