#include <device/JubiterBLEDevice.hpp>
#include <utility/uchar_vector.h>

#ifdef BLE_MODE
#include <unistd.h>
#include <sys/time.h>

//#include <device/DeviceManager.hpp>
#include <utility/util.hpp>
#include <bleTransmit/bleTransmit.h>
#include <utility/Singleton.h>
#include <device/JubiterBLEDevice.hpp>
//#include <logUtils.h>
#include <device/Fido.h>

#ifdef __ANDROID__
#include <bleTransmit/android/BTManager.h>
#else
#include "ErrorCodesAndMacros.h"
#include "BLEInterface.h"
#endif


namespace jub {

JubiterBLEDevice::JubiterBLEDevice()
    : _param{nullptr, BLE_ReadCallBack, BLE_ScanCallBack, BLE_DiscCallBack},
      _handle(0), _connected(false) {

}

JubiterBLEDevice::~JubiterBLEDevice() { disconnect(); }


JUB_RV JubiterBLEDevice::matchErrorCode(int error) {
#ifdef __ANDROID__
    switch (error) {
        case BT_SUCCESS:
            return JUBR_OK;
        case BT_ERR_CONNECT_FAIL:
            return JUBR_CONNECT_DEVICE_ERROR;
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
        case CKR_OK:
            return JUBR_OK;
        case CKR_BLE_BOND_FAIL:
            return JUBR_BT_BOND_FAILED;
        case CKR_BLE_CONNECT_FAIL:
            return JUBR_CONNECT_DEVICE_ERROR;
        default:
            return JUBR_TRANSMIT_DEVICE_ERROR;
    } // switch ((unsigned long)error) end
#endif
}

JUB_RV JubiterBLEDevice::connect(const std::string params /* = ""*/) {
    // parse parms
//     unsigned char guid[] = "0000-0000-0000";
//
//     FT_BLE_ConnDev(guid, 0, &_handle, 12000);

    return JUBR_OK;
}

JUB_RV JubiterBLEDevice::disconnect() {
    if (             0 != _handle
        && IFD_SUCCESS == FT_BLE_DisConn(_handle)
        ) {

        _handle = 0;
    }

    return JUBR_OK;
}

JUB_RV JubiterBLEDevice::sendData(IN JUB_BYTE_CPTR sendData,
                                  IN JUB_ULONG sendLen,
                                  OUT JUB_BYTE_PTR pRetData,
                                  INOUT JUB_ULONG_PTR pulRetLen,
                                  IN JUB_ULONG ulMiliSecondTimeout) {

    if (    0 == _handle
        || !FT_BLE_IsConn(_handle)
        ) {
        return JUBR_NOT_CONNECT_DEVICE;
    }

    auto& fido = Fido::instance();
    fido.clear();

    unsigned char _sendMsg[2048] = {0,};
    unsigned int _sendLen = (unsigned int) sizeof(_sendMsg);
    // 封装fido协议
    fido.wrapFidoApdu(CMD_MSG, const_cast<unsigned char *>(sendData), sendLen, _sendMsg, &_sendLen);

    int ret = FT_BLE_SendAPDU(_handle, const_cast<unsigned char *>(_sendMsg), _sendLen);
    if (ret != 0) {
        return matchErrorCode(ret);
    }

    auto status = fido.waitForReceive(ulMiliSecondTimeout);
    if (!_connected) { return JUBR_NOT_CONNECT_DEVICE; };
    if (status == std::cv_status::timeout) {
        return JUBR_TRANSACT_TIMEOUT;
    }

    auto respStatus = fido.checkResponse();
    switch (respStatus) {
        case Fido::ResponseStatus::empty:
            return JUBR_TRANSMIT_DEVICE_ERROR;
        case Fido::ResponseStatus::invalid:
            return JUBR_TRANSMIT_DEVICE_ERROR;
        case Fido::ResponseStatus::receiving:
            return JUBR_TRANSACT_TIMEOUT;
        case Fido::ResponseStatus::done:
            break;
    } // switch (respStatus) end

    auto resp = fido.response();
    if (   NULL == pRetData
        || NULL == pulRetLen
        ) {
        return JUBR_OK;
    }

    ret = fido.parseFidoApdu(resp.data(), resp.size(), pRetData, (unsigned int *) pulRetLen);

    return matchErrorCode(ret);
}

unsigned int JubiterBLEDevice::initialize(const BLE_INIT_PARAM& params) {
    
	// init with inner _param
	_param.param = params.param;
	unsigned int ret = FT_BLE_Initialize(_param);
	if (IFD_SUCCESS == ret) {
		outerParams = params;
	}

    return matchErrorCode(ret);
}

unsigned int JubiterBLEDevice::finalize() { 

	outerParams = {0, 0, 0, 0};
	return FT_BLE_Finalize(); 
}

unsigned int JubiterBLEDevice::scan() { 

	return matchErrorCode(FT_BLE_Scan());
}

unsigned int JubiterBLEDevice::stopScan() { 

	return matchErrorCode(FT_BLE_StopScan());
}

unsigned int JubiterBLEDevice::connect(unsigned char* bBLEUUID,
                                       unsigned int connectType,
                                       unsigned long* pDevHandle,
                                       unsigned int timeout) {
    unsigned int ret =
        FT_BLE_ConnDev(bBLEUUID, connectType, pDevHandle, timeout);
    if (IFD_SUCCESS == ret) {
        _handle = *pDevHandle;
        _connected = true;
    }
    extraSetting();
    return matchErrorCode(ret);
}

unsigned int JubiterBLEDevice::cancelConnect(unsigned char* bBLEUUID) {

	unsigned int ret = FT_BLE_CancelConnDev(bBLEUUID);
	_handle = 0;
    _connected = false;

	return matchErrorCode(ret);
}

unsigned int JubiterBLEDevice::disconnect(unsigned long handle) { 

	unsigned int ret = FT_BLE_DisConn(handle);

	if (IFD_SUCCESS == ret) {
		_handle = 0;
        _connected = false;
	}

	return matchErrorCode(ret);
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

void JubiterBLEDevice::setConnectStatuteFalse(){
    _connected = false;
}

int JubiterBLEDevice::BLE_ReadCallBack(unsigned long devHandle,
                                       unsigned char* data,
                                       unsigned int dataLen) {
//    if (!(IS_BLE_MODE)) {
//        return IFD_NOT_SUPPORTED;
//    }

    // analyse data here...

     auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (bleDevice) {
        Fido::RecvCallBack(devHandle, data, dataLen);
    }

    return IFD_SUCCESS;
}

void JubiterBLEDevice::BLE_ScanCallBack(unsigned char* devName,
                                        unsigned char* uuid,
                                        unsigned int type) {
//    if (!(IS_BLE_MODE)) {
//        return;
//    }

     auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (bleDevice) {
        if (bleDevice->outerParams.scanCallBack) {
            bleDevice->outerParams.scanCallBack(devName, uuid, type);
        }
    }

    return;
}

void JubiterBLEDevice::BLE_DiscCallBack(unsigned char* uuid) {
//    if (!(IS_BLE_MODE)) {
//        return;
//    }

    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (bleDevice) {
		bleDevice->setHandle(0);
        bleDevice->setConnectStatuteFalse();
        Fido::instance().stopReceiving();
        if (bleDevice->outerParams.discCallBack) {
            bleDevice->outerParams.discCallBack(uuid);
        }
	}
    return;
}

void JubiterBLEDevice::extraSetting() {
#if defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_IOS // ios
    FT_BLESetIsReConnected(false);
    // cmd: 00A40400
    uchar_vector cmd("00A40400");
    unsigned char resp[0x100] = {0,};
    JUB_ULONG respLen = sizeof(resp);
    auto rv = sendData(cmd.data(), cmd.size(), resp, &respLen);
    if (rv != JUBR_OK) { return; }
    if (respLen < 2) { return; }
    JUB_RV ret = 0;
    ret = resp[respLen - 2] * 0x100 + resp[respLen - 1];
    if (ret != 0x9000) { return;}

    // cmd:  80CB800005DFFF028100
    // resp: 2003D86A571205504A7542697465722D71686A769000
    cmd.setHex("80CB800005DFFF028100");
    respLen = sizeof(resp);
    rv = sendData(cmd.data(), cmd.size(), resp, &respLen);
    if ( rv != JUBR_OK) { return; }
    if ( respLen < 4) { return; }
    ret = resp[respLen - 2] * 0x100;
    ret += resp[respLen - 1];
    if (ret != 0x9000) { return; }
    if (   resp[0] != 0x20
        || resp[1] != 0x03
        ) {
        return;
    }

    FT_BLESetIsReConnected(true);
#endif // #if TARGET_OS_OSX
#endif
}

}  // namespace jub

#endif // USE_BLE_DEVICE
