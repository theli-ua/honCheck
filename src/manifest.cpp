#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include "logger.h"
#include <sstream>
#include <ios>
#include "manifest.h"

Manifest::Manifest()
    :version(), os(), arch()
{
}
Manifest::~Manifest()
{
#pragma omp critical (xmlCleanupParser)
    xmlCleanupParser();
}

int Manifest::Parse(std::string const& path)
{
    return Parse(path.c_str());
}

int Manifest::Parse(const char* path)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    xmlChar *str;
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
    str = xmlGetProp(root_element,reinterpret_cast<const xmlChar*>("version"));
    version.assign(reinterpret_cast<const char*>(str));
    if ( str == NULL )
    {
        logger.error(0) << "manifest tag is missing version attribute" << logger.end;
    }
    else
    {
        xmlFree(str);
        logger.verbose(0) << "manifest version: " << version << logger.end;
    }
    str = xmlGetProp(root_element,reinterpret_cast<const xmlChar*>("os"));
    os.assign(reinterpret_cast<const char*>(str));
    if ( str == NULL )
    {
        logger.error(0) << "manifest tag is missing os attribute" << logger.end;
    }
    else
    {
        xmlFree(str);
        logger.verbose(0) << "manifest os: " << os << logger.end;
    }
    str = xmlGetProp(root_element,reinterpret_cast<const xmlChar*>("arch"));
    arch.assign(reinterpret_cast<const char*>(str));
    if ( str == NULL )
    {
        logger.error(0) << "manifest tag is missing arch attribute" << logger.end;
    }
    else
    {
        xmlFree(str);
        logger.verbose(0) << "manifest arch: " << arch << logger.end;
    }
    for (xmlNode *cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrEqual(cur_node->name, reinterpret_cast<const xmlChar*>("file")))
            {
                logger.verbose(0) << "Found non-file node in manifest: " << cur_node->name << logger.end;
            }
            else
            {
                entries.push_back(Entry(cur_node));
            }
        }
    }

    xmlFreeDoc(doc);
    return 0;
}

std::vector<Manifest::Entry>::size_type Manifest::size()
{
    return entries.size();
}

Manifest::Entry::Entry(xmlNode* node)
{
	Logger& logger = Logger::get_instance();
    xmlChar *str;

    str = xmlGetProp(node,reinterpret_cast<const xmlChar*>("path"));
    _path.assign(reinterpret_cast<const char*>(str));
    if (str == NULL)
    {
        logger.error(0) << "Node is missing path attribute" << logger.end;
    }
    else
    {
        xmlFree(str);
        logger.trace(0) << "Node path: " << _path << logger.end;
    }


    str = xmlGetProp(node,reinterpret_cast<const xmlChar*>("checksum"));
    if (str != NULL)
    {
        std::stringstream ss;
        ss << std::hex << reinterpret_cast<const char*>(str);
        ss >> _checksum;
        xmlFree(str);
        if(_checksum == 0)
        {
            logger.error(0) << "Node has missing or zeroed checksum attribute (";
            logger.error(0) << _path << ")" << logger.end;
        }
        else
        {
            logger.trace(0) << "Node checksum: " << _checksum << logger.end;
        }
    }
    else
    {
        logger.error(0) << "Node has missing checksum attribute (";
        logger.error(0) << _path << ")" << logger.end;
    }


    str = xmlGetProp(node,reinterpret_cast<const xmlChar*>("size"));
    if (str != NULL)
    {
        std::stringstream ss;
        ss << reinterpret_cast<const char*>(str);
        ss >> _size;
        xmlFree(str);
    }
    else
    {
        logger.error(0) << "Node is missing size attribute (";
        logger.error(0) << _path << ")" << logger.end;
    }
}

std::string const& Manifest::Entry::path() const
{
    return _path;
}

unsigned long Manifest::Entry::size() const
{
    return _size;
}

unsigned long Manifest::Entry::checksum() const
{
    return _checksum;
}

Manifest::Entry const& Manifest::operator[](int i)const
{
    return entries[i];
}
