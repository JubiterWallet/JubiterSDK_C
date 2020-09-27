#include <token/JubiterNFC/JubiterNFCToken.h>
#include <device/DeviceTypeBase.hpp>
#include <device/JubiterHidDevice.hpp>
#include <utility/util.h>
#include <token/ErrorHandler.h>
#include <TrezorCrypto/bip39.h>
#include "scp03/scp03.hpp"
#include "scp11/scp11c.hpp"


namespace jub {
namespace token {


stAppInfos JubiterNFCToken::g_appInfo[] = {
//    {
//        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
//        "NFC",
//        "0000000"
//    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "BTC",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "ETH",
        "0000000"
    },
    // BTC and ETH index position fixed, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "ETC",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "BCH",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "LTC",
        "0000000",
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "USDT",
        "0000000"
    },
    // MISC applet, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "EOS",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "XRP",
        "0000000"
    },
};


JubiterNFCToken::JubiterNFCToken(JUB_UINT16 deviceID)
    : JubiterBladeToken(deviceID) {

}


JUB_RV JubiterNFCToken::_SendApdu(const APDU *apdu, JUB_UINT16 &wRet, JUB_BYTE *retData /*= nullptr*/,
                                  JUB_ULONG *pulRetDataLen /*= nullptr*/,
                                  JUB_ULONG ulMiliSecondTimeout /*= 0*/) {

    auto device = jub::device::DeviceManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(device);

    auto scp03Ptr = (scp03*)device->GetSCP03();
    JUB_CHECK_NULL(scp03Ptr);

    auto scp11Ptr = (scp11*)device->GetSCP11();
    JUB_CHECK_NULL(scp11Ptr);

    if (!scp11Ptr->isOpen()) {
        return JubiterBladeToken::_SendApdu(apdu,
                                            wRet, retData, pulRetDataLen,
                                            ulMiliSecondTimeout);
    }
    else {
        return _SendSafeApdu(apdu,
                             wRet, retData, pulRetDataLen,
                             ulMiliSecondTimeout);
    }
}


JUB_RV JubiterNFCToken::_SendSafeApdu(const APDU *apdu, JUB_UINT16 &wRet, JUB_BYTE *retData /*= nullptr*/,
                                      JUB_ULONG *pulRetDataLen /*= nullptr*/,
                                      JUB_ULONG ulMiliSecondTimeout /*= 0*/) {

    auto device = jub::device::DeviceManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(device);

    auto scp03Ptr = (scp03*)device->GetSCP03();
    JUB_CHECK_NULL(scp03Ptr);

    auto scp11Ptr = (scp11*)device->GetSCP11();
    JUB_CHECK_NULL(scp11Ptr);

    if (!scp11Ptr->isOpen()) {
        JUB_VERIFY_RV(scp11Ptr->initialize() ? JUBR_OK : JUBR_ERROR);
        JUB_VERIFY_RV(PerformSecurityOperation(scp11Ptr->getOCECert()));
        uchar_vector vResponse;
        JUB_VERIFY_RV(MutualAuthenticate(scp11Ptr->getMutualAuthData(), vResponse));

        JUB_VERIFY_RV(scp11Ptr->openSecureChannel(vResponse) ? JUBR_OK : JUBR_ERROR);
        scp03Ptr->macChaining(scp11Ptr->getMacChain());
    }

    JUB_BYTE _retData[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = { 0, };
    JUB_ULONG ulRetDataLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;

    if (JUBR_OK != _apduBuilder->SetSCP03(scp03Ptr)) {
        return JUBR_ARGUMENTS_BAD;
    };

    if (JUBR_OK != _apduBuilder->SetSCP11(scp11Ptr)) {
        return JUBR_ARGUMENTS_BAD;
    };

    std::vector<JUB_BYTE> vSendApdu;
    if (JUBR_OK != _apduBuilder->BuildSafeApdu(apdu, vSendApdu)) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    if (JUBR_OK != device->SendData(vSendApdu.data(), (JUB_ULONG)vSendApdu.size(), _retData, &ulRetDataLen, ulMiliSecondTimeout)) {
        JUB_VERIFY_RV(JUBR_TRANSMIT_DEVICE_ERROR);
    }

    JUB_VERIFY_RV(_apduBuilder->ParseSafeApduResp(_retData, ulRetDataLen,
                                                  retData, pulRetDataLen,
                                                  wRet));
    return JUBR_OK;
}


//JUB_RV JubiterNFCToken::_TranPack(const TW::Data &apduData, const JUB_BYTE highMark, const JUB_BYTE sigType, const JUB_ULONG ulSendOnceLen, int finalData/* = false*/, int bOnce/* = false*/) {
//
//    if (apduData.empty()) {
//        JUB_VERIFY_RV(JUBR_ERROR);
//    }
//
//    JUB_UINT16 ret = 0;
//    if (bOnce) {
//        // one pack enough
//        JUB_BYTE p1 = 0x00;
//        p1 |= highMark;
//        APDU apdu(0x00, 0xF8, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//
//        return JUBR_OK;
//    }
//
//    // else send pack by pack
//    auto nextTimes = apduData.size() / ulSendOnceLen;
//    auto left = apduData.size() % ulSendOnceLen;
//
//    // split last pack
//    if (   0 == left
//        && 0 != nextTimes
//        ) {
//        nextTimes--;
//        left = ulSendOnceLen;
//    }
//
//    // pack by pack
//    JUB_BYTE p1 = 0x02;
//    p1 |= highMark;
//    APDU apdu(0x00, 0xF8, p1, sigType, 0x00);
//    apdu.lc = ulSendOnceLen;
//    JUB_UINT32 times = 0;
//    for (times = 0; times < nextTimes; times++) {
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    // next pack
//    apdu.lc = (JUB_ULONG)left;
//    if (apdu.lc) {
//        if (finalData) {
//            apdu.p1 = 0x03;
//            apdu.p1 |= highMark;
//        }
//
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    return JUBR_OK;
//}
//
//
//JUB_RV JubiterNFCToken::_TranPackApdu(const JUB_ULONG ncla, const JUB_ULONG nins,
//                                      const TW::Data &apduData,
//                                      const JUB_BYTE highMark,
//                                      const JUB_BYTE sigType,
//                                      const JUB_ULONG ulSendOnceLen,
//                                      JUB_BYTE *retData/* = nullptr*/, JUB_ULONG *pulRetDataLen/* = nullptr*/,
//                                      int finalData/* = false*/, int bOnce/* = false*/) {
//
//    if (apduData.empty()) {
//        JUB_VERIFY_RV(JUBR_ERROR);
//    }
//
//    JUB_UINT16 ret = 0;
//    if (bOnce) {
//        // one pack enough
//        JUB_BYTE p1 = 0x00;
//        p1 |= highMark;
//        APDU apdu(ncla, nins, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//
//        return JUBR_OK;
//    }
//
//    // else send pack by pack
//    auto nextTimes = apduData.size() / ulSendOnceLen;
//    auto left = apduData.size() % ulSendOnceLen;
//
//    // split last pack
//    if (   0 == left
//        && 0 != nextTimes
//        ) {
//        nextTimes--;
//        left = ulSendOnceLen;
//    }
//
//    // pack by pack
//    JUB_BYTE p1 = 0x02;
//    p1 |= highMark;
//    APDU apdu(ncla, nins, p1, sigType, 0x00);
//    apdu.lc = ulSendOnceLen;
//    JUB_UINT32 times = 0;
//    for (times = 0; times < nextTimes; times++) {
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    // next pack
//    apdu.lc = (JUB_ULONG)left;
//    if (apdu.lc) {
//        if (finalData) {
//            apdu.p1 = 0x03;
//            apdu.p1 |= highMark;
//        }
//
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, pulRetDataLen));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    return JUBR_OK;
//}
//
//
JUB_RV JubiterNFCToken::QueryBattery(JUB_BYTE &percent) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCToken::ShowVirtualPwd() {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCToken::CancelVirtualPwd() {

    return JUBR_IMPL_NOT_SUPPORT;
}


bool JubiterNFCToken::IsBootLoader() {

    return (JUBR_OK == SelectMainSecurityDomain() ? true:false);
}


JUB_RV JubiterNFCToken::SelectMainSecurityDomain() {

    // select main safe scope
    APDU apdu(0x00, 0xA4, 0x04, 0x00, sizeof(kPKIAID_NFC_DOMAIN)/sizeof(JUB_BYTE), kPKIAID_NFC_DOMAIN);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    // Clear SCP11c Session for NFC
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(device);
    device->Reset();

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::GetPinRetry(JUB_BYTE& retry) {

    // When pinMaxRetry == retry, and JUBR_PIN_LOCKED == rv
    // it means the NFC card is already be reset.
    JUB_RV rv = JubiterBladeToken::GetPinRetry(retry);
    if (   0x6982 == rv
        || 0x6985 == rv
        ) {
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }

    return rv;
}


JUB_RV JubiterNFCToken::GetBleVersion(JUB_BYTE bleVersion[4]) {

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::EnumApplet(std::string& appletList) {

    // send apdu, then decide which coin types supports.
    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8106")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    std::vector<uint8_t> tlvData(retData, retData + ulRetDataLen);
    auto appList = ParseTlv(tlvData);

    for (auto appID : appList) {
        uchar_vector id(appID);
        appletList += id.getHex();
        appletList += " ";
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::GetAppletVersionBlade(const std::string& appID, std::string& version) {

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::EnumSupportCoins(std::string& coinList) {

    std::string appletList;
    JUB_VERIFY_RV(EnumApplet(appletList));

    std::vector<std::string> coinNameList;
    auto vAppList = Split(appletList, " ");
    for (auto appID : vAppList) {
        for (auto appInfo : JubiterNFCToken::g_appInfo) {
            uchar_vector _appID(appInfo.appID);
            if (_appID.getHex() != appID) {
                continue;
            }
            if (coinNameList.end() == std::find(coinNameList.begin(), coinNameList.end(), appInfo.coinName)) {
                coinList += appInfo.coinName;
                coinList += " ";
                coinNameList.insert(coinNameList.end(), appInfo.coinName);
            }
        }
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) {

    std::vector<uint8_t> pin;
    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pin),
                   [](const char elem) {
        return (uint8_t)elem;
    });

    uchar_vector apduData = tlv_buf(pin).encodeLV();
    APDU apdu(0x80, 0x20, 0x00, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) { //locked
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }
    if (0x63C0 == (ret & 0xfff0)) {
        retry = (ret & 0xf);
        JUB_VERIFY_RV(JUBR_DEVICE_PIN_ERROR);
    }
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::ChangePIN(const std::string &pinMix, const std::string &pinNew) {

    std::vector<uint8_t> pinOld;
    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pinOld),
                   [](const char elem) {
        return (uint8_t)elem;
    });

    std::vector<uint8_t> pin;
    std::transform(pinNew.begin(),
                   pinNew.end(),
                   std::back_inserter(pin),
                   [](const char elem) {
        return (uint8_t)elem;
    });

    uchar_vector subDataLV;
    subDataLV << tlv_buf(pinOld).encodeTBAV() << tlv_buf(pin).encodeTBAV();
    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8204, subDataLV).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret, retData, &ulRetDataLen));
    if (   0x6982 == ret
        || 0x6985 == ret
        ) {
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }
    if (   0x9B01 == ret
        || 0x6A80 == ret
        ) {// the length or value of PIN is incorrect or verified failed
        JUB_VERIFY_RV(JUBR_DEVICE_PIN_ERROR);
    }
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::SetTimeout(const JUB_UINT16 timeout) {

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::Reset() {

    // reset
    uchar_vector apduData = tlv_buf(0xDFFE, uchar_vector("8205")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::GenerateSeed(const std::string& pinMix,
                                     const JUB_ENUM_CURVES& curve) {

    switch (curve) {
    case JUB_ENUM_CURVES::SECP256K1:
        break;
    case JUB_ENUM_CURVES::NIST256P1:
    case JUB_ENUM_CURVES::ED25519:
    default:
        return JUBR_ARGUMENTS_BAD;
    }   // switch (curve) end

    // send apdu.
    std::vector<uint8_t> pin;
    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pin),
                   [](const char elem) {
        return (uint8_t)elem;
    });

    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8203, tlv_buf(pin).encodeLV()).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterNFCToken::ImportMnemonic(const std::string& pinMix,
                                       const std::string& mnemonic) {

    std::vector<std::string> words = Split(mnemonic, " ");
    JUB_ENUM_MNEMONIC_STRENGTH strength;
    int entropy_len = 0;
    switch (words.size()) {
    case 12:
    case 15:
    case 18:
        strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH192;
        entropy_len = 0x18;
        break;
    case 21:
    case 24:
        strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH256;
        entropy_len = 0x20;
        break;
    default:
        return JUBR_ARGUMENTS_BAD;
    }   // switch (words.size()) end

    uint8_t entropy[32+1] = {0x0,};
    int seed_len = mnemonic_to_entropy(mnemonic.c_str(), entropy);
    if (0 == seed_len) {
        return JUBR_ARGUMENTS_BAD;
    }
    uint8_t seed[512 / 8] = {0x0,};
    const char* passphrase = "";//"Bitcoin seed";
    mnemonic_to_seed(mnemonic.c_str(), passphrase, seed, 0);

    return _SetMnemonic(pinMix,
                        strength,
                        uchar_vector(entropy, entropy+entropy_len).getHex(),
                        uchar_vector(seed, seed+(512 / 8)).getHex());
}


JUB_RV JubiterNFCToken::_SetMnemonic(const std::string& pinMix,
                                     const JUB_ENUM_MNEMONIC_STRENGTH& strength,
                                     const std::string& entropy,
                                     const std::string& seed) {

    std::vector<uint8_t> vEntropy = HexStr2CharPtr(entropy);
    if ((size_t)strength/8 != vEntropy.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<uint8_t> vSeed = HexStr2CharPtr(seed);
    if (0x40 != vSeed.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    // send apdu.
    std::vector<uint8_t> pin;
    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pin),
                   [](const char elem) {
        return (uint8_t)elem;
    });

    uchar_vector subDataLV;
    subDataLV << tlv_buf(pin).encodeLV();
    subDataLV << tlv_buf(vEntropy).encodeLV();
    subDataLV << tlv_buf(vSeed).encodeLV();
    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8202, subDataLV).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::ExportMnemonic(const std::string& pinMix,
                                       OUT std::string& mnemonic) {

    std::string entropy;
    JUB_VERIFY_RV(_GetEntropy(pinMix, entropy));

    // remove '00'
    uchar_vector vEntropy(entropy);
    for (size_t i=vEntropy.size()-1; i>0; --i) {
        if (0x00 == vEntropy[i]) {
            vEntropy.pop_back();
        }
        else {
            break;
        }
    }

    while (0 != (vEntropy.size() % 4)) {
        vEntropy.pop_back();
    }

    char vMnemonic[512] = {0x00,};
    if(!mnemonic_from_data(&vEntropy[0], vEntropy.size(), vMnemonic)) {
        return JUBR_ARGUMENTS_BAD;
    }
    mnemonic = vMnemonic;

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::_GetEntropy(const std::string& pinMix,
                                    OUT std::string& entropy) {

    // send apdu.
    std::vector<uint8_t> pin;
    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pin),
                   [](const char elem) {
        return (uint8_t)elem;
    });

    // send apdu.
    uchar_vector apduData = tlv_buf(0xDFFF, tlv_buf(0x8202, tlv_buf(pin).encodeLV()).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        uchar_vector vEntropy(retData, (unsigned int)ulRetDataLen);
        entropy = vEntropy.getHex();

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


bool JubiterNFCToken::HasRootKey() {

    // send apdu
    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8105")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret
        ||   1 != ulRetDataLen
        ) {
        return false;
    }

    //        ‘5A’ – root key has been generated
    // Other value – root key has not been generated
    if (0x5A != retData[0]) {
        return false;
    }

    return true;
}


JUB_RV JubiterNFCToken::PerformSecurityOperation(const uchar_vector& oceCert) {

    if (0 == oceCert.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    uchar_vector apduData(oceCert);
    APDU apdu(0x80, 0x2A, 0x18, 0x10, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterNFCToken::MutualAuthenticate(const uchar_vector& apduData,
                                           uchar_vector& receipt) {

    if (0 == apduData.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    APDU apdu(0x80, 0x82, 0x18, 0x15, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        uchar_vector vReceipt(retData, retData+ulRetDataLen);
        receipt = vReceipt;
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


} // namespace token end
} // namespace jub end
