#include "token/EOS/JubiterBIOEOSImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "utility/util.h"

#include <EOS/Transaction.h>
#include "token/ErrorHandler.h"


namespace jub {
namespace token {


JUB_RV JubiterBIOEOSImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    switch (format) {
    case JUB_ENUM_PUB_FORMAT::HEX:
        break;
    case JUB_ENUM_PUB_FORMAT::XPUB:
    {
        //Version higher than 5.1.5 supports XPub
        stVersionExp vSupportXpub(5, 1, 5);
        if (JubiterBladeEOSImpl::_appletVersion < vSupportXpub) {
            return JUBR_ERROR_ARGS;
        }
        break;
    }
    default:
        return JUBR_ERROR_ARGS;
    }

    //path = "m/44'/194'/0'";
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
