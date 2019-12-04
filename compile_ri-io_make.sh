#!/bin/bash
#compile_ri-io.sh
PROJ_DIR=/home/bellout/git/IOC/ResInsight-io
cd ${PROJ_DIR}/cmake-build-debug

cmake --build ${PROJ_DIR}/cmake-build-debug --target ResInsight -- -j 6