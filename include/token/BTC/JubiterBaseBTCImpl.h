#pragma once
#include "token/interface/BTCTokenInterface.hpp"
#include <TrezorCrypto/bip32.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <Bitcoin/Address.h>
#include <Bitcoin/Transaction.h>
#include <Hash.h>


namespace jub {
namespace token {


class JubiterBaseBTCImpl :
virtual public BTCTokenInterface {
public:
    JubiterBaseBTCImpl() {
        _coin = TWCoinType::TWCoinTypeBitcoin;
    };

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const JUB_UINT32 version,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw);

    virtual JUB_RV VerifyTx(const bool witness,
                            const uchar_vector& signedRaw,
                            const std::vector<JUB_UINT64>& vInputAmount,
                            const std::vector<TW::Data>& vInputPublicKey);
    
    virtual JUB_RV CheckAddress(const std::string& address);

protected:
    virtual JUB_RV _serializeUnsignedTx(const uint32_t coin,
                                        const std::vector<INPUT_BTC>& vInputs,
                                        const std::vector<OUTPUT_BTC>& vOutputs,
                                        TW::Bitcoin::Transaction& tx);

    virtual JUB_RV _scriptPubKey(const TWCoinType& coin, const TW::PublicKey publicKey, TW::Bitcoin::Script& script);

    virtual JUB_RV _scriptCode(const TWCoinType& coin, const TW::PublicKey publicKey, TW::Bitcoin::Script& scriptCode);

    virtual JUB_RV _verifyPayToPublicKeyHashScriptSig(const TWCoinType& coin,
                                                      const TW::Bitcoin::Transaction& tx,
                                                      const size_t index, const uint32_t& hashType, const uint64_t amount,
                                                      const TW::Data& signature,
                                                      const TW::PublicKey publicKey,
                                                      bool witness=false);

    virtual TW::Data pushAll(const TW::Data& results);

    virtual JUB_RV _verifyTx(const TWCoinType& coin,
                             const TW::Bitcoin::Transaction* tx,
                             const uint32_t& hashType,
                             const std::vector<JUB_UINT64>& vInputAmount,
                             const std::vector<TW::PublicKey>& vInputPublicKey);

    virtual JUB_RV _serializeTx(bool witness,
                                const std::vector<JUB_UINT64>& vInputAmount,
                                const std::vector<TW::Data>& vInputPublicKey,
                                const std::vector<uchar_vector>& vSignatureRaw,
                                TW::Bitcoin::Transaction* tx,
                                uchar_vector& signedRaw);

    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));

    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
    virtual JUB_RV _getSegwitAddress(const TW::Data& publicKey, std::string& address);

protected:
    uint32_t _hashType = TWBitcoinSigHashTypeAll;
}; // class JubiterBaseBTCImpl end


} // namespace token end
} // namespace jub end
