#pragma once
#ifndef __BSPABORT_H__
#define __BSPABORT_H__

#include <atomic>
#include <string>
#include <stdint.h>
#include <exception>

namespace BspInternal
{
    class BspAbort
        : public std::exception
    {
    public:

        BspAbort( std::string m )
            : msg( m )
        {
        }

        ~BspAbort() throw()
        {
        }

        const char *what() const throw()
        {
            return msg.c_str();
        }

    private:
        std::string msg;
    };
}

#endif