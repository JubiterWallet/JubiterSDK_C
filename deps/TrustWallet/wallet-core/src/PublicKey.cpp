// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PublicKey.h"
#include "Data.h"

#include "TWPublicKeyType.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/ed25519-donna/ed25519-blake2b.h>
#include <TrezorCrypto/ed25519-donna/ed25519-donna.h>
#include <TrezorCrypto/nist256p1.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/sodium/keypair.h>

#include <assert.h>
#include <cstring>

namespace TW {

/// Determines if a collection of bytes makes a valid public key of the
/// given type.
bool PublicKey::isValid(const Data &data, enum TWPublicKeyType type) {
    const auto size = data.size();
    if (size == 0) {
        return false;
    }
    switch (type) {
    case TWPublicKeyTypeED25519:
        return size == ed25519Size || (size == ed25519Size + 1 && data[0] == 0x01);
    case TWPublicKeyTypeCURVE25519:
    case TWPublicKeyTypeED25519Blake2b:
        return size == ed25519Size;
    case TWPublicKeyTypeED25519Extended:
        return size == ed25519ExtendedSize;
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeNIST256p1:
        return size == secp256k1Size && (data[0] == 0x02 || data[0] == 0x03);
    case TWPublicKeyTypeSECP256k1Extended:
    case TWPublicKeyTypeNIST256p1Extended:
        return size == secp256k1ExtendedSize && data[0] == 0x04;
    default:
        return false;
    }
}

/// Initializes a public key with a collection of bytes.
///
/// @throws std::invalid_argument if the data is not a valid public key.
PublicKey::PublicKey(const Data &data, enum TWPublicKeyType type) : type(type) {
    if (!isValid(data, type)) {
        throw std::invalid_argument("Invalid public key data");
    }
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeSECP256k1Extended:
    case TWPublicKeyTypeNIST256p1Extended:
        bytes.reserve(data.size());
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
        break;

    case TWPublicKeyTypeED25519:
    case TWPublicKeyTypeCURVE25519:
        bytes.reserve(ed25519Size);
        if (data.size() == ed25519Size + 1) {
            std::copy(std::begin(data) + 1, std::end(data), std::back_inserter(bytes));
        } else {
            std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
        }
        break;
    case TWPublicKeyTypeED25519Blake2b:
        bytes.reserve(ed25519Size);
        assert(data.size() == ed25519Size); // ensured by isValid() above
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
        break;
    case TWPublicKeyTypeED25519Extended:
        bytes.reserve(ed25519ExtendedSize);
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
    }
}

PublicKey PublicKey::compressed() const {
    if (type != TWPublicKeyTypeSECP256k1Extended && type != TWPublicKeyTypeNIST256p1Extended) {
        return *this;
    }

    Data newBytes(secp256k1Size);
    assert(bytes.size() >= 65);
    newBytes[0] = 0x02 | (bytes[64] & 0x01);

    assert(type == TWPublicKeyTypeSECP256k1Extended || type == TWPublicKeyTypeNIST256p1Extended);
    switch (type) {
    case TWPublicKeyTypeSECP256k1Extended:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeSECP256k1);

    case TWPublicKeyTypeNIST256p1Extended:
    default:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeNIST256p1);
    }
}

PublicKey PublicKey::extended() const {
    Data newBytes(secp256k1ExtendedSize);
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
    case TWPublicKeyTypeCURVE25519:
    case TWPublicKeyTypeED25519Blake2b:
    case TWPublicKeyTypeED25519Extended:
        return *this;
    }
}

// JuBiter-defined
bool PublicKey::isValid() { return PublicKey::isValid(bytes, type); }

// JuBiter-defined
bool PublicKey::verifyAsDER(const Data &signatureAsDER, const Data &message) const {
    size_t iDerSignatureLen = signatureAsDER.size();
    uint8_t *derSignature   = new uint8_t[iDerSignatureLen + 1];
    memset(derSignature, 0x00, iDerSignatureLen + 1);
    std::copy(signatureAsDER.begin(), signatureAsDER.end(), derSignature);
    uint8_t sig[TW::Hash::sha512Size] = {
        0x00,
    };
    if (TW::Hash::sha512Size != der_to_ecdsa_sig(derSignature, sig)) {
        delete[] derSignature;
        derSignature = nullptr;
        return false;
    }
    delete[] derSignature;
    derSignature = nullptr;

    uchar_vector vSig(sig, TW::Hash::sha512Size);
    Data signature(vSig);

    return verify(signature, message);
}

bool PublicKey::verify(const Data &signature, const Data &message) const {
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
    case TWPublicKeyTypeED25519Extended:
        throw std::logic_error("Not yet implemented");
        // ed25519_sign_open(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    case TWPublicKeyTypeCURVE25519:
        auto ed25519PublicKey = Data();
        ed25519PublicKey.resize(PublicKey::ed25519Size);
        curve25519_pk_to_ed25519(ed25519PublicKey.data(), bytes.data());

        ed25519PublicKey[31] &= 0x7F;
        ed25519PublicKey[31] |= signature[63] & 0x80;

        // remove sign bit
        auto verifyBuffer = Data();
        append(verifyBuffer, signature);
        verifyBuffer[63] &= 127;
        return ed25519_sign_open(message.data(), message.size(), ed25519PublicKey.data(), verifyBuffer.data()) == 0;
    }
}

// JuBiter-defined
bool PublicKey::verify(const Data &signature, const Data &message,
                       int (*canonicalChecker)(uint8_t by, uint8_t sig[64])) const {

    // graphene adds 31 to the recovery id
    uint8_t by = signature[0];
    by -= 31;

    // check if the signature is acceptable or retry
    uint8_t sig[64] = {
        0x00,
    };
    std::copy(std::begin(signature) + 1, std::end(signature), std::begin(sig));
    if (canonicalChecker && !canonicalChecker(by, sig)) {
        return false;
    }

    return verify(Data(uchar_vector(sig, sizeof(sig) / sizeof(uint8_t))), message, by);
}

// JuBiter-defined
bool PublicKey::verify(const Data &signature, const Data &message, const int recid) const {

    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended: {
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
    case TWPublicKeyTypeNIST256p1Extended: {
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
    case TWPublicKeyTypeCURVE25519:
        throw std::logic_error("Not yet implemented");
        return false;
    case TWPublicKeyTypeED25519Extended:
        throw std::logic_error("Not yet implemented");
        return false;
    }
}

bool PublicKey::verifySchnorr(const Data &signature, const Data &message) const {
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
        return zil_schnorr_verify(&secp256k1, bytes.data(), signature.data(), message.data()) == 0;
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended:
    case TWPublicKeyTypeED25519:
    case TWPublicKeyTypeED25519Blake2b:
    default:
        return false;
    }
}

Data PublicKey::hash(const Data &prefix, Hash::Hasher hasher, bool skipTypeByte) const {
    const auto offset = std::size_t(skipTypeByte ? 1 : 0);
    const auto hash   = hasher(bytes.data() + offset, bytes.size() - offset);

    auto result = Data();
    result.reserve(prefix.size() + hash.size());
    append(result, prefix);
    append(result, hash);
    return result;
}

// JuBiter-defined
bool PublicKey::recover(Data &signature, const Data &message, int (*canonicalChecker)(uint8_t by, uint8_t sig[64])) {

    // graphene adds 31 to the recovery id
    int by = signature[0];
    by -= 31;

    uint8_t sig[64] = {
        0x00,
    };
    std::copy(std::begin(signature) + 1, std::end(signature), std::begin(sig));
    if (!recover(Data(uchar_vector(sig, sizeof(sig) / sizeof(uint8_t))), message, &by)) {
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
bool PublicKey::recover(const Data &signature, const Data &message, int *recid) {

    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended: {
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
    case TWPublicKeyTypeNIST256p1Extended: {
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
    case TWPublicKeyTypeCURVE25519:
        throw std::logic_error("Not yet implemented");
        return false;
    case TWPublicKeyTypeED25519Extended:
        throw std::logic_error("Not yet implemented");
        return false;
    }
}

PublicKey PublicKey::recover(const Data &signature, const Data &message) {
    if (signature.size() < 65) {
        throw std::invalid_argument("signature too short");
    }
    auto v = signature[64];
    if (v >= 27) {
        v -= 27;
    }
    TW::Data result(65);
    if (ecdsa_recover_pub_from_sig(&secp256k1, result.data(), signature.data(), message.data(), v) != 0) {
        throw std::invalid_argument("recover failed");
    }
    return PublicKey(result, TWPublicKeyTypeSECP256k1Extended);
}

bool PublicKey::isValidED25519() const {
    if (type != TWPublicKeyTypeED25519) {
        return false;
    }
    assert(bytes.size() == ed25519Size);
    ge25519 r;
    return ge25519_unpack_negative_vartime(&r, bytes.data()) != 0;
}

PublicKey PublicKey::p2trPublicKey() const {
    curve_point P;
    bignum256 t;
    curve_point Q;
    ecdsa_read_pubkey(&secp256k1, bytes.data(), &P);
    auto digest = TW::Hash::hash_TapTweak(bytes.data() + 1, 32);
    bn_read_be(digest.data(), &t);
    scalar_multiply(&secp256k1, &t, &Q);
    point_add(&secp256k1, &P, &Q);
    digest.resize(secp256k1Size);
    digest[0] = 0x02 | (Q.y.val[0] & 0x01);
    bn_write_be(&Q.x, digest.data() + 1);

    return PublicKey(digest, TWPublicKeyTypeSECP256k1);
}

} // namespace TW
