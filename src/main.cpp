/**
 * @brief Main file
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>

#include "logger.h"
#include "ansi_colors.h"

#include "manifest.h"

namespace TC = TermAnsiColor;


// Not optimal, but ok for a small vector such as args list (otherwise id sort
// and use binary_search
template <typename T> bool contains( const std::vector<T>& a, const T& b ) {
  return std::find(a.begin(),a.end(),b) != a.end();
}

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv + 1/* skip binary name */, argv + argc);

    std::ostream verbose(std::cout.rdbuf());
	std::ostream error(std::cerr.rdbuf());
	std::ostream trace(std::cout.rdbuf());


	Logger& logger = Logger::get_instance();
    logger.set_error(error, 0);

    if (contains(args,std::string("--verbose")))
        logger.set_verbose(verbose, 0);
    if (contains(args,std::string("--trace")))
        logger.set_trace(trace);


    if (contains(args,std::string("--help")))
    {
        logger.error(0) << "options: " << logger.end;
        logger.error(0) << "\t--help\t\tthis message" << logger.end;
        logger.error(0) << "\t--verbose\tverbose output" << logger.end;
        //logger.error(0) << "\t--trace\t\ttrace output" << logger.end;
        //unused ^^
        logger.error(0) << "\t--dir <dir>\tdirectory to read manifest/files from" << logger.end;
        return 0;
    }
    std::vector<std::string>::iterator it = std::find(args.begin(),args.end(),std::string("--dir"));
    if (it != args.end() && ++it != args.end())
    {
        logger.verbose(0) << "Changing current directory to " << *it << logger.end;
        if (chdir(it->c_str()) != 0)
        {
            logger.error(0) << "Could not change directory to ";
            logger.error(0) << TC::GREEN << *it << logger.end;
            return -1;
        }
    }

    Manifest manifest;

    if (0 != manifest.Parse("manifest.xml"))
    {
        logger.error(0) << "Could not parse manifest.xml" << logger.end;
        return -1;
    }






	//logger.set_verbose(verbose, opt.progress ? 2 : 0);
	//logger.set_error(error, opt.progress ? 2 : (opt.very_verbose ? -2 : 0));
	//logger.set_trace(trace);
    
}
