#include "token/EOS/JubiterBIOEOSImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "utility/util.h"

#include <EOS/Transaction.h>
#include "token/ErrorHandler.h"


namespace jub {
namespace token {


JUB_RV JubiterBIOEOSImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //Version higher than 5.1.5 supports XPub
    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        stVersionExp vSupportXpub(5, 1, 5);
        if (JubiterBladeEOSImpl::_appletVersion < vSupportXpub) {
            return JUBR_ERROR_ARGS;
        }
    }

    JUB_VERIFY_RV(JubiterBladeEOSImpl::GetHDNodeBase(format, path, pubkey));

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
