
--[[ @cond ___LICENSE___
-- Copyright (c) 2017 Zefiros Software.
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.
--
-- @endcond
--]]

workspace "BSPEdupack"

    configurations { "Debug", "Release" }
    platforms { "x86_64", "x86" }

    flags {
        "WinMain",
        "C++11"
    }
    
    startproject "bench-modern"
    location "./"
    objdir "bin/obj/"
        
    defines "M_PI=3.14159265358979323846"
    
    vectorextensions "SSE2"
    warnings "Extra"
    
    filter "platforms:x86"
        targetdir "bin/x86/"
        debugdir "bin/x86/"
        architecture "x86"
        
    filter "platforms:x86_64"
        targetdir "bin/x86_64/"
        debugdir "bin/x86_64/"
        architecture "x86_64"
        
    filter "*Debug"
        targetsuffix "d"
        symbols "On"
        defines "DEBUG"
        optimize "Off"
        
    filter "*Release"
        flags "LinkTimeOptimization"
        optimize "Speed"
    
    filter {}
    
    project "bench"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "config.h",
            "bspedupack.cpp",
            "bspbench.cpp",
            }
            
    project "bench-modern"                
        kind "ConsoleApp"
                
        zpm.uses "Zefiros-Software/BSPLib"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "config.h",
            "bspedupack.cpp",
            "bspBenchModern.cpp",
            "bench.h"
            }
            
    project "fft"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "bspedupack.cpp",
            "bspfft.cpp",
            "bspfft_test.cpp",
            }
            
    project "fft-modern"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"
        defines "BSP_USE_PROFILER"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "bspedupack.cpp",
            "modernFft.h",
            "modernFftTest.cpp",
            }
            
    project "lu"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"
        
        defines "M_PI=3.14159265358979323846"
        defines "BSP_USE_PROFILER"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "bspedupack.cpp",
            "bsplu.cpp",
            "bsplu_test.cpp",
            }
            
    project "ip"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "bspedupack.cpp",
            "bspinprod.cpp",
            }
            
    project "matvec"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "bspedupack.cpp",
            "bspmv.cpp",
            "bspmv_test.cpp",
            }
            
    project "fft"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "bspedupack.cpp",
            "bspfft.cpp",
            "bspfft_test.cpp",
            }
            
    project "lu"                
        kind "ConsoleApp"
             
        zpm.uses "Zefiros-Software/BSPLib"

        includedirs "./"
            
        files { 
            "bspedupack.h",
            "bspedupack.cpp",
            "bsplu.cpp",
            "bsplu_test.cpp",
            }