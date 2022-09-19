#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/interface/QTUMTokenInterface.hpp"
#include <string>

namespace jub {
namespace token {

class JubiterBladeQTUMImpl : public JubiterBladeBTCImpl, public QTUMTokenInterface {
  public:
    JubiterBladeQTUMImpl(const JUB_UINT16 deviceID) : JubiterBladeBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _coin = TWCoinType::TWCoinTypeQtum;
    }
    ~JubiterBladeQTUMImpl() {}

    // QTUM functions
    virtual JUB_RV SetQRC20ETHToken(const std::string &tokenName, const JUB_UINT16 unitDP,
                                    const std::string &contractAddress) override;
}; // class JubiterBladeQTUMImpl end

} // namespace token
} // namespace jub
