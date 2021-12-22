#pragma once
#include "Data.h"
#include "JUB_SDK_BTC.h"
#include "TWCoinType.h"
#include "token/interface/BTCTokenInterface.hpp"
#include <Bitcoin/Address.h>
#include <Bitcoin/Transaction.h>
#include <Hash.h>
#include <TrezorCrypto/bip32.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterBaseBTCImpl : virtual public BTCTokenInterface {
  public:
    JubiterBaseBTCImpl() { _coin = TWCoinType::TWCoinTypeBitcoin; };

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT32 version,
                                       const std::vector<INPUT_BTC> &vInputs, const std::vector<OUTPUT_BTC> &vOutputs,
                                       const JUB_UINT32 lockTime, uchar_vector &unsignedRaw,
                                       const TWCoinType &coinNet) override;

    virtual JUB_RV CheckAddress(const std::string &address, const TWCoinType &coinNet) override;

  protected:
    virtual JUB_RV _verifyTx(JUB_ENUM_BTC_TRANS_TYPE type, const uchar_vector &signedRaw,
                             const std::vector<JUB_UINT64> &vInputAmount, const std::vector<TW::Data> &vInputPublicKey,
                             const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin);

    virtual JUB_RV _unsignedTx(const uint32_t coin, const std::vector<INPUT_BTC> &vInputs,
                               const std::vector<OUTPUT_BTC> &vOutputs, TW::Bitcoin::Transaction &tx);

    virtual JUB_RV _scriptPubKey(const TWCoinType &coin, const TW::PublicKey publicKey, TW::Bitcoin::Script &script);

    virtual JUB_RV _scriptCode(const TWCoinType &coin, const TW::PublicKey publicKey, TW::Bitcoin::Script &scriptCode);

    virtual JUB_RV _verifyPayToPublicKeyHashScriptSig(const TWCoinType &coin, const TW::Bitcoin::Transaction &tx,
                                                      const size_t index, const uint32_t &hashType,
                                                      const uint64_t amount, const TW::Data &signature,
                                                      const TW::PublicKey publicKey, bool witness = false);
    virtual JUB_RV _verifyPayToWitnessPublicKeyHashScriptSig(const TWCoinType &coin, const TW::Bitcoin::Transaction &tx,
                                                             const size_t index, const uint32_t &hashType,
                                                             const uint64_t amount, const TW::Data &signature,
                                                             const TW::PublicKey publicKey);

    virtual JUB_RV _verifyPayToTaprootKeyPathSpending(const TWCoinType &coin, const TW::Bitcoin::Transaction &tx,
                                                      const size_t index, const uint32_t &hashType,
                                                      const uint64_t amount, const TW::Data &signature,
                                                      const TW::PublicKey publicKey);
    virtual TW::Data pushAll(const TW::Data &results);

    virtual JUB_RV _verifyTx(const TWCoinType &coin, const TW::Bitcoin::Transaction *tx, const uint32_t &hashType,
                             const std::vector<JUB_UINT64> &vInputAmount,
                             const std::vector<TW::PublicKey> &vInputPublicKey);

    virtual JUB_RV _serializeTx(JUB_ENUM_BTC_TRANS_TYPE type, const std::vector<JUB_UINT64> &vInputAmount,
                                const std::vector<TW::Data> &vInputPublicKey,
                                const std::vector<uchar_vector> &vSignatureRaw, TW::Bitcoin::Transaction *tx,
                                uchar_vector &signedRaw);

    virtual JUB_RV _getAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet);
    virtual JUB_RV _getSegwitAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet);
    virtual JUB_RV _getNestedSegwitAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet);
    virtual JUB_RV _getTaprootAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet);

  protected:
    uint32_t _hashType = TWBitcoinSigHashTypeAll;
}; // class JubiterBaseBTCImpl end

} // namespace token
} // namespace jub
