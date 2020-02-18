#include <token/HC/TrezorCryptoHCImpl.h>
#include <TrustWalletCore/TWCurve.h>
#include "HDKey/HDKey.hpp"

namespace jub {
namespace token {


JUB_RV TrezorCryptoHCImpl::_HdnodePrivCkd(std::string path, HDNode* node, JUB_UINT32* parentFingerprint) {

    HDNode hdkey;
    if (0 == hdnode_deserialize(_MasterKey_XPRV.c_str(),
                                TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin), TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin),
                                SECP256K1_NAME, &hdkey, parentFingerprint)) {
        try {
            auto vPath = parsePath(path);
            // replace curve to SECP256K1_HCASH_NAME
            hdkey.curve = get_curve_by_name(_curve_name);
            uint32_t depth = hdkey.depth;
            for (uint32_t i=0; i<vPath.size(); ++i) {
                if (i < depth) {
                    continue;
                }
                *parentFingerprint = hdnode_fingerprint(&hdkey);
                if (1 != hdnode_private_ckd(&hdkey, vPath[i])) {
                    return JUBR_ARGUMENTS_BAD;
                }
                hdnode_fill_public_key(&hdkey);
            }
        }
        catch( ...) {
            return JUBR_ARGUMENTS_BAD;
        }
        hdnode_fill_public_key(&hdkey);
        *node = hdkey;
        return JUBR_OK;
    }

    return JUBR_ARGUMENTS_BAD;
}


JUB_RV TrezorCryptoHCImpl::_GetAddress(const TW::Data publicKey, std::string& address) {

    try {
        TW::Hash::Hasher hasherPubkey;
        TW::Hash::Hasher hasherBase58;
        if (   !hasher2HasherType(get_curve_by_name(_curve_name)->hasher_pubkey, hasherPubkey)
            || !hasher2HasherType(get_curve_by_name(_curve_name)->hasher_base58, hasherBase58)
            ) {
            return JUBR_ARGUMENTS_BAD;
        }

        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);
        TW::Data bytes = twpk.hash(TWCoinTypeP2pkhPrefixData(_coin), hasherPubkey);
        address = TW::Base58::bitcoin.encodeCheck(bytes, hasherBase58);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


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


} // namespace token end
} // namespace jub end
