#include <JUB_SDK.h>

#ifdef BLE_MODE


#include <device/JubiterBLEDevice.hpp>
#include <utility/util.hpp>
#include <bleTransmit/bleTransmit.h>
//#include <logUtils.h>
#include <utility/Singleton.h>
#include <unistd.h>
#include <device/Fido.h>
#include <utils/logUtils.h>

namespace jub {

	JubiterBLEDevice::JubiterBLEDevice()
		: _param{ nullptr, BLE_ReadCallBack, BLE_ScanCallBack, BLE_DiscCallBack },
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

//        LOG_BIN("sendData", sendData, sendLen);

		if (0 == _handle || IFD_SUCCESS != FT_BLE_IsConn(_handle)) {
			return JUBR_NOT_CONNECT_DEVICE;
		}

		GetFido()->clear();

		unsigned char _sendMsg[2048] = { 0 };
		unsigned int _sendLen = (unsigned int) sizeof(_sendMsg);
		// 封装fido协议
		GetFido()->wrapFidoApdu(CMD_MSG, const_cast<unsigned char *>(sendData), sendLen, _sendMsg, &_sendLen);

		int ret = FT_BLE_SendAPDU(_handle, const_cast<unsigned char*>(_sendMsg), _sendLen);
		if (ret != 0) {
			return JUBR_OTHER_ERROR;
		}

		// 解析fido协议
        ret = JUBR_OK;
		struct timeval  mStart;         /**< 指令开始时间 */
		struct timeval end;
		// 开始计算超时
		gettimeofday(&mStart, NULL);
		// 获取指令结果
		MSGTYPE recv;
		long timeused = 0;
		while (true) {
            bool isError = GetFido()->hasError();
            if (isError) {
                ret = JUBR_CMD_ERROR;
                break;
            }

			bool isGetRecv = GetFido()->getRecv(&recv);
			if (false == isGetRecv) {
				gettimeofday(&end, NULL);

				long timeuse =
					1000000 * (end.tv_sec - mStart.tv_sec) + end.tv_usec - mStart.tv_usec;
				if (timeused >= timeuse && 0 < timeused) {
					ret = JUBR_OTHER_ERROR;
					break;
				}

				if (ulMiliSecondTimeout <= timeuse / 1000 && 0 < timeuse) {
					ret = JUBR_TRANSACT_TIMEOUT;
					break;
				}

				timeused = timeuse;
				usleep(10);
			}
			else {
				switch (recv[0]) {
				case CMD_PING:
				case CMD_MSG:
					break;
				case CMD_ERROR:
					ret = JUBR_DEVICE_BUSY;
					break;
				default:
					ret = JUBR_TRANSACT_TIMEOUT;
					break;
				}
				if (ret != 0) {
					break;
				}
				if (NULL == pRetData || NULL == pulRetLen) {
					break;
				}
				ret = GetFido()->parseFidoApdu(&recv[0], recv.size(), pRetData, (unsigned int*)pulRetLen);
				break;
			}
		}

		return ret;
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

		outerParams = { 0, 0, 0, 0 };
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
		return;
#endif // BLE_MODE

		// analyse data here...

		auto bleDevice = getThis();
		if (bleDevice) {
			GetFido()->RecvCallBack(devHandle, data, dataLen);
		}

		return IFD_SUCCESS;
	}

	void JubiterBLEDevice::BLE_ScanCallBack(unsigned char* devName,
		unsigned char* uuid,
		unsigned int type) {
#ifndef BLE_MODE
		return;
#endif // BLE_MODE

		auto bleDevice = getThis();
		if (bleDevice) {
			if (bleDevice->outerParams.scanCallBack) {
				bleDevice->outerParams.scanCallBack(devName, uuid, type);
			}
		}

		return;
	}

	void JubiterBLEDevice::BLE_DiscCallBack(unsigned char* uuid) {
#ifndef BLE_MODE
		return;
#endif // BLE_MODE

		auto bleDevice = getThis();
		if (bleDevice) {
			if (bleDevice->outerParams.discCallBack) {
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


#endif
