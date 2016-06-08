#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma warning(push)
#pragma warning(disable : 4702)
#define ARMA_DONT_USE_BLAS
#include <armadillo>
#pragma warning(pop)

#define PLOTLIB_ARMA

#include "plot/plotting.h"

using namespace arma;

#endif