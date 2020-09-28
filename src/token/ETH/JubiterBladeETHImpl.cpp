#include <token/ETH/JubiterBladeETHImpl.h>
#include <token/ErrorHandler.h>

namespace jub {
namespace token {


#define SWITCH_TO_ETH_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE)));\
} while (0);                                    \


JUB_RV JubiterBladeETHImpl::SelectApplet() {

    SWITCH_TO_ETH_APP;
    return JUBR_OK;
}


JUB_RV JubiterBladeETHImpl::GetAppletVersion(std::string &version) {

    uchar_vector appID(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE));
    JUB_VERIFY_RV(JubiterBladeToken::GetAppletVersion(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}


JUB_RV JubiterBladeETHImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    uchar_vector data(path.begin(), path.end());

    APDU apdu(0x00, 0xF6, 0x00, (JUB_BYTE)tag, (JUB_ULONG)data.size(), data.data(), 0x14);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    uchar_vector vAddress(retData, (unsigned int)ulRetDataLen);
    address = std::string(ETH_PRDFIX) + vAddress.getHex();

    return JUBR_OK;
}


JUB_RV JubiterBladeETHImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //path = "m/44'/60'/0'";
    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(0x08, vPath);

    //0x00 for hex, 0x01 for xpub
    if (   (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX  != format
        && (JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB != format
        ) {
        return JUBR_ERROR_ARGS;
    }

    APDU apdu(0x00, 0xE6, 0x00, format, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX == format) {
        uchar_vector vPubkey(retData, (unsigned int)ulRetDataLen);
        pubkey = std::string(ETH_PRDFIX) + vPubkey.getHex();
    }
    else if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        pubkey = (JUB_CHAR_PTR)retData;
    }

    return JUBR_OK;
}


JUB_RV JubiterBladeETHImpl::SignTX(const bool bERC20,
                                   const std::vector<JUB_BYTE>& vNonce,
                                   const std::vector<JUB_BYTE>& vGasPrice,
                                   const std::vector<JUB_BYTE>& vGasLimit,
                                   const std::vector<JUB_BYTE>& vTo,
                                   const std::vector<JUB_BYTE>& vValue,
                                   const std::vector<JUB_BYTE>& vInput,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainID,
                                   std::vector<JUB_BYTE>& vRaw) {

    uchar_vector data;

    if (0x00 == vNonce[0]) {
        data << (JUB_BYTE)0x41;
        data << (JUB_BYTE)0x00;
    }
    else {
        data << ToTlv(0x41, vNonce);
    }

    data << ToTlv(0x42, vGasPrice);
    data << ToTlv(0x43, vGasLimit);
    data << ToTlv(0x44, vTo);

    // If value=0, when sending apdu,
    // it is clear that this part is empty
    uchar_vector vValueInWei(vValue);
    if (   1 == vValueInWei.size()
        && 0 == vValueInWei[0]
        ) {
        vValueInWei.clear();
    }
    data << ToTlv(0x45, vValueInWei);

    data << ToTlv(0x46, vInput);
    data << ToTlv(0x47, vPath);
    data << ToTlv(0x48, vChainID);

    JUB_BYTE ins = 0x2a;
    if (bERC20) {
        ins = 0xc8;
    }

    //one pack can do it
    APDU apdu(0x00, ins, 0x01, 0x00, (JUB_ULONG)data.size(), data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    vRaw.clear();
    vRaw.insert(vRaw.end(), retData, retData + ulRetDataLen);

    return JUBR_OK;
}


JUB_RV JubiterBladeETHImpl::VerifyTX(const std::vector<JUB_BYTE>& vChainID,
                                     const std::string& path,
                                     const std::vector<JUB_BYTE>& vSigedTrans) {

    // verify signature
    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    std::string xpub;
    JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB, path, xpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey,
                                     hdVersionPub, hdVersionPrv));

    // verify signature
    return VerifyTx(vChainID,
                    vSigedTrans,
                    publicKey);
}


JUB_RV JubiterBladeETHImpl::SetERC20ETHToken(const std::string& tokenName, const JUB_UINT16 unitDP, const std::string& contractAddress) {

    uchar_vector lvName = Tollv(tokenName);
    uchar_vector address;
    address << ETHHexStr2CharPtr(contractAddress);

    uchar_vector data;
    data << (uint8_t)unitDP;
    data << (uint8_t)lvName.size();
    data << lvName;
    data << (uint8_t)address.size();
    data << address;

    APDU apdu(0x00, 0xC7, 0x00, 0x00, (JUB_ULONG)data.size(), data.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
