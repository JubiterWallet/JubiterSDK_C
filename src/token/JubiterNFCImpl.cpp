#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <utility/util.h>
#include <token/ErrorHandler.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include "BinaryCoding.h"
#include <Bitcoin/Address.h>
#include <Bitcoin/SegwitAddress.h>
#include <Bitcoin/Script.h>
#include <PrivateKey.h>

namespace jub {
namespace token {


#define SWITCH_TO_NFC_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_NFC, 16)); \
} while (0)                                     \


JUB_RV JubiterNFCImpl::SelectApplet() {
    SWITCH_TO_NFC_APP;
    return JUBR_OK;
}


JUB_RV JubiterNFCImpl::GetHDNode(const JUB_BYTE& type, const std::string& path, std::string& xpub) {

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(0x08, vPath);
    JUB_BYTE p2 = type;

    APDU apdu(0x00, 0xe6, 0x00, p2, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    xpub = (JUB_CHAR_PTR)retData;

    return JUBR_OK;
}


JUB_RV JubiterNFCImpl::GetAddress(const JUB_BYTE addrFmt,
                                  const JUB_ENUM_BTC_TRANS_TYPE& type,
                                  const std::string& path,
                                  const JUB_UINT16 tag,
                                  std::string& address) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCImpl::SignTX(const std::string& path,
                              const JUB_BYTE& hashMode,
                              const JUB_BYTE& hashType,
                              const std::vector<JUB_BYTE>& vPreImageHash,
                              std::vector<JUB_BYTE>& vSignatureRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    // if double-hash
    JUB_BYTE p1 = hashMode;
    JUB_BYTE p2 = hashType;

    //  first pack
    uchar_vector apduData = ToTlv(0x07, vPreImageHash);
    APDU apdu(0x00, 0xF8, p1, p2, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);
    apduData.clear();

    uchar_vector vPath;
    vPath << path;
    apduData << ToTlv(0x08, vPath);

    JUB_VERIFY_RV(_TranPack(apduData, p1, p2, kSendOnceLen, true)); // last data.
    apduData.clear();

    //  sign transactions
    JUB_BYTE retData[2] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    //one pack can do it
    apdu.SetApdu(0x00, 0xCA, p1, p2, 0);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6f09 == ret) {
        return JUBR_USER_CANCEL;
    }
    JUB_VERIFY_COS_ERROR(ret);

    // get transactions (pack by pack)
    if (2 != ulRetDataLen) { // total length
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    JUB_UINT16 totalReadLen = TW::decode16BE(retData);
    TW::Data sigRawTx(totalReadLen, 0x00);

    constexpr JUB_UINT16 kReadOnceLen = 256;
    apdu.le = kReadOnceLen;
    JUB_ULONG ulRetLen = kReadOnceLen;

    apdu.SetApdu(0x00, 0xF9, 0x00, 0x00, 0x00);
    JUB_UINT16 times = 0;
    for (times = 0; times < (totalReadLen / kReadOnceLen); times++) {

        JUB_UINT16 offset = times * kReadOnceLen;
        apdu.p1 = offset >> 8;
        apdu.p2 = offset & 0x00ff;

        JUB_VERIFY_RV(_SendApdu(&apdu, ret, sigRawTx.data() + times * kReadOnceLen, &ulRetLen));
        JUB_VERIFY_COS_ERROR(ret);
    }

    apdu.le = totalReadLen % kReadOnceLen;
    if (apdu.le) {
        JUB_UINT16 offset = times * kReadOnceLen;
        apdu.p1 = offset >> 8;
        apdu.p2 = offset & 0x00ff;

        ulRetLen = totalReadLen - times * kReadOnceLen;

        JUB_VERIFY_RV(_SendApdu(&apdu, ret, sigRawTx.data() + times * kReadOnceLen, &ulRetLen));
        JUB_VERIFY_COS_ERROR(ret);
    }

    vSignatureRaw.clear();
    vSignatureRaw = sigRawTx;

    return JUBR_OK;
}


//JUB_RV JubiterNFCImpl::VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
//                                const std::vector<JUB_UINT64>& vInputAmount,
//                                const std::vector<std::string>& vInputPath,
//                                const std::vector<JUB_BYTE>& vSigedTrans) {
//
//    bool witness = false;
//    if (p2sh_p2wpkh == type) {
//        witness = true;
//    }
//
//    // verify signature
//    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin,  witness);
//    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin, witness);
//
//    JUB_RV rv = JUBR_ERROR;
//    std::vector<TW::Data> vInputPublicKey;
//    for (const auto& inputPath:vInputPath) {
//        std::string xpub;
//        rv = GetHDNode(type, inputPath, xpub);
//        if (JUBR_OK != rv) {
//            break;
//        }
//
//        HDNode hdkey;
//        JUB_UINT32 parentFingerprint;
//        if (0 != hdnode_deserialize(xpub.c_str(), hdVersionPub, hdVersionPrv, _curve_name, &hdkey, &parentFingerprint)) {
//            rv = JUBR_ERROR;
//            break;
//        }
//
//        uchar_vector pk(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
//        vInputPublicKey.push_back(TW::Data(pk));
//    }
//    if (JUBR_OK != rv) {
//        return rv;
//    }
//
//    return VerifyTx(witness,
//                    vSigedTrans,
//                    vInputAmount,
//                    vInputPublicKey);
//}


} // namespace token end
} // namespace jub end
