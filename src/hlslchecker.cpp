#ifdef _WIN32
#include "hlslchecker.h"
#include "d3dcompiler.h"

HLSLChecker::HLSLChecker(const char* target,const char* regex):
    target(target),_regex(regex)
{
    prologue = NULL;
}

std::string const& HLSLChecker::cmdOption() const
{
    static std::string cmd = "--hlsl";
    return cmd;
}
std::string const& HLSLChecker::name() const
{
    static std::string cmd = "HLSL Checker";
    return cmd;
}
std::string const& HLSLChecker::reString() const
{
    return _regex;
}
int HLSLChecker::Compile(const char** strings,int stringCount, const char* path) const
{
    D3DCompile(NULL, 0,NULL,NULL,NULL,NULL,NULL,0,0,NULL,NULL);
    return 0;
}
IChecker* HLSLChecker::clone() const
{
    return new HLSLChecker(target,_regex.c_str());
}
#endif
