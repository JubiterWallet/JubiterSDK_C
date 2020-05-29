cmake_minimum_required(VERSION 3.0.0)
project(JubSDK VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE "Debug")

if(devOnly)
else()  #if(devOnly)
add_subdirectory(trezor-crypto)
endif() #if(devOnly)

add_subdirectory(src)

if(devOnly)
else()  #if(devOnly)
if(NOT ANDROID AND NOT IOS)
add_subdirectory(test)
endif() #if(NOT ANDROID AND NOT IOS)
endif() #if(devOnly)
#add_subdirectory(monitor)