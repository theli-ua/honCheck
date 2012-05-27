#include "shaderchecker.h"
#include "logger.h"
#include "cart_product.h"
#include <algorithm>

static CartesianProduct<const char*> cart_product;

/*  k_ ones are a strings to search in shader to include current set of defines
 *  for combinations product
 *  after all matching sets are found cartesian product is build on them
 *  and the shader is compiled with each combination
 */
static const char*  k_1 [] = {  
	"LIGHTING",
	"LIGHTING_QUALITY",
	"REFLECTIONS",
};
static const char*  d_1 [] = {  
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 0\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 2\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 0\n#define REFLECTIONS 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 1\n#define REFLECTIONS 1\n",
	"#define LIGHTING 1\n#define LIGHTING_QUALITY 2\n#define REFLECTIONS 1\n",
	//"#define LIGHTING 0\n",
};
static const char*  k_2 [] = { "CLOUDS" };
static const char*  d_2 [] = { 
	"#define CLOUDS\n",
	""
};
static const char*  k_3 [] = { "TERRAIN_DEREPEAT" };
static const char*  d_3 [] = { 
	"#define TERRAIN_DEREPEAT 1\n",
	"#define TERRAIN_DEREPEAT 0\n"
};
static const char* k_4 [] = {
    "FALLOFF_QUALITY",
    "FOG_TYPE",
    "FOG_OF_WAR",
    "FOG_QUALITY",
};
static const char* d_4 [] = {
    //"#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    //"#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 0\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 0\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 1\n#define FALLOFF_QUALITY 0\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 1\n",
    "#define FOG_TYPE 1\n#define FOG_OF_WAR 1\n#define FOG_QUALITY 0\n#define FALLOFF_QUALITY 0\n",
};    
//static const char*  k_4 [] = { "FALLOFF_QUALITY" };
//static const char*  d_4 [] = { 
	//"#define FALLOFF_QUALITY 1\n",
	//"#define FALLOFF_QUALITY 0\n"
//};
//static const char*  k_5 [] = { "FOG_TYPE" };
//static const char*  d_5 [] = { 
	//"#define FOG_TYPE 0\n",
	//"#define FOG_TYPE 1\n"
//};
//static const char*  k_10 [] = { "FOG_QUALITY" };
//static const char*  d_10[] = { 
	//"#define FOG_QUALITY 0\n",
	//"#define FOG_QUALITY 1\n"
//};
//static const char*  k_8 [] = { "FOG_OF_WAR" };
//static const char*  d_8 [] = { 
	//"#define FOG_OF_WAR 0\n",
	//"#define FOG_OF_WAR 1\n"
//};
static const char*  k_6 [] = { "SHADOWS" };
static const char*  d_6 [] = { 
	"#define SHADOWS 0\n",
	"#define SHADOWS 1\n"
};
static const char*  k_7 [] = { "NUM_BONES" };
static const char*  d_7 [] = { 
	"#define NUM_BONES 1\n#define NUM_BONE_WEIGHTS 3\n",
	"#define NUM_BONES 0\n"
};
static const char*  k_9 [] = { "SHADOWMAP_TYPE" };
static const char*  d_9 [] = { 
	"#define SHADOWMAP_TYPE 0\n",
	"#define SHADOWMAP_TYPE 1\n"
};
static const char*  k_11 [] = { "GROUND_AMBIENT" };
static const char*  d_11 [] = { 
	"#define GROUND_AMBIENT 1\n",
	""
};
static const char*  k_12 [] = { "TERRAIN_ALPHAMAP" };
static const char*  d_12 [] = { 
	"#define TERRAIN_ALPHAMAP 1\n",
	""
};
static const char*  k_13 [] = { "RXGB_NORMALMAP" };
static const char*  d_13 [] = { 
	"#define RXGB_NORMALMAP 1\n",
	""
};
static const char*  k_14 [] = { "NUM_POINT_LIGHTS" };
static const char*  d_14 [] = { 
	"#define NUM_POINT_LIGHTS 1\n",
	"#define NUM_POINT_LIGHTS 0\n"
};
static const char*  k_15 [] = { "SHADOWMAP_FILTER_WIDTH" };
static const char*  d_15 [] = { 
	"#define SHADOWMAP_FILTER_WIDTH 1\n",
	"#define SHADOWMAP_FILTER_WIDTH 0\n"
};
static const char*  k_16 [] = { "TEXKILL" };
static const char*  d_16 [] = { 
	"#define TEXKILL 1\n",
	"#define TEXKILL 0\n"
};
static const char*  k_17 [] = { "WATER_QUALITY" };
static const char*  d_17 [] = { 
	"#define WATER_QUALITY 0\n",
	"#define WATER_QUALITY 1\n",
	"#define WATER_QUALITY 2\n",
	"#define WATER_QUALITY 3\n"
};

template <typename T,size_t K,size_t N>
inline void add_define_pair(std::vector<std::pair<std::vector<T>,std::vector<T> > >& v,T (&k)[K],T (&d)[N])
{
    v.push_back(
		std::pair<std::vector<T>,std::vector<T> >(
			std::vector<T>(&k[0], &k[0] + K),
			std::vector<T>(&d[0], &d[0] + N)
			)
		);
}
#define ADD_SHADER_MACRO_DEFINE(N) add_define_pair(_defines,k_##N,d_##N)

static std::vector<std::pair<std::vector<const char*>,std::vector<const char*> > > _gen_defines()
{
std::vector<std::pair<std::vector<const char*>,std::vector<const char*> > > _defines;
    ADD_SHADER_MACRO_DEFINE(1);
    ADD_SHADER_MACRO_DEFINE(2);
    ADD_SHADER_MACRO_DEFINE(3);
    ADD_SHADER_MACRO_DEFINE(4);
    //ADD_SHADER_MACRO_DEFINE(5);
    ADD_SHADER_MACRO_DEFINE(6);
    ADD_SHADER_MACRO_DEFINE(7);
    //ADD_SHADER_MACRO_DEFINE(8);
    ADD_SHADER_MACRO_DEFINE(9);
    //ADD_SHADER_MACRO_DEFINE(10);
    ADD_SHADER_MACRO_DEFINE(11);
    ADD_SHADER_MACRO_DEFINE(12);
    ADD_SHADER_MACRO_DEFINE(13);
    ADD_SHADER_MACRO_DEFINE(14);
    ADD_SHADER_MACRO_DEFINE(15);
    ADD_SHADER_MACRO_DEFINE(16);
    ADD_SHADER_MACRO_DEFINE(17);
    return _defines;
}

int ShaderChecker::Check(Manifest::Entry const& entry, std::vector<char> const& bdata) const
{
    /* those are the same for all shaders, gonna initialize once and reuse */
    static std::vector<std::pair<std::vector<const char*>,std::vector<const char*> > > _defines = _gen_defines();
	int result = 0;

	if(_defines.size() > 0)
	{
		std::string data;
		data.assign(bdata.begin(),bdata.end());

		std::vector<std::vector<const char*> > neededDefines;
        if(prologue != NULL)
        {
            neededDefines.push_back(std::vector<const char*>());
            neededDefines.back().push_back(prologue);
        }
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
			result += Compile(strings,1,entry.path().c_str());
		}
		else
		{
			for(int i = 0 ; i < combinedDefines.size() && result == 0; ++i)
			{
				std::copy(combinedDefines[i].begin(),combinedDefines[i].end(),&strings[0]);
				result += Compile(strings,size + 1,entry.path().c_str());
			}
		}
		delete[] strings;
		
	}
	return result;
}
