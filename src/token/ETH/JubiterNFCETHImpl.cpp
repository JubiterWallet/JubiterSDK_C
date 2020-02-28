#include <token/ETH/JubiterNFCETHImpl.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/bip32.h>
#include <utility/util.h>
#include <token/ErrorHandler.h>

namespace jub {
namespace token {


#define SWITCH_TO_ETH_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_NFC, 16)); \
} while (0);                                    \


JUB_RV JubiterNFCETHImpl::SelectApplet() {

    SWITCH_TO_ETH_APP;
    return JUBR_OK;
}


JUB_RV JubiterNFCETHImpl::GetAppletVersion(std::string &version) {

    uchar_vector appID(kPKIAID_NFC, 16);
    JUB_VERIFY_RV(GetAppletVersionBlade(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}


JUB_RV JubiterNFCETHImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    std::string btcXpub;
    JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(0x00, path, btcXpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(btcXpub, publicKey));

    return _getAddress(publicKey, address);
}


JUB_RV JubiterNFCETHImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //path = "m/44'/60'/0'";
    std::string btcXpub;
    JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(0x00, path, btcXpub));

    //    typedef enum {
    //        HEX = 0x00,
    //        XPUB = 0x01
    //    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) {//hex
        TW::Data publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(btcXpub, publicKey));

        uchar_vector vPublicKey(publicKey);
        pubkey = vPublicKey.getHex();
        pubkey = ETH_PRDFIX + pubkey;
    }
    else { //xpub
        pubkey = btcXpub;
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCETHImpl::SignTX(const bool bERC20,
                                 const std::vector<JUB_BYTE>& vNonce,
                                 const std::vector<JUB_BYTE>& vGasPrice,
                                 const std::vector<JUB_BYTE>& vGasLimit,
                                 const std::vector<JUB_BYTE>& vTo,
                                 const std::vector<JUB_BYTE>& vValue,
                                 const std::vector<JUB_BYTE>& vInput,
                                 const std::vector<JUB_BYTE>& vPath,
                                 const std::vector<JUB_BYTE>& vChainID,
                                 std::vector<JUB_BYTE>& vRaw) {

//    uchar_vector data;
//
//    if (0x00 == vNonce[0]) {
//        data << (JUB_BYTE)0x41;
//        data << (JUB_BYTE)0x00;
//    }
//    else {
//        data << ToTlv(0x41, vNonce);
//    }
//
//    data << ToTlv(0x42, vGasPrice);
//    data << ToTlv(0x43, vGasLimit);
//    data << ToTlv(0x44, vTo);
//    data << ToTlv(0x45, vValue);
//    data << ToTlv(0x46, vInput);
//    data << ToTlv(0x47, vPath);
//    data << ToTlv(0x48, vChainID);
//
//    JUB_BYTE ins = 0x2a;
//    if (bERC20) {
//        ins = 0xc8;
//    }
//
//    //one pack can do it
//    APDU apdu(0x00, ins, 0x01, 0x00, (JUB_ULONG)data.size(), data.data());
//    JUB_UINT16 ret = 0;
//    JUB_BYTE retData[2048] = { 0, };
//    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
//    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
//    JUB_VERIFY_COS_ERROR(ret);
//
//    vRaw.clear();
//    vRaw.insert(vRaw.end(), retData, retData + ulRetDataLen);

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCETHImpl::SetERC20ETHToken(const std::string& tokenName, const JUB_UINT16 unitDP, const std::string& contractAddress) {

    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
