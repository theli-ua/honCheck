#ifndef _GLSL_CHECKER_H
#define _GLSL_CHECKER_H
#include "shaderchecker.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif
#include <GL/gl.h>

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
