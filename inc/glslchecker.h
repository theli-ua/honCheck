#ifndef _GLSL_CHECKER_H
#define _GLSL_CHECKER_H
#include "shaderchecker.h"
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
};
class GLSLPSChecker : public GLSLChecker
{
    public:
    std::string const& reString() const;
    GLSLPSChecker();
};
#endif
