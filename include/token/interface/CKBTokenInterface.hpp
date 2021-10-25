#ifndef __CKBTokenInterface__
#define __CKBTokenInterface__

#include "JUB_SDK_CKB.h"

#include "token/interface/BaseToken.h"
#include "utility/util.h"


namespace jub {
namespace token {


class CKBTokenInterface :
virtual public BaseToken {
public:
    virtual ~CKBTokenInterface() = default;

    virtual JUB_RV SelectApplet() = 0;

    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) = 0;
    virtual JUB_RV SetCoin(const JUB_ENUM_COINTYPE_BTC& type) = 0;

    virtual JUB_RV GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt,
                              const JUB_ENUM_BTC_TRANS_TYPE& type,
                              const std::string& path,
                              const JUB_UINT16 tag,
                              std::string& address,
                              const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;

    virtual JUB_RV CheckAddress(const std::string& address, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;

    virtual JUB_RV SignTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const std::vector<std::string>& vInputPath,
                          const JUB_UINT32 version,
                          const std::vector<CELL_DEP>& vDeps,
                          const std::vector<CELL_INPUT>& vInputs,
                          const std::vector<CELL_OUTPUT>& vOutputs,
                          std::vector<uchar_vector>& vSignatureRaw,
                          const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
    virtual JUB_RV VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                            const std::vector<std::string>& vInputPath,
                            const JUB_UINT32 version,
                            const std::vector<CELL_DEP>& vDeps,
                            const std::vector<CELL_INPUT>& vInputs,
                            const std::vector<CELL_OUTPUT>& vOutputs,
                            const std::vector<uchar_vector>& vSignatureRaw,
                            const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;

    virtual JUB_RV SerializeUnsignedTx(const JUB_UINT32 version,
                                       const std::vector<CELL_DEP>& vDeps,
                                       const std::vector<CELL_INPUT>& vInputs,
                                       const std::vector<CELL_OUTPUT>& vOutputs,
                                       TW::Data& unsignedRaw,
                                       const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin) = 0;
}; // class CKBTokenInterface end


} // namespace token end
} // namespace jub end

#endif
