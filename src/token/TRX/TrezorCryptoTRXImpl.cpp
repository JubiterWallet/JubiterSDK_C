#include "token/TRX/TrezorCryptoTRXImpl.h"

#include "HDKey/HDKey.hpp"
#include "utility/util.h"

//#include <HexCoding.h>
#include <Tron/Signer.h>
#include <Tron/Transaction.h>

namespace jub {
namespace token {


JUB_RV TrezorCryptoTRXImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoTRXImpl::SetCoin() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoTRXImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
    TW::Data publicKey(vPublicKey);

    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoTRXImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

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
    else if (0x01 == format) {//xpub
        JUB_CHAR _pk[200] = { 0, };
        if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, TWHDVersion::TWHDVersionXPUB, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        pubkey = std::string(_pk);
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoTRXImpl::SignTX(const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vRaw,
                                   std::vector<uchar_vector>& vSignatureRaw) {

    try {
        TW::Tron::Transaction tx;
        tx.raw_data.deserialize(vRaw);

        std::string path(vPath.begin(), vPath.end());

        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

        uchar_vector privk(hdkey.private_key, hdkey.private_key + 32);
        TW::PrivateKey twprivk = TW::PrivateKey(TW::Data(privk));
        uchar_vector v(32);
        std::copy(std::begin(twprivk.bytes), std::end(twprivk.bytes), std::begin(v));
        TW::Tron::Signer signer;
        if (!signer.sign(twprivk,
                         tx)) {
            return JUBR_ERROR;
        }

        vSignatureRaw.push_back(tx.signature);
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoTRXImpl::SetTRC20Token(const std::string& tokenName,
                                          const JUB_UINT16 unitDP,
                                          const std::string& contractAddress) {

    return JUBR_OK;
}
} // namespace token end
} // namespace jub end
