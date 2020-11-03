// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "RLP.h"
#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
//#include "../uint256.h"
//#include "../proto/Ethereum.pb.h"

//#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <tuple>
#include <vector>

namespace TW::Ethereum {

/// Helper class that performs Ethereum transaction signing.
class Signer {
  public:
//    uint256_t chainID;
    Data chainID;

    // JuBiter-modified
    /// Initializes a signer with a chain identifier.
//    explicit Signer(uint256_t chainID) : chainID(std::move(chainID)) {}
    explicit Signer(Data chainID) : chainID(std::move(chainID)) {}

//    /// Signs a Proto::SigningInput transaction
//    Proto::SigningOutput sign(const Proto::SigningInput& input) const noexcept;
//
    /// Signs the given transaction.
    void sign(const PrivateKey &privateKey, Transaction &transaction) const noexcept;

    // JuBiter-defined
    bool verify(const PublicKey& publicKey, Transaction& transaction) const noexcept;
    // JuBiter-defined
    bool verify(const Data chainID, const PublicKey& publicKey, Transaction& transaction) const noexcept;

    // JuBiter-defined
    void sign(const PrivateKey &privateKey, const Data& bytestring, Data& signature) const noexcept;
    // JuBiter-defined
    bool verify(const Data chainID, const PublicKey& publicKey, const Data& bytestring, const Data& signature) const noexcept;

  public:
    /// Signs a hash with the given private key for the given chain identifier.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static std::tuple<Data, Data, Data>
    sign(const Data& chainID, const PrivateKey& privateKey, const Data& hash) noexcept;

    // JuBiter-modified
    /// R, S, and V values for the given chain identifier and signature.
    ///
    /// @returns the r, s, and v values of the transaction signature
//    static std::tuple<uint256_t, uint256_t, uint256_t> values(const uint256_t& chainID,
//                                                              const Data& signature) noexcept;
     static std::tuple<Data, Data, Data> values(const Data& chainID,
                                                const Data& signature) noexcept;

//  protected:
    /// Computes the transaction hash.
    Data hash(const Transaction &transaction) const noexcept;

    // JuBiter-defined
    /// Computes the bytestring hash.
    Data hash(const Data &bytestring) const noexcept;
};

} // namespace TW::Ethereum

/// Wrapper for C interface.
struct TWEthereumSigner {
    TW::Ethereum::Signer impl;
};
