#ifdef _WIN32
#include "hlslchecker.h"
#include <vector>
#include "utils.h"

HLSLChecker::HLSLChecker(const char* target,const char* regex,const char* pEntryPoint):
    target(target),_regex(regex),pEntryPoint(pEntryPoint),resReader()
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
    LPD3DXBUFFER ppShader;
    LPD3DXBUFFER ppErrorMsgs;
    Logger& logger = Logger::get_instance();
    std::vector<D3DXMACRO> defines;
    /* I assume all defines are in form
     * "#define NAME VALUE\n" ....
     * as that what i have defined them like in 
     * shaderchecker.cpp
     */
    for (int i = 0 ; i < stringCount - 1; ++i)
    {
        if(strings[i][0] == '\0')
            continue;
        std::vector<std::string> parts = split(strings[i],"\n",false);
        for(std::vector<std::string>::iterator it = parts.begin();
            it != parts.end(); ++it)
        {
            std::vector<std::string> define = split(*it," ");

            char* Definition = new char[define[2].size() + 1];
            Definition[define[2].size()] = 0;
            std::copy(define[2].begin(),define[2].end(),Definition);

            char* Name = new char[define[1].size() + 1];
            Name[define[1].size()] = 0;
            std::copy(define[1].begin(),define[1].end(),Name);

            D3DXMACRO macro = {Name,Definition};
            defines.push_back(macro);
        }
    }
    D3DXMACRO macro = {0,0};
    defines.push_back(macro);

    /*
    HRESULT D3DXCompileShader(
      __in   LPCSTR pSrcData,
      __in   UINT srcDataLen,
      __in   const D3DXMACRO *pDefines,
      __in   LPD3DXINCLUDE pInclude,
      __in   LPCSTR pFunctionName,
      __in   LPCSTR pProfile,
      __in   DWORD Flags,
      __out  LPD3DXBUFFER *ppShader,
      __out  LPD3DXBUFFER *ppErrorMsgs,
      __out  LPD3DXCONSTANTTABLE *ppConstantTable
    );
    */
    HRESULT result = D3DXCompileShader(strings[stringCount-1], strlen(strings[stringCount-1]),
        &defines[0],(LPD3DXINCLUDE)this,pEntryPoint,target,NULL,&ppShader,&ppErrorMsgs,NULL);

    if (result != D3D_OK)
#pragma omp critical (error)
    {
        DWORD size = ppErrorMsgs->GetBufferSize();
        LPCSTR ErrorMsgs = reinterpret_cast<LPCSTR>(ppErrorMsgs->GetBufferPointer());
        std::string err;
        err.assign(ErrorMsgs,size);
        logger.error(0) << "Error compiling HLSL shader:" <<logger.end;
		logger.error(0) << err << logger.end;
		logger.error(0) << "Defines combination: " << logger.end;
		for (int i = 0; i < stringCount - 1 ;++i)
			logger.error(0) << strings[i] << logger.end;
		logger.error(0) << logger.end;
	}

    for(std::vector<D3DXMACRO>::iterator it = defines.begin();
        it != defines.end(); ++it)
    {
        if(it->Definition)
            delete[] it->Definition;
        if(it->Name)
            delete[] it->Name;
    }

	if(ppErrorMsgs)
		ppErrorMsgs->Release();
	if(ppShader)
		ppShader->Release();
    return result;
}
IChecker* HLSLChecker::clone() const
{
    return new HLSLChecker(target,_regex.c_str(),pEntryPoint);
}
HRESULT HLSLChecker::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	/* Compiler refuses to pass anything in pParentData argument
     * thus i'm just "having a number of standard locations"
     * as MSDN recommends to do
     */
	std::string realPath = "game/core/shaders/";
	if(pFileName[0] == '.')
	{
		realPath += "whatever/";
		realPath += pFileName;
	}
	else
	{
		realPath += target;
		realPath += "/";
		realPath += pFileName;
	}

    if(includeCache.find(realPath) == includeCache.end())
    {
        includeCache[realPath] = resReader.Read(realPath.c_str());
    }
	*pBytes = includeCache[realPath].size();
	if(*pBytes != 0)
		*ppData = &includeCache[realPath][0];
	else
		*ppData = NULL;
    return S_OK;
}
HRESULT HLSLChecker::Close(void const *pData)
{
    return S_OK;
}
#endif
