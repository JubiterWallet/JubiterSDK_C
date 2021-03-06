#include "token/HC/TrezorCryptoHCImpl.h"
#include <TrustWalletCore/TWCurve.h>
#include <TrezorCrypto/base58.h>
#include "HDKey/HDKey.hpp"

namespace jub {
namespace token {

JUB_RV TrezorCryptoHCImpl::SignTX(const JUB_BYTE addrFmt,
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

    TW::Bitcoin::HcashTransaction tx;
    if (!tx.decode(!witness, vUnsigedTrans)) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<TW::Data> vInputPublicKey;
    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(_SignTx(!witness,
                          vInputAmount,
                          vInputPath,
                          vChangeIndex,
                          vChangePath,
                          tx,
                          vInputPublicKey,
                          vSignatureRaw));

    uchar_vector signedRaw;
    JUB_VERIFY_RV(_serializeTx(!witness,
                               vInputAmount,
                               vInputPublicKey,
                               vSignatureRaw,
                               &tx,
                               signedRaw));

    vRaw = signedRaw;

    return JUBR_OK;
}


JUB_RV TrezorCryptoHCImpl::_HdnodeCkd(std::string path, HDNode* node, JUB_UINT32* parentFingerprint) {

    JUB_RV rv = JUBR_ERROR;

    if(JUB_SoftwareTokenType::PRIVATE == _type) {
        std::string dprv;
        rv = _xpub2dpub(_MasterKey_XPRV, dprv);
        if (JUBR_OK != rv) {
            return rv;
        }

        return hdnode_priv_ckd(dprv, path, _curve_name, TWCoinType2HDVersionPublic(_coin),  TWCoinType2HDVersionPrivate(_coin), node, parentFingerprint);
    }

    if(JUB_SoftwareTokenType::PUBLIC == _type) {
        std::string dpub;
        rv = _xpub2dpub(_MasterKey_XPUB, dpub);
        if (JUBR_OK != rv) {
            return rv;
        }

        return hdnode_pub_ckd(dpub, path, _curve_name, TWCoinType2HDVersionPublic(_coin),  TWCoinType2HDVersionPrivate(_coin), node, parentFingerprint);
    }

    return JUBR_ERROR;
}


JUB_RV TrezorCryptoHCImpl::_xpub2dpub(const std::string& xpub, std::string& dpub) {

    JUB_RV rv = JUBR_ERROR;

    // check if xpub
    JUB_UINT32 xpubPrefix = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);
    JUB_UINT32 xprvPrefix = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);

    HDNode hdkey;
    uint32_t parentFingerprint;
    rv = hdnode_deserialize(xpub.c_str(),
                            xpubPrefix, xprvPrefix,
                            SECP256K1_NAME, &hdkey, &parentFingerprint);
    if(JUBR_OK == rv) {
        // xpub -> dpub
        hdkey.curve = get_curve_by_name(_curve_name);
        xpubPrefix = TWCoinType2HDVersionPublic(_coin);
        xprvPrefix = TWCoinType2HDVersionPrivate(_coin);
    }

    JUB_BYTE zero[33] = {0,};
    if(0 != memcmp(hdkey.private_key, zero, sizeof(hdkey.private_key)/sizeof(uint8_t))) {
        hdnode_fill_public_key(&hdkey);

        JUB_CHAR _pk[200] = {0,};
        if (0 == hdnode_serialize_private(&hdkey, parentFingerprint, xprvPrefix, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
            _type = JUB_SoftwareTokenType::NONE;
            return JUBR_ARGUMENTS_BAD;
        }

        dpub = _pk;
        return JUBR_OK;
    }

    JUB_CHAR _pk[200] = {0,};
    if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, xpubPrefix, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
        return JUBR_ARGUMENTS_BAD;
    }

    dpub = _pk;
    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
