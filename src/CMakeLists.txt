cmake_minimum_required(VERSION 3.6.0)


if(devOnly)

#ADD_LIBRARY(JUB_SDK STATIC ${SRC_LIST})
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/JUB_SDK_DEV.cpp)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/JUB_SDK_DEV_HID.cpp)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/utility SRC_LIST)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/context/BaseContext.cpp)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/device/JubiterHidDevice.cpp)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/token/JubiterBladeToken.cpp)


list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_DEV.h)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_DEV_HID.h)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/../include/device/JubiterHidDevice.hpp)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/../include/token/interface/BaseToken.h)
list(APPEND SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/../include/token/JubiterBlade/JubiterBladeToken.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../trezor-crypto/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/json/include)

else()  #if(devOnly)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/utility SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/context SRC_LIST)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/device SRC_LIST)
if(IOS OR ANDROID)
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/device/JubiterBridgeDevice.cpp")
else()
set (CMAKE_CXX_STANDARD 17)
set(SRC_LIST ${SRC_LIST} "${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/jubiter_bridge.pb.cc")
endif()
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token/BTC SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token/ETH SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token/EOS SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token/HC SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token/XRP SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/token/TRX SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/HDKey SRC_LIST)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bigint SRC_LIST)
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/bigint/sample.cc")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/bigint/testsuite.cc")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/interface SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Bitcoin SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/EOS SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Ethereum SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Ripple SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron SRC_LIST)
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron/ContractShield.h")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron/ContractShield.cpp")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron/ContractVoteAsset.h")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron/ContractVoteAsset.cpp")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron/ContractWitness.h")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron/ContractWitness.cpp")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract SRC_LIST)
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract/shield_contract.pb.cc")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract/shield_contract.pb.h")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract/vote_asset_contract.pb.cc")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract/vote_asset_contract.pb.h")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract/witness_contract.pb.cc")
list(REMOVE_ITEM SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract/witness_contract.pb.h")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../gpc/src SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../pbparse SRC_LIST)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps)


if(IOS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bleTransmit SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/nfcTransmit SRC_LIST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bleTransmit/ios)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/nfcTransmit/ios)
elseif(ANDROID) #if(IOS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bleTransmit SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/nfcTransmit SRC_LIST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bleTransmit/android)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/nfcTransmit/android)
endif() #if(IOS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/protobuf/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/bigint)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/json/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/include/TrustWalletCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Bitcoin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/EOS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Ethereum)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Ripple)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core/src/Tron)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/TrustWallet/wallet-core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../trezor-crypto/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gpc/include)

endif() #if(devOnly)

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../include DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
# 把需要打包的头文件加进工程,方便打包BLE和NFC暂时先加进来，应该没有影响
set(HEADERS
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_CORE.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_BTC.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_COMM.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_DEV_BIO.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_DEV_BLE.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_DEV_HID.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_DEV_NFC.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_DEV.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_EOS.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_ETH.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_Hcash.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_XRP.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK_TRX.h
${CMAKE_CURRENT_SOURCE_DIR}/../include/JUB_SDK.h
)

ADD_LIBRARY(JUB_SDK STATIC ${SRC_LIST})
set_target_properties(JUB_SDK PROPERTIES PUBLIC_HEADER "${HEADERS}")

# test endian.h
find_path(ENDIAN_PATH endian.h)
find_path(SYS_ENDIAN_PATH sys/endian.h)

if(ENDIAN_PATH)
    message("ENDIAN_PATH = ${ENDIAN_PATH}")
    target_compile_definitions(JUB_SDK PRIVATE HAVE_ENDIAN_H=1)
elseif(SYS_ENDIAN_PATH)
    message("SYS_ENDIAN_PATH = ${SYS_ENDIAN_PATH}")
    target_compile_definitions(JUB_SDK PRIVATE HAVE_SYS_ENDIAN_H=1)
endif() #if(ENDIAN_PATH)

install(TARGETS JUB_SDK
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        )

#endif() #if(devOnly)
