#include "shaderchecker.h"
#include "logger.h"
#include <algorithm>


int ShaderChecker::Check(Manifest::Entry const& entry, std::vector<char> const& bdata) const
{
	int result = 0;

	if(_defines.size() > 0)
	{
		int size = _defines[0].size();

		std::string data;
		data.assign(bdata.begin(),bdata.end());

		const char** strings = new const char*[size + 1];
		strings[size] = data.c_str();
		for(int i = 0 ; i < _defines.size(); ++i)
		{
			std::copy(_defines[i].begin(),_defines[i].end(),&strings[0]);
			result += Compile(strings,size + 1);
		}
		delete[] strings;
	}
	return result;
}
