#!/bin/bash
set -e

if [ "$TYPE" == "zpm" ]; then
    cd test
    
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install

    cd zpm/
    make
    cd ../../

    test/bin/x86/bsp-zpm-test

else
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install
    cd bsp
    make config=${TYPE}_${ARCH}
    cd ../


    if [ "$TYPE" == "debug" ]; then
        bin/${ARCH}/bsp-testd

    elif [ "$TYPE" == "coverage" ]; then
        ./bsp-testcd
    else
        bin/${ARCH}/bsp-test
    fi
fi