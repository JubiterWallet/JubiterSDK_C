#ifndef __BTCTokenInterface__
#define __BTCTokenInterface__

#include "JUB_SDK.h"

#include <vector>

namespace jub {

class BTCTokenInterface {

public:
    virtual JUB_RV SelectAppletBTC() = 0;
    virtual JUB_RV SetUnitBTC(JUB_BTC_UNIT_TYPE unit) = 0;
    virtual JUB_RV SetCoinTypeBTC(JUB_ENUM_COINTYPE_BTC type) = 0;
    virtual JUB_RV SignTXBTC(JUB_BTC_TRANS_TYPE type,
                             JUB_UINT16 inputCount,
                             std::vector<JUB_UINT64> vInputAmount,
                             std::vector<std::string> vInputPath,
                             std::vector<JUB_UINT16> vChangeIndex,
                             std::vector<std::string> vChangePath,
                             std::vector<JUB_BYTE> vUnsigedTrans,
                             std::vector<JUB_BYTE>& vRaw) = 0;

    virtual JUB_RV GetHDNodeBTC(JUB_BTC_TRANS_TYPE type, std::string path, std::string& xpub) = 0;
    virtual JUB_RV GetAddressBTC(JUB_BTC_TRANS_TYPE type,
                                 std::string path,
                                 JUB_UINT16 tag,
                                 std::string& address) = 0;
}; // class BTCTokenInterface end

} // namespace jub end

#endif
