// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PublicKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/ed25519-donna/ed25519-blake2b.h>
#include <TrezorCrypto/nist256p1.h>
#include <TrezorCrypto/secp256k1.h>
#include "mSIGNA/stdutils/uchar_vector.h"

using namespace TW;

PublicKey PublicKey::compressed() const {
    if (type != TWPublicKeyTypeSECP256k1Extended && type != TWPublicKeyTypeNIST256p1Extended) {
        return *this;
    }

    std::array<uint8_t, secp256k1Size> newBytes;
    newBytes[0] = 0x02 | (bytes[64] & 0x01);

    switch (type) {
    case TWPublicKeyTypeSECP256k1Extended:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeSECP256k1);
    case TWPublicKeyTypeNIST256p1Extended:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeNIST256p1);
    default:
        return *this;
    }
}

PublicKey PublicKey::extended() const {
    std::array<uint8_t, secp256k1ExtendedSize> newBytes;
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
        ecdsa_uncompress_pubkey(&secp256k1, bytes.data(), newBytes.data());
        return PublicKey(newBytes, TWPublicKeyTypeSECP256k1Extended);
    case TWPublicKeyTypeSECP256k1Extended:
        return *this;
    case TWPublicKeyTypeNIST256p1:
        ecdsa_uncompress_pubkey(&nist256p1, bytes.data(), newBytes.data());
        return PublicKey(newBytes, TWPublicKeyTypeNIST256p1Extended);
    case TWPublicKeyTypeNIST256p1Extended:
        return *this;
    case TWPublicKeyTypeED25519:
    case TWPublicKeyTypeED25519Blake2b:
       return *this;
    }
}

// JuBiter-defined
bool PublicKey::verifyAsDER(const Data& signatureAsDER, const Data& message) const {
    size_t iDerSignatureLen = signatureAsDER.size();
    uint8_t *derSignature = new uint8_t[iDerSignatureLen+1];
    memset(derSignature, 0x00, iDerSignatureLen+1);
    std::copy(signatureAsDER.begin(), signatureAsDER.end(), derSignature);
    uint8_t sig[TW::Hash::sha512Size] = {0x00,};
    if (TW::Hash::sha512Size != der_to_ecdsa_sig(derSignature, sig)) {
        delete[] derSignature; derSignature = nullptr;
        return false;
    }
    delete[] derSignature; derSignature = nullptr;

    uchar_vector vSig(sig, TW::Hash::sha512Size);
    Data signature(vSig);

    return verify(signature, message);
}

bool PublicKey::verify(const Data& signature, const Data& message) const {
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
        return ecdsa_verify_digest(&secp256k1, bytes.data(), signature.data(), message.data()) == 0;
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended:
        return ecdsa_verify_digest(&nist256p1, bytes.data(), signature.data(), message.data()) == 0;
    case TWPublicKeyTypeED25519:
        return ed25519_sign_open(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    case TWPublicKeyTypeED25519Blake2b:
        return ed25519_sign_open_blake2b(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    }
}

// JuBiter-defined
bool PublicKey::verify(const Data& signature, const Data& message, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) const {

    // graphene adds 31 to the recovery id
    uint8_t by = signature[0];
    by -= 31;

    // check if the signature is acceptable or retry
    uint8_t sig[64] = {0x00,};
    std::copy(std::begin(signature)+1, std::end(signature), std::begin(sig));
    if (canonicalChecker && !canonicalChecker(by, sig)) {
        return false;
    }

    return verify(Data(uchar_vector(sig, sizeof(sig)/sizeof(uint8_t))), message, by);
}

// JuBiter-defined
bool PublicKey::verify(const Data& signature, const Data& message, const int recid) const {

    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
    {
        TW::Data recoverPk(PublicKey::secp256k1ExtendedSize);
        if (0 != ecdsa_recover_pub_from_sig(&secp256k1, recoverPk.data(), signature.data(), message.data(), recid)) {
            return false;
        }

        TW::Data uncompressed(PublicKey::secp256k1ExtendedSize);
        if (1 != ecdsa_uncompress_pubkey(&secp256k1, bytes.data(), uncompressed.data())) {
            return false;
        }

        return (recoverPk == uncompressed);
    }
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended:
    {
        TW::Data recoverPk(PublicKey::secp256k1ExtendedSize);
        if (0 != ecdsa_recover_pub_from_sig(&nist256p1, recoverPk.data(), signature.data(), message.data(), recid)) {
            return false;
        }

        TW::Data uncompressed(PublicKey::secp256k1ExtendedSize);
        if (1 != ecdsa_uncompress_pubkey(&nist256p1, bytes.data(), uncompressed.data())) {
            return false;
        }

        return (recoverPk == uncompressed);
    }
    case TWPublicKeyTypeED25519:
        return ed25519_sign_open(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    case TWPublicKeyTypeED25519Blake2b:
        return ed25519_sign_open_blake2b(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    }
}

// JuBiter-defined
bool PublicKey::recover(Data& signature, const Data& message, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) {

    // graphene adds 31 to the recovery id
    int by = signature[0];
    by -= 31;

    uint8_t sig[64] = {0x00,};
    std::copy(std::begin(signature)+1, std::end(signature), std::begin(sig));
    if (!recover(Data(uchar_vector(sig, sizeof(sig)/sizeof(uint8_t))), message, &by)) {
        return false;
    }

    // check if the signature is acceptable or retry
    if (canonicalChecker && !canonicalChecker(by, sig)) {
        return false;
    }

    // graphene adds 31 to the recovery id
    by += 31;
    signature[0] = by;

    return true;
}

// JuBiter-defined
/// Recover the recover id of a signature for the provided message.
bool PublicKey::recover(const Data& signature, const Data& message, int *recid) {

    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
    {
        TW::Data uncompressed(PublicKey::secp256k1ExtendedSize);
        if (1 != ecdsa_uncompress_pubkey(&secp256k1, &bytes[0], &uncompressed[0])) {
            return false;
        }

        if (0 != ecdsa_recover_sig(&secp256k1, uncompressed.data(), signature.data(), message.data(), recid)) {
            return false;
        }

        return true;
    }
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended:
    {
        TW::Data uncompressed(PublicKey::secp256k1ExtendedSize);
        if (1 != ecdsa_uncompress_pubkey(&nist256p1, bytes.data(), uncompressed.data())) {
            return false;
        }

        if (0 != ecdsa_recover_sig(&nist256p1, uncompressed.data(), signature.data(), message.data(), recid)) {
            return false;
        }

        return true;
    }
    case TWPublicKeyTypeED25519:
        return ed25519_sign_open(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    case TWPublicKeyTypeED25519Blake2b:
        return ed25519_sign_open_blake2b(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    }
}

bool PublicKey::verifySchnorr(const Data& signature, const Data& message) const {
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
        return zil_schnorr_verify(&secp256k1, bytes.data(), signature.data(), message.data(), static_cast<uint32_t>(message.size())) == 0;
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended:
        return false;
    case TWPublicKeyTypeED25519:
    case TWPublicKeyTypeED25519Blake2b:
        return false;
    }
}

Data PublicKey::hash(const Data& prefix, Hash::Hasher hasher, bool skipTypeByte) const {
    const auto offset = std::size_t(skipTypeByte ? 1 : 0);
    const auto hash = hasher(bytes.data() + offset, bytes.data() + bytes.size());

    auto result = Data();
    result.reserve(prefix.size() + hash.size());
    append(result, prefix);
    append(result, hash);
    return result;
}
