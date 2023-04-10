#include "context/BaseContext.h"
#include "context/BTCContextMultiSigExt.h"
//#include "token/JubiterBIO/JubiterBIOToken.h"
//#include "token/JubiterBlade/JubiterBladeToken.h"
//#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/interface/BTCTokenInterface.hpp"
#include "utility/util.h"
#include <TrezorCrypto/secp256k1.h>
#include <Bitcoin/Script.h>
#include <string>
#include <vector>
#include <cstring>
#include <HDKey/HDKey.hpp>

namespace jub {
namespace context {


JUB_RV BTCContextMultiSigExt::PreparatoryFlow(const std::string mainXpub) {
    order(mainXpub);
    return JUBR_OK;
}

std::string BTCContextMultiSigExt::getCosignerFullBip44Path(const std::string _mainPath, const BIP44_Path& path, const long cosignerIndex) {

    return _mainPath
            + "/" + std::to_string(cosignerIndex)
            + "/" + std::to_string(path.change)
            + "/" + std::to_string(path.addressIndex);
}

std::string BTCContextMultiSigExt::_FullBip44Path(const std::string _mainPath, const BIP44_Path& path) {

    if (-1 == _cosignerIndex) {
        return "";
    }

    return getCosignerFullBip44Path(_mainPath, path, _cosignerIndex);
}

JUB_RV BTCContextMultiSigExt::OrderDHPublicKey(const std::string _mainPath,
                                               const BIP44_Path& path,
                                               std::vector<uchar_vector>& vOrderedPublickey) {
    JUB_RV rv = JUBR_ERROR;

    // derive key
    std::vector<std::string> vPublickey;
    //hex main xpub, derive xpub
    std::map<std::string, std::string> mapDeriveXpub;
    for (int i=0; i<_n; ++i) {
        // derive sub pubkey
        std::string mainXpub = getCosignerMainXpub(i);
        if ("" == mainXpub) {
            JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
            break;
        }
        std::string fullPath = getCosignerFullBip44Path(_mainPath, path, i);

        HDNode hdkey;
        JUB_UINT32 parentFingerprint = 0;

        rv = hdnode_pub_ckd(mainXpub, fullPath, _curveName, _xpubPrefix, _xprvPrefix, &hdkey, &parentFingerprint);
        if (JUBR_OK != rv) {
            break;
        }

        uchar_vector publicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
        std::string xpub = publicKey.getHex();
        vPublickey.push_back(xpub);
        mapDeriveXpub.insert({mainXpub, xpub});
    }
    JUB_VERIFY_RV(rv);

    // order
    std::sort(vPublickey.begin(), vPublickey.end());
    // ordered Index
    std::string myMainXpub = getCosignerMainXpub();
    auto search = mapDeriveXpub.find(myMainXpub);
    if (mapDeriveXpub.end() == search) {
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }
    auto pos = std::find(vPublickey.begin(), vPublickey.end(), search->second);
    long orderedIndex = pos - vPublickey.begin();
    setOrderedIndex(orderedIndex);

    // out
    for (const auto& pubkey:vPublickey) {
        uchar_vector vPubkey;
        vPubkey.setHex(pubkey);
        vOrderedPublickey.push_back(vPubkey);
    }

    return JUBR_OK;
}

JUB_RV BTCContextMultiSigExt::BuildRedeemScript(const std::string _mainPath,
                                                const BIP44_Path& path,
                                                uchar_vector& redeemScript) {

    if (-1 == getCosignerIndex()) {
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }

    std::vector<uchar_vector> vOrderedPublickey;
    JUB_VERIFY_RV(OrderDHPublicKey(_mainPath,
                                   path,
                                   vOrderedPublickey));

    std::vector<TW::Data> publicKeys;
    publicKeys.resize(vOrderedPublickey.size());
    for (int i=0; i<vOrderedPublickey.size(); ++i) {
        publicKeys[i] = vOrderedPublickey[i];
    }

    auto script = TW::Bitcoin::Script::buildRedeemScript(_m, _n, publicKeys);
    if (script.size() == 0) {
        JUB_VERIFY_RV(JUBR_ERROR);
    }
    redeemScript = script.bytes;

    return JUBR_OK;
}

JUB_RV BTCContextMultiSigExt::BuildRedeemScriptTlv(const std::string _mainPath,
                                                   const BIP44_Path& path,
                                                   uchar_vector& redeemScriptTlv) {

    if (-1 == getCosignerIndex()) {
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }

    std::vector<uchar_vector> vOrderedPublickey;
    JUB_VERIFY_RV(OrderDHPublicKey(_mainPath,
                                   path,
                                   vOrderedPublickey));

    std::vector<TW::Data> publicKeys;
    publicKeys.resize(vOrderedPublickey.size());
    for (int i=0; i<vOrderedPublickey.size(); ++i) {
        publicKeys[i] = vOrderedPublickey[i];
    }

    auto script = TW::Bitcoin::Script::buildRedeemScript(_m, _n, publicKeys, _orderedIndex);
    if (script.size() == 0) {
        JUB_VERIFY_RV(JUBR_ERROR);
    }
    redeemScriptTlv = script.bytes;

    return JUBR_OK;
}


JUB_RV BTCContextMultiSigExt::GetAddress(std::shared_ptr<token::BaseToken> tokenPtr,
                                         const std::string _mainPath,
                                         const JUB_ENUM_BTC_ADDRESS_FORMAT &addrFmt, const JUB_ENUM_BTC_TRANS_TYPE _transType, const BIP44_Path &path, const JUB_UINT16 tag,
                                         std::string &address,
                                         const TWCoinType& _coinNet) {

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    uchar_vector redeemScriptTlv;
    JUB_VERIFY_RV(BuildRedeemScriptTlv(_mainPath, path, redeemScriptTlv));

    std::string strPath = _FullBip44Path(_mainPath, path);
    JUB_VERIFY_RV(token->GetAddressMultiSig(addrFmt, _transType, strPath, tag, redeemScriptTlv, address, _coinNet));

    return JUBR_OK;
}


JUB_RV BTCContextMultiSigExt::pushAll(const JUB_ENUM_COINTYPE_BTC _coinType, std::vector<uchar_vector>& results) {

    std::vector<uchar_vector> vSignatureRaw;
    for (const auto& result:results) {
        uchar_vector raw(result);
        pushAll(_coinType, raw);

        vSignatureRaw.push_back(raw);
    }

    results.clear();
    results = vSignatureRaw;

    return JUBR_OK;
}

JUB_RV BTCContextMultiSigExt::pushAll(const JUB_ENUM_COINTYPE_BTC _coinType, uchar_vector& result) {

    uint8_t hashType = 0x01;//[ALL]
    uint8_t hashTypeLen = sizeof(hashType)/sizeof(uint8_t);

    switch (_coinType) {
        case COINBTC:
        case COINLTC:
        case COINUSDT:
        case COINDASH:
        case COINQTUM:
            hashType |= 0x00;
            break;
        case COINBCH:
            hashType |= 0x40;
            break;
        default:
            hashTypeLen = 0x00;
            break;
    }

    uchar_vector raw;
    raw.push_back(result.size()+hashTypeLen);
    raw << result;

    if (hashTypeLen) {
        raw.push_back(hashType);
    }

    result.clear();
    result = raw;

    return JUBR_OK;
}

JUB_RV BTCContextMultiSigExt::pullAll(const JUB_ENUM_COINTYPE_BTC _coinType, uchar_vector& result) {

    uint8_t hashType = 0x01;//[ALL]
    uint8_t hashTypeLen = sizeof(hashType)/sizeof(uint8_t);

    switch (_coinType) {
        case COINBTC:
        case COINLTC:
        case COINUSDT:
        case COINDASH:
        case COINQTUM:
            hashType |= 0x00;
            break;
        case COINBCH:
            hashType |= 0x40;
            break;
        default:
            hashTypeLen = 0x00;
            break;
    }

    if (hashTypeLen) {
        if (hashType == result[result.size()-1]) {
            result.pop_back();
        }
        else {
            return JUBR_ARGUMENTS_BAD;
        }
    }

    return JUBR_OK;
}

JUB_RV BTCContextMultiSigExt::OrderSignatures(std::shared_ptr<token::BaseToken> tokenPtr,
                                              const std::string _mainPath,
                                              const JUB_ENUM_COINTYPE_BTC _coinType,
                                              const JUB_ENUM_BTC_TRANS_TYPE _transType,
                                              const JUB_UINT32 version,
                                              const std::vector<INPUT_BTC>& vInputs, const std::vector<uchar_vector>& vRedeemScript,
                                              const std::vector<OUTPUT_BTC>& vOutputs,
                                              const JUB_UINT32 lockTime,
                                              const MAP_COSIGNER_SIGNATURE& mapCosignerSignature,
                                              const std::vector<uchar_vector>& vSignatureRaw,
                                              MAP_COSIGNER_SIGNATURE& mapSignatureOrdered) {

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    for (size_t inputIndex=0; inputIndex<vInputs.size(); ++inputIndex) {

        uchar_vector preimageRaw;
        JUB_VERIFY_RV(token->SerializePreimage(_coinType,
                                                  _transType,
                                               version,
                                                  inputIndex,
                                                  vInputs, vRedeemScript,
                                                  vOutputs,
                                                  lockTime,
                                                  preimageRaw));

        std::vector<uchar_vector> vOrderedPublickey;
        JUB_VERIFY_RV(OrderDHPublicKey(_mainPath,
                                       vInputs[inputIndex].path,
                                       vOrderedPublickey));

        std::vector<std::string> vSignature;
        bool isFind = false;
        for (size_t orderIndex=0; orderIndex<vOrderedPublickey.size(); ++orderIndex) {

            isFind = false;
            // enum signatures in incTx
            std::vector<std::string> vSignatureFromIncTx = getVectorFromMap(std::to_string(inputIndex), mapCosignerSignature);
            for (size_t signatureFromIncTxIndex=0; signatureFromIncTxIndex<vSignatureFromIncTx.size(); ++signatureFromIncTxIndex) {
                uchar_vector signatureFromIncTxRaw;
                signatureFromIncTxRaw.setHex(vSignatureFromIncTx[signatureFromIncTxIndex]);
                uint8_t ulSignatureLen = signatureFromIncTxRaw[0];
                uchar_vector derSignatureFromIncTx(signatureFromIncTxRaw.begin()+1, signatureFromIncTxRaw.begin()+1+ulSignatureLen);

                pullAll(_coinType, derSignatureFromIncTx);

                uchar_vector signatureFromIncTx(64);
                if (1 != ecdsa_der_to_sig(&derSignatureFromIncTx[0], &signatureFromIncTx[0])) {
                    JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
                }

                if (0 == ecdsa_verify(&secp256k1, HasherType::HASHER_SHA2D,
                                      &vOrderedPublickey[orderIndex][0], &signatureFromIncTx[0],
                                      &preimageRaw[0], (uint32_t)preimageRaw.size())
                    ) {
                    vSignature.push_back(vSignatureFromIncTx[signatureFromIncTxIndex]);
                    isFind = true;
                }
            }
            // enum signature that device gen
            if (!isFind) {
                uint8_t ulSignatureLen = vSignatureRaw[inputIndex][0];
                uchar_vector derSignature(vSignatureRaw[inputIndex].begin()+1, vSignatureRaw[inputIndex].begin()+1+ulSignatureLen);

                pullAll(_coinType, derSignature);

                uchar_vector signature(64);
                if (1 != ecdsa_der_to_sig(&derSignature[0], &signature[0])) {
                    JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
                }

                if (0 == ecdsa_verify(&secp256k1, HasherType::HASHER_SHA2D,
                                      &vOrderedPublickey[orderIndex][0], &signature[0],
                                      &preimageRaw[0], (uint32_t)preimageRaw.size())
                    ) {

                    vSignature.push_back(vSignatureRaw[inputIndex].getHex());
                    isFind = true;
                }
            }
        }

        mapSignatureOrdered.insert({std::to_string(inputIndex), vSignature});
    }

    if (0 == mapSignatureOrdered.size()) {
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }

    return JUBR_OK;
}

JUB_RV BTCContextMultiSigExt::SignTX(std::shared_ptr<token::BaseToken> tokenPtr,
                                     const std::string _mainPath,
                                     const JUB_ENUM_COINTYPE_BTC _coinType,
                                     const JUB_ENUM_BTC_ADDRESS_FORMAT &addrFmt, const JUB_ENUM_BTC_TRANS_TYPE _transType, const JUB_UINT32 version,
                                     const std::vector<INPUT_BTC> &vInputs, const std::vector<OUTPUT_BTC> &vOutputs,
                                     const JUB_UINT32 lockTime, std::string &raw,
                                     const TWCoinType& _coinNet) {
    JUB_RV ret = JUBR_OK;

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    //deal inputs
    std::vector<JUB_UINT64> vInputAmount;
    std::vector<std::string> vInputPath;
    std::vector<uchar_vector> vRedeemScriptTlv;
    std::vector<uchar_vector> vRedeemScript;
    for (auto input : vInputs) {
        vInputAmount.push_back(input.amount);
        vInputPath.push_back(_FullBip44Path(_mainPath, input.path));

        uchar_vector redeemScript;
        JUB_VERIFY_RV(BuildRedeemScript(_mainPath, input.path, redeemScript));
        vRedeemScript.push_back(redeemScript);

        uchar_vector redeemScriptTlv;
        JUB_VERIFY_RV(BuildRedeemScriptTlv(_mainPath, input.path, redeemScriptTlv));
        vRedeemScriptTlv.push_back(redeemScriptTlv);
    }

    //deal outputs
    std::vector<JUB_UINT16> vChangeIndex;
    std::vector<std::string> vChangePath;
    std::vector<uchar_vector> vChangeRedeemScriptTlv;
    for (std::size_t i = 0, e = vOutputs.size(); i != e; ++i) {
        if (JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0 == vOutputs[i].type) {
            ret = JUBR_ARGUMENTS_BAD;
            break;
        }
        if (vOutputs[i].stdOutput.changeAddress) {
            vChangeIndex.push_back((JUB_UINT16)i);
            vChangePath.push_back(_FullBip44Path(_mainPath, vOutputs[i].stdOutput.path));

            if (JUB_ENUM_SCRIPT_BTC_TYPE::P2SH_MULTISIG == vOutputs[i].type) {
                // The change address is the same as the transfer address
                uchar_vector changeRedeemScriptTlv;
                JUB_VERIFY_RV(BuildRedeemScriptTlv(_mainPath, vOutputs[i].stdOutput.path, changeRedeemScriptTlv));
                vChangeRedeemScriptTlv.push_back(changeRedeemScriptTlv);
            }
        }
    }
    if (JUBR_OK != ret) {
        JUB_VERIFY_RV(ret);
    }

    // build unsigned transaction
    uchar_vector unsignedTrans;
    JUB_VERIFY_RV(
        token->SerializeUnsignedTx(_transType, version, vInputs, vOutputs, lockTime, unsignedTrans, _coinNet));

    MAP_COSIGNER_SIGNATURE mapCosignerSignature;
    if (0 != _hexIncTx.compare("")) {
        uchar_vector unsignedTx;
        JUB_VERIFY_RV(DeserializeTx(tokenPtr,
                                    vRedeemScript,
                                    _hexIncTx,
                                    mapCosignerSignature,
                                    unsignedTx));
        if (unsignedTx != unsignedTrans) {
            JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
        }
    }

    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(token->SignTXMultiSig(addrFmt, _transType,
                                        (JUB_UINT16)vInputs.size(), vInputAmount, vInputPath,
                                        vRedeemScriptTlv, vChangeIndex, vChangePath, vChangeRedeemScriptTlv,
                                        unsignedTrans,
                                        vSignatureRaw, _coinNet));

    //pushAll
    JUB_VERIFY_RV(pushAll(_coinType, vSignatureRaw));

    //order signature
    MAP_COSIGNER_SIGNATURE mapSignatureOrdered;
    JUB_VERIFY_RV(OrderSignatures(tokenPtr,
                                  _mainPath, _coinType,
                                  _transType, version,
                                  vInputs, vRedeemScript,
                                  vOutputs,
                                  lockTime,
                                  mapCosignerSignature, vSignatureRaw,
                                  mapSignatureOrdered));

    //build transaction
    uchar_vector vRaw;
    JUB_RV rv = token->SerializeTx(_coinType,
                                   _transType, version,
                                   _m,
                                   vInputs, mapSignatureOrdered, vRedeemScript,
                                   vOutputs,
                                   lockTime,
                                   vRaw);
    if (            JUBR_OK != rv
        && JUBR_MULTISIG_OK != rv
        ) {

        JUB_VERIFY_RV(rv);
    }

    raw = vRaw.getHex();

    return JUBR_OK;
}

JUB_RV BTCContextMultiSigExt::DeserializeTx(std::shared_ptr<token::BaseToken> tokenPtr,
                                            const std::string& hexTx,
                                            std::vector<INPUT_BTC>& vInputs,
                                            std::vector<OUTPUT_BTC>& vOutputs,
                                            JUB_UINT32_PTR plockTime) {

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    bool bWitness = false;

    return token->DeserializeTx(bWitness, hexTx, vInputs, vOutputs, plockTime);
}

JUB_RV BTCContextMultiSigExt::DeserializeTx(std::shared_ptr<token::BaseToken> tokenPtr,
                                            const std::vector<uchar_vector>& vRedeemScript,
                                            const std::string& hexIncTx,
                                            MAP_COSIGNER_SIGNATURE& mapCosignerSignature,
                                            uchar_vector& unsignedTx) {

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    bool bWitness = false;

    return token->DeserializeTx(bWitness, vRedeemScript, hexIncTx, mapCosignerSignature, unsignedTx);
}


JUB_RV BTCContextMultiSigExt::ParseTransaction(std::shared_ptr<token::BaseToken> tokenPtr,
                                               const std::string& incRaw,
                                               std::vector<INPUT_BTC>& vInputs,
                                               std::vector<OUTPUT_BTC>& vOutputs,
                                               JUB_UINT32_PTR plockTime) {

    auto token = std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    bool bWitness = false;
    setIncTx(incRaw);
    JUB_VERIFY_RV(token->DeserializeTx(bWitness,
                                       incRaw,
                                       vInputs,
                                       vOutputs,
                                       plockTime));
}


} // namespace context
} // namespace jub
