    
workspace "BSPEdupack"

    configurations { "Debug", "Release" }
    platforms { "x86_64", "x86" }
    flags { "Unicode", "Symbols", "C++11" }
    
    startproject "bench-modern"
    location "edupack/"
    objdir "bin/obj/"
    
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
        defines "DEBUG"
        optimize "Off"
        
    filter "*Release"
        flags "LinkTimeOptimization"
        optimize "Speed"
        
    zpm.buildLibraries()
    
    filter {}
    
    project "bench"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }

        includedirs {
            "bsp/include/",
            "edupack/"
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/config.h",
            "edupack/bspedupack.cpp",
            "edupack/bspbench.cpp",
            }
            
    project "bench-modern"                
        kind "ConsoleApp"
        flags "WinMain"
        
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }

        includedirs {
            "bsp/include/",
            "edupack/"
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/config.h",
            "edupack/bspedupack.cpp",
            "edupack/bspBenchModern.cpp",
            "edupack/bench.h"
            }
            
    project "fft"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }
        
        defines "M_PI=3.14159265358979323846"

        includedirs {
            "bsp/include/",
            "edupack/"
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/bspedupack.cpp",
            "edupack/bspfft.cpp",
            "edupack/bspfft_test.cpp",
            }
            
    project "fft-modern"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }
        
        defines "M_PI=3.14159265358979323846"
        defines "BSP_USE_PROFILER"

        includedirs {
            "bsp/include/",
            "edupack/",
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/bspedupack.cpp",
            "edupack/modernFft.h",
            "edupack/modernFftTest.cpp",
            }
            
    project "lu"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }
        
        defines "M_PI=3.14159265358979323846"
        defines "BSP_USE_PROFILER"

        includedirs {
            "bsp/include/",
            "edupack/",
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/bspedupack.cpp",
            "edupack/bsplu.cpp",
            "edupack/bsplu_test.cpp",
            }
            
    project "ip"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }

        includedirs {
            "bsp/include/",
            "edupack/"
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/bspedupack.cpp",
            "edupack/bspinprod.cpp",
            }
            
    project "matvec"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }

        includedirs {
            "bsp/include/",
            "edupack/"
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/bspedupack.cpp",
            "edupack/bspmv.cpp",
            "edupack/bspmv_test.cpp",
            }
            
    project "fft"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }
        
        defines "M_PI=3.14159265358979323846"

        includedirs {
            "bsp/include/",
            "edupack/"
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/bspedupack.cpp",
            "edupack/bspfft.cpp",
            "edupack/bspfft_test.cpp",
            }
            
    project "lu"                
        kind "ConsoleApp"
        flags "WinMain"
     
        zpm.uses {
            "Zefiros-Software/ArmadilloExt",
            "Zefiros-Software/PlotLib"
        }
        
        defines "M_PI=3.14159265358979323846"

        includedirs {
            "bsp/include/",
            "edupack/"
            }   
            
        files { 
            "edupack/bspedupack.h",
            "edupack/bspedupack.cpp",
            "edupack/bsplu.cpp",
            "edupack/bsplu_test.cpp",
            }