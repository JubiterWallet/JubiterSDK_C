#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/HC/JubiterBaseHCImpl.h"
#include "token/HC/JubiterLiteHCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterLiteHCImpl : public JubiterLiteBTCImpl, public JubiterBaseHCImpl {
  public:
    JubiterLiteHCImpl(const JUB_UINT16 deviceID) : JubiterLiteBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _hashType = TWHcashSigHashAll;
    }
    ~JubiterLiteHCImpl() {}

    virtual JUB_RV SignTX(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64> &vInputAmount, const std::vector<std::string> &vInputPath,
                          const std::vector<JUB_UINT16> &vChangeIndex, const std::vector<std::string> &vChangePath,
                          const std::vector<JUB_BYTE> &vUnsigedTrans, std::vector<JUB_BYTE> &vRaw,
                          const TWCoinType &coinNet) override;
}; // class JubiterLiteHCImpl end

} // namespace token
} // namespace jub
