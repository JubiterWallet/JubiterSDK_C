#include "utility/util.h"
#include "token/BTC/JubiterBladeQTUMImpl.h"
#include "token/ErrorHandler.h"


namespace jub {
namespace token {


JUB_RV JubiterBladeQTUMImpl::SetQRC20ETHToken(const std::string& tokenName,
                                              const JUB_UINT16 unitDP,
                                              const std::string& contractAddress) {

    return SetERC20Token(tokenName.c_str(), unitDP, contractAddress.c_str());
}


} // namespace token end
} // namespace jub end
