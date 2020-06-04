#include <token/BTC/JubiterNFCBTCImpl.h>
#include <TrezorCrypto/base58.h>

namespace jub {
namespace token {


JUB_RV JubiterNFCBTCImpl::SelectApplet() {

    return JubiterNFCImpl::SelectApplet();
}


JUB_RV JubiterNFCBTCImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) {

    std::string btcXpub;
    JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(JUB_ENUM_BTC_TRANS_TYPE::p2pkh, path, btcXpub));

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

    JUB_RV rv = JUBR_ERROR;

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterNFCImpl::GetCompPubKey((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    switch (type) {
    case p2pkh:
    {
        rv = _getAddress(publicKey, address);
        break;
    }
    case p2sh_p2wpkh:
    {
        rv = _getSegwitAddress(publicKey, address);
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


JUB_RV JubiterNFCBTCImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) {

    return JUBR_OK;
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


JUB_RV JubiterNFCBTCImpl::_encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature) {

    ecdsa_curve *curve = (ecdsa_curve *)get_curve_by_name(_curve_name)->params;

    // RSV: 32 + 32 + 1
    curve_point R;
    uint8_t r[32] = {0x00,};
    memcpy(r, &vRSV[0], 32);
    bn_read_be(r, &R.x);
    uint8_t s[32] = {0x00,};
    memcpy(s, &vRSV[32], 32);
    bn_read_be(s, &R.y);
    uint8_t by = R.y.val[0] & 1;// signature recovery byte

    uint8_t sig[64] = {0x00,};
    // if S > order/2 => S = -S
    if (bn_is_less(&curve->order_half, &R.y)) {
        bn_subtract(&curve->order, &R.y, &R.y);
        by ^= 1;
    }

    // we are done, R.x and s is the result signature
    bn_write_be(&R.x, sig);
    bn_write_be(&R.y, sig + 32);

    // signature DER encode
    std::array<uint8_t, 72> resultBytes;
    size_t size = ecdsa_sig_to_der(sig, resultBytes.data());
    std::copy(resultBytes.begin(), resultBytes.begin() + size, std::back_inserter(signature));

    return JUBR_OK;
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

    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(_SignTx(witness,
                          type,
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


JUB_RV JubiterNFCBTCImpl::_SignTx(bool witness,
                                  const JUB_ENUM_BTC_TRANS_TYPE& type,
                                  const std::vector<JUB_UINT64>& vInputAmount,
                                  const std::vector<std::string>& vInputPath,
                                  const std::vector<JUB_UINT16>& vChangeIndex,
                                  const std::vector<std::string>& vChangePath,
                                  const TW::Bitcoin::Transaction& tx,
                                  std::vector<TW::Data>& vInputPublicKey,
                                  std::vector<uchar_vector>& vSignatureRaw) {

    TW::Hash::Hasher halfHasher;
    JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

    std::vector<TW::Data> vPreImageHash;
    for (size_t index=0; index<tx.inputs.size(); ++index) {
        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterNFCImpl::GetCompPubKey(JUB_ENUM_BTC_TRANS_TYPE::p2pkh, vInputPath[index], publicKey));

        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);

        // script code - scriptPubKey
        uint8_t prefix = TWCoinTypeP2pkhPrefix(_coin);
        TW::Bitcoin::Address addr(twpk, prefix);
        TW::Bitcoin::Script scriptCode = TW::Bitcoin::Script::buildForAddress(addr.string(), _coin);
        if (scriptCode.empty()) {
            return JUBR_ARGUMENTS_BAD;
        }

        TW::Data preImage;
        if (!witness) {
            preImage = tx.getPreImage(scriptCode, index, _hashType);
        }
        else {
            preImage = tx.getPreImage(scriptCode, index, _hashType, vInputAmount[index]);
        }

        const auto begin = reinterpret_cast<const uint8_t*>(preImage.data());
        TW::Data halfDigest = halfHasher(begin, begin+preImage.size());

        vPreImageHash.push_back(halfDigest);

        vInputPublicKey.push_back(publicKey);
    }

    std::vector<TW::Data> vRSV;
    JUB_VERIFY_RV(JubiterNFCImpl::SignTX(vInputPath.size(),
                                         vInputPath,
                                         _getSignType(_curve_name),
                                         halfHasherType,
                                         vPreImageHash,
                                         vRSV));

    for (const auto& rsv : vRSV) {
        TW::Data sign;
        JUB_VERIFY_RV(_encodeRSV(rsv, sign));
        TW::Data signature = pushAll(sign);
        vSignatureRaw.push_back(signature);
    }

    return JUBR_OK;
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
        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterNFCImpl::GetCompPubKey(JUB_ENUM_BTC_TRANS_TYPE::p2pkh, inputPath, publicKey));

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
