cmake_minimum_required(VERSION 3.0.0)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/utility SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/context SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/device SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/libETH SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/libBCH SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/token SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/deps/formats SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/deps/polarssl SRC_LIST)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/deps)


ADD_LIBRARY(JUB_SDK STATIC ${SRC_LIST})

# test endian.h
find_path(ENDIAN_PATH endian.h)
find_path(SYS_ENDIAN_PATH sys/endian.h)

if(ENDIAN_PATH)
    message("ENDIAN_PATH = ${ENDIAN_PATH}")
    target_compile_definitions(JUB_SDK PRIVATE HAVE_ENDIAN_H=1)
elseif(SYS_ENDIAN_PATH)
    message("SYS_ENDIAN_PATH = ${SYS_ENDIAN_PATH}")
    target_compile_definitions(JUB_SDK PRIVATE HAVE_SYS_ENDIAN_H=1)
endif()

