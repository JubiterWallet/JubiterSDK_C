//
//  JUB_SDK_DEV_SIM.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/12/2.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_DEV_SIM.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "product/ProductFactory.h"


/*****************************************************************************
 * @function name : JUB_ListLiteSIM
 * @in  param :  ip - PCSC virtual device's IP file name
 *         prod -  JUB_ENUM_DEVICE::LITE
 *         param - LITE_DEVICE_INIT_PARAM
 * @out param : deviceIDs - device ID list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ListLiteSIM(IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, IN LITE_DEVICE_INIT_PARAM param, OUT JUB_UINT16 deviceIDs[MAX_DEVICE]) {

#if defined(SIM_MODE)
    CREATE_THREAD_LOCK_GUARD

    if (JUB_ENUM_DEVICE::LITE != prod) {
        return JUBR_ARGUMENTS_BAD;
    }

    auto name_list = jub::device::JubiterBridgeDevice::EnumDevice(std::string(ip));
    //std::cout <<"** "<< name_list.size() << std::endl;

    // add new key
    for (auto name : name_list) {
        //new inserted key
        auto device = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::SIM, prod, std::string(ip), name);
        if (!device) {
            continue;
        }

        JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterBridgeLITEDevice*>(device))->Initialize(param));

        jub::device::DeviceManager::GetInstance()->AddOne(device);
    }   // add new key end

    auto _vDeviceIDs = jub::device::DeviceManager::GetInstance()->GetHandleList();
    for (JUB_UINT16 i = 0 ; i < std::min((size_t)MAX_DEVICE, _vDeviceIDs.size()); i++) {
        deviceIDs[i] = _vDeviceIDs[i];
    }

    return JUBR_OK;
#else   // #if defined(SIM_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SIM_MODE) end
}


/*****************************************************************************
 * @function name : JUB_ListDeviceSIM
 * @in  param : ip - PCSC virtual device's IP file name
 *           prod -  JUB_ENUM_DEVICE::BLADE
 *                JUB_ENUM_DEVICE::BIO
 * @out param : deviceIDs - device ID list
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ListDeviceSIM(IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, OUT JUB_UINT16 deviceIDs[MAX_DEVICE]) {

#if defined(SIM_MODE)
    CREATE_THREAD_LOCK_GUARD

    if (   JUB_ENUM_DEVICE::BLADE != prod
        && JUB_ENUM_DEVICE::BIO   != prod
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    auto name_list = jub::device::JubiterBridgeDevice::EnumDevice(std::string(ip));
    //std::cout <<"** "<< name_list.size() << std::endl;

    // add new key
    for (auto name : name_list) {
        //new inserted key
        auto device = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::SIM, prod, std::string(ip), name);
        if (!device) {
            continue;
        }
        jub::device::DeviceManager::GetInstance()->AddOne(device);
    }   // add new key end

    auto _vDeviceIDs = jub::device::DeviceManager::GetInstance()->GetHandleList();
    for (JUB_UINT16 i = 0 ; i < std::min((size_t)MAX_DEVICE, _vDeviceIDs.size()); i++) {
        deviceIDs[i] = _vDeviceIDs[i];
    }

    return JUBR_OK;
#else   // #if defined(SIM_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SIM_MODE) end
}


/*****************************************************************************
 * @function name : JUB_ConnetDeviceSIM
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ConnetDeviceSIM(IN JUB_UINT16 deviceID) {

#if defined(SIM_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

	JUB_VERIFY_RV(device->Connect());

    return JUBR_OK;
#else   // #if defined(SIM_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SIM_MODE) end
}


/*****************************************************************************
 * @function name : JUB_DisconnetDeviceSIM
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_DisconnetDeviceSIM(IN JUB_UINT16 deviceID) {

#if defined(SIM_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    JUB_VERIFY_RV(device->Disconnect());

    return JUBR_OK;
#else   // #if defined(SIM_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SIM_MODE) end
}
