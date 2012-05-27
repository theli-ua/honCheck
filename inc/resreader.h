#ifndef _RESREADER_H
#define _RESREADER_H
#include "manifest.h"
#include "unzip.h"
#include <map>

/**
 * @brief Class to read raw/zipped files
 */
class ResourceReader
{
    public:
    ResourceReader();
    ~ResourceReader();
    /**
     * @brief Read entry's data and return it
     *
     * @param entry Manifest::Entry
     *
     * @return entry's data in a std::string buffer
     */
    std::vector<char> Read(Manifest::Entry entry);
    private:
    std::map<std::string,unzFile> zipFiles;
};
#endif
