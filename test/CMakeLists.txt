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

add_executable(JUB_test ${SRC_LIST})

FIND_LIBRARY(HID_API_LIB_STATIC
    NAMES hidapi hidapi-hidraw
)
MESSAGE( STATUS "hidapi = ${HID_API_LIB_STATIC}.")

#user lib
if(APPLE)
    target_link_libraries(JUB_test PRIVATE JUB_SDK TrezorCrypto libprotobuf ${HID_API_LIB_STATIC}" -framework CoreFoundation" "-framework IOKit")
else()
    target_link_libraries(JUB_test PRIVATE JUB_SDK TrezorCrypto libprotobuf ${HID_API_LIB_STATIC})  
endif(APPLE)


install(TARGETS JUB_test
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/json
        DESTINATION bin
)