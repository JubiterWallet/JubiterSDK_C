// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>
#include <TrustWalletCore/TWCoinType.h>

#include <string>
#include <vector>
#include <assert.h>

using namespace TW;

namespace TW::NervosCKB {

static const uint8_t personal_ckb[16] =
{
    99, 107, 98, 45, 100, 101, 102, 97, 117, 108, 116, 45, 104, 97, 115, 104
};

enum ScriptHashType {
    DATA,
    TYPE
};

class Script {
  public:
    static size_t LOCK_SCRIPT_ITEM_COUNT;

    /// Script raw bytes.
    Data bytes;

    /// Initializes an empty script.
    Script() = default;

    /// Initializes a script with a collection of raw bytes.
    template <typename It>
    Script(It begin, It end) : bytes(begin, end) {}

    /// Initializaes a script with a collection of raw bytes by moving.
    explicit Script(Data&& bytes) : bytes(bytes) {}

    /// Whether the script is empty.
    bool empty() const { return bytes.empty(); }

    /// Returns the script's script hash.
    Data hash() const;

    /// Builds a lock script from a public key hash.
    static bool buildLockScript(const TW::Data& codeHash, const uint8_t type, const TW::Data& args, TW::Data& scriptLock);
    /// Builds a type script from a public key hash.
    static bool buildTypeScript(const TW::Data& codeHash, const uint8_t type, const TW::Data& args, TW::Data& scriptType);

    static bool parseScript(const TW::Data& srp, TW::Data& codeHash, uint8_t& type, TW::Data& args);
    static std::vector<TW::Data> serializeScript(const TW::Data& srp);

    /// Encodes the script.
    void encode(Data& data) const;
    Data encode() const;

    /// Encodes the zero script.
    void encodeZero(Data& data) const;

    /// Decodes the script.
    bool decode(const Data& data);

    /// Return the size of the script,
    /// including the number of bytes that represent the number of script bytes(decodeVarInt())
    size_t size();

    /// Encodes a small integer
    static uint8_t encodeNumber(int n) {
        assert(n >= 0 && n <= 16);
        if (n == 0) {
            return OP_0;
        }
        return OP_1 + uint8_t(n - 1);
    }

    Script& operator+(const Script& rhs) {
        Data rds;
        rhs.encode(rds);

        this->bytes.insert(this->bytes.end(), rds.begin(), rds.end());
        return *this;
    }

    Script& operator+=(const Script& rhs) {
        return (*this + rhs);
    }
};

inline bool operator==(const Script& lhs, const Script& rhs) {
    return lhs.bytes == rhs.bytes;
}
inline bool operator!=(const Script& lhs, const Script& rhs) {
    return !(lhs == rhs);
}

} // namespace TW::NervosCKB

/// Wrapper for C interface.
struct TWNervosCKBScript {
    TW::NervosCKB::Script impl;
};
