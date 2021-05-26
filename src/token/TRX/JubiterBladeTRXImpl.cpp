#include "token/TRX/JubiterBladeTRXImpl.h"
#include "token/interface/ETHTokenInterface.hpp"
#include "utility/util.h"

#include <Tron/Transaction.h>
#include <Ethereum/ERC20Abi.h>
#include <Ethereum/ERC721Abi.h>
#include "token/ErrorHandler.h"

namespace jub {
namespace token {


#define SWITCH_TO_TRX_APP                           \
do {				                                \
    if (JUBR_OK == _SelectApp(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE))) {  \
        break;                                      \
    }                                               \
} while (0);                                        \


JUB_RV JubiterBladeTRXImpl::SelectApplet() {

    SWITCH_TO_TRX_APP;
    return JUBR_OK;
}


JUB_RV JubiterBladeTRXImpl::GetAppletVersion(stVersion& version) {

    uchar_vector appID(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE));
    JUB_VERIFY_RV(JubiterBladeToken::GetAppletVersion(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}


//MISC functions
JUB_RV JubiterBladeTRXImpl::SetCoin() {

    APDU apdu(0x00, 0xF5, (JUB_BYTE)JUB_ENUM_COINTYPE_MISC::COINTRX, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (   0x9000 == ret
        || 0x6d00 == ret
        ) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeTRXImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    uchar_vector vPath;
    vPath << path;

    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);
    JUB_BYTE p1 = (JUB_BYTE)tag;

    APDU apdu(0x00, 0xF6, p1, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    address = (JUB_CHAR_PTR)retData;

    return JUBR_OK;
}

JUB_RV JubiterBladeTRXImpl::GetHDNodeBase(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //path = "m/44'/195'/0'";
    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);

    APDU apdu(0x00, 0xE6, 0x00, format, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);
    
    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX == format) {
        uchar_vector vPubkey(retData, (unsigned int)ulRetDataLen);
        pubkey = vPubkey.getHex();
    }
    else if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        pubkey = (JUB_CHAR_PTR)retData;
    }

    return JUBR_OK;
}


JUB_RV JubiterBladeTRXImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //Version higher than 1.1.6 supports XPub
    if ((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB == format) {
        stVersionExp vSupportXpub(1, 1, 7);
        if (JubiterBladeToken::_appletVersion < vSupportXpub) {
            return JUBR_ERROR_ARGS;
        }
    }

    JUB_VERIFY_RV(JubiterBladeTRXImpl::GetHDNodeBase(format, path, pubkey));

    return JUBR_OK;
}


JUB_RV JubiterBladeTRXImpl::SignTX(const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vRaw,
                                   std::vector<uchar_vector>& vSignatureRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    try {
        TW::Tron::Transaction tx;
        tx.raw_data.deserialize(vRaw);

        uint16_t total = 0;
        // pathTLV
        uchar_vector pathLV;
        pathLV << (JUB_BYTE)(vPath.size());
        pathLV << vPath;
        uchar_vector pathTLV;
        pathTLV << ToTlv(0x08, pathLV);
        total += pathTLV.size();

        //contractAssistBodyTLV     - tag = 0x02(offset/length = Base128Varints)
        //contractType
        //  transfer                    - 0x01
        //  transferAsset               - 0x02
        //  freezeBalance               - 0x0B
        //  unfreezeBalance             - 0x0C
        //  createSmart                 - 0x1E
        //  triggerSmart                - 0x1F
        //contractTLV

        //contractAssistBody
        size_t contrIndex = tx.raw_data.contractOffset(0);
        if (0 == contrIndex) {
            return JUBR_ERROR_ARGS;
        }
        uchar_vector vContractAssist;
        vContractAssist << (uint8_t)tx.raw_data.contracts[0].type;
        switch (tx.raw_data.contracts[0].type) {
        case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferContract:
        {
            TW::Tron::TransferContract contract;
            if (!tx.raw_data.contracts[0].from_parameter<::protocol::TransferContract>(contract)) {
                break;
            }

            //transferTLV           - transfer.type = 0x01
            //    toAddress             - toAddress.tag = 0x01
            //       toAddressOffset
            //       toAddressLength
            //    amount                -    amount.tag = 0x02
            //       amountOffset
            //       amountLength
            //
            //eg. 0102027b0101026515
            //01        - type
            //02 02         - transfer.amount.tag
            //      7b          - amount offset
            //      01          - amount size
            //01 02         - transfer.toAddress.tag
            //      65          - toAddress offset
            //      15          - toAddress size
            uchar_vector vItem;
            vItem << contract.amountOffset(contrIndex);
            vItem << contract.amountSize();
            vContractAssist << ToTlv(0x02, vItem);

            vItem.clear();
            vItem << contract.toAddressOffset(contrIndex);
            vItem << contract.toAddressSize();

            vContractAssist << ToTlv(0x01, vItem);
            break;
        }
        case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferAssetContract:
        {
            TW::Tron::TransferAssetContract contract;
            if (!tx.raw_data.contracts[0].from_parameter<::protocol::TransferAssetContract>(contract)) {
                break;
            }

            //transferAssetTLV(TRC-10)  - transferAsset.type = 0x02
            //    assetName                 - assetName.tag = 0x03
            //       assetNameOffset
            //       assetNameLength
            //    toAddress
            //       toAddressOffset
            //       toAddressLength
            //    amount
            //       amountIndex
            //       amountLength
            //
            //eg. 0202038901010102731503025307
            //02        - type
            //02 03         - transferAsset.amount.tag
            //      8901        - amount offset
            //      01          - amount size
            //01 02         - transferAsset.toAddress.tag
            //      73          - toAddress offset
            //      15          - toAddress size
            //03 02         - transferAsset.assetName.tag
            //      53          - assetName offset
            //      07          - assetName size
            uchar_vector vItem;
            vItem << contract.amountOffset(contrIndex);
            vItem << contract.amountSize();
            vContractAssist << ToTlv(0x02, vItem);

            vItem.clear();
            vItem << contract.toAddressOffset(contrIndex);
            vItem << contract.toAddressSize();
            vContractAssist << ToTlv(0x01, vItem);

            vItem.clear();
            vItem << contract.assetNameOffset(contrIndex);
            vItem << contract.assetNameSize();
            vContractAssist << ToTlv(0x03, vItem);
            break;
        }
        case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_FreezeBalanceContract:
        {
            TW::Tron::FreezeBalanceContract contract;
            if (!tx.raw_data.contracts[0].from_parameter<::protocol::FreezeBalanceContract>(contract)) {
                break;
            }

            //freezeBalanceTLV          - freezeBalance.type = 0x0B
            //    frozenBalance
            //       frozenBalanceIndex
            //       frozenBalanceLength
            //    frozenDuration
            //       frozenDurationIndex
            //       frozenDurationLength
            //    resource
            //       resourceIndex
            //       resourceLength
            //    receiverAddress
            //       receiverAddressOffset
            //       receiverAddressLength
            //
            //eg. 0B020231020b0234010c02360101023815
            //0B        - type
            //02 02         - FreezeBalance.frozenBalance.tag
            //      31          - frozenBalance offset
            //      02          - frozenBalance size
            //0b 02         - FreezeBalance.frozenDuration.tag
            //      34          - frozenDuration offset
            //      01          - frozenDuration size
            //01 02         - FreezeBalance.receiverAddress.tag
            //      38          - receiverAddress offset
            //      15          - receiverAddress size
            uchar_vector vItem;
            vItem << contract.frozenBalanceOffset(contrIndex);
            vItem << contract.frozenBalanceSize();
            vContractAssist << ToTlv(0x02, vItem);

            vItem.clear();
            vItem << contract.frozenDurationOffset(contrIndex);
            vItem << contract.frozenDurationSize();
            vContractAssist << ToTlv(0x0B, vItem);

            vItem.clear();
            vItem << contract.receiverAddressOffset(contrIndex);
            vItem << contract.receiverAddressSize();
            vContractAssist << ToTlv(0x01, vItem);
            break;
        }
        case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UnfreezeBalanceContract:
        {
            TW::Tron::UnfreezeBalanceContract contract;
            if (!tx.raw_data.contracts[0].from_parameter<::protocol::UnfreezeBalanceContract>(contract)) {
                break;
            }

            //unfreezeBalanceTLV        - unfreezeBalance.type = 0x0C
            //    resource
            //       resourceIndex
            //       resourceLength
            //    receiverAddress
            //       receiverAddressOffset
            //       receiverAddressLength
            //
            //eg. 0c0c02310101023315
            //0C        - type
            //01 02         - UnfreezeBalance.receiverAddress.tag
            //      33          - receiverAddress offset
            //      15          - receiverAddress size
            uchar_vector vItem;
            vItem << contract.receiverAddressOffset(contrIndex);
            vItem << contract.receiverAddressSize();
            vContractAssist << ToTlv(0x01, vItem);
            break;
        }
        case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_CreateSmartContract:
        {
            //createSmartTLV(TRC-20)    - createSmart.type = 0x1E
            //    smartContract             - martContract.tag = 0x09
            //       smartContractOffset
            //       smartContractLength
            break;
        }
        case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TriggerSmartContract:
        {
            TW::Tron::TriggerSmartContract contract;
            if (!tx.raw_data.contracts[0].from_parameter<::protocol::TriggerSmartContract>(contract)) {
                break;
            }

            int erc = JUB_ENUM_APDU_ERC_ETH::ERC_INVALID;
            if (0 == memcmp(uchar_vector(contract.getData().getValue()).getHex().c_str(),
                            ERC20_ABI_METHOD_ID_TRANSFER, strlen(ERC20_ABI_METHOD_ID_TRANSFER))
                ) { // erc20 function sign
                erc = JUB_ENUM_APDU_ERC_ETH::ERC_20;
            }
            else if (0 == memcmp(uchar_vector(contract.getData().getValue()).getHex().c_str(),
                                 ERC721_ABI_METHOD_ID_TRANSFER_FROM, strlen(ERC721_ABI_METHOD_ID_TRANSFER_FROM))
                     ) { // erc20 function sign
                erc = JUB_ENUM_APDU_ERC_ETH::ERC_721;
            }

            //triggerSmartTLV(TRC-20)   - triggerSmart.type = 0x1F
            //    contractAddress               - contractAddress.tag = 0x04
            //       contractAddressOffset
            //       contractAddressLength
            //    callValue                     -       callValue.tag = 0x05
            //       callValueOffset
            //       callValueLength
            //    data                          -            data.tag = 0x06
            //       dataOffset
            //       dataLength
            //    callTokenValue                -  callTokenValue.tag = 0x07
            //       callTokenValueOffset
            //       callTokenValueLength
            //    tokenID                       -         tokenID.tag = 0x08
            //       tokenIDOffset
            //       tokenIDLength
            //    feeLimit                       -       feeLimit.tag = 0x0A
            //       feeLimitOffset
            //       feeLimitLength
            //
            //eg. 1f050248010402321506024a040a03860101
            //1f        - type
            //05 02         - triggerSmart.callValue.tag
            //      48          - callValue offset
            //      01          - callValue size
            //04 02         - triggerSmart.contractAddress.tag
            //      32          - contractAddress offset
            //      15          - contractAddress size
            //06 02         - triggerSmart.data.tag
            //      4a          - data offset
            //      04          - data size
            //0a 03         - tx.raw.feeLimit.tag
            //      8601        - feeLimit offset
            //      01          - feeLimit size
            uchar_vector vItem;
            if (!( 0 == JUB_ENUM_APDU_ERC_ETH::ERC_20
                || 0 == JUB_ENUM_APDU_ERC_ETH::ERC_721)
                ) {
                vItem << contract.callValueOffset(contrIndex);
                vItem << contract.callValueSize();
                vContractAssist << ToTlv(0x05, vItem);
            }

            vItem.clear();
            vItem << contract.contractAddressOffset(contrIndex);
            vItem << contract.contractAddressSize();
            vContractAssist << ToTlv(0x04, vItem);

            vItem.clear();
            vItem << contract.dataOffset(contrIndex);
            vItem << contract.dataSize();
            vContractAssist << ToTlv(0x06, vItem);

            vItem.clear();
            vItem << tx.raw_data.feeLimitOffset(0);
            vItem << tx.raw_data.feeLimitSize();
            vContractAssist << ToTlv(0x0A, vItem);
            break;
        }
        default:
            break;
        }
        if (0 == vContractAssist.size()) {
            return JUBR_ERROR_ARGS;
        }

        uchar_vector vContractAssistTLV;
        vContractAssistTLV << ToTlv(0x02, vContractAssist);
        total += vContractAssistTLV.size();
        total += vRaw.size();

        uchar_vector apduData;
        apduData << total;
        apduData.reverse();
        // pathTLV
        apduData << pathTLV;
        // contractAssist
        apduData << vContractAssistTLV;

        //  first pack
        APDU apdu(0x00, 0xF8, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
        apduData.clear();

        // tx
        apduData << vRaw;
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


JUB_RV JubiterBladeTRXImpl::SetTRC20Token(const std::string& tokenName,
                                          const JUB_UINT16 unitDP,
                                          const std::string& contractAddress) {

    // TRC20 token extension apdu
    if (typeid(JubiterBladeTRXImpl) == typeid(*this)) {
        if (JubiterBladeToken::_appletVersion < stVersionExp::FromString(JubiterBladeToken::MISC_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return JubiterBladeToken::SetERC20Token(tokenName.c_str(), unitDP, contractAddress.c_str());
}


JUB_RV JubiterBladeTRXImpl::SetTRC721Token(const std::string& tokenName,
                                           const std::string& contractAddress) {

    // TRC721 token extension apdu
    if (typeid(JubiterBladeTRXImpl) == typeid(*this)) {
        if (JubiterBladeToken::_appletVersion < stVersionExp::FromString(JubiterBladeToken::MISC_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return JubiterBladeToken::SetERC721Token(tokenName.c_str(), contractAddress.c_str());
}
} // namespace token end
} // namespace jub end
