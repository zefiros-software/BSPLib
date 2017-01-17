if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then 
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo apt-get update -y

    sudo apt-get install gcc-${VERSION} g++-${VERSION} -y
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${VERSION} 60
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${VERSION} 60
    sudo update-alternatives --config gcc
    sudo update-alternatives --config g++
    sudo apt-get install gcc-${VERSION}-multilib g++-${VERSION}-multilib

    # for coverage
    sudo pip install codecov
fi