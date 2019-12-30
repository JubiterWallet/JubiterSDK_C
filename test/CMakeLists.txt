cmake_minimum_required(VERSION 3.0.0)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/test/JUB_SDK_test)

include_directories(${CMAKE_SOURCE_DIR}/deps/jsoncpp/include)
include_directories(${CMAKE_SOURCE_DIR}/deps)
include_directories(${CMAKE_SOURCE_DIR}/deps/libbitcoin)
include_directories(${CMAKE_SOURCE_DIR}/deps/TrustWallet/wallet-core/src)

aux_source_directory(${CMAKE_SOURCE_DIR}/deps/jsoncpp/src/lib_json SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/test/JUB_SDK_test SRC_LIST)

add_executable(JUB_test ${SRC_LIST})

#user lib
if(APPLE)
    FIND_LIBRARY(HID_API_LIB_MAC_STATIC
        NAMES libhidapi.a
    )
    target_link_libraries(JUB_test PRIVATE JUB_SDK TrezorCrypto ${HID_API_LIB_MAC_STATIC}  "-framework CoreFoundation" "-framework IOKit")
else()
    target_link_libraries(JUB_test PRIVATE JUB_SDK TrezorCrypto hidapi)  
endif(APPLE)



