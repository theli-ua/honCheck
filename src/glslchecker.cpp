#include "glslchecker.h"
#include "GL/glext.h"
#include "logger.h"
#ifndef _WIN32
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#else
#include "GL/wglext.h"
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

/*  k_ ones are a strings to search in shader to include current set of defines
 *  for combinations product
 *  after all matching sets are found cartesian product is build on them
 *  and the shader is compiled with each combination
 */
static const char* k_0 [] = { " " };
static const char* d_0 [] = { "#version 120\n" };

static const char*  k_1 [] = {  
	"LIGHTING",
	"LIGHTING_QUALITY",
	"REFLECTIONS",
};
static const char*  d_1 [] = {  
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 0\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 2\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 0\n#define REFLECTIONS 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 1\n#define REFLECTIONS 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 2\n#define REFLECTIONS 1\n",
	//"#define LIGHTING 0\n",
};
static const char*  k_2 [] = { "CLOUDS" };
static const char*  d_2 [] = { 
	"#define CLOUDS\n",
	""
};
static const char*  k_3 [] = { "TERRAIN_DEREPEAT" };
static const char*  d_3 [] = { 
	"#define TERRAIN_DEREPEAT 1\n",
	"#define TERRAIN_DEREPEAT 0\n"
};
static const char* k_4 [] = {
    "FALLOFF_QUALITY",
    "FOG_TYPE",
    "FOG_OF_WAR",
    "FOG_QUALITY",
};
static const char* d_4 [] = {
    //"#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    //"#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
};    
//static const char*  k_4 [] = { "FALLOFF_QUALITY" };
//static const char*  d_4 [] = { 
	//"#define FALLOFF_QUALITY 1\n",
	//"#define FALLOFF_QUALITY 0\n"
//};
//static const char*  k_5 [] = { "FOG_TYPE" };
//static const char*  d_5 [] = { 
	//"#define FOG_TYPE 0\n",
	//"#define FOG_TYPE 1\n"
//};
//static const char*  k_10 [] = { "FOG_QUALITY" };
//static const char*  d_10[] = { 
	//"#define FOG_QUALITY 0\n",
	//"#define FOG_QUALITY 1\n"
//};
//static const char*  k_8 [] = { "FOG_OF_WAR" };
//static const char*  d_8 [] = { 
	//"#define FOG_OF_WAR 0\n",
	//"#define FOG_OF_WAR 1\n"
//};
static const char*  k_6 [] = { "SHADOWS" };
static const char*  d_6 [] = { 
	"#define SHADOWS 0\n",
	"#define SHADOWS 1\n"
};
static const char*  k_7 [] = { "NUM_BONES" };
static const char*  d_7 [] = { 
	"#define NUM_BONES 1\n#define NUM_BONE_WEIGHTS 3\n",
	"#define NUM_BONES 0\n"
};
static const char*  k_9 [] = { "SHADOWMAP_TYPE" };
static const char*  d_9 [] = { 
	"#define SHADOWMAP_TYPE 0\n",
	"#define SHADOWMAP_TYPE 1\n"
};
static const char*  k_11 [] = { "GROUND_AMBIENT" };
static const char*  d_11 [] = { 
	"#define GROUND_AMBIENT 1\n",
	"#undef GROUND_AMBIENT\n"
};
static const char*  k_12 [] = { "TERRAIN_ALPHAMAP" };
static const char*  d_12 [] = { 
	"#define TERRAIN_ALPHAMAP 1\n",
	"#undef TERRAIN_ALPHAMAP\n"
};
static const char*  k_13 [] = { "RXGB_NORMALMAP" };
static const char*  d_13 [] = { 
	"#define RXGB_NORMALMAP 1\n",
	"#undef RXGB_NORMALMAP\n"
};
static const char*  k_14 [] = { "NUM_POINT_LIGHTS" };
static const char*  d_14 [] = { 
	"#define NUM_POINT_LIGHTS 1\n",
	"#define NUM_POINT_LIGHTS 0\n"
};
static const char*  k_15 [] = { "SHADOWMAP_FILTER_WIDTH" };
static const char*  d_15 [] = { 
	"#define SHADOWMAP_FILTER_WIDTH 1\n",
	"#define SHADOWMAP_FILTER_WIDTH 0\n"
};
static const char*  k_16 [] = { "TEXKILL" };
static const char*  d_16 [] = { 
	"#define TEXKILL 1\n",
	"#define TEXKILL 0\n"
};
static const char*  k_17 [] = { "WATER_QUALITY" };
static const char*  d_17 [] = { 
	"#define WATER_QUALITY 0\n",
	"#define WATER_QUALITY 1\n",
	"#define WATER_QUALITY 2\n",
	"#define WATER_QUALITY 3\n"
};

template <typename T,size_t K,size_t N>
inline void add_define_pair(std::vector<std::pair<std::vector<T>,std::vector<T> > >& v,T (&k)[K],T (&d)[N])
{
    v.push_back(
		std::pair<std::vector<T>,std::vector<T> >(
			std::vector<T>(&k[0], &k[0] + K),
			std::vector<T>(&d[0], &d[0] + N)
			)
		);
}
#define ADD_SHADER_MACRO_DEFINE(N) add_define_pair(_defines,k_##N,d_##N)

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

GLSLChecker::GLSLChecker(): ShaderChecker()
{
    Logger& logger = Logger::get_instance();
#ifdef _WIN32
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
#pragma critical (log)
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
#pragma critical (log)
        logger.error(0) << "CreateWindowEx fail " << GetLastError() << logger.end;
        return;
    }

    g_hDC = GetDC(g_hWnd);

    if (!g_hDC)
    {
#pragma critical (log)
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
#pragma critical (log)
        logger.error(0) << "Setting pixel format fail " << GetLastError() << logger.end;
        return ;
    }

    hRCTemp = wglCreateContext(g_hDC);
    if (!hRCTemp || !wglMakeCurrent(g_hDC, hRCTemp))
    {
#pragma critical (log)
        logger.error(0) << "Сreating temp render context fail " << GetLastError() << logger.end;
        return ;
    }

#pragma critical (log)
    {
        OPENGL_GET_PROC(PFNGLCREATESHADERPROC,      glCreateShader);
        OPENGL_GET_PROC(PFNGLDELETESHADERPROC,      glDeleteShader);
        OPENGL_GET_PROC(PFNGLSHADERSOURCEPROC,      glShaderSource);
        OPENGL_GET_PROC(PFNGLCOMPILESHADERPROC,     glCompileShader);
        OPENGL_GET_PROC(PFNGLGETSHADERIVPROC,       glGetShaderiv);
        OPENGL_GET_PROC(PFNGLGETSHADERINFOLOGPROC,  glGetShaderInfoLog);
    }


#else
    /* I'm not gonna render anything so minimal context creation using root window */
    Display *dpy;
    Window root;
    GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi;
    GLXContext glc;

    /* open display */
    if ( ! (dpy = XOpenDisplay(NULL)) ) {
#pragma critical (log)
        logger.error(0) << "cannot connect to X server" << logger.end;
        return;
    }

    /* get root window */
    root = DefaultRootWindow(dpy);

    /* get visual matching attr */
    if( ! (vi = glXChooseVisual(dpy, 0, attr)) ) {
#pragma critical (log)
        logger.error(0) << "no appropriate visual found" << logger.end;
        return;
    }

    /* create a context using the root window */
    if ( ! (glc = glXCreateContext(dpy, vi, NULL, GL_TRUE)) ){
#pragma critical (log)
        logger.error(0) << "failed to create OpenGL context" << logger.end;
        return;
    }
    glXMakeCurrent(dpy, root, glc);

#endif
    ADD_SHADER_MACRO_DEFINE(0);
#if 1
    ADD_SHADER_MACRO_DEFINE(1);
    ADD_SHADER_MACRO_DEFINE(2);
    ADD_SHADER_MACRO_DEFINE(3);
    ADD_SHADER_MACRO_DEFINE(4);
    //ADD_SHADER_MACRO_DEFINE(5);
    ADD_SHADER_MACRO_DEFINE(6);
    ADD_SHADER_MACRO_DEFINE(7);
    //ADD_SHADER_MACRO_DEFINE(8);
    ADD_SHADER_MACRO_DEFINE(9);
    //ADD_SHADER_MACRO_DEFINE(10);
    ADD_SHADER_MACRO_DEFINE(11);
    ADD_SHADER_MACRO_DEFINE(12);
    ADD_SHADER_MACRO_DEFINE(13);
    ADD_SHADER_MACRO_DEFINE(14);
    ADD_SHADER_MACRO_DEFINE(15);
    ADD_SHADER_MACRO_DEFINE(16);
    ADD_SHADER_MACRO_DEFINE(17);
#endif
}


std::string const& GLSLVSChecker::reString() const
{
    static std::string cmd = ".*/vs_glsl/.*\\.vsh";
    return cmd;
}
std::string const& GLSLPSChecker::reString() const
{
    static std::string cmd = ".*/ps_glsl.*\\.psh";
    return cmd;
}

GLSLVSChecker::GLSLVSChecker() : GLSLChecker()
{
    type = GL_VERTEX_SHADER;
}
GLSLPSChecker::GLSLPSChecker() : GLSLChecker()
{
	type = GL_FRAGMENT_SHADER;
}


int GLSLChecker::Compile(const char** strings,int stringCount) const
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
#pragma critical (error)
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

IChecker* GLSLPSChecker::clone()
{
    return new GLSLPSChecker();
}
IChecker* GLSLVSChecker::clone()
{
    return new GLSLVSChecker();
}
