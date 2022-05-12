#include <token/ETH/JubiterBaseETHImpl.h>
#include <TrezorCrypto/bip32.h>
#include <PublicKey.h>
#include <Ethereum/RLP.h>
#include <Ethereum/AddressChecksum.h>
#include <Ethereum/Transaction.h>
#include <Ethereum/Signer.h>
#include "Ethereum/EIP712.h"

namespace jub {
namespace token {


JUB_RV JubiterBaseETHImpl::VerifyTx(const std::vector<JUB_BYTE>& vChainID,
                                    const uchar_vector& signedRaw,
                                    const TW::Data& publicKey) {

    TW::Ethereum::Signature signature;
    TW::Ethereum::TransactionNonTyped nonTyped;
    TW::Ethereum::TransactionEip1559 eip1559;
    TW::Ethereum::TransactionOptionalAccessList eip2930;
    if (!(  nonTyped.decoded(signedRaw, signature)
          || eip1559.decoded(signedRaw, signature)
          || eip2930.decoded(signedRaw, signature)
          )) {
        return JUBR_SIGN_FAILED;
    }

    bool b = false;
    if (nonTyped.isValid()) {
        b = TW::Ethereum::Signer::verify(TW::PublicKey(publicKey, _publicKeyType),
                                         vChainID,
                                         std::make_shared<TW::Ethereum::TransactionNonTyped>(nonTyped),
                                         TW::Ethereum::Signer::structToSignatureData(signature, nonTyped.usesReplayProtection(), vChainID));
    }
    else if (eip2930.isValid()) {
        b = TW::Ethereum::Signer::verify(TW::PublicKey(publicKey, _publicKeyType),
                                         vChainID,
                                         std::make_shared<TW::Ethereum::TransactionOptionalAccessList>(eip2930),
                                         TW::Ethereum::Signer::structToSignatureData(signature, eip2930.usesReplayProtection(), vChainID));
    }
    else if (eip1559.isValid()) {
        b = TW::Ethereum::Signer::verify(TW::PublicKey(publicKey, _publicKeyType),
                                         vChainID,
                                         std::make_shared<TW::Ethereum::TransactionEip1559>(eip1559),
                                         TW::Ethereum::Signer::structToSignatureData(signature, eip1559.usesReplayProtection(), vChainID));
    }
    else {
        return JUBR_SIGN_FAILED;
    }

    return (true == b ? JUBR_OK : JUBR_SIGN_FAILED);
}


JUB_RV JubiterBaseETHImpl::VerifyBytestring(const uchar_vector& vData,
                                            const uchar_vector& vSignature,
                                            const TW::Data& publicKey) {

    return (true == TW::Ethereum::Signer::verify(TW::PublicKey(publicKey, _publicKeyType),
                                                 vData,
                                                 vSignature) ? JUBR_OK : JUBR_VERIFY_SIGN_FAILED);
}


JUB_RV JubiterBaseETHImpl::VerifyTypedData(const bool& bMetamaskV4Compat,
                                           const std::string& typedDataInJSON,
                                           const std::vector<JUB_BYTE>& vSignature,
                                           const TW::Data& publicKey) {

    return (true == TW::Ethereum::Signer::verify(TW::PublicKey(publicKey, _publicKeyType),
                                                 typedDataInJSON,
                                                 vSignature) ? JUBR_OK : JUBR_VERIFY_SIGN_FAILED);
}


JUB_RV JubiterBaseETHImpl::_getAddress(const TW::Data& publicKey, std::string& address) {

    try {
        TW::PublicKey pubkey(publicKey, _publicKeyType);
        TW::PublicKey pubkeyExt = pubkey.extended();
        uint8_t extendedPubkey[TW::PublicKey::secp256k1ExtendedSize] = {0x00,};
        std::copy(std::begin(pubkeyExt.bytes), std::end(pubkeyExt.bytes), std::begin(extendedPubkey));

        JUB_BYTE ethKeyHash[20] = { 0, };
        if (1 != pubkey_get_ethereum_pubkeyhash(extendedPubkey, TW::PublicKey::secp256k1ExtendedSize,
                                                ethKeyHash)) {
            return JUBR_ARGUMENTS_BAD;
        }

        uchar_vector _address(ethKeyHash, ethKeyHash + 20);
        address = TW::Ethereum::checksumed(TW::Ethereum::Address(_address), TW::Ethereum::ChecksumType::eip55);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
