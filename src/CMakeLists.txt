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

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract SRC_LIST)

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

if(APPLE) #MacOS与iOS打出来的为framework
    #打framework时，只能添加自己这个目录下的文件，先把需要的头文件拷过来
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../include DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
    # 把需要打包的头文件加进工程,方便打包BLE和NFC暂时先加进来，应该没有影响
    set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_CORE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_BTC.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_COMM.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_DEV_BIO.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_DEV_BLE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_DEV_HID.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_DEV_NFC.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_DEV.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_EOS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_ETH.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_Hcash.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_TRX.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK_XRP.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/JUB_SDK.h
    )

    #framework需要codesign，证书怎么处理还没想明白，先写死，不同的人打包请先手动修改，这里指定TeamID
    set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "PSRT22QP6C" CACHE INTERNAL "")
    #动态的framework，HEADERS需要加到工程里，否则打出的framework没有头文件
    add_library (JubSDK SHARED ${SRC_LIST} ${HEADERS})
    set_target_properties(JubSDK PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_IDENTIFIER com.ftsafe.JubSDK
    #MACOSX_FRAMEWORK_INFO_PLIST Info.plist
    #PUBLIC_HEADER只能是这种格式，不能直接用${HEADERS}，WHY？？？不知道CMake还是XCode闹哪样。
    PUBLIC_HEADER "include/JUB_CORE.h;include/JUB_SDK_BTC.h;include/JUB_SDK_COMM.h;include/JUB_SDK_DEV_BIO.h;include/JUB_SDK_DEV_BLE.h;include/JUB_SDK_DEV_HID.h;include/JUB_SDK_DEV_NFC.h;include/JUB_SDK_DEV.h;include/JUB_SDK_EOS.h;include/JUB_SDK_ETH.h;include/JUB_SDK_Hcash.h;include/JUB_SDK_TRX.h;include/JUB_SDK_XRP.h;include/JUB_SDK.h"
    #这里指定签名证书，也需要手动修改
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development: zxckiller@163.com (BV9H9Z227X)"
    )

    #链接静态库
    if(IOS)

        FIND_LIBRARY(BT_LIB_STATIC
        NAMES FTTransmission 
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../deps/bleTransmit/ios
        )   
        MESSAGE( STATUS "FTTransmission = ${BT_LIB_STATIC}.")

        FIND_LIBRARY(NFC_LIB_STATIC
        NAMES FTNFCTransmission 
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../deps/nfcTransmit/ios
        )   
        MESSAGE( STATUS "NFCFTTransmission = ${NFC_LIB_STATIC}.")

        target_link_libraries(JubSDK PRIVATE libprotobuf TrezorCrypto ${BT_LIB_STATIC} ${NFC_LIB_STATIC} "-framework CoreFoundation" "-framework IOKit")   
    else()#MacOS

        FIND_LIBRARY(HID_API_LIB_STATIC
        NAMES hidapi hidapi-hidraw
        )   
        MESSAGE( STATUS "hidapi = ${HID_API_LIB_STATIC}.")
        target_link_libraries(JubSDK PRIVATE libprotobuf TrezorCrypto ${HID_API_LIB_STATIC}  "-framework CoreFoundation" "-framework IOKit")
    endif()


else() #其他平台为.a静态库
    ADD_LIBRARY(JubSDK STATIC ${SRC_LIST})
endif()


# test endian.h
find_path(ENDIAN_PATH endian.h)
find_path(SYS_ENDIAN_PATH sys/endian.h)

if(ENDIAN_PATH)
    message("ENDIAN_PATH = ${ENDIAN_PATH}")
    target_compile_definitions(JubSDK PRIVATE HAVE_ENDIAN_H=1)
elseif(SYS_ENDIAN_PATH)
    message("SYS_ENDIAN_PATH = ${SYS_ENDIAN_PATH}")
    target_compile_definitions(JubSDK PRIVATE HAVE_SYS_ENDIAN_H=1)
endif() #if(ENDIAN_PATH)

#endif() #if(devOnly)
