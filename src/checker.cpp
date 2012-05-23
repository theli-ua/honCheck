#include "checker.h"

IChecker::IChecker():
    comp(NULL)
{
}

IChecker::~IChecker()
{
    if (comp != NULL)  
        xmlRegFreeRegexp(comp);  
    xmlCleanupParser();
}

int IChecker::Initialise()
{
	Logger& logger = Logger::get_instance();
    logger.trace(0) << "Compiling regular expression for checker " << name() << logger.end;
    logger.trace(0) << "Expression " << reString() << logger.end;
    comp = xmlRegexpCompile(reinterpret_cast<const xmlChar *>(reString().c_str()));  
    if (comp == NULL)
    {
        logger.error(0) << "Failed to compile matching expression for " << name() ;
        logger.error(0) << " checker." << logger.end;
        logger.error(0) << "Expression was: " << reString() << logger.end;
        return 1;
    }
    return 0;
}
bool IChecker::Match(Manifest::Entry const& entry) const
{
    return xmlRegexpExec(comp,reinterpret_cast<const xmlChar*>(entry.path().c_str())) == 1;
}
