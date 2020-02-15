#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <TrezorCrypto/curves.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWBitcoin.h>
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
    JUB_RV _serializeUnsignedTx(const uint32_t coin,
                                const std::vector<INPUT_BTC>& vInputs,
                                const std::vector<OUTPUT_BTC>& vOutputs,
                                TW::Bitcoin::Transaction& tx);

    JUB_RV _verifyPayToPublicKeyHashScriptSig(const TWCoinType& coin,
                                              const TW::Bitcoin::Transaction& tx,
                                              const size_t index, const uint32_t& hashType, const uint64_t amount,
                                              const TW::Data& signature,
                                              const TW::PublicKey publicKey,
                                              bool witness=false);

    virtual JUB_RV _verifyTx(const TWCoinType& coin,
                             const TW::Bitcoin::Transaction& tx,
                             const uint32_t& hashType,
                             const std::vector<JUB_UINT64>& vInputAmount,
                             const std::vector<TW::PublicKey>& vInputPublicKey);

    virtual JUB_RV _serializeTx(bool witness,
                                const std::vector<JUB_UINT64>& vInputAmount,
                                const std::vector<TW::Data>& vInputPublicKey,
                                const std::vector<uchar_vector>& vSignatureRaw,
                                TW::Bitcoin::Transaction& tx,
                                uchar_vector& signedRaw);

protected:
    // add curve, prefix here
    TWCoinType _coin = TWCoinType::TWCoinTypeBitcoin;
    char *_curve_name = (char*)SECP256K1_NAME;
    TWPublicKeyType _publicKeyType = TWPublicKeyType::TWPublicKeyTypeSECP256k1;
    uint32_t _hashType = TWSignatureHashTypeAll;
}; // class JubiterBaseBTCImpl end


} // namespace token end
} // namespace jub end
