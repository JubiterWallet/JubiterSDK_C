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

namespace jub {

TrezorCryptoImpl::TrezorCryptoImpl(std::string masterkey_XPRV){
    _MasterKey_XPRV = masterkey_XPRV;
}

JUB_RV TrezorCryptoImpl::ConnectToken() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::DisconnectToken() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletBTC() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetHDNodeBTC(JUB_BTC_TRANS_TYPE type, std::string path, std::string& xpub) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, bitcoinXPUB, bitcoinXPRV, &hdkey, &parentFingerprint));

    JUB_CHAR _xpub[200] = {0,};
    hdnode_fill_public_key(&hdkey);
    JUB_UINT32 version = bitcoinXPUB;
    if(p2sh_p2wpkh == type) {
        version = bitcoinYPUB;
    }

    if(0 == hdnode_serialize_public(&hdkey, parentFingerprint, version, _xpub, sizeof(_xpub)/sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    xpub = std::string(_xpub);
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAddressBTC(JUB_BTC_TRANS_TYPE type, std::string path, JUB_UINT16 tag, std::string& address) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path.c_str(), SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

    JUB_CHAR _address[200] = {0,};
    hdnode_fill_public_key(&hdkey);

    hdnode_get_address(&hdkey, 0x00, _address, sizeof(_address)/sizeof(JUB_CHAR));
    address = _address;

    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetUnitBTC(JUB_BTC_UNIT_TYPE unit) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SetCoinTypeBTC(JUB_ENUM_COINTYPE_BTC type) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SignTXBTC(JUB_BTC_TRANS_TYPE type,
                                   JUB_UINT16 inputCount,
                                   std::vector<JUB_UINT64> vInputAmount,
                                   std::vector<std::string> vInputPath,
                                   std::vector<JUB_UINT16> vChangeIndex,
                                   std::vector<std::string> vChangePath,
                                   std::vector<JUB_BYTE> vUnsigedTrans,
                                   std::vector<JUB_BYTE>& vRaw) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::SelectAppletETH() {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::GetAddressETH(std::string path, JUB_UINT16 tag, std::string& address){
    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));
    
    JUB_BYTE ethKeyHash[20] = {0,};
    if(1 == hdnode_get_ethereum_pubkeyhash(&hdkey,ethKeyHash)){   
        uchar_vector _address(ethKeyHash, ethKeyHash+20);
        address = jub::eth::checksumed(_address.getHex(),jub::eth::eip55);
        return JUBR_OK;
    }
    return JUBR_ERROR;
}

JUB_RV TrezorCryptoImpl::GetHDNodeETH(JUB_BYTE format, std::string path, std::string& pubkey) {
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

    if (format == 0x00) {//hex
        uchar_vector pk(hdkey.public_key,hdkey.public_key+33);
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

JUB_RV TrezorCryptoImpl::SignTXETH(bool bERC20,
                                   std::vector<JUB_BYTE> vNonce,
                                   std::vector<JUB_BYTE> vGasPrice,
                                   std::vector<JUB_BYTE> vGasLimit,
                                   std::vector<JUB_BYTE> vTo,
                                   std::vector<JUB_BYTE> vValue,
                                   std::vector<JUB_BYTE> vData,
                                   std::vector<JUB_BYTE> vPath,
                                   std::vector<JUB_BYTE> vChainID,
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

JUB_RV TrezorCryptoImpl::QueryBattery(JUB_BYTE &percent) {
    return JUBR_OK;
}

JUB_RV TrezorCryptoImpl::ShowVirtualPwd(){
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

JUB_RV TrezorCryptoImpl::GetAppletVersion(std::string appID, std::string& version) {
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

JUB_RV TrezorCryptoImpl::SetTimeout(JUB_UINT16 timeout){
    return JUBR_OK;
}
}
