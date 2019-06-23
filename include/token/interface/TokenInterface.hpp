#ifndef __TokenInterface__
#define __TokenInterface__
#include <JUB_SDK.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>
#include "BTCTokenInterface.hpp"
#include "ETHTokenInterface.hpp"



namespace jub {

class TokenInterface:public BTCTokenInterface,public ETHTokenInterface {
   public:
    /* functions */
    virtual JUB_RV connectToken() = 0;
	virtual JUB_RV disconnectToken() = 0;
	virtual JUB_RV showVirtualPwd() = 0;
	virtual JUB_RV cancelVirtualPwd() = 0;
	virtual bool   isInitialize() = 0;
	virtual bool   isBootLoader() = 0;
	virtual JUB_RV getSN(JUB_BYTE sn[24]) = 0;
	virtual JUB_RV getLabel(JUB_BYTE label[32]) = 0;
	virtual JUB_RV getPinRetry(JUB_BYTE& retry) = 0;
	virtual JUB_RV getPinMaxRetry(JUB_BYTE& max_retry) = 0;
	virtual JUB_RV getBleVersion(JUB_BYTE ble_version[4]) = 0;
	virtual JUB_RV getFwVersion(JUB_BYTE fw_version[4]) = 0;
	virtual JUB_RV enumApplet(std::string& applet_list) = 0;
	virtual JUB_RV getAppletVersion(std::string appID, std::string& version) = 0;
	virtual JUB_RV enumSupportCoins(std::string& coinList) = 0;
	virtual JUB_RV getDeviceCert(std::string& cert) = 0;
	virtual JUB_RV sendOneApdu(const std::string& apdu, std::string& response) = 0;

	virtual JUB_RV queryBattery(JUB_BYTE &percent) = 0;

	virtual JUB_RV verifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) = 0;
	virtual JUB_RV setTimeout(JUB_UINT16 timeout) = 0;
};

using TokenManager = Singleton<xManager<jub::TokenInterface>>;

}  // namespace jub
#endif  // __TokenInterface__
