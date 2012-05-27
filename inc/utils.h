#ifndef _UTILS_H
#define _UTILS_H
#include <vector>
#include <string>
/**
 * @brief Splits std::string using std::string as delimiter
 *
 * @param s input string
 * @param delim delimiter
 * @param keep_empty whether to keep empty strings, default - true
 *
 * @return 
 */
std::vector<std::string> split(const std::string& s, const std::string& delim, const bool keep_empty = true);
#endif
