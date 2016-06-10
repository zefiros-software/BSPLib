


workspace "BSPLib"

    configurations { "Debug", "Release", "Coverage" }
    platforms { "x86_64", "x86" }
    flags { "Unicode", "Symbols", "C++11" }
    
    startproject "bsp-test"
    location "bsp/"
    objdir "bin/obj"
    
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
        
    filter "Coverage"
        targetsuffix "cd"
        links "gcov"
        buildoptions "-coverage"
        
    zpm.buildLibraries()
    
    filter {}
    
    project "bsp-test"
        
        kind "ConsoleApp"
        flags "WinMain"
        
        location "test/"
        
        zpm.uses "Zefiros-Software/GoogleTest"
        
        includedirs {
            "bsp/include/",
            "test/"
        }
        
        files {
            "test/**.h",
            "test/**.cpp"
        }
        
        filter "action:gmake"
            links "pthread"
            
        filter { "*Debug", "platforms:x86" }
            defines "PREFIX=X86D_"
        
        filter { "*Debug", "platforms:x86_64" }
            defines "PREFIX=X86_64D_"
        
        filter { "*Release", "platforms:x86" }
            defines "PREFIX=X86R_"
        
        filter { "*Release", "platforms:x86_64" }
            defines "PREFIX=X86_64R_"
    
        filter {}
        
    project "bsp-library"
    
        kind "StaticLib"
        
        includedirs {
            "bsp/include/"
        }
        
        files {
            "bsp/include/**.h",
        }
        
        
workspace "BSPEdupack"

    configurations { "Debug", "Release" }
    platforms { "x86_64", "x86" }
    flags { "Unicode", "Symbols", "C++11" }
    
    startproject "bench-modern"
    location "edupack/"
    objdir "bin/obj"
    
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