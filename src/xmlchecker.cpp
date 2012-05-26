#include "xmlchecker.h"
#include <libxml/parser.h>

std::string const& XMLChecker::cmdOption() const
{
    static std::string cmd = "--xml";
    return cmd;
}
std::string const& XMLChecker::name() const
{
    static std::string cmd = "XML Checker";
    return cmd;
}
std::string const& XMLChecker::reString() const
{
    static std::string cmd = ".*\\.(entity|material|package|interface|mdf|effect|gamemechanics|upgrades)";
    return cmd;
}
int XMLChecker::Check(Manifest::Entry const& entry, std::vector<char> const& data) const
{
    xmlDoc *doc = NULL;
    doc = xmlReadMemory(&data[0],data.size(),NULL,NULL,0);
    if (doc != NULL)
    {
        xmlFreeDoc(doc);
        return 0;
    }

    return 1;
}
IChecker* XMLChecker::clone()
{
    return new XMLChecker();
}
