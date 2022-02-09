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

    TW::Ethereum::Transaction tx = TW::Ethereum::RLP::decode(TW::Data(signedRaw));
    if (!tx.isValid()) {
        return JUBR_ARGUMENTS_BAD;
    }

    TW::Ethereum::Signer signer(vChainID);
    if (!signer.verify(vChainID,
                       TW::PublicKey(publicKey, _publicKeyType),
                       tx)) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseETHImpl::VerifyBytestring(const std::vector<JUB_BYTE>& vChainID,
                                            const uchar_vector& vData,
                                            const uchar_vector& vSignature,
                                            const TW::Data& publicKey) {

    TW::Ethereum::Signer signer(vChainID);
    if (!signer.verify(vChainID,
                       TW::PublicKey(publicKey, _publicKeyType),
                       vData,
                       vSignature)) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseETHImpl::VerifyTypedData(const bool& bMetamaskV4Compat,
                                           const std::string& typedDataInJSON,
                                           const std::vector<JUB_BYTE>& vSignature,
                                           const TW::Data& publicKey) {

    nlohmann::json typedData = nlohmann::json::parse(typedDataInJSON);
    if (nlohmann::detail::value_t::object != typedData.type()) {
        return JUBR_ARGUMENTS_BAD;
    }

    if (!eth::EIP712::parseJSON(typedData)) {
        return JUBR_ARGUMENTS_BAD;
    }

    uchar_vector hashDomain = eth::EIP712::typed_data_envelope("EIP712Domain", typedData["domain"]);
    if (hashDomain.empty()) {
        return JUBR_ERROR;
    }

    uchar_vector hashMessage = eth::EIP712::typed_data_envelope(typedData["primaryType"].get<std::string>().c_str(),
                                                                typedData["message"]);
    if (hashMessage.empty()) {
        return JUBR_ERROR;
    }

    TW::Ethereum::Signer signer({});
    if (!signer.verify({},
                       TW::PublicKey(publicKey, _publicKeyType),
                       hashDomain, hashMessage,
                       vSignature)) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseETHImpl::_getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
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
