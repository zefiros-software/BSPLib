premake5 install-package --allow-install --allow-module  || exit /b 1
premake5 vs2015 --ignore-updates || exit /b 1
msbuild bsp/BSPLib.sln /property:Configuration=Release /property:Platform=Win32 || exit /b 1
::msbuild bsp/BSPLib.sln /property:Configuration=Debug /property:Platform=Win32 || exit /b 1
msbuild bsp/BSPLib.sln /property:Configuration=Release /property:Platform=x64 || exit /b 1
::msbuild bsp/BSPLib.sln /property:Configuration=Debug /property:Platform=x64 || exit /b 1

cd test/
premake5 vs2015 || exit /b 1
msbuild zpm/BSPLib-ZPM.sln || exit /b 1

cd ../

bin\x86\bsp-test.exe || exit /b 1
::bin\x86\bsp-testd.exe || exit /b 1

bin\x86_64\bsp-test.exe || exit /b 1
::bin\x86_64\bsp-testd.exe || exit /b 1

test\bin\x86\bsp-zpm-test.exe || exit /b 1