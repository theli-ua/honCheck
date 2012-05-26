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
    GLSLChecker();
    std::string const& cmdOption() const;
    std::string const& name() const;
    protected:
    GLuint type;
    int Compile(const char** strings,int stringCount) const;
};

class GLSLVSChecker : public GLSLChecker
{
    public:
    std::string const& reString() const;
    GLSLVSChecker();
    IChecker* clone();
};
class GLSLPSChecker : public GLSLChecker
{
    public:
    std::string const& reString() const;
    GLSLPSChecker();
    IChecker* clone();
};
#endif
