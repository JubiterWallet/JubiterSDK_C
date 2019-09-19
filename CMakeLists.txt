cmake_minimum_required(VERSION 3.0.0)
project(JubSDK VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE "Debug")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/protos/cpp)

execute_process(COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/protos/cpp --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/protos ${CMAKE_CURRENT_SOURCE_DIR}/protos/Jub_Common.proto)
execute_process(COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/protos/cpp --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/protos ${CMAKE_CURRENT_SOURCE_DIR}/protos/Jub_Bitcoin.proto)
execute_process(COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/protos/cpp --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/protos ${CMAKE_CURRENT_SOURCE_DIR}/protos/Jub_Ethereum.proto)
add_subdirectory(src)
add_subdirectory(trezor-crypto)
add_subdirectory(test)
#add_subdirectory(monitor)


