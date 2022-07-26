#include "Address.h"
#include "Data.h"
#include "JUB_SDK_COMM.h"
#include "JUB_SDK_DEV.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "token/SOL/JubiterBaseSOLImpl.h"
#include "token/interface/SOLTokenInterface.hpp"
#include "utility/util.h"
#include <Solana/Address.h>
#include <Solana/Signer.h>
#include <Solana/Transaction.h>
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <token/SOL/JubiterBladeSOLImpl.h>
#include <token/ErrorHandler.h>
#include <typeinfo>
#include <vector>

namespace jub {
namespace token {

#define SWITCH_TO_SOL_APP                                                                                              \
    do {                                                                                                               \
        JUB_VERIFY_RV(_SelectApp(kPKIAID_SOL, sizeof(kPKIAID_SOL) / sizeof(JUB_BYTE)));                                \
    } while (0);

JUB_RV JubiterBladeSOLImpl::SelectApplet() {

    SWITCH_TO_SOL_APP;
    return JUBR_OK;
}

JUB_RV JubiterBladeSOLImpl::GetAppletVersion(stVersion &version) {

    uchar_vector appID(kPKIAID_SOL, sizeof(kPKIAID_SOL) / sizeof(JUB_BYTE));
    JUB_VERIFY_RV(JubiterBladeToken::GetAppletVersion(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}

JUB_RV JubiterBladeSOLImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    uchar_vector vAddress;
    JUB_VERIFY_RV(JubiterBladeToken::GetAddress(0x00, tag, TW::Data(path.begin(), path.end()), vAddress));

//    address = std::string(ETH_PRDFIX) + vAddress.getHex();

    return JUBR_OK;
}

JUB_RV JubiterBladeSOLImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    switch (format) {
    case JUB_ENUM_PUB_FORMAT::HEX:
    case JUB_ENUM_PUB_FORMAT::XPUB:
        break;
    default:
        return JUBR_ERROR_ARGS;
    }

    // path = "m/44'/60'/0'";
    uchar_vector vPubkey;
    JUB_VERIFY_RV(JubiterBladeToken::GetHDNode(0x00, format, path, vPubkey));

    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX == format) {
        pubkey = std::string(ETH_PRDFIX) + vPubkey.getHex();
    } else if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        pubkey = std::string(vPubkey.begin(), vPubkey.end());
    }

    return JUBR_OK;
}
} // namespace token
} // namespace jub
