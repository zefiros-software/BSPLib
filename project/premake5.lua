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
            root .. "bsp/include/",
            }   
            
        files { 
            root .. "bsp/include/**.h",
            root .. "bsp/include/**.cpp",
            }
            
    project "bsptest"
        location(  root .. "test/" )
                
        kind "ConsoleApp"
        flags "WinMain"
        defines "GTEST_HAS_TR1_TUPLE=0"
        
        includedirs {
            root .. "extern/gtest/include/",
            root .. "extern/gtest/",
            
            root .. "bsp/include/",
            root .. "test/"
            }   
        
        files { 
            root .. "extern/gtest/src/gtest-all.cc",
            root .. "test/**.h",
            root .. "test/*.cpp"
            }            
			
		configuration "gmake"
			links "pthread"
            
        configuration { "Debug", "x32" }
            defines "PREFIX=X32D_"
        
        configuration { "Debug", "x64" }
            defines "PREFIX=X64D_"
        
        configuration { "Release", "x32" }
            defines "PREFIX=X32R_"
        
        configuration { "Release", "x64" }
            defines "PREFIX=X64R_"
            
    project "bench"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root .. "bsp/include/",
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
            root .. "bsp/include/",
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
            root .. "bsp/include/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bsplu.cpp",
            root .. "edupack/bsplu_test.cpp",
            }
            
    project "ip"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root .. "bsp/include/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bspinprod.cpp",
            }
            
    project "matvec"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root .. "bsp/include/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bspmv.cpp",
            root .. "edupack/bspmv_test.cpp",
            }
            
    project "fft"
        location(  root )
                
        kind "ConsoleApp"
        flags "WinMain"
        
        defines "M_PI=3.14159265358979323846"

        includedirs {
            root .. "bsp/include/",
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
            root .. "bsp/include/",
            root .. "edupack/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bsplu.cpp",
            root .. "edupack/bsplu_test.cpp",
            }