#include <token/EOS/JubiterBladeEOSImpl.h>
#include <utility/util.h>

#include "EOS/Transaction.h"
#include <token/ErrorHandler.h>

namespace jub {
namespace token {


typedef enum class enumCoinTypeMisc {
    COIN = 0x00,
    COINEOS = 0x01,
    COINXRP = 0x02,
    Default = COIN
} JUB_ENUM_COINTYPE_MISC;


#define SWITCH_TO_EOS_APP                           \
do {				                                \
    if (JUBR_OK == _SelectApp(kPKIAID_MISC, 16)) {  \
        break;                                      \
    }                                               \
    if (JUBR_OK == _SelectApp(kPKIAID_EOS, 16)) {   \
        return JUBR_EOS_APP_INDEP_OK;               \
    }                                               \
} while (0);                                        \


JUB_RV JubiterBladeEOSImpl::SelectApplet() {

    SWITCH_TO_EOS_APP;
    return JUBR_OK;
}


//MISC functions
JUB_RV JubiterBladeEOSImpl::SetCoinType() {

    APDU apdu(0x00, 0xf5, (JUB_BYTE)JUB_ENUM_COINTYPE_MISC::COINEOS, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (   0x9000 == ret
        || 0x6d00 == ret
        ) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeEOSImpl::GetAddress(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {

    uchar_vector vPath;
    vPath << path;

    uchar_vector apduData = ToTlv(0x08, vPath);
    JUB_BYTE p1 = (JUB_BYTE)tag;
    JUB_BYTE eosType = 0x00;
    switch (type) {
    case TW::EOS::Type::Legacy:
    {
        eosType = 0x01;
        break;
    } // case TW::EOS::Type::Legacy end
    case TW::EOS::Type::ModernK1:
    {
        eosType = 0x00;
        break;
    } // case TW::EOS::Type::ModernK1 end
    case TW::EOS::Type::ModernR1:
    {
        eosType = 0x02;
        break;
    } // case TW::EOS::Type::ModernR1 end
    default:
        return JUBR_IMPL_NOT_SUPPORT;
    } // switch (type) end

    APDU apdu(0x00, 0xf6, p1, eosType, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    address = (JUB_CHAR_PTR)retData;

    return JUBR_OK;
}


JUB_RV JubiterBladeEOSImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //path = "m/44'/194'/0'";
    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(0x08, vPath);

    //0x00 for hex
    if (JUB_ENUM_PUB_FORMAT::HEX != format
        ) {
        return JUBR_ERROR_ARGS;
    }

    APDU apdu(0x00, 0xe6, 0x00, format, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    uchar_vector vPubkey(retData, (unsigned int)ulRetDataLen);
    pubkey = vPubkey.getHex();

    return JUBR_OK;
}


JUB_RV JubiterBladeEOSImpl::SignTX(const TW::EOS::Type& type,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainId,
                                   const std::vector<JUB_BYTE>& vRaw,
                                   std::vector<uchar_vector>& vSignatureRaw,
                                   bool bWithType) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    try {
        TW::EOS::Transaction tx;
        tx.deserialize(vRaw, bWithType);

        uint16_t total = 0;
        // pathTLV
        uchar_vector pathLV;
        pathLV << (JUB_BYTE)(vPath.size());
        pathLV << vPath;
        uchar_vector pathTLV;
        pathTLV << ToTlv(0x08, pathLV);
        total += pathTLV.size();

        // chainID
        total += vChainId.size();
        uchar_vector chainIdV(vChainId);

        // expiration
        uchar_vector vExpiration;
        TW::encode32LE(tx.expiration, vExpiration);
        total += vExpiration.size();

        // refBlockNumber
        uchar_vector vRefBlockNumber;
        TW::encode16LE(tx.refBlockNumber, vRefBlockNumber);
        total += vRefBlockNumber.size();

        // refBlockPrefix
        uchar_vector vRefBlockPrefix;
        TW::encode32LE(tx.refBlockPrefix, vRefBlockPrefix);
        total += vRefBlockPrefix.size();

        // maxCPUUsageInMS
        uchar_vector vMaxCPUUsageInMS;
        vMaxCPUUsageInMS << tx.maxCPUUsageInMS;
        total += vMaxCPUUsageInMS.size();

        // maxNetUsageWords
        uchar_vector vMaxNetUsageWords;
        TW::Bravo::encodeVarInt32(tx.maxNetUsageWords, vMaxNetUsageWords);
        uchar_vector maxNetUsageWordsLV;
        maxNetUsageWordsLV << (JUB_BYTE)vMaxNetUsageWords.size();
        maxNetUsageWordsLV << vMaxNetUsageWords;
        total += maxNetUsageWordsLV.size();

        // delaySeconds
        uchar_vector vDelaySeconds;
        TW::Bravo::encodeVarInt32(tx.delaySeconds, vDelaySeconds);
        uchar_vector delaySecondsLV;
        delaySecondsLV << (JUB_BYTE)vDelaySeconds.size();
        delaySecondsLV << vDelaySeconds;
        total += delaySecondsLV.size();

        // contextFreeActions
        uchar_vector vContextFreeActions;
        TW::Bravo::encodeCollection(tx.contextFreeActions, vContextFreeActions);
        uchar_vector contextFreeActionsTLV;
        contextFreeActionsTLV << ToTlv(0x01, vContextFreeActions);
        total += contextFreeActionsTLV.size();

        // actionAssistTLV
        uchar_vector vActionAssist;
        vActionAssist << (JUB_BYTE)tx.actions.size();
        uint16_t offset = tx.actionCntLength(); // action cnt
        for (TW::EOS::Action& action : tx.actions) {
            uint16_t actionOffset = 0;
            actionOffset += offset;
            actionOffset += action.startingPostData();

            uint16_t memoOffset = 0;
            uint16_t memoLength = 0;
            switch ((JUB_ENUM_EOS_ACTION_TYPE)action.getType()) {
            case JUB_ENUM_EOS_ACTION_TYPE::XFER:
            {
                TW::EOS::TransferAction txAction;
                txAction.deserialize(action.data);
                memoOffset = txAction.startingPostData();
                memoOffset += actionOffset;
                memoLength = txAction.memoLength();
                break;
            }
            case JUB_ENUM_EOS_ACTION_TYPE::DELE:
            case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
            case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
            default:
                break;
            }

            // data offset
            uchar_vector vOffset;
            vOffset << actionOffset;
            vOffset.reverse();
            vActionAssist << vOffset;

            // memo offset
            vOffset.clear();
            vOffset << memoOffset;
            vOffset.reverse();
            vActionAssist << vOffset;

            // memo length
            vOffset.clear();
            vOffset << memoLength;
            vOffset.reverse();
            vActionAssist << vOffset;

            vActionAssist << action.getType();

            offset += action.size();
        }
        uchar_vector actionAssistTLV;
        actionAssistTLV << ToTlv(0x02, vActionAssist);
        total += actionAssistTLV.size();

        // actions
        uchar_vector vActions;
        TW::Bravo::encodeCollection(tx.actions, vActions);
        uchar_vector actionsTLV;
        actionsTLV << ToTlv(0x03, vActions);
        total += actionsTLV.size();

        // transactionExtensions
        uchar_vector vTransactionExtensions;
        TW::Bravo::encodeCollection(tx.transactionExtensions, vTransactionExtensions);
        uchar_vector transactionExtensionsTLV;
        transactionExtensionsTLV << ToTlv(0x04, vTransactionExtensions);
        total += transactionExtensionsTLV.size();

        // contextFreeData
        uchar_vector contextFreeDataTLV;
        contextFreeDataTLV << ToTlv(0x05, tx.contextFreeData);
        total += contextFreeDataTLV.size();

        uchar_vector apduData;
        apduData << total;
        apduData.reverse();
        // pathTLV
        apduData << pathTLV;
        // chainID
        apduData << vChainId;
        // expiration
        apduData << vExpiration;
        // refBlockNumber
        apduData << vRefBlockNumber;
        // refBlockPrefix
        apduData << vRefBlockPrefix;
        // maxCPUUsageInMS
        apduData << vMaxCPUUsageInMS;
        // maxNetUsageWords
        apduData << maxNetUsageWordsLV;
        // delaySeconds
        apduData << delaySecondsLV;
        // contextFreeActions
        apduData << contextFreeActionsTLV;

        //  first pack
        APDU apdu(0x00, 0xF8, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
        apduData.clear();

        // actionAssist
        apduData << actionAssistTLV;
        // actions
        apduData << actionsTLV;
        // transactionExtensions
        apduData << transactionExtensionsTLV;
        // contextFreeData
        apduData << contextFreeDataTLV;
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
        apdu.SetApdu(0x00, 0x2A, 0x00, 0x00, 0);
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
