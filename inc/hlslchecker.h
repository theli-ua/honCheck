#ifndef _HLSLCHECKER_H
#define _HLSLCHECKER_H
#include "shaderchecker.h"

class HLSLChecker : public ShaderChecker
{
    public:
        HLSLChecker(const char* target,const char* regex);
        std::string const& cmdOption() const;
        std::string const& name() const;
        std::string const& reString() const;
        IChecker* clone() const;
    protected:
        int Compile(const char** strings,int stringCount,const char* path) const;

    private:
        const char* target;
        std::string _regex;
};
#endif
