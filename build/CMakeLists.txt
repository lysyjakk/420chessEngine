cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(NeuralChess)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

#find_file(SDL2_INCLUDE_DIRS NAME SDL.h HINTS SDL2)
#find_library(SDL2_LIBRARIES NAME SDL2)

add_executable(
  NeuralChess
  ../src/chess.cc
  ../src/game.cc
  ../src/consoleLogger.cc
  ../src/textureManager.cc
)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

#target_include_directories(NeuralChess ${SDL2_INCLUDE_DIRS})
target_link_libraries(NeuralChess ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})