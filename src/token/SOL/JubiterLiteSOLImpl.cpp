#include "JUB_SDK_COMM.h"
#include "token/SOL/JubiterLiteSOLImpl.h"
#include <TrezorCrypto/bip32.h>

namespace jub {
namespace token {


#define SWITCH_TO_SOL_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE)));\
} while (0);                                    \


JUB_RV JubiterLiteSOLImpl::SelectApplet() {

    SWITCH_TO_SOL_APP;
    return JUBR_OK;
}


JUB_RV JubiterLiteSOLImpl::GetAppletVersion(stVersion& version) {

    return JubiterLiteImpl::GetAppletVersion(version);
}


JUB_RV JubiterLiteSOLImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterLiteSOLImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //path = "m/44'/501'/0'";
    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
