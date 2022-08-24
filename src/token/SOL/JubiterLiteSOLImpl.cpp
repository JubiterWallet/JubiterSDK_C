#include "token/SOL/JubiterLiteSOLImpl.h"
#include "JUB_SDK_COMM.h"
#include <TrezorCrypto/bip32.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

#define SWITCH_TO_SOL_APP                                                                                              \
    do {                                                                                                               \
        JUB_VERIFY_RV(_SelectApp(kPKIAID_NFC, sizeof(kPKIAID_NFC) / sizeof(JUB_BYTE)));                                \
    } while (0);

JUB_RV JubiterLiteSOLImpl::SelectApplet() {

    SWITCH_TO_SOL_APP;
    return JUBR_OK;
}

JUB_RV JubiterLiteSOLImpl::GetAppletVersion(stVersion &version) { return JubiterLiteImpl::GetAppletVersion(version); }

JUB_RV JubiterLiteSOLImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    return JUBR_IMPL_NOT_SUPPORT;
}
JUB_RV JubiterLiteSOLImpl::SetCoin() { return JUBR_IMPL_NOT_SUPPORT; }

JUB_RV JubiterLiteSOLImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    // path = "m/44'/501'/0'";
    return JUBR_IMPL_NOT_SUPPORT;
}
JUB_RV JubiterLiteSOLImpl::SetTokenInfo(const std::string &name, JUB_UINT8 decimal, const std::string &tokenMint) {
    return JubiterBladeToken::SetERC20Token(name.c_str(), decimal, tokenMint.c_str());
}

JUB_RV JubiterLiteSOLImpl::SignTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                          const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount,
                                          std::vector<JUB_BYTE> &raw) {
    return JUBR_IMPL_NOT_SUPPORT;
}
JUB_RV JubiterLiteSOLImpl::SignTokenTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                               const std::vector<JUB_BYTE> token, const std::vector<JUB_BYTE> &from,
                                               const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                               std::vector<JUB_BYTE> &raw) {
    return JUBR_IMPL_NOT_SUPPORT;
}
JUB_RV JubiterLiteSOLImpl::SignCreateTokenAccountTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                                    const std::vector<JUB_BYTE> &owner,
                                                    const std::vector<JUB_BYTE> &token, std::vector<JUB_BYTE> &raw) {
    return JUBR_IMPL_NOT_SUPPORT;
}

} // namespace token
} // namespace jub
