#ifndef __BTCTokenInterface__
#define __BTCTokenInterface__

#include "JUB_SDK_BTC.h"

#include <vector>
#include <string>
#include <token/interface/BaseToken.h>
#include <mSIGNA/stdutils/uchar_vector.h>

namespace jub {
namespace token {


class BTCTokenInterface :
virtual public BaseToken {
public:
    virtual ~BTCTokenInterface() = default;

    virtual JUB_RV SelectApplet() = 0;
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) = 0;
    virtual JUB_RV SetCoinType(const JUB_ENUM_COINTYPE_BTC& type) = 0;
    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw) = 0;

    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) = 0;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt,
                              const JUB_ENUM_BTC_TRANS_TYPE& type,
                              const std::string& path,
                              const JUB_UINT16 tag,
                              std::string& address) = 0;


    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw) = 0;
}; // class BTCTokenInterface end


} // namespace token end
} // namespace jub end

#endif
