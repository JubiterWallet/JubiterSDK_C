#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/CKB/JubiterBaseCKBImpl.h"


namespace jub {
namespace token {


class JubiterBladeCKBImpl :
        public JubiterBladeBTCImpl,
virtual public JubiterBaseCKBImpl {
public:
    JubiterBladeCKBImpl(const JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
    }
    ~JubiterBladeCKBImpl() {}

    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt,
                              const JUB_ENUM_BTC_TRANS_TYPE& type,
                              const std::string& path,
                              const JUB_UINT16 tag,
                              std::string& address,
                              const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;

    virtual JUB_RV SignTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const std::vector<std::string>& vInputPath,
                          const JUB_UINT32 version,
                          const std::vector<CELL_DEP>& vDeps,
                          const std::vector<CELL_INPUT>& vInputs,
                          const std::vector<CELL_OUTPUT>& vOutputs,
                          std::vector<uchar_vector>& vSignatureRaw,
                          const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) override;

protected:
    virtual JUB_RV _SignTX(const std::vector<std::string>& vInputPath,
                          TW::NervosCKB::Transaction& tx,
                          const TWCoinType& coinNet) override;
    virtual JUB_RV _VerifyTX(const std::vector<std::string>& vInputPath,
                             const TW::NervosCKB::Transaction& tx,
                             const std::vector<uchar_vector>& vSignatureRaw,
                             const TWCoinType& coinNet) override;
}; // class JubiterBladeCKBImpl end


} // namespace token end
} // namespace jub end
