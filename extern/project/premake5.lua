local root      = "../../"

solution "plot-library"

	location( root .. "plot/" )
	objdir( root .. "bin/obj/" )
	debugdir( root .. "bin/" )
	
	configurations { "Debug", "Release", "Coverage"  }

	platforms { "x64" }

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"
	
    configuration "x64"
		targetdir( root .. "bin/x64/" )
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
        
    configuration "Coverage"
        targetsuffix "cd"
        flags "Symbols"
        links "gcov"
        buildoptions "-coverage -fno-inline"
				
	configuration {}
			
	project "plot-test"
		location(  root .. "test/" )
				
		kind "ConsoleApp"
		flags "WinMain"
		defines "GTEST_HAS_TR1_TUPLE=0"
		
		includedirs {
			root .. "extern/gtest/include/",
			root .. "extern/gtest/",
			
			root .. "extern/armadillo/include/",
			root .. "plot/include/",
			root .. "test/"
			}	
		
		files { 
			root .. "extern/gtest/src/gtest-all.cc",
			root .. "test/**.h",
			root .. "test/**.cpp"
			}
			
		configuration "gmake"
			links "pthread"
			buildoptions "-std=c++11"
		
		configuration { "Debug", "x64" }
			defines "PREFIX=X64D_"
		
		configuration { "Release", "x64" }
			defines "PREFIX=X64R_"
        
        configuration { "Coverage", "x32" }
            defines "PREFIX=X32C_"
        
        configuration { "Coverage", "x64" }
            defines "PREFIX=X64C_"
			
	project "plot"
		targetname "plot"	 
		kind "StaticLib"
                
		includedirs {
			root .. "plot/include/"
			}				
		
		files { 
			root .. "plot/include/**.h"
			}
            
		configuration "gmake"
			linkoptions {
				"-Wl,--no-as-needed",
				"-pthread"
				}