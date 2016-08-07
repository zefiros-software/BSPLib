premake5 install-package --allow-install --allow-module  || exit /b
premake5 vs2015 || exit /b
msbuild bsp/BSPLib.sln /property:Configuration=Release /property:Platform=Win32 || exit /b
msbuild bsp/BSPLib.sln /property:Configuration=Debug /property:Platform=Win32 || exit /b
msbuild bsp/BSPLib.sln /property:Configuration=Release /property:Platform=x64 || exit /b
msbuild bsp/BSPLib.sln /property:Configuration=Debug /property:Platform=x64 || exit /b

cd test/
premake5 vs2015 || exit /b
msbuild zpm/BSPLib-ZPM.sln || exit /b

cd ../

bin\x86\bsp-test.exe || exit /b
bin\x86\bsp-testd.exe || exit /b

bin\x86_64\bsp-test.exe || exit /b
bin\x86_64\bsp-testd.exe || exit /b

test\bin\x86\bsp-zpm-test.exe || exit /b