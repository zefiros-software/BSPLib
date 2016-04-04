#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__

//#define ARMA_USE_MKL_ALLOC
#define ARMA_USE_CXX11

#pragma warning(push)
#pragma warning(disable : 4702)
#include <armadillo>
#pragma warning(pop)

#define PLOTLIB_ARMA

#include "plot/plotting.h"

using namespace arma;

#endif