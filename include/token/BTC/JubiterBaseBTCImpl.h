#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>
#include <Hash.h>

namespace jub {
namespace token {


class JubiterBaseBTCImpl :
virtual public BTCTokenInterface {
public:
    JubiterBaseBTCImpl() {
        _coin = TWCoinType::TWCoinTypeBitcoin;
        _curve = TWCurve::TWCurveSECP256k1;
        _publicKeyType = TWPublicKeyType::TWPublicKeyTypeSECP256k1;
    };

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw);

protected:
    TW::Hash::Hasher _getHasherForPublicKey();

protected:
    // add curve, prefix here
    TWCoinType _coin;
    TWCurve _curve;
    TWPublicKeyType _publicKeyType;
}; // class JubiterBaseBTCImpl end


} // namespace token end
} // namespace jub end
