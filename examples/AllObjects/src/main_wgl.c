/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : main_wgl.c
 DECSRIPTION       : Main file with creation of window and management of mouse 
					and keyboard for WGL
 COPYRIGHT (C)     : 2015 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
*============================================================================================*/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

#include "oglx.h"

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sdy_events.h"
#include "specific.h"
#include "aol_color_table.h"

#include "oglx_display.h"
#include "target_configuration.h"

#include "aol_texture_table.h"

/* Include utilitary function oglxReadErrors to have a readable output of errors */
#include "oglxReadErrors.h"

extern void BHVR_Pointer(const sdy_pointer_event_t * evt);

extern HWND hwnd;

int global_button = 0;
int global_mousestate = 0;
int global_keycode = 0;
int global_keystate = 0;
int global_modifiers = 0;
float global_x = 0.0f;
float global_y = 0.0f;

int global_error = 0;

#define CYCLE_TIME 33

/******************************************************************************
 **                           File variables
 *****************************************************************************/

static GLint timer_value = 20;  /* integer: Time (in ms) between two frames */
static GLint fps = 0;           /* integer: Number of displayed Frame Per Second */
static GLint pause = 0;         /* boolean: Enable/disable refresh of drawings */
static GLint rawlinemode = 0;   /* boolean: Enable/disable RAW OpenGL mode for OGLX lines */
static GLenum glErrorStatus;    /* enum: OpenGL error index */

static sgl_type_statemachine glob_s_context;

extern HDC hdc;
static HGLRC hglrc;
static HANDLE hSignal = 0;

extern int g_latest_w;
extern int g_latest_h;

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_init
    DESCRIPTION:    Complete initialization sequence for OGLX
---------------------------------------------------------------------+*/

void oglx_init()
{
    /* Initialize OGLX */
    static SGLbyte glob_tub_texture_buffer[4 * SGL_TEXTURE_MAX_WIDTH * SGL_TEXTURE_MAX_HEIGHT];
    sgl_texture_attrib *glob_texture_attrib = malloc(sizeof(sgl_texture_attrib) * getTextureTableSize());
    sgl_gradient_attrib *glob_gradient_attrib = malloc(sizeof(sgl_gradient_attrib) * getGradientTableSize());
    static sgl_parameters lParameters;

    lParameters.ul_screen_width = getW();
    lParameters.ul_screen_height = getH();
    lParameters.pb_texture_buffer = glob_tub_texture_buffer;
    lParameters.ul_texture_max_width = SGL_TEXTURE_MAX_WIDTH;
    lParameters.ul_texture_max_height = SGL_TEXTURE_MAX_HEIGHT;
    lParameters.p_texture_attrib = glob_texture_attrib;
    lParameters.ul_number_of_textures = getTextureTableSize();
    lParameters.p_gradient_attrib = glob_gradient_attrib;
    lParameters.ul_number_of_gradients = getGradientTableSize();

    sglInit(&glob_s_context, &lParameters);

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

    g_latest_w = getW();
    g_latest_h = getH();
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_reset
    DESCRIPTION:    Resets the OGLX state
---------------------------------------------------------------------+*/

void oglx_reset()
{
    oglx_display_enter_io_mutex();

    /* Change viewport in case of a resize */
    sglReset();
    sglViewport(0L, 0L, g_latest_w, g_latest_h);
    sglOrtho(0, (float) (getW() * getRatioX() / 1.0F), 0, (float) (getH() * getRatioY() / 1.0F));

    /* Check if there is an OpenGL error after OGLX reset */
    glErrorStatus = glGetError();
    if (glErrorStatus != GL_NO_ERROR) {
        printf("\nError %d raised during OGLX reset\n\n", glErrorStatus);
    }

    oglx_display_end_io_mutex();
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
    sdy_pointer_event_t lEvent = oglx_display__get_mouse_event();
    oglx_reset();

    BHVR_Pointer(&lEvent);

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

    glFlush();

    /* Print OGLX errors to standard output */
    DisplayOGLXErrors();

    /* To clear OGLX errors buffer, call sglClearErrors */
    /* sglClearErrors(); */
}

typedef BOOL(APIENTRY * PFNWGLSWAPINTERVALFARPROC) (int);
static void setVSync(int interval)
{
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC) wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT) {
        wglSwapIntervalEXT(interval);
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           wgl_init
    DESCRIPTION:    Initialisation of wgl context and bitmap
---------------------------------------------------------------------+*/

void wgl_init(void)
{
    /* WGL Buffer init */
    BITMAPINFO bmpInfo;
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormat = 0;

    /* Create OglxWContext */
    hglrc = NULL;

    /* Create Pixel Format */
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 16;
    pfd.cStencilBits = 8;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    /* Create Bitmap format */
    ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biHeight = target_screen_height;
    bmpInfo.bmiHeader.biWidth = target_screen_width;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    /* Set Pixel Format on Device Context */
    iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, iPixelFormat, &pfd);

    /* Create Rendering Context on Device Context */
    hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    /* Disable Vertical synchronization to improve FPS */
    setVSync(0);

    /* Initialize all OGLX states and variables */
    oglx_init();

    /* Initialize the SCADE Suite and/or Display application */
    init_scene();
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           application_draw
    DESCRIPTION:    Cyclic draw function called by drawing thread
---------------------------------------------------------------------+*/

int application_draw(void)
{
    /* Call of primitives drawing */
    display_callback();

    /* render framebuffer into windows bitmap and poll message queue */
    SwapBuffers(hdc);

    return 0;
}

static HANDLE hIOMutex = NULL;
static HANDLE hDrawingThread = NULL;
static HANDLE hDrawingSignal = NULL;

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           launch_drawing
    DESCRIPTION:    Sequence of the drawing thread
---------------------------------------------------------------------+*/

unsigned __stdcall launch_drawing(void *pArguments)
{
    long start;
    long finish;
    long lDuration;
    long loc_time_sleep;
    int loc_loop = 1;

    SetEvent(hDrawingSignal);

    oglx_display_enter_io_mutex();
    wgl_init();
    oglx_display_end_io_mutex();

    /* Then - cyclic drawing */
    while (loc_loop == 1) {
        start = clock();
        oglx_display_enter_io_mutex();
        application_draw();
        oglx_display_end_io_mutex();

        finish = clock();
        lDuration = (long) ((double) (finish - start) / CLOCKS_PER_SEC * 1000.0);

        loc_time_sleep = CYCLE_TIME - lDuration;

        /* Sleep loc_time_sleep */
        if (loc_time_sleep > 0) {
            SleepEx(loc_time_sleep, FALSE);
        }

        if (global_error == 1) {
            loc_loop = 0;
        }
    }

    return 0;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           start_drawing_thread
    DESCRIPTION:    Creation and launch of the drawing thread
---------------------------------------------------------------------+*/

void start_drawing_thread()
{
    UINT nThreadId;

    hIOMutex = CreateMutex(NULL, FALSE, NULL);

    if (hIOMutex == NULL) {
        printf("cannot create mutex.\n");
        return;
    }

    hDrawingSignal = CreateEvent(NULL, FALSE, FALSE, NULL);

    hDrawingThread = (HANDLE) _beginthreadex((void *) 0, 0, launch_drawing, (void *) 0, 0, &nThreadId);

    if (hDrawingThread == NULL) {
        printf("cannot create thread.\n");
        return;
    }

    WaitForSingleObject(hDrawingSignal, INFINITE);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display_enter_io_mutex
    DESCRIPTION:    Take the mutex on the drawing thread
---------------------------------------------------------------------+*/

void oglx_display_enter_io_mutex()
{
    if (hIOMutex != NULL) {
        WaitForSingleObject(hIOMutex, INFINITE);
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display_end_io_mutex
    DESCRIPTION:    Release the mutex on the drawing thread
---------------------------------------------------------------------+*/

void oglx_display_end_io_mutex()
{
    if (hIOMutex != NULL) {
        ReleaseMutex(hIOMutex);
    }
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           main
    DESCRIPTION:    Main entry point for the display application
---------------------------------------------------------------------+*/

int main(int argc, char *argv[])
{
    MSG msg;

    /* Initialisation of the window parameters */
    oglx_display__initialise(1);

    /* Creation of the window */
    open_window();

    /* Creation and start of the drawing thread */
    start_drawing_thread();

    /* Event main loop */
    while (hwnd != NULL && WaitMessage()) {
        if (GetMessage(&msg, hwnd, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            return 0;
        }

    }

    return 0;
}
