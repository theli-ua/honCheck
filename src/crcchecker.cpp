#include "crcchecker.h"

std::string const& CRCChecker::cmdOption() const
{
    static std::string cmd = "--no-crc";
    return cmd;
}
std::string const& CRCChecker::name() const
{
    static std::string cmd = "CRC Checker";
    return cmd;
}
std::string const& CRCChecker::reString() const
{
    static std::string cmd = ".*so";
    return cmd;
}
int CRCChecker::Check(Manifest::Entry const& entry, std::string const& data) const
{
    printf("%s\n",entry.path().c_str());
    if (entry.checksum() == 0 || entry.size() != data.size())
    {
        return 1;
    }
    return 0;
}
