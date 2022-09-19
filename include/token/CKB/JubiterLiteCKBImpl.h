#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/CKB/JubiterBaseCKBImpl.h"
#include "token/CKB/JubiterLiteCKBImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterLiteCKBImpl : public JubiterLiteImpl, public JubiterBaseCKBImpl {
  public:
    JubiterLiteCKBImpl(const JUB_UINT16 deviceID) : JubiterLiteImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterLiteCKBImpl() {}

    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE &unit) override;
    virtual JUB_RV SetCoin(const JUB_ENUM_COINTYPE_BTC &type) override;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path,
                              const JUB_UINT16 tag, std::string &address,
                              const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path, std::string &pubkey,
                             const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin) override;

  protected:
    virtual JUB_RV _SignTX(const std::vector<std::string> &vInputPath, TW::NervosCKB::Transaction &tx,
                           const TWCoinType &coinNet) override;
    virtual JUB_RV _VerifyTX(const std::vector<std::string> &vInputPath, const TW::NervosCKB::Transaction &tx,
                             const std::vector<uchar_vector> &vSignatureRaw, const TWCoinType &coinNet) override;
}; // class JubiterLiteCKBImpl end

} // namespace token
} // namespace jub
