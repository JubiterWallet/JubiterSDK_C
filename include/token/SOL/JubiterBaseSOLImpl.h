#pragma once
#include "token/interface/SOLTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>
#include <string>
#include <sys/types.h>

namespace jub {
namespace token {

class JubiterBaseSOLImpl : virtual public SOLTokenInterface {
  public:
    JubiterBaseSOLImpl() {
        _coin       = TWCoinType::TWCoinTypeSolana;
        _curve_name = (char *)ED25519_NAME;
    };
    virtual JUB_RV VerifyTx(const uchar_vector &signedRaw, const TW::Data &publicKey);
    virtual JUB_RV CheckAddress(const std::string &address) override;

  protected:
    virtual JUB_RV _getAddress(const TW::Data &publicKey, std::string &address);
}; // class JubiterBaseSOLImpl end

} // namespace token
} // namespace jub
