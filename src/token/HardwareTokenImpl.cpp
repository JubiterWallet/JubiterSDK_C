#include <JUB_SDK.h>
#include <token/TokenInterface.hpp>
#include <token/HardwareTokenImpl.hpp>
#include <utility/util.hpp>
#include <cassert>


namespace jub {

	HardwareTokenImpl::HardwareTokenImpl(std::string path) 
		:_apduBuiler(std::make_shared<JubApudBuiler>()),_device(std::make_shared<device_type>()),_path(path){

	};
	HardwareTokenImpl::~HardwareTokenImpl() {};

	JUB_RV HardwareTokenImpl::connectToken()
	{
		return _device->connect(_path);
	}


	JUB_RV HardwareTokenImpl::disconnectToken()
	{
		return _device->disconnect();
	}


	JUB_RV HardwareTokenImpl::getHDNode_BTC(int index, std::string& xpub)
	{
		SWITCH_TO_BTC_APP



		return JUBR_OK;
	}


	JUB_RV HardwareTokenImpl::signTX_BTC(JUB_BTC_TRANS_TYPE type, 
		JUB_UINT16 input_count,
		std::vector<JUB_UINT64> input_amount, 
		std::vector<std::string> input_path,
		std::vector<JUB_UINT16> change_index, 
		std::vector<std::string> change_path, 
		std::vector<JUB_BYTE> unsiged_trans)
	{
		SWITCH_TO_BTC_APP

		return JUBR_OK;

	}


	JUB_RV HardwareTokenImpl::_selectApp(const JUB_BYTE PKIAID[8]) {
		APDU apdu(0x00, 0xA4, 0x04, 0x00, sizeof(PKIAID), PKIAID);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret));
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}
		
		return JUBR_OK;
	}



	JUB_RV HardwareTokenImpl::_sendApdu(const APDU *apdu, JUB_UINT16 &wRet, JUB_BYTE *pRetData /*= nullptr*/,
		JUB_ULONG *pulRetLen /*= nullptr*/,
		JUB_ULONG ulMiliSecondTimeout /*= 0*/) {

		JUB_CHECK_NULL(_apduBuiler);
		JUB_CHECK_NULL(_device);

		JUB_BYTE retdata[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = { 0, };
		JUB_ULONG ulRetLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;

		std::vector<JUB_BYTE> sendApdu;
		if (JUBR_OK == _apduBuiler->buildApdu(apdu, sendApdu))
		{
			if (JUBR_OK != _device->sendData(sendApdu.data(), sendApdu.size(), retdata, &ulRetLen, ulMiliSecondTimeout))
			{
				return JUBR_TRANSMIT_DEVICE_ERROR;
			}

			if (NULL == pulRetLen)
			{
				wRet = retdata[ulRetLen - 2] * 0x100 + retdata[ulRetLen - 1];
				return JUBR_OK;
			}

			if (NULL == pRetData)
			{
				*pulRetLen = ulRetLen - 2;
				wRet = (retdata[ulRetLen - 2] * 0x100 + retdata[ulRetLen - 1]);
				return JUBR_OK;
			}

			if (*pulRetLen < (ulRetLen - 2))
			{
				*pulRetLen = ulRetLen - 2;
				return JUBR_BUFFER_TOO_SMALL;
			}

			*pulRetLen = ulRetLen - 2;
			memcpy(pRetData, retdata, ulRetLen - 2);

			wRet = retdata[ulRetLen - 2] * 0x100 + retdata[ulRetLen - 1];
			return JUBR_OK;
		}

		return JUBR_TRANSMIT_DEVICE_ERROR;
	}



}  // namespace jub
