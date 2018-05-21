project(coreNDK)

cmake_minimum_required(VERSION 3.6.0)

message("===============================>>> ndk")

# **************************************************************************************************
#                               引入相关头文件
# 注：因为是分开编译且涉及相互调用，所以需要引入其他库模块中的头文件
# **************************************************************************************************

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jni)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jni/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/formats)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/polarssl)



# **************************************************************************************************
#                      查找指定目录下的所有源文件，并将其存入指定变量中
# **************************************************************************************************

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/jni SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/jni/utils SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/context SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/device SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/token SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/utility SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/deps/formats SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/deps/polarssl SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/deps/bleTransmit SRC_LIST)




# **************************************************************************************************
#                                添加其他预构建库（第三方库）
# **************************************************************************************************

find_library(libLog
             log
)

# 需要改成平台自适应的宏
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/bleTransmit/android/${ANDROID_ABI})


# **************************************************************************************************
#                                        生成动态库
# **************************************************************************************************

add_library(
            coreNDK
            SHARED
            ${SRC_LIST}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_ENABLE_RTREE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_ENABLE_COLUMN_METADATA")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_HAS_CODEC=1")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE3ENCRYPT_EXPORTS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_ENABLE_FTS3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_ENABLE_FTS3_PARENTHESIS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_SECURE_DELETE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_SOUNDEX")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCODEC_TYPE=CODEC_TYPE_AES128")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE_ENABLE_RTREE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE_ENABLE_COLUMN_METADATA")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE_HAS_CODEC=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE3ENCRYPT_EXPORTS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE_ENABLE_FTS3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE_ENABLE_FTS3_PARENTHESIS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE_SECURE_DELETE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSQLITE_SOUNDEX")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCODEC_TYPE=CODEC_TYPE_AES128")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_ICU")


# **************************************************************************************************
#                                        链接所有相关库
# 切记链接日志库，否则编译报错 undefined reference to '__android_log_print'
# **************************************************************************************************

target_link_libraries(
                      coreNDK

                      BTSDK

                      ${libLog}
)
