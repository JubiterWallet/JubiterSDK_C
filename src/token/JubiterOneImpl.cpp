#include <JUB_SDK.h>
#include <token/TokenInterface.hpp>
#include <token/JubiterOneImpl.h>
#include <utility/util.hpp>
#include <utility/util.h>
#include <cassert>
#include <utility/uchar_vector.h>


namespace jub {

	constexpr JUB_BYTE mainnet_p2pkh = 0x00;
	constexpr JUB_BYTE mainnet_p2sh = 0x01;
	constexpr JUB_BYTE mainnet_p2wpkh = 0x02;
	constexpr JUB_BYTE mainnet_p2wsh = 0x03;
	constexpr JUB_BYTE mainnet_p2sh_p2wpkh = 0x04;
	constexpr JUB_BYTE mainnet_p2sh_p2wsh = 0x05;

	
	JubiterOneImpl::JubiterOneImpl(std::string path)
		:_apduBuiler(std::make_shared<JubApudBuiler>()),_device(std::make_shared<device_type>()),_path(path){

	};
	JubiterOneImpl::JubiterOneImpl(device_type* device)
		:_apduBuiler(std::make_shared<JubApudBuiler>()), _device(device)
	{

	}

	JubiterOneImpl::~JubiterOneImpl() {};

	JUB_RV JubiterOneImpl::connectToken()
	{
		return _device->connect(_path);
	}


	JUB_RV JubiterOneImpl::disconnectToken()
	{
		return _device->disconnect();
	}


	JUB_RV JubiterOneImpl::getHDNode_BTC(std::string path, std::string& xpub)
	{
		SWITCH_TO_BTC_APP

		
		uchar_vector vPath;
		vPath << path;
		uchar_vector apduData = toTlv(0x08, vPath);
		APDU apdu(0x00, 0xe6, 0x00, 0x00,apduData.size(), apduData.data());

		JUB_BYTE retData[2048] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}

		xpub = (JUB_CHAR_PTR)retData;
		return JUBR_OK;
	}

	JUB_RV JubiterOneImpl::getAddress_BTC(JUB_BTC_TRANS_TYPE type,std::string path, JUB_UINT16 tag, std::string& address)
	{
		if (tag != 0x02)
		{
			SWITCH_TO_BTC_APP
		}
		uchar_vector vPath;
		vPath << path;

		uchar_vector apduData = toTlv(0x08, vPath);
		JUB_BYTE p1 = (JUB_BYTE)tag;
		JUB_BYTE sigType;
		switch (type)
		{
		case p2pkh:
			sigType = mainnet_p2pkh;
			break;
		case p2sh_p2wpkh:
			sigType = mainnet_p2sh_p2wpkh;
			break;
		default:
			return JUBR_IMPL_NOT_SUPPORT;
		}
		APDU apdu(0x00, 0xf6, p1, sigType, apduData.size(), apduData.data());

		JUB_BYTE retData[2048] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}
		address = (JUB_CHAR_PTR)retData;
		return JUBR_OK;
	}

	JUB_RV JubiterOneImpl::enumApplet(std::string& applet_list)
	{
		{
			// select main safe scope
			APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00);
			JUB_UINT16 ret = 0;
			JUB_VERIFY_RV(_sendApdu(&apdu, ret));
			if (0x9000 != ret) {
				return JUBR_TRANSMIT_DEVICE_ERROR;
			}
		}

		// send apdu, then decide which coin types supports.
		APDU apdu(0x80, 0xCB, 0x80, 0x00, 0x05,
			(const JUB_BYTE *)"\xDF\xFF\x02\x81\x06");
		// JUB_BYTE retData[1024] = { 0 };
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}

		DataChunk tlvData(retData, retData + retLen);
		auto appList = parseTlv(tlvData);


		for (auto appid : appList)
		{
			uchar_vector id(appid);
			uchar_vector mainid(PKIAID_MAIN, 8);
			if(id == mainid)
				continue;
			applet_list += id.getHex();
			applet_list += " ";
		}


		return JUBR_OK;
	}

	JUB_RV JubiterOneImpl::getDeviceCert(std::string& cert)
	{
		uchar_vector apdu_data = "A60483021318";
		APDU apdu(0x80, 0xca, 0xbf, 0x21, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 == ret)
		{
			uchar_vector vcert(retData, retData + retLen);
			cert = vcert.getHex();
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}

	JUB_RV JubiterOneImpl::sendOneApdu(const std::string& apdu, std::string& response)
	{
		uchar_vector sendApdu(apdu);
		JUB_BYTE retdata[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = { 0, };
		JUB_ULONG ulRetLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;
		JUB_RV rv = _device->sendData(sendApdu.data(), sendApdu.size(), retdata, &ulRetLen);
		if(rv !=  JUBR_OK)
			return JUBR_TRANSMIT_DEVICE_ERROR;
		uchar_vector vResponse(retdata, retdata + ulRetLen);
		response = vResponse.getHex();
		return JUBR_OK;
	}


	JUB_RV JubiterOneImpl::signTX_BTC(JUB_BTC_TRANS_TYPE type,
		JUB_UINT16 input_count,
		std::vector<JUB_UINT64> input_amount, 
		std::vector<std::string> input_path,
		std::vector<JUB_UINT16> change_index, 
		std::vector<std::string> change_path, 
		std::vector<JUB_BYTE> unsiged_trans,
		std::vector<JUB_BYTE>& raw)
	{
		//SWITCH_TO_BTC_APP


		constexpr JUB_UINT32 sendLenOnce = 230;

		JUB_BYTE sigType;
		switch (type)
		{
		case p2pkh:
			sigType = mainnet_p2pkh;
			break;
			
		case p2sh_p2wpkh:
			sigType = mainnet_p2sh_p2wpkh;
			break;
		default:
			return JUBR_IMPL_NOT_SUPPORT;
		}

		// number of input
		uchar_vector apduData;
		apduData << (JUB_BYTE)(input_count);
		// ammountTLV
		uchar_vector amountTLV;
		for (auto amount : input_amount)
		{
			amountTLV << amount;
		}
		apduData << toTlv(0x0e, amountTLV);

		//  first pack
		APDU apdu(0x00, 0xF8, 0x01, sigType, apduData.size(), apduData.data());
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret));
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}
		apduData.clear();

		// pathTLV
		uchar_vector pathLV;
		for (auto path : input_path)
		{
			pathLV << (JUB_BYTE)(path.size());
			pathLV << path;
		}

		apduData << toTlv(0x0F, pathLV);

		JUB_VERIFY_RV(_tranPack(apduData, sigType, sendLenOnce));
		apduData.clear();


		//tx TLV
		uchar_vector transTlv;
		apduData << toTlv(0x0D, unsiged_trans);

		JUB_VERIFY_RV(_tranPack(apduData, sigType, sendLenOnce));
		apduData.clear();

		//change TLV
		uchar_vector changeIndexTLV;
		uchar_vector change_LV;

		change_LV << (JUB_BYTE)change_index.size();
		for (size_t i = 0; i < change_index.size(); i++)
		{
			change_LV << (JUB_BYTE)change_index[i];
			change_LV << (JUB_BYTE)change_path[i].length();
			change_LV << change_path[i];
		}

		changeIndexTLV = toTlv(0x10, change_LV);
		apduData << changeIndexTLV;

		JUB_VERIFY_RV(_tranPack(apduData, sigType, sendLenOnce, true)); // last data.
		apduData.clear();


		//  sign transactions
		JUB_BYTE retData[2] = { 0 };
		JUB_ULONG retLen = sizeof(retData);

		apdu.SetApdu(0x00, 0x2A, 0x00, sigType, 0);
		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x6f08 == ret)
		{
			return JUBR_USER_CANCEL;
		}
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}

		// get transactions (pack by pack)
		if (2 != retLen) { // total length
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}

		JUB_UINT16 totalReadLen = ReadBE16(retData);
		DataChunk sigRawTx(totalReadLen, 0x00);

		constexpr JUB_UINT16 readLenOnce = 256;
		apdu.le = readLenOnce;
		JUB_ULONG ulRetLen = readLenOnce;

		apdu.SetApdu(0x00, 0xF9, 0x00, 0x00, 0x00);
		JUB_UINT16 ulTimes = 0;
		for (ulTimes = 0; ulTimes < (totalReadLen / readLenOnce); ulTimes++) {

			JUB_UINT16 offset = ulTimes * readLenOnce;
			apdu.p1 = offset >> 8;
			apdu.p2 = offset & 0x00ff;

			JUB_VERIFY_RV(_sendApdu(&apdu, ret, sigRawTx.data() + ulTimes * readLenOnce, &ulRetLen));
			if (0x9000 != ret) {
				return JUBR_TRANSMIT_DEVICE_ERROR;
			}
		}

		apdu.le = totalReadLen % readLenOnce;
		if (apdu.le) {
			JUB_UINT16 offset = ulTimes * readLenOnce;
			apdu.p1 = offset >> 8;
			apdu.p2 = offset & 0x00ff;

			ulRetLen = totalReadLen - ulTimes * readLenOnce;

			JUB_VERIFY_RV(_sendApdu(&apdu, ret, sigRawTx.data() + ulTimes * readLenOnce, &ulRetLen));
			if (0x9000 != ret) {
				return JUBR_TRANSMIT_DEVICE_ERROR;
			}
		}

		raw.clear();
		raw = sigRawTx;
		return JUBR_OK;

	}

	JUB_RV JubiterOneImpl::showVirtualPwd()
	{
		JUB_CHECK_NULL(_device);
		SWITCH_TO_BTC_APP

		APDU apdu(0x00, 0x29, 0x00, 0x00, 0x00);
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}

		return JUBR_OK;
	}
	JUB_RV JubiterOneImpl::verifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry)
	{

		JUB_CHECK_NULL(_device);

		// select app first 
		SWITCH_TO_BTC_APP

		DataChunk pinCoord;
		//auto pinData = buildData({ pin });

		std::transform(pinMix.begin(), pinMix.end(), std::back_inserter(pinCoord), [](const char elem) {
			return (uint8_t)(elem - 0x30);
		});

		APDU apdu(0x00, 0x20, 0x02, 0x00, pinCoord.size(), pinCoord.data());
		//APDU apdu(0x00, 0x10, 0x00, 0x00, pinCoord.size(), pinCoord.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x63C0 == (ret & 0xfff0)) {
			retry = (ret & 0xf);
			return JUBR_DEVICE_PIN_ERROR;
		}
		else if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}
		return JUBR_OK;

	}

	bool  JubiterOneImpl::isInitialize()
	{
		uchar_vector apdu_data = "DFFF028105";
		APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		auto rv = _sendApdu(&apdu, ret, retData, &retLen);
		if (rv == JUBR_OK)
		{
			if (retData[0] == 0x5a)
			{
				return true;
			}
		}
		return false;
	}

	bool   JubiterOneImpl::isBootLoader()
	{
		APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00);
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		auto rv = _sendApdu(&apdu, ret, retData, &retLen);
		if (rv == JUBR_OK && ret  == 0x6e00)
		{
			return true;
		}
		return false;
	}

	JUB_RV JubiterOneImpl::getBleVersion(JUB_BYTE ble_version[4])
	{
		uchar_vector apdu_data = "DFFF028100";
		APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 == ret)
		{
			uchar_vector version(retData, retData + retLen);
			memset(ble_version, 0x00, 4);
			memcpy(ble_version, version.getHex().c_str(), 4);
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}
	JUB_RV JubiterOneImpl::getFwVersion(JUB_BYTE fw_version[4])
	{
		uchar_vector apdu_data = "DFFF028003";
		APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 == ret)
		{
			uchar_vector version(retData, retData + retLen);
			memset(fw_version, 0x00, 4);
			memcpy(fw_version, version.getHex().c_str(), 4);
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}

	JUB_RV JubiterOneImpl::getSN(JUB_BYTE sn[24])
	{
		uchar_vector apdu_data = "DFFF028101";
		APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 == ret)
		{
			memset(sn, 0x00, 24);
			memcpy(sn, retData, 24);
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}

	JUB_RV JubiterOneImpl::getLabel(JUB_BYTE label[32])
	{
		uchar_vector apdu_data = "DFFF028104";
		APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 == ret)
		{
			memset(label, 0x00, 32);
			memcpy(label, retData, 32);
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}

	JUB_RV JubiterOneImpl::getPinRetry(JUB_BYTE& retry)
	{
		uchar_vector apdu_data = "DFFF028102";
		APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 == ret)
		{
			retry = retData[0];
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}

	JUB_RV JubiterOneImpl::setUnit_BTC(JUB_BTC_UNIT_TYPE unit)
	{
		APDU apdu(0x00, 0xfa, JUB_BYTE(unit), 0x00, 0x00);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret));
		if (0x9000 == ret)
		{
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}

	JUB_RV JubiterOneImpl::getPinMaxRetry(JUB_BYTE& max_retry)
	{
		uchar_vector apdu_data = "DFFF028103";
		APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_UINT16 ret = 0;

		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 == ret)
		{
			max_retry = retData[0];
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}

	JUB_RV JubiterOneImpl::_selectApp(const JUB_BYTE PKIAID[8]) {
		APDU apdu(0x00, 0xA4, 0x04, 0x00, 8, PKIAID);
		JUB_UINT16 ret = 0;
		JUB_BYTE retData[1024] = { 0 };
		JUB_ULONG retLen = sizeof(retData);
		JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
		if (0x9000 != ret) {
			return JUBR_TRANSMIT_DEVICE_ERROR;
		}
		
		return JUBR_OK;
	}

	JUB_RV JubiterOneImpl::_tranPack(const DataSlice &apduData, JUB_BYTE sigType,JUB_ULONG sendLenOnce, int finalData/* = false*/,int bOnce/* = false*/) {

		if (apduData.empty()) {
			return JUBR_ERROR;
		}

		JUB_UINT16 ret = 0;
		if (bOnce) {
			// one pack enough
			APDU apdu(0x00, 0xF8, 0x00, sigType, apduData.size(), apduData.data());
			JUB_VERIFY_RV(_sendApdu(&apdu, ret));
			if (0x9000 != ret) {
				return JUBR_TRANSMIT_DEVICE_ERROR;
			}

			return JUBR_OK;
		}

		// else send pack by pack
		auto nextTimes = apduData.size() / sendLenOnce;
		auto left = apduData.size() % sendLenOnce;

		// split last pack
		if (0 == left && 0 != nextTimes) {
			nextTimes--;
			left = sendLenOnce;
		}

		// pack by pack
		APDU apdu(0x00, 0xF8, 0x02, sigType, 0x00);
		apdu.lc = sendLenOnce;
		JUB_UINT32 ulTimes = 0;
		for (ulTimes = 0; ulTimes < nextTimes; ulTimes++) {
			apdu.SetData(apduData.data() + ulTimes * sendLenOnce, apdu.lc);
			JUB_VERIFY_RV(_sendApdu(&apdu, ret));
			if (0x9000 != ret) {
				return JUBR_TRANSMIT_DEVICE_ERROR;
			}
		}

		// next pack
		apdu.lc = left;
		if (apdu.lc) {
			if (finalData) {
				apdu.p1 = 0x03;
			}

			apdu.SetData(apduData.data() + ulTimes * sendLenOnce, apdu.lc);
			JUB_VERIFY_RV(_sendApdu(&apdu, ret));
			if (0x9000 != ret) {
				return JUBR_TRANSMIT_DEVICE_ERROR;
			}
		}

		return JUBR_OK;
	}



	JUB_RV JubiterOneImpl::_sendApdu(const APDU *apdu, JUB_UINT16 &wRet, JUB_BYTE *pRetData /*= nullptr*/,
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
