/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "token/TrezorCryptoImpl.h"
#include "HDKey/HDKey.hpp"
#include <TrezorCrypto/bip32.h>
#include "TrezorCrypto/curves.h"
#include <utility/util.h>
#include <libETH/RLP.h>
#include <TrezorCrypto/sha3.h>
#include <libETH/AddressChecksum.h>

#include "HexCoding.h"
#include "PublicKey.h"
#include "EOS/Address.h"
#include "EOS/Signer.h"
#include "EOS/Transaction.h"
#include "Ripple/Signer.h"
#include "Ripple/Transaction.h"

namespace jub {

TrezorCryptoImpl::TrezorCryptoImpl(const std::string& masterkey_XPRV) {
    _MasterKey_XPRV = masterkey_XPRV;
}

JUB_RV TrezorCryptoImpl::ConnectToken() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::DisconnectToken() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::selectApplet_HC() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletBTC() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetHDNodeBTC(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, bitcoinXPUB, bitcoinXPRV, &hdkey, &parentFingerprint));

    JUB_CHAR _xpub[200] = {0,};
    hdnode_fill_public_key(&hdkey);
    JUB_UINT32 version = bitcoinXPUB;
    if (p2sh_p2wpkh == type) {
        version = bitcoinYPUB;
    }

    if(0 == hdnode_serialize_public(&hdkey, parentFingerprint, version, _xpub, sizeof(_xpub)/sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    xpub = std::string(_xpub);
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAddressBTC(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path.c_str(), SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

    JUB_CHAR _address[200] = {0,};
    hdnode_fill_public_key(&hdkey);

    hdnode_get_address(&hdkey, 0x00, _address, sizeof(_address)/sizeof(JUB_CHAR));
    address = _address;

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetUnitBTC(const JUB_ENUM_BTC_UNIT_TYPE& unit) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetCoinTypeBTC(const JUB_ENUM_COINTYPE_BTC& type) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SignTXBTC(const JUB_BYTE addrFmt,
                                   const JUB_ENUM_BTC_TRANS_TYPE& type,
                                   const JUB_UINT16 inputCount,
                                   const std::vector<JUB_UINT64>& vInputAmount,
                                   const std::vector<std::string>& vInputPath,
                                   const std::vector<JUB_UINT16>& vChangeIndex,
                                   const std::vector<std::string>& vChangePath,
                                   const std::vector<JUB_BYTE>& vUnsigedTrans,
                                   std::vector<JUB_BYTE>& vRaw) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletETH() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAppletVersionETH(std::string& version) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAddressETH(const std::string& path, const JUB_UINT16 tag, std::string& address) {
    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));
    
    JUB_BYTE ethKeyHash[20] = {0,};
    if (1 == hdnode_get_ethereum_pubkeyhash(&hdkey,ethKeyHash)) {
        uchar_vector _address(ethKeyHash, ethKeyHash+20);
        address = jub::eth::checksumed(_address.getHex(), jub::eth::eip55);
        return JUBR_OK;
    }
    return JUBR_ERROR;
}

JUB_RV TrezorCryptoImpl::GetHDNodeETH(const JUB_BYTE format, const std::string& path, std::string& pubkey) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

//    typedef enum {
//        HEX = 0x00,
//        XPUB = 0x01
//    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) {//hex
        uchar_vector pk(hdkey.public_key, hdkey.public_key+33);
        pubkey = pk.getHex();
        pubkey = ETH_PRDFIX + pubkey;
    }
    else { //xpub
        JUB_CHAR _pk[200] = {0,};
        if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, bitcoinXPUB, _pk, sizeof(_pk)/sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        pubkey = std::string(_pk);
    }

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SignTXETH(const bool bERC20,
                                   const std::vector<JUB_BYTE>& vNonce,
                                   const std::vector<JUB_BYTE>& vGasPrice,
                                   const std::vector<JUB_BYTE>& vGasLimit,
                                   const std::vector<JUB_BYTE>& vTo,
                                   const std::vector<JUB_BYTE>& vValue,
                                   const std::vector<JUB_BYTE>& vData,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainID,
                                   std::vector<JUB_BYTE>& vRaw) {

    auto encoded = abcd::DataChunk();
    abcd::append(encoded, jub::eth::RLP::encode(vNonce));
    abcd::append(encoded, jub::eth::RLP::encode(vGasPrice));
    abcd::append(encoded, jub::eth::RLP::encode(vGasLimit));
    abcd::append(encoded, jub::eth::RLP::encode(vTo));
    abcd::append(encoded, jub::eth::RLP::encode(vValue));
    abcd::append(encoded, jub::eth::RLP::encode(vData));
    abcd::append(encoded, jub::eth::RLP::encode(vChainID));
    abcd::append(encoded, jub::eth::RLP::encode(abcd::DataChunk{0}));
    abcd::append(encoded, jub::eth::RLP::encode(abcd::DataChunk{0}));
    encoded = jub::eth::RLP::encodeList(encoded);

    uchar_vector rlps = encoded;
    std::string rlprlp = rlps.getHex();

    JUB_BYTE digest[32] = {0,};
    keccak_256(&encoded[0], encoded.size(), digest);

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    std::string path(&vPath[0], &vPath[0]+vPath.size());
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

    JUB_BYTE sig[64] = {0,};
    JUB_BYTE by = 0;
    if (0 != hdnode_sign_digest(&hdkey, digest, sig, &by, nullptr)) {
        return JUBR_ERROR;
    }
    abcd::DataChunk r(sig,sig+32);
    abcd::DataChunk s(sig+32,sig+64);;
    abcd::DataChunk v(by);

    abcd::append(vRaw, jub::eth::RLP::encode(vNonce));
    abcd::append(vRaw, jub::eth::RLP::encode(vGasPrice));
    abcd::append(vRaw, jub::eth::RLP::encode(vGasLimit));
    abcd::append(vRaw, jub::eth::RLP::encode(vTo));
    abcd::append(vRaw, jub::eth::RLP::encode(vValue));
    abcd::append(vRaw, jub::eth::RLP::encode(vData));
    abcd::append(vRaw, jub::eth::RLP::encode(v));
    abcd::append(vRaw, jub::eth::RLP::encode(r));
    abcd::append(vRaw, jub::eth::RLP::encode(s));
    vRaw = jub::eth::RLP::encodeList(vRaw);

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetERC20ETHToken(const std::string& tokenName,
                                          const JUB_UINT16 unitDP,
                                          const std::string& contractAddress) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletEOS() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetCoinTypeEOS() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAddressEOS(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {
    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

    uchar_vector pk(hdkey.public_key, hdkey.public_key+33);
    TW::PublicKey twpk = TW::PublicKey(TW::Data(pk), TWPublicKeyType::TWPublicKeyTypeSECP256k1);
    TW::EOS::Address addr(twpk);
    address = addr.string();

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetHDNodeEOS(const JUB_BYTE format, const std::string& path, std::string& pubkey) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

//    typedef enum class JubEOSPubFormat {
//        HEX = 0x00,
//        EOS = 0x01
//    } JUB_ENUM_EOS_PUB_FORMAT;
    uchar_vector pk(hdkey.public_key, hdkey.public_key+33);
    if (0x00 == format) {//hex
        pubkey = pk.getHex();
        pubkey = ETH_PRDFIX + pubkey;
    }
    else if (0x01 == format) {
        TW::PublicKey twpk = TW::PublicKey(TW::Data(pk), TWPublicKeyType::TWPublicKeyTypeSECP256k1);
        TW::EOS::Address address(twpk);
        pubkey = address.string();
        if (!TW::EOS::Address::isValid(pubkey)) {
            return JUBR_ERROR;
        }
    }
    else { //xpub
        JUB_CHAR _pk[200] = {0,};
        if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, bitcoinXPUB, _pk, sizeof(_pk)/sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        pubkey = std::string(_pk);
    }

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SignTXEOS(const TW::EOS::Type& type,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainId,
                                   const std::vector<JUB_BYTE>& vRaw,
                                   std::vector<uchar_vector>& vSignatureRaw,
                                   const bool bWithType) {

    try {
        TW::EOS::Transaction tx;
        tx.deserialize(vRaw, bWithType);

        std::string path(vPath.begin(), vPath.end());

        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

        uchar_vector privk(hdkey.private_key, hdkey.private_key+32);
        TW::PrivateKey twprivk = TW::PrivateKey(TW::Data(privk));

        TW::EOS::Signer signer {vChainId};
        signer.sign(twprivk,
                    type,
                    tx);

        for (const TW::EOS::Signature& signature : tx.signatures) {
            vSignatureRaw.push_back(signature.data);
        }
    }
    catch(...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletXRP() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetCoinTypeXRP() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAddressXRP(const std::string& path, const JUB_UINT16 tag, std::string& address) {
    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

    uchar_vector pk(hdkey.public_key, hdkey.public_key+33);
    TW::PublicKey twpk = TW::PublicKey(TW::Data(pk), TWPublicKeyType::TWPublicKeyTypeSECP256k1);
    TW::Ripple::Address addr(twpk);
    address = addr.string();

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetHDNodeXRP(const JUB_BYTE format, const std::string& path, std::string& pubkey) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

//    typedef enum class {
//        HEX = 0x00,
//        XRP = 0x01
//    } JUB_ENUM_PUB_FORMAT;
    uchar_vector pk(hdkey.public_key, hdkey.public_key+33);
    if (0x00 == format) {//hex
        pubkey = pk.getHex();
    }
    else { //xpub
        JUB_CHAR _pk[200] = {0,};
        if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, bitcoinXPUB, _pk, sizeof(_pk)/sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        pubkey = std::string(_pk);
    }

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SignTXXRP(const std::vector<JUB_BYTE>& vPath,
                                   std::vector<JUB_BYTE>& vUnsignedRaw,
                                   std::vector<uchar_vector>& vSignatureRaw) {

    try {
        TW::Ripple::Transaction tx;
        tx.setPreImage(vUnsignedRaw);

        std::string path(vPath.begin(), vPath.end());

        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

        uchar_vector privk(hdkey.private_key, hdkey.private_key+32);
        TW::PrivateKey twprivk = TW::PrivateKey(TW::Data(privk));
        TW::PublicKey twpubk = twprivk.getPublicKey(TWPublicKeyTypeSECP256k1);
        tx.pub_key.insert(tx.pub_key.end(), twpubk.bytes.begin(), twpubk.bytes.end());
        tx.serialize();
        vUnsignedRaw.clear();
        vUnsignedRaw = tx.getPreImage();

        TW::Ripple::Signer signer;
        signer.sign(twprivk,
                    tx);

        vSignatureRaw.push_back(tx.signature);
    }
    catch(...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::QueryBattery(JUB_BYTE &percent) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::ShowVirtualPwd() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::CancelVirtualPwd() {
    return JUBR_OK;
}

bool   TrezorCryptoImpl::IsInitialize() {
    return true;
}

bool   TrezorCryptoImpl::IsBootLoader() {
    return false;
}

JUB_RV TrezorCryptoImpl::GetSN(JUB_BYTE sn[24]) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetLabel(JUB_BYTE label[32]) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetPinRetry(JUB_BYTE& retry) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetPinMaxRetry(JUB_BYTE& maxRetry) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetBleVersion(JUB_BYTE bleVersion[4]) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetFwVersion(JUB_BYTE fwVersion[4]) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::EnumApplet(std::string& appletList) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAppletVersion(const std::string& appID, std::string& version) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::EnumSupportCoins(std::string& coinList) {
    coinList = "BTC LTC BCH ETH USDT ";
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetDeviceCert(std::string& cert) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SendOneApdu(const std::string& apdu, std::string& response) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetTimeout(const JUB_UINT16 timeout) {
    return JUBR_OK;
}
}
