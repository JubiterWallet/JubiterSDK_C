#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <token/BTC/JubiterBaseBTCImpl.h>
#include "Bitcoin/TransactionDash.h"

namespace jub {
namespace token {


class JubiterBaseDashImpl :
virtual public JubiterBaseBTCImpl {
public:
    JubiterBaseDashImpl() : JubiterBaseBTCImpl() {};

    virtual JUB_RV SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                       const std::vector<INPUT_BTC>& vInputs,
                                       const std::vector<OUTPUT_BTC>& vOutputs,
                                       const JUB_UINT32 lockTime,
                                       uchar_vector& unsignedRaw) override;

    virtual JUB_RV VerifyTx(const bool witness,
                            const uchar_vector& signedRaw,
                            const std::vector<JUB_UINT64>& vInputAmount,
                            const std::vector<TW::Data>& vInputPublicKey) override;
}; // class JubiterBaseDashImpl end


} // namespace token end
} // namespace jub end
