//
//  JUB_SDK_DEV_SIM.h
//  JubSDK
//
//  Created by Pan Min on 2020/12/2.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DEV_SIM_h
#define JUB_SDK_DEV_SIM_h

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
 * @function name : JUB_ListLiteSIM
 * @in  param :  ip - PCSC virtual device's IP file name
 *         prod -  JUB_ENUM_DEVICE::LITE
 *         param - LITE_DEVICE_INIT_PARAM
 * @out param : deviceIDs - device ID list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ListLiteSIM(IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, IN LITE_DEVICE_INIT_PARAM param, OUT JUB_UINT16 deviceIDs[MAX_DEVICE]);


/*****************************************************************************
 * @function name : JUB_ListDeviceSIM
 * @in  param : ip - PCSC virtual device's IP file name
 *           prod -  JUB_ENUM_DEVICE::BLADE
 *                JUB_ENUM_DEVICE::BIO
 * @out param : deviceIDs - device ID list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ListDeviceSIM(IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, OUT JUB_UINT16 deviceIDs[MAX_DEVICE]);


/*****************************************************************************
 * @function name : JUB_ConnetDeviceSIM
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ConnetDeviceSIM(IN JUB_UINT16 deviceID);


/*****************************************************************************
 * @function name : JUB_DisconnetDeviceSIM
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_DisconnetDeviceSIM(IN JUB_UINT16 deviceID);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_DEV_SIM_h */
