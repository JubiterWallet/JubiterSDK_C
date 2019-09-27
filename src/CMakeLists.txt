cmake_minimum_required(VERSION 3.6.0)

#file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../protos/cpp)

#execute_process(COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}}/../protos/cpp --proto_path=${CMAKE_CURRENT_SOURCE_DIR}}/../protos ${CMAKE_CURRENT_SOURCE_DIR}}/../protos/Jub_Common.proto)
#execute_process(COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}}/../protos/cpp --proto_path=${CMAKE_CURRENT_SOURCE_DIR}}/../protos ${CMAKE_CURRENT_SOURCE_DIR}}/../protos/Jub_Bitcoin.proto)
#execute_process(COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}}/../protos/cpp --proto_path=${CMAKE_CURRENT_SOURCE_DIR}}/../protos ${CMAKE_CURRENT_SOURCE_DIR}}/../protos/Jub_Ethereum.proto)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/utility SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/context SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/device SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/libETH SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/HDKey SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bigint SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bitcoin SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bitcoinABC SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/polarssl SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bleTransmit SRC_LIST)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bigint)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bitcoin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bitcoinABC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/libbitcoin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../trezor-crypto/include)


ADD_LIBRARY(JUB_SDK STATIC ${SRC_LIST})

# test endian.h
find_path(ENDIAN_PATH endian.h)
find_path(SYS_ENDIAN_PATH sys/endian.h)

if(ENDIAN_PATH)
    message("ENDIAN_PATH = ${ENDIAN_PATH}")
    target_compile_definitions(JUB_SDK PRIVATE HAVE_ENDIAN_H=1)
elseif(SYS_ENDIAN_PATH)
    message("SYS_ENDIAN_PATH = ${SYS_ENDIAN_PATH}")
    target_compile_definitions(JUB_SDK PRIVATE HAVE_SYS_ENDIAN_H=1)
endif()

