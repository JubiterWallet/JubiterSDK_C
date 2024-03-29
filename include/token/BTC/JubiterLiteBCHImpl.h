#pragma once
#include <memory>

#include "token/BTC/JubiterBaseBCHImpl.h"
#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterLiteBCHImpl : public JubiterLiteBTCImpl, public JubiterBaseBCHImpl {
  public:
    JubiterLiteBCHImpl(const JUB_UINT16 deviceID) : JubiterLiteBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _hashType = TWBitcoinSigHashTypeForkBCH;
    }
    ~JubiterLiteBCHImpl() {}

  protected:
    virtual JUB_RV _SignTx(bool witness, const JUB_ENUM_BTC_TRANS_TYPE &type,
                           const std::vector<JUB_UINT64> &vInputAmount, const std::vector<std::string> &vInputPath,
                           const std::vector<JUB_UINT16> &vChangeIndex, const std::vector<std::string> &vChangePath,
                           const TW::Bitcoin::Transaction &tx, std::vector<TW::Data> &vInputPublicKey,
                           std::vector<uchar_vector> &vSignatureRaw, const TWCoinType &coinNet) override;
}; // class JubiterLiteBCHImpl end

} // namespace token
} // namespace jub
