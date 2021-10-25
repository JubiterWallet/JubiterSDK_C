#pragma once
#include "token/interface/CKBTokenInterface.hpp"
#include "token/BTC/JubiterBaseBTCImpl.h"
#include <NervosCKB/SegwitAddress.h>
#include <NervosCKB/Transaction.h>

namespace jub {
namespace token {

class JubiterBaseCKBImpl :
virtual public CKBTokenInterface {
public:
    JubiterBaseCKBImpl() {
            _coin = TWCoinType::TWCoinTypeNervosCKB;
            _curve_name = (char*)SECP256K1_CKB_NAME;
    };

    static JUB_RV ParseAddress(const std::string& address,
                               int* format,
                               int* codeHashIndex,
                               std::string& codeHash,
                               std::string& args);
    static JUB_RV IsValidAddress(const std::string& address);
    virtual JUB_RV  CheckAddress(const std::string& address, const TWCoinType& coinNet) override;

    virtual JUB_RV SerializeUnsignedTx(const JUB_UINT32 version,
                                       const std::vector<CELL_DEP>& vDeps,
                                       const std::vector<CELL_INPUT>& vInputs,
                                       const std::vector<CELL_OUTPUT>& vOutputs,
                                       TW::Data& unsignedRaw,
                                       const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;

    virtual JUB_RV UnsignedTx(const JUB_UINT32 version,
                              const std::vector<CELL_DEP>& vDeps,
                              const std::vector<CELL_INPUT>& vInputs,
                              const std::vector<CELL_OUTPUT>& vOutputs,
                              TW::NervosCKB::Transaction& unsignedTx,
                              const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin);

    virtual JUB_RV SignTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const std::vector<std::string>& vInputPath,
                          const JUB_UINT32 version,
                          const std::vector<CELL_DEP>& vDeps,
                          const std::vector<CELL_INPUT>& vInputs,
                          const std::vector<CELL_OUTPUT>& vOutputs,
                          std::vector<uchar_vector>& vSignatureRaw,
                          const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                            const std::vector<std::string>& vInputPath,
                            const JUB_UINT32 version,
                            const std::vector<CELL_DEP>& vDeps,
                            const std::vector<CELL_INPUT>& vInputs,
                            const std::vector<CELL_OUTPUT>& vOutputs,
                            const std::vector<uchar_vector>& vSignatureRaw,
                            const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;

protected:
    virtual JUB_RV _getSegwitAddress(const TW::Data& publicKey, std::string& address, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin);

    virtual JUB_RV _SignTX(const std::vector<std::string>& vInputPath,
                          TW::NervosCKB::Transaction& tx,
                          const TWCoinType& coinNet) = 0;
    virtual JUB_RV _VerifyTX(const std::vector<std::string>& vInputPath,
                             const TW::NervosCKB::Transaction& tx,
                             const std::vector<uchar_vector>& vSignatureRaw,
                             const TWCoinType& coinNet) = 0;

    virtual JUB_RV _verifyTX(const TW::NervosCKB::Transaction& tx,
                             const std::vector<TW::PublicKey>& vInputPublicKey,
                             const TWCoinType& coinNet);

    virtual JUB_RV _unsignedTx(const uint32_t coin,
                               const JUB_UINT32 version,
                               const std::vector<CELL_DEP>& vDeps,
                               const std::vector<CELL_INPUT>& vInputs,
                               const std::vector<CELL_OUTPUT>& vOutputs,
                               TW::NervosCKB::Transaction& tx);
}; // class JubiterBaseCKBImpl end


} // namespace token end
} // namespace jub end
