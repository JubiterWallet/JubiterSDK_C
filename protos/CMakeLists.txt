project(protobuf_lite)

cmake_minimum_required(VERSION 3.6.0)

message(">>> in protobuf")

set(protobuf_source_dir ${CMAKE_CURRENT_SOURCE_DIR}/protobuf_java)
message(">>>protobuf_source_dir 目录： ${protobuf_source_dir}")

set(libprotobuf_files
        ${protobuf_source_dir}/src/google/protobuf/any.cc
        ${protobuf_source_dir}/src/google/protobuf/any.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/api.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/compiler/importer.cc
        ${protobuf_source_dir}/src/google/protobuf/compiler/parser.cc
        ${protobuf_source_dir}/src/google/protobuf/descriptor.cc
        ${protobuf_source_dir}/src/google/protobuf/descriptor.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/descriptor_database.cc
        ${protobuf_source_dir}/src/google/protobuf/duration.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/dynamic_message.cc
        ${protobuf_source_dir}/src/google/protobuf/empty.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/extension_set_heavy.cc
        ${protobuf_source_dir}/src/google/protobuf/field_mask.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/generated_message_reflection.cc
        ${protobuf_source_dir}/src/google/protobuf/generated_message_table_driven.cc
        ${protobuf_source_dir}/src/google/protobuf/io/gzip_stream.cc
        ${protobuf_source_dir}/src/google/protobuf/io/printer.cc
        ${protobuf_source_dir}/src/google/protobuf/io/tokenizer.cc
        ${protobuf_source_dir}/src/google/protobuf/map_field.cc
        ${protobuf_source_dir}/src/google/protobuf/message.cc
        ${protobuf_source_dir}/src/google/protobuf/reflection_ops.cc
        ${protobuf_source_dir}/src/google/protobuf/service.cc
        ${protobuf_source_dir}/src/google/protobuf/source_context.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/struct.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/stubs/mathlimits.cc
        ${protobuf_source_dir}/src/google/protobuf/stubs/substitute.cc
        ${protobuf_source_dir}/src/google/protobuf/text_format.cc
        ${protobuf_source_dir}/src/google/protobuf/timestamp.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/type.pb.cc
        ${protobuf_source_dir}/src/google/protobuf/unknown_field_set.cc
        ${protobuf_source_dir}/src/google/protobuf/util/delimited_message_util.cc
        ${protobuf_source_dir}/src/google/protobuf/util/field_comparator.cc
        ${protobuf_source_dir}/src/google/protobuf/util/field_mask_util.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/datapiece.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/default_value_objectwriter.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/error_listener.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/field_mask_utility.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/json_escaping.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/json_objectwriter.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/json_stream_parser.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/object_writer.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/proto_writer.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectsource.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectwriter.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/type_info.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/type_info_test_helper.cc
        ${protobuf_source_dir}/src/google/protobuf/util/internal/utility.cc
        ${protobuf_source_dir}/src/google/protobuf/util/json_util.cc
        ${protobuf_source_dir}/src/google/protobuf/util/message_differencer.cc
        ${protobuf_source_dir}/src/google/protobuf/util/time_util.cc
        ${protobuf_source_dir}/src/google/protobuf/util/type_resolver_util.cc
        ${protobuf_source_dir}/src/google/protobuf/wire_format.cc
        ${protobuf_source_dir}/src/google/protobuf/wrappers.pb.cc
        )

message(">>> 1")

set(libprotobuf_includes
        ${protobuf_source_dir}/src/google/protobuf/any.h
        ${protobuf_source_dir}/src/google/protobuf/any.pb.h
        ${protobuf_source_dir}/src/google/protobuf/api.pb.h
        ${protobuf_source_dir}/src/google/protobuf/compiler/importer.h
        ${protobuf_source_dir}/src/google/protobuf/compiler/parser.h
        ${protobuf_source_dir}/src/google/protobuf/descriptor.h
        ${protobuf_source_dir}/src/google/protobuf/descriptor.pb.h
        ${protobuf_source_dir}/src/google/protobuf/descriptor_database.h
        ${protobuf_source_dir}/src/google/protobuf/duration.pb.h
        ${protobuf_source_dir}/src/google/protobuf/dynamic_message.h
        ${protobuf_source_dir}/src/google/protobuf/empty.pb.h
        ${protobuf_source_dir}/src/google/protobuf/field_mask.pb.h
        ${protobuf_source_dir}/src/google/protobuf/generated_message_reflection.h
        ${protobuf_source_dir}/src/google/protobuf/io/gzip_stream.h
        ${protobuf_source_dir}/src/google/protobuf/io/printer.h
        ${protobuf_source_dir}/src/google/protobuf/io/tokenizer.h
        ${protobuf_source_dir}/src/google/protobuf/map_field.h
        ${protobuf_source_dir}/src/google/protobuf/message.h
        ${protobuf_source_dir}/src/google/protobuf/reflection_ops.h
        ${protobuf_source_dir}/src/google/protobuf/service.h
        ${protobuf_source_dir}/src/google/protobuf/source_context.pb.h
        ${protobuf_source_dir}/src/google/protobuf/struct.pb.h
        ${protobuf_source_dir}/src/google/protobuf/stubs/mathlimits.h
        ${protobuf_source_dir}/src/google/protobuf/stubs/substitute.h
        ${protobuf_source_dir}/src/google/protobuf/text_format.h
        ${protobuf_source_dir}/src/google/protobuf/timestamp.pb.h
        ${protobuf_source_dir}/src/google/protobuf/type.pb.h
        ${protobuf_source_dir}/src/google/protobuf/unknown_field_set.h
        ${protobuf_source_dir}/src/google/protobuf/util/delimited_message_util.h
        ${protobuf_source_dir}/src/google/protobuf/util/field_comparator.h
        ${protobuf_source_dir}/src/google/protobuf/util/field_mask_util.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/datapiece.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/default_value_objectwriter.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/error_listener.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/field_mask_utility.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/json_escaping.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/json_objectwriter.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/json_stream_parser.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/object_writer.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/proto_writer.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectsource.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectwriter.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/type_info.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/type_info_test_helper.h
        ${protobuf_source_dir}/src/google/protobuf/util/internal/utility.h
        ${protobuf_source_dir}/src/google/protobuf/util/json_util.h
        ${protobuf_source_dir}/src/google/protobuf/util/message_differencer.h
        ${protobuf_source_dir}/src/google/protobuf/util/time_util.h
        ${protobuf_source_dir}/src/google/protobuf/util/type_resolver_util.h
        ${protobuf_source_dir}/src/google/protobuf/wire_format.h
        ${protobuf_source_dir}/src/google/protobuf/wrappers.pb.h

        ########################### 追加 ##############################
        ${protobuf_source_dir}/src/google/protobuf/stubs/logging.h
        ${protobuf_source_dir}/src/google/protobuf/stubs/common.h
        ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream.h
        )


message(">>> 2")

include_directories(${protobuf_source_dir}/src)

add_library(libprotobuf
        STATIC
        ${libprotobuf_files}
        ${libprotobuf_includes}
        )

message(">>> 3")
add_definitions(-DHAVE_PTHREAD)
message(">>> 4")
target_link_libraries(libprotobuf ${CMAKE_THREAD_LIBS_INIT})

message(">>> 5")

#target_include_directories(libprotobuf PUBLIC ${protobuf_source_dir}/src)
#
#set_target_properties(libprotobuf PROPERTIES
#        VERSION ${protobuf_VERSION}
#        OUTPUT_NAME protobuf
#        DEBUG_POSTFIX "${protobuf_DEBUG_POSTFIX}")
#add_library(protobuf::libprotobuf ALIAS libprotobuf)