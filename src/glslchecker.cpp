#include "glslchecker.h"
#include "GL/glext.h"
#include "logger.h"
#include "cart_product.h"
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


static const char*  d_1 [] = {  
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 0\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 2\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 0\n#define REFLECTIONS 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 1\n#define REFLECTIONS 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 2\n#define REFLECTIONS 1\n",
	"#define LIGHTING 0\n#undef LIGHTING_QUALITY\n",
};
static const char*  d_2 [] = { 
	"#define CLOUDS 1\n",
	"#undef CLOUDS\n"
};
static const char*  d_3 [] = { 
	"#define TERRAIN_DEREPEAT 1\n",
	"#undef TERRAIN_DEREPEAT\n"
};
static const char*  d_4 [] = { 
	"#define FALLOFF_QUALITY 1\n",
	"#define FALLOFF_QUALITY 0\n"
};
static const char*  d_5 [] = { 
	"#define FOG_TYPE 0\n",
	"#define FOG_TYPE 1\n"
};
static const char*  d_6 [] = { 
	"#define SHADOWS 0\n",
	"#define SHADOWS 1\n"
};
static const char*  d_7 [] = { 
	"#define NUM_BONES 1\n#define NUM_BONE_WEIGHTS 3\n",
	"#define NUM_BONES 0\n"
};
static const char*  d_8 [] = { 
	"#define FOG_OF_WAR 0\n",
	"#define FOG_OF_WAR 1\n"
};
static const char*  d_9 [] = { 
	"#define SHADOWMAP_TYPE 0\n",
	"#define SHADOWMAP_TYPE 1\n"
};
static const char*  d_10[] = { 
	"#define FOG_QUALITY 0\n",
	"#define FOG_QUALITY 1\n"
};
static const char*  d_11 [] = { 
	"#define GROUND_AMBIENT 1\n",
	"#undef GROUND_AMBIENT\n"
};
static const char*  d_12 [] = { 
	"#define TERRAIN_ALPHAMAP 1\n",
	"#undef TERRAIN_ALPHAMAP\n"
};
static const char*  d_13 [] = { 
	"#define RXGB_NORMALMAP 1\n",
	"#undef RXGB_NORMALMAP\n"
};
static const char*  d_14 [] = { 
	"#define NUM_POINT_LIGHTS 1\n",
	"#define NUM_POINT_LIGHTS 0\n"
};
static const char*  d_15 [] = { 
	"#define SHADOWMAP_FILTER_WIDTH 1\n",
	"#define SHADOWMAP_FILTER_WIDTH 0\n"
};
static const char*  d_17 [] = { 
	"#define TEXKILL 1\n",
	"#define TEXKILL 0\n"
};
static const char*  d_18 [] = { 
	"#define WATER_QUALITY 0\n",
	"#define WATER_QUALITY 1\n",
	"#define WATER_QUALITY 2\n",
	"#define WATER_QUALITY 3\n"
};

static CartesianProduct<const char*> cart_product;
static bool openGLcontextInitialized = false;

template <typename T,size_t N>
inline void push_vector_back(std::vector<std::vector<T> >& v,T (&va)[N])
{
    v.push_back(std::vector<T>(&va[0], &va[0] + N));
}

std::string const& GLSLChecker::cmdOption() const
{
    static std::string cmd = "--no-glsl";
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
	if(!openGLcontextInitialized)
	{
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
		logger.error(0) << "CreateWindowEx fail " << GetLastError() << logger.end;
		return;
	}

	g_hDC = GetDC(g_hWnd);

	if (!g_hDC)
	{
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
		logger.error(0) << "Setting pixel format fail " << GetLastError() << logger.end;
		return ;
	}

	hRCTemp = wglCreateContext(g_hDC);
	if (!hRCTemp || !wglMakeCurrent(g_hDC, hRCTemp))
	{
		logger.error(0) << "Сreating temp render context fail " << GetLastError() << logger.end;
		return ;
	}

	OPENGL_GET_PROC(PFNGLCREATESHADERPROC,      glCreateShader);
	OPENGL_GET_PROC(PFNGLDELETESHADERPROC,      glDeleteShader);
	OPENGL_GET_PROC(PFNGLSHADERSOURCEPROC,      glShaderSource);
	OPENGL_GET_PROC(PFNGLCOMPILESHADERPROC,     glCompileShader);
	OPENGL_GET_PROC(PFNGLGETSHADERIVPROC,       glGetShaderiv);
	OPENGL_GET_PROC(PFNGLGETSHADERINFOLOGPROC,  glGetShaderInfoLog);



#else
    /* I'm not gonna render anything so minimal context creation using root window */
    Display *dpy;
    Window root;
    GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi;
    GLXContext glc;

    /* open display */
    if ( ! (dpy = XOpenDisplay(NULL)) ) {
        logger.error(0) << "cannot connect to X server" << logger.end;
        return;
    }

    /* get root window */
    root = DefaultRootWindow(dpy);

    /* get visual matching attr */
    if( ! (vi = glXChooseVisual(dpy, 0, attr)) ) {
        logger.error(0) << "no appropriate visual found" << logger.end;
        return;
    }

    /* create a context using the root window */
    if ( ! (glc = glXCreateContext(dpy, vi, NULL, GL_TRUE)) ){
        logger.error(0) << "failed to create OpenGL context" << logger.end;
        return;
    }
    glXMakeCurrent(dpy, root, glc);

    logger.verbose(0) << "OpenGL vendor:" << glGetString(GL_VENDOR) << logger.end;
#endif
	openGLcontextInitialized = true;
	}
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
    std::vector<std::vector<const char*> > tmp;
    push_vector_back(tmp,v_1);
    push_vector_back(tmp,v_2);
    push_vector_back(tmp,v_3);
    push_vector_back(tmp,v_4);
    push_vector_back(tmp,v_5);
    push_vector_back(tmp,v_6);
    push_vector_back(tmp,v_7);
    push_vector_back(tmp,v_8);
    push_vector_back(tmp,v_9);
    push_vector_back(tmp,v_10);
    //push_vector_back(tmp,v_11);

    cart_product(_defines,tmp);

}
GLSLPSChecker::GLSLPSChecker() : GLSLChecker()
{
    type = GL_FRAGMENT_SHADER;
    std::vector<std::vector<const char*> > tmp;
    /*push_vector_back(tmp,v_1);
    push_vector_back(tmp,v_2);
    push_vector_back(tmp,v_3);
    push_vector_back(tmp,v_4);
    push_vector_back(tmp,v_5);
    push_vector_back(tmp,v_6);
    push_vector_back(tmp,v_7);
    push_vector_back(tmp,v_8);
    push_vector_back(tmp,v_9);
    push_vector_back(tmp,v_10);*/
    //push_vector_back(tmp,v_11);


    
	/*push_vector_back(tmp,p_1);
    push_vector_back(tmp,p_2);
    push_vector_back(tmp,p_3);
    push_vector_back(tmp,p_4);
    push_vector_back(tmp,p_5);
    push_vector_back(tmp,p_6);
    push_vector_back(tmp,p_7);*/
    //push_vector_back(tmp,p_8);

	push_vector_back(tmp,v_xxx);
	

    cart_product(_defines,tmp);
}


int GLSLChecker::Compile(const char** strings,int stringCount) const
{
    int res = 0;
#pragma omp critical(opengl)
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
			logger.error(0) << logger.end;
            logger.error(0) << "Error compiling shader, log: " << logger.end;
            logger.error(0) << infoLog << logger.end;
            logger.error(0) << "Defines combination: " << logger.end;
			for (int i = 0; i < stringCount - 1 ;++i)
				logger.error(0) << strings[i] << logger.end;
			logger.error(0) << logger.end;
            delete[] infoLog;
            res = 1;
        }
        glDeleteShader(shader);
    }
    return res;
}
