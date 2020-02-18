#include <token/HC/TrezorCryptoHCImpl.h>
#include <TrustWalletCore/TWCurve.h>

namespace jub {
namespace token {


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
