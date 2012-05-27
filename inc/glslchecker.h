#ifndef _GLSL_CHECKER_H
#define _GLSL_CHECKER_H
#include "shaderchecker.h"
#include <GL/gl.h>
#ifndef _WIN32
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include "GL/wglext.h"
#endif

class GLSLChecker : public ShaderChecker
{
    public:
    GLSLChecker(GLuint type,const char* regex);
    std::string const& cmdOption() const;
    std::string const& name() const;
    std::string const& reString() const;
    IChecker* clone();
    protected:
    int Compile(const char** strings,int stringCount) const;
    private:
    std::string _regex;
    GLuint type;
};
#endif
