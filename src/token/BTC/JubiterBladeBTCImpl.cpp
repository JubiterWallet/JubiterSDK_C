#include <token/BTC/JubiterBladeBTCImpl.h>
#include <utility/util.h>
#include <token/ErrorHandler.h>
#include "BinaryCoding.h"

namespace jub {
namespace token {

constexpr JUB_BYTE PKIAID_BTC[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x42, 0x54, 0x43, 0x01
};

#define SWITCH_TO_BTC_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(PKIAID_BTC, 16));  \
} while (0)                                     \

JUB_RV JubiterBladeBTCImpl::SelectAppletBTC() {
    SWITCH_TO_BTC_APP;
    return JUBR_OK;
}

constexpr JUB_BYTE kMainnetP2PKH = 0x00;
//constexpr JUB_BYTE kMainnetP2SH = 0x01;
//constexpr JUB_BYTE kMainnetP2WPKH = 0x02;
//constexpr JUB_BYTE kMainnetP2WSH = 0x03;
constexpr JUB_BYTE kMainnetP2SH_P2WPKH = 0x04;
//constexpr JUB_BYTE kMainnetP2SH_P2WSH = 0x05;

#define APPLET_BTC_SUPPORT_LEGACY_ADDRESS_VERSION "01090205"

JUB_RV JubiterBladeBTCImpl::GetHDNodeBTC(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) {

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(0x08, vPath);
    JUB_BYTE p2 = 0x00;
    switch (type) {
    case p2pkh:
    {
        p2 = 0x00;   //xpub  format
        break;
    } // case p2pkh end
    case p2sh_p2wpkh:
    {
        p2 = 0x01;   //ypub  format
        break;
    } // case p2sh_p2wpkh end
    default:
        break;
    } // switch (type) end

    APDU apdu(0x00, 0xe6, 0x00, p2, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    xpub = (JUB_CHAR_PTR)retData;

    return JUBR_OK;
}

JUB_RV JubiterBladeBTCImpl::GetAddressBTC(const JUB_BYTE addrFmt,
                                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                                          const std::string& path,
                                          const JUB_UINT16 tag,
                                          std::string& address) {

    JUB_BYTE p1 = (JUB_BYTE)tag;
    if (_isSupportLegacyAddress()) {
        p1 |= _RealAddressFormat(addrFmt);
    }
    JUB_BYTE sigType;
    switch (type) {
    case p2pkh:
    {
        sigType = kMainnetP2PKH;
        break;
    } // case p2pkh end
    case p2sh_p2wpkh:
    {
        sigType = kMainnetP2SH_P2WPKH;
        break;
    } // case p2sh_p2wpkh end
    default:
        return JUBR_IMPL_NOT_SUPPORT;
    } // switch (type) end

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(0x08, vPath);
    APDU apdu(0x00, 0xf6, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    address = (JUB_CHAR_PTR)retData;

    return JUBR_OK;
}

bool JubiterBladeBTCImpl::_isSupportLegacyAddress() {

    if (0 <= _appletVersion.compare(APPLET_BTC_SUPPORT_LEGACY_ADDRESS_VERSION)) {
        return true;
    }

    return false;
};

JUB_BYTE JubiterBladeBTCImpl::_RealAddressFormat(const JUB_ULONG& addrFmt) {

    return _HighMark((JUB_ULONG)addrFmt);
}

JUB_BYTE JubiterBladeBTCImpl::_RealAddressFormat(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt) {

    return _RealAddressFormat((JUB_ULONG)addrFmt);
}

JUB_RV JubiterBladeBTCImpl::SetUnitBTC(const JUB_ENUM_BTC_UNIT_TYPE& unit) {

    APDU apdu(0x00, 0xfa, JUB_BYTE(unit), 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (0x9000 == ret) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBladeBTCImpl::SetCoinTypeBTC(const JUB_ENUM_COINTYPE_BTC& type) {

    APDU apdu(0x00, 0xf5, (JUB_BYTE)type, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (   0x9000 == ret
        || 0x6d00 == ret
        ) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBladeBTCImpl::SignTXBTC(const JUB_BYTE addrFmt,
                                      const JUB_ENUM_BTC_TRANS_TYPE& type,
                                      const JUB_UINT16 inputCount,
                                      const std::vector<JUB_UINT64>& vInputAmount,
                                      const std::vector<std::string>& vInputPath,
                                      const std::vector<JUB_UINT16>& vChangeIndex,
                                      const std::vector<std::string>& vChangePath,
                                      const std::vector<JUB_BYTE>& vUnsigedTrans,
                                      std::vector<JUB_BYTE>& vRaw) {
    //SWITCH_TO_BTC_APP

    constexpr JUB_UINT32 kSendOnceLen = 230;

    JUB_BYTE p1 = 0x01;
    if (_isSupportLegacyAddress()) {
        p1 |= _RealAddressFormat(addrFmt);
    }

    JUB_BYTE sigType;
    switch (type) {
    case p2pkh:
    {
        sigType = kMainnetP2PKH;
        break;
    } // case p2pkh end
    case p2sh_p2wpkh:
    {
        sigType = kMainnetP2SH_P2WPKH;
        break;
    } // case p2sh_p2wpkh end
    default:
        return JUBR_IMPL_NOT_SUPPORT;
    } // switch (type) end

    // number of input
    uchar_vector apduData;
    apduData << (JUB_BYTE)(inputCount);
    // ammountTLV
    uchar_vector amountTLV;
    for (auto amount : vInputAmount) {
        amountTLV << (uint64_t)amount;
    }

    apduData << ToTlv(0x0e, amountTLV);

    //  first pack
    APDU apdu(0x00, 0xF8, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);
    apduData.clear();

    // pathTLV
    uchar_vector pathLV;
    for (auto path : vInputPath) {
        pathLV << (JUB_BYTE)(path.size());
        pathLV << path;
    }

    apduData << ToTlv(0x0F, pathLV);

    JUB_BYTE highMark = 0x00;
    if (_isSupportLegacyAddress()) {
        highMark |= _RealAddressFormat(addrFmt);
    }
    JUB_VERIFY_RV(_TranPack(apduData, highMark, sigType, kSendOnceLen));
    apduData.clear();

    //tx TLV
    apduData << ToTlv(0x0D, vUnsigedTrans);

    JUB_VERIFY_RV(_TranPack(apduData, highMark, sigType, kSendOnceLen));
    apduData.clear();

    //change TLV
    uchar_vector changeLV;
    changeLV << (JUB_BYTE)vChangeIndex.size();
    for (size_t i = 0; i < vChangeIndex.size(); i++) {
        changeLV << (JUB_BYTE)vChangeIndex[i];
        changeLV << (JUB_BYTE)vChangePath[i].length();
        changeLV << vChangePath[i];
    }

    uchar_vector changeIndexTLV;
    changeIndexTLV = ToTlv(0x10, changeLV);
    apduData << changeIndexTLV;

    JUB_VERIFY_RV(_TranPack(apduData, highMark, sigType, kSendOnceLen, true)); // last data.
    apduData.clear();

    //  sign transactions
    JUB_BYTE retData[2] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    apdu.SetApdu(0x00, 0x2A, 0x00, sigType, 0);
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
    abcd::DataChunk sigRawTx(totalReadLen, 0x00);

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

    vRaw.clear();
    vRaw = sigRawTx;

    return JUBR_OK;
}

JUB_RV JubiterBladeBTCImpl::SetQRC20ETHToken(const std::string& tokenName,
                                             const JUB_UINT16 unitDP,
                                             const std::string& contractAddress) {

    uchar_vector lvName = Tollv(tokenName);
    uchar_vector address;
    address << ETHHexStr2CharPtr(contractAddress);

    uchar_vector data;
    data << (uint8_t)unitDP;
    data << (uint8_t)lvName.size();
    data << lvName;
    data << (uint8_t)address.size();
    data << address;

    APDU apdu(0x00, 0xc7, 0x00, 0x00, (JUB_ULONG)data.size(), data.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}

} // namespace token end
} // namespace jub end
