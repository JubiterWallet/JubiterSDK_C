// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base58.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <cassert>
#include <stdexcept>

namespace TW::Tron {


bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }

    if (decoded[0] != prefix) {
        return false;
    }

    return true;
}


Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Invalid public key type");
    }
    const auto pkdata = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    const auto keyhash = Hash::keccak256(pkdata);
    bytes[0] = prefix;
    std::copy(keyhash.end() - size + 1, keyhash.end(), bytes.begin() + 1);
}


// JuBiter-defined
std::string Address::fromHex(const std::string& decoded) {

    std::string toHex = hex(decoded);

    return Address::fromHex(parse_hex(std::begin(toHex), std::end(toHex)));
}


// JuBiter-defined
std::string Address::fromHex(const Data& decoded) {

    std::string o = "";
    if (decoded.size() != Address::size) {
        return o;
    }

    if (decoded[0] != prefix) {
        return o;
    }

    return Address(decoded).string();
}


// JuBiter-defined
Data Address::toHex(const std::string& string) {
    Data o;

    if (Address::isValid(string)) {
        o = Base58::bitcoin.decodeCheck(string);
    }
    else {
        o = TW::parse_hex(string);
    }

    return o;
}


} // namespace TW::Tron end
