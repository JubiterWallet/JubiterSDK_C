project(Jub_SDK)

cmake_minimum_required(VERSION 3.6.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_library(
            Jub_SDK
            STATIC
            ${SRC_LIST}
)