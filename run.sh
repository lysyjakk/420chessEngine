#!/bin/bash

ROOT_DIR=$(git rev-parse --show-toplevel)
BUILD_DIR=${ROOT_DIR}/build
LOGS_DIR=${ROOT_DIR}/log

#COLORIZATE ECHO
BLACK_FONT=`tput setaf 0`
WHITE_BG=`tput setab 7`
RESET=`tput sgr0`

#CHANGE ACTIVE DIR
cd ${BUILD_DIR}

echo ""
echo "${BLACK_FONT} ${WHITE_BG}-----------------------------------------------------------${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}|                                                         |${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}|                          BUILD                          |${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}|                                                         |${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}-----------------------------------------------------------${RESET}"
echo ""

#BUILD
cmake ./
make #| tee ${LOGS_DIR}/buildLogs.log

sleep 0.5

echo ""
echo "${BLACK_FONT} ${WHITE_BG}-----------------------------------------------------------${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}|                                                         |${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}|                      RUN PROGRAM                        |${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}|                                                         |${RESET}"
echo "${BLACK_FONT} ${WHITE_BG}-----------------------------------------------------------${RESET}"
echo ""

#RUN PROGRAM
${BUILD_DIR}/NeuralChess $1 #2>&1 | tee ${LOGS_DIR}/logs.log
