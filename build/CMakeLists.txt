cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(NeuralChess)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(
  NeuralChess
  ../src/chess.cc
  ../src/game.cc
  ../src/consoleLogger.cc
  ../src/textureManager.cc
  ../src/gameManager.cc
  ../src/pieces.cc
  ../src/bitboard.cc
  ../src/movelookup.cc
  ../src/negamax.cc
)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(NeuralChess ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})