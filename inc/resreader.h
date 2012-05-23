#ifndef _RESREADER_H
#define _RESREADER_H
#include "manifest.h"

class ResourceReader
{
    public:
    /**
     * @brief Read entry's data and return it
     *
     * @param entry Manifest::Entry
     *
     * @return entry's data in a std::string buffer
     */
    std::string Read(Manifest::Entry entry);
};
#endif
