#include "token/DOT/TrezorCryptoDOTImpl.h"

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include "HDKey/HDKey.hpp"

#include <Polkadot/Signer.h>
//#include <Polkadot/Transaction.h>


namespace jub {
namespace token {


JUB_RV TrezorCryptoDOTImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoDOTImpl::SetCoin() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoDOTImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
    TW::Data publicKey(vPublicKey);
//    std::string hex = "beff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908";
//    uchar_vector dest(hex);
//    TW::Data publicKey(dest);
    
    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoDOTImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    //    typedef enum class {
    //        HEX = 0x00,
    //        DOT = 0x01
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


JUB_RV TrezorCryptoDOTImpl::SignTX(const uint64_t& nonce,
                                   const uint64_t& gprice,
                                   const uint64_t& glimit,
                                   const std::string& to,
                                   const uint64_t& value,
                                   const std::string& input,
                                   const std::string& path,
                                   std::vector<JUB_BYTE>& vSignatureRaw) {

//    try {
//        std::string from;
//        JUB_VERIFY_RV(GetAddress(path, 0, from));
//
//        TW::Polkadot::Transaction tx(TW::Polkadot::Address(to),
//                                     TW::Polkadot::Address(from),
//                                     nonce,
//                                     value,
//                                     gprice,
//                                     glimit);
//
////        tx.setPreImage(vUnsignedRaw);
////
//        HDNode hdkey;
//        JUB_UINT32 parentFingerprint;
//        JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));
//
//        uchar_vector privk(hdkey.private_key, hdkey.private_key + 32);
//        TW::PrivateKey twprivk = TW::PrivateKey(TW::Data(privk));
//
//        vSignatureRaw = TW::Polkadot::Signer::sign(twprivk, tx);
//    }
//    catch (...) {
//        return JUBR_ERROR_ARGS;
//    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
