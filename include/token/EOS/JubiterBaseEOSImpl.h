#pragma once
#include <token/interface/EOSTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>

namespace jub {
namespace token {


class JubiterBaseEOSImpl :
virtual public EOSTokenInterface {
public:
    JubiterBaseEOSImpl() {
        _coin = TWCoinType::TWCoinTypeEOS;
        _curve_name = (char*)SECP256K1_EOS_NAME;
    };

    virtual nlohmann::json SerializeAction(const JUB_ACTION_EOS& action);
    virtual JUB_RV SerializeActions(const JUB_ACTION_EOS_PTR actions, JUB_UINT16 actionCount,
                                    std::string& actionsInJSON);

    virtual JUB_RV SerializePreimage(const std::string& expiration,
                                     const std::string& referenceBlockId,
                                     const std::string& referenceBlockTime,
                                     const std::string& actionsInJSON,
                                     uchar_vector& preimageRaw,
                                     const bool bWithType);

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseEOSImpl end


} // namespace token end
} // namespace jub end
