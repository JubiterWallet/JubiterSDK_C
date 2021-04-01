#include "token/TRX/JubiterBIOTRXImpl.h"
//#include "utility/util.h"
//
//#include <Tron/Transaction.h>
#include <Ethereum/ERC20Abi.h>
//#include "token/ErrorHandler.h"

namespace jub {
namespace token {

JUB_RV JubiterBIOTRXImpl::SetTRC20Token(const std::string& tokenName,
                                          const JUB_UINT16 unitDP,
                                          const std::string& contractAddress) {

    // TRC20 token extension apdu
    if (typeid(JubiterBIOTRXImpl) == typeid(*this)) {
        if (JubiterBIOToken::_appletVersion < stVersionExp::FromString(JubiterBIOToken::MISC_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return JubiterBIOToken::SetERC20Token(tokenName.c_str(), unitDP, contractAddress.c_str());
}


} // namespace token end
} // namespace jub end
