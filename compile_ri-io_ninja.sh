#!/bin/bash
#compile_ri-io_ninja.sh
PROJ_DIR=/home/bellout/git/IOC/ResInsight-io
cd ${PROJ_DIR}/ninja-build-debug

ninja -C ${PROJ_DIR}/ninja-build-debug -j 6 ResInsight
# cmake --build /home/bellout/git/IOC/ResInsight-io/cmake-build-debug --target ResInsight -- -j 6