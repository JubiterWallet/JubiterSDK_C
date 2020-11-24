#pragma once

#include "JUB_SDK.h"
#include <vector>
#include "token/interface/BTCTokenInterface.hpp"


namespace jub {
namespace token {

class QTUMTokenInterface :
virtual public BTCTokenInterface {
public:

//QTUM functions
virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
                                const JUB_UINT16 unitDP,
                                const std::string& contractAddress) = 0;
}; // class QTUMTokenInterface end


} // namespace token end
} // namespace jub end


