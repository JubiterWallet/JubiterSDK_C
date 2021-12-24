#pragma once
#include "JUB_SDK_BTC.h"
#include "token/BTC/JubiterBaseBTCImpl.h"
#include "token/interface/BTCTokenInterface.hpp"
#include <Hcash/Transaction.h>
#include <TrustWalletCore/TWHcashSigHashType.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterBaseHCImpl : virtual public JubiterBaseBTCImpl {
  public:
    JubiterBaseHCImpl() : JubiterBaseBTCImpl() {
        _coin       = TWCoinType::TWCoinTypeHcash;
        _curve_name = (char *)SECP256K1_HCASH_NAME;
    };

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT32 version,
                                       const std::vector<INPUT_BTC> &vInputs, const std::vector<OUTPUT_BTC> &vOutputs,
                                       const JUB_UINT32 lockTime, uchar_vector &unsignedRaw,
                                       const TWCoinType &coinNet) override;

  protected:
    virtual JUB_RV _verifyTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const uchar_vector &signedRaw,
                             const std::vector<JUB_UINT64> &vInputAmount, const std::vector<TW::Data> &vInputPublicKey,
                             const TWCoinType &coinNet) override;

    virtual JUB_RV _getAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet) override;

    virtual JUB_RV _verifyTx(const JUB_ENUM_BTC_TRANS_TYPE &type,
                             const TWCoinType &coin,
                             const TW::Bitcoin::Transaction *tx, const uint32_t &hashType,
                             const std::vector<JUB_UINT64> &vInputAmount,
                             const std::vector<TW::PublicKey> &vInputPublicKey) override;

    virtual JUB_RV _serializeTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::vector<JUB_UINT64> &vInputAmount,
                                const std::vector<TW::Data> &vInputPublicKey,
                                const std::vector<uchar_vector> &vSignatureRaw, TW::Bitcoin::Transaction *tx,
                                uchar_vector &signedRaw) override;

    virtual JUB_RV _pkScript(const TW::PublicKey publicKey, TW::Bitcoin::Script &script);

    virtual JUB_RV _scriptCode(const TWCoinType &coin, const TW::PublicKey publicKey,
                               TW::Bitcoin::Script &scriptCode) override;

    virtual JUB_RV _verifyPayToPublicKeyHashScriptSig(const TWCoinType &coin, const TW::Bitcoin::Transaction &tx,
                                                      const size_t index, const uint32_t &hashType,
                                                      const uint64_t amount, const TW::Data &signature,
                                                      const TW::PublicKey publicKey, bool witness = false) override;

}; // class JubiterBaseHCImpl end

} // namespace token
} // namespace jub
