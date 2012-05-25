#include "shaderchecker.h"
#include "logger.h"
#include "cart_product.h"
#include <algorithm>

static CartesianProduct<const char*> cart_product;

int ShaderChecker::Check(Manifest::Entry const& entry, std::vector<char> const& bdata) const
{
	int result = 0;

	if(_defines.size() > 0)
	{
		std::string data;
		data.assign(bdata.begin(),bdata.end());

		std::vector<std::vector<const char*> > neededDefines;
		/*
		 * Cause compiling each shader 40k times for each of possible defines' combination
		 * is totally not cool we'll just take only needed ones
		 */
		for(ShaderChecker::DefinesV::const_iterator it = _defines.begin();
			it != _defines.end(); ++it)
		{
			const std::vector<const char*> &Keys = it->first;
			const std::vector<const char*> &Defines = it->second;
			for(std::vector<const char*>::const_iterator keyIt = Keys.begin();
				keyIt != Keys.end(); ++keyIt)
			{
				if(data.find(*keyIt,0) != std::string::npos)
				{
					neededDefines.push_back(std::vector<const char*>(Defines.begin(),Defines.end()));
					break;
				}
			}
		}
		std::vector<std::vector<const char*> > combinedDefines;
		cart_product(combinedDefines,neededDefines);
		
		int size = 0;
		if(combinedDefines.size() > 0)
			size = combinedDefines[0].size();


		const char** strings = new const char*[size + 1];
		strings[size] = data.c_str();
		if (size == 0)
		{
			result += Compile(strings,1);
		}
		else
		{
			for(int i = 0 ; i < combinedDefines.size(); ++i)
			{
				std::copy(combinedDefines[i].begin(),combinedDefines[i].end(),&strings[0]);
				result += Compile(strings,size + 1);
			}
		}
		delete[] strings;
		
	}
	return result;
}
