// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full JuBiter copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <cstdint>
#include <string>

namespace TW::NervosCKB {


TW_EXPORT_ENUM(uint8_t)
enum TWNervosCKBAddressFormat {
    TWNervosCKBAddressShort    = 0x01,
    TWNervosCKBAddressFullData = 0x02,
    TWNervosCKBAddressFullType = 0x04
};


//TW_EXPORT_ENUM(uint8_t)
//enum TWNervosCKBCodeHashIndex {
//    TWNervosCKBSingleSig    = 0x00,
//    TWNervosCKBMultiSig     = 0x01,
//    TWNervosCKBAnyOneCanPay = 0x02
//};


/// A Segwit address.
/// Note: Similar to Bech32Address, but it differs enough so that reuse makes no sense.
class SegwitAddress {
  public:
    /// Human-readable part.
    ///
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string hrp;

    /// Witness program version.
    int format;

    /// code hash index.
    int codeHashIndex;

    /// code hash.
    std::vector<uint8_t> codeHash;

    /// args.
    std::vector<uint8_t> args;

    /// Determines whether a string makes a valid Bech32 address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid Bech32 address, and the HRP
    /// matches.
    static bool isValid(const std::string& string, const std::string& hrp);

    /// Initializes a Bech32 address with a human-readable part, a code hash
    /// index, and an args.
    SegwitAddress(std::string hrp, int format, int codeHashIndex, std::vector<uint8_t> args)
        : hrp(std::move(hrp)), format(format), codeHashIndex(codeHashIndex), args(std::move(args)), codeHash() {}

    /// Initializes a Bech32 address with a human-readable part, a code hash
    /// index, a code hash and an args.
    SegwitAddress(std::string hrp, int format, std::vector<uint8_t> codeHash, std::vector<uint8_t> args)
        : hrp(std::move(hrp)), format(format), codeHashIndex(0x00), codeHash(std::move(codeHash)), args(std::move(args)) {}

    /// Initializes a Bech32 address with a public key and a HRP prefix.
    SegwitAddress(const PublicKey& publicKey, int codeHashIndex, std::string hrp);

    /// Decodes a SegWit address.
    ///
    /// \returns a pair with the address and a success flag.
    static std::pair<SegwitAddress, bool> decode(const std::string& addr);

    /// Encodes the SegWit address.
    ///
    /// \returns encoded address string, or empty string on failure.
    std::string string() const;

    /// Initializes a Bech32 address with raw data.
    static std::pair<SegwitAddress, bool> fromRaw(const std::string& hrp,
                                                  const std::vector<uint8_t>& data);
    static std::pair<SegwitAddress, bool> fromPublickey(const std::string& hrp,
                                                        int format, int codeHashIndex,
                                                        const std::vector<uint8_t>& data);
    static std::pair<SegwitAddress, bool> fromPublickey(const std::string& hrp,
                                                        int format, int codeHashIndex,
                                                        const PublicKey& publickey);

    bool operator==(const SegwitAddress& rhs) const {
        return hrp == rhs.hrp && codeHashIndex == rhs.codeHashIndex &&
        args == rhs.args && codeHash == rhs.codeHash;
    }

  private:
    SegwitAddress() = default;
};

} // namespace TW::NervosCKB
//
///// Wrapper for C interface.
//struct TWSegwitAddress {
//    TW::NervosCKB::SegwitAddress impl;
//};
