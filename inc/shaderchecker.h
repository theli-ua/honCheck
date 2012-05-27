#ifndef _SHADERCHECKER_H
#define _SHADERCHECKER_H
#include "checker.h"
#include "manifest.h"
#include <vector>
#include <map>

/**
 * @brief Common class for all shader checkers. Will produce a product of
 * define combinations and feed them into Compile method. Abstract.
 */
class ShaderChecker: public IChecker
{
    public:
	typedef std::pair<std::vector<const char*>,std::vector<const char*> > DefinesPair;
	typedef std::vector<DefinesPair> DefinesV;
    int Check(Manifest::Entry const& entry, std::vector<char> const& data) const;

    protected:
    /**
     * @brief Compiles a shader
     *
     * @param strings array of string pointer for shader source code
     * @param stringCount count of strings for shader code
     * @param path source file path
     *
     * @return error code
     */
    virtual int Compile(const char** strings,int stringCount,const char* path) const = 0;

    /**
     * @brief String to include as first in each set of defines
     */
    const char* prologue;
};
#endif
