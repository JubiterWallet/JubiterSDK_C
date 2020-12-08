cmake_minimum_required(VERSION 3.0.0)
project(JubSDK VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/./outputs/${CMAKE_SYSTEM_NAME})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/./outputs/${CMAKE_SYSTEM_NAME})
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/./install)

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