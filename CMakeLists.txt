cmake_minimum_required(VERSION 3.0.0)
project(JubSDK VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE "Debug")

#将编译出的文件统一放在一处，方便后面查找目录
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/./outputs/${CMAKE_SYSTEM_NAME})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/./outputs/${CMAKE_SYSTEM_NAME})

if(APPLE)
#打成framework之后，即使在一个工程里面，链接时也会找不到，需要手动指定查找目录
#不知道为什么这句只有放在根CMakeLists.txt才会生效，放在子目录里无效
set(CMAKE_XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/./outputs)
#去掉RPATH的引用，以防编译完成后，应用程序找不到动态库
set(CMAKE_SKIP_RPATH TRUE)
endif() #if(APPLE)

if(devOnly)
else()  #if(devOnly)
add_subdirectory(trezor-crypto)
endif() #if(devOnly)

option(protobuf_BUILD_TESTS "Build tests" OFF)
option(protobuf_BUILD_PROTOC_BINARIES "Build libprotoc and protoc compiler" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/cmake)

add_subdirectory(src)

if(devOnly)
else()  #if(devOnly)
if(NOT ANDROID AND NOT IOS)
add_subdirectory(test)
endif() #if(NOT ANDROID AND NOT IOS)
endif() #if(devOnly)
#add_subdirectory(monitor)