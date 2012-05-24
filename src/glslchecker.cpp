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
//TODO
#endif

static const char*  v_1 [] = { "#define LIGHTING_QUALITY 1\n","#define LIGHTING_QUALITY 2\n","#define LIGHTING_QUALITY 0\n"};
static const char*  v_2 [] = { "#define CLOUDS 1\n","#undef CLOUDS\n"};
static const char*  v_3 [] = { "#define TERRAIN_DEREPEAT 1\n","#undef TERRAIN_DEREPEAT\n"};
static const char*  v_4 [] = { "#define FALLOFF_QUALITY 1\n","#define FALLOFF_QUALITY 0\n"};
static const char*  v_5 [] = { "#define FOG_TYPE 0\n","#define FOG_TYPE 1\n"};
static const char*  v_6 [] = { "#define SHADOWS 0\n","#define SHADOWS 1\n"};
static const char*  v_7 [] = { "#define NUM_BONES 1\n#define NUM_BONE_WEIGHTS 3\n","#define NUM_BONES 0\n"};
static const char*  v_8 [] = { "#define FOG_OF_WAR 0\n","#define FOG_OF_WAR 1\n"};
static const char*  v_9 [] = { "#define SHADOWMAP_TYPE 0\n","#define SHADOWMAP_TYPE 1\n"};
static const char*  v_10[] = { "#define FOG_QUALITY 0\n","#define FOG_QUALITY 1\n"};
static const char*  v_11[] = { "#define LIGHTING 0\n","#define LIGHTING 1\n"};

static const char*  p_1 [] = { "#define GROUND_AMBIENT 1\n","#undef GROUND_AMBIENT\n"};
static const char*  p_2 [] = { "#define REFLECTIONS 1\n","#undef REFLECTIONS\n"};
static const char*  p_3 [] = { "#define TERRAIN_ALPHAMAP 1\n","#undef TERRAIN_ALPHAMAP\n"};
static const char*  p_4 [] = { "#define RXGB_NORMALMAP 1\n","#undef RXGB_NORMALMAP\n"};
static const char*  p_5 [] = { "#define NUM_POINT_LIGHTS 1\n","#define NUM_POINT_LIGHTS 0\n"};
static const char*  p_6 [] = { "#define SHADOWMAP_FILTER_WIDTH 1\n","#define SHADOWMAP_FILTER_WIDTH 0\n"};
static const char*  p_7 [] = { "#define TEXKILL 1\n","#define TEXKILL 0\n"};
static const char*  p_8 [] = { "#define WATER_QUALITY 1\n","#define WATER_QUALITY 2\n","#define WATER_QUALITY 0\n","#define WATER_QUALITY 3\n"};

static CartesianProduct<const char*> cart_product;

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
#ifdef _WIN32

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
    push_vector_back(tmp,v_11);

    cart_product(_defines,tmp);

}
GLSLPSChecker::GLSLPSChecker() : GLSLChecker()
{
    type = GL_FRAGMENT_SHADER;
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
    push_vector_back(tmp,v_11);


    push_vector_back(tmp,p_1);
    push_vector_back(tmp,p_2);
    push_vector_back(tmp,p_3);
    push_vector_back(tmp,p_4);
    push_vector_back(tmp,p_5);
    push_vector_back(tmp,p_6);
    push_vector_back(tmp,p_7);
    push_vector_back(tmp,p_8);

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
            logger.error(0) << "Error compiling shader, log: " << logger.end;
            logger.error(0) << infoLog << logger.end;
            logger.error(0) << "shader: " << logger.end;
            logger.error(0) << strings[0] << logger.end;
            delete[] infoLog;
            res = 1;
        }
        glDeleteShader(shader);
    }
    return res;
}
