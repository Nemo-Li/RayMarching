cmake_minimum_required(VERSION 3.15)
project(2DRayMarchingRenderer)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_SOURCE_DIR} root)

add_executable(2DRayMarchingRenderer ${root})