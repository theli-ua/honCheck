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

std::string ResourceReader::Read(Manifest::Entry entry)
{
	Logger& logger = Logger::get_instance();
    std::vector<std::string> pathElements = split(entry.path(),".s2z/");
    std::string data;
    if(pathElements.size() == 1)
    {
        //unpacked file
        std::ifstream file (entry.path().c_str(), std::ios::in|std::ios::binary|std::ios::ate);
        if (file.tellg() == entry.size())
        {
            file.seekg(0, std::ios::beg);
            data.assign((std::istreambuf_iterator<char>(file)),
                    (std::istreambuf_iterator<char>()));
        }
    }

    return data;
}
