/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "JUB_SDK_DEV_SWI.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "product/ProductFactory.h"


/*****************************************************************************
 * @function name : JUB_ConnetDeviceSWI
 * @in  param :
 * @out param : deviceID - device ID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ConnetDeviceSWI(JUB_UINT16_PTR pDeviceID) {

#if defined(SWI_MODE)
    CREATE_THREAD_LOCK_GUARD
    std::string deviceName = jub::device::JubiterSWIDevice::randomName();
    auto swiDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::SWI, deviceName);
    if (   !swiDevice
//        || !jub::device::xBLEDeviceFactory::CheckTypeid(swiDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }
    jub::device::DeviceManager::GetInstance()->AddOne(swiDevice);

    auto _vDeviceIDs = jub::device::DeviceManager::GetInstance()->GetHandleList();
    for (JUB_UINT16 i = 0 ; i < std::min((size_t)MAX_DEVICE, _vDeviceIDs.size()); ++i) {
        auto *devicePtr = jub::device::DeviceManager::GetInstance()->GetOne(_vDeviceIDs[i]);
        if (dynamic_cast<jub::device::JubiterSWIImplDevice*>(devicePtr)
            && deviceName == dynamic_cast<jub::device::JubiterSWIImplDevice*>(devicePtr)->getName()
            ) {
            *pDeviceID = _vDeviceIDs[i];
            break;
        }
    }
    if (0 == (*pDeviceID)) {
        return JUBR_ERROR;
    }

    auto device = jub::device::DeviceManager::GetInstance()->GetOne(*pDeviceID);
    JUB_CHECK_NULL(device);

    JUB_VERIFY_RV(device->Connect());

    return JUBR_OK;
#else   // #if defined(SWI_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SWI_MODE) end
}


/*****************************************************************************
 * @function name : JUB_DisconnetDeviceSWI
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_DisconnetDeviceSWI(IN JUB_UINT16 deviceID) {

#if defined(SWI_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    JUB_VERIFY_RV(device->Disconnect());

    return JUBR_OK;
#else   // #if defined(SWI_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SWI_MODE) end
}


/*****************************************************************************
 * @function name : JUB_BuildFromMasterPrivateKey
 * @in  param : deviceID - device ID
 *           curve - JUB_ENUM_CURVES::SECP256K1, JUB_ENUM_CURVES::ED25519
 *           masterPrivateKey - Master PrivateKey
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_BuildFromMasterPrivateKey(IN JUB_UINT16 deviceID,
                                     IN JUB_ENUM_CURVES curve,
                                     IN JUB_CHAR_CPTR masterPrivateKey) {

#if defined(SWI_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    if (!dynamic_cast<jub::device::JubiterSWIDevice*>(device)) {
        return JUBR_ARGUMENTS_BAD;
    }

    return dynamic_cast<jub::device::JubiterSWIDevice*>(device)->ImportMasterPrivateKey(curve, masterPrivateKey);
#else   // #if defined(SWI_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SWI_MODE) end
}


/*****************************************************************************
 * @function name : JUB_BuildFromMnemonic
 * @in  param : deviceID - device ID
 *           mnemonic - mnemonic
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_BuildFromMnemonic(IN JUB_UINT16 deviceID,
                             IN JUB_CHAR_CPTR passphrase,
                             IN JUB_CHAR_CPTR mnemonic) {

#if defined(SWI_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    if (!dynamic_cast<jub::device::JubiterSWIDevice*>(device)) {
        return JUBR_ARGUMENTS_BAD;
    }

    return dynamic_cast<jub::device::JubiterSWIDevice*>(device)->ImportMnemonic(passphrase, mnemonic);
#else   // #if defined(SWI_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(SWI_MODE) end
}
