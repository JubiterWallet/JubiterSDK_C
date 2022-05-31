#include "Address.h"
#include "Data.h"
#include "EIP712.h"
#include "JUB_SDK_COMM.h"
#include "JUB_SDK_DEV.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "token/ETH/JubiterBaseETHImpl.h"
#include "token/interface/ETHTokenInterface.hpp"
#include "utility/util.h"
#include <Ethereum/AddressChecksum.h>
#include <Ethereum/ContractAbi.h>
#include <Ethereum/RLP.h>
#include <Ethereum/Signer.h>
#include <Ethereum/Transaction.h>
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <token/ETH/JubiterBladeETHImpl.h>
#include <token/ErrorHandler.h>
#include <typeinfo>
#include <vector>

namespace jub {
namespace token {

#define SWITCH_TO_ETH_APP                                                                                              \
    do {                                                                                                               \
        JUB_VERIFY_RV(_SelectApp(kPKIAID_ETH, sizeof(kPKIAID_ETH) / sizeof(JUB_BYTE)));                                \
    } while (0);

JUB_RV JubiterBladeETHImpl::SelectApplet() {

    SWITCH_TO_ETH_APP;
    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::GetAppletVersion(stVersion &version) {

    uchar_vector appID(kPKIAID_ETH, sizeof(kPKIAID_ETH) / sizeof(JUB_BYTE));
    JUB_VERIFY_RV(JubiterBladeToken::GetAppletVersion(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    uchar_vector vAddress;
    JUB_VERIFY_RV(JubiterBladeToken::GetAddress(0x00, tag, TW::Data(path.begin(), path.end()), vAddress));

    address = std::string(ETH_PRDFIX) + vAddress.getHex();

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

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

JUB_RV JubiterBladeETHImpl::SignTX(const int erc, const std::vector<JUB_BYTE> &vNonce,
                                   const std::vector<JUB_BYTE> &vGasPrice, const std::vector<JUB_BYTE> &vGasLimit,
                                   const std::vector<JUB_BYTE> &vTo, const std::vector<JUB_BYTE> &vValue,
                                   const std::vector<JUB_BYTE> &vInput, const std::vector<JUB_BYTE> &vPath,
                                   const std::vector<JUB_BYTE> &vChainID, std::vector<JUB_BYTE> &vRaw) {

    // ETH token extension apdu
    if (JubiterBladeETHImpl::_appletVersion >=
        stVersionExp::FromString(JubiterBladeETHImpl::ETH_APPLET_VERSION_SUPPORT_EXT_TOKENS)) {
        return _SignTXUpgrade(erc, vNonce, vGasPrice, vGasLimit, vTo, vValue, vInput, vPath, vChainID, vRaw);
    } else {
        return _SignTX(erc, vNonce, vGasPrice, vGasLimit, vTo, vValue, vInput, vPath, vChainID, vRaw);
    }
}

JUB_RV JubiterBladeETHImpl::_SignTX(const int erc, const std::vector<JUB_BYTE> &vNonce,
                                    const std::vector<JUB_BYTE> &vGasPrice, const std::vector<JUB_BYTE> &vGasLimit,
                                    const std::vector<JUB_BYTE> &vTo, const std::vector<JUB_BYTE> &vValue,
                                    const std::vector<JUB_BYTE> &vInput, const std::vector<JUB_BYTE> &vPath,
                                    const std::vector<JUB_BYTE> &vChainID, std::vector<JUB_BYTE> &vRaw) {

    uchar_vector apduData;

    if (0x00 == vNonce[0]) {
        apduData << (JUB_BYTE)JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41;
        apduData << (JUB_BYTE)0x00;
    } else {
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41, vNonce);
    }

    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_PRICE_42, vGasPrice);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_LIMIT_43, vGasLimit);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_TO_44, vTo);

    // If value=0, when sending apdu,
    // it is clear that this part is empty
    uchar_vector vValueInWei(vValue);
    if (1 == vValueInWei.size() && 0 == vValueInWei[0]) {
        vValueInWei.clear();
    }
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_VALUE_45, vValueInWei);

    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_INPUT_46, vInput);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CHAIN_ID_48, vChainID);

    JUB_BYTE ins = JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A;
    JUB_BYTE p1 = JUB_ENUM_APDU_ERC_P1::ERC20;
    switch (erc) {
    case JUB_ENUM_APDU_ERC_ETH::ERC_721:
        p1 = JUB_ENUM_APDU_ERC_P1::ERC721;
    case JUB_ENUM_APDU_ERC_ETH::ERC_20:
        ins = JUB_ENUM_APDU_CMD::INS_SIGN_ERC20_C8;
        break;
    case JUB_ENUM_APDU_ERC_ETH::ERC_INVALID:
    default:
        break;
    }

    // one pack can do it
    APDU apdu(0x00, ins, p1, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    vRaw.clear();
    vRaw.insert(vRaw.end(), retData, retData + ulRetDataLen);

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::_SignTXUpgrade(const int erc, const std::vector<JUB_BYTE> &vNonce,
                                           const std::vector<JUB_BYTE> &vGasPrice,
                                           const std::vector<JUB_BYTE> &vGasLimit, const std::vector<JUB_BYTE> &vTo,
                                           const std::vector<JUB_BYTE> &vValue, const std::vector<JUB_BYTE> &vInput,
                                           const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vChainID,
                                           std::vector<JUB_BYTE> &vRaw) {

    // COS subpackage size 512 Byte
    uchar_vector apduData;

    if (0x00 == vNonce[0]) {
        apduData << (JUB_BYTE)JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41;
        apduData << (JUB_BYTE)0x00;
    } else {
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41, vNonce);
    }

    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_PRICE_42, vGasPrice);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_LIMIT_43, vGasLimit);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_TO_44, vTo);

    // If value=0, when sending apdu,
    // it is clear that this part is empty
    uchar_vector vValueInWei(vValue);
    if (1 == vValueInWei.size() && 0 == vValueInWei[0]) {
        vValueInWei.clear();
    }
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_VALUE_45, vValueInWei);

    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_INPUT_46, vInput);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CHAIN_ID_48, vChainID);

    JUB_BYTE ins = JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A;
    JUB_BYTE p1 = JUB_ENUM_APDU_ERC_P1::ERC20;
    switch (erc) {
    case JUB_ENUM_APDU_ERC_ETH::ERC_721:
        p1 = JUB_ENUM_APDU_ERC_P1::ERC721;
    case JUB_ENUM_APDU_ERC_ETH::ERC_20:
        ins = JUB_ENUM_APDU_CMD::INS_SIGN_ERC20_C8;
        break;
    case JUB_ENUM_APDU_ERC_ETH::ERC_INVALID:
    default:
        break;
    }

    // subpackage
    {
        constexpr JUB_UINT32 kSendOnceLen = 230;
        JUB_ULONG offset = 23;

        //  first pack
        APDU apdu(0x00, 0xF8, 0x01, 0x00, offset, apduData.data());
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        unsigned long iCnt = (apduData.size() - offset) / kSendOnceLen;
        JUB_UINT32 iRemainder = (apduData.size() - offset) % kSendOnceLen;
        if (iCnt) {
            int bOnce = false;
            for (unsigned long i = 0; i < iCnt; ++i) {
                if ((i + 1) == iCnt && 0 == iRemainder) {
                    bOnce = true;
                }
                uchar_vector apduDataPart(&apduData[offset + i * kSendOnceLen], kSendOnceLen);
                JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, bOnce)); // last data or not.
            }
        }
        if (iRemainder) {
            uchar_vector apduDataPart(&apduData[offset + iCnt * kSendOnceLen], iRemainder);
            JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, true)); // last data.
        }
        apduData.clear();
    }

    // one pack can do it
    APDU apdu(0x00, ins, p1, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    // finish transaction
    try {
        std::vector<JUB_BYTE> signatureRaw;
        signatureRaw.insert(signatureRaw.end(), retData, retData + ulRetDataLen);

        // parse signature
        TW::Ethereum::Signature signature = TW::Ethereum::Signer::signatureDataToStruct(
            signatureRaw,
            false, // TW::Ethereum::TransactionNonTyped::usesReplayProtection() is handled by the hardware
            vChainID);
        if (!signature.isValid()) {
            return JUBR_SIGN_FAILED;
        }

        vRaw.clear();
        vRaw =
            TW::Ethereum::TransactionNonTyped(vNonce, vGasPrice, vGasLimit, TW::Ethereum::Address(vTo), vValue, vInput)
                .encoded(signature, vChainID);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::SignTX(const int erc, const std::vector<JUB_BYTE> &vNonce,
                                   const std::vector<JUB_BYTE> &vGasPrice, const std::vector<JUB_BYTE> &vGasLimit,
                                   const std::vector<JUB_BYTE> &vTo, const std::vector<JUB_BYTE> &vValue,
                                   const std::vector<JUB_BYTE> &vInput, const std::string &accessListInJSON,
                                   const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vChainID,
                                   std::vector<JUB_BYTE> &vRaw) {
    // COS subpackage size 512 Byte
    uchar_vector apduData;
    using Tag = JUB_ENUM_APDU_DATA_ETH;
    // [path, chainId, nonce, gasPrice, gasLimit, to, value, data, accessList]
    apduData << ToTlv(Tag::TAG_PATH_47, vPath);
    apduData << ToTlv(Tag::TAG_CHAIN_ID_48, vChainID);
    if (0x00 == vNonce[0]) {
        apduData << (JUB_BYTE)Tag::TAG_NONCE_41;
        apduData << (JUB_BYTE)0x00;
    } else {
        apduData << ToTlv(Tag::TAG_NONCE_41, vNonce);
    }

    apduData << ToTlv(Tag::TAG_GAS_PRICE_42, vGasPrice);
    apduData << ToTlv(Tag::TAG_GAS_LIMIT_43, vGasLimit);
    apduData << ToTlv(Tag::TAG_TO_44, vTo);

    // If value=0, when sending apdu,
    // it is clear that this part is empty
    uchar_vector vValueInWei(vValue);
    if (1 == vValueInWei.size() && 0 == vValueInWei[0]) {
        vValueInWei.clear();
    }
    apduData << ToTlv(Tag::TAG_VALUE_45, vValueInWei);

    apduData << ToTlv(Tag::TAG_INPUT_46, vInput);

    auto accessList = TW::Ethereum::accessListFromJson(accessListInJSON);

    auto encoded = TW::Data();
    using RLP = TW::Ethereum::RLP;
    for (const auto &access : accessList) {
        auto item = RLP::encode(access.address.bytes);
        auto storageKeys = RLP::encodeList(access.storageKeys);
        std::copy(storageKeys.begin(), storageKeys.end(), std::back_inserter(item));
        auto itemList = RLP::encodeList(item);
        std::copy(itemList.begin(), itemList.end(), std::back_inserter(encoded));
    }
    apduData << ToTlv(Tag::TAG_ACCESS_LIST_53, RLP::encodeList(encoded));

    JUB_BYTE ins = JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A;
    JUB_BYTE p1 = JUB_ENUM_APDU_ERC_P1::ERC20;
    switch (erc) {
    case JUB_ENUM_APDU_ERC_ETH::ERC_721:
        p1 = JUB_ENUM_APDU_ERC_P1::ERC721;
    case JUB_ENUM_APDU_ERC_ETH::ERC_20:
        ins = JUB_ENUM_APDU_CMD::INS_SIGN_ERC20_C8;
        break;
    case JUB_ENUM_APDU_ERC_ETH::ERC_INVALID:
    default:
        break;
    }

    // subpackage
    {
        constexpr JUB_UINT32 kSendOnceLen = 230;
        JUB_ULONG offset = 23;

        //  first pack
        APDU apdu(0x00, 0xF8, 0x01, 0x00, offset, apduData.data());
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        unsigned long iCnt = (apduData.size() - offset) / kSendOnceLen;
        JUB_UINT32 iRemainder = (apduData.size() - offset) % kSendOnceLen;
        if (iCnt) {
            int bOnce = false;
            for (unsigned long i = 0; i < iCnt; ++i) {
                if ((i + 1) == iCnt && 0 == iRemainder) {
                    bOnce = true;
                }
                uchar_vector apduDataPart(&apduData[offset + i * kSendOnceLen], kSendOnceLen);
                JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, bOnce)); // last data or not.
            }
        }
        if (iRemainder) {
            uchar_vector apduDataPart(&apduData[offset + iCnt * kSendOnceLen], iRemainder);
            JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, true)); // last data.
        }
        apduData.clear();
    }

    // one pack can do it
    APDU apdu(0x00, ins, p1, JUB_ENUM_APDU_TYPED_p2::EIP2930, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    // finish transaction
    try {
        std::vector<JUB_BYTE> signatureRaw;
        signatureRaw.insert(signatureRaw.end(), retData, retData + ulRetDataLen);

        auto tx = TW::Ethereum::TransactionOptionalAccessList(vNonce, vGasPrice, vGasLimit, TW::Ethereum::Address{vTo},
                                                              vValue, vInput, accessListInJSON);
        // parse signature
        auto signature = TW::Ethereum::Signer::signatureDataToStruct(
            signatureRaw,
            false, // TW::Ethereum::TransactionNonTyped::usesReplayProtection() is handled by the hardware
            vChainID);
        if (!signature.isValid()) {
            return JUBR_SIGN_FAILED;
        }
        vRaw = tx.encoded(signature, vChainID);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::SignTX(const int erc, const std::vector<JUB_BYTE> &vNonce,
                                   const std::vector<JUB_BYTE> &vGasLimit,
                                   const std::vector<JUB_BYTE> &vMaxPriorityFeePerGas,
                                   const std::vector<JUB_BYTE> &vMaxFeePerGas,
                                   const std::vector<JUB_BYTE> &vDestination, const std::vector<JUB_BYTE> &vValue,
                                   const std::vector<JUB_BYTE> &vData, const std::string &accessListInJSON,
                                   const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vChainID,
                                   std::vector<JUB_BYTE> &vRaw) {

    // COS subpackage size 512 Byte
    uchar_vector apduData;
    using Tag = JUB_ENUM_APDU_DATA_ETH;
    // [path, chain_id, nonce, max_priority_fee_per_gas, max_fee_per_gas, gas_limit, destination, amount, data,
    // access_list]
    apduData << ToTlv(Tag::TAG_PATH_47, vPath);
    apduData << ToTlv(Tag::TAG_CHAIN_ID_48, vChainID);
    if (0x00 == vNonce[0]) {
        apduData << (JUB_BYTE)Tag::TAG_NONCE_41;
        apduData << (JUB_BYTE)0x00;
    } else {
        apduData << ToTlv(Tag::TAG_NONCE_41, vNonce);
    }

    apduData << ToTlv(Tag::MAX_PRIORITY_FEE_PER_GAS_54, vMaxPriorityFeePerGas);
    apduData << ToTlv(Tag::MAX_FEE_PER_GAS_55, vMaxFeePerGas);
    apduData << ToTlv(Tag::TAG_GAS_LIMIT_43, vGasLimit);
    apduData << ToTlv(Tag::TAG_TO_44, vDestination);

    // If value=0, when sending apdu,
    // it is clear that this part is empty
    uchar_vector vValueInWei(vValue);
    if (1 == vValueInWei.size() && 0 == vValueInWei[0]) {
        vValueInWei.clear();
    }
    apduData << ToTlv(Tag::TAG_VALUE_45, vValueInWei);

    apduData << ToTlv(Tag::TAG_INPUT_46, vData);

    auto accessList = TW::Ethereum::accessListFromJson(accessListInJSON);

    auto encoded = TW::Data();
    using RLP = TW::Ethereum::RLP;
    for (const auto &access : accessList) {
        auto item = RLP::encode(access.address.bytes);
        auto storageKeys = RLP::encodeList(access.storageKeys);
        std::copy(storageKeys.begin(), storageKeys.end(), std::back_inserter(item));
        auto itemList = RLP::encodeList(item);
        std::copy(itemList.begin(), itemList.end(), std::back_inserter(encoded));
    }
    apduData << ToTlv(Tag::TAG_ACCESS_LIST_53, RLP::encodeList(encoded));

    JUB_BYTE ins = JUB_ENUM_APDU_CMD::INS_SIGN_TX_2A;
    JUB_BYTE p1 = JUB_ENUM_APDU_ERC_P1::ERC20;
    switch (erc) {
    case JUB_ENUM_APDU_ERC_ETH::ERC_721:
        p1 = JUB_ENUM_APDU_ERC_P1::ERC721;
    case JUB_ENUM_APDU_ERC_ETH::ERC_20:
        ins = JUB_ENUM_APDU_CMD::INS_SIGN_ERC20_C8;
        break;
    case JUB_ENUM_APDU_ERC_ETH::ERC_INVALID:
    default:
        break;
    }

    // subpackage
    {
        constexpr JUB_UINT32 kSendOnceLen = 230;
        JUB_ULONG offset = 23;

        //  first pack
        APDU apdu(0x00, 0xF8, 0x01, 0x00, offset, apduData.data());
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        unsigned long iCnt = (apduData.size() - offset) / kSendOnceLen;
        JUB_UINT32 iRemainder = (apduData.size() - offset) % kSendOnceLen;
        if (iCnt) {
            int bOnce = false;
            for (unsigned long i = 0; i < iCnt; ++i) {
                if ((i + 1) == iCnt && 0 == iRemainder) {
                    bOnce = true;
                }
                uchar_vector apduDataPart(&apduData[offset + i * kSendOnceLen], kSendOnceLen);
                JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, bOnce)); // last data or not.
            }
        }
        if (iRemainder) {
            uchar_vector apduDataPart(&apduData[offset + iCnt * kSendOnceLen], iRemainder);
            JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, true)); // last data.
        }
        apduData.clear();
    }

    // one pack can do it
    APDU apdu(0x00, ins, p1, JUB_ENUM_APDU_TYPED_p2::EIP1559, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    // finish transaction
    try {
        std::vector<JUB_BYTE> signatureRaw;
        signatureRaw.insert(signatureRaw.end(), retData, retData + ulRetDataLen);

        auto tx =
            TW::Ethereum::TransactionEip1559(vNonce, vMaxPriorityFeePerGas, vMaxFeePerGas, vGasLimit,
                                             TW::Ethereum::Address{vDestination}, vValue, vData, accessListInJSON);
        // parse signature
        auto signature = TW::Ethereum::Signer::signatureDataToStruct(
            signatureRaw,
            false, // TW::Ethereum::TransactionNonTyped::usesReplayProtection() is handled by the hardware
            vChainID);
        if (!signature.isValid()) {
            return JUBR_SIGN_FAILED;
        }
        vRaw = tx.encoded(signature, vChainID);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::VerifyTX(const std::vector<JUB_BYTE> &vChainID, const std::string &path,
                                     const std::vector<JUB_BYTE> &vSigedTrans) {

    // verify signature
    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    std::string xpub;
    JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB, path, xpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey, hdVersionPub, hdVersionPrv));

    // verify signature
    return VerifyTx(vChainID, vSigedTrans, publicKey);
}

JUB_RV JubiterBladeETHImpl::SetERC20ETHTokens(const ERC20_TOKEN_INFO tokens[], const JUB_UINT16 iCount) {

    // ETH token extension apdu
    if (JubiterBladeETHImpl::_appletVersion >=
        stVersionExp::FromString(JubiterBladeETHImpl::ETH_APPLET_VERSION_SUPPORT_EXT_TOKENS)) {
        return SetERC20Tokens(tokens, iCount);
    } else if (1 == iCount) {
        return SetERC20ETHToken(tokens[0].tokenName, tokens[0].unitDP, tokens[0].contractAddress);
    }

    return JUBR_ARGUMENTS_BAD;
}

JUB_RV JubiterBladeETHImpl::SetERC20ETHToken(const std::string &tokenName, const JUB_UINT16 unitDP,
                                             const std::string &contractAddress) {

    // ERC20 token extension apdu
    if (typeid(JubiterBladeETHImpl) == typeid(*this)) {
        if (JubiterBladeETHImpl::_appletVersion < stVersionExp::FromString(ETH_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return SetERC20Token(tokenName.c_str(), unitDP, contractAddress.c_str());
}

JUB_RV JubiterBladeETHImpl::SetERC721ETHToken(const std::string &tokenName, const std::string &contractAddress) {

    // ERC20 token extension apdu
    if (typeid(JubiterBladeETHImpl) == typeid(*this)) {
        if (JubiterBladeETHImpl::_appletVersion < stVersionExp::FromString(ETH_APPLET_VERSION_SUPPORT_EXT_TOKEN)) {
            return JUBR_OK;
        }
    }

    return SetERC721Token(tokenName.c_str(), contractAddress.c_str());
}

JUB_RV JubiterBladeETHImpl::SignContract(const JUB_BYTE inputType, const std::vector<JUB_BYTE> &vNonce,
                                         const std::vector<JUB_BYTE> &vGasPrice, const std::vector<JUB_BYTE> &vGasLimit,
                                         const std::vector<JUB_BYTE> &vTo, const std::vector<JUB_BYTE> &vValue,
                                         const std::vector<JUB_BYTE> &vInput, const std::vector<JUB_BYTE> &vPath,
                                         const std::vector<JUB_BYTE> &vChainID, std::vector<JUB_BYTE> &vRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    uchar_vector apduData;

    if (0x00 == vNonce[0]) {
        apduData << (JUB_BYTE)JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41;
        apduData << (JUB_BYTE)0x00;
    } else {
        apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_NONCE_41, vNonce);
    }

    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_PRICE_42, vGasPrice);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_LIMIT_43, vGasLimit);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_TO_44, vTo);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_VALUE_45, vValue);
    // Too length to send at here
    //    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_INPUT_46, vInput);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CHAIN_ID_48, vChainID);

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
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_INPUT_46, tlvInput);
    unsigned long iCnt = apduData.size() / kSendOnceLen;
    JUB_UINT32 iRemainder = apduData.size() % kSendOnceLen;
    if (iCnt) {
        int bOnce = false;
        for (unsigned long i = 0; i < iCnt; ++i) {
            if ((i + 1) == iCnt && 0 == iRemainder) {
                bOnce = true;
            }
            uchar_vector apduDataPart(&apduData[i * kSendOnceLen], kSendOnceLen);
            JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, bOnce)); // last data or not.
        }
    }
    if (iRemainder) {
        uchar_vector apduDataPart(&apduData[iCnt * kSendOnceLen], iRemainder);
        JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, true)); // last data.
    }
    apduData.clear();

    //  sign transactions
    apdu.SetApdu(0x00, JUB_ENUM_APDU_CMD::INS_SIGN_CONTR_C9, 0x00, 0x00, 0);
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6f09 == ret) {
        return JUBR_USER_CANCEL;
    }
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    // finish transaction
    try {
        std::vector<JUB_BYTE> signatureRaw;
        signatureRaw.insert(signatureRaw.end(), retData, retData + ulRetDataLen);

        // parse signature
        TW::Ethereum::Signature signature = TW::Ethereum::Signer::signatureDataToStruct(
            signatureRaw,
            false, // TW::Ethereum::TransactionNonTyped::usesReplayProtection() is handled by the hardware
            vChainID);
        if (!signature.isValid()) {
            return JUBR_SIGN_FAILED;
        }

        vRaw.clear();
        vRaw =
            TW::Ethereum::TransactionNonTyped(vNonce, vGasPrice, vGasLimit, TW::Ethereum::Address(vTo), vValue, vInput)
                .encoded(signature, vChainID);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::SignContractHash(const JUB_BYTE inputType, const std::vector<JUB_BYTE> &vNonce,
                                             const std::vector<JUB_BYTE> &vGasPrice,
                                             const std::vector<JUB_BYTE> &vGasLimit, const std::vector<JUB_BYTE> &vTo,
                                             const std::vector<JUB_BYTE> &vValue, const std::vector<JUB_BYTE> &vInput,
                                             const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vChainID,
                                             std::vector<JUB_BYTE> &vRaw) {
    try {
        TW::Ethereum::TransactionNonTyped tx(vNonce, vGasPrice, vGasLimit, TW::Ethereum::Address(vTo), vValue, vInput);
        uchar_vector vPreimageHash = tx.preHash(vChainID);
        if (0 >= vPreimageHash.size()) {
            return JUBR_ARGUMENTS_BAD;
        }

        TW::Data vInputHash = TW::Hash::sha256(vInput);

        uchar_vector signatureRaw;
        JUB_VERIFY_RV(SignContractHash(jub::token::ENUM_CONTRACT_ABI::CREATE_CONTRACT, vGasLimit, vInputHash,
                                       vPreimageHash, vPath, vChainID, signatureRaw));

        // parse signature
        TW::Ethereum::Signature signature = TW::Ethereum::Signer::signatureDataToStruct(
            signatureRaw,
            false, // TW::Ethereum::TransactionNonTyped::usesReplayProtection() is handled by the hardware
            vChainID);
        if (!signature.isValid()) {
            return JUBR_SIGN_FAILED;
        }

        vRaw.clear();
        vRaw = tx.encoded(signature, vChainID);
    } catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::SignContractHash(const JUB_BYTE inputType, const std::vector<JUB_BYTE> &vGasLimit,
                                             const std::vector<JUB_BYTE> &vInputHash,
                                             const std::vector<JUB_BYTE> &vUnsignedTxHash,
                                             const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vChainID,
                                             std::vector<JUB_BYTE> &signatureRaw) {

    uchar_vector apduData;

    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_GAS_LIMIT_43, vGasLimit);
    apduData << ToTlv(0x07, vUnsignedTxHash);
    // Too length to send at here
    //    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_INPUT_46, vInput);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CHAIN_ID_48, vChainID);
    uchar_vector tlvInput;
    tlvInput << ToTlv(inputType, vInputHash);
    apduData << ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_INPUT_46, tlvInput);

    // one pack can do it
    APDU apdu(0x00, JUB_ENUM_APDU_CMD::INS_SIGN_CONTR_HASH_CA, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    signatureRaw.clear();
    signatureRaw.insert(signatureRaw.end(), retData, retData + ulRetDataLen);

    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::SignBytestring(const std::vector<JUB_BYTE> &vData, const std::vector<JUB_BYTE> &vPath,
                                           const std::vector<JUB_BYTE> &vChainID, std::vector<JUB_BYTE> &signatureRaw) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    uint16_t total = 0;

    uchar_vector pathTLV = ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_PATH_47, vPath);
    total += pathTLV.size();
    uchar_vector chainIdTLV = ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_CHAIN_ID_48, vChainID);
    total += chainIdTLV.size();
    uchar_vector dataTLV = ToTlv(JUB_ENUM_APDU_DATA_ETH::TAG_MSG_49, vData);
    total += dataTLV.size();

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

    apduData << dataTLV;
    unsigned long iCnt = apduData.size() / kSendOnceLen;
    JUB_UINT32 iRemainder = apduData.size() % kSendOnceLen;
    if (iCnt) {
        int bOnce = false;
        for (unsigned long i = 0; i < iCnt; ++i) {
            if ((i + 1) == iCnt && 0 == iRemainder) {
                bOnce = true;
            }
            uchar_vector apduDataPart(&apduData[i * kSendOnceLen], kSendOnceLen);
            JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, bOnce)); // last data or not.
        }
    }
    if (iRemainder) {
        uchar_vector apduDataPart(&apduData[iCnt * kSendOnceLen], iRemainder);
        JUB_VERIFY_RV(_TranPack(apduDataPart, 0x00, 0x00, kSendOnceLen, true)); // last data.
    }
    apduData.clear();

    //  sign transactions
    apdu.SetApdu(0x00, JUB_ENUM_APDU_CMD::INS_SIGN_MSG_CB, 0x00, 0x00, 0);
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
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

JUB_RV JubiterBladeETHImpl::VerifyBytestring(const std::string &path, const std::vector<JUB_BYTE> &vData,
                                             const std::vector<JUB_BYTE> &vSignature) {

    // verify signature
    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    std::string xpub;
    JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB, path, xpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey, hdVersionPub, hdVersionPrv));

    // verify signature
    return JubiterBaseETHImpl::VerifyBytestring(vData, vSignature, publicKey);
}

JUB_RV JubiterBladeETHImpl::SignTypedData(const bool &bMetamaskV4Compat, const std::string &typedDataInJSON,
                                          const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vChainID,
                                          std::vector<JUB_BYTE> &signatureRaw) {
    nlohmann::json typedData = nlohmann::json::parse(typedDataInJSON);
    if (nlohmann::detail::value_t::object != typedData.type()) {
        return JUBR_DATA_INVALID;
    }

    if (!jub::eth::EIP712::parseJSON(typedData)) {
        return JUBR_DATA_INVALID;
    }

    TW::Data domainSeparator =
        jub::eth::EIP712::typed_data_envelope("EIP712Domain", typedData["domain"], bMetamaskV4Compat);
    if (domainSeparator.empty()) {
        return JUBR_DATA_INVALID;
    }

    TW::Data hashStructMessage = jub::eth::EIP712::typed_data_envelope(
        typedData["primaryType"].get<std::string>().c_str(), typedData["message"], bMetamaskV4Compat);
    if (hashStructMessage.empty()) {
        return JUBR_DATA_INVALID;
    }

    auto domainName = typedData.at("domain").at("name").get<std::string>();
    auto name = TW::Data(domainName.size());
    std::copy(domainName.begin(), domainName.end(), name.begin());

    uint16_t total = 0;
    using Tag = JUB_ENUM_APDU_DATA_ETH;
    auto pathTLV = ToTlv(Tag::TAG_PATH_47, vPath);
    total += pathTLV.size();
    auto chainIdTLV = ToTlv(Tag::TAG_CHAIN_ID_48, vChainID);
    total += chainIdTLV.size();
    auto nameTLV = ToTlv(Tag::TAG_DOMAIN_NAME_50, name);
    total += nameTLV.size();
    auto domainTLV = ToTlv(Tag::TAG_DOMAIN_SEPARATOR_51, domainSeparator);
    total += domainTLV.size();
    auto msgHashTLV = ToTlv(Tag::TAG_STRUCT_MSG_HASH_52, hashStructMessage);
    total += msgHashTLV.size();

    uchar_vector apduData;
    // little ending
    apduData << total;
    // big ending
    apduData.reverse();

    for (auto &tlv : {pathTLV, chainIdTLV, nameTLV, domainTLV, msgHashTLV}) {
        apduData << tlv;
    }

    // can send by one apdu
    APDU apdu(0x00, 0xF8, 0x01, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    //  sign transactions
    apdu.SetApdu(0x00, JUB_ENUM_APDU_CMD::INS_SIGN_712_CC, 0x00, 0x00, 0);
    JUB_BYTE retData[2048] = {
        0,
    };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6f09 == ret) {
        return JUBR_USER_CANCEL;
    }
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    TW::Ethereum::TransactionTypedData tx(typedDataInJSON);
    auto sig = TW::Ethereum::Signature{
        {retData, retData + 32},      // r
        {retData + 32, retData + 64}, // s
        {retData + 64, retData + 65}, // v
    };
    signatureRaw = tx.encoded(sig);
    return JUBR_OK;
}

JUB_RV JubiterBladeETHImpl::VerifyTypedData(const bool &bMetamaskV4Compat, const std::string &path,
                                            const std::string &typedDataInJSON,
                                            const std::vector<JUB_BYTE> &vSignature) {
    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    std::string xpub;
    JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB, path, xpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey, hdVersionPub, hdVersionPrv));

    // verify signature
    return JubiterBaseETHImpl::VerifyTypedData(bMetamaskV4Compat, typedDataInJSON, vSignature, publicKey);
}

} // namespace token
} // namespace jub
