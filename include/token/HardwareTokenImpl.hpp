
#ifndef __HardwareTokenImpl__
#define __HardwareTokenImpl__

#include <JUB_SDK.h>
#include <token/TokenInterface.hpp>
#include <device/ApduBuilder.hpp>
#include <device/JubiterHidDevice.hpp>
#include <memory>

namespace jub {

	constexpr JUB_BYTE PKIAID_BTC[8] = {
		0xA0, 0x00, 0x00, 0x06, 0x47, 0x2F, 0x00, 0x01
	};

#define SWITCH_TO_BTC_APP  do {				\
		_selectApp(PKIAID_BTC);         \
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

	private:

	JUB_RV _selectApp(const JUB_BYTE PKIAID[8]);

	JUB_RV _sendApdu(const APDU *apdu, JUB_UINT16 &wRet,
		JUB_BYTE *pRetData = nullptr,
		JUB_ULONG *pulRetLen = nullptr,
		JUB_ULONG ulMiliSecondTimeout = 120000);

    std::shared_ptr<ApduBuilder> _apduBuiler;
    std::shared_ptr<device_type> _device;
	std::string _path;

};
}  // namespace jub
#endif  // __HardwareTokenImpl__
