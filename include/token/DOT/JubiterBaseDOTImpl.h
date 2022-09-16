#pragma once
#include "token/interface/DOTTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>
#include <string>

namespace jub {
namespace token {

class JubiterBaseDOTImpl : virtual public DOTTokenInterface {
  public:
    JubiterBaseDOTImpl() {
        _coin = TWCoinType::TWCoinTypePolkadot;
        //        _curve_name = (char*)ED25519_NAME;
        //        _publicKeyType = TWPublicKeyType::TWPublicKeyTypeED25519;
    };

    static JUB_RV IsValidAddress(const std::string &address, const TWCoinType &coin, const bool bTest = false);
    virtual JUB_RV CheckAddress(const std::string &address, const TWCoinType &coin, const TWCoinType &coinNet) override;

    virtual JUB_RV SerializeCall(const uint64_t network, const JUB_TX_DOT &txCall, TW::Data &call) override;

  protected:
    JUB_RV _getChainCodeFromPolkadptPath(std::string pathStr, std::string &chainCode);
    JUB_RV _getSr25519KeypairFromMasterKp(const std::string kp, std::string &derivPrv, std::string &derivPub,
                                          const std::string path, JUB_ENUM_CURVES curve);
    JUB_RV _getEd25519PrvKeyFromMasterKey(const std::string prvKey, std::string &derivPrv, std::string &derivPub,
                                          const std::string path, JUB_ENUM_CURVES curve);
    JUB_RV _getAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet);
}; // class JubiterBaseDOTImpl end

} // namespace token
} // namespace jub
