cmake_minimum_required(VERSION 3.14.0)

message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ SRC_LIST)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../JubSDK SRC_LIST)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/libbitcoin)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JubSDK)

ADD_LIBRARY(JubSDK SHARED ${SRC_LIST})
set_target_properties(JubSDK PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION C
  MACOSX_FRAMEWORK_IDENTIFIER com.ftsafe.JubSDK
  #MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/../JubSDK/Info.plist
  # "current version" in semantic format in Mach-O binary file
  VERSION 16.4.0
  # "compatibility version" in semantic format in Mach-O binary file
  SOVERSION 1.0.0
  PUBLIC_HEADER JubSDKCore.h
  PUBLIC_HEADER JubSDKCore+DEV.h
  PUBLIC_HEADER JubSDKCore+DEV_BLE.h
  PUBLIC_HEADER JubSDKCore+COIN_BTC.h
  PUBLIC_HEADER JubSDKCore+COIN_ETH.h
  PUBLIC_HEADER JubSDKCore+HCash.h
  PUBLIC_HEADER JubSDKCore+COIN_EOS.h
  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: 敏 潘"
)
target_link_libraries(JubSDK LINK_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../deps/bleTransmit/ios/libFTTransmission.a)
target_link_libraries(JubSDK LINK_PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../build/src/Release-iphoneos/libJUB_SDK.a)

if(APPLE)
#    message("ENDIAN_PATH = ${ENDIAN_PATH}")
    target_compile_definitions(JubSDK PRIVATE)
endif()
