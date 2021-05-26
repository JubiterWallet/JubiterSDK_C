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

    switch (format) {
    case JUB_ENUM_PUB_FORMAT::HEX:
        break;
    case JUB_ENUM_PUB_FORMAT::XPUB:
    {
        //Version higher than 5.1.5 supports XPub
        stVersionExp vSupportXpub(5, 1, 5);
        if (JubiterBladeTRXImpl::_appletVersion < vSupportXpub) {
            return JUBR_ERROR_ARGS;
        }
        break;
    }
    default:
        return JUBR_ERROR_ARGS;
    }

    //path = "m/44'/195'/0'";
    uchar_vector vPubkey;
    JUB_VERIFY_RV(JubiterBIOToken::GetHDNode(0x00, format, path, vPubkey));

    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX == format) {
        pubkey = vPubkey.getHex();
    }
    else if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        pubkey = std::string(vPubkey.begin(), vPubkey.end());
    }

    return JUBR_OK;
}

} // namespace token end
} // namespace jub end
