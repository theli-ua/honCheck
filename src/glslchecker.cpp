#include "glslchecker.h"
#include "GL/glext.h"
#include "logger.h"
#ifndef _WIN32
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#else
//TODO
#endif

static char v[][4][3] = { {"a", "b", 0}, {"c", "d", "e", 0}, 0};



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
    static std::string cmd = ".*/ps_glsl.*\\.vsh";
    return cmd;
}

GLSLVSChecker::GLSLVSChecker() : GLSLChecker()
{
    type = GL_VERTEX_SHADER;
    _defines = reinterpret_cast<char***>(&v);
}
GLSLPSChecker::GLSLPSChecker() : GLSLChecker()
{
    type = GL_FRAGMENT_SHADER;
    _defines = reinterpret_cast<char***>(&v);
}


int GLSLChecker::Compile(const char** strings,int stringCount) const
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
        delete[] infoLog;
        glDeleteShader(shader);
        return 1;
    }
    glDeleteShader(shader);
    return 0;
}
