#include <JUB_SDK.h>
#include <device/JubiterBLEDevice.hpp>
#include <utility/util.hpp>
#include <bleTransmit/bleTransmit.h>
#include <utility/Singleton.h>

namespace jub {

JubiterBLEDevice::JubiterBLEDevice()
    : _param{nullptr, BLE_ReadCallBack, BLE_ScanCallBack, BLE_DiscCallBack},
      _handle(0) {

}

JubiterBLEDevice::~JubiterBLEDevice() { disconnect(); }

JUB_RV JubiterBLEDevice::connect(const std::string path) {
    // parse parms
//     unsigned char guid[] = "0000-0000-0000";
//
//     FT_BLE_ConnDev(guid, 0, &_handle, 12000);

    return JUBR_OK;
}

JUB_RV JubiterBLEDevice::disconnect() {
    if (0 != _handle &&
		IFD_SUCCESS == FT_BLE_DisConn(_handle)) {
        
        _handle = 0;
    }

    return JUBR_OK;
}

JUB_RV JubiterBLEDevice::sendData(IN JUB_BYTE_CPTR sendData,
                                  IN JUB_ULONG sendLen,
                                  OUT JUB_BYTE_PTR pRetData,
                                  INOUT JUB_ULONG_PTR pulRetLen,
                                  IN JUB_ULONG ulMiliSecondTimeout) {


    if (0 == _handle || IFD_SUCCESS != FT_BLE_IsConn(_handle)) {
        return JUBR_NOT_CONNECT_DEVICE;
    }

    FT_BLE_SendAPDU_Sync(_handle, const_cast<unsigned char*>(sendData), sendLen,
                         pRetData, (unsigned int*)pulRetLen,
                         ulMiliSecondTimeout);
    return JUBR_OK;
}

unsigned int JubiterBLEDevice::initialize(const BLE_INIT_PARAM& params) {
    
	// init with inner _param
	_param.param = params.param;
	unsigned int ret = FT_BLE_Initialize(_param);
	if (IFD_SUCCESS == ret) {
		outerParams = params;
	}

    return ret;
}

unsigned int JubiterBLEDevice::finalize() { 

	outerParams = {0, 0, 0, 0};
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
#ifndef BLE_MODE
	return IFD_NOT_SUPPORTED;
#endif // BLE_MODE
       
    // analyse data here...

    auto bleDevice = getThis();
    if (bleDevice) {
        if (!bleDevice->outerParams.callBack) {
            bleDevice->outerParams.callBack(devHandle, data, dataLen);
        }
    }

    return IFD_SUCCESS;
}

void JubiterBLEDevice::BLE_ScanCallBack(unsigned char* devName,
                                        unsigned char* uuid,
                                        unsigned int type) {
#ifndef BLE_MODE
	return ;
#endif // BLE_MODE

    auto bleDevice = getThis();
    if (bleDevice) {
        if (!bleDevice->outerParams.scanCallBack) {
            bleDevice->outerParams.scanCallBack(devName, uuid, type);
        }
    }

    return;
}

void JubiterBLEDevice::BLE_DiscCallBack(unsigned char* uuid) {
#ifndef BLE_MODE
	return ;
#endif // BLE_MODE

    auto bleDevice = getThis();
    if (bleDevice) {
        if (!bleDevice->outerParams.discCallBack) {
            bleDevice->outerParams.discCallBack(uuid);
        }
    
		bleDevice->setHandle(0);
	}

    return;
}

jub::JubiterBLEDevice* JubiterBLEDevice::getThis() {
    return Singleton<jub::JubiterBLEDevice>::GetInstance();
}

}  // namespace jub
