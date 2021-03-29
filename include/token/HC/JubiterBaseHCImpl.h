#pragma once
#include "token/interface/BTCTokenInterface.hpp"
#include "token/BTC/JubiterBaseBTCImpl.h"
#include <Bitcoin/TransactionHcash.h>
#include <TrustWalletCore/TWHcashSigHashType.h>

namespace jub {
namespace token {

class JubiterBaseHCImpl :
virtual public JubiterBaseBTCImpl {
public:
    JubiterBaseHCImpl() :
        JubiterBaseBTCImpl() {
            _coin = TWCoinType::TWCoinTypeHcash;
            _curve_name = (char*)SECP256K1_HCASH_NAME;
    };

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const JUB_UINT32 version,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw) override;

    virtual JUB_RV VerifyTx(const bool witness,
                            const uchar_vector& signedRaw,
                            const std::vector<JUB_UINT64>& vInputAmount,
                            const std::vector<TW::Data>& vInputPublicKey) override;

protected:
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address) override;

    virtual JUB_RV _verifyTx(const TWCoinType& coin,
                             const TW::Bitcoin::Transaction* tx,
                             const uint32_t& hashType,
                             const std::vector<JUB_UINT64>& vInputAmount,
                             const std::vector<TW::PublicKey>& vInputPublicKey) override;

    virtual JUB_RV _serializeTx(bool witness,
                                const std::vector<JUB_UINT64>& vInputAmount,
                                const std::vector<TW::Data>& vInputPublicKey,
                                const std::vector<uchar_vector>& vSignatureRaw,
                                TW::Bitcoin::Transaction* tx,
                                uchar_vector& signedRaw) override;

    virtual JUB_RV _pkScript(const TW::PublicKey publicKey, TW::Bitcoin::Script& script);

    virtual JUB_RV _scriptCode(const TWCoinType& coin, const TW::PublicKey publicKey, TW::Bitcoin::Script& scriptCode) override;

    virtual JUB_RV _verifyPayToPublicKeyHashScriptSig(const TWCoinType& coin,
                                                      const TW::Bitcoin::Transaction& tx,
                                                      const size_t index, const uint32_t& hashType, const uint64_t amount,
                                                      const TW::Data& signature,
                                                      const TW::PublicKey publicKey,
                                                      bool witness=false) override;

}; // class JubiterBaseHCImpl end


} // namespace token end
} // namespace jub end
