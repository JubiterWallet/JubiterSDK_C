#pragma once
#include "token/interface/FILTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>
#include <string>

namespace jub {
namespace token {

class JubiterBaseFILImpl : public FILTokenInterface {
  public:
    JubiterBaseFILImpl() {
        _coin       = TWCoinType::TWCoinTypeFilecoin;
        _curve_name = (char *)SECP256K1_FIL_NAME;
    };

    static JUB_RV IsValidAddress(const std::string &address);
    virtual JUB_RV CheckAddress(const std::string &address) override;

  protected:
    virtual JUB_RV _getAddress(const TW::Data &publicKey, std::string &address);
}; // class JubiterBaseFILImpl end

} // namespace token
} // namespace jub
