
#ifndef __TokenInterface__
#define __TokenInterface__
#include <JUB_SDK.h>
#include <string>


namespace jub {

class TokenInterface {
   public:
    /* functions */
    virtual JUB_RV connectToken() = 0;
	virtual JUB_RV disconnectToken() = 0;
	virtual JUB_RV getHDNode_BTC(int index, std::string& xpub) = 0;

};

}  // namespace jub
#endif  // __TokenInterface__
