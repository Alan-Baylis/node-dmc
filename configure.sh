#!/bin/bash

echo "===> BUILDING EIGEN 3"
mkdir eigen
cd eigen
echo "==> Getting Eigen from git"
git clone https://github.com/eigenteam/eigen-git-mirror.git src
echo "==> Building Eigen from source"
mkdir bin
mkdir build
cd build
cmake ../src
cmake -DCMAKE_INSTALL_PREFIX=../bin .
make install

echo "===> BUILDING DMC LIB"
cd ../../
cmake-js build --CDEigen3_DIR="eigen/bin/share/eigen3/cmake"