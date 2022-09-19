#pragma once
#include "token/interface/XRPTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>
#include <string>

namespace jub {
namespace token {

class JubiterBaseXRPImpl : public XRPTokenInterface {
  public:
    JubiterBaseXRPImpl() {
        _coin       = TWCoinType::TWCoinTypeXRP;
        _curve_name = (char *)SECP256K1_XRP_NAME;
    };

    static JUB_RV IsValidAddress(const std::string &address, std::string &rAddress, std::string &tagged);
    virtual JUB_RV CheckAddress(const std::string &address, std::string &rAddress, std::string &tagged) override;

    JUB_RV SerializePreimage(const JUB_TX_XRP &tx, uchar_vector &preimageRaw) override;

  protected:
    virtual JUB_RV _getAddress(const TW::Data &publicKey, std::string &address);
}; // class JubiterBaseXRPImpl end

} // namespace token
} // namespace jub
