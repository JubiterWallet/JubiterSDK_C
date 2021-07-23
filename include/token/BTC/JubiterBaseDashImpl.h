#pragma once
#include "token/interface/BTCTokenInterface.hpp"
#include "token/BTC/JubiterBaseBTCImpl.h"
#include <Dash/Transaction.h>

namespace jub {
namespace token {


class JubiterBaseDashImpl :
virtual public JubiterBaseBTCImpl {
public:
    JubiterBaseDashImpl() :
        JubiterBaseBTCImpl() {
        _coin = TWCoinType::TWCoinTypeDash;
    };

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const JUB_UINT32 version,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw,
                                       const TWCoinType& coinNet) override;
protected:
    virtual JUB_RV _verifyTx(const bool witness,
                             const uchar_vector& signedRaw,
                             const std::vector<JUB_UINT64>& vInputAmount,
                             const std::vector<TW::Data>& vInputPublicKey,
                             const TWCoinType& coinNet) override;
}; // class JubiterBaseDashImpl end


} // namespace token end
} // namespace jub end
