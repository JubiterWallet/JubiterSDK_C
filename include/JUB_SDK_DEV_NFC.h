//
//  JUB_SDK_DEV_NFC.h
//  JubSDK
//
//  Created by Pan Min on 2020/3/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DEV_NFC_h
#define JUB_SDK_DEV_NFC_h

#include "JUB_SDK_DEV.h"

///// ble device APIs //////////////////////////////////////////
//#define DEV_SUCCESS 0               /**< no error */
//#define DEV_TIME*OUT 1               /**< conn time out */
//#define DEV_COMMUNICATION_ERROR 612 /**< generic error */
//#define DEV_RESPONSE_TIMEOUT    613 /**< timeout */
//#define DEV_NOT_SUPPORTED       614 /**< request is not supported */
//#define DEV_NO_DEVICE           615 /**< no device>*/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef void (*NFC_ScanFuncCallBack)(
    unsigned int errorCode, /* 错误码 */
    const char*  uuid,      /* tag uuid */
    unsigned int devType    /* 设备类型 */
);

// Remove c++ features for swift framework
typedef struct stNFCDeviceInitParam {
    void* param;
    NFC_ScanFuncCallBack scanCallBack;
//
//     stNFCDeviceInitParam();
//    ~stNFCDeviceInitParam() = default;
} NFC_DEVICE_INIT_PARAM;
// Remove c++ features for swift framework end

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_initNFCDevice(IN NFC_DEVICE_INIT_PARAM param);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_connectNFCDevice(JUB_BYTE_PTR bUUID,     /**< nfc device UUID */
                            JUB_UINT16* pDeviceID   /**< output nfc device connect handle */
);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_disconnectNFCDevice(JUB_UINT16 deviceID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_isDeviceNFCConnect(JUB_UINT16 deviceID);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif /* JUB_SDK_DEV_NFC_h */
