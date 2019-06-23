
#ifndef __HardwareTokenImpl__
#define __HardwareTokenImpl__

#include <JUB_SDK.h>
#include <token/interface/TokenInterface.hpp>
#include <device/ApduBuilder.hpp>
#include <device/JubiterHidDevice.hpp>
#include <memory>
#include <utility/util.hpp>


#ifdef HID_MODE // modify later..
#include <device/JubiterHidDevice.hpp>
using device_type = jub::JubiterHidDevice;
#else
#include <device/JubiterBLEDevice.hpp>
using device_type = jub::JubiterBLEDevice;
#endif




namespace jub {

	constexpr JUB_BYTE PKIAID_FIDO[8] = {
		0xa0, 0x00,	0x00, 0x06, 0x47, 0x2f, 0x00, 0x01
	};


	typedef struct _stAppInfos_ {
		DataChunk appId;
		std::string coinName;
		std::string minimumAppletVersion;
	} stAppInfos;


class JubiterBLDImpl : public TokenInterface {
   public:
	   JubiterBLDImpl(std::string path);
	   JubiterBLDImpl(device_type* device);
    ~JubiterBLDImpl();

   public:
    /* functions */
    virtual JUB_RV connectToken();
	virtual JUB_RV disconnectToken();

	//BTC functions
	virtual JUB_RV selectApplet_BTC();
	virtual JUB_RV getHDNode_BTC(JUB_BTC_TRANS_TYPE type, std::string path, std::string& xpub);
	virtual JUB_RV getAddress_BTC(JUB_BTC_TRANS_TYPE type,std::string path, JUB_UINT16 tag, std::string& address);
	virtual JUB_RV setUnit_BTC(JUB_BTC_UNIT_TYPE unit);
	virtual JUB_RV setCoinType_BTC(JUB_ENUM_COINTYPE_BTC type);
	virtual JUB_RV signTX_BTC(JUB_BTC_TRANS_TYPE type, JUB_UINT16 input_count,
		std::vector<JUB_UINT64> input_amount,
		std::vector<std::string> input_path,
		std::vector<JUB_UINT16> change_index,
		std::vector<std::string> change_path,
		std::vector<JUB_BYTE> unsiged_trans,
		std::vector<JUB_BYTE>& raw
	);

	//ETH functions

	virtual JUB_RV selectApplet_ETH();
	virtual JUB_RV getAddress_ETH(std::string path, JUB_UINT16 tag, std::string& address);
	virtual JUB_RV getHDNode_ETH(JUB_BYTE format,std::string path, std::string& pubkey);
	virtual JUB_RV signTX_ETH(bool Is_ERC20, std::vector<JUB_BYTE> nonce, 
		std::vector<JUB_BYTE> gasPrice,
		std::vector<JUB_BYTE> gasLimit,
		std::vector<JUB_BYTE> To, 
		std::vector<JUB_BYTE> value, 
		std::vector<JUB_BYTE> data, 
		std::vector<JUB_BYTE> path, 
		std::vector<JUB_BYTE> chainID,
		std::vector<JUB_BYTE>& raw);



	//common token functions
	virtual JUB_RV queryBattery(JUB_BYTE &percent);
	virtual JUB_RV showVirtualPwd();
	virtual JUB_RV cancelVirtualPwd();
	virtual bool   isInitialize();
	virtual bool   isBootLoader();
	virtual JUB_RV getSN(JUB_BYTE sn[24]);
	virtual JUB_RV getLabel(JUB_BYTE label[32]);
	virtual JUB_RV getPinRetry(JUB_BYTE& retry);
	virtual JUB_RV getPinMaxRetry(JUB_BYTE& max_retry);
	virtual JUB_RV getBleVersion(JUB_BYTE ble_version[4]);
	virtual JUB_RV getFwVersion(JUB_BYTE fw_version[4]);

	virtual JUB_RV enumApplet(std::string& applet_list);
	virtual JUB_RV getAppletVersion(std::string appID, std::string& version);
	virtual JUB_RV enumSupportCoins(std::string& coinList);
	virtual JUB_RV getDeviceCert(std::string& cert);
	virtual JUB_RV sendOneApdu(const std::string& apdu, std::string& response);


	virtual JUB_RV verifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry);

	virtual JUB_RV setTimeout(JUB_UINT16 timeout);


	// get function
	std::string getPath() { return _path; };
	static stAppInfos g_appinfo[];


	private:

	JUB_RV _selectApp(const JUB_BYTE PKIAID[],JUB_BYTE length);

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
