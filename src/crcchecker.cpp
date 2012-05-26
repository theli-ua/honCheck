#include "crcchecker.h"
#include <zlib.h>

std::string const& CRCChecker::cmdOption() const
{
    static std::string cmd = "--crc";
    return cmd;
}
std::string const& CRCChecker::name() const
{
    static std::string cmd = "CRC Checker";
    return cmd;
}
std::string const& CRCChecker::reString() const
{
    static std::string cmd = ".*";
    return cmd;
}
int CRCChecker::Check(Manifest::Entry const& entry, std::vector<char> const& data) const
{
    if (entry.checksum() == 0 && entry.size() == 0)
    {
        return 0;
    }
    else
    {
        if (entry.checksum() != crc32(0,reinterpret_cast<const Bytef*>(&data[0]),data.size()))
            return 1;
    }

    return 0;
}
IChecker* CRCChecker::clone()
{
    return new CRCChecker();
}
