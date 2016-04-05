local root      = "../../"

newoption {
   trigger     = "openmp",
   description = "Enable openmp in the bechmark."
}

function ConfigureBase( solutionDir )
    location( root .. solutionDir )
    objdir( root .. "bin/obj/" )
	debugdir( root .. "bin/" )
    
    configurations { "Debug", "Release", "Coverage" }

    platforms { "x64", "x32" }

    vectorextensions "SSE2"

    warnings "Extra"

    flags "Unicode" 

    configuration "x32"
        targetdir( root .. "bin/x32/" )
        architecture "x32"

    configuration "x64"
        targetdir( root .. "bin/x64/" )
        architecture "x64"
end

function ConfigureDebug()
    configuration "Debug"
        targetsuffix "d"
        defines "DEBUG"
        flags "Symbols"
        optimize "Off"
end

function ConfigureRelease()
    configuration "Release"     
        flags { "LinkTimeOptimization", "Symbols", "FloatStrict" }
        optimize "Full"
end

function ConfigureGmake()
    configuration "gmake"
        linkoptions {
            "-Wl,--no-as-needed",
            "-pthread"
            }
        if _OPTIONS["openmp"] then
            buildoptions {
                "-std=c++11",
                "-pthread",
                "-ggdb",
                "-g",
                "-fopenmp-simd"
                }
        else
            buildoptions {
                "-std=c++11",
                "-pthread",
                "-ggdb",
                "-g"
                }
        end 
end

function ConfigureVS()
    if _OPTIONS["openmp"] then
        configuration "vs*"
            buildoptions {
                "/openmp"
            }
    end
end

function ConfigureCoverage()
    configuration "Coverage"
        targetsuffix "cd"
        flags "Symbols"
        links "gcov"
        buildoptions "-coverage -fno-inline"
end

function ConfigureSolution( solutionDir )
    ConfigureBase( solutionDir )
        
    ConfigureDebug()

    ConfigureRelease()
			
    ConfigureGmake()
        
    ConfigureCoverage()
end

solution "bsp-library"

    ConfigureSolution( "bsp/" )
            
    project "bsp"
        kind "StaticLib"

        includedirs {
            root .. "bsp/include/",
            }   
            
        files { 
            root .. "bsp/include/**.h",
            root .. "bsp/include/**.cpp",
            }
            
    project "bsp-test"
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
        
        configuration { "Coverage", "x32" }
            defines "PREFIX=X32C_"
        
        configuration { "Coverage", "x64" }
            defines "PREFIX=X64C_"
          
solution "bsp-edupack"

    ConfigureSolution( "edupack" )
            
    project "bench"                
        kind "ConsoleApp"
        flags "WinMain"

        includedirs {
            root .. "bsp/include/",
            root .. "edupack/",
            root .. "extern/plotting-library/plot/include/",
            root .. "extern/plotting-library/extern/armadillo/include/"
            }   
            
        files { 
            root .. "edupack/bspedupack.h",
            root .. "edupack/config.h",
            root .. "edupack/bspedupack.cpp",
            root .. "edupack/bspbench.cpp",
            }
            
    project "fft"                
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