// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Base58.h"
#include <TrezorCrypto/ecdsa.h>

using namespace TW::Ripple;

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::ripple.decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::ripple.decodeCheck(string);
    if (decoded.size() != Address::size) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    /// see type prefix: https://developers.ripple.com/base58-encodings.html
    bytes[0] = 0x00;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.data() + 1);
}
Address::Address(const std::array<byte, size>& data)
{
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

std::string Address::string() const {
    return Base58::ripple.encodeCheck(bytes);
}

// JuBiter-defined
/// Returns the length of the pubkey hash.
size_t Address::pubkeyHashSize() {
    return (size - 1);
}
