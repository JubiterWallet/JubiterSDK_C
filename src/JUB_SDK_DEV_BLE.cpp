//
//  JUB_SDK_DEV_BLE.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_DEV_BLE.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "product/ProductFactory.h"


#ifdef __ANDROID__
#include "utils/logUtils.h"
#endif


/// ble device APIs //////////////////////////////////////////
// only works in ble (android and ios)

//#if defined(ANDROID) || defined(TARGET_OS_IPHONE)

// Remove c++ features for swift framework
//stDeviceInitParam::stDeviceInitParam() {
//     callBack = nullptr;
//     scanCallBack = nullptr;
//     discCallBack = nullptr;
//}
// Remove c++ features for swift framework end

JUB_RV JUB_initDevice(IN DEVICE_INIT_PARAM param) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto bleDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::BLE);
    if (   !bleDevice
        || !jub::device::xBLEDeviceFactory::CheckTypeid(bleDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterBLEDevice*>(bleDevice))->Initialize({
        param.param,
        param.callBack,
        param.scanCallBack,
        param.discCallBack})
    );

    return JUBR_OK;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}


JUB_RV JUB_enumDevices(void) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto bleDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::BLE);
    if (   !bleDevice
        || !jub::device::xBLEDeviceFactory::CheckTypeid(bleDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterBLEDevice*>(bleDevice))->Scan());

    return JUBR_OK;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}


JUB_RV JUB_stopEnumDevices(void) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto bleDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::BLE);
    if (   !bleDevice
        || !jub::device::xBLEDeviceFactory::CheckTypeid(bleDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterBLEDevice*>(bleDevice))->StopScan());

    return JUBR_OK;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}


JUB_RV JUB_connectDevice(JUB_BYTE_PTR devName,
                         JUB_BYTE_PTR bBLEUUID,
                         JUB_UINT32 connectType,
                         JUB_UINT16* pDeviceID,
                         JUB_UINT32 timeout) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto bleDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::BLE, std::string((char*)devName));
    if (   !bleDevice
        || !jub::device::xBLEDeviceFactory::CheckTypeid(bleDevice)
        ) {
        LOG_ERR("JUB_connectDevice 参数错误");
        return JUBR_ARGUMENTS_BAD;
    }
    LOG_ERR("JUB_connectDevice 参数正常");

    JUB_ULONG * pdevHandle = new JUB_ULONG;
    JUB_RV rv = (dynamic_cast<jub::device::JubiterBLEDevice*>(bleDevice))->Connect(devName, bBLEUUID, connectType, pdevHandle, timeout);
    LOG_ERR("JUB_connectDevice handle: %lu", *pdevHandle);
    LOG_ERR("JUB_connectDevice 1 rv: %lu", rv);
    JUB_VERIFY_RV(rv);

    *pDeviceID = device_map::GetInstance()->AddOne(pdevHandle);
    LOG_ERR("JUB_connectDevice deviceID: %hu", *pDeviceID);

    jub::device::DeviceManager::GetInstance()->AddOne(*pDeviceID, bleDevice);

    return rv;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}


JUB_RV JUB_cancelConnect(JUB_BYTE_PTR devName, JUB_BYTE_PTR bBLEUUID) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto bleDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::BLE);
    if (   !bleDevice
        || !jub::device::xBLEDeviceFactory::CheckTypeid(bleDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterBLEDevice*>(bleDevice))->CancelConnect(devName, bBLEUUID));

    return JUBR_OK;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}


JUB_RV JUB_disconnectDevice(JUB_UINT16 deviceID) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto bleDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !bleDevice
        || !jub::device::xBLEDeviceFactory::CheckTypeid(bleDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_ULONG *devHandle = device_map::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(devHandle);
    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterBLEDevice*>(bleDevice))->Disconnect(*devHandle));

    return JUBR_OK;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}


JUB_RV JUB_isDeviceConnect(JUB_UINT16 deviceID) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto bleDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !bleDevice
        || !jub::device::xBLEDeviceFactory::CheckTypeid(bleDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_ULONG *devHandle = device_map::GetInstance()->GetOne(deviceID);
    if (NULL == devHandle) {
        return JUBR_CONNECT_DEVICE_ERROR;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterBLEDevice*>(bleDevice))->IsConnect(*devHandle));

    return JUBR_OK;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}


/*****************************************************************************
 * @function name : JUB_QueryBattery
 * @in  param : deviceID - device ID
 * @out param : percent
 * @last change :
 *****************************************************************************/
JUB_RV JUB_QueryBattery(IN JUB_UINT16 deviceID,
                        OUT JUB_BYTE_PTR percent) {

#if defined(BLE_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBladeToken>(deviceID);

    JUB_VERIFY_RV(token->QueryBattery(*percent));

    return JUBR_OK;
#else   // #if defined(BLE_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) end
}
