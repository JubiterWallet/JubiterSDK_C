
#include "Data.h"
#include "JUB_SDK_BTC.h"
#include "JUB_SDK_COMM.h"
#include "TWPublicKeyType.h"
#include <PublicKey.h>
#include <Solana/Address.h>
#include <Solana/Signer.h>
#include <Solana/Transaction.h>
#include <TrezorCrypto/bip32.h>
#include <string>
#include <token/SOL/JubiterBaseSOLImpl.h>

namespace jub {
namespace token {
JUB_RV JubiterBaseSOLImpl::VerifyTx(const uchar_vector &signedRaw, const TW::Data &publicKey) {

    try {
        auto tx       = TW::Solana::Transaction::decode(signedRaw);
        auto key      = TW::PublicKey(publicKey, TWPublicKeyTypeED25519);
        auto addr     = TW::Solana::Address{publicKey};
        auto index    = tx.getAccountIndex(addr);
        auto sig      = tx.signatures.at(index);
        auto sigBytes = TW::Data{sig.bytes.begin(), sig.bytes.end()};

        return key.verify(sigBytes, tx.messageData()) ? JUBR_OK : JUBR_VERIFY_SIGN_FAILED;
    } catch (...) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseSOLImpl::_getAddress(const TW::Data &publicKey, std::string &address) {
    try {
        auto _address = TW::Solana::Address(publicKey);

        address = _address.string();
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }
    return JUBR_OK;
}

JUB_RV JubiterBaseSOLImpl::CheckAddress(const std::string &address) {
    return TW::Solana::Address::isValid(address) ? JUBR_OK : JUBR_ARGUMENTS_BAD;
}

} // namespace token
} // namespace jub
