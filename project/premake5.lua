local root      = "../"

solution "bsp-library"

    location( root )
    objdir( root .. "bin/obj/" )
    
    configurations { "Debug", "Release" }

    platforms { "x64", "x32" }

    vectorextensions "SSE2"

    warnings "Extra"

    flags "Unicode" 

    configuration "x32"
        targetdir( root .. "bin/x32/" )
        debugdir( root .. "bin/x32/" )
        architecture "x32"

    configuration "x64"
        targetdir( root .. "bin/x64/" )
        debugdir( root .. "bin/x64/" )
        architecture "x64"
        
    configuration "Debug"
        targetsuffix "d"
        defines "DEBUG"
        flags "Symbols"
        optimize "Off"

    configuration "Release"     
        flags "LinkTimeOptimization"
        optimize "Speed"
			
    configuration "gmake"
        linkoptions {
            "-Wl,--no-as-needed",
            "-pthread"
            }
            
        buildoptions {
            "-std=c++11",
            "-pthread"
            }   
                
    configuration {}
            
    project "bsplib"
        location( root )
                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root .. "bsp/",
            root
            }   
            
        files { 
            root .. "bsp/**.h",
            root .. "bsp/**.cpp",
            }
            
    project "bench"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root .. "bsp/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bspbench.cpp",
            }
            
    project "fft"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"
        
        defines "M_PI=3.14159265358979323846"

        includedirs {
            root .. "bsp/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bspfft.cpp",
            root .. "edupack/bspfft_test.cpp",
            }
            
    project "lu"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"
        
        defines "M_PI=3.14159265358979323846"

        includedirs {
            root .. "bsp/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bsplu.cpp",
            root .. "edupack/bsplu_test.cpp",
            }