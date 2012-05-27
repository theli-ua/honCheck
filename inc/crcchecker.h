#ifndef _CRCCHECKER_H
#define _CRCCHECKER_H
#include "checker.h"

/**
 * @brief Implements IChecker to check checksum of files
 */
class CRCChecker : public IChecker
{
    public:
        std::string const& cmdOption() const;
        std::string const& name() const;
        std::string const& reString() const;
        int Check(Manifest::Entry const& entry, std::vector<char> const& data) const;
        IChecker* clone() const;
};
#endif
