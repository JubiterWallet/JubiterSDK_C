#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <token/BTC/JubiterBaseBTCImpl.h>
#include "Bitcoin/Transaction.h"

namespace jub {
namespace token {


class JubiterBaseBCHImpl :
virtual public JubiterBaseBTCImpl {
public:
    JubiterBaseBCHImpl() {};

protected:
    virtual JUB_RV _verifyTx(const TWCoinType& coin,
                             const TW::Bitcoin::Transaction* tx,
                             const uint32_t& hashType,
                             const std::vector<JUB_UINT64>& vInputAmount,
                             const std::vector<TW::PublicKey>& vInputPublicKey) override;
}; // class JubiterBaseBCHImpl end


} // namespace token end
} // namespace jub end
