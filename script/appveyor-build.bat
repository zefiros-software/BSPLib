
if "%TYPE%" == "zpm" (    
    cd test
    
    zpm install-package --allow-install --allow-module || exit /b 1
    zpm vs2015 --allow-install || exit /b 1

    msbuild zpm/BSPLib-ZPM.sln || exit /b 1

    test\bin\x86\bsp-zpm-test.exe || exit /b 1
) else (
    zpm install-package --allow-install --allow-module || exit /b 1
    zpm %VSTUD% --allow-install || exit /b 1

    msbuild bsp/BSPLib.sln /property:Configuration=Release /property:Platform=%PLAT% || exit /b 1

    if %TYPE% == "debug" (
        bin\%ARCH%\bsp-testd
    ) else (
        bin\%ARCH%\bsp-test
    )
)