cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(NeuralChess)

add_executable(
  NeuralChess
  ../src/chess.cc
  ../src/game.cc
  ../src/consoleLogger.cc
)
