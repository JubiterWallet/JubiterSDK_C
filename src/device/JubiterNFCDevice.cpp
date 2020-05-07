//
//  JubiterNFCDevice.cpp
//  JubSDK
//
//  Created Pan Min on 2020/3/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//
#include <bleTransmit/android/BTManager.h>
#include "JUB_SDK_COMM.h"


#ifdef NFC_MODE
#include "utility/util.h"
#include "utility/Singleton.h"
#include "device/JubiterNFCDevice.hpp"


#ifdef __ANDROID__
#include "nfcTransmit/android/FTNFCTransmission.h"
#else
#include "nfcTransmit/ios/FTMacro.h"
#include "nfcTransmit/ios/FTNFCTransmission.h"
#endif


namespace jub {
namespace device {


JubiterNFCDevice::JubiterNFCDevice() :
    _param {
        NFC_ScanFuncCallBack
    },
    _handle(0),
    _bConnected(false) {

}


JubiterNFCDevice::~JubiterNFCDevice() {
    Disconnect();
}


JUB_RV JubiterNFCDevice::MatchErrorCode(int error) {

#ifdef __ANDROID__
    switch (error) {
    case BT_SUCCESS:
        return JUBR_OK;
    case BT_ERR_CONNECT_FAIL:
        return JUBR_CONNECT_DEVICE_ERROR;
    case BT_ERR_INIT:
        return JUBR_INIT_DEVICE_LIB_ERROR;
    case BT_ERR_BOND_FAIL:
        return JUBR_BT_BOND_FAILED;
    case BT_ERR_NOT_ENABLE:
    case BT_ERR_NOT_SUPPORT:
    case BT_ERR_NO_LOCATION_PERMISSION:
    case BT_ERR_TRANSMIT_ERROR:
    default:
        return JUBR_TRANSMIT_DEVICE_ERROR;
    } // switch (error) end
#else
    switch ((unsigned long)error) {
    case FT_SUCCESS:
        return JUBR_OK;
    case FT_TAG_CONNECTION_LOST:
        return JUBR_CONNECT_DEVICE_ERROR;
    case FT_COMM_ERROR:
        return JUBR_ERROR;
    default:
        return JUBR_TRANSMIT_DEVICE_ERROR;
    } // switch ((unsigned long)error) end
#endif
}


JUB_RV JubiterNFCDevice::Connect() {

    return JUBR_OK;
}


JUB_RV JubiterNFCDevice::Disconnect() {

    if (             0 != _handle
        &&  FT_SUCCESS == FTIsConnectedNFC(_handle)
        ) {

        _handle = 0;
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCDevice::SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                                  OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                                  IN JUB_ULONG ulMiliSecondTimeout) {

    if (   0 == _handle
        || !FTIsConnectedNFC(_handle)
        ) {
        return JUBR_NOT_CONNECT_DEVICE;
    }

    return FTTransmitNFC(_handle,
                         (JUB_BYTE_PTR)sendData, (JUB_UINT32)ulSendLen,
                         retData, (JUB_UINT32_PTR)pulRetDataLen,
                         (int)ulMiliSecondTimeout);
}


unsigned int JubiterNFCDevice::Initialize(const NFC_INIT_PARAM& params) {

    // init with inner _param
    _param.scanCallBack = params.scanCallBack;
    unsigned int ret = InitializeNFC(nullptr, &_param);
    if (FT_SUCCESS == ret) {
        outerParams = params;
    }

    return (unsigned int)MatchErrorCode(ret);
}


unsigned int JubiterNFCDevice::Finalize() {

    return (unsigned int)MatchErrorCode(FT_SUCCESS);
}


unsigned int JubiterNFCDevice::Scan() {

    return JUBR_IMPL_NOT_SUPPORT;
}


unsigned int JubiterNFCDevice::StopScan() {

    return JUBR_IMPL_NOT_SUPPORT;
}


unsigned int JubiterNFCDevice::Connect(unsigned char* bUUID,      /**< nfc device UUID */
                                       unsigned long* pdevHandle  /**< output ble device connect handle */) {

    unsigned int ret = FTConnectNFC((const char*)bUUID, pdevHandle);
    if (FT_SUCCESS == ret) {
        _handle = *pdevHandle;
        _bConnected = true;
    }

    return (unsigned int)MatchErrorCode(ret);
}


unsigned int JubiterNFCDevice::CancelConnect(unsigned char* bBLEUUID) {

    return JUBR_IMPL_NOT_SUPPORT;
}


unsigned int JubiterNFCDevice::Disconnect(unsigned long handle) {

    return FTDisconnectNFC(handle);
}


unsigned int JubiterNFCDevice::IsConnect(unsigned long handle) {

    if (   0 == handle
        || !FTIsConnectedNFC(handle)
        ) {
        return JUBR_NOT_CONNECT_DEVICE;
    }

    return JUBR_OK;
}


unsigned long JubiterNFCDevice::GetHandle() {

    return _handle;
}


void JubiterNFCDevice::SetHandle(unsigned long handle) {

    _handle = handle;
}


void JubiterNFCDevice::SetConnectStatuteFalse() {

    _bConnected = false;
}


void JubiterNFCDevice::NFC_ScanFuncCallBack(unsigned int errorCode,/* 错误码 */
                                            const char*  uuid,     /* tag uuid */
                                            unsigned int devType   /* 设备类型 */) {
//    if (FT_SUCCESS != errorCode) {
//        _handle = 0;
//        SetConnectStatuteFalse();
//    }
//    else {
//        _handle = handle;
//    }
}


} // namespace device end
} // namespace jub end


#endif // NFC_MODE end
