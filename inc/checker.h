#ifndef _CHECKER_H
#define _CHECKER_H
#include <libxml/xmlregexp.h>
#include "logger.h"
#include "manifest.h"

/**
 * @brief Common interface to all checkers. Abstract
 */
class IChecker
{
    public:
        IChecker();
        ~IChecker();
        /**
         * @brief Initialise this checker (will compile regex at least)
         */
        int Initialise();

        /**
         * @brief 
         *
         * @return command line switch string to enable this checker 
         */
        virtual std::string const& cmdOption() const = 0;

        /**
         * @brief 
         *
         * @return name string 
         */
        virtual std::string const& name() const = 0;

        /**
         * @brief 
         *
         * @return Regular expression for matching filenames 
         */
        virtual std::string const& reString() const = 0;

        /**
         * @brief Check if path of file matches the ones specified for checker
         *
         * @param entry Manifest::Entry to match
         *
         * @return 
         */
        bool Match(Manifest::Entry const& entry) const;

        /**
         * @brief Do an actuall check on entry's data
         *
         * @param entry Manifest::Entry descriptor
         * @param data entry's data buffer
         *
         * @return 0 if data is correct, non-zero otherwise
         */
        virtual int Check(Manifest::Entry const& entry, std::vector<char> const& data) const = 0;

        /**
         * @brief Clone/Prototype pattern, will be used for parallelisation so that
         * each thread has his own copy of IChecker (eg to recreate OGL context for
         * each thread)
         *
         * @return newly created class' object
         */
        virtual IChecker* clone() const = 0;

    private:
        xmlRegexpPtr comp;
};
#endif
