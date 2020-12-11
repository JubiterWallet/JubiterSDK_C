//
//  JUB_SDK_DEV_GRPC.h
//  JubSDK
//
//  Created by Pan Min on 2020/12/2.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DEV_GRPC_h
#define JUB_SDK_DEV_GRPC_h

#include "JUB_SDK.h"

#define MAX_DEVICE                  8

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Remove c++ features for swift framework
typedef struct stLiteDeviceInitParam {
    JUB_CHAR_PTR crt;
    JUB_CHAR_PTR sk;
    JUB_UINT8 keyLength;
    JUB_CHAR_PTR hostID;
//
//     stLiteDeviceInitParam();
//    ~stLiteDeviceInitParam() = default;
} LITE_DEVICE_INIT_PARAM;
// Remove c++ features for swift framework end


/*****************************************************************************
 * @function name : JUB_ListLiteGRPC
 * @in  param :  ip - gRPC virtual device's IP file name
 *         prod -  JUB_ENUM_DEVICE::LITE
 *         param - LITE_DEVICE_INIT_PARAM
 * @out param : deviceIDs - device ID list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ListLiteGRPC(IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, IN LITE_DEVICE_INIT_PARAM param, OUT JUB_UINT16 deviceIDs[MAX_DEVICE]);


/*****************************************************************************
 * @function name : JUB_ListDeviceGRPC
 * @in  param : ip - gRPC virtual device's IP file name
 *           prod -  JUB_ENUM_DEVICE::BLADE
 *                JUB_ENUM_DEVICE::BIO
 * @out param : deviceIDs - device ID list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ListDeviceGRPC(IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, OUT JUB_UINT16 deviceIDs[MAX_DEVICE]);


/*****************************************************************************
 * @function name : JUB_ConnetDeviceGRPC
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ConnetDeviceGRPC(IN JUB_UINT16 deviceID);


/*****************************************************************************
 * @function name : JUB_DisconnetDeviceGRPC
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_DisconnetDeviceGRPC(IN JUB_UINT16 deviceID);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_DEV_GRPC_h */
