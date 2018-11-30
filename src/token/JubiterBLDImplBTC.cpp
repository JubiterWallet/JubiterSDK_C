#include <JUB_SDK.h>
#include <token/JubiterBLDImpl.h>
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

	constexpr JUB_BYTE PKIAID_BTC[16] = {
		0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x42, 0x54, 0x43, 0x01
	};


#define SWITCH_TO_BTC_APP  do {				                    \
		JUB_VERIFY_RV(_selectApp(PKIAID_BTC,16));				\
	} while (0)                                                 \



	JUB_RV JubiterBLDImpl::selectApplet_BTC()
	{
		SWITCH_TO_BTC_APP;
		return JUBR_OK;
	}

	JUB_RV JubiterBLDImpl::getHDNode_BTC(JUB_BTC_TRANS_TYPE type, std::string path, std::string& xpub)
	{
		uchar_vector vPath;
		vPath << path;
		uchar_vector apduData = toTlv(0x08, vPath);
		JUB_BYTE p2 = 0x00;
		switch (type)
		{
		case p2pkh:
			p2 = 0x00;   //xpub  format
			break;
		case p2sh_p2wpkh:
			p2 = 0x01;   //ypub  format
			break;
		default:
			break;
		}
		APDU apdu(0x00, 0xe6, 0x00, p2, apduData.size(), apduData.data());

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



	JUB_RV JubiterBLDImpl::getAddress_BTC(JUB_BTC_TRANS_TYPE type, std::string path, JUB_UINT16 tag, std::string& address)
	{
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


	JUB_RV JubiterBLDImpl::signTX_BTC(JUB_BTC_TRANS_TYPE type,
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
			amountTLV << (uint64_t)amount;
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



	JUB_RV JubiterBLDImpl::setUnit_BTC(JUB_BTC_UNIT_TYPE unit)
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

	JUB_RV JubiterBLDImpl::setForkID_BTC(JUB_UINT16 forkID)
	{
		JUB_BYTE coin_type = 0;
		if (forkID == 0x40)  //bch
		{
			coin_type = 0x01;
		}
		APDU apdu(0x00, 0xf5, coin_type, JUB_BYTE(forkID), 0x00);
		JUB_UINT16 ret = 0;
		JUB_VERIFY_RV(_sendApdu(&apdu, ret));
		if (0x9000 == ret)
		{
			return JUBR_OK;
		}

		return JUBR_ERROR;
	}


}