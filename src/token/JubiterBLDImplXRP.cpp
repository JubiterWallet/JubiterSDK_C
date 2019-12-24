//
//  JubiterBLDImplXRP.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "token/JubiterBLDImpl.h"
#include "Ripple/Address.h"
#include "Ripple/Transaction.h"

namespace jub {

#define SWITCH_TO_XRP_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_MISC, 16));\
} while (0);                                    \

JUB_RV JubiterBLDImpl::SelectAppletXRP() {
    SWITCH_TO_XRP_APP;
    return JUBR_OK;
}

//MISC functions
JUB_RV JubiterBLDImpl::SetCoinTypeXRP() {
    return _SetCoinType((JUB_BYTE)JUB_ENUM_COINTYPE_MISC::COINXRP);
}

JUB_RV JubiterBLDImpl::GetAddressXRP(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    uchar_vector vPath;
    vPath << path;

    uchar_vector apduData = ToTlv(0x08, vPath);
    JUB_BYTE p1 = (JUB_BYTE)tag;
    APDU apdu(0x00, 0xf6, p1, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    address = (JUB_CHAR_PTR)retData;

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::GetHDNodeXRP(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    return JUBR_IMPL_NOT_SUPPORT;
}

JUB_RV JubiterBLDImpl::SignTXXRP(const std::vector<JUB_BYTE>& vPath,
                                 std::vector<JUB_BYTE>& vUnsignedRaw,
                                 std::vector<uchar_vector>& vSignatureRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    try {
        TW::Ripple::Transaction tx;
        tx.setPreImage(vUnsignedRaw);

        std::string path(vPath.begin(), vPath.end());
        std::string address;
        JUB_VERIFY_RV(GetAddressXRP(path, false, address));

        TW::Ripple::Address addr(address);
        tx.pub_key.insert(tx.pub_key.end(), addr.bytes.begin(), addr.bytes.end());
        tx.serialize();
        vUnsignedRaw.clear();
        vUnsignedRaw = tx.getPreImage();

        uint16_t total = 0;
        // pathTLV
        uchar_vector pathLV;
        pathLV << (JUB_BYTE)(vPath.size());
        pathLV << vPath;
        uchar_vector pathTLV;
        pathTLV << ToTlv(0x08, pathLV);
        total += pathTLV.size();

        //txAssistTLV                           - tag = 0x02
        //txType
        //txAssistBodyTLV                       - tag = 0x01
        //    pymtType
        //    dxrpTLV                           - tag = 0x05
        //        amountIndex
        //        feeIndex
        //        signingPubKeyTLV              - tag = 0x03
        //           signingPubKeyIndex
        //           signingPubKeyLength
        //        destinationTLV                - tag = 0x04
        //           destinationIndex
        //           destinationLength
        //NETWORK_PREFIX TLV                    - tag = 0x06
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

        //dxrpTLV
        uchar_vector vDxrp;
        vDxrp << tx.getAmountIndex();
        vDxrp << tx.getFeeIndex();
        vDxrp << vSigningPubKeyTLV;
        vDxrp << vDestinationTLV;
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
        vNetWorkPrefix << (uint8_t)(sizeof(TW::Ripple::NETWORK_PREFIX)/sizeof(uint8_t));
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
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
        apduData.clear();

        // tx
        apduData << tx.serialize();
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
        JUB_BYTE retData[2048] = {0,};
        JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
        apdu.SetApdu(0x00, 0x2A, 0x00, 0x00, 0);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
        if (0x6f09 == ret) {
            return JUBR_USER_CANCEL;
        }
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        uchar_vector signatureRaw(retData, retData + ulRetDataLen);
        vSignatureRaw.push_back(signatureRaw);
    }
    catch(...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}

} // namespace jub end
