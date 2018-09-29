cmake_minimum_required(VERSION 3.0.0)

include_directories(${CMAKE_SOURCE_DIR}/deps/jsoncpp/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/deps/jsoncpp/src/lib_json SRC_LIST)

if(MSVC)   # msvc

    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")   #x86

        FIND_LIBRARY(HID_API_LIBRARY_RELEASE
        NAMES hidapi
        PATHS ${CMAKE_SOURCE_DIR}/deps/hidapi/libhidapi
        )

        FIND_LIBRARY(HID_API_LIBRARY_DEBUG
        NAMES hidapi_d
        PATHS ${CMAKE_SOURCE_DIR}/deps/hidapi/libhidapi
        )

    elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8") #x64

        FIND_LIBRARY(HID_API_LIBRARY_RELEASE
        NAMES hidapi
        PATHS ${CMAKE_SOURCE_DIR}/deps/hidapi/libhidapi/x64
        )

        FIND_LIBRARY(HID_API_LIBRARY_DEBUG
        NAMES hidapi_d
        PATHS ${CMAKE_SOURCE_DIR}/deps/hidapi/libhidapi/x64
        )

    endif()

endif()


SET(JUB_SDK_LIBRARY debug ${JUB_SDK_LIBRARY_DEBUG} optimized ${JUB_SDK_LIBRARY_RELEASE})
SET(HID_API_LIBRARY debug ${HID_API_LIBRARY_DEBUG} optimized ${HID_API_LIBRARY_RELEASE})


message("==========================")
message("${CMAKE_SIZEOF_VOID_P}  ${JUB_SDK_LIBRARY}   ${HID_API_LIBRARY} " )

add_executable(test "JUB_SDK_test/JUB_SDK_test.cpp" ${SRC_LIST})

#user lib
target_link_libraries(test PRIVATE JUB_SDK ${HID_API_LIBRARY} Setupapi)

