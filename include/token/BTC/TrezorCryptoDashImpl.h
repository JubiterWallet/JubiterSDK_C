#pragma once
#include <memory>

#include "token/BTC/JubiterBaseDashImpl.h"
#include "token/BTC/TrezorCryptoBTCImpl.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class TrezorCryptoDashImpl : public TrezorCryptoBTCImpl, public JubiterBaseDashImpl {
  public:
    TrezorCryptoDashImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::SECP256K1)
        : TrezorCryptoBTCImpl(deviceID, curve) {}
    ~TrezorCryptoDashImpl() {}

    virtual JUB_RV SignTX(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64> &vInputAmount, const std::vector<std::string> &vInputPath,
                          const std::vector<JUB_UINT16> &vChangeIndex, const std::vector<std::string> &vChangePath,
                          const std::vector<JUB_BYTE> &vUnsigedTrans, std::vector<JUB_BYTE> &vRaw,
                          const TWCoinType &coinNet) override;
}; // class TrezorCryptoDashImpl end

} // namespace token
} // namespace jub
