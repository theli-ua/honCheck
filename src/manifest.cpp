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
    version.assign(reinterpret_cast<const char*>(xmlGetProp(root_element,reinterpret_cast<const xmlChar*>("version"))));
    if ( version == "" )
    {
        logger.error(0) << "manifest tag is missing version attribute" << logger.end;
    }
    else
    {
        logger.verbose(0) << "manifest version: " << version << logger.end;
    }
    os.assign(reinterpret_cast<const char*>(xmlGetProp(root_element,reinterpret_cast<const xmlChar*>("os"))));
    if ( version == "" )
    {
        logger.error(0) << "manifest tag is missing os attribute" << logger.end;
    }
    else
    {
        logger.verbose(0) << "manifest os: " << os << logger.end;
    }
    arch.assign(reinterpret_cast<const char*>(xmlGetProp(root_element,reinterpret_cast<const xmlChar*>("arch"))));
    if ( version == "" )
    {
        logger.error(0) << "manifest tag is missing arch attribute" << logger.end;
    }
    else
    {
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

    _path.assign(reinterpret_cast<const char*>(xmlGetProp(node,
                    reinterpret_cast<const xmlChar*>("path"))));
    if (_path == "")
    {
        logger.error(0) << "Node is missing path attribute" << logger.end;
    }
    else
    {
        logger.trace(0) << "Node path: " << _path << logger.end;
    }


    {
        std::stringstream ss;
        ss << std::hex << reinterpret_cast<const char*>(xmlGetProp(node,
                    reinterpret_cast<const xmlChar*>("checksum")));
        ss >> _checksum;
    }

    if(_checksum == 0)
    {
        logger.error(0) << "Node is missing checksum attribute (";
        logger.error(0) << _path << ")" << logger.end;
    }
    else
    {
        logger.trace(0) << "Node checksum: " << _checksum << logger.end;
    }

    {
        std::stringstream ss;
        ss << reinterpret_cast<const char*>(xmlGetProp(node,
                    reinterpret_cast<const xmlChar*>("size")));
        ss >> _size;
    }


    if(_size == 0)
    {
        logger.error(0) << "Node is missing size attribute (";
        logger.error(0) << _path << ")" << logger.end;
    }
    else
    {
        logger.trace(0) << "Node size: " << _size << logger.end;
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
