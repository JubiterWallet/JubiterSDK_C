#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <TrezorCrypto/curves.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "Bitcoin/Address.h"
#include "Bitcoin/Transaction.h"
#include <Hash.h>

namespace jub {
namespace token {


class JubiterBaseBTCImpl :
virtual public BTCTokenInterface {
public:
    JubiterBaseBTCImpl() {};

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

    JUB_RV _serializeUnsignedTx(const uint32_t coin,
                                const std::vector<INPUT_BTC>& vInputs,
                                const std::vector<OUTPUT_BTC>& vOutputs,
                                TW::Bitcoin::Transaction& tx);

    JUB_RV _verifyPayToPublicKeyHashScriptSig(const TWCoinType& coin,
                                              const TW::Bitcoin::Transaction& tx,
                                              const size_t index, const TWBitcoinSigHashType& hashType, const uint64_t amount,
                                              const TW::Data& signature,
                                              const TW::PublicKey publicKey,
                                              bool witness=false);

    JUB_RV _verifyTx(const TWCoinType& coin,
                     const TW::Bitcoin::Transaction& tx,
                     const TWBitcoinSigHashType& hashType,
                     const std::vector<JUB_UINT64>& vInputAmount,
                     const std::vector<TW::PublicKey>& vInputPublicKey);

    JUB_RV _serializeTx(const uint32_t coin,
                        const JUB_ENUM_BTC_TRANS_TYPE& type,
                        const std::vector<INPUT_BTC>& vInputs,
                        const std::vector<OUTPUT_BTC>& vOutputs,
                        const JUB_UINT32 lockTime,
                        const std::vector<uchar_vector>& vSignatureRaw,
                        uchar_vector& signedRaw);

protected:
    // add curve, prefix here
    TWCoinType _coin = TWCoinType::TWCoinTypeBitcoin;
    TWCurve _curve = TWCurve::TWCurveSECP256k1;
    TWPublicKeyType _publicKeyType = TWPublicKeyType::TWPublicKeyTypeSECP256k1;
    TWBitcoinSigHashType _hashType = TWBitcoinSigHashType::TWBitcoinSigHashTypeAll;
}; // class JubiterBaseBTCImpl end


} // namespace token end
} // namespace jub end
