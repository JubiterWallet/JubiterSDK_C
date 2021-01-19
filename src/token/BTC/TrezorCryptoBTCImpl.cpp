#include "token/BTC/TrezorCryptoBTCImpl.h"
#include <TrezorCrypto/curves.h>
#include "HDKey/HDKey.hpp"
#include "utility/util.h"
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
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

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


JUB_RV TrezorCryptoBTCImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {

    JUB_RV rv = JUBR_ERROR;

    // derive key using BTC version
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    uchar_vector pk(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
    switch (type) {
    case p2pkh:
    {
        rv = _getAddress(TW::Data(pk), address);
        break;
    }
    case p2sh_p2wpkh:
    {
        rv = _getSegwitAddress(TW::Data(pk), address);
        break;
    }
//    case p2wpkh:
//    case p2sh_multisig:
//    case p2wsh_multisig:
//    case p2sh_p2wsh_multisig:
    default:
        rv = JUBR_ARGUMENTS_BAD;
    }   // switch (type) end

    return rv;
}


JUB_RV TrezorCryptoBTCImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::SetCoin(const JUB_ENUM_COINTYPE_BTC& type) {

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

    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(_SignTx(witness,
                          vInputAmount,
                          vInputPath,
                          vChangeIndex,
                          vChangePath,
                          tx,
                          vInputPublicKey,
                          vSignatureRaw));

    uchar_vector signedRaw;
    JUB_VERIFY_RV(_serializeTx(witness,
                               vInputAmount,
                               vInputPublicKey,
                               vSignatureRaw,
                               &tx,
                               signedRaw));

    vRaw = signedRaw;

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::_SignTx(bool witness,
                                    const std::vector<JUB_UINT64>& vInputAmount,
                                    const std::vector<std::string>& vInputPath,
                                    const std::vector<JUB_UINT16>& vChangeIndex,
                                    const std::vector<std::string>& vChangePath,
                                    const TW::Bitcoin::Transaction& tx,
                                    std::vector<TW::Data>& vInputPublicKey,
                                    std::vector<uchar_vector>& vSignatureRaw) {

    JUB_RV rv = JUBR_ERROR;

    for (size_t index=0; index<tx.inputs.size(); ++index) {
        // derive key using BTC version
        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(_HdnodeCkd(vInputPath[index].c_str(), &hdkey, &parentFingerprint));

        TW::Data prvKey;
        prvKey.insert(prvKey.end(), &hdkey.private_key[0], &hdkey.private_key[0]+TW::PrivateKey::size);
        TW::PrivateKey twprvk(prvKey);

        uchar_vector publicKey(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);

        // script code - scriptPubKey
        TW::Bitcoin::Script scriptCode;
        rv = _scriptCode(_coin, twpk, scriptCode);
        if (JUBR_OK != rv) {
            break;
        }

        TW::Data preImage;
        if (!witness) {
            preImage = tx.getPreImage(scriptCode, index, _hashType);
        }
        else {
            preImage = tx.getPreImage(scriptCode, index, _hashType, vInputAmount[index]);
        }

        const auto begin = reinterpret_cast<const uint8_t*>(preImage.data());
        TW::Data digest = tx.hasher(begin, preImage.size());
        TW::Data sign = twprvk.signAsDER(digest, curveName2TWCurve(_curve_name));
        TW::Data signature = pushAll(sign);
        if (!twpk.verifyAsDER(signature, digest)) {
            rv = JUBR_VERIFY_SIGN_FAILED;
            break;
        }
        else {
            rv = JUBR_OK;
        }

        // move to JubiterBaseBTCImpl::_serializeTx()
//        if (!witness) {
//            // P2PKH
//            tx.inputs[index]->script = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSig(signature, TW::Data(publicKey));
//        }
//        else {
//            // P2WPKH
//            TW::Data scriptPubkey;
//            TW::Bitcoin::Script::buildPayToWitnessPubkeyHash(twpk.hash(TW::Data())).encode(scriptPubkey);
//            tx.inputs[index]->script.bytes = scriptPubkey;
//
//            tx.inputs[index]->scriptWitness = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSigWitness(signature, TW::Data(publicKey));
//        }

        vInputPublicKey.push_back(TW::Data(publicKey));
        vSignatureRaw.push_back(signature);
    }

    return rv;
}


JUB_RV TrezorCryptoBTCImpl::VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                     const std::vector<JUB_UINT64>& vInputAmount,
                                     const std::vector<std::string>& vInputPath,
                                     const std::vector<JUB_BYTE>& vSigedTrans) {

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin,  witness);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin, witness);

    std::vector<TW::Data> vInputPublicKey;
    for (const auto& inputPath:vInputPath) {
        std::string xpub;
        JUB_VERIFY_RV(GetHDNode(type, inputPath, xpub));

        TW::Data publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey,
                                         hdVersionPub, hdVersionPrv));

        vInputPublicKey.push_back(publicKey);
    }

    // verify signature
    return VerifyTx(witness,
                    vSigedTrans,
                    vInputAmount,
                    vInputPublicKey);
}


} // namespace token end
} // namespace jub end
