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
#include <coreavi_display/coreavi_display.h>
#include <e8860/e8860_display.h>
#include <global/coreavi_globals.h>
#include <GLSC2/glsc2.h>        /* GL header file */
#include <GLSC2/glsc2ext.h>     /* GL EXT header file */
#include <EGL/egl.h>            /* EGL header file */
#include <bit/bit.h>            /* BIT header file */
#include <math.h>
#include <stdio.h>
#include <os_helper/os_helper.h>
#include "oglx.h"
#include "specific.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <Windows.h>

/* Include utilitary function oglxReadErrors to have a readable output of errors */
#include "oglxReadErrors.h"

/***************************************************************
Macro Definitions
***************************************************************/
#define DISPLAY_WIDTH           1024
#define DISPLAY_HEIGHT          768
#define DISPLAY_COLOR_DEPTH     32
#define DISPLAY_SCREEN_REFRESH  60
#define DISPLAY_INTERLACED      FALSE
#define DISPLAY_SCREEN_HEAD1    E8860_OUTPUT_HEAD_1
#define DISPLAY_OUTPUT_TYPE1    E8860_OUTPUT_TYPE_DP
#define DISPLAY_SCREEN_HEAD2    E8860_OUTPUT_HEAD_2
#define DISPLAY_OUTPUT_TYPE2    E8860_OUTPUT_TYPE_DP_2
#define DISPLAY_CARD_ID         0
#define NUM_DISPLAYS            1
#define BUFFER_SIZE             (64 * 1024)

#define STATUS_FRAMES   300

#ifndef PI
#define PI 3.14159265358979323846
#endif

/***************************************************************
Local Variables
***************************************************************/

static EGLDisplay f_dpy[] = { EGL_NO_DISPLAY, EGL_NO_DISPLAY };
static EGLNativeWindowType f_win[] = { (EGLNativeWindowType) - 1, (EGLNativeWindowType) - 1 };
static EGLContext f_context[] = { EGL_NO_CONTEXT, EGL_NO_CONTEXT };
static EGLConfig f_config[] = { NULL, NULL };
static EGLSurface f_surface[] = { EGL_NO_SURFACE, EGL_NO_SURFACE };
static EGLNativeDisplayType f_disp_id[] = { -1, -1 };

static EGLint f_attribs[]
    = {
    EGL_BUFFER_SIZE, 32,
    EGL_DEPTH_SIZE, 24,
    EGL_STENCIL_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SAMPLE_BUFFERS, 1,
    EGL_SAMPLES, 4,
    EGL_NONE
};

static CoreAVIDisplayMode f_dispMode[] = {
    {DISPLAY_CARD_ID,
     DISPLAY_SCREEN_HEAD1,
     DISPLAY_OUTPUT_TYPE1,
     DISPLAY_WIDTH,
     DISPLAY_HEIGHT,
     DISPLAY_SCREEN_REFRESH,
     DISPLAY_COLOR_DEPTH,
     DISPLAY_INTERLACED, NULL, NULL},

    {DISPLAY_CARD_ID,
     DISPLAY_SCREEN_HEAD2,
     DISPLAY_OUTPUT_TYPE2,
     DISPLAY_WIDTH,
     DISPLAY_HEIGHT,
     DISPLAY_SCREEN_REFRESH,
     DISPLAY_COLOR_DEPTH,
     DISPLAY_INTERLACED, NULL, NULL}
};

static const char *f_dispString[] = {
    "HEAD_1",
    "HEAD_2",
    "HEAD_3",
    "HEAD_4",
    "HEAD_5",
    "HEAD_6"
};

#define TEXTURE_HEAP_SIZE   (16*1024*1024)      /* Texture heap size */

/***************************************************************
Function Prototypes
***************************************************************/
static EGLBoolean F_InitializeWindow(int h);
static void F_UninitializeDriver(void);

static GLint timer_value = 20;  /* integer: Time (in ms) between two frames */
static GLint fps = 0;           /* integer: Number of displayed Frame Per Second */
static GLint pause = 0;         /* boolean: Enable/disable refresh of drawings */
static GLint rawlinemode = 0;   /* boolean: Enable/disable RAW OpenGL mode for OGLX lines */
static GLenum glErrorStatus;    /* enum: OpenGL error index */

static void cleanup();
/* EGL variables */
EGLDisplay eglDisplay = 0;
EGLConfig eglConfig = 0;
EGLSurface eglSurface = 0;
EGLContext eglContext = 0;
EGLNativeWindowType eglWindow = 0;



static sgl_type_statemachine glob_s_context;

/*+ FUNCTION DESCRIPTION ----------------------------------------------
NAME:           oglx_init
DESCRIPTION:    Complete initialization sequence for OGLX
---------------------------------------------------------------------+*/
int oglx_init()
{
    /* Initialize OGLX */
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
    lParameters.ul_binary_format = GL_PROGRAM_BINARY_COREAVI;

    if (sglInit(&glob_s_context, &lParameters)) {

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

        sglViewport(0L, 0L, getW(), getH());
        sglOrtho(0, (float) (getW() * getRatioX() / 1.0F), 0, (float) (getH() * getRatioY() / 1.0F));

        /* Check if there is an OpenGL error after OGLX initialization */
        glErrorStatus = glGetError();
        if (glErrorStatus != GL_NO_ERROR) {
            printf("\nError %d raised during OGLX initialization\n\n", glErrorStatus);
        }
    }
    else {
        return 0;
    }
    return 1;
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
NAME:           oglx_terminate
DESCRIPTION:    Clears OGLX data
---------------------------------------------------------------------+*/
void oglx_terminate()
{
    sglTerminate();
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


void display_callback(void)
{

    /* Clear OpenGL colors, masks and buffers */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    /* Render the graphical scene */
    animate();
    draw();

}

extern sgl_type_statemachine glob_s_context;

int main(void)
{
    int frame = 0;
    int h = 0;
    static struct carddata_bit_test_temperature temperature = { 0, {0.0f, 0.0f, 0.0f}, 0.0f };
    bit_test tempTest = { BIT_MODULE_CARDDATA, BIT_CARDDATA_TEMPERATURE, &temperature, sizeof(temperature) };
    OSHelperTimerInfo currTime, prevTime;


    for (h = 0; h < NUM_DISPLAYS; h++) {
        if (EGL_FALSE == F_InitializeWindow(h)) {
            printf("%s: Initialization failure\n", f_dispString[h]);
            F_UninitializeDriver();
            return -1;
        }
    }
    OsHelperGetTime(&prevTime);
    oglx_init();
    context_init();
    anim_init();
    draw_init();

    for (;;) {
        for (h = 0; h < NUM_DISPLAYS; h++) {
            if (NUM_DISPLAYS > 1) {
                eglMakeCurrent(f_dpy[h], f_surface[h], f_surface[h], f_context[h]);
            }

            display_callback();

            eglSwapBuffers(f_dpy[h], f_surface[h]);

            if (0 == h) {
                frame++;
                if ((frame % STATUS_FRAMES) == 0) {
                    float numSeconds;
                    int fps;
                    OsHelperGetTime(&currTime);
                    numSeconds = ((float) (currTime.timeMicroseconds - prevTime.timeMicroseconds)) / 1000000.0f;
                    prevTime = currTime;
                    fps = (int) ((float) STATUS_FRAMES / numSeconds);
                    printf("%s: %d fps\n", f_dispString[h], fps);
                    CoreAVIRunBitTests(1, &tempTest);
                    printf("%s: TS0: %0.1f Celsius, Fan %0.0f%%\n", f_dispString[h], temperature.sensorTemp[0], temperature.fanDutyCycle);
                }
            }
        }
    }

    /* Clean up and exit */
    F_UninitializeDriver();
    return 0;
}

/*******************************************************
Method Name : F_InitializeWindow
Description : Setup the display, EGL, and initial GL
state.
Parameters  : h - context to initialize
Errors      : None
Return value: EGL_TRUE on success, EGL_FALSE otherwise
*******************************************************/
static EGLBoolean F_InitializeWindow(int h)
{
    EGLint numCfgs = 0;

    /* Open Native Display */
    if (COREAVI_ERROR_SUCCESS != CoreAVIInitializeDriver(NULL)) {
        return EGL_FALSE;
    }
    if (COREAVI_ERROR_SUCCESS != CoreAVIOpenDisplay(&f_dispMode[h], &f_disp_id[h])) {
        return EGL_FALSE;
    }

    /* Obtain an EGL Display */
    f_dpy[h] = eglGetDisplay(f_disp_id[h]);
    if (EGL_NO_DISPLAY == f_dpy[h]) {
        return EGL_FALSE;
    }

    /* Initialize EGL */
    if (EGL_TRUE != eglInitialize(f_dpy[h], 0, 0)) {
        return EGL_FALSE;
    }

    /* Obtain the first configuration with a buffer size of 32 bits */
    if (EGL_TRUE != eglChooseConfig(f_dpy[h], f_attribs, &f_config[h], 1, &numCfgs)) {
        return EGL_FALSE;
    }

    /* Create a Native Window */
    if (COREAVI_ERROR_SUCCESS != CoreAVICreateWindow(f_dpy[h], 0, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, &f_win[h])) {
        return EGL_FALSE;
    }

    /* Create an EGL Surface */
    f_surface[h] = eglCreateWindowSurface(f_dpy[h], f_config[h], f_win[h], NULL);
    if (EGL_NO_SURFACE == f_surface[h]) {
        return EGL_FALSE;
    }

    /* Create an EGL Context */
    {
        EGLint f_attribList[] = {
            COREAVI_HW_TEXTURE_HEAP_SIZE, TEXTURE_HEAP_SIZE,
            EGL_NONE
        };
        f_context[h] = eglCreateContext(f_dpy[h], f_config[h], EGL_NO_CONTEXT, f_attribList);

        if (EGL_NO_CONTEXT == f_context[h]) {
            return EGL_FALSE;
        }
    }

    /* Create an EGL Context */
    /*
       f_context[h] = eglCreateContext(f_dpy[h], f_config[h], EGL_NO_CONTEXT, NULL);
     */
    if (EGL_NO_CONTEXT == f_context[h]) {
        return EGL_FALSE;
    }

    if (EGL_TRUE != eglMakeCurrent(f_dpy[h], f_surface[h], f_surface[h], f_context[h])) {
        return EGL_FALSE;
    }

    /* Setting swap interval to 0 will rely on hsync */
    eglSwapInterval(f_dpy[h], 1);
    /*eglSwapInterval(f_dpy[h], 0); */
    return EGL_TRUE;
}


/*******************************************************
Method Name : F_UninitializeDriver
Description : Shuts down the driver.
Parameters  : None
Return value: None
*******************************************************/
static void F_UninitializeDriver(void)
{
    int i;
    for (i = 0; i < NUM_DISPLAYS; i++) {
        if (EGL_NO_DISPLAY != f_dpy[i]) {
            eglMakeCurrent(f_dpy[i], EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (EGL_NO_CONTEXT != f_context[i])
                eglDestroyContext(f_dpy[i], f_context[i]);
            if (EGL_NO_SURFACE != f_surface[i])
                eglDestroySurface(f_dpy[i], f_surface[i]);
            if ((EGLNativeWindowType) - 1 != f_win[i])
                CoreAVIDestroyWindow(f_win[i]);
            eglTerminate(f_dpy[i]);
        }
        if ((EGLNativeDisplayType) - 1 != f_disp_id[i]) {
            CoreAVICloseDisplay(f_disp_id[i]);
        }
    }
    CoreAVIUninitializeDriver();
}
