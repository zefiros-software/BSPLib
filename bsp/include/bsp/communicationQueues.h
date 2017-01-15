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
#ifndef __BSPLIB_COMMUNICATIONQUEUES_H__
#define __BSPLIB_COMMUNICATIONQUEUES_H__

#include <vector>


namespace BSPInternal
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

        explicit CommunicationQueues( std::size_t nProcs )
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

        void ResetResize( std::size_t maxProcs )
        {
            //std::size_t maxProcsSqr = maxProcs * maxProcs;
            mQueues.clear();
            mQueues.resize( maxProcs );

            for ( auto &queue : mQueues )
            {
                queue.resize( maxProcs );
            }

            mProcCount = maxProcs;
        }

        /**
         * Gets the queues of the source processor communicating to the current processor.
         *
         * @param   source Source to get the queues from.
         * @param   me     Processors to get the queues to.
         *
         * @return The queue.
         */

        inline tQueue &GetQueueToMe( std::size_t source, std::size_t me )
        {
            return GetQueue( source, me );
        }

        /**
         * Gets the queues of the current processor communicating to the source processor.
         *
         * @param   target The processor to get the queues for.
         * @param   me     The processor to get the queues from.
         *
         * @return The queue.
         */

        inline tQueue &GetQueueFromMe( std::size_t target, std::size_t me )
        {
            return GetQueue( me, target );
        }

    private:

        std::vector< std::vector< tQueue > > mQueues;

        /// The amount of processors that may use the queue
        std::size_t mProcCount;

        tQueue &GetQueue( std::size_t owner, std::size_t target )
        {
            return mQueues[owner][target];
        }
    };
}

#endif