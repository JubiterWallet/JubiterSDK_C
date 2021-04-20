#include "token/FIL/TrezorCryptoFILImpl.h"

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include "HDKey/HDKey.hpp"

#include <Filecoin/Signer.h>
#include <Filecoin/Transaction.h>


namespace jub {
namespace token {


JUB_RV TrezorCryptoFILImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoFILImpl::SetCoin() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoFILImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
    TW::Data publicKey(vPublicKey);

    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoFILImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    //    typedef enum class {
    //        HEX = 0x00,
    //        FIL = 0x01
    //    } JUB_ENUM_PUB_FORMAT;
    uchar_vector pk(hdkey.public_key, hdkey.public_key + 33);
    if (0x00 == format) {//hex
        pubkey = pk.getHex();
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


JUB_RV TrezorCryptoFILImpl::SignTX(const uint64_t& nonce,
                                   const uint256_t& glimit,
                                   const uint256_t& gfeeCap,
                                   const uint256_t& gpremium,
                                   const std::string& to,
                                   const uint256_t& value,
                                   const std::string& input,
                                   const std::string& path,
                                   std::vector<uchar_vector>& vSignatureRaw) {

    try {
        std::string from;
        JUB_VERIFY_RV(GetAddress(path, 0, from));

        TW::Filecoin::Transaction tx(TW::Filecoin::Address(to),
                                     TW::Filecoin::Address(from),
                                     nonce,
                                     value,
                                     glimit,
                                     gfeeCap,
                                     gpremium);

//        tx.setPreImage(vUnsignedRaw);
//
        HDNode hdkey;
        JUB_UINT32 parentFingerprint;
        JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

        uchar_vector privk(hdkey.private_key, hdkey.private_key + 32);
        TW::PrivateKey twprivk = TW::PrivateKey(TW::Data(privk));

        vSignatureRaw.push_back(uchar_vector(TW::Filecoin::Signer::sign(twprivk, tx)));
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
