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
#ifndef __BSPLIB_PROCESSORDATA_H__
#define __BSPLIB_PROCESSORDATA_H__

#include "bsp/registerMapType.h"
#include "bsp/stackAllocator.h"
#include "bsp/requestVector.h"
#include "bsp/barrierType.h"

struct ProcessorData
{
    ProcessorData()
        : sendReceivedIndex( 0 ),
          registerCount( 0 ),
          newTagSize( 0 ),
          sendRequestsSize( 0 ),
          pushRequestsSize( 0 ),
          popRequestsSize( 0 ),
          putBufferStack( 9064 ),
          getBufferStack( 9064 ),
          sendBufferStack( 9064 )
    {
        sendRequests.Reserve( 9064 );
        pushRequests.Reserve( 9064 );
        popRequests.Reserve( 9064 );

    }

    uint32_t sendReceivedIndex;
    uint32_t registerCount;
    uint32_t newTagSize;
    uint32_t sendRequestsSize;
    uint32_t pushRequestsSize;
    uint32_t popRequestsSize;
    BSPInternal::StackAllocator putBufferStack;
    BSPInternal::StackAllocator getBufferStack;
    BSPInternal::StackAllocator sendBufferStack;
    std::vector<BSPInternal::StackAllocator> tmpSendBufferStacks;
    BSPUtil::TicTimer startTimer;
    BSPUtil::TicTimer ticTimer;
    std::vector<BSPInternal::RequestVector< BSPInternal::PutRequest >> putRequests;
    std::vector<BSPInternal::RequestVector< BSPInternal::GetRequest >> getRequests;
    std::vector<BSPInternal::RequestVector< BSPInternal::BufferedGetRequest >> bufferedGetRequests;
    std::vector<BSPInternal::RequestVector< BSPInternal::SendRequest >> tmpSendRequests;
    BSPInternal::RequestVector< BSPInternal::SendRequest > sendRequests;
    BSPInternal::RequestVector< BSPInternal::PushRequest > pushRequests;
    BSPInternal::RequestVector< BSPInternal::PopRequest > popRequests;
    tRegisterMap threadRegisters;

    struct
    {
        bool hasPutRequests = false;
        bool hasGetRequests = false;
        bool hasPushRequests = false;
        bool hasPopRequests = false;
        bool hasSendRequests = false;
        bool hasTagSizeUpdate = false;
    } syncBools;
};

#endif