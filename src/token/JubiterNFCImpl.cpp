
#include "JUB_SDK_COMM.h"
#if defined(NFC_MODE)
#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <utility/util.h>
#include <token/ErrorHandler.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include "BinaryCoding.h"
#include <Bitcoin/Address.h>
#include <Bitcoin/SegwitAddress.h>
#include <Bitcoin/Script.h>
#include <PrivateKey.h>

namespace jub {
namespace token {


#define SWITCH_TO_NFC_APP                       \
do {				                            \
    JUB_VERIFY_RV(SelectMainSecurityDomain());  \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE)));\
} while (0)                                     \


JUB_RV JubiterNFCImpl::SelectApplet() {
    SWITCH_TO_NFC_APP;
    return JUBR_OK;
}


JUB_RV JubiterNFCImpl::GetHDNode(const JUB_BYTE& curveType,
                                 const JUB_BYTE& type,
                                 const std::string& path,
                                 std::string& xpub) {

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(TAG_PATH_08, vPath);

    APDU apdu(0x00, 0xE6, curveType, type, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    if (_isOpenSecureChannel()) {
        JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret, retData, &ulRetDataLen));
    }
    else {
        JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    }
    JUB_VERIFY_COS_ERROR(ret);

    xpub = (JUB_CHAR_PTR)retData;

    return JUBR_OK;
}


JUB_RV JubiterNFCImpl::GetCompPubKey(const JUB_BYTE& curveType,
                                     const JUB_BYTE& type,
                                     const std::string& path,
                                     TW::Data& pubkey) {

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(TAG_PATH_08, vPath);

    APDU apdu(0x00, 0xF6, curveType, type, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    if (_isOpenSecureChannel()) {
        JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret, retData, &ulRetDataLen));
    }
    else {
        JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    }
    JUB_VERIFY_COS_ERROR(ret);

    pubkey.insert(pubkey.end(), retData, retData+ulRetDataLen);

    return JUBR_OK;
}


JUB_RV JubiterNFCImpl::GetAddress(const JUB_BYTE addrFmt,
                                  const JUB_ENUM_BTC_TRANS_TYPE& type,
                                  const std::string& path,
                                  const JUB_UINT16 tag,
                                  std::string& address) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCImpl::SignTX(const JUB_UINT16 inputCount,
                              const std::vector<std::string>& vInputPath,
                              const JUB_BYTE& curveType,
                              const JUB_BYTE&  hashType,
                              const std::vector<TW::Data>& vPreImageHash,
                              std::vector<TW::Data>& vRSV) {

    JUB_RV rv = JUBR_ERROR;

    for (JUB_UINT16 i=1; i<=inputCount; ++i) {
        TW::Data rsv;
        rv = SignOne(inputCount,
                     i,
                     vInputPath[i-1],
                     curveType,
                      hashType,
                     vPreImageHash[i-1],
                     rsv);
        if (JUBR_OK != rv) {
            return rv;
        }

        vRSV.push_back(rsv);
    }

    return rv;
}


JUB_RV JubiterNFCImpl::SignOne(const JUB_UINT16 inputCount,
                               const JUB_UINT16 inputIndex,
                               const std::string& path,
                               const JUB_BYTE& signType,
                               const JUB_BYTE& hashType,
                               const TW::Data& preImageHash,
                               TW::Data& rsv) {

    constexpr JUB_UINT32 kSendOnceLen = 230;

    JUB_BYTE p1 = signType;
    JUB_BYTE p2 = hashType;

    // number of input
    uchar_vector apduData;
    apduData << (JUB_BYTE)(inputCount);
    apduData << (JUB_BYTE)(inputIndex);

    uchar_vector preImageHashLV;
    preImageHashLV << (JUB_BYTE)(preImageHash.size());
    preImageHashLV << preImageHash;
    apduData << ToTlv(0x07, preImageHashLV);

    //  first pack
    APDU apdu(0x00, 0xF8, 0x00, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendSafeApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);
    apduData.clear();

    // pathTLV
    uchar_vector pathLV;
    pathLV << (JUB_BYTE)(path.size());
    pathLV << path;
    apduData << ToTlv(TAG_PATH_08, pathLV);

    JUB_VERIFY_RV(_TranPack(apduData, 0x00, 0x00, kSendOnceLen, true)); // last data.
    apduData.clear();

    // sign transactions
    JUB_BYTE retData[kSendOnceLen] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    //one pack can do it
    apdu.SetApdu(0x00, 0xCB, p1, p2, 0);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6f09 == ret) {
        return JUBR_USER_CANCEL;
    }
    JUB_VERIFY_COS_ERROR(ret);

    JUB_UINT16 totalReadLen = ulRetDataLen;
    TW::Data totalData;
    totalData.insert(totalData.end(), retData, retData + ulRetDataLen);

    JUB_UINT32 rsvLVLen = 66;
    if (0 != (totalReadLen%rsvLVLen)) {
        return JUBR_ERROR;
    }
    JUB_UINT16 times = totalReadLen / rsvLVLen;
    for (times = 0; times < (totalReadLen / rsvLVLen); times++) {
        JUB_UINT16 offset = times * rsvLVLen;
        JUB_UINT32 rsvLen = totalData[offset];
        rsv.insert(rsv.end(), totalData.data()+offset+1, totalData.data()+offset+1+rsvLen);
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCImpl::SignBytestring(const JUB_UINT16 inputCount,
                                      const std::vector<std::string>& vInputPath,
                                      const JUB_BYTE& signType,
                                      const JUB_BYTE& hashType,
                                      const std::vector<TW::Data>& vPreImageHash,
                                      std::vector<TW::Data>& vRSV) {
    return JUBR_IMPL_NOT_SUPPORT;
}



JUB_BYTE JubiterNFCImpl::_getSignType(const char *curve_name) {

    if (0 == strcmp(curve_name, NIST256P1_NAME)) {
        return JUB_ENUM_CURVES::NIST256P1;
    }
    if (0 == strcmp(curve_name, ED25519_NAME)) {
        return JUB_ENUM_CURVES::ED25519;
    }

    return JUB_ENUM_CURVES::SECP256K1;
}


JUB_BYTE JubiterNFCImpl::_getHalfHasher(const HasherType hasherType, TW::Hash::Hasher& halfHasher) {
    JUB_BYTE halfHasherType = 0xEE; // Identifiers do not hash

    switch (hasherType) {
    case HASHER_SHA2D:
        halfHasherType = (JUB_BYTE)HASHER_SHA2;
    case HASHER_SHA2:
        halfHasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::sha256);
        break;
    case HASHER_SHA2_RIPEMD:
//        halfHasherType = HASHER_SHA2_RIPEMD;
        halfHasher = TW::Hash::sha256ripemd;
        break;
    case HASHER_SHA3:
//        halfHasherType = HASHER_SHA3;
        halfHasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::sha512);
        break;
    case HASHER_BLAKED:
        halfHasherType = HASHER_BLAKE;
    case HASHER_BLAKE:
        halfHasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::blake256);
        break;
    case HASHER_BLAKE_RIPEMD:
//        halfHasherType = HASHER_BLAKE_RIPEMD;
        halfHasher = TW::Hash::blake256ripemd;
        break;
    case HASHER_GROESTLD_TRUNC:
//        halfHasherType = HASHER_GROESTLD_TRUNC;
        halfHasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::groestl512);
        break;
    case HASHER_BLAKE2B:
//        halfHasherType = HASHER_BLAKE2B;
    case HASHER_BLAKE2B_PERSONAL:
//        halfHasherType = HASHER_BLAKE2B_PERSONAL;
    case HASHER_SHA2_KECCAK:
    case HASHER_SHA3_KECCAK:
    default:
        break;
    }   // switch (hasherType) end

    return halfHasherType;
}


} // namespace token end
} // namespace jub end

#endif //end NFC_MODE