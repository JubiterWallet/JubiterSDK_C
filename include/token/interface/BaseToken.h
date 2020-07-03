#pragma once
#include "JUB_SDK_COMM.h"
#include <TrezorCrypto/curves.h>
#include <TrustWalletCore/TWCoinType.h>

namespace jub {
namespace token {


class BaseToken {
public:
    virtual ~BaseToken() {};

protected:
    TWCoinType _coin;
    JUB_ENUM_NETTYPE _net;
    char *_curve_name = (char*)SECP256K1_NAME;
    TWPublicKeyType _publicKeyType = TWPublicKeyType::TWPublicKeyTypeSECP256k1;
}; // class BaseToken end


} // namespace token end
} // namespace jub end
