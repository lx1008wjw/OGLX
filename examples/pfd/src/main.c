/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : main.c
 DESCRIPTION       : Main file with creation of window, management of mouse 
					and keyboard
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

#include "oglx.h"

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "specific.h"

/* Include utilitary function oglxReadErrors to have a readable output of errors */
#include "oglxReadErrors.h"

/******************************************************************************
 **                           File variables
 *****************************************************************************/

static GLint timer_value = 20;  /* integer: Time (in ms) between two frames */
static GLint fps = 0;           /* integer: Number of displayed Frame Per Second */
static GLint pause = 0;         /* boolean: Enable/disable refresh of drawings */
static GLint rawlinemode = 0;   /* boolean: Enable/disable RAW OpenGL mode for OGLX lines */
static GLenum glErrorStatus;    /* enum: OpenGL error index */

static sgl_type_statemachine glob_s_context;

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_init
    DESCRIPTION:    Complete initialization sequence for OGLX
---------------------------------------------------------------------+*/

void oglx_init()
{
    /* Initialize OGLX */
    static sgl_parameters lParameters;

    /* Initialize OGLX */
    /* 
       The following initialization is done to disable textures (NULL pointer as buffer and sizes set to 0). 
     */
    lParameters.ul_screen_width = getW();
    lParameters.ul_screen_height = getH();
    lParameters.pb_texture_buffer = SGL_NULL;
    lParameters.ul_texture_max_width = 0;
    lParameters.ul_texture_max_height = 0;
    lParameters.p_texture_attrib = SGL_NULL;
    lParameters.ul_number_of_textures = 0;
    lParameters.p_gradient_attrib = SGL_NULL;
    lParameters.ul_number_of_gradients = 0UL;

    sglInit(&glob_s_context, &lParameters);

    /*
       If the user wants to use textures or bitmap fonts, the initialisation shall be (with maximum texture size
       (SGL_TEXTURE_MAX_WIDTH,SGL_TEXTURE_MAX_HEIGHT)):

       static SGLbyte glob_tub_texture_buffer[4 * SGL_TEXTURE_MAX_WIDTH * SGL_TEXTURE_MAX_HEIGHT];
       sgl_texture_attrib *glob_texture_attrib = malloc(sizeof(sgl_texture_attrib) * getTextureTableSize());
       static sgl_parameters lParameters;

       lParameters.ul_screen_width = getW();
       lParameters.ul_screen_height = getH();
       lParameters.pb_texture_buffer = glob_tub_texture_buffer;
       lParameters.ul_texture_max_width = SGL_TEXTURE_MAX_WIDTH;
       lParameters.ul_texture_max_height = SGL_TEXTURE_MAX_HEIGHT;
       lParameters.p_texture_attrib = glob_texture_attrib;
       lParameters.ul_number_of_textures = getTextureTableSize();     
     */

    /* Load the color table */
    sglColorPointerf(getColorTable(), getColorTableSize());

    /* Set the OGLX line mode (RAW/SMOOTH) and load the corresponding line width table */
    if (rawlinemode) {
        sglSetRenderMode(SGL_RAW_OPENGL_LINES);
    }
    else {
        sglSetRenderMode(SGL_SMOOTH_LINES);
    }

    sglLineWidthPointerf(getLineWidthTable(), getLineWidthTableSize());

    /* Load the line stipple table */
    sglLineStipplePointer(getLineStippleTable(), getLineStippleTableSize());

    /* Load the fonts table */
    sgluLoadFonts(getFontTable());

    glShadeModel(GL_FLAT);

    sglViewport(0L, 0L, getW(), getH());
    sglOrtho(0, (float) (getW() * getRatioX() / 1.0F), 0, (float) (getH() * getRatioY() / 1.0F));

    /* Check if there is an OpenGL error after OGLX initialization */
    glErrorStatus = glGetError();
    if (glErrorStatus != GL_NO_ERROR) {
        printf("\nError %d raised during OGLX initialization\n\n", glErrorStatus);
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_reset
    DESCRIPTION:    Resets the OGLX state
---------------------------------------------------------------------+*/

void oglx_reset()
{
    sglReset();
    sglViewport(0L, 0L, getW(), getH());
    sglOrtho(0, (float) (getW() * getRatioX() / 1.0F), 0, (float) (getH() * getRatioY() / 1.0F));

    /* Check if there is an OpenGL error after OGLX reset */
    glErrorStatus = glGetError();
    if (glErrorStatus != GL_NO_ERROR) {
        printf("\nError %d raised during OGLX reset\n\n", glErrorStatus);
    }

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           switch_line_mode
    DESCRIPTION:    Switches the OGLX line mode (RAW or SMOOTH), and
                    reloads the corresponding line width table
---------------------------------------------------------------------+*/

void switch_line_mode()
{
    rawlinemode = !rawlinemode;
    sglTerminate();
    oglx_init();
    draw_init();
}




/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           display_fps
    DESCRIPTION:    Computes and refreshes, every second, the number of
					displayed frames per second (FPS)
---------------------------------------------------------------------+*/

static void display_fps(void)
{
    static int frame = 0;
    static int timebase = 0;
    static char buffer[32];
    static char buffer2[16];
    char *bufptr;
    int time;

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);

    if (time - timebase > 1000) {
        sprintf(buffer, "FPS:%3d", (int) (frame * 1000.0 / (time - timebase)));
        timebase = time;
        frame = 0;
    }

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, getW(), getH(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();
    glRasterPos3f(getW() - 100.0f, 30.0f, 0.0f);
    for (bufptr = buffer; *bufptr; bufptr++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *bufptr);
    }

    if (rawlinemode) {
        sprintf(buffer2, "RAW MODE");
    }
    else {
        sprintf(buffer2, "SMOOTH MODE");
    }
    glRasterPos3f(getW() - 100.0f, 45.0f, 0.0f);
    for (bufptr = buffer2; *bufptr; bufptr++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *bufptr);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    /* Direct OpenGL commands => need to reset OGLX states */
    sglReset();

}


/******************************************************************************
 **                           GLUT callbacks
 *****************************************************************************/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           timer_callback
    DESCRIPTION:    Evaluates the time (in ms) between 2 frames
---------------------------------------------------------------------+*/

static void timer_callback(int value)
{
    if (!pause)
        glutPostRedisplay();
    glutTimerFunc(timer_value > 0 ? timer_value : 1, timer_callback, value);
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           DisplayOGLXErrors
    DESCRIPTION:   Displays OGLX errors to standard output
---------------------------------------------------------------------+*/
void DisplayOGLXErrors()
{
    static SGLuint32 loc_errors_number = 0;
    SGLuint32 loc_last_errors_number = 0;
    SGLuint32 loc_errors[SGL_ERROR_MAX][2];

    /*  OGLX interface sglGetErrors fills the input table with the 32 first errors since the start of the application or since last call to sgl ClearErrors */
    SGLbyte loc_b_status = sglGetErrors((SGLulong *) loc_errors, &loc_last_errors_number);

    if (loc_last_errors_number != loc_errors_number) {
        loc_errors_number = loc_last_errors_number;

        /* Fill the last error index only if there is at least one error */
        if (loc_b_status != SGL_NO_ERROR) {
            SGLulong i = 0;

            printf("**************************************************************\n");
            printf("\n\nNumber of OGLX stored errors: %d\n", (int) loc_errors_number);
            for (i = 0; i < loc_errors_number; i++) {
                /* Utilitary function oglxGetErrorDefinition provides additional informations on the detected errors */
                oglx_error_definition loc_error = oglxGetErrorDefinition(loc_errors[i][0]);
                printf("\nError number %d\n", (int) i + 1);
                /* Print outputs provided by sglGetErrors */
                printf("Error ID: %d, Parameter value: %d\n", (int) loc_errors[i][0], (int) loc_errors[i][1]);
                /* Print outputs provided by oglxReadErrors */
                printf("Error Name: %s\nRationale: %s\nParameter name: %s\nParameter value: %d\n",
                       loc_error.s_error_name, loc_error.s_rationale, loc_error.s_parameter_name, (int) loc_errors[i][1]);
            }
        }
        else {
            printf("**************************************************************\n");
            printf("\n\nNo OGLX error.\n");
        }
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           display_callback
    DESCRIPTION:    Renders each frame
---------------------------------------------------------------------+*/

void display_callback(void)
{
    /* Clear OpenGL colors, masks and buffers */
    glClearColor(0.f, 0.f, 0.f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Render the graphical scene */
    draw_scene();

    /* Check if there is an OpenGL error after drawing of the scene */
    glErrorStatus = glGetError();
    if (glErrorStatus != GL_NO_ERROR) {
        printf("\nError %d raised during scene rendering\n\n", glErrorStatus);
    }

    /* When enabled, display (in overlay) the FPS value */
    if (fps)
        display_fps();

    /* Swap OpenGL buffers at the end of scene rendering */
    glutSwapBuffers();

    /* Display OGLX Errors to standard output */
    DisplayOGLXErrors();

    /* To clear OGLX errors buffer, call sglClearErrors */
    /* sglClearErrors(); */
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           reshape_callback
    DESCRIPTION:    Redraws the scene when window is resized
---------------------------------------------------------------------+*/

void reshape_callback(int w, int h)
{
    oglx_reset();
    display_callback();
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           keyboard_callback
    DESCRIPTION:    Manages keyboard inputs
---------------------------------------------------------------------+*/

void keyboard_callback(unsigned char key, int x, int y)
{
    switch (key) {
        /* escape or q or Q => exit */
    case 27:
    case 'q':
    case 'Q':
        exit(0);
        break;
        /* p or P => enable/disable pause */
    case 'p':
    case 'P':
        pause = !pause;
        break;
        /* space => enable/disable FPS display */
    case ' ':
        fps = !fps;
        break;
        /* plus => increase rendering speed (reduce timer value) */
    case '+':
        if (timer_value >= 10)
            timer_value -= 10;
        break;
        /* minus => reduce rendering speed (increase timer value) */
    case '-':
        timer_value += 10;
        break;
        /* m or M => switch between RAW and SMOOTH OGLX line modes */
    case 'm':
    case 'M':
        switch_line_mode();
        break;
    default:
        break;
    }
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           main
    DESCRIPTION:    Main entry point for the display application
---------------------------------------------------------------------+*/

int main(int argc, char *argv[])
{
    /* Step 1: Initialize the GLUT library and negotiate a session with the
       window system */
    glutInit(&argc, argv);

    /* Step 2: Set the initial display mode, which is used when creating
       top-level windows, subwindows, and overlays to determine the
       OpenGL display mode for the to-be-created window or overlay */
    glutInitDisplayMode(GLUT_ALPHA | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

    /* Step 3: Set the initial window size in pixels */
    glutInitWindowSize(getW(), getH());


    /* Step 4: Create a top-level window. The name will be provided to the window
       system as the window's name. Implicitly, the current window is set
       to the newly created window. Each created window has a unique associated
       OpenGL context. State changes to a window's associated OpenGL context
       can be done immediately after the window is created. */
    glutCreateWindow(getSpecName());


    /* Step 5: Initialize all OGLX states and variables */
    oglx_init();

    /* Step 6: Initialize the SCADE Suite and/or Display application */
    init_scene();

    /* Step 7: Define the GLUT callbacks for display, window, keyboard,
       mouse, etc. management */
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);
    timer_callback(0);

    /* On Windows only, disable vertical synchronization (V-Sync) to avoid limiting FPS */
#ifdef _WIN32
    {
        typedef BOOL(APIENTRY * PFNWGLSWAPINTERVALFARPROC) (int);
        PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC) wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapIntervalEXT) {
            wglSwapIntervalEXT(0);
        }
    }
#endif

    /* Final Step: Enter the GLUT event processing loop. Once called, this
       routine will never return. It will call as necessary any callbacks
       that have been registered. */
    glutMainLoop();

    return 0;
}