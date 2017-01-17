#!/bin/bash
set -e

if [ "$TYPE" == "zpm" ]; then
    cd test
    
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install

    cd zpm/
    make
    cd ../../

    test/bin/x86/math-zpm-test

else
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install
    cd math
    make config=${TYPE}_${ARCH}
    cd ../


    if [ "$TYPE" == "debug" ]; then
        bin/${ARCH}/math-testd

    elif [ "$TYPE" == "coverage" ]; then
        ./math-testcd
    else
        bin/${ARCH}/math-test
    fi
fi