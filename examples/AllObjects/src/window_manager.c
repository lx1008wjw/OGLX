/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : window_manager.c
 DESCRIPTION       : Window management for WGL
 COPYRIGHT (C)     : 2015 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

#define _CRT_NON_CONFORMING_SWPRINTFS
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>

#include "oglx_display.h"
#include "target_configuration.h"

#define TRACE_LEVEL 0

#define SDYVIEWER_CLASSNAME _T("Graphical Panel Viewer")
#define SDYVIEWER_TITLE_FORMAT _T("%hs")

#define SDYVIEWER_WINDOW_STYLE (WS_POPUP | WS_VISIBLE  | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

#define then

HANDLE hmodule = NULL;
TCHAR ini_path[2048];
HWND hwnd = NULL;
HDC hdc = NULL;
HANDLE hMutex = NULL;
HANDLE hThread = 0;

int exit_flag = 0;
int shift_pressed = 0;
int control_pressed = 0;

int dest_x = 0, dest_y = 0, dest_w = 0, dest_h = 0;
double zoom = 1;

double ratio_w_h = 1.0;
RECT delta = { 0, 0, 0, 0 };

int g_max_w;
int g_max_h;

int g_latest_x;
int g_latest_y;
int g_latest_w;
int g_latest_h;

extern float global_x;
extern float global_y;

extern void oglx_reset();

static void WritePreference(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
static void ReadPreference(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPTSTR lpszBuffer, int nBufferSize);

#define QUEUESIZE 1024

extern double zoom;

HANDLE hLockBuffer = NULL;

typedef struct {
    int count;
    int first;
    int last;
    sdy_pointer_event_t values[QUEUESIZE + 1];
} mouse_queue;


typedef struct {
    int count;
    int first;
    int last;
    sdy_keyboard_event_t values[QUEUESIZE + 1];
} keyboard_queue;


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           lock_buffer
    DESCRIPTION:    Lock buffer to avoid inter-locking
---------------------------------------------------------------------+*/

static void lock_buffer()
{
    WaitForSingleObject(hLockBuffer, INFINITE);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           unlock_buffer
    DESCRIPTION:    Unlock buffer after having worked on buffer
---------------------------------------------------------------------+*/

static void unlock_buffer()
{
    ReleaseMutex(hLockBuffer);
}


static keyboard_queue buffer_keyboard_event = { 0, 0, QUEUESIZE - 1 };

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           record_keyboard_event
    DESCRIPTION:    Stores the keyboard events in the buffer_keyboard_event structure
---------------------------------------------------------------------+*/

void record_keyboard_event(int key, int pressed, int released, int modifiers)
{
    lock_buffer();
    if (buffer_keyboard_event.count < QUEUESIZE) {
        buffer_keyboard_event.last = (buffer_keyboard_event.last + 1) % QUEUESIZE;
        buffer_keyboard_event.values[buffer_keyboard_event.last].pressed = (SGLbool) pressed;
        buffer_keyboard_event.values[buffer_keyboard_event.last].released = (SGLbool) released;
        buffer_keyboard_event.values[buffer_keyboard_event.last].keycode = key;
        buffer_keyboard_event.values[buffer_keyboard_event.last].modifiers = modifiers;
        buffer_keyboard_event.count++;
    }
    unlock_buffer();
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display__get_keyboard_event
    DESCRIPTION:    Returns the next stored keyboard event
---------------------------------------------------------------------+*/

sdy_keyboard_event_t oglx_display__get_keyboard_event()
{
    sdy_keyboard_event_t keyboard_event = { 0, 0, 0, 0, 0 };
    lock_buffer();
    if (buffer_keyboard_event.count > 0) {
        keyboard_event = buffer_keyboard_event.values[buffer_keyboard_event.first];
        buffer_keyboard_event.first = (buffer_keyboard_event.first + 1) % QUEUESIZE;
        buffer_keyboard_event.count--;
    }
    unlock_buffer();
    return keyboard_event;
}

static mouse_queue buffer_mouse_event = { 0, 0, QUEUESIZE - 1 };

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           record_mouse_event
    DESCRIPTION:    Stores the mouse events in the buffer_mouse_event structure
---------------------------------------------------------------------+*/

void record_mouse_event(int button, int pressed, int released, int modifiers)
{
    lock_buffer();
    if (buffer_mouse_event.count < QUEUESIZE) {
        buffer_mouse_event.last = (buffer_mouse_event.last + 1) % QUEUESIZE;
        buffer_mouse_event.values[buffer_mouse_event.last].pressed = (SGLbool) pressed;
        buffer_mouse_event.values[buffer_mouse_event.last].released = (SGLbool) released;
        buffer_mouse_event.values[buffer_mouse_event.last].button = button;
        buffer_mouse_event.values[buffer_mouse_event.last].modifiers = modifiers;
        buffer_mouse_event.values[buffer_mouse_event.last].position[0] = global_x;
        buffer_mouse_event.values[buffer_mouse_event.last].position[1] = global_y;
        buffer_mouse_event.count++;
    }
    unlock_buffer();
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display__get_mouse_event
    DESCRIPTION:    Returns the next stored mouse event
---------------------------------------------------------------------+*/

sdy_pointer_event_t oglx_display__get_mouse_event()
{
    sdy_pointer_event_t mouse_event = { 0, 0.0F, 0.0F, 0, 0, 0, 0 };
    lock_buffer();
    if (buffer_mouse_event.count > 0) {
        mouse_event = buffer_mouse_event.values[buffer_mouse_event.first];
        buffer_mouse_event.first = (buffer_mouse_event.first + 1) % QUEUESIZE;
        buffer_mouse_event.count--;
    }
    else {
        mouse_event.position[0] = global_x;
        mouse_event.position[1] = global_y;
    }
    unlock_buffer();
    return mouse_event;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           record_mouse_pos
    DESCRIPTION:    Records the mouse position
---------------------------------------------------------------------+*/

void record_mouse_pos(double posx, double posy)
{
    global_x = (float) posx;
    global_y = (float) posy;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display__get_mouse_posx
    DESCRIPTION:    Returns the global x position of the mouse
---------------------------------------------------------------------+*/

double oglx_display__get_mouse_posx()
{
    return global_x;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display__get_mouse_posy
    DESCRIPTION:    Returns the global y position of the mouse
---------------------------------------------------------------------+*/

double oglx_display__get_mouse_posy()
{
    return global_y;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           WritePreference
    DESCRIPTION:    Writes the main window profile preferences
---------------------------------------------------------------------+*/

static void WritePreference(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
    if (lpszSection && lpszEntry && lpszValue) {
        WritePrivateProfileString(lpszSection, lpszEntry, lpszValue, ini_path);
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           ReadPreference
    DESCRIPTION:    Reads the window profile preferences
---------------------------------------------------------------------+*/

static void ReadPreference(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPTSTR lpszBuffer, int nBufferSize)
{
    if (lpszSection && lpszEntry) {
        GetPrivateProfileString(lpszSection, lpszEntry, lpszBuffer, lpszBuffer, nBufferSize, ini_path);
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           WriteWindowPref
    DESCRIPTION:    Writes the main window preferences
---------------------------------------------------------------------+*/

static void WriteWindowPref()
{
    /* Write settings */
    TCHAR prefs[1024];
    _stprintf(prefs, _T("%dx%dx%dx%d"), g_latest_x, g_latest_y, g_latest_w, g_latest_h);
    WritePreference(_T("Window"), _T("Position"), prefs);
}


static void oglx_display__resize(int w, int h);
static TCHAR *get_title();
static void transfer();

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           my_wnd_proc
    DESCRIPTION:    Events management callback function
---------------------------------------------------------------------+*/

static LRESULT WINAPI my_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL inside_client_area = FALSE;
    static int loc_main_key_down = 0;
    static int loc_keyboard_modifier = 0;

    /* Message management */
    switch (message) {

    case WM_LBUTTONDOWN:
        record_mouse_event(1, 1, 0, (wParam & 0xC) >> 2);
        return 0;
    case WM_LBUTTONUP:
        record_mouse_event(1, 0, 1, (wParam & 0xC) >> 2);
        return 0;
    case WM_RBUTTONDOWN:
        record_mouse_event(2, 1, 0, (wParam & 0xC) >> 2);
        return 0;
    case WM_RBUTTONUP:
        record_mouse_event(2, 0, 1, (wParam & 0xC) >> 2);
        return 0;
    case WM_MBUTTONDOWN:
        record_mouse_event(3, 1, 0, (wParam & 0xC) >> 2);
        return 0;
    case WM_MBUTTONUP:
        record_mouse_event(3, 0, 1, (wParam & 0xC) >> 2);
        return 0;

    case WM_MOUSEMOVE:
        /* Ensure cursor is displayed */
        if (!inside_client_area) {
            inside_client_area = TRUE;
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            ShowCursor(TRUE);
        }
        record_mouse_pos((LOWORD(lParam) / zoom), (target_screen_height - HIWORD(lParam) / zoom));
        return 0;

    case WM_NCMOUSEMOVE:
        /* Ensure cursor is displayed */
        if (inside_client_area) {
            inside_client_area = FALSE;
            ShowCursor(TRUE);
        }
        return DefWindowProc(hWnd, message, wParam, lParam);

    case WM_CHAR:
    {
        loc_main_key_down = (int) wParam;
        if (shift_pressed && control_pressed) {
            loc_keyboard_modifier = 3;
        }
        else if (shift_pressed) {
            loc_keyboard_modifier = 1;
        }
        else if (control_pressed) {
            loc_keyboard_modifier = 2;
        }
        else {
            loc_keyboard_modifier = 0;
        }

        record_keyboard_event(loc_main_key_down, 1, 0, loc_keyboard_modifier);
        return 0;
    }

    case WM_KEYDOWN:
    {

        switch ((int) wParam) {
        case VK_SHIFT:
            shift_pressed = 1;
            break;

        case VK_CONTROL:
            control_pressed = 1;
            break;

        case VK_F1:
            loc_main_key_down = 0x10001;
            break;

        case VK_F2:
            loc_main_key_down = 0x10002;
            break;

        case VK_F3:
            loc_main_key_down = 0x10003;
            break;

        case VK_F4:
            loc_main_key_down = 0x10004;
            break;

        case VK_F5:
            loc_main_key_down = 0x10005;
            break;

        case VK_F6:
            loc_main_key_down = 0x10006;
            break;

        case VK_F7:
            loc_main_key_down = 0x10007;
            break;

        case VK_F8:
            loc_main_key_down = 0x10008;
            break;

        case VK_F9:
            loc_main_key_down = 0x10009;
            break;

            /* VK_F10 is handled via WM_SYSKEYDOWN */
        case VK_F11:
            loc_main_key_down = 0x1000B;
            break;

        case VK_F12:
            loc_main_key_down = 0x1000C;
            break;

        case VK_PRIOR:
            loc_main_key_down = 0x1000D;
            break;              /* PAGE UP key */

        case VK_NEXT:
            loc_main_key_down = 0x1000E;
            break;              /* PAGE DOWN key */

        case VK_END:
            loc_main_key_down = 0x1000F;
            break;

        case VK_HOME:
            loc_main_key_down = 0x10010;
            break;

        case VK_LEFT:
            loc_main_key_down = 0x10011;
            break;

        case VK_UP:
            loc_main_key_down = 0x10012;
            break;

        case VK_RIGHT:
            loc_main_key_down = 0x10013;
            break;

        case VK_DOWN:
            loc_main_key_down = 0x10014;
            break;

        case VK_INSERT:
            loc_main_key_down = 0x10015;
            break;

        case VK_DELETE:
            loc_main_key_down = 0x10016;
            break;

        default:
            loc_main_key_down = 0;
            break;
        }

        if (loc_main_key_down) {
            record_keyboard_event(loc_main_key_down, 1, 0, loc_keyboard_modifier);
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    case WM_KEYUP:
    {
        switch ((int) wParam) {
        case VK_SHIFT:
            shift_pressed = 0;
            break;

        case VK_CONTROL:
            control_pressed = 0;
            break;
        default:
            break;
        }


        if (loc_main_key_down) {
            if (shift_pressed && control_pressed) {
                loc_keyboard_modifier = 3;
            }
            else if (shift_pressed) {
                loc_keyboard_modifier = 1;
            }
            else if (control_pressed) {
                loc_keyboard_modifier = 2;
            }
            else {
                loc_keyboard_modifier = 0;
            }

            record_keyboard_event(loc_main_key_down, 0, 1, loc_keyboard_modifier);
            loc_main_key_down = 0;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }


    case WM_DESTROY:
    case WM_CLOSE:
    {
        exit_flag = 1;
        if (hwnd) {
            WriteWindowPref(hwnd);
            DestroyWindow(hwnd);
        }
        hwnd = NULL;
        return 0;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpmmi = (LPMINMAXINFO) lParam;     // address of structure 
        RECT bbox;
        GetWindowRect(hWnd, &bbox);
        lpmmi->ptMaxSize.x = g_max_w;
        lpmmi->ptMaxSize.y = g_max_h;
        lpmmi->ptMaxPosition.x = bbox.left;
        lpmmi->ptMaxPosition.y = bbox.top;
        return 0;
    }

    case WM_SIZING:
    {
        LPRECT lprc = (LPRECT) lParam;  // screen coordinates of drag rectangle 

        switch (wParam) {
        case WMSZ_LEFT:
        case WMSZ_BOTTOMLEFT:
            lprc->bottom = (LONG) (delta.bottom + lprc->top - delta.top + (double) ((lprc->right - delta.right - lprc->left + delta.left) / ratio_w_h));
            wParam = WMSZ_BOTTOMLEFT;
            break;
        case WMSZ_TOPLEFT:
            lprc->top = (LONG) (delta.top + lprc->bottom - delta.bottom - (double) ((lprc->right - delta.right - lprc->left + delta.left) / ratio_w_h));
            wParam = WMSZ_TOPLEFT;
            break;
        case WMSZ_RIGHT:
        case WMSZ_BOTTOMRIGHT:
            lprc->bottom = (LONG) (delta.bottom + 1 + lprc->top - delta.top + (double) ((lprc->right - delta.right - lprc->left + delta.left) / ratio_w_h));
            wParam = WMSZ_BOTTOMRIGHT;
            break;
        case WMSZ_TOP:
        case WMSZ_TOPRIGHT:
            lprc->right = (LONG) (delta.right + 1 + lprc->left - delta.left + (double) ((lprc->bottom - delta.bottom - lprc->top + delta.top) * ratio_w_h));
            wParam = WMSZ_TOPRIGHT;
            break;
        case WMSZ_BOTTOM:
            lprc->right = (LONG) (delta.right + 1 + lprc->left - delta.left + (double) ((lprc->bottom - delta.bottom - lprc->top + delta.top) * ratio_w_h));
            wParam = WMSZ_BOTTOMRIGHT;
            break;
        default:
            break;
        }

        switch (wParam) {
        case WMSZ_LEFT:
        case WMSZ_BOTTOMLEFT:
        case WMSZ_TOPLEFT:
            if (lprc->right - lprc->left > g_max_w)
                lprc->left = lprc->right - g_max_w;
            break;
        case WMSZ_RIGHT:
        case WMSZ_BOTTOMRIGHT:
        case WMSZ_TOPRIGHT:
            if (lprc->right - lprc->left > g_max_w)
                lprc->right = lprc->left + g_max_w;
            break;
        default:
            break;
        }

        switch (wParam) {
        case WMSZ_BOTTOM:
        case WMSZ_BOTTOMLEFT:
        case WMSZ_BOTTOMRIGHT:
            if (lprc->bottom - lprc->top > g_max_h)
                lprc->bottom = lprc->top + g_max_h;
            break;
        case WMSZ_TOP:
        case WMSZ_TOPLEFT:
        case WMSZ_TOPRIGHT:
            if (lprc->bottom - lprc->top > g_max_h)
                lprc->top = lprc->bottom - g_max_h;
            break;
        default:
            break;
        }
        return 0;
    }
    case WM_SIZE:
    {
        if ((wParam == SIZE_RESTORED) || (wParam == SIZE_MAXIMIZED)) {
            g_latest_w = LOWORD(lParam);        // width of client area 
            g_latest_h = HIWORD(lParam);        // height of client area 
            oglx_display__resize(g_latest_w, g_latest_h);
            WriteWindowPref();
            return 0;
        }
    }
    case WM_MOVE:
    {
        if (!IsIconic(hWnd) && (LOWORD(lParam) > 0) && (HIWORD(lParam) > 0)) {
            g_latest_x = LOWORD(lParam);        // xpos of client area 
            g_latest_y = HIWORD(lParam);        // ypos of client area 
            WriteWindowPref();
            return 0;
        }
    }
    case WM_PAINT:
    {
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           open_window
    DESCRIPTION:    Initializes and creates the windows window
---------------------------------------------------------------------+*/

int open_window()
{
    WNDCLASS wndclass;
    ATOM aRegister;
    DWORD style = SDYVIEWER_WINDOW_STYLE;
    RECT bbox;

    /* Definition de la classe */
    wndclass.style = 0;
    wndclass.lpfnWndProc = (WNDPROC) my_wnd_proc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hmodule;
    wndclass.hIcon = NULL;      //LoadIcon(hmodule, MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor = LoadCursor(hmodule, IDC_ARROW);
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = SDYVIEWER_CLASSNAME;

    /* Enregistrement de la classe */
    UnregisterClass(SDYVIEWER_CLASSNAME, hmodule);
    aRegister = RegisterClass(&wndclass);
    if (0 == aRegister) {
        printf("Failed to register window class\n");
    }

    bbox.left = g_latest_x;
    bbox.top = g_latest_y;
    bbox.right = g_latest_x + g_latest_w;
    bbox.bottom = g_latest_y + g_latest_h;

    AdjustWindowRect(&bbox, style, 0);

    delta.left = bbox.left - g_latest_x;
    delta.top = bbox.top - g_latest_y;
    delta.right = bbox.right - (g_latest_x + g_latest_w);
    delta.bottom = bbox.bottom - (g_latest_y + g_latest_h);

    g_max_w += delta.right - delta.left;
    g_max_h += delta.bottom - delta.top;

    hwnd = CreateWindow(SDYVIEWER_CLASSNAME,
                        get_title(), style, bbox.left, bbox.top, bbox.right - bbox.left, bbox.bottom - bbox.top, NULL, (HMENU) NULL, hmodule, NULL);

    if (hwnd == NULL) {
        printf("Failed to create window...\n");
        return -2;
    }

    /* Preparation du contexte graphique */
    hdc = GetDC(hwnd);
    if (hdc == NULL) {
        DestroyWindow(hwnd);
        hwnd = NULL;
        printf("no device context.\n");
        return -3;
    }

    /* Affichage de la fenetre */
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    /* Store the blit mode */
    SetStretchBltMode(hdc, COLORONCOLOR);

    exit_flag = 0;

    return 0;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display__initialise
    DESCRIPTION:    Initializes the size settings of the window (dimensions, ratio)
---------------------------------------------------------------------+*/

void oglx_display__initialise(int par_i_rotate)
{
    int window_w, window_h;
    TCHAR prefs[1024];

    if (hwnd != NULL) {
        return;
    }

    if (par_i_rotate) {
        window_w = target_screen_width;
        window_h = target_screen_height;
    }
    else {
        window_w = target_screen_height;
        window_h = target_screen_width;
    }

    dest_w = target_screen_width;
    dest_h = target_screen_height;
    dest_x = 0;
    dest_y = 0;
    zoom = 1;
    ratio_w_h = (double) dest_w / (double) dest_h;

    g_max_w = window_w;
    g_max_h = window_h;

    /* Read settings */
    _stprintf(prefs, _T("%dx%dx%dx%d"), 50, 50, window_w, window_h);
    ReadPreference(_T("Window"), _T("Position"), prefs, sizeof(prefs));

    if ((_stscanf(prefs, _T("%dx%dx%dx%d"), &g_latest_x, &g_latest_y, &g_latest_w, &g_latest_h) == 4)
        && (g_latest_x > 0) && (g_latest_y > 0) && (g_latest_w > 0) && (g_latest_h > 0)
        && (g_latest_x <= 2048) && (g_latest_y <= 2048) && (g_latest_w <= 2048) && (g_latest_h <= 2048)) {
        oglx_display__resize(g_latest_w, g_latest_h);
    }
    else {
        g_latest_x = 50;
        g_latest_y = 50;
        g_latest_w = window_w;
        g_latest_h = window_h;
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           oglx_display__resize
    DESCRIPTION:    Computes the new dimensions and ratio of the window after a resize request
---------------------------------------------------------------------+*/

static void oglx_display__resize(int w, int h)
{
    double factor_x = 1, factor_y = 1;
    double factor = 1;

    factor_x = (double) target_screen_width / (double) w;
    factor_y = (double) target_screen_height / (double) h;
    factor = (factor_x > factor_y ? factor_x : factor_y);
    if (factor <= 1) {
        factor = 1;
    }

    if (hdc) {
        SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        FloodFill(hdc, dest_x, dest_y, GRAY_BRUSH);
    }
    zoom = 1 / factor;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
    NAME:           get_title
    DESCRIPTION:    Returns the title of the window
---------------------------------------------------------------------+*/

static TCHAR *get_title()
{
    static TCHAR title[1024];
    _stprintf(title, SDYVIEWER_TITLE_FORMAT, "AllObjects");
    return title;
}
