#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include "logger.h"
#include <cstring>
#include "manifest.h"

Manifest::Manifest()
    :version(), os(), arch()
{
}
Manifest::~Manifest()
{
    xmlCleanupParser();
}

/**
 * @brief Parse XML Manifest from file
 *
 * @param path - path to manifest.xml file
 *
 * @return 0 - succes, nonzero - error
 */
int Manifest::Parse(std::string const& path)
{
    return Parse(path.c_str());
}

/**
 * @brief Parse XML Manifest from file
 *
 * @param path - path to manifest.xml file
 *
 * @return 0 - succes, nonzero - error
 */
int Manifest::Parse(const char* path)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
	Logger& logger = Logger::get_instance();

    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
    doc = xmlReadFile(path, NULL, 0);

    if (doc == NULL) {
        return -1;
    }
    root_element = xmlDocGetRootElement(doc); 
    if (!xmlStrEqual(root_element->name, reinterpret_cast<const xmlChar*>("manifest")))
    {
        logger.error(0) << "Root node is not manifest node!" << logger.end;
        xmlFreeDoc(doc);
        return -1;
    }
    for (xmlNode *cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrEqual(cur_node->name, reinterpret_cast<const xmlChar*>("file")))
            {
                logger.verbose(0) << "Found non-file node in manifest: " << cur_node->name << logger.end;
            }
            else
            {
            }
        }
    }

    xmlFreeDoc(doc);
    return 0;
}

Manifest::Entry::Entry(xmlNode* node)
{
}

std::string const& Manifest::Entry::path()
{
    return _path;
}

unsigned long Manifest::Entry::size()
{
    return _size;
}

unsigned long Manifest::Entry::checksum()
{
    return _checksum;
}
