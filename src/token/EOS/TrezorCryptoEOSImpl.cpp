#include <token/EOS/TrezorCryptoEOSImpl.h>

#include <HDKey/HDKey.hpp>
#include <utility/util.h>

#include "HexCoding.h"
#include "EOS/Signer.h"
#include "EOS/Transaction.h"

namespace jub {
namespace token {


JUB_RV TrezorCryptoEOSImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoEOSImpl::SetCoin() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoEOSImpl::GetAddress(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {

    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
    TW::Data publicKey(vPublicKey);

    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoEOSImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    JUB_RV rv = JUBR_ERROR;

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

//    typedef enum class JubPubFormat {
//        HEX = 0x00,
//        XPUB = 0x01
//    } JUB_ENUM_PUB_FORMAT;
    uchar_vector pk(hdkey.public_key, hdkey.public_key + 33);
    if (0x00 == format) {//hex
        pubkey = pk.getHex();
    }
    else if (0x01 == format) {
        rv = _getAddress(TW::Data(pk), pubkey);
        if (JUBR_OK != rv) {
            return rv;
        }
    }
    else { //xpub
        JUB_CHAR _pk[200] = { 0, };
        if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, TWHDVersion::TWHDVersionXPUB, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        pubkey = std::string(_pk);
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoEOSImpl::SignTX(const TW::EOS::Type& type,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainId,
                                   const std::vector<JUB_BYTE>& vRaw,
                                   std::vector<uchar_vector>& vSignatureRaw,
                                   const bool bWithType) {

    try {
        TW::EOS::Transaction tx;
        tx.deserialize(vRaw, bWithType);

        std::string path(vPath.begin(), vPath.end());

        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
         JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

        uchar_vector privk(hdkey.private_key, hdkey.private_key + 32);
        TW::PrivateKey twprivk = TW::PrivateKey(TW::Data(privk));

        TW::EOS::Signer signer{ vChainId };
        signer.sign(twprivk,
                    type,
                    tx);

        for (const TW::EOS::Signature& signature : tx.signatures) {
            vSignatureRaw.push_back(signature.data);
        }
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
