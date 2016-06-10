
workspace "BSPLib"

    configurations { "Debug", "Release", "Coverage" }
    platforms { "x86_64", "x86" }
    flags { "Unicode", "Symbols", "C++11" }
    
    startproject "bsp-test"
    location "bsp/"
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