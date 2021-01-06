#include <token/ETH/JubiterBladeETHImpl.h>
#include <Ethereum/Transaction.h>
#include <Ethereum/RLP.h>
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
    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);

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
        data << (JUB_BYTE)JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41;
        data << (JUB_BYTE)0x00;
    }
    else {
        data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41, vNonce);
    }

    data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_PRICE_42, vGasPrice);
    data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_LIMIT_43, vGasLimit);
    data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_TO_44, vTo);

    // If value=0, when sending apdu,
    // it is clear that this part is empty
    uchar_vector vValueInWei(vValue);
    if (   1 == vValueInWei.size()
        && 0 == vValueInWei[0]
        ) {
        vValueInWei.clear();
    }
    data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_VALUE_45, vValueInWei);

    data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_INPUT_46, vInput);
    data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
    data << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CHAIN_ID_48, vChainID);

    JUB_BYTE ins = JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A;
    if (bERC20) {
        ins = JUB_ENUM_APDU_CMD::INS_SIGN_ERC20_C8;
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


JUB_RV JubiterBladeETHImpl::SignContract(const JUB_BYTE inputType,
                                         const std::vector<JUB_BYTE>& vNonce,
                                         const std::vector<JUB_BYTE>& vGasPrice,
                                         const std::vector<JUB_BYTE>& vGasLimit,
                                         const std::vector<JUB_BYTE>& vTo,
                                         const std::vector<JUB_BYTE>& vValue,
                                         const std::vector<JUB_BYTE>& vInput,
                                         const std::vector<JUB_BYTE>& vPath,
                                         const std::vector<JUB_BYTE>& vChainID,
                                         std::vector<JUB_BYTE>& vRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    uchar_vector apduData;

    if (0x00 == vNonce[0]) {
        apduData << (JUB_BYTE)0x41;
        apduData << (JUB_BYTE)0x00;
    }
    else {
        apduData << ToTlv(0x41, vNonce);
    }

    apduData << ToTlv(0x42, vGasPrice);
    apduData << ToTlv(0x43, vGasLimit);
    apduData << ToTlv(0x44, vTo);
    apduData << ToTlv(0x45, vValue);
    // Too length to send at here
//    apduData << ToTlv(0x46, vInput);
    apduData << ToTlv(0x47, vPath);
    apduData << ToTlv(0x48, vChainID);

    //  first pack
    APDU apdu(0x00, 0xF8, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }
    apduData.clear();

    uchar_vector tlvInput;
    tlvInput << ToTlv(inputType, vInput);
    apduData << ToTlv(0x46, tlvInput);
    unsigned long iCnt = apduData.size()/kSendOnceLen;
    JUB_UINT32 iRemainder = apduData.size()%kSendOnceLen;
    if (iCnt) {
        int bOnce = false;
        for (unsigned long i=0; i<iCnt; ++i) {
            if (   (i+1) == iCnt
                &&    0  == iRemainder
                ) {
                bOnce = true;
            }
            uchar_vector apduDataPart(&apduData[i*kSendOnceLen], kSendOnceLen);
            JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, bOnce));  // last data or not.
        }
    }
    if (iRemainder) {
        uchar_vector apduDataPart(&apduData[iCnt*kSendOnceLen], iRemainder);
        JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, true));  // last data.
    }
    apduData.clear();

    //  sign transactions
    JUB_BYTE ins = 0xc9;
    apdu.SetApdu(0x00, ins, 0x00, 0x00, 0);
    JUB_BYTE retData[2048] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6f09 == ret) {
        return JUBR_USER_CANCEL;
    }
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    std::vector<JUB_BYTE> signatureRaw;
    signatureRaw.insert(signatureRaw.end(), retData, retData + ulRetDataLen);

    // parse signature
    std::vector<JUB_BYTE> r(32);
    std::copy(signatureRaw.begin(), signatureRaw.begin()+32, r.begin());

    uchar_vector s(32);
    std::copy(signatureRaw.begin()+32, signatureRaw.begin()+32+32, s.begin());

    uchar_vector v(1);
    std::copy(signatureRaw.begin()+32+32, signatureRaw.end(), v.begin());
    TW::Ethereum::Transaction tx(vNonce,
                                 vGasPrice,
                                 vGasLimit,
                                 TW::Ethereum::Address(vTo),
                                 vValue,
                                 vInput,
                                 v, r, s);

    vRaw.clear();
    vRaw = TW::Ethereum::RLP::encode(tx);

    return JUBR_OK;
}


JUB_RV JubiterBladeETHImpl::SignBytestring(const std::vector<JUB_BYTE>& vTypedData,
                                           const std::vector<JUB_BYTE>& vPath,
                                           const std::vector<JUB_BYTE>& vChainID,
                                           std::vector<JUB_BYTE>& signatureRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    uint16_t total = 0;

    uchar_vector pathTLV = ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
    total += pathTLV.size();
    uchar_vector chainIdTLV = ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CHAIN_ID_48, vChainID);
    total += chainIdTLV.size();
    uchar_vector typedDataTLV = ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_MSG_49, vTypedData);
    total += typedDataTLV.size();

    uchar_vector apduData;
    apduData << total;
    apduData.reverse();
    apduData << pathTLV;
    apduData << chainIdTLV;

    //  first pack
    APDU apdu(0x00, 0xF8, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }
    apduData.clear();

    apduData << typedDataTLV;
    unsigned long iCnt = apduData.size()/kSendOnceLen;
    JUB_UINT32 iRemainder = apduData.size()%kSendOnceLen;
    if (iCnt) {
        int bOnce = false;
        for (unsigned long i=0; i<iCnt; ++i) {
            if (   (i+1) == iCnt
                &&    0  == iRemainder
                ) {
                bOnce = true;
            }
            uchar_vector apduDataPart(&apduData[i*kSendOnceLen], kSendOnceLen);
            JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, bOnce));  // last data or not.
        }
    }
    if (iRemainder) {
        uchar_vector apduDataPart(&apduData[iCnt*kSendOnceLen], iRemainder);
        JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, true));  // last data.
    }
    apduData.clear();

    //  sign transactions
    apdu.SetApdu(0x00, JUB_ENUM_APDU_CMD::INS_SIGN_MSG_CB, 0x00, 0x00, 0);
    JUB_BYTE retData[2048] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6f09 == ret) {
        return JUBR_USER_CANCEL;
    }
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    signatureRaw.clear();
    signatureRaw.insert(signatureRaw.end(), retData, retData + ulRetDataLen);

    return JUBR_OK;
}


JUB_RV JubiterBladeETHImpl::VerifyBytestring(const std::vector<JUB_BYTE>& vChainID,
                                             const std::string& path,
                                             const std::vector<JUB_BYTE>& vTypedData,
                                             const std::vector<JUB_BYTE>& vSignature) {

    // verify signature
    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    std::string xpub;
    JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB, path, xpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey,
                                     hdVersionPub, hdVersionPrv));

    // verify signature
    return JubiterBaseETHImpl::VerifyBytestring(vChainID,
                                                vTypedData,
                                                vSignature,
                                                publicKey);
}


} // namespace token end
} // namespace jub end
