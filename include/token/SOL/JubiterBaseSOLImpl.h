#pragma once
#include <sys/types.h>
#include "token/interface/SOLTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>
#include <string>
#include <vector>


namespace jub {
namespace token {

class JubiterBaseSOLImpl :
virtual public SOLTokenInterface {
public:
    JubiterBaseSOLImpl() {
        _coin = TWCoinType::TWCoinTypeSolana;
        _curve_name = (char*)ED25519_NAME;
    };

    virtual JUB_RV VerifyTx(const uchar_vector& signedRaw,
                            const TW::Data& publicKey);

protected:
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseSOLImpl end


} // namespace token end
} // namespace jub end
