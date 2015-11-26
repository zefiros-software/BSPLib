/**
 * Copyright (c) 2015 Mick van Duijn, Koen Visscher and Paul Visscher
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
#ifndef __COMMUNICATIONQUEUES_H__
#define __COMMUNICATIONQUEUES_H__

#include <vector>

namespace BspInternal
{
    /**
     * A communication queue implementation. Allows easier
     * implementation of communication queues of various types.
     *
     * @tparam  tQueue Type of the queue.
     */

    template< typename tQueue >
    class CommunicationQueues
    {
    public:

        CommunicationQueues()
            : mProcCount( 0 )
        {
        }

        /**
         * Constructor.
         *
         * @param   nProcs The amount of processors that can use the queue.
         */

        explicit CommunicationQueues( size_t nProcs )
            : mQueues( 0 ),
              mProcCount( nProcs )
        {
            ResetResize( nProcs );
        }

        /**
         * Resets the queues, and set the maximum amount of threads that may
         * use the communication queue.
         *
         * @param   maxProcs The maximum number of processors.
         */

        void ResetResize( size_t maxProcs )
        {
            size_t maxProcsSqr = maxProcs * maxProcs;
            mQueues.clear();
            mQueues.resize( maxProcsSqr );

            mProcCount = maxProcs;
        }

        /**
         * Gets queue to me.
         *
         * @param   source Source for the.
         * @param   me     me.
         *
         * @return The queue to me.
         */

        tQueue &GetQueueToMe( size_t source, size_t me )
        {
            return GetQueue( source, me );
        }

        /**
         * Gets queue from me.
         *
         * @param   target Target for the.
         * @param   me     me.
         *
         * @return The queue from me.
         */

        tQueue &GetQueueFromMe( size_t target, size_t me )
        {
            return GetQueue( me, target );
        }

    private:

        /// an obimination we have to live with,
        /// a flattened std::vector< std::vector <> > but without
        /// inneficient cache thrashing.
        std::vector< tQueue > mQueues;

        /// The amount of processors that may use the queue
        size_t mProcCount;

        tQueue &GetQueue( size_t owner, size_t target )
        {
            return mQueues[owner * mProcCount + target];
        }
    };
}

#endif