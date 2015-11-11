#pragma once
#ifndef __REQUESTS_H__
#define __REQUESTS_H__

#include "stackAllocator.h"

namespace BspInternal
{
    struct RegisterInfo
    {
        size_t size;
        size_t registerCount;
    };

    struct PutRequest
    {
        StackAllocator::StackLocation bufferLocation;
        const void *destination;
        size_t size;
    };

    struct GetRequest
    {
        const void *destination;
        const void *source;
        size_t size;
    };

    struct SendRequest
    {
        StackAllocator::StackLocation bufferLocation;
        size_t bufferSize;

        StackAllocator::StackLocation tagLocation;
        size_t tagSize;
    };

    struct PushRequest
    {
        const void *pushRegister;
        RegisterInfo registerInfo;
    };

    struct PopRequest
    {
        const void *popRegister;
    };

}

#endif