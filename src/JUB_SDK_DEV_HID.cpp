//
//  JUB_SDK_DEV_HID.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_DEV_HID.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "product/ProductFactory.h"
#include "device/JubiterHidDevice.hpp"


/*****************************************************************************
 * @function name : JUB_ListDeviceHid
 * @in  param :
 * @out param : deviceIDs - device ID list
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE]) {

#if defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    std::vector<unsigned short> pid_list = jub::device::xHidDeviceFactory::EnumPID();

    for (auto pid : pid_list) {

    auto path_list = jub::device::JubiterHidDevice::EnumDevice(pid);
    //std::cout <<"** "<< path_list.size() << std::endl;

    // deal with removed key
    auto vDeviceIDs = jub::device::DeviceManager::GetInstance()->GetHandleList();
    for (JUB_UINT16 i = 0; i < vDeviceIDs.size(); i++) {
        auto device = dynamic_cast<jub::device::JubiterHidDevice*>(jub::device::DeviceManager::GetInstance()->GetOne(vDeviceIDs[i]));
        if (!device) {
            continue;
        }

        if (std::end(path_list) == std::find(std::begin(path_list), std::end(path_list), device->getPath())
            && jub::device::xHidDeviceFactory::CheckTypeid(pid, device)
            ) {
            //removed key
            jub::device::DeviceManager::GetInstance()->ClearOne(vDeviceIDs[i]);
        }
    }   // deal with removed key end

    // deal with inserted key
    auto isInManager = [](const std::string& path, const unsigned short pid) -> bool {
        auto vDeviceIDs = jub::device::DeviceManager::GetInstance()->GetHandleList();
        for (JUB_UINT16 i = 0; i < vDeviceIDs.size(); i++) {
            auto device = dynamic_cast<jub::device::JubiterHidDevice*>(jub::device::DeviceManager::GetInstance()->GetOne(vDeviceIDs[i]));
            if (!device) {
                continue;
            }

            if (   device
                && path == device->getPath()
                && jub::device::xHidDeviceFactory::CheckTypeid(pid, device)
                ) {
                return true;
            }
        }

        return false;
    };  // deal with inserted key end

    // add new key
    for (auto path : path_list) {
        if (isInManager(path, pid)) {
            continue;
        }

        //new inserted key
        auto device = jub::product::prdsFactory::GetInstance()->CreateProduct(pid, path);
        if (!device) {
            continue;
        }
        jub::device::DeviceManager::GetInstance()->AddOne(device);
    }   // // add new key end
    }   // for (auto pid : pid_list) end

    auto _vDeviceIDs = jub::device::DeviceManager::GetInstance()->GetHandleList();
    for (JUB_UINT16 i = 0 ; i < std::min((size_t)MAX_DEVICE, _vDeviceIDs.size()); i++) {
        deviceIDs[i] = _vDeviceIDs[i];
    }

    return JUBR_OK;
#else   // #if defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(HID_MODE) end
}

/*****************************************************************************
 * @function name : JUB_ConnetDeviceHid
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ConnetDeviceHid(IN JUB_UINT16 deviceID) {

#if defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

	JUB_VERIFY_RV(device->Connect());

    return JUBR_OK;
#else   // #if defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(HID_MODE) end
}

/*****************************************************************************
 * @function name : JUB_DisconnetDeviceHid
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_DisconnetDeviceHid(IN JUB_UINT16 deviceID) {

#if defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    JUB_VERIFY_RV(device->Disconnect());

    return JUBR_OK;
#else   // #if defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(HID_MODE) end
}
