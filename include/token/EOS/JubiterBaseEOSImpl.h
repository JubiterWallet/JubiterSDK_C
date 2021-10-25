#pragma once
#include "token/interface/EOSTokenInterface.hpp"
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

    static JUB_RV IsValidAddress(const std::string& address);
    virtual JUB_RV CheckAddress(const TW::EOS::Type& type, const std::string& address) override;

    virtual nlohmann::json SerializeAction(const JUB_ACTION_EOS& action) override;
    virtual JUB_RV SerializeActions(const JUB_ACTION_EOS_PTR actions, JUB_UINT16 actionCount,
                                    std::string& actionsInJSON) override;

    virtual JUB_RV SerializePreimage(const std::string& expiration,
                                     const std::string& referenceBlockId,
                                     const std::string& referenceBlockTime,
                                     const std::string& actionsInJSON,
                                     uchar_vector& preimageRaw,
                                     const bool bWithType) override;

protected:
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseEOSImpl end


} // namespace token end
} // namespace jub end
