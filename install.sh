#!/usr/bin/env bash

git submodule update --init --recursive

# Copy public include files to include folder
sudo cp -r kktest_lib/include/* /usr/local/include

# Build the library
./compile_prod.sh
sudo cp build/lib/libkktest.a /usr/lib/libkktest.a
sudo cp build/lib/libautojson.a /usr/lib/libautojson.a
sudo cp build/lib/libeasyflags.a /usr/lib/libeasyflags.a

# Copy the cmake library
sudo cp kktest_lib/kktest.cmake /usr/local/include/kktest.cmake

# Install and init box
cd third_party/sandman
sudo apt install -y acl
make build
sudo mv box /usr/local/bin/box
sudo mkdir -p /eval
sudo touch /eval/isolate.log
sudo box --init
