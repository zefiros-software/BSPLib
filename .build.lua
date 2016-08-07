

project "BSPLib"
  
    kind "Utility"

    if zpm.option( "Profiler" ) then
        defines "PLOTLIB_USE_ZPM_ANACONDA"

        zpm.uses "Zefiros-Software/PlotLib"

        if zpm.option( "OwnArmadillo" ) then

            zpm.uses "Zefiros-Software/ArmadilloExt"

        end
    end

    zpm.export [[
        includedirs "bsp/include/"
        flags "C++11"
    ]]