// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>
#include <TrustWalletCore/TWCoinType.h>

#include <string>
#include <vector>
#include <assert.h>

namespace TW::Bitcoin {

class Script {
  public:
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

    /// Determines whether this is a pay-to-script-hash (P2SH) script.
    bool isPayToScriptHash() const;

    /// Determines whether this is a pay-to-witness-script-hash (P2WSH) script.
    bool isPayToWitnessScriptHash() const;

    /// Determines whether this is a witness programm script.
    bool isWitnessProgram() const;

    /// Matches the script to a pay-to-public-key (P2PK) script.
    bool matchPayToPubkey(Data& publicKey) const;

    /// Matches the script to a pay-to-public-key-hash (P2PKH).
    bool matchPayToPubkeyHash(Data& keyHash) const;

    /// Matches the script to a pay-to-script-hash (P2SH).
    bool matchPayToScriptHash(Data& scriptHash) const;

    /// Matches the script to a pay-to-witness-public-key-hash (P2WPKH).
    bool matchPayToWitnessPublicKeyHash(Data& keyHash) const;

    /// Matches the script to a pay-to-witness-script-hash (P2WSH).
    bool matchPayToWitnessScriptHash(Data& scriptHash) const;

    /// Matches the script to a multisig script.
    bool matchMultisig(std::vector<Data>& publicKeys, int& required) const;

    // JuBiter-defined
    /// Matches the script to a scriptSig for a pay-to-public-key-hash (P2PKH).
    bool matchPayToPublicKeyHashScriptSig(Data& signature, Data& publicKey) const;

    /// Builds a pay-to-public-key-hash (P2PKH) script from a public key hash.
    static Script buildPayToPublicKeyHash(const Data& hash);

    /// Builds a pay-to-script-hash (P2SH) script from a script hash.
    static Script buildPayToScriptHash(const Data& scriptHash);

    /// Builds a pay-to-witness-program script, P2WSH or P2WPKH.
    static Script buildPayToWitnessProgram(const Data& program);

    /// Builds a pay-to-witness-public-key-hash (P2WPKH) script from a public
    /// key hash.
    static Script buildPayToWitnessPubkeyHash(const Data& hash);

    /// Builds a pay-to-witness-script-hash (P2WSH) script from a script hash.
    static Script buildPayToWitnessScriptHash(const Data& scriptHash);

    // JuBiter-defined
    /// Builds a redeem script for pay-to-script-hash (P2SH).
    static Script buildRedeemScript(const uint8_t m, const uint8_t n, const std::vector<Data>& publicKeys);

    // JuBiter-defined
    /// Builds a scriptSig for pay-to-public-key-hash (P2PKH) script.
    static Script buildPayToPublicKeyHashScriptSig(const Data& signature, const Data& publicKey);

    // JuBiter-defined
    /// Builds a scriptSig for pay-to-public-key-hash (P2WPKH) script.
    static std::vector<Data> buildPayToPublicKeyHashScriptSigWitness(Data signature, Data publicKey);

    // JuBiter-defined
    /// Builds a scriptSig for the redeem of pay-to-script-hash (P2SH) script.
    static Script buildPayToScriptHashWitness(const Data& redeemScript, const std::vector<Data>& signatures);

    // JuBiter-defined
    /// Builds a return0 script from a script.
    static Script buildReturn0(const Data& data, const Data& check={});

    /// Builds a pay-to-public-key-hash (P2PKH) script appropriate for the given
    /// address.
    static Script buildForAddress(const std::string& address, enum TWCoinType coin);

    // JuBiter-defined
    static bool parseWitnessStackToPayToWitnessScriptHash(const std::vector<Data>& scriptWitness, Data& signature, Data& publicKey);

    /// Encodes the script.
    void encode(Data& data) const;

    // JuBiter-defined
    /// Encodes the zero script.
    void encodeZero(Data& data) const;

    // JuBiter-defined
    /// Decodes the script.
    bool decode(const Data& data);

    // JuBiter-defined
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

    // JuBiter-defined
    Script& operator+(const Script& rhs) {
        Data rds;
        rhs.encode(rds);

        this->bytes.insert(this->bytes.end(), rds.begin(), rds.end());
        return *this;
    }

    // JuBiter-defined
    Script& operator+=(const Script& rhs) {
        return (*this + rhs);
    }

  private:
    /// The number of bytes that represent the number of script bytes(decodeVarInt())
    size_t _varIntSize = 0;
    
    /// Extracts a single opcode at the given index including its operand.
    ///
    /// \param index [in/out] index where the operation starts, on return the
    /// index of the next operation. \param opcode [out] the opcode. \param
    /// operand [out] the opcode's operand. \returns whether an opcode was
    /// available.
    bool getScriptOp(size_t& index, uint8_t& opcode, Data& operand) const;
};

inline bool operator==(const Script& lhs, const Script& rhs) {
    return lhs.bytes == rhs.bytes;
}
inline bool operator!=(const Script& lhs, const Script& rhs) {
    return !(lhs == rhs);
}

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinScript {
    TW::Bitcoin::Script impl;
};
