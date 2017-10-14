

project "BSPLib"
    kind "StaticLib"
        
    if zpm.setting( "profiler" ) and zpm.uses "Zefiros-Software/PlotLib" then
        defines "PLOTLIB_USE_ZPM_ANACONDA"
    end

    zpm.export [[
        includedirs "bsp/include/"
        cppdialect "C++11"

        filter "system:not windows"
            links "pthread"
    ]]
