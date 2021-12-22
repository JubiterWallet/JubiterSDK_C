#include "token/BTC/TrezorCryptoBTCImpl.h"
#include "Data.h"
#include "Hash.h"
#include "JUB_SDK_BTC.h"
#include "PublicKey.h"
#include "TWPublicKeyType.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "utility/util.h"
#include <Base58Address.h>
#include <Bitcoin/Script.h>
#include <PrivateKey.h>
#include <TrezorCrypto/curves.h>
#include <iostream>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV TrezorCryptoBTCImpl::SelectApplet() { return JUBR_OK; }

JUB_RV TrezorCryptoBTCImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path, std::string &xpub,
                                      const TWCoinType &coinNet) {

    // derive key using BTC version
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint, coinNet));

    hdnode_fill_public_key(&hdkey);
    bool witness = false;
    bool nested  = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
        nested  = true;
    } else if (p2tr == type || p2wpkh == type) {
        witness = true;
        nested  = false;
    }
    JUB_UINT32 version = TWCoinType2HDVersionPublic((coinNet ? coinNet : _coin), witness, nested);

    JUB_CHAR _xpub[200] = {
        0,
    };
    if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, version, _xpub, sizeof(_xpub) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    xpub = std::string(_xpub);

    return JUBR_OK;
}

JUB_RV TrezorCryptoBTCImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                       const std::string &path, const JUB_UINT16 tag, std::string &address,
                                       const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ERROR;

    // derive key using BTC version
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint, coinNet));

    uchar_vector pk(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key) / sizeof(uint8_t));
    switch (type) {
    case p2pkh: {
        rv = _getAddress(TW::Data(pk), address, coinNet);
        break;
    }
    case p2sh_p2wpkh: {
        rv = _getNestedSegwitAddress(TW::Data(pk), address, coinNet);
        break;
    }
    case p2wpkh: {
        rv = _getSegwitAddress(TW::Data(pk), address, coinNet);
        break;
    }
    case p2tr: {
        rv = _getTaprootAddress(TW::Data(pk), address, coinNet);
        break;
    }
        //    case p2sh_multisig:
        //    case p2wsh_multisig:
        //    case p2sh_p2wsh_multisig:
    default:
        rv = JUBR_ARGUMENTS_BAD;
    } // switch (type) end

    return rv;
}

JUB_RV TrezorCryptoBTCImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE &unit) { return JUBR_OK; }

JUB_RV TrezorCryptoBTCImpl::SetCoin(const JUB_ENUM_COINTYPE_BTC &type) { return JUBR_OK; }

JUB_RV TrezorCryptoBTCImpl::SignTX(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                   const JUB_UINT16 inputCount, const std::vector<JUB_UINT64> &vInputAmount,
                                   const std::vector<std::string> &vInputPath,
                                   const std::vector<JUB_UINT16> &vChangeIndex,
                                   const std::vector<std::string> &vChangePath,
                                   const std::vector<JUB_BYTE> &vUnsigedTrans, std::vector<JUB_BYTE> &vRaw,
                                   const TWCoinType &coinNet) {

    bool witness = false;
    bool nested  = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
        nested  = true;
    } else if (p2tr == type || p2wpkh == type) {
        witness = true;
        nested  = false;
    }

    TW::Bitcoin::Transaction tx;
    if (!tx.decode(witness, vUnsigedTrans)) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    if (type != p2tr) {
        JUB_VERIFY_RV(_SignTx(witness, vInputAmount, vInputPath, vChangeIndex, vChangePath, tx, vInputPublicKey,
                              vSignatureRaw, coinNet));
    } else {
        tx.hasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::sha256);
        JUB_VERIFY_RV(_SignTaproot(vInputAmount, vInputPath, vChangeIndex, vChangePath, tx, vInputPublicKey,
                                   vSignatureRaw, coinNet));
    }

    uchar_vector signedRaw;
    JUB_VERIFY_RV(_serializeTx(type, vInputAmount, vInputPublicKey, vSignatureRaw, &tx, signedRaw));

    vRaw = signedRaw;
    std::cout << signedRaw.getHex() << std::endl;

    return JUBR_OK;
}

JUB_RV TrezorCryptoBTCImpl::_SignTx(bool witness, const std::vector<JUB_UINT64> &vInputAmount,
                                    const std::vector<std::string> &vInputPath,
                                    const std::vector<JUB_UINT16> &vChangeIndex,
                                    const std::vector<std::string> &vChangePath, const TW::Bitcoin::Transaction &tx,
                                    std::vector<TW::Data> &vInputPublicKey, std::vector<uchar_vector> &vSignatureRaw,
                                    const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ERROR;

    for (size_t index = 0; index < tx.inputs.size(); ++index) {
        // derive key using BTC version
        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(_HdnodeCkd(vInputPath[index].c_str(), &hdkey, &parentFingerprint, coinNet));

        TW::Data prvKey;
        prvKey.insert(prvKey.end(), &hdkey.private_key[0], &hdkey.private_key[0] + TW::PrivateKey::size);
        TW::PrivateKey twprvk(prvKey);

        uchar_vector publicKey(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key) / sizeof(uint8_t));
        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);

        // script code - scriptPubKey
        TW::Bitcoin::Script scriptCode;

        // not correct
        rv = _scriptCode((coinNet ? coinNet : _coin), twpk, scriptCode);
        if (JUBR_OK != rv) {
            break;
        }

        TW::Data preImage;
        if (!witness) {
            preImage = tx.getPreImage(scriptCode, index, _hashType);
        } else {
            preImage = tx.getPreImage(scriptCode, index, _hashType, vInputAmount[index]);
        }

        const auto begin   = reinterpret_cast<const uint8_t *>(preImage.data());
        TW::Data digest    = tx.hasher(begin, preImage.size());
        TW::Data sign      = twprvk.signAsDER(digest, curveName2TWCurve(_curve_name));
        TW::Data signature = pushAll(sign);
        if (!twpk.verifyAsDER(signature, digest)) {
            rv = JUBR_VERIFY_SIGN_FAILED;
            break;
        } else {
            rv = JUBR_OK;
        }

        vInputPublicKey.push_back(TW::Data(publicKey));
        vSignatureRaw.push_back(signature);
    }

    return rv;
}

JUB_RV TrezorCryptoBTCImpl::_SignTaproot(const std::vector<JUB_UINT64> &vInputAmount,
                                         const std::vector<std::string> &vInputPath,
                                         const std::vector<JUB_UINT16> &vChangeIndex,
                                         const std::vector<std::string> &vChangePath,
                                         const TW::Bitcoin::Transaction &tx, std::vector<TW::Data> &vInputPublicKey,
                                         std::vector<uchar_vector> &vSignatureRaw, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ERROR;

    for (size_t index = 0; index < tx.inputs.size(); ++index) {
        // derive key using BTC version
        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(_HdnodeCkd(vInputPath[index].c_str(), &hdkey, &parentFingerprint, coinNet));

        TW::Data prvKey;
        prvKey.insert(prvKey.end(), &hdkey.private_key[0], &hdkey.private_key[0] + TW::PrivateKey::size);
        TW::PrivateKey twprvk = TW::PrivateKey(prvKey).p2trPrivateKey();

        uchar_vector publicKey(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key) / sizeof(uint8_t));
        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType).p2trPublicKey();

        // script code - scriptPubKey
        auto script =
            TW::Bitcoin::Script::buildPayToTaprootKeyPathSpending({twpk.bytes.begin() + 1, twpk.bytes.begin() + 33});

        auto preImage = tx.getSigMsg(script, index, _hashType, vInputAmount[index], 1, {});
        // https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#taproot-key-path-spending-signature-validation
        // SigMsg(hash_type, ext_flag)
        preImage.insert(preImage.begin(), (unsigned char)(_hashType));
        TW::Data digest = TW::Hash::hash_TapSigHash(preImage);
        TW::Data sig    = twprvk.signSchnorr(digest, curveName2TWCurve(_curve_name));
        if (_hashType != 0)
            sig.push_back((unsigned char)_hashType);

        if (!twpk.verifySchnorr(sig, digest)) {
            rv = JUBR_VERIFY_SIGN_FAILED;
            break;
        } else {
            rv = JUBR_OK;
        }

        vInputPublicKey.push_back(twpk.bytes);
        vSignatureRaw.push_back(sig);
    }

    return rv;
}

JUB_RV TrezorCryptoBTCImpl::VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::vector<JUB_UINT64> &vInputAmount,
                                     const std::vector<std::string> &vInputPath,
                                     const std::vector<JUB_BYTE> &vSigedTrans, const TWCoinType &coinNet) {

    bool witness = false;
    if (p2sh_p2wpkh == type || p2wpkh == type || p2tr == type) {
        witness = true;
    }

    uint32_t hdVersionPub = TWCoinType2HDVersionPublic((coinNet ? coinNet : _coin), witness);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate((coinNet ? coinNet : _coin), witness);

    std::vector<TW::Data> vInputPublicKey;
    for (const auto &inputPath : vInputPath) {
        std::string xpub;
        JUB_VERIFY_RV(GetHDNode(type, inputPath, xpub, coinNet));

        TW::Data publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey, hdVersionPub, hdVersionPrv));
        if (type == p2tr) {
            auto pk   = TW::PublicKey(publicKey, TWPublicKeyTypeSECP256k1).p2trPublicKey();
            publicKey = pk.bytes;
        }

        vInputPublicKey.push_back(publicKey);
    }

    // verify signature
    return _verifyTx(type, vSigedTrans, vInputAmount, vInputPublicKey);
}

} // namespace token
} // namespace jub
