#include "token/DOT/JubiterBaseDOTImpl.h"

//#include <bigint/BigIntegerUtils.hh>
#include <uint256_t/uint256_t.h>
#include <TrezorCrypto/bip32.h>
#include <Polkadot/Address.h>
#include <Kusama/Address.h>
#include <PublicKey.h>
#include <../PrivateKey.h>
#include <Cbor.h>
#include <string>
#include "../BinaryCoding.h"

extern "C" {
#include "sr25519.h"
}

namespace jub {
namespace token {


JUB_RV JubiterBaseDOTImpl::IsValidAddress(const std::string& address, const TWCoinType& coin, const bool bTest) {

    if (TWCoinType::TWCoinTypePolkadot == coin) {
        return !TW::Polkadot::Address::isValid(address, bTest);
    }
    else if (TWCoinType::TWCoinTypeKusama == coin) {
        return !TW::Kusama::Address::isValid(address, bTest);
    }
    else {
        return JUBR_ARGUMENTS_BAD;
    }
}


JUB_RV JubiterBaseDOTImpl::CheckAddress(const std::string& address, const TWCoinType& coin, const TWCoinType& coinNet) {

    bool bTest = (TWCoinType::TWCoinTypeBitcoin == coinNet) ? false : true;
    return JubiterBaseDOTImpl::IsValidAddress(address, coin, bTest);
}


JUB_RV JubiterBaseDOTImpl::_getChainCodeFromPolkadptPath(std::string pathStr, std::string& chainCode) {

    if (isNumber(pathStr)) {
        long long number = atoll(pathStr.c_str());

        if (number > 255) { // if num More than 255, return little - Endian
            std::string hexStr = polkadot_string_to_hex(number);
            uint8_t *res = uchar_vector(hexStr).data();
            EndianSwap(res, hexStr.length());
            uint8_t endian[hexStr.length()/2];
            memcpy(endian, &res[0]+hexStr.length()/2, hexStr.length()/2);
            uchar_vector vEndian(endian, (unsigned int)sizeof(endian)/sizeof(uint8_t));
            chainCode.append(vEndian.getHex());
        }
        else {
            chainCode.append(polkadot_string_to_hex(number));
        }
    }
    else {
        std::string pathLen = std::to_string(pathStr.length() * 4);
        long long number = atoll(pathLen.c_str());
        chainCode.append(polkadot_string_to_hex(number));
        chainCode.append(get_raw_string(pathStr));
    }

    chainCode = appendZero(chainCode,SR25519_CHAINCODE_SIZE * 2);

    return JUBR_OK;
}


JUB_RV JubiterBaseDOTImpl::_getSr25519KeypairFromMasterKp(const std::string kp, std::string& derivPrv, std::string& derivPub, const std::string path, JUB_ENUM_CURVES curve) {

    std::vector<std::string> pathSeveralVer;
    std::vector<bool> isHardVer;

    bool isSucceed = chainCodeFromPolkadotPath(path, curve, pathSeveralVer, isHardVer);
    if (!isSucceed) {
        return JUBR_ERROR;
    }

    uint8_t privateKey[SR25519_SECRET_SIZE] = {0x00,};
    uint8_t  publicKey[SR25519_PUBLIC_SIZE] = {0x00,};
    std::vector<uint8_t> kpOut(SR25519_KEYPAIR_SIZE, 0);
    std::vector<uint8_t> kpIn( SR25519_KEYPAIR_SIZE, 0);
    kpIn = uchar_vector(kp);
    std::string chainCode;

    for (int i = 0; i < pathSeveralVer.size(); i++) {
        memset(privateKey, 0, SR25519_SECRET_SIZE);
        memset( publicKey, 0, SR25519_PUBLIC_SIZE);
        std::string pathStr = pathSeveralVer[i];
        chainCode = "";

        _getChainCodeFromPolkadptPath(pathStr, chainCode);

        if (true == isHardVer[i]) {
            sr25519_derive_keypair_hard(kpOut.data(), kpIn.data(), uchar_vector(chainCode).data());
        }
        else {
            sr25519_derive_keypair_soft(kpOut.data(), kpIn.data(), uchar_vector(chainCode).data());
        }
        if (SR25519_KEYPAIR_SIZE != kpOut.size()) {
            return JUBR_ERROR;
        }

        memcpy(privateKey, &kpOut[0], SR25519_SECRET_SIZE);
        memcpy( publicKey, &kpOut[0] + SR25519_SECRET_SIZE, SR25519_PUBLIC_SIZE);

        uchar_vector vPublicKey(publicKey, sizeof(publicKey)/sizeof(uint8_t));
        uchar_vector vprivateKey(privateKey, sizeof(privateKey)/sizeof(uint8_t));

        derivPub = vPublicKey.getHex() ;
        derivPrv = vprivateKey.getHex();

        kpIn = uchar_vector(kpOut);
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseDOTImpl::_getEd25519PrvKeyFromMasterKey(const std::string prvKey, std::string& derivPrv, std::string& derivPub, const std::string path, JUB_ENUM_CURVES curve) {

    std::vector<std::string> pathSeveralVer;
    std::vector<bool> isHardVer;
    std::string prvIn = prvKey;
    bool isSucceed = chainCodeFromPolkadotPath(path, curve, pathSeveralVer, isHardVer);
    if (!isSucceed) {
        return JUBR_ERROR;
    }

    std::string chainCode;
    //"Ed25519HDKD" is ed25519 fixed value
    std::string HDKDValue = "Ed25519HDKD";
    std::string length = polkadot_string_to_hex(HDKDValue.length() << 2);
    std::string HDKD = get_raw_string(HDKDValue);

    for (int i = 0; i < pathSeveralVer.size(); i++) {
        std::string pathStr = pathSeveralVer[i];
        chainCode = "";
        _getChainCodeFromPolkadptPath(pathStr, chainCode);

        //msg = length(HDKD) + HDKDStr + chainCode + prv
        std::string msg = "";
        msg.append(length);
        msg.append(HDKD);
        msg.append(prvIn);
        msg.append(chainCode);

        uint8_t out[32] = {0x00,};
        blake2b(uchar_vector(msg).data(), (unsigned int)msg.length()/2, &out, 32);
        uchar_vector prvData(out, sizeof(out)/sizeof(uint8_t));
        prvIn = prvData.getHex();

        auto privateKey = TW::PrivateKey(prvData);
        TW::PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

        derivPub = uchar_vector( publicKey.bytes).getHex();
        derivPrv = uchar_vector(privateKey.bytes).getHex();
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseDOTImpl::_getAddress(const TW::Data& publicKey, std::string& address, const TWCoinType &coinNet) {

    try {
        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);
        if (!twpk.isValid()) {
            return JUBR_ARGUMENTS_BAD;
        }

        bool bTest = (TWCoinType::TWCoinTypeBitcoinTestNet == coinNet) ? true : false;

        std::string addr;
        switch (_coin) {
        case TWCoinType::TWCoinTypePolkadot:
        {
            addr = TW::Polkadot::Address(twpk, bTest).string();
            if (!TW::Polkadot::Address::isValid(addr, bTest)) {
                return JUBR_ERROR;
            }
            break;
        }
        case TWCoinType::TWCoinTypeKusama:
        {
            addr = TW::Kusama::Address(twpk, bTest).string();
            if (!TW::Kusama::Address::isValid(addr, bTest)) {
                return JUBR_ERROR;
            }
            break;
        }
        default:
            return JUBR_ERROR;
        }

        address = addr;
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
