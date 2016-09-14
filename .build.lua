

project "BSPLib"
    

    filter "platforms:Windows"
        kind "Utility"

    filter "platforms:Unix"
        kind "StaticLib"

    filter "platforms:Mac"
        kind "StaticLib"

    filter {}

    if zpm.option( "Profiler" ) then
        defines "PLOTLIB_USE_ZPM_ANACONDA"

        zpm.uses "Zefiros-Software/PlotLib"

        if zpm.option( "OwnArmadillo" ) then

            zpm.uses "Zefiros-Software/Armadillo"

        end
    end

    zpm.export [[
        includedirs "bsp/include/"
        flags "C++11"
    ]]
