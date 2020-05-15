#pragma once
#include <token/interface/BTCTokenInterface.hpp>
#include <token/BTC/JubiterBaseBTCImpl.h>

namespace jub {
namespace token {


class JubiterBaseBCHImpl :
virtual public JubiterBaseBTCImpl {
public:
    JubiterBaseBCHImpl() {
        _coin = TWCoinType::TWCoinTypeBitcoinCash;
    };
    
    virtual JUB_RV CheckAddress(const std::string address);

protected:
    virtual JUB_RV _getAddress(const TW::Data publicKey, std::string& address) override;

    virtual JUB_RV _verifyTx(const TWCoinType& coin,
                             const TW::Bitcoin::Transaction* tx,
                             const uint32_t& hashType,
                             const std::vector<JUB_UINT64>& vInputAmount,
                             const std::vector<TW::PublicKey>& vInputPublicKey) override;
}; // class JubiterBaseBCHImpl end


} // namespace token end
} // namespace jub end
