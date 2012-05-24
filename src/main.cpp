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
#include "checker.h"
#include "resreader.h"

#include "crcchecker.h"
#include "glslchecker.h"

namespace TC = TermAnsiColor;

// Not optimal, but ok for a small vector such as args list (otherwise id sort
// and use binary_search
template <typename T> bool contains( const std::vector<T>& a, const T& b ) {
  return std::find(a.begin(),a.end(),b) != a.end();
}

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv + 1/* skip binary name */, 
            argv + argc /* this is an invalid address but works as a valid end for this case */);

    std::ostream verbose(std::cout.rdbuf());
	std::ostream error(std::cerr.rdbuf());
	std::ostream trace(std::cout.rdbuf());

    std::vector<IChecker*> checkers;

	Logger& logger = Logger::get_instance();
    logger.set_error(error, 0);

    if (contains(args,std::string("--verbose")))
        logger.set_verbose(verbose, 0);
    if (contains(args,std::string("--trace")))
        logger.set_trace(trace);

    /* Add checker classes here */

    checkers.push_back(new CRCChecker());
    checkers.push_back(new GLSLVSChecker());
    checkers.push_back(new GLSLPSChecker());

    /* Stop adding checker classes */

    if (contains(args,std::string("--help")))
    {
        logger.error(0) << "Options: " << logger.end;
        logger.error(0) << "\t--help\t\tthis message" << logger.end;
        logger.error(0) << "\t--verbose\tverbose output" << logger.end;
        logger.error(0) << "\t--trace\t\ttrace output (slows down execution a lot)" << logger.end;
        logger.error(0) << "\t--dir <dir>\tdirectory to read manifest/files from(default is current directory)" << logger.end;
        logger.error(0) << "Checkers' switches:" << logger.end;
        for (std::vector<IChecker*>::iterator it = checkers.begin() ; it != checkers.end();
                ++it)
        {
            IChecker const& checker = **it;
            logger.error(0) << "\t" << checker.cmdOption() << "\tdisable ";
            logger.error(0) << checker.name() << logger.end;
        }
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

    for(int i = 0 ; i < checkers.size() ; ++i)
    {
        if(contains(args,checkers[i]->cmdOption()))
        {
            delete(checkers[i]);
            checkers.erase(checkers.begin() + i--);
        }
        else
        {
            checkers[i]->Initialise();
        }
    }

    logger.verbose(0) << "Enabled checkers:" << logger.end;
    if(checkers.empty())
    {
        logger.verbose(0) << "\tNone" << logger.end;
    }
    else
    {
        for (std::vector<IChecker*>::iterator it = checkers.begin() ; it != checkers.end();
                ++it)
        {
            logger.verbose(0) << "\t" << (*it)->name() << logger.end;
        }
    }

    Manifest manifest;

    if (0 != manifest.Parse("manifest.xml"))
    {
        logger.error(0) << "Could not parse manifest.xml" << logger.end;
        return -1;
    }

    logger.verbose(0) << "Total entries in manifest: " << manifest.size() << logger.end;

    ResourceReader resReader;

    int errCount = 0;

    int size = manifest.size();

    int processed = 0;
    
#pragma omp parallel for private(resReader) firstprivate(manifest,checkers,size) schedule(dynamic) default(none)\
    shared(errCount,processed,logger)
    for(int i = 0; i < size ; ++i)
    {
        Manifest::Entry const& entry = manifest[i];

        std::vector<char> data;
        bool read = false;
        
        for (std::vector<IChecker*>::iterator it = checkers.begin() ; it != checkers.end();
                ++it)
        {
            if((*it)->Match(entry))
            {
                if(!read)
                {
                    data = resReader.Read(entry);
                    read = true;
                }
                if ((*it)->Check(entry,data) != 0)
                {
                    logger.error(0) << (*it)->name() << " returned error for" << logger.end;
                    logger.error(0) << "\t" << entry.path() << logger.end;
#pragma omp atomic
                    errCount++;
                }
            }
        }
#pragma omp atomic
        processed++;
        logger.verbose(0) << "Processed: " << processed << "/" << size << logger.rend;
    }
    logger.verbose(0) << logger.end;


    logger.error(0) << "Total errors: " << errCount << logger.end;


    for (std::vector<IChecker*>::iterator it = checkers.begin() ; it != checkers.end();
            ++it)
    {
        delete(*it);
    }
    return errCount;
}
