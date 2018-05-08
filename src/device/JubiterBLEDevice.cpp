
#include <jub/device/DeviceManager.hpp>
#include <jub/device/JubiterBLEDevice.hpp>
#include <jub/utility/util.hpp>
#include <bleTransmit/bleTransmit.h>

namespace jub {

JubiterBLEDevice::JubiterBLEDevice()
    : _param{nullptr, BLE_ReadCallBack, BLE_ScanCallBack, BLE_DiscCallBack},
      _handle(0) {
    _status = FT_BLE_Initialize(_param);
}

JubiterBLEDevice::~JubiterBLEDevice() { disconnect(); }

JUB_RV JubiterBLEDevice::connect(const std::string& params /* = ""*/) {
    // parse parms
    //     unsigned char guid[] = "0000-0000-0000";
    //
    //     FT_BLE_ConnDev(guid, 0, &_handle, 12000);

    return JUBR_OK;
}

JUB_RV JubiterBLEDevice::disconnect() {
    if (0 != _handle) {
        FT_BLE_DisConn(_handle);
        _handle = 0;
    }

    return JUBR_OK;
}

JUB_RV JubiterBLEDevice::sendData(IN JUB_BYTE_CPTR sendData,
                                  IN JUB_ULONG sendLen,
                                  OUT JUB_BYTE_PTR pRetData,
                                  INOUT JUB_ULONG_PTR pulRetLen,
                                  IN JUB_ULONG ulMiliSecondTimeout) {
    if (0 == FT_BLE_IsConn(_handle)) {
        return JUBR_NOT_CONNECT_DEVICE;
    }

    FT_BLE_SendAPDU_Sync(_handle, const_cast<unsigned char*>(sendData), sendLen,
                         pRetData, (unsigned int*)pulRetLen,
                         ulMiliSecondTimeout);
    return JUBR_OK;
}

unsigned int JubiterBLEDevice::initialize(const BLE_INIT_PARAM& params) {
    
	outerParams = params;
    return JUBR_OK;
}

unsigned int JubiterBLEDevice::finalize() { 

	return FT_BLE_Finalize(); 
}

unsigned int JubiterBLEDevice::scan() { 

	return FT_BLE_Scan(); 
}

unsigned int JubiterBLEDevice::stopScan() { 

	return FT_BLE_StopScan(); 
}

unsigned int JubiterBLEDevice::connect(unsigned char* bBLEUUID,
                                       unsigned int connectType,
                                       unsigned long* pDevHandle,
                                       unsigned int timeout) {
    unsigned int ret =
        FT_BLE_ConnDev(bBLEUUID, connectType, pDevHandle, timeout);
    if (IFD_SUCCESS == ret) {
        _handle = *pDevHandle;
    }

    return ret;
}

unsigned int JubiterBLEDevice::cancelConnect(unsigned char* bBLEUUID) {

	unsigned int ret = FT_BLE_CancelConnDev(bBLEUUID);
	_handle = 0;

	return ret;
}

unsigned int JubiterBLEDevice::disconnect(unsigned long handle) { 

	unsigned int ret = FT_BLE_DisConn(handle);

	if (IFD_SUCCESS == ret) {
		_handle = 0;
	}

	return ret;
}

unsigned int JubiterBLEDevice::isConnect(unsigned long handle) { 

	return FT_BLE_IsConn(handle);
}

unsigned long JubiterBLEDevice::getHandle() { 

	return _handle; 
}

void JubiterBLEDevice::setHandle(unsigned long handle) {

	_handle = handle;
}

int JubiterBLEDevice::BLE_ReadCallBack(unsigned long devHandle,
                                       unsigned char* data,
                                       unsigned int dataLen) {
    if (!(IS_BLE_MODE)) {
        return IFD_NOT_SUPPORTED;
    }

    // analyse data here...

    auto bleDevice = getThis();
    if (bleDevice) {
        if (!bleDevice->outerParams.callBack) {
            bleDevice->outerParams.callBack(devHandle, data, dataLen);
        }
    }

    return 0;
}

void JubiterBLEDevice::BLE_ScanCallBack(unsigned char* devName,
                                        unsigned char* uuid,
                                        unsigned int type) {
    if (!(IS_BLE_MODE)) {
        return;
    }

    auto bleDevice = getThis();
    if (bleDevice) {
        if (!bleDevice->outerParams.scanCallBack) {
            bleDevice->outerParams.scanCallBack(devName, uuid, type);
        }
    }

    return;
}

void JubiterBLEDevice::BLE_DiscCallBack(unsigned char* uuid) {
    if (!(IS_BLE_MODE)) {
        return;
    }

    auto bleDevice = getThis();
    if (bleDevice) {
        if (!bleDevice->outerParams.discCallBack) {
            bleDevice->outerParams.discCallBack(uuid);
        }
    
		bleDevice->setHandle(0);
	}

    return;
}

std::shared_ptr<jub::JubiterBLEDevice> JubiterBLEDevice::getThis() {
    return std::static_pointer_cast<jub::JubiterBLEDevice>(
        jub::DeviceManager::getDevice());
}

}  // namespace jub
