#include "token/XRP/JubiterBladeXRPImpl.h"
#include "utility/util.h"

#include <Ripple/Transaction.h>
#include "token/ErrorHandler.h"


namespace jub {
namespace token {


#define SWITCH_TO_XRP_APP                       \
do {                                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE)));\
} while (0);                                    \


JUB_RV JubiterBladeXRPImpl::SelectApplet() {

    SWITCH_TO_XRP_APP;
    return JUBR_OK;
}


JUB_RV JubiterBladeXRPImpl::GetAppletVersion(stVersion& version) {

    uchar_vector appID(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE));
    JUB_VERIFY_RV(JubiterBladeToken::GetAppletVersion(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}


//MISC functions
JUB_RV JubiterBladeXRPImpl::SetCoin() {
    APDU apdu(0x00, 0xF5, (JUB_BYTE)JUB_ENUM_COINTYPE_MISC::COINXRP, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (   0x9000 == ret
        || 0x6d00 == ret
        ) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeXRPImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);

    TW::Data vAddress;
    JUB_VERIFY_RV(JubiterBladeToken::GetAddress(tag, 0x00, apduData, vAddress));

    address = std::string(vAddress.begin(), vAddress.end());

    return JUBR_OK;
}


JUB_RV JubiterBladeXRPImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    switch (format) {
    case JUB_ENUM_PUB_FORMAT::HEX:
        break;
    case JUB_ENUM_PUB_FORMAT::XPUB:
    {
        //Version higher than 1.1.6 supports XPub
        stVersionExp vSupportXpub(1, 1, 7);
        if (JubiterBladeToken::_appletVersion < vSupportXpub) {
            return JUBR_ERROR_ARGS;
        }
        break;
    }
    default:
        return JUBR_ERROR_ARGS;
    }

    //path = "m/44'/144'/0'";
    uchar_vector vPubkey;
    JUB_VERIFY_RV(JubiterBladeToken::GetHDNode(0x00, format, path, vPubkey));

    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX == format) {
        pubkey = vPubkey.getHex();
    }
    else if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        pubkey = std::string(vPubkey.begin(), vPubkey.end());
    }

    return JUBR_OK;
}


JUB_RV JubiterBladeXRPImpl::SignTX(const std::vector<JUB_BYTE>& vPath,
                                   std::vector<JUB_BYTE>& vUnsignedRaw,
                                   std::vector<uchar_vector>& vSignatureRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    try {
        TW::Ripple::Transaction tx;
        tx.setPreImage(vUnsignedRaw);

        std::string path(vPath.begin(), vPath.end());
        std::string pubkey;
        JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, pubkey));
        uchar_vector vPubkey(pubkey);
        tx.pub_key.insert(tx.pub_key.end(), vPubkey.begin(), vPubkey.end());
        tx.serialize();
        vUnsignedRaw.clear();
        vUnsignedRaw = tx.getPreImage();

        uint16_t total = 0;
        // pathTLV
        uchar_vector pathLV;
        pathLV << (JUB_BYTE)(vPath.size());
        pathLV << vPath;
        uchar_vector pathTLV;
        pathTLV << ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, pathLV);
        total += pathTLV.size();

        //txAssistTLV                       - tag = 0x02
        //txType
        //txAssistBodyTLV                   - tag = 0x01
        //    pymtTypeType
        //    transferTLV                       - dxrp.tag = 0x05
        //        amountIndex
        //        feeIndex
        //        signingPubKeyTLV                  -  signingPubKey.tag = 0x03
        //           signingPubKeyIndex
        //           signingPubKeyLength
        //        destinationTLV                    -    destination.tag = 0x04
        //           destinationIndex
        //           destinationLength
        //        destinationTagTLV                 - destinationTag.tag = 0x07
        //           destinationTagIndex
        //           destinationTagLength
        //        memoTLV                           -           memo.tag = 0x08
        //           memoIndex
        //           memoLength
        //NETWORK_PREFIX TLV                - tag = 0x06
        //signingPubKeyTLV
        uchar_vector vSigningPubKey;
        vSigningPubKey << tx.getSignPubkeyIndex();
        vSigningPubKey << (uint8_t)tx.pub_key.size();
        uchar_vector vSigningPubKeyTLV;
        vSigningPubKeyTLV << ToTlv(0x03, vSigningPubKey);
        //destinationTLV
        uchar_vector vDestination;
        vDestination << tx.getDestinationIndex();
        vDestination << (uint8_t)tx.destination.pubkeyHashSize();
        uchar_vector vDestinationTLV;
        vDestinationTLV << ToTlv(0x04, vDestination);
        //destinationTagTLV
        uchar_vector vDestinationTag;
        vDestinationTag << tx.getDestinationTagIndex();
        vDestinationTag << (uint8_t)(0 < tx.destination_tag ? 4 : 0);
        uchar_vector vDestinationTagTLV;
        vDestinationTagTLV << ToTlv(0x07, vDestinationTag);
        //memoTLV
        uchar_vector vMemo;
        vMemo << tx.getShowMemoIndex();
        vMemo << (uint8_t)tx.getShowMemoSize();
        uchar_vector vMemoTLV;
        vMemoTLV << ToTlv(0x08, vMemo);

        //dxrpTLV
        uchar_vector vDxrp;
        vDxrp << tx.getAmountIndex();
        vDxrp << tx.getFeeIndex();
        vDxrp << vSigningPubKeyTLV;
        vDxrp << vDestinationTLV;
        vDxrp << vDestinationTagTLV;
        vDxrp << vMemoTLV;
        uchar_vector vDxrpTLV;
        vDxrpTLV << ToTlv(0x05, vDxrp);

        //txAssistBody
        uchar_vector vTxAssistBody;
        vTxAssistBody << (uint8_t)JUB_ENUM_XRP_PYMT_TYPE::DXRP;
        vTxAssistBody << vDxrpTLV;
        uchar_vector vTxAssistBodyTLV;
        vTxAssistBodyTLV << ToTlv(0x01, vTxAssistBody);

        //NETWORK_PREFIX TLV
        uchar_vector vNetWorkPrefix;
        vNetWorkPrefix << (uint8_t)(sizeof(TW::Ripple::NETWORK_PREFIX) / sizeof(uint8_t));
        vNetWorkPrefix << tx.getNetworkPrefix();

        //txAssistTLV
        uchar_vector vTxAssist;
        vTxAssist << (uint8_t)TW::Ripple::TransactionType::payment;
        vTxAssist << vTxAssistBodyTLV;
        uchar_vector vTxAssistTLV;
        vTxAssistTLV << ToTlv(0x02, vTxAssist);
        uchar_vector vNetWorkPrefixTLV;
        vTxAssistTLV << ToTlv(0x06, vNetWorkPrefix);
        total += vTxAssistTLV.size();
        total += tx.size();

        uchar_vector apduData;
        apduData << total;
        apduData.reverse();
        // pathTLV
        apduData << pathTLV;
        // txAssist
        apduData << vTxAssistTLV;

        //  first pack
        APDU apdu(0x00, 0xF8, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
        apduData.clear();

        // tx
        apduData << tx.serialize();
        unsigned long iCnt = apduData.size() / kSendOnceLen;
        JUB_UINT32 iRemainder = apduData.size() % kSendOnceLen;
        if (iCnt) {
            int bOnce = false;
            for (unsigned long i = 0; i < iCnt; ++i) {
                if ((i + 1) == iCnt
                    &&    0 == iRemainder
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
        JUB_BYTE retData[2048] = { 0, };
        JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
        apdu.SetApdu(0x00, JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A, 0x00, 0x00, 0);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
        if (0x6f09 == ret) {
            return JUBR_USER_CANCEL;
        }
        JUB_VERIFY_COS_ERROR(ret);

        uchar_vector signatureRaw(retData, retData + ulRetDataLen);
        vSignatureRaw.push_back(signatureRaw);
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
