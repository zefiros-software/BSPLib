

project "BSPLib"
    kind "StaticLib"
        
    if zpm.setting( "profiler" ) then
        defines "PLOTLIB_USE_ZPM_ANACONDA"

        zpm.uses "Zefiros-Software/PlotLib"
    end

    zpm.export [[
        includedirs "bsp/include/"
        cppdialect "C++11"

        filter "system:not windows"
            links "pthread"
    ]]
