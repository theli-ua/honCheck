#ifndef _SHADERCHECKER_H
#define _SHADERCHECKER_H
#include "checker.h"
#include "manifest.h"

class ShaderChecker: public IChecker
{
    public:
    int Check(Manifest::Entry const& entry, std::vector<char> const& data) const;

    protected:
    /**
     * @brief Compiles a shader
     *
     * @param strings array of string pointer for shader source code
     * @param stringCount count of strings for shader code
     *
     * @return error code
     */
    virtual int Compile(const char** strings,int stringCount) const = 0;
    /**
     * @brief array of defines
     */
    char*** _defines;
};
#endif
