#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <Hash.h>

namespace jub {
namespace token {


class JubiterBaseBTCImpl :
virtual public BTCTokenInterface {
public:
    JubiterBaseBTCImpl() {
        // TWBitcoinSigHashType::TWBitcoinSigHashTypeAll|_forkID for BCH
    };

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw);

    virtual JUB_RV VerifyTx(const bool witness,
                            const uchar_vector& signedRaw,
                            const std::vector<JUB_UINT64>& vInputAmount,
                            const std::vector<TW::Data>& vInputPublicKey);

protected:
    TW::Hash::Hasher _getHasherForPublicKey();

protected:
    // add curve, prefix here
    TWCoinType _coin = TWCoinType::TWCoinTypeBitcoin;
    TWCurve _curve = TWCurve::TWCurveSECP256k1;
    TWPublicKeyType _publicKeyType = TWPublicKeyType::TWPublicKeyTypeSECP256k1;
    TWBitcoinSigHashType _hashType = TWBitcoinSigHashType::TWBitcoinSigHashTypeAll;
}; // class JubiterBaseBTCImpl end


} // namespace token end
} // namespace jub end
