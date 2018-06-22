
#ifndef __HardwareTokenImpl__
#define __HardwareTokenImpl__

#include <JUB_SDK.h>
#include <token/TokenInterface.hpp>
#include <device/ApduBuilder.hpp>
#include <device/JubiterHidDevice.hpp>
#include <memory>
#include <utility/util.hpp>

namespace jub {

	constexpr JUB_BYTE PKIAID_BTC[8] = {
		0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x25, 0x01
	};

	constexpr JUB_BYTE PKIAID_MAIN[8] = {
		0xa0, 0x00,	0x00, 0x06, 0x47, 0x2f, 0x00, 0x01
	};

#define SWITCH_TO_BTC_APP  do {				\
		JUB_VERIFY_RV(_selectApp(PKIAID_BTC));				\
	} while (0);                        


class JubiterOneImpl : public TokenInterface {
   public:
	   JubiterOneImpl(std::string path);
	   JubiterOneImpl(device_type* device);
    ~JubiterOneImpl();

   public:
    /* functions */
    virtual JUB_RV connectToken();
	virtual JUB_RV disconnectToken();
	virtual JUB_RV getHDNode_BTC(std::string path, std::string& xpub);
	virtual JUB_RV getAddress_BTC(JUB_BTC_TRANS_TYPE type,std::string path, JUB_UINT16 tag, std::string& address);

	virtual JUB_RV showVirtualPwd();
	virtual bool   isInitialize();
	virtual bool   isBootLoader();
	virtual JUB_RV getSN(JUB_BYTE sn[24]);
	virtual JUB_RV getLabel(JUB_BYTE label[32]);
	virtual JUB_RV getPinRetry(JUB_BYTE& retry);
	virtual JUB_RV getPinMaxRetry(JUB_BYTE& max_retry);
	virtual JUB_RV getBleVersion(JUB_BYTE ble_version[4]);
	virtual JUB_RV getFwVersion(JUB_BYTE fw_version[4]);
	virtual JUB_RV setUnit_BTC(JUB_BTC_UNIT_TYPE unit);
	virtual JUB_RV enumApplet(std::string& applet_list);
	virtual JUB_RV getDeviceCert(std::string& cert);
	virtual JUB_RV sendOneApdu(const std::string& apdu, std::string& response);


	virtual JUB_RV verifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry);
	virtual JUB_RV signTX_BTC(JUB_BTC_TRANS_TYPE type, JUB_UINT16 input_count, 
		std::vector<JUB_UINT64> input_amount, 
		std::vector<std::string> input_path, 
		std::vector<JUB_UINT16> change_index, 
		std::vector<std::string> change_path, 
		std::vector<JUB_BYTE> unsiged_trans,
		std::vector<JUB_BYTE>& raw
		);

	private:

	JUB_RV _selectApp(const JUB_BYTE PKIAID[8]);

	JUB_RV _tranPack(const DataSlice &apduData,
		JUB_BYTE sigType,
		JUB_ULONG sendLenOnce,
		int finalData = false,
		int bOnce = false);

	JUB_RV _sendApdu(const APDU *apdu, JUB_UINT16 &wRet,
		JUB_BYTE *pRetData = nullptr,
		JUB_ULONG *pulRetLen = nullptr,
		JUB_ULONG ulMiliSecondTimeout = 1200000); 

    std::shared_ptr<ApduBuilder> _apduBuiler;
    std::shared_ptr<device_type> _device;
	std::string _path;

};
}  // namespace jub
#endif  // __HardwareTokenImpl__
