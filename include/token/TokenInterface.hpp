
#ifndef __TokenInterface__
#define __TokenInterface__
#include <JUB_SDK.h>
#include <string>
#include <vector>


namespace jub {

class TokenInterface {
   public:
    /* functions */
    virtual JUB_RV connectToken() = 0;
	virtual JUB_RV disconnectToken() = 0;
	virtual JUB_RV getHDNode_BTC(int index, std::string& xpub) = 0;
	virtual JUB_RV showVirtualPwd() = 0;
	virtual JUB_RV verifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) = 0;
	virtual JUB_RV signTX_BTC(JUB_BTC_TRANS_TYPE type, JUB_UINT16 input_count,std::vector<JUB_UINT64> input_amount,std::vector<std::string> input_path,std::vector<JUB_UINT16> change_index,     \
		 std::vector<std::string> change_path, std::vector<JUB_BYTE> unsiged_trans, std::vector<JUB_BYTE>& raw) = 0;

};

}  // namespace jub
#endif  // __TokenInterface__
