#include <token/BTC/TrezorCryptoBTCImpl.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <HDKey/HDKey.hpp>
#include "utility/util.h"
#include <Bitcoin/Address.h>
#include <Bitcoin/SegwitAddress.h>
#include <Bitcoin/Script.h>
#include <Base58Address.h>
#include "libBTC/libBTC.hpp"

namespace jub {
namespace token {


JUB_RV TrezorCryptoBTCImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) {

    JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, hdVersionPub, hdVersionPrv, &hdkey, &parentFingerprint));

    hdnode_fill_public_key(&hdkey);
    JUB_UINT32 version = hdVersionPub;
    if (p2sh_p2wpkh == type) {
        version = TWHDVersion::TWHDVersionYPUB;
    }

    JUB_CHAR _xpub[200] = { 0, };
    if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, version, _xpub, sizeof(_xpub) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    xpub = std::string(_xpub);

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {

    JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path.c_str(), SECP256K1_NAME, hdVersionPub, hdVersionPrv, &hdkey, &parentFingerprint));

    uchar_vector pk(hdkey.public_key, hdkey.public_key + TW::PublicKey::secp256k1Size);
    TW::PublicKey twpk = TW::PublicKey(TW::Data(pk), TWPublicKeyType::TWPublicKeyTypeSECP256k1);

    uint8_t prefix = 0;
    switch (type) {
    case p2pkh:
    {
        prefix = TWCoinTypeP2pkhPrefix(_coin);

        TW::Bitcoin::Address addr(twpk, prefix);
        address = addr.string();
        break;
    }
    case p2sh_p2wpkh:
    {
        prefix = TWCoinTypeP2shPrefix(_coin);

        // keyhash
        TW::Bitcoin::SegwitAddress segwitAddr(twpk, OpCode::OP_0, std::string(stringForHRP(TWCoinTypeHRP(_coin))));

        // redeemScript
        TW::Bitcoin::Script redeemScript = TW::Bitcoin::Script::buildPayToWitnessProgram(segwitAddr.witnessProgram);
        TW::Data hRedeemScript = TW::Hash::sha256ripemd(&redeemScript.bytes[0], &redeemScript.bytes[0]+redeemScript.bytes.size());

        // address
        TW::Data bytes;
        bytes.insert(bytes.end(), prefix);
        bytes.insert(bytes.end(), hRedeemScript.begin(), hRedeemScript.end());

        address = TW::Base58::bitcoin.encodeCheck(bytes);

        break;
    }
//    case p2wpkh:
//    case p2sh_multisig:
//    case p2wsh_multisig:
//    case p2sh_p2wsh_multisig:
    default:
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unit) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoBTCImpl::SetCoinType(const JUB_ENUM_COINTYPE_BTC& type) {

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

    JUB_RV rv = JUBR_IMPL_NOT_SUPPORT;

    return rv;
}


JUB_RV TrezorCryptoBTCImpl::SetQRC20ETHToken(const std::string& tokenName,
                                             const JUB_UINT16 unitDP,
                                             const std::string& contractAddress) {

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
