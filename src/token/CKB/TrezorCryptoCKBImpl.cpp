#include "token/CKB/TrezorCryptoCKBImpl.h"
#include <TrustWalletCore/TWCurve.h>
#include "Bech32Address.h"
#include <NervosCKB/Transaction.h>
#include <NervosCKB/WitnessArgs.h>
#include "HDKey/HDKey.hpp"
#include <PublicKey.h>
#include <PrivateKey.h>

namespace jub {
namespace token {


JUB_RV TrezorCryptoCKBImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoCKBImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoCKBImpl::SetCoin(const JUB_ENUM_COINTYPE_BTC& type) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoCKBImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& hex, const TWCoinType& coinNet) {

    // derive key using BTC version
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint, coinNet));

    hdnode_fill_public_key(&hdkey);

    hex = uchar_vector(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t)).getHex();
    hex = ETH_PRDFIX + hex;

    return JUBR_OK;
}


JUB_RV TrezorCryptoCKBImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address, const TWCoinType& coinNet) {

    // derive key using BTC version
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint, coinNet));

    uchar_vector pk(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));

    return JubiterBaseCKBImpl::_getSegwitAddress(TW::Data(pk), address, coinNet);
}


JUB_RV TrezorCryptoCKBImpl::SignTX(const std::vector<std::string>& vInputPath,
                                   const JUB_UINT32 version,
                                   const std::vector<CELL_DEP>& vDeps,
                                   const std::vector<CELL_INPUT>& vInputs,
                                   const std::vector<CELL_OUTPUT>& vOutputs,
                                   std::string& rawInJSON,
                                   const TWCoinType& coinNet) {

    TW::NervosCKB::Transaction unsignedTx(version);
    JUB_VERIFY_RV(UnsignedTx(version, vDeps, vInputs, vOutputs, unsignedTx));

    if (!unsignedTx.isValid()) {
        return JUBR_ARGUMENTS_BAD;
    }

    auto witness = unsignedTx.witnessGroups();
    for (size_t index=0; index<witness.size(); ++index) {
        if (!witness[index].isZero()) {
            // derive key using BTC version
            HDNode hdkey;
            JUB_UINT32 parentFingerprint;
            JUB_VERIFY_RV(_HdnodeCkd(vInputPath[index].c_str(), &hdkey, &parentFingerprint, coinNet));

            TW::Data prvKey;
            prvKey.insert(prvKey.end(), &hdkey.private_key[0], &hdkey.private_key[0]+TW::PrivateKey::size);
            TW::PrivateKey twprvk(prvKey);

            uchar_vector publicKey(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
            TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);

            Data preImage = unsignedTx.getPreImage(index);
            TW::Data sign = twprvk.sign(preImage, TWCurve::TWCurveSECP256k1);

            TW::NervosCKB::WitnessArgs arg = witness[index];
            arg.lock = sign;
            unsignedTx.witnesses.push_back(arg);
        }
        else {
            unsignedTx.witnesses.push_back(TW::NervosCKB::WitnessArgs(0x00));
        }
    }

    rawInJSON = unsignedTx.serialize().dump();

    return JUBR_OK;
}
//
//
//JUB_RV TrezorCryptoCKBImpl::_SignTx(bool witness,
//                                const std::vector<JUB_UINT64>& vInputAmount,
//                                    const std::vector<std::string>& vInputPath,
//                                    const std::vector<JUB_UINT16>& vChangeIndex,
//                                    const std::vector<std::string>& vChangePath,
//                                    const TW::Bitcoin::Transaction& tx,
//                                    std::vector<TW::Data>& vInputPublicKey,
//                                    std::vector<uchar_vector>& vSignatureRaw,
//                                    const TWCoinType& coinNet) {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    for (size_t index=0; index<tx.inputs.size(); ++index) {
//        // derive key using BTC version
//        HDNode hdkey;
//        JUB_UINT32 parentFingerprint;
//        JUB_VERIFY_RV(_HdnodeCkd(vInputPath[index].c_str(), &hdkey, &parentFingerprint, coinNet));
//
//        TW::Data prvKey;
//        prvKey.insert(prvKey.end(), &hdkey.private_key[0], &hdkey.private_key[0]+TW::PrivateKey::size);
//        TW::PrivateKey twprvk(prvKey);
//
//        uchar_vector publicKey(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
//        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);
//
//        // script code - scriptPubKey
//        TW::Bitcoin::Script scriptCode;
//        rv = _scriptCode((coinNet?coinNet:_coin), twpk, scriptCode);
//        if (JUBR_OK != rv) {
//            break;
//        }
//
//        TW::Data preImage;
//        if (!witness) {
//            preImage = tx.getPreImage(scriptCode, index, _hashType);
//        }
//        else {
//            preImage = tx.getPreImage(scriptCode, index, _hashType, vInputAmount[index]);
//        }
//
//        const auto begin = reinterpret_cast<const uint8_t*>(preImage.data());
//        TW::Data digest = tx.hasher(begin, preImage.size());
//        TW::Data sign = twprvk.signAsDER(digest, curveName2TWCurve(_curve_name));
//        TW::Data signature = pushAll(sign);
//        if (!twpk.verifyAsDER(signature, digest)) {
//            rv = JUBR_VERIFY_SIGN_FAILED;
//            break;
//        }
//        else {
//            rv = JUBR_OK;
//        }
//
//        // move to JubiterBaseBTCImpl::_serializeTx()
////        if (!witness) {
////            // P2PKH
////            tx.inputs[index]->script = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSig(signature, TW::Data(publicKey));
////        }
////        else {
////            // P2WPKH
////            TW::Data scriptPubkey;
////            TW::Bitcoin::Script::buildPayToWitnessPubkeyHash(twpk.hash(TW::Data())).encode(scriptPubkey);
////            tx.inputs[index]->script.bytes = scriptPubkey;
////
////            tx.inputs[index]->scriptWitness = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSigWitness(signature, TW::Data(publicKey));
////        }
//
//        vInputPublicKey.push_back(TW::Data(publicKey));
//        vSignatureRaw.push_back(signature);
//    }
//
//    return rv;
//}


} // namespace token end
} // namespace jub end
