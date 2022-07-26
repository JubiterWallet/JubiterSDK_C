#include "token/SOL/TrezorCryptoSOLImpl.h"
#include "HDKey/HDKey.hpp"
#include <Solana/Signer.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV TrezorCryptoSOLImpl::SelectApplet() { return JUBR_OK; }


JUB_RV TrezorCryptoSOLImpl::GetAppletVersion(stVersion &version) { return JUBR_OK; }


JUB_RV TrezorCryptoSOLImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    // tag used by hardware,this imp not use.
//    if (JUB_ENUM_CURVES::ED25519 == _curve && !path.empty()) {
//        JUB_VERIFY_RV(_getEd25519PrvKeyFromMasterKey(_MasterKey_XPRV, derivPrv, derivpub, path, _curve));
//        pubStr = derivpub;
//    }
//
//    uchar_vector pub(pubStr);
//    TW::Data publicKey(pub);
//    return _getAddress(publicKey, address, coinNet);

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key) / sizeof(uint8_t));
    TW::Data publicKey(vPublicKey);

    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoSOLImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    //    typedef enum {
    //        HEX = 0x00,
    //        XPUB = 0x01
    //    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) { // hex
        uchar_vector pk(hdkey.public_key, hdkey.public_key + 33);
        pubkey = pk.getHex();
        pubkey = ETH_PRDFIX + pubkey;
    } else { // xpub
        JUB_CHAR _pk[200] = {
            0,
        };
        if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, TWHDVersion::TWHDVersionXPUB, _pk,
                                         sizeof(_pk) / sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        pubkey = std::string(_pk);
    }

    return JUBR_OK;
}


} // namespace token
} // namespace jub
