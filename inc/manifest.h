/**
 * @file manifest.h
 * @brief Class For HoN's file manifest
 * @author Anton Romanov
 * @version 1
 * @date 2012-05-22
 */
#include <vector>
#include <string>
#include <libxml/tree.h>

/**
 * @brief Class for handling xml Manifest files
 */
class Manifest
{
    public:
    class Entry
    {
        public:
            Entry(xmlNode* node);
            std::string const& path();
            unsigned long size();
            unsigned long checksum();
        private:
            std::string _path;
            unsigned long _size;
            unsigned long _checksum;
    };
    Manifest();
    ~Manifest();
    int Parse(std::string const& path);
    int Parse(const char *filename);

    private:
    std::string version;
    std::string os;
    std::string arch;
    std::vector<Entry> entries;
};
