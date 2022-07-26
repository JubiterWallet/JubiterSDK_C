#include <token/SOL/JubiterBaseSOLImpl.h>
#include <TrezorCrypto/bip32.h>
#include <PublicKey.h>
#include <Solana/Address.h>
#include <Solana/Transaction.h>
#include <Solana/Signer.h>

namespace jub {
namespace token {


JUB_RV JubiterBaseSOLImpl::VerifyTx(const uchar_vector& signedRaw,
                                    const TW::Data& publicKey) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBaseSOLImpl::_getAddress(const TW::Data& publicKey, std::string& address) {

    try {
//        TW::PublicKey pubkey(publicKey, _publicKeyType);
//        TW::PublicKey pubkeyExt = pubkey.extended();
//        uint8_t extendedPubkey[TW::PublicKey::secp256k1ExtendedSize] = {0x00,};
//        std::copy(std::begin(pubkeyExt.bytes), std::end(pubkeyExt.bytes), std::begin(extendedPubkey));
//
//        JUB_BYTE ethKeyHash[20] = { 0, };
//        if (1 != pubkey_get_ethereum_pubkeyhash(extendedPubkey, TW::PublicKey::secp256k1ExtendedSize,
//                                                ethKeyHash)) {
//            return JUBR_ARGUMENTS_BAD;
//        }
//
//        uchar_vector _address(ethKeyHash, ethKeyHash + 20);
//        address = TW::Solana::checksumed(TW::Solana::Address(_address), TW::Solana::ChecksumType::eip55);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
