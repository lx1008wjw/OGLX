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

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
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

static void cleanup();
/* EGL variables */
EGLDisplay eglDisplay = 0;
EGLConfig eglConfig = 0;
EGLSurface eglSurface = 0;
EGLContext eglContext = 0;
EGLNativeWindowType eglWindow = 0;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
/* Windows variables */
HWND hWnd = 0;
HDC hDC = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        /*
           Here we are handling 2 system messages: screen saving and monitor power.
           They are especially relevent on mobile devices.
         */

    case WM_SYSCOMMAND:
    {
        switch (wParam) {
        case SC_SCREENSAVE:    /* Screensaver trying to start ? */
        case SC_MONITORPOWER:  /* Monitor trying to enter powersave ? */
            return 0;           /* Prevent this from happening */
        }
        break;
    }

        /* Handles the close message when a user clicks the quit icon of the window */
    case WM_CLOSE:
        PostQuitMessage(0);
        cleanup();
        exit(0);

    default:
        break;
    }

    /* Calls the default window procedure for messages we did not handle */
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/*****************************************************************************
** Number of textures available in SGL
*****************************************************************************/
#define SGL_MAX_TEXTURES                      1024U

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
    lParameters.p_gradient_attrib = SGL_NULL;
    lParameters.ul_number_of_gradients = 0UL;


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

        glShadeModel(GL_FLAT);

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


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           display_callback
    DESCRIPTION:    Renders each frame
---------------------------------------------------------------------+*/

void display_callback(void)
{
    MSG msg;
    /* Clear OpenGL colors, masks and buffers */
    glClearColor(0.f, 0.f, 0.f, 0.0f);
    glClearStencil(0x0000);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    /* Render the graphical scene */
    draw_scene();

    /* Check if there is an OpenGL error after drawing of the scene */
    glErrorStatus = glGetError();
    if (glErrorStatus != GL_NO_ERROR) {
        printf("\nError %d raised during scene rendering\n\n", glErrorStatus);
    }

    /* Swap OpenGL buffers at the end of scene rendering */
    eglSwapBuffers(eglDisplay, eglSurface);

    /* Managing the window messages */
    PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    /* Display OGLX Errors to standard output */
    DisplayOGLXErrors();

    /* To clear OGLX errors buffer, call sglClearErrors */
    /* sglClearErrors(); */
}




/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           main
    DESCRIPTION:    Main entry point for the display application
---------------------------------------------------------------------+*/

int main(int argc, char **argv)
{
    /*
       Step 0 - Create a EGLNativeWindowType that we can use for OpenGL ES output
     */
    /* Register the windows class   */
    WNDCLASS sWC;
    unsigned int nWidth = getW() + 2;
    unsigned int nHeight = getH() + 2;
    RECT sRect;
    EGLint aEGLAttributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_STENCIL_SIZE, 8,
        EGL_SAMPLE_BUFFERS, 1,
        EGL_SAMPLES, 4,
        EGL_NONE
    };
    EGLint aEGLContextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 1,
        EGL_NONE
    };
    EGLint iConfigs;
    HANDLE hInstance;

    hInstance = GetModuleHandle(NULL);

    sWC.style = CS_HREDRAW | CS_VREDRAW;
    sWC.lpfnWndProc = WndProc;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = hInstance;
    sWC.hIcon = 0;
    sWC.hCursor = 0;
    sWC.lpszMenuName = 0;
    sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    sWC.lpszClassName = "PVRShellClass";

    RegisterClass(&sWC);
    SetRect(&sRect, 0, 0, nWidth, nHeight);
    AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, 0, 0);
    hWnd = CreateWindow("PVRShellClass", "PFD", WS_VISIBLE | WS_SYSMENU, 0, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);
    eglWindow = hWnd;

    /* Get the associated device context */
    hDC = GetDC(hWnd);
    if (!hDC) {
        cleanup();
        return 1;
    }

    eglDisplay = eglGetDisplay(hDC);

    eglInitialize(eglDisplay, NULL, NULL);

    eglChooseConfig(eglDisplay, aEGLAttributes, &eglConfig, 1, &iConfigs);

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, aEGLContextAttributes);

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    if (oglx_init()) {

        init_scene();

        /* Check if there is an OpenGL error after initialization of the scene */
        glErrorStatus = glGetError();
        if (glErrorStatus != GL_NO_ERROR) {
            printf("\nError %d raised during scene initialization\n\n", glErrorStatus);
        }

        while (1) {
            display_callback();
            /* Wait for 33 ms between each cycle */
            Sleep(33);
        }
    }
    return 0;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           cleanup
    DESCRIPTION:    Cleans EGL and Windows windowing system
---------------------------------------------------------------------+*/
static void cleanup()
{
    /* Release the contexts and surfaces so they can be deleted */
    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_SURFACE);

    eglTerminate(eglDisplay);

    /*
       Step 11 - Destroy the eglWindow.
       Again, this is platform specific and delegated to a separate function.
     */
    /* Release the device context */
    if (hDC)
        ReleaseDC(hWnd, hDC);

    /* Destroy the eglWindow */
    if (hWnd)
        DestroyWindow(hWnd);
}
