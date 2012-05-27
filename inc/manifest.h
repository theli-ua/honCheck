#ifndef _MANIFEST_H
#define _MANIFEST_H

#include <vector>
#include <string>
#include <libxml/tree.h>

/**
 * @brief Class for handling xml Manifest files
 */
class Manifest
{
    public:
        /**
         * @brief Entry in the manifest
         */
    class Entry
    {
        public:
            /**
             * @brief Manifest's entry constructor
             *
             * @param node pointer to xmlNode \<file/\>
             */
            Entry(xmlNode* node);
            /**
             * @brief
             *
             * @return entry's path value
             */
            std::string const& path() const;
            /**
             * @brief
             *
             * @return entry's size value
             */
            unsigned long size() const;
            /**
             * @brief
             *
             * @return Entry's checksum value
             */
            unsigned long checksum() const;
        private:
            std::string _path;
            unsigned long _size;
            unsigned long _checksum;
    };
    Manifest();
    ~Manifest();
    /**
     * @brief Parse XML Manifest from file
     *
     * @param path - path to manifest.xml file
     *
     * @return 0 - succes, nonzero - error
     */
    int Parse(std::string const& path);
    /**
     * @brief Parse XML Manifest from file
     *
     * @param path - path to manifest.xml file
     *
     * @return 0 - succes, nonzero - error
     */
    int Parse(const char *path);
    /**
     * @brief 
     *
     * @return number of entries in manifest
     */
    std::vector<Entry>::size_type size();

    /**
     * @brief Return const reference to entry by index
     *
     * @param i index
     *
     * @return  const reference to entry
     */
    Entry const& operator[](int i)const;

    private:
    std::string version;
    std::string os;
    std::string arch;
    std::vector<Entry> entries;
};
#endif
