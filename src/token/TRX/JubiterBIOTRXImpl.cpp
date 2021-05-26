#include "token/TRX/JubiterBIOTRXImpl.h"
//#include "utility/util.h"
//
//#include <Tron/Transaction.h>
#include <Ethereum/ERC20Abi.h>
//#include "token/ErrorHandler.h"

namespace jub {
namespace token {


JUB_RV JubiterBIOTRXImpl::SelectApplet() {

    return JubiterBIOToken::_SelectApp(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE));
}


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


JUB_RV JubiterBIOTRXImpl::SetTRC721Token(const std::string& tokenName,
                                         const std::string& contractAddress) {

    // TRC721 token extension apdu
    if (typeid(JubiterBIOTRXImpl) == typeid(*this)) {
        if (JubiterBIOToken::_appletVersion < stVersionExp::FromString(JubiterBIOToken::MISC_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return JubiterBIOToken::SetERC721Token(tokenName.c_str(), contractAddress.c_str());
}


JUB_RV JubiterBIOTRXImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //Version higher than 5.1.5 supports XPub
    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        stVersionExp vSupportXpub(5, 1, 5);
        if (JubiterBladeTRXImpl::_appletVersion < vSupportXpub) {
            return JUBR_ERROR_ARGS;
        }
    }

    JUB_VERIFY_RV(JubiterBladeTRXImpl::GetHDNodeBase(format, path, pubkey));

    return JUBR_OK;
}

} // namespace token end
} // namespace jub end
