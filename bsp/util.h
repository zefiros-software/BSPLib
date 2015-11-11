#pragma once
#ifndef __UTIL_H__
#define __UTIL_H__

// From boost
#if !defined(BSP_FORCEINLINE)
#  if defined(_MSC_VER)
#    define BSP_FORCEINLINE __forceinline
#  elif defined(__GNUC__) && __GNUC__ > 3
// Clang also defines __GNUC__ (as 4)
#    define BSP_FORCEINLINE inline __attribute__ ((__always_inline__))
#  else
#    define BSP_FORCEINLINE inline
#  endif
#endif

#endif