#include <token/BTC/TrezorCryptoBTCImpl.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <HDKey/HDKey.hpp>
#include "utility/util.h"
#include <Bitcoin/Address.h>
#include <Bitcoin/SegwitAddress.h>
#include <Bitcoin/Script.h>
#include <Base58Address.h>
#include <PrivateKey.h>

namespace jub {
namespace token {


JUB_RV TrezorCryptoBTCImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) {

    // derive key using BTC version
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, TWCurve2name(_curve), TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),  TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin), &hdkey, &parentFingerprint));

    hdnode_fill_public_key(&hdkey);
    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }
    JUB_UINT32 version = TWCoinType2HDVersionPublic(_coin, witness);

    JUB_CHAR _xpub[200] = { 0, };
    if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, version, _xpub, sizeof(_xpub) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    xpub = std::string(_xpub);

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::_GetAddress(const TW::Data publicKey, std::string& address) {

    try {
        TW::Bitcoin::Address addr(TW::PublicKey(publicKey, _publicKeyType), TWCoinTypeP2pkhPrefix(_coin));
        address = addr.string();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::_GetSegwitAddress(const TW::Data publicKey, std::string& address) {

    try {
        // keyhash
        TW::Bitcoin::SegwitAddress segwitAddr(TW::PublicKey(publicKey, _publicKeyType), OpCode::OP_0, std::string(stringForHRP(TWCoinTypeHRP(_coin))));

        // redeemScript
        TW::Bitcoin::Script redeemScript = TW::Bitcoin::Script::buildPayToWitnessProgram(segwitAddr.witnessProgram);
        TW::Data hRedeemScript = TW::Hash::sha256ripemd(&redeemScript.bytes[0], &redeemScript.bytes[0]+redeemScript.bytes.size());

        // address
        TW::Data bytes;
        bytes.insert(bytes.end(), TWCoinTypeP2shPrefix(_coin));
        bytes.insert(bytes.end(), hRedeemScript.begin(), hRedeemScript.end());

        address = TW::Base58::bitcoin.encodeCheck(bytes);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {

    JUB_RV rv = JUBR_ERROR;

    // derive key using BTC version
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path.c_str(), TWCurve2name(_curve), TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin), TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin), &hdkey, &parentFingerprint));

    uchar_vector pk(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
    switch (type) {
    case p2pkh:
    {
        rv = _GetAddress(TW::Data(pk), address);
        break;
    }
    case p2sh_p2wpkh:
    {
        rv = _GetSegwitAddress(TW::Data(pk), address);
        break;
    }
//    case p2wpkh:
//    case p2sh_multisig:
//    case p2wsh_multisig:
//    case p2sh_p2wsh_multisig:
    default:
        rv = JUBR_ARGUMENTS_BAD;
    }

    return rv;
}


JUB_RV TrezorCryptoBTCImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::SetCoinType(const JUB_ENUM_COINTYPE_BTC& type) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::SignTX(const JUB_BYTE addrFmt,
                                   const JUB_ENUM_BTC_TRANS_TYPE& type,
                                   const JUB_UINT16 inputCount,
                                   const std::vector<JUB_UINT64>& vInputAmount,
                                   const std::vector<std::string>& vInputPath,
                                   const std::vector<JUB_UINT16>& vChangeIndex,
                                   const std::vector<std::string>& vChangePath,
                                   const std::vector<JUB_BYTE>& vUnsigedTrans,
                                   std::vector<JUB_BYTE>& vRaw) {

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    TW::Bitcoin::Transaction tx;
    if (!tx.decode(witness, vUnsigedTrans)) {
        return JUBR_ARGUMENTS_BAD;
    }

    return _SignTx(witness,
                   vInputAmount,
                   vInputPath,
                   vChangeIndex,
                   vChangePath,
                   tx,
                   vRaw);
}

JUB_RV TrezorCryptoBTCImpl::_SignTx(bool witness,
                                    const std::vector<JUB_UINT64>& vInputAmount,
                                    const std::vector<std::string>& vInputPath,
                                    const std::vector<JUB_UINT16>& vChangeIndex,
                                    const std::vector<std::string>& vChangePath,
                                    TW::Bitcoin::Transaction& tx,
                                    std::vector<JUB_BYTE>& vRaw) {

    JUB_RV rv = JUBR_ERROR;

//    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    for (size_t index=0; index<tx.inputs.size(); ++index) {
        // derive key using BTC version
        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, vInputPath[index].c_str(), TWCurve2name(_curve), TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin), TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin), &hdkey, &parentFingerprint));

        TW::Data prvKey;
        prvKey.insert(prvKey.end(), &hdkey.private_key[0], &hdkey.private_key[0]+TW::PrivateKey::size);
        TW::PrivateKey twprvk(prvKey);

        uchar_vector publicKey(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);
//        vInputPublicKey.push_back(TW::Data(publicKey));

        // script code - scriptPubKey
        uint8_t prefix = TWCoinTypeP2pkhPrefix(_coin);
        TW::Bitcoin::Address addr(twpk, prefix);
        TW::Bitcoin::Script scriptCode = TW::Bitcoin::Script::buildForAddress(addr.string(), _coin);

        TW::Data preImage;
        if (!witness) {
            preImage = tx.getPreImage(scriptCode, index, _hashType);
        }
        else {
            preImage = tx.getPreImage(scriptCode, index, _hashType, vInputAmount[index]);
        }
        const auto begin = reinterpret_cast<const uint8_t*>(preImage.data());
        TW::Data digest = tx.hasher(begin, begin+preImage.size());
        TW::Data signature = twprvk.signAsDER(digest, _curve);
        if (!twpk.verifyAsDER(signature, digest)) {
            rv = JUBR_ERROR;
            break;
        }
        else {
            rv = JUBR_OK;
        }

        if (!witness) {
            // P2PKH
            tx.inputs[index].script = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSig(signature, TW::Data(publicKey));
        }
        else {
            // P2WPKH
            TW::Data scriptPubkey;
            TW::Bitcoin::Script::buildPayToWitnessPubkeyHash(twpk.hash(TW::Data())).encode(scriptPubkey);
            tx.inputs[index].script.bytes = scriptPubkey;

            tx.inputs[index].scriptWitness = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSigWitness(signature, TW::Data(publicKey));
        }

        vSignatureRaw.push_back(signature);
    }
    if (JUBR_OK != rv) {
        return rv;
    }

    uchar_vector signedRaw;
    tx.encode(witness, signedRaw);
    if (0 >= signedRaw.size()) {
        rv = JUBR_ERROR;
    }

    vRaw = signedRaw;

//    rv = VerifyTx(witness,
//                  signedRaw,
//                  vInputAmount,
//                  vInputPublicKey);

    return rv;
}


} // namespace token end
} // namespace jub end
