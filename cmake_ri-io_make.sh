#!/bin/bash
#cmake_ri-io.sh
PROJ_DIR=/home/bellout/git/IOC/ResInsight-io
cd ${PROJ_DIR}/cmake-build-debug

cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ${PROJ_DIR}