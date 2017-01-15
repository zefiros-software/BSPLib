/**
 * Copyright (c) 2017 Zefiros Software.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#ifndef __BSPLIB_BSPABORT_H__
#define __BSPLIB_BSPABORT_H__

#include <exception>
#include <string>

namespace BSPInternal
{
    /**
     * A bsp abort exception. By inherriting from std::exception we can throw
     * this exception to abort all threads from continuing easily.
     *
     * @sa std::exception
     */

    class BspAbort
        : public std::exception
    {
    public:

        /**
         * Constructor.
         *
         * @param   m The error message.
         */

        explicit BspAbort( std::string m )
            : msg( m )
        {
        }

        ~BspAbort() throw()
        {
        }

        virtual const char *what() const throw() override
        {
            return msg.c_str();
        }

    private:

        std::string msg;
    };
};

#endif