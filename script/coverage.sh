set -e

cd bsp
make config=coverage_x86
cd ../

./bsp-testcd

codecov
