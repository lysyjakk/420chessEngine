#!/bin/bash

ROOT_DIR=$(git rev-parse --show-toplevel)
BUILD_DIR=${ROOT_DIR}/build

#CHANGE ACTIVE DIR
cd ${BUILD_DIR}

#BUILD
cmake ./
make

${BUILD_DIR}/NeuralChess $1
