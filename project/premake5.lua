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
                
    configuration {}
            
    project "bsplib"
        location( root )
                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root
            }   
            
        files { 
            root .. "lib/**.h",
            root .. "lib/**.cpp",
            }
            
    project "bench"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root .. "lib/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bspbench.cpp",
            }