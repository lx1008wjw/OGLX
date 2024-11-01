/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : specific.c
 DESCRIPTION       : Specific interfaces of the application
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

#include "specific.h"

#if defined(COEXECUTION)
extern void Reset();
extern void Step();
#elif defined(ANIMATION)
#include "animation.h"
#endif

/* Generated .H files for resource tables and specification properties */
#include "target_configuration.h"
#include "aol_texture_table.h"
#include "aol_gradient_table.h"
#include "aol_color_table.h"
#include "aol_line_width_table.h"
#include "aol_line_stipple_table.h"
#include "aol_font_table.h"

/* TODO GENERATED INCLUDES: include in this section generated .H and .P files for each layer */
#include "aol_symbology_layer.h"
#include "aol_static_sequences.h"
/* END GENERATED INCLUDES */

extern void BHVR_Step(void);
extern void switch_line_mode();
extern float global_x;
extern float global_y;

static int ANIMATE = 1;

/******************************************************************************
 **                           File variables
 *****************************************************************************/

/* TODO LAYER CONTEXT: define in this section one (plugs) context per included layer */
aol_typ_symbology_layer symbology_layer_ctx;    /* Plugs context for symbology_layer */
/* END LAYER CONTEXT */


/*+ FUNCTIONS DESCRIPTION ----------------------------------------------
    NAME:           getW / getH / getRatioX / getRatioY / getSpecName
    DESCRIPTION:    Return respectively:
                        - the width (in user units) of layers
                        - the height (in user units) of layers
                        - the X ratio of layers
                        - the Y ratio of layers
                        - the specification name
                    as generated in the target_configuration.c file
---------------------------------------------------------------------+*/

int getW()
{
    return target_screen_width;
}

int getH()
{
    return target_screen_height;
}

float getRatioX()
{
    return (float) ratio_x;
}

float getRatioY()
{
    return (float) ratio_y;
}

const char *getSpecName()
{
    return "AllObjects";
}


/*+ FUNCTIONS DESCRIPTION ----------------------------------------------
    NAME:           getColorTable / getLineWidthTable / getLineStippleTable /
                    getFontTable
    DESCRIPTION:    Return respectively:
                        - the color table
                        - the line width table
                        - the line stipple table
                        - the font table
                    as generated from resource tables
---------------------------------------------------------------------+*/

void *getColorTable()
{
    return (void *) aol_color_table;
}

int getColorTableSize()
{
    return aol_color_table_size;
}

void *getLineWidthTable()
{
    return (void *) aol_line_width_table;
}

int getLineWidthTableSize()
{
    return aol_line_width_table_size;
}

void *getLineStippleTable()
{
    return (void *) aol_line_stipple_table;
}

int getLineStippleTableSize()
{
    return aol_line_stipple_table_size;
}

void *getFontTable(void)
{
    return (void *) (&aol_font_table);
}

int getTextureTableSize()
{
    return aol_texture_table_size;
}

int getGradientTableSize()
{
    return aol_gradient_table_size;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           context_init
    DESCRIPTION:    Initializes the context for each included layer
---------------------------------------------------------------------+*/

void context_init()
{
    /* TODO CONTEXT INIT: call the init context function for each included layer */
    aol_symbology_layer_init(&symbology_layer_ctx);
    /* END CONTEXT INIT */

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           draw_init
    DESCRIPTION:    Initialize the drawings for each included layer
---------------------------------------------------------------------+*/

void draw_init()
{
    /* TODO DRAW INIT: call the drawings initialization function for each included layer */
    aol_texture_table();
    aol_gradient_table();
    aol_static_sequences();
    /* END DRAW INIT */

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           draw
    DESCRIPTION:    Draws each included layer
---------------------------------------------------------------------+*/

void draw()
{
    int i;
    /* TODO DRAW LAYERS: call the draw function for each included layer */
    aol_symbology_layer_predraw(&symbology_layer_ctx);
    for (i = 0; i < 8; i++) {
        aol_symbology_layer_draw(&symbology_layer_ctx, i);
    }
    /* END DRAW LAYERS */

    /* TO DO Update interactive plugs */
    BHVR_Step();
    /* END Update interactive plugs */
}

#if defined(ANIMATION)

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           anim_init
    DESCRIPTION:    Initialize the animations for each included layer
---------------------------------------------------------------------+*/

void anim_init()
{
    /* TODO INIT ANIMATION: add in this section the manual initialization code for plugs */
    /* END INIT ANIMATION */

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           animate
    DESCRIPTION:    Computes the animations for each included layer
---------------------------------------------------------------------+*/

void animate()
{
    /* TODO RUN ANIMATION: add in this section the manual computation code for plugs */
    if (ANIMATE) {
    }
    /* END RUN ANIMATION */
}

#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           init_scene
    DESCRIPTION:    Initializes (or resets) the scene drawings.
                    In SCADE Suite COEXECUTION mode, also initializes the
                    SCADE Suite context.
---------------------------------------------------------------------+*/

void init_scene()
{

/* If SCADE Suite COEXECUTION is enabled, plug values are initialized by SCADE Suite */
#if defined(COEXECUTION)
    Reset();

/* Else if ANIMATION only is enabled, plug values are initialized via hand-coded init functions */
#elif defined(ANIMATION)
    context_init();
    anim_init();
    draw_init();

/* Else plug values are default ones */
#else
    context_init();
    draw_init();

#endif

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           draw_scene
    DESCRIPTION:    Draws the scene.
                    In SCADE Suite COEXECUTION mode, also executes the
                    SCADE Suite generated code, and computes plug values
                    from it.
---------------------------------------------------------------------+*/

void draw_scene()
{

/* If SCADE Suite COEXECUTION is enabled, plug values are computed by SCADE Suite */
#if defined(COEXECUTION)
    Step();

/* Else if ANIMATION only is enabled, plug values are computed via hand-coded animation functions */
#elif defined(ANIMATION)
    animate();
    draw();

/* Else plug values remain default ones (no moves) */
#else
    draw();

#endif
}
