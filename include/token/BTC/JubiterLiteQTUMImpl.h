#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/interface/BaseToken.h"
#include "token/interface/QTUMTokenInterface.hpp"
#include <string>

namespace jub {
namespace token {

class JubiterLiteQTUMImpl : public JubiterLiteBTCImpl, public QTUMTokenInterface {
  public:
    JubiterLiteQTUMImpl(const JUB_UINT16 deviceID) : JubiterLiteBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _coin = TWCoinType::TWCoinTypeQtum;
    }
    ~JubiterLiteQTUMImpl() {}

    // QTUM functions
    virtual JUB_RV SetQRC20ETHToken(const std::string &tokenName, const JUB_UINT16 unitDP,
                                    const std::string &contractAddress) override;
}; // class JubiterLiteQTUMImpl end

} // namespace token
} // namespace jub
