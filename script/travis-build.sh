set -e

premake5 install-package --allow-install --allow-module
premake5 gmake
cd bsp
make config=debug_x86
make config=debug_x86_64
make config=release_x86
make config=release_x86_64
#make config=coverage_x86
#make config=coverage_x86_64

cd ../test/

premake5 gmake

cd zpm/
make
cd ../

bin/x86/bsp-test
bin/x86/bsp-testd
#bin/x86/bsp-testcd

bin/x86_64/bsp-test
bin/x86_64/bsp-testd
#bin/x86_64/bsp-testcd

test/bin/x86/bsp-zpm-test