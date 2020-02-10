#include <token/HC/TrezorCryptoHCImpl.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <HDKey/HDKey.hpp>
#include "utility/util.h"
#include <Bitcoin/Address.h>
#include <Bitcoin/SegwitAddress.h>
#include <Bitcoin/Script.h>
#include <Base58Address.h>

namespace jub {
namespace token {

JUB_RV TrezorCryptoHCImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path.c_str(), TWCurve2name(_curve), TWHDVersion::TWHDVersionXPUB, TWHDVersion::TWHDVersionXPRV, &hdkey, &parentFingerprint));

    uchar_vector pk(hdkey.public_key, hdkey.public_key + sizeof(hdkey.public_key)/sizeof(uint8_t));
    TW::PublicKey twpk = TW::PublicKey(TW::Data(pk), _publicKeyType);

    TW::Data prefix;
    switch (type) {
    case p2pkh:
    {
        prefix = TWCoinTypeP2pkhPrefixData(_coin);
        TW::Data bytes = twpk.hash(prefix, TW::Hash::blake256ripemd);

        address = TW::Base58::bitcoin.encodeCheck(bytes,TW::Hash::blake256d);
        break;
    }
    case p2sh_p2wpkh:
//    case p2wpkh:
//    case p2sh_multisig:
//    case p2wsh_multisig:
//    case p2sh_p2wsh_multisig:
    default:
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
