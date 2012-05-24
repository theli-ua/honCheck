#include "shaderchecker.h"
#include "logger.h"


int ShaderChecker::Check(Manifest::Entry const& entry, std::vector<char> const& data) const
{
    /* we will generate a cartesian product of all possible define combinations
     * and try to compile with each */
	Logger& logger = Logger::get_instance();

    char*** p = _defines;
    while(*p != 0)
    {
        char** p2 = *p;
        while(*p2 != 0)
        {
            logger.error(0) << *p2 << " ";
            p2++;
        }
        logger.error(0) << logger.end;
        p++;
    }


    const char* aaa[] = {&data[0]};
    return Compile(aaa,1);
}
