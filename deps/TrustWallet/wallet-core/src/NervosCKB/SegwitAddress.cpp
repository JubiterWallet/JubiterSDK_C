// Copyright © 2021 JuBiter
// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full JuBiter copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SegwitAddress.h"
#include "../Bech32.h"

#include "../NervosCKB/Script.h"
#include <TrezorCrypto/ecdsa.h>
#include <cstring>
#include <trezor-crypto/src/hasher.c>
#include <utility>
#include <string>
#include <vector>

using namespace TW::NervosCKB;

bool SegwitAddress::isValid(const std::string &string) {
    auto dec = Bech32::decode(string);
    //    if (dec.second.empty()) {
    if (dec.data.empty()) {
        return false;
    }

    Data conv;
    //    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin(), dec.second.end()))
    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.data.begin(), dec.data.end())) || 0 == conv[0] ||
        TWNervosCKBAddressFormat::TWNervosCKBAddressFullType < conv[0]) {
        return false;
    }

    if (TWNervosCKBAddressFormat::TWNervosCKBAddressShort == conv[0] && (1 + 1 + TW::Hash::ripemdSize) != conv.size()) {
        return false;
    } else if (TWNervosCKBAddressFormat::TWNervosCKBAddressFullData == conv[0] &&
               (1 + 1 + TW::Hash::ripemdSize) != conv.size()) {
        return false;
    } else if (TWNervosCKBAddressFormat::TWNervosCKBAddressFullType == conv[0] &&
               (1 + TW::Hash::sha256Size + TW::Hash::ripemdSize) != conv.size()) {
        return false;
    }

    return true;
}

bool SegwitAddress::isValid(const std::string &string, const std::string &hrp) {
    auto dec = Bech32::decode(string);
    //    if (dec.second.empty()) {
    if (dec.data.empty()) {
        return false;
    }
    //    if (dec.first != hrp) {
    if (dec.hrp != hrp) {
        return false;
    }

    return TW::NervosCKB::SegwitAddress::isValid(string);
}

SegwitAddress::SegwitAddress(const PublicKey &publicKey, int codeHashIndex, std::string hrp)
    : hrp(std::move(hrp)), format(TWNervosCKBAddressFormat::TWNervosCKBAddressShort), codeHashIndex(codeHashIndex),
      codeHash(), args() {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("SegwitAddress needs a compressed SECP256k1 public key.");
    }
    args.resize(TW::Hash::ripemdSize);
    ecdsa_get_pubkeyhash(publicKey.compressed().bytes.data(), HASHER_BLAKE2B, args.data());
}

std::pair<SegwitAddress, bool> SegwitAddress::decode(const std::string &addr) {
    auto dec = Bech32::decode(addr);
    //    if (dec.second.empty()) {
    if (dec.data.empty()) {
        return std::make_pair(SegwitAddress(), false);
    }

    Data conv;
    //    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin(), dec.second.end()))
    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.data.begin(), dec.data.end())) || 0 == conv[0] ||
        TWNervosCKBAddressFormat::TWNervosCKBAddressFullType < conv[0]) {
        return std::make_pair(SegwitAddress(), false);
    }

    if (TWNervosCKBAddressFormat::TWNervosCKBAddressShort == conv[0] && (1 + 1 + TW::Hash::ripemdSize) != conv.size()) {
        return std::make_pair(SegwitAddress(), false);
    } else if (TWNervosCKBAddressFormat::TWNervosCKBAddressFullData == conv[0] &&
               (1 + 1 + TW::Hash::ripemdSize) != conv.size()) {
        return std::make_pair(SegwitAddress(), false);
    } else if (TWNervosCKBAddressFormat::TWNervosCKBAddressFullType == conv[0] &&
               (1 + TW::Hash::sha256Size + TW::Hash::ripemdSize) != conv.size()) {
        return std::make_pair(SegwitAddress(), false);
    }

    if (TWNervosCKBAddressFormat::TWNervosCKBAddressShort == conv[0]) {
        //        return std::make_pair(SegwitAddress(dec.first, conv[0], conv[1], TW::Data(conv.begin()+2,
        //        conv.end())), true);
        return std::make_pair(SegwitAddress(dec.hrp, conv[0], conv[1], TW::Data(conv.begin() + 2, conv.end())), true);
    } else {
        //        return std::make_pair(SegwitAddress(dec.first, conv[0], TW::Data(conv.begin()+1,
        //        conv.begin()+1+TW::Hash::sha256Size), TW::Data(conv.begin()+1+TW::Hash::sha256Size, conv.end())),
        //        true);
        return std::make_pair(SegwitAddress(dec.hrp, conv[0],
                                            TW::Data(conv.begin() + 1, conv.begin() + 1 + TW::Hash::sha256Size),
                                            TW::Data(conv.begin() + 1 + TW::Hash::sha256Size, conv.end())),
                              true);
    }
}

std::string SegwitAddress::string() const {
    // https://github.com/nervosnetwork/rfcs/blob/master/rfcs/0021-ckb-address-format/0021-ckb-address-format.md
    Data payload;
    payload.push_back(static_cast<uint8_t>(format));
    switch (format) {
    case TWNervosCKBAddressFormat::TWNervosCKBAddressShort:
    case TWNervosCKBAddressFormat::TWNervosCKBAddressFullData:
        payload.push_back(static_cast<uint8_t>(codeHashIndex));
        payload.insert(payload.end(), args.begin(), args.end());
        break;
    case TWNervosCKBAddressFormat::TWNervosCKBAddressFullType:
        payload.insert(payload.end(), codeHash.begin(), codeHash.end());
        payload.insert(payload.end(), args.begin(), args.end());
        break;
    default:
        return {};
    }

    Data enc;
    //    Bech32::convertBits<8, 5, true>(enc, payload);
    Bech32::convertBits<8, 5, true>(enc, payload);
    std::string result = Bech32::encode(hrp, enc);
    if (!decode(result).second) {
        return {};
    }
    return result;
}

std::pair<SegwitAddress, bool> SegwitAddress::fromRaw(const std::string &hrp, const std::vector<uint8_t> &payload) {
    Data conv;
    //    if (!Bech32::convertBits<5, 8, false>(conv, Data(payload.begin(), payload.end()))
    if (!Bech32::convertBits<5, 8, false>(conv, Data(payload.begin(), payload.end())) || 0 == conv[0] ||
        TWNervosCKBAddressFormat::TWNervosCKBAddressFullType < conv[0]) {
        return std::make_pair(SegwitAddress(), false);
    }

    if (TWNervosCKBAddressFormat::TWNervosCKBAddressShort == conv[0]) {
        if ((1 + 1 + TW::Hash::ripemdSize) != conv.size()) {
            return std::make_pair(SegwitAddress(), false);
        }

        return std::make_pair(SegwitAddress(hrp, conv[0], conv[1], TW::Data(conv.begin() + 2, conv.end())), true);
    } else {
        if ((1 + TW::Hash::sha256Size + TW::Hash::ripemdSize) != conv.size()) {
            return std::make_pair(SegwitAddress(), false);
        }

        return std::make_pair(SegwitAddress(hrp, conv[0],
                                            TW::Data(conv.begin() + 1, conv.begin() + 1 + TW::Hash::sha256Size),
                                            TW::Data(conv.begin() + 1 + TW::Hash::sha256Size, conv.end())),
                              true);
    }
}

std::pair<SegwitAddress, bool> SegwitAddress::fromPublickey(const std::string &hrp, int format, int codeHashIndex,
                                                            const std::vector<uint8_t> &data) {
    Hasher hasher;
    hasher_InitParam(&hasher, HasherType::HASHER_BLAKE2B_PERSONAL, personal_ckb,
                     sizeof(personal_ckb) / sizeof(uint8_t));

    TW::Data blake(HASHER_DIGEST_LENGTH);
    if (0 != hasher_InitParam_Raw(&hasher, &data[0], (uint32_t)data.size(), &blake[0])) {
        return std::make_pair(SegwitAddress(), false);
    }
    TW::Data args(TW::Hash::ripemdSize);
    std::memcpy(&args[0], &blake[0], TW::Hash::ripemdSize);
    return std::make_pair(TW::NervosCKB::SegwitAddress(hrp, format, codeHashIndex, args), true);
}

std::pair<SegwitAddress, bool> SegwitAddress::fromPublickey(const std::string &hrp, int format, int codeHashIndex,
                                                            const TW::PublicKey &publickey) {
    if (TWPublicKeyType::TWPublicKeyTypeSECP256k1 != publickey.type) {
        return std::make_pair(SegwitAddress(), false);
    }
    return SegwitAddress::fromPublickey(hrp, format, codeHashIndex, publickey.bytes);
}
