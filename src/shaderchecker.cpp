#include "shaderchecker.h"
#include "logger.h"
#include <algorithm>


int ShaderChecker::Check(Manifest::Entry const& entry, std::vector<char> const& data) const
{
	Logger& logger = Logger::get_instance();

    int size = _defines.size();
    int result = 0;
    const char** strings = new const char*[size + 1];
    strings[size] = &data[0];
    for(int i = 0 ; i < size && result == 0; ++i)
    {
        std::copy(_defines[i].begin(),_defines[i].end(),strings);
        result = Compile(strings,size + 1);
    }
    return result;
}
