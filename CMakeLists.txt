cmake_minimum_required(VERSION 3.0.0)
project(JubSDK VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE "Debug")

add_subdirectory(src)
add_subdirectory(trezor-crypto)
if(NOT ANDROID)
add_subdirectory(test)
endif()
#add_subdirectory(monitor)


