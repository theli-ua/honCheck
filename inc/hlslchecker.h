#ifdef _WIN32
#ifndef _HLSLCHECKER_H
#define _HLSLCHECKER_H
#include "shaderchecker.h"
#include "D3DX9Shader.h"
#include "resreader.h"

/**
 * @brief Implements IChecker to check HLSL shaders
 */
class HLSLChecker : public ShaderChecker,public ID3DXInclude
{
    public:
        /**
         * @brief Constructor
         *
         * @param target target for compiling, "ps_2_0" etc
         * @param regex regular expression to match filepath
         * @param pEntryPoint string with shaders entry point
         */
        HLSLChecker(const char* target,const char* regex,const char* pEntryPoint);
        std::string const& cmdOption() const;
        std::string const& name() const;
        std::string const& reString() const;
        IChecker* clone() const;
        STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
        STDMETHOD(Close)(LPCVOID pData);
    protected:
        int Compile(const char** strings,int stringCount,const char* path) const;

    private:
        const char* target;
        const char* pEntryPoint;
        std::string _regex;
        ResourceReader resReader;
		std::map<std::string,std::vector<char> > includeCache;
};
#endif
#endif
