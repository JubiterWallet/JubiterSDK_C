#include "token/FIL/JubiterBladeFILImpl.h"
#include "token/ETH/JubiterBaseETHImpl.h"

#include "token/ETH/JubiterBladeETHImpl.h"
#include "token/ErrorHandler.h"
#include <Filecoin/Transaction.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

#define SWITCH_TO_FIL_APP                                                                                              \
    do {                                                                                                               \
        JUB_VERIFY_RV(_SelectApp(kPKIAID_ETH, sizeof(kPKIAID_ETH) / sizeof(JUB_BYTE)));                                \
    } while (0);

JUB_RV JubiterBladeFILImpl::SelectApplet() {
    SWITCH_TO_FIL_APP;
    return JUBR_OK;
}

// MISC functions
JUB_RV JubiterBladeFILImpl::SetCoin() {

    APDU apdu(0x00, 0xF5, (JUB_BYTE)JUB_ENUM_COINTYPE_ETH::COINFIL, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (0x9000 == ret || 0x6d00 == ret) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBladeFILImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    TW::Data vAddress;
    JUB_VERIFY_RV(JubiterBladeToken::GetAddress(0x00, tag, TW::Data(path.begin(), path.end()), vAddress));

    address = std::string(vAddress.begin(), vAddress.end());

    return JUBR_OK;
}

JUB_RV JubiterBladeFILImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    switch (format) {
    case JUB_ENUM_PUB_FORMAT::HEX:
    case JUB_ENUM_PUB_FORMAT::XPUB:
        break;
    default:
        return JUBR_ERROR_ARGS;
    }

    // path = "m/44'/461'/0'";
    uchar_vector vPubkey;
    JUB_VERIFY_RV(JubiterBladeToken::GetHDNode(0x00, format, path, vPubkey));

    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX == format) {
        pubkey = vPubkey.getHex();
    } else if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        pubkey = std::string(vPubkey.begin(), vPubkey.end());
    }

    return JUBR_OK;
}

JUB_RV JubiterBladeFILImpl::SignTX(const uint64_t &nonce, const int64_t &glimit, const uint256_t &gfeeCap,
                                   const uint256_t &gpremium, const std::string &to, const uint256_t &value,
                                   const std::string &input, const std::string &path,
                                   std::vector<uchar_vector> &vSignatureRaw) {

    try {
        uchar_vector apduData;

        // cidPrefix
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CID_PREFIX_4A, TW::Filecoin::Transaction::getCidPrefix());
        // version
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_VERSION_4B, {0x00});
        // to address
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_TO_44, TW::Filecoin::Address(to).bytes);
        // from address
        std::vector<JUB_BYTE> vPath(path.begin(), path.end());
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
        // nonce
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41, TW::encodeBENoZero(nonce));
        // value
        auto lambdaLoadingZero = [](const uint256_t &x) -> TW::Data {
            auto bytes = TW::encodeBENoZero(x);
            bytes.insert(bytes.begin(), 0);
            return bytes;
        };
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_VALUE_45, lambdaLoadingZero(value));
        // gas limit
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_LIMIT_43, lambdaLoadingZero(glimit));
        // gas fee cap
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_FEE_CAP_4E, lambdaLoadingZero(gfeeCap));
        // gas premium
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_PREMIUM_4F, lambdaLoadingZero(gpremium));
        // abi.MethodNum (0 => send)
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_ABI_METHODNUM_4C, {0x00});
        // data (empty)
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_DATA_4D, TW::Data());

        JUB_BYTE ins = JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A;
        //        if (bERC20) {
        //            ins = JUB_ENUM_APDU_CMD::INS_SIGN_ERC20_C8;
        //        }

        // one pack can do it
        APDU apdu(0x00, ins, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
        JUB_UINT16 ret         = 0;
        JUB_BYTE retData[2048] = {
            0,
        };
        JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
        JUB_VERIFY_COS_ERROR(ret);

        uchar_vector signatureRaw(retData, retData + ulRetDataLen);
        vSignatureRaw.push_back(signatureRaw);
    } catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}

} // namespace token
} // namespace jub
