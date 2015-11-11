#pragma once
#ifndef __COMMUNICATIONQUEUES_H__
#define __COMMUNICATIONQUEUES_H__

namespace BspInternal
{
    template< typename tQueue >
    class CommunicationQueues
    {
    public:

        CommunicationQueues( size_t nProcs )
            : mQueues( 0 ),
              mProcCount( nProcs )
        {
            ResetResize( nProcs );
        }

        CommunicationQueues()
            : mProcCount( 0 )
        {

        }

        void ResetResize( size_t maxProcs )
        {
            mQueues.clear();
            mQueues.resize( maxProcs );

            for ( auto &queue : mQueues )
            {
                queue.resize( maxProcs );
            }

            mProcCount = maxProcs;
        }

        tQueue &GetQueueToMe( size_t source, size_t me )
        {
            return GetQueue( source, me );
        }

        tQueue &GetQueueFromMe( size_t target, size_t me )
        {
            return GetQueue( me, target );
        }

    private:

        std::vector< std::vector< tQueue > > mQueues;

        size_t mProcCount;

        tQueue &GetQueue( size_t owner, size_t target )
        {
            return mQueues[owner][target];
        }
    };
}

#endif