cmake_minimum_required(VERSION 3.15)
project(2DRayMarchingRenderer)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
aux_source_directory(${CMAKE_SOURCE_DIR} root)

add_executable(2DRayMarchingRenderer ${root})
target_link_libraries(2DRayMarchingRenderer tbb)