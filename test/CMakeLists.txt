cmake_minimum_required(VERSION 3.0.0)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/deps/jsoncpp/include)
include_directories(${CMAKE_SOURCE_DIR}/deps)
include_directories(${CMAKE_SOURCE_DIR}/deps/TrustWallet/wallet-core/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gpc/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../trezor-crypto/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/deps/jsoncpp/src/lib_json SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/gpc/src SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/trezor-crypto/src SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/test/JUB_SDK_test SRC_LIST)
if(APPLE)
list(REMOVE_ITEM SRC_LIST "${CMAKE_SOURCE_DIR}/test/JUB_SDK_test/stdafx.h")
list(REMOVE_ITEM SRC_LIST "${CMAKE_SOURCE_DIR}/test/JUB_SDK_test/stdafx.cpp")
list(REMOVE_ITEM SRC_LIST "${CMAKE_SOURCE_DIR}/test/JUB_SDK_test/targetver.h")
endif(APPLE)

FIND_LIBRARY(HID_API_LIB_STATIC
    NAMES hidapi hidapi-hidraw
)
MESSAGE( STATUS "hidapi = ${HID_API_LIB_STATIC}.")

add_executable(JUB_test ${SRC_LIST})

#user lib
if(APPLE)
    #MacOS与iOS链接framework
    target_link_libraries(JUB_test PRIVATE  "-framework JubSDK" "-framework CoreFoundation" "-framework IOKit")
else()
    #其他平台链接静态库
    target_link_libraries(JUB_test PRIVATE JubSDK TrezorCrypto ${HID_API_LIB_STATIC})  
endif(APPLE)
