// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "Hash.h"

#include <TrustWalletCore/TWHDVersion.h>
#include <TrustWalletCore/TWPublicKeyType.h>

#include <cstdint>
#include <stdexcept>
#include <string>

namespace TW {

class PublicKey {
  public:
    /// The number of bytes in a secp256k1 and nist256p1 public key.
    static const size_t secp256k1Size = 33;

    /// The number of bytes in a ed25519 public key.
    static const size_t ed25519Size = 32;

    static const size_t ed25519ExtendedSize = 64;

    /// The number of bytes in a secp256k1 and nist256p1 extended public key.
    static const size_t secp256k1ExtendedSize = 65;

    /// The public key bytes.
    Data bytes;

    /// The type of the public key.
    ///
    /// This has information about the elliptic curve and other parameters
    /// used when generating the public key.
    enum TWPublicKeyType type;

    /// Determines if a collection of bytes makes a valid public key of the
    /// given type.
    static bool isValid(const Data &data, enum TWPublicKeyType type);

    /// Get public key from xpub
    static PublicKey FromXpub(const std::string &xpub, const std::string curveName, uint32_t pubVer = TWHDVersionXPUB);

    /// Initializes a public key with a collection of bytes.
    ///
    /// @throws std::invalid_argument if the data is not a valid public key.
    explicit PublicKey(const Data &data, enum TWPublicKeyType type);

    /// Determines if this is a compressed public key.
    bool isCompressed() const {
        return type != TWPublicKeyTypeSECP256k1Extended && type != TWPublicKeyTypeNIST256p1Extended;
    }

    /// Returns a compressed version of this public key.
    PublicKey compressed() const;

    /// Returns an extended version of this public key.
    PublicKey extended() const;

    // JuBiter-defined
    bool isValid();

    // JuBiter-defined
    /// Verifies a signature for the provided message. The signature is encoded with
    /// DER.
    bool verifyAsDER(const Data &signatureAsDER, const Data &message) const;

    /// Verifies a signature for the provided message.
    bool verify(const Data &signature, const Data &message) const;
    // JuBiter-defined
    /// Verifies a signature for the provided message, including recover id.
    bool verify(const Data &signature, const Data &message, int (*canonicalChecker)(uint8_t by, uint8_t sig[64])) const;
    // JuBiter-defined
    /// Verifies a signature for the provided message, include recover id.
    bool verify(const Data &signature, const Data &message, const int recid) const;

    /// Verifies a schnorr signature for the provided message.
    bool verifySchnorr(const Data &signature, const Data &message) const;

    /// Computes the public key hash.
    ///
    /// The public key hash is computed by applying the hasher to the public key
    /// bytes and then prepending the prefix.
    Data hash(const Data &prefix, Hash::Hasher hasher = Hash::sha256ripemd, bool skipTypeByte = false) const;

    // JuBiter-defined
    /// Recover the recover id of a signature for the provided message.
    bool recover(Data &signature, const Data &message, int (*canonicalChecker)(uint8_t by, uint8_t sig[64]));
    bool recover(const Data &signature, const Data &message, int *recid);
    /// Recover public key from signature (SECP256k1Extended)
    static PublicKey recover(const Data &signature, const Data &message);

    /// Check if this key makes a valid ED25519 key (it is on the curve)
    bool isValidED25519() const;

    // bip86 derivation p2tr public key
    PublicKey p2trPublicKey() const;
};

inline bool operator==(const PublicKey &lhs, const PublicKey &rhs) { return lhs.bytes == rhs.bytes; }
inline bool operator!=(const PublicKey &lhs, const PublicKey &rhs) { return lhs.bytes != rhs.bytes; }

} // namespace TW

/// Wrapper for C interface.
struct TWPublicKey {
    TW::PublicKey impl;
};
