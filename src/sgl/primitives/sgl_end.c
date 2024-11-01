/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgl_end.c
 DESCRIPTION       : sgl_end.c provides interfaces to end the drawing of a line 
					of a primitive or a group of primitives.
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/
/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_end_line
  DESCRIPTION:
    Function shall end the drawing of a line.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_end_line(void)
{
    sgl_close_line_loop();

    sgl_draw_line();

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_end_shape
  DESCRIPTION:
    Function shall end the drawing of a shape.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_end_shape(void)
{
    sgl_finish_shape_arc();

    sgl_draw_shape();

    return;
}

/* End of File ***************************************************************/
