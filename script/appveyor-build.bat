
if "%TYPE%" == "zpm" (    
    cd test
    
    zpm install-package --allow-install --allow-module || exit /b 1
    zpm vs2015 --allow-install || exit /b 1

    msbuild zpm/MathLib-ZPM.sln || exit /b 1

    test\bin\x86\math-zpm-test.exe || exit /b 1
) else (
    zpm install-package --allow-install --allow-module || exit /b 1
    zpm %VSTUD% --allow-install || exit /b 1

    msbuild math/MathLib.sln /property:Configuration=Release /property:Platform=%PLAT% || exit /b 1

    if %TYPE% == "debug" (
        bin\%ARCH%\math-testd
    ) else (
        bin\%ARCH%\math-test
    )
)