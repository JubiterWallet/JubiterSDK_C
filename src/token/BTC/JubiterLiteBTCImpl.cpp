#include "token/BTC/JubiterLiteBTCImpl.h"
#include "JUB_SDK_COMM.h"
#include <TrezorCrypto/base58.h>
#include <cstring>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV JubiterLiteBTCImpl::SelectApplet() { return JubiterLiteImpl::SelectApplet(); }

JUB_RV JubiterLiteBTCImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path, std::string &xpub,
                                     const TWCoinType &coinNet) {
    uint8_t nodeData[128] = {
        0x00,
    };
    int nodeDatalen = sizeof(nodeData) / sizeof(uint8_t);
    JUB_VERIFY_RV(JubiterLiteImpl::GetHDNode(_getSignType(SECP256K1_NAME), JUB_ENUM_BTC_TRANS_TYPE::p2pkh,
                                             get_curve_by_name(SECP256K1_NAME), path, nodeData, &nodeDatalen));

    // replace version
    auto witness = type == p2sh_p2wpkh;

    JUB_UINT32 version = TWCoinType2HDVersionPublic((coinNet ? coinNet : _coin), witness);
    write_be(nodeData, version);

    JUB_CHAR _xpub[200] = {
        0,
    };
    if (0 == base58_encode_check(nodeData, nodeDatalen, get_curve_by_name(_curve_name)->hasher_base58, _xpub,
                                 sizeof(_xpub) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }
    xpub = std::string(_xpub);

    return JUBR_OK;
}

JUB_RV JubiterLiteBTCImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                      const std::string &path, const JUB_UINT16 tag, std::string &address,
                                      const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ERROR;

    TW::Data publicKey;
    JUB_VERIFY_RV(
        JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    switch (type) {
    case p2pkh: {
        rv = _getAddress(publicKey, address, coinNet);
        break;
    } // case p2pkh end
    case p2sh_p2wpkh: {
        rv = _getNestedSegwitAddress(publicKey, address, coinNet);
        break;
    } // case p2sh_p2wpkh end
      //    case p2wpkh:
      //    case p2sh_multisig:
      //    case p2wsh_multisig:
      //    case p2sh_p2wsh_multisig:
    default:
        rv = JUBR_ARGUMENTS_BAD;
    } // switch (type) end

    return rv;
}

JUB_RV JubiterLiteBTCImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE &unit) { return JUBR_OK; }

JUB_RV JubiterLiteBTCImpl::SetCoin(const JUB_ENUM_COINTYPE_BTC &type) {

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
    case COINCKB:
        _coin = TWCoinType::TWCoinTypeNervosCKB;
        break;
    case COINUSDT:
    default:
        _coin = TWCoinType::TWCoinTypeBitcoin;
        break;
    } // switch (type) end

    return JUBR_OK;
}

JUB_RV JubiterLiteBTCImpl::_encodeRSV(const std::vector<JUB_BYTE> &vRSV, std::vector<JUB_BYTE> &signature) {

    ecdsa_curve *curve = (ecdsa_curve *)get_curve_by_name(_curve_name)->params;

    // RSV: 32 + 32 + 1
    curve_point R;
    uint8_t r[32] = {
        0x00,
    };
    memcpy(r, &vRSV[0], 32);
    bn_read_be(r, &R.x);
    uint8_t s[32] = {
        0x00,
    };
    memcpy(s, &vRSV[32], 32);
    bn_read_be(s, &R.y);
    uint8_t by = R.y.val[0] & 1; // signature recovery byte

    uint8_t sig[64] = {
        0x00,
    };
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

JUB_RV JubiterLiteBTCImpl::SignTX(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                  const JUB_UINT16 inputCount, const std::vector<JUB_UINT64> &vInputAmount,
                                  const std::vector<std::string> &vInputPath,
                                  const std::vector<JUB_UINT16> &vChangeIndex,
                                  const std::vector<std::string> &vChangePath,
                                  const std::vector<JUB_BYTE> &vUnsigedTrans, std::vector<JUB_BYTE> &vRaw,
                                  const TWCoinType &coinNet) {

    auto witness = type == p2sh_p2wpkh;

    TW::Bitcoin::Transaction tx;
    if (!tx.decode(witness, vUnsigedTrans)) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(_SignTx(witness, type, vInputAmount, vInputPath, vChangeIndex, vChangePath, tx, vInputPublicKey,
                          vSignatureRaw, coinNet));

    uchar_vector signedRaw;
    JUB_VERIFY_RV(_serializeTx(type, vInputAmount, vInputPublicKey, vSignatureRaw, &tx, signedRaw));

    vRaw = signedRaw;

    return JUBR_OK;
}

JUB_RV JubiterLiteBTCImpl::_SignTx(bool witness, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                   const std::vector<JUB_UINT64> &vInputAmount,
                                   const std::vector<std::string> &vInputPath,
                                   const std::vector<JUB_UINT16> &vChangeIndex,
                                   const std::vector<std::string> &vChangePath, const TW::Bitcoin::Transaction &tx,
                                   std::vector<TW::Data> &vInputPublicKey, std::vector<uchar_vector> &vSignatureRaw,
                                   const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ERROR;

    TW::Hash::Hasher halfHasher;
    JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

    std::vector<TW::Data> vPreImageHash;
    for (size_t index = 0; index < tx.inputs.size(); ++index) {
        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), JUB_ENUM_BTC_TRANS_TYPE::p2pkh,
                                                     vInputPath[index], publicKey));

        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);

        // script code - scriptPubKey
        TW::Bitcoin::Script scriptCode;
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

        const auto begin    = reinterpret_cast<const uint8_t *>(preImage.data());
        TW::Data halfDigest = halfHasher(begin, preImage.size());

        vPreImageHash.push_back(halfDigest);

        vInputPublicKey.push_back(publicKey);
    }

    std::vector<TW::Data> vRSV;
    JUB_VERIFY_RV(JubiterLiteImpl::SignTX(vInputPath.size(), vInputPath, _getSignType(_curve_name), halfHasherType,
                                          vPreImageHash, vRSV));

    for (const auto &rsv : vRSV) {
        TW::Data sign;
        JUB_VERIFY_RV(_encodeRSV(rsv, sign));
        TW::Data signature = pushAll(sign);
        vSignatureRaw.push_back(signature);
    }

    return JUBR_OK;
}

JUB_RV JubiterLiteBTCImpl::VerifyTX(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::vector<JUB_UINT64> &vInputAmount,
                                    const std::vector<std::string> &vInputPath,
                                    const std::vector<JUB_BYTE> &vSigedTrans, const TWCoinType &coinNet) {

    std::vector<TW::Data> vInputPublicKey;
    for (const auto &inputPath : vInputPath) {
        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), JUB_ENUM_BTC_TRANS_TYPE::p2pkh,
                                                     inputPath, publicKey));

        vInputPublicKey.push_back(publicKey);
    }

    // verify signature
    return _verifyTx(type, vSigedTrans, vInputAmount, vInputPublicKey, coinNet);
}

JUB_RV JubiterLiteBTCImpl::GetAddressMultiSig(const JUB_BYTE addrFmt,
                                              const JUB_ENUM_BTC_TRANS_TYPE& type,
                                              const std::string& path,
                                              const JUB_UINT16 tag,
                                              const uchar_vector& vRedeemScriptTlv,
                                              std::string& address,
                                              const TWCoinType &coinNet) {
    return JUBR_IMPL_NOT_SUPPORT;
}

JUB_RV JubiterLiteBTCImpl::SignTXMultiSig(const JUB_BYTE addrFmt,
                                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                                          const JUB_UINT16 inputCount,
                                          const std::vector<JUB_UINT64>& vInputAmount,
                                          const std::vector<std::string>& vInputPath,
                                          const std::vector<uchar_vector>& vRedeemScriptTlv,
                                          const std::vector<JUB_UINT16>& vChangeIndex,
                                          const std::vector<std::string>& vChangePath,
                                          const std::vector<uchar_vector>& vChangeRedeemScriptTlv,
                                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                                          std::vector<uchar_vector>& vSignatureRaw,
                                          const TWCoinType& coinNet) {
    return JUBR_IMPL_NOT_SUPPORT;
}

} // namespace token
} // namespace jub
