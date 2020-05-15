cmake_minimum_required(VERSION 3.0.0)
project(JubSDK VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE "Debug")

add_subdirectory(trezor-crypto)
add_subdirectory(src)
if(NOT ANDROID AND NOT IOS)
add_subdirectory(test)
endif()
#add_subdirectory(monitor)