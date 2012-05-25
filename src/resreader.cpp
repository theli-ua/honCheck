#include "resreader.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include "logger.h"

std::vector<std::string> split(const std::string& s, const std::string& delim, const bool keep_empty = true) {
    std::vector<std::string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    std::string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = std::search(substart, s.end(), delim.begin(), delim.end());
        std::string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

ResourceReader::ResourceReader():
    zipFiles()
{
}

ResourceReader::~ResourceReader()
{
    for(std::map<std::string,unzFile>::iterator it = zipFiles.begin();
            it != zipFiles.end(); ++it)
    {
        unzClose(it->second);
    }
}

std::vector<char> ResourceReader::Read(Manifest::Entry entry)
{
	Logger& logger = Logger::get_instance();
    std::vector<std::string> pathElements = split(entry.path(),".s2z/");
    std::vector<char> data;
    if(pathElements.size() == 1)
    {
        //unpacked file
        std::ifstream file (entry.path().c_str(), std::ios::in|std::ios::binary|std::ios::ate);
        //if (file.tellg() == entry.size())
        {
            file.seekg(0, std::ios::beg);
            data.assign((std::istreambuf_iterator<char>(file)),
                    (std::istreambuf_iterator<char>()));
        }
    }
    else //s2z
    {
        if(zipFiles.find(pathElements[0]) == zipFiles.end())
        {
            std::string s2zpath = pathElements[0] + ".s2z";
            zipFiles[pathElements[0]] = unzOpen(s2zpath.c_str());
            if(zipFiles[pathElements[0]] == NULL)
            {
                logger.error(0) << "Erorr opening " << s2zpath << logger.end;
            }
        }

        if(zipFiles.find(pathElements[0]) != zipFiles.end() && zipFiles[pathElements[0]] != NULL)
        {
            unzFile zip = zipFiles[pathElements[0]];
            if(unzLocateFile(zip, pathElements[1].c_str(),0 ) == UNZ_OK)
            {
                unz_file_info info;
                if(unzGetCurrentFileInfo( zip, &info, NULL, 0, NULL, 0, NULL, 0 ) == UNZ_OK)
                {
                    if(unzOpenCurrentFile( zip ) == UNZ_OK)
                    {
                        data.resize(info.uncompressed_size);
                        if(unzReadCurrentFile(zip, &data[0], info.uncompressed_size) != data.size())
                        {
                            data.clear();
                            logger.error(0) << "Could not read " << pathElements[1];
                            logger.error(0) << " inside " << pathElements[0] << logger.end;
                        }
                    }
                    else
                    {
                        logger.error(0) << "Could not open " << pathElements[1];
                        logger.error(0) << " inside " << pathElements[0] << logger.end;
                    }
                }
                else
                {
                    logger.error(0) << "Could not read info on " << pathElements[1];
                    logger.error(0) << " inside " << pathElements[0] << logger.end;
                }
            }
            else
            {
                logger.error(0) << "Could not locate " << pathElements[1];
                logger.error(0) << " inside " << pathElements[0] << logger.end;
            }
            unzCloseCurrentFile(zip);
        }
    }

    return data;
}
