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
//
//    virtual JUB_RV VerifyTx(const bool witness,
//                            const uchar_vector& signedRaw,
//                            const std::vector<JUB_UINT64>& vInputAmount,
//                            const std::vector<TW::Data>& vInputPublicKey) override;
//
protected:
    virtual JUB_RV _getSegwitAddress(const TW::Data& publicKey, std::string& address, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin);
//    virtual JUB_RV _verifyTx(const TWCoinType& coin,
//                             const TW::Bitcoin::Transaction* tx,
//                             const uint32_t& hashType,
//                             const std::vector<JUB_UINT64>& vInputAmount,
//                             const std::vector<TW::PublicKey>& vInputPublicKey) override;
//
    virtual JUB_RV _unsignedTx(const uint32_t coin,
                               const JUB_UINT32 version,
                               const std::vector<CELL_DEP>& vDeps,
                               const std::vector<CELL_INPUT>& vInputs,
                               const std::vector<CELL_OUTPUT>& vOutputs,
                               TW::NervosCKB::Transaction& tx);
}; // class JubiterBaseCKBImpl end


} // namespace token end
} // namespace jub end
