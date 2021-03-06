#ifndef _XMLCHECKER_H
#define _XMLCHECKER_H
#include "checker.h"

/**
 * @brief Implements IChecker to check for malformed XML files
 */
class XMLChecker : public IChecker
{
    public:
        std::string const& cmdOption() const;
        std::string const& name() const;
        std::string const& reString() const;
        int Check(Manifest::Entry const& entry, std::vector<char> const& data) const;
        IChecker* clone() const;
};
#endif
