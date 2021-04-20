#include "token/FIL/JubiterBaseFILImpl.h"

#include <TrezorCrypto/bip32.h>
#include <Filecoin/Address.h>
#include <Filecoin/Transaction.h>
#include <PublicKey.h>
#include <../PrivateKey.h>
#include <Cbor.h>


namespace jub {
namespace token {


JUB_RV JubiterBaseFILImpl::_getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                              uint32_t hdVersionPub, uint32_t hdVersionPrv) {

    try {
        HDNode hdkey;
        uint32_t fingerprint = 0;
        if (0 != hdnode_deserialize(xpub.c_str(),
                                    hdVersionPub, hdVersionPrv,
                                    _curve_name, &hdkey, &fingerprint)) {
            return JUBR_ARGUMENTS_BAD;
        }

        uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
        publicKey = TW::Data(vPublicKey);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseFILImpl::_getAddress(const TW::Data& publicKey, std::string& address) {

    try {
        // https://spec.filecoin.io/appendix/address/#section-appendix.address.protocol-1-libsecpk1-elliptic-curve-public-keys
        // Protocol 1 addresses represent secp256k1 public encryption keys. The payload field contains the Blake2b 160 hash of the uncompressed public key (65 bytes).
        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);
        if (twpk.isCompressed()) {
            twpk = twpk.extended();
        }
        TW::Filecoin::Address addr(twpk);
        address = addr.string();
        if (!TW::Filecoin::Address::isValid(address)) {
            return JUBR_ERROR;
        }
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
