#include "glslchecker.h"
#include "logger.h"


#ifdef _WIN32
#define OPENGL_GET_PROC(p,n) \
	n = (p)wglGetProcAddress(#n); \
	if (NULL == n) \
	{ \
		logger.error(0) << "Loading extension '"<< #n << "' fail " << GetLastError() << logger.end; \
		/* return 1; */ \
	}
PFNGLCREATESHADERPROC      glCreateShader = 0;
PFNGLDELETESHADERPROC      glDeleteShader = 0;
PFNGLSHADERSOURCEPROC      glShaderSource = 0;
PFNGLCOMPILESHADERPROC     glCompileShader = 0;
PFNGLGETSHADERIVPROC       glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog = 0;
#endif

std::string const& GLSLChecker::cmdOption() const
{
    static std::string cmd = "--glsl";
    return cmd;
}
std::string const& GLSLChecker::name() const
{
    static std::string cmd = "GLSL Checker";
    return cmd;
}
GLSLChecker::GLSLChecker(GLuint type,const char* regex):
    type(type),_regex(regex)
{
    prologue = "#version 120\n";
    Logger& logger = Logger::get_instance();
    /* we need context for each thread, but only one per thread */
#ifdef _WIN32
    if (wglGetCurrentContext() == NULL)
    {
        const unsigned char INPUT_UP = 0, INPUT_DOWN = 1, INPUT_PRESSED = 2;

        static const char GLWINDOW_CLASS_NAME[] = "GLWindow_class";

        static HINSTANCE g_hInstance = NULL;
        static HWND      g_hWnd      = NULL;
        static HDC       g_hDC       = NULL;
        static HGLRC     g_hRC       = NULL;

        int width = 800;
        int height = 600;
        const char* title = "honCheck";
        WNDCLASSEX            wcx;
        PIXELFORMATDESCRIPTOR pfd;
        RECT                  rect;
        HGLRC                 hRCTemp;
        DWORD                 style, exStyle;
        int                   x, y, format;


        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        g_hInstance = (HINSTANCE)GetModuleHandle(NULL);

        memset(&wcx, 0, sizeof(wcx));
        wcx.cbSize        = sizeof(wcx);
        wcx.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.lpfnWndProc   = (WNDPROC)DefWindowProc;
        wcx.hInstance     = g_hInstance;
        wcx.lpszClassName = GLWINDOW_CLASS_NAME;
        wcx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);

        if (!RegisterClassEx(&wcx))
        {
#pragma omp critical (log)
            logger.error(0) << "RegisterClassEx fail " << GetLastError() << logger.end;
            return;
        }

        style   = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        exStyle = WS_EX_APPWINDOW;

        x = (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
        y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

        rect.left   = x;
        rect.right  = x + width;
        rect.top    = y;
        rect.bottom = y + height;

        AdjustWindowRectEx (&rect, style, FALSE, exStyle);

        g_hWnd = CreateWindowEx(exStyle, GLWINDOW_CLASS_NAME, title, style, rect.left, rect.top,
                rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, g_hInstance, NULL);

        if (!g_hWnd)
        {
#pragma omp critical (log)
            logger.error(0) << "CreateWindowEx fail " << GetLastError() << logger.end;
            return;
        }

        g_hDC = GetDC(g_hWnd);

        if (!g_hDC)
        {
#pragma omp critical (log)
            logger.error(0) << "GetDC fail " << GetLastError() << logger.end;
            return ;
        }

        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize      = sizeof(pfd);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;

        format = ChoosePixelFormat(g_hDC, &pfd);
        if (!format || !SetPixelFormat(g_hDC, format, &pfd))
        {
#pragma omp critical (log)
            logger.error(0) << "Setting pixel format fail " << GetLastError() << logger.end;
            return ;
        }

        hRCTemp = wglCreateContext(g_hDC);
        if (!hRCTemp || !wglMakeCurrent(g_hDC, hRCTemp))
        {
#pragma omp critical (log)
            logger.error(0) << "Сreating temp render context fail " << GetLastError() << logger.end;
            return ;
        }

#pragma omp critical (log)
        {
            OPENGL_GET_PROC(PFNGLCREATESHADERPROC,      glCreateShader);
            OPENGL_GET_PROC(PFNGLDELETESHADERPROC,      glDeleteShader);
            OPENGL_GET_PROC(PFNGLSHADERSOURCEPROC,      glShaderSource);
            OPENGL_GET_PROC(PFNGLCOMPILESHADERPROC,     glCompileShader);
            OPENGL_GET_PROC(PFNGLGETSHADERIVPROC,       glGetShaderiv);
            OPENGL_GET_PROC(PFNGLGETSHADERINFOLOGPROC,  glGetShaderInfoLog);
        }
#else /* #ifdef _WIN32 */
    if(glXGetCurrentContext() == NULL)
    {
        /* I'm not gonna render anything so minimal context creation using root window */
        Display *dpy;
        Window root;
        GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        XVisualInfo *vi;
        GLXContext glc;

        /* open display */
        if ( ! (dpy = XOpenDisplay(NULL)) ) {
#pragma omp critical (log)
            logger.error(0) << "cannot connect to X server" << logger.end;
            return;
        }

        /* get root window */
        root = DefaultRootWindow(dpy);

        /* get visual matching attr */
        if( ! (vi = glXChooseVisual(dpy, 0, attr)) ) {
#pragma omp critical (log)
            logger.error(0) << "no appropriate visual found" << logger.end;
            return;
        }

        /* create a context using the root window */
        if ( ! (glc = glXCreateContext(dpy, vi, NULL, GL_TRUE)) ){
#pragma omp critical (log)
            logger.error(0) << "failed to create OpenGL context" << logger.end;
            return;
        }
        glXMakeCurrent(dpy, root, glc);
#pragma omp critical (log)
        logger.verbose(0) << "OpenGL vendor:" << glGetString(GL_VENDOR) << logger.end;
#endif
    }
}
std::string const& GLSLChecker::reString() const
{
    return _regex;
}
int GLSLChecker::Compile(const char** strings,int stringCount, const char* path) const
{
    int res = 0;
    {
        GLuint shader = glCreateShader(type);
        GLint result = GL_TRUE;

        glShaderSource(shader, stringCount, strings, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if(result == GL_FALSE)
        {
            Logger& logger = Logger::get_instance();
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
            char* infoLog = new char[result];
            glGetShaderInfoLog(shader, result, NULL, infoLog);
#pragma omp critical (error)
            {
                logger.error(0) << logger.end;
                logger.error(0) << "Error compiling shader, log: " << logger.end;
                logger.error(0) << infoLog << logger.end;
                logger.error(0) << "Defines combination: " << logger.end;
                for (int i = 0; i < stringCount - 1 ;++i)
                    logger.error(0) << strings[i] << logger.end;
                logger.error(0) << logger.end;
            }
            delete[] infoLog;
            res = 1;
        }
        glDeleteShader(shader);
    }
    return res;
}
IChecker* GLSLChecker::clone() const
{
    return new GLSLChecker(type,_regex.c_str());
}
