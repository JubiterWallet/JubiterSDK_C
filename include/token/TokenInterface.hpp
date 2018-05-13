#ifndef __TokenInterface__
#define __TokenInterface__
#include <JUB_SDK.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>

#ifdef HID_MODE // modify later..
#include <device/JubiterHidDevice.hpp>
using device_type = jub::JubiterHidDevice;
#else
#include <device/JubiterBLEDevice.hpp>
using device_type = jub::JubiterBLEDevice;
#endif


namespace jub {

class TokenInterface {
   public:
    /* functions */
    virtual JUB_RV connectToken() = 0;
	virtual JUB_RV disconnectToken() = 0;
	virtual JUB_RV getHDNode_BTC(std::string path, std::string& xpub) = 0;
	virtual JUB_RV showVirtualPwd() = 0;
	virtual bool   isInitialize() = 0;
	virtual bool   isBootLoader() = 0;
	//virtual JUB_RV getVMversion() = 0;
	virtual JUB_RV getSN(JUB_BYTE sn[25]) = 0;
	virtual JUB_RV getLabel(JUB_BYTE label[33]) = 0;
	virtual JUB_RV getPinRetry(JUB_BYTE& retry) = 0;
	virtual JUB_RV getPinMaxRetry(JUB_BYTE& max_retry) = 0;
	virtual JUB_RV verifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) = 0;
	virtual JUB_RV signTX_BTC(JUB_BTC_TRANS_TYPE type, JUB_UINT16 input_count,std::vector<JUB_UINT64> input_amount,std::vector<std::string> input_path,std::vector<JUB_UINT16> change_index,     \
		 std::vector<std::string> change_path, std::vector<JUB_BYTE> unsiged_trans, std::vector<JUB_BYTE>& raw) = 0;

	virtual JUB_RV getAddress_BTC(JUB_BTC_TRANS_TYPE type,std::string path, JUB_UINT16 bshow, std::string& address) = 0;
};

using TokenManager = Singleton<xManager<jub::TokenInterface>>;

}  // namespace jub
#endif  // __TokenInterface__
