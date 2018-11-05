#!/usr/bin/env bash

# Uninstall previous instalations
./uninstall.sh

git submodule update --init --recursive

# Copy public include files to include folder
sudo cp -r kktest_lib/include/* /usr/local/include

# Build the library
cmake -H. -Bbuild/cmake_prod -DCMAKE_BUILD_TYPE=Release -DBUILD_TYPE:STRING=PROD
cd build/cmake_prod && make -j4 && cd ../..
sudo cp build/lib/libkktest.so   /usr/local/lib/libkktest.so
sudo cp build/lib/libkktest.so   /usr/lib/libkktest.so
sudo cp build/lib/libkktest.so.1 /usr/local/lib/libkktest.so.1
sudo cp build/lib/libkktest.so.1 /usr/lib/libkktest.so.1

# Copy the cmake library
sudo cp kktest_lib/kktest.cmake /usr/local/include/kktest.cmake

# Install and init box
if ! [ -x "$(command -v box)" ]; then
    sudo apt install -y acl
    cd third_party/sandman && make build && cd ../..
    sudo mv third_party/sandman/box /usr/local/bin/box
    sudo mkdir -p /eval
    sudo touch /eval/isolate.log
fi
sudo box --init

# Copy the kktest_runner binary to a binary path
sudo cp build/bin/kktest_runner /usr/local/bin/kktest
