
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

#define SWITCH_TO_BTC_APP  do {				\
		JUB_VERIFY_RV(_selectApp(PKIAID_BTC));				\
	} while (0);                        


class HardwareTokenImpl : public TokenInterface {
   public:
    HardwareTokenImpl(std::string path);
    ~HardwareTokenImpl();

#ifdef _WIN32 // modify later..
	using device_type = JubiterHidDevice;
#else
	using device_type = JubiterBLEDevice;
#endif

   public:
    /* functions */
    virtual JUB_RV connectToken();
	virtual JUB_RV disconnectToken();
	virtual JUB_RV getHDNode_BTC(int index, std::string& xpub);

	virtual JUB_RV showVirtualPwd();
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
