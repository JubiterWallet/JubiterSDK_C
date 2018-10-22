cmake_minimum_required(VERSION 3.0.0)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/utility SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/context SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/device SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/libETH SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/token SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/deps/formats SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/deps/polarssl SRC_LIST)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/deps)


ADD_LIBRARY(JUB_SDK STATIC ${SRC_LIST})

message("==========================")
message("${SRC_LIST}")