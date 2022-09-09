#include "token/CKB/TrezorCryptoCKBImpl.h"
#include <TrustWalletCore/TWCurve.h>
#include "Bech32Address.h"
#include <NervosCKB/Transaction.h>
#include <NervosCKB/WitnessArgs.h>
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


JUB_RV TrezorCryptoCKBImpl::_SignTX(const std::vector<std::string>& vInputPath,
                                    TW::NervosCKB::Transaction& tx,
                                    const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_OK;

    auto witness = tx.witnessGroups();
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

            Data preImageHash = tx.getPreImageHash(index);
            TW::Data sign = twprvk.sign(preImageHash, TWCurve::TWCurveSECP256k1);
            if (0 == sign.size()) {
                rv = JUBR_ERROR;
                break;
            }

            TW::NervosCKB::WitnessArgs arg = witness[index];
            arg.lock = sign;
            tx.witnesses.push_back(arg);
        }
        else {
            tx.witnesses.push_back(TW::NervosCKB::WitnessArgs(0x00));
        }
    }

    return rv;
}


JUB_RV TrezorCryptoCKBImpl::_VerifyTX(const std::vector<std::string>& vInputPath,
                                      const TW::NervosCKB::Transaction& tx,
                                      const std::vector<uchar_vector>& vSignatureRaw,
                                      const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_OK;

    TW::NervosCKB::Transaction signedTx(tx);

    auto witness = signedTx.witnessGroups();
    if (vSignatureRaw.size() != witness.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<TW::PublicKey> vInputPublicKey;
    for (size_t index=0; index<witness.size(); ++index) {
        // derive key using BTC version
        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(_HdnodeCkd(vInputPath[index].c_str(), &hdkey, &parentFingerprint, coinNet));

        uchar_vector publicKey(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);

        vInputPublicKey.push_back(twpk);

        if (!witness[index].isZero()) {
            TW::NervosCKB::WitnessArgs arg = witness[index];
            arg.lock = vSignatureRaw[index];
            signedTx.witnesses.push_back(arg);
        }
        else {
            signedTx.witnesses.push_back(TW::NervosCKB::WitnessArgs(0x00));
        }
    }

    JUB_VERIFY_RV(_verifyTX(signedTx, vInputPublicKey, coinNet));

    return rv;
}


} // namespace token end
} // namespace jub end
