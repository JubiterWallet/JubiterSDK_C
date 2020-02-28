#include <token/BTC/JubiterNFCBTCImpl.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/bip32.h>
#include <TrustWalletCore/TWCurve.h>

namespace jub {
namespace token {


JUB_RV JubiterNFCBTCImpl::SelectApplet() {

    return JubiterNFCImpl::SelectApplet();
}


JUB_RV JubiterNFCBTCImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) {

    std::string btcXpub;
    JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(0x00, path, btcXpub));

    const curve_info *curve = get_curve_by_name(_curve_name);
    uint8_t nodeData[128] = {0x00,};
    int nodeDatalen = sizeof(nodeData)/sizeof(uint8_t);
    nodeDatalen = base58_decode_check(btcXpub.c_str(),
                                      curve->hasher_base58,
                                      nodeData, nodeDatalen);
    if (0 == nodeDatalen) {
        return JUBR_ERROR;
    }

    // replace version
    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }
    JUB_UINT32 version = TWCoinType2HDVersionPublic(_coin, witness);
    write_be(nodeData, version);

    JUB_CHAR _xpub[200] = { 0, };
    if (0 == base58_encode_check(nodeData, nodeDatalen,
                                 curve->hasher_base58,
                                 _xpub, sizeof(_xpub)/sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }
    xpub = std::string(_xpub);

    return JUBR_OK;
}


JUB_RV JubiterNFCBTCImpl::GetAddress(const JUB_BYTE addrFmt,
                                     const JUB_ENUM_BTC_TRANS_TYPE& type,
                                     const std::string& path,
                                     const JUB_UINT16 tag,
                                     std::string& address) {

    std::string btcXpub;
    JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(type, path, btcXpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(btcXpub, publicKey));

    return _getAddress(publicKey, address);
}


JUB_RV JubiterNFCBTCImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCBTCImpl::SetCoinType(const JUB_ENUM_COINTYPE_BTC& type) {

    switch (type) {
    case COINBCH:
        _coin = TWCoinType::TWCoinTypeBitcoinCash;
        break;
    case COINLTC:
        _coin = TWCoinType::TWCoinTypeLitecoin;
        break;
    case COINDASH:
        _coin = TWCoinType::TWCoinTypeDash;
        break;
    case COINQTUM:
        _coin = TWCoinType::TWCoinTypeQtum;
        break;
    case COINUSDT:
    default:
        _coin = TWCoinType::TWCoinTypeBitcoin;
        break;
    }
    return JUBR_OK;
}

inline bool halfHasher(const HasherType hasherType, HasherType& halfHasherType, TW::Hash::Hasher& halfHasher) {
    bool bMode = true;

    switch (hasherType) {
    case HASHER_SHA2:
        halfHasherType = HASHER_SHA2;
        bMode = false;
    case HASHER_SHA2D:
        halfHasherType = HASHER_SHA2;
        halfHasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
           return TW::Hash::sha256(begin, end);
        };
        break;
    case HASHER_SHA2_RIPEMD:
        bMode = false;
        halfHasherType = HASHER_SHA2_RIPEMD;
        halfHasher = TW::Hash::sha256ripemd;
        break;
    case HASHER_BLAKE:
        bMode = false;
        halfHasherType = HASHER_BLAKE;
    case HASHER_BLAKED:
        halfHasherType = HASHER_BLAKE;
        halfHasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
           return TW::Hash::blake256(begin, end);
        };
        break;
    case HASHER_BLAKE_RIPEMD:
        bMode = false;
        halfHasherType = HASHER_BLAKE_RIPEMD;
        halfHasher = TW::Hash::blake256ripemd;
        break;
    case HASHER_GROESTLD_TRUNC:
        bMode = false;
        halfHasherType = HASHER_GROESTLD_TRUNC;
        halfHasher = TW::Hash::groestl512d;
        break;
    case HASHER_BLAKE2B:
        halfHasherType = HASHER_BLAKE2B;
    case HASHER_BLAKE2B_PERSONAL:
        halfHasherType = HASHER_BLAKE2B_PERSONAL;
    default:
        bMode = false;
        break;
    }

    return bMode;
}

JUB_RV JubiterNFCBTCImpl::SignTX(const JUB_BYTE addrFmt,
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

    JUB_RV rv = JUBR_ERROR;

    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    for (size_t index=0; index<tx.inputs.size(); ++index) {
        std::string btcXpub;
        JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(type, vInputPath[index], btcXpub));

        TW::Data publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(btcXpub, publicKey));

        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);

        // script code - scriptPubKey
        uint8_t prefix = TWCoinTypeP2pkhPrefix(_coin);
        TW::Bitcoin::Address addr(twpk, prefix);
        TW::Bitcoin::Script scriptCode = TW::Bitcoin::Script::buildForAddress(addr.string(), _coin);
        if (0 == scriptCode.size()) {
            rv = JUBR_ARGUMENTS_BAD;
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
        HasherType halfHasherType;
        TW::Hash::Hasher halfHasherSign;
        JUB_BYTE hashMode = (JUB_BYTE)JUB_ENUM_NFC_HASHMODE::DOUBLE;
        if (!halfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasherType, halfHasherSign)) {
            hashMode = (JUB_BYTE)JUB_ENUM_NFC_HASHMODE::SINGLE;
        }
        TW::Data digest = halfHasherSign(begin, begin+preImage.size());
        TW::Data signature;
        JUB_VERIFY_RV(JubiterNFCImpl::SignTX(vInputPath[index],
                                             hashMode,
                                             (JUB_BYTE)halfHasherType,
                                             digest,
                                             signature));
        if (!twpk.verifyAsDER(signature, digest)) {
            rv = JUBR_ERROR;
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

        vInputPublicKey.push_back(publicKey);
        vSignatureRaw.push_back(signature);
    }

    return rv;
}


JUB_RV JubiterNFCBTCImpl::VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                   const std::vector<JUB_UINT64>& vInputAmount,
                                   const std::vector<std::string>& vInputPath,
                                   const std::vector<JUB_BYTE>& vSigedTrans) {

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    std::vector<TW::Data> vInputPublicKey;
    for (const auto& inputPath:vInputPath) {
        std::string btcXpub;
        JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(type, inputPath, btcXpub));

        TW::Data publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(btcXpub, publicKey));

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
