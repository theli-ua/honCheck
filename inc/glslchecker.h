#ifndef _GLSL_CHECKER_H
#define _GLSL_CHECKER_H
#include "shaderchecker.h"
#ifndef _WIN32
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include "GL/glext.h"
#ifdef _WIN32
#include "GL/wglext.h"
#endif

/**
 * @brief Implements IChecker to check GLSL shaders
 */
class GLSLChecker : public ShaderChecker
{
    public:
        /**
         * @brief GLSLChecker constructor
         *
         * @param type GLuint type to use when compiling
         * @param regex regular expression to match files' path
         */
        GLSLChecker(GLuint type,const char* regex);
        ~GLSLChecker();
        std::string const& cmdOption() const;
        std::string const& name() const;
        std::string const& reString() const;
        IChecker* clone() const;
    protected:
        int Compile(const char** strings,int stringCount,const char* path) const;
    private:
        std::string _regex;
        GLuint type;
#ifdef _WIN32
        HINSTANCE g_hInstance;
        HWND      g_hWnd;
        HDC       g_hDC;
        HGLRC     g_hRC;
#else
        Display *dpy;
        XVisualInfo *vi;
        GLXContext glc;
#endif
};
#endif
