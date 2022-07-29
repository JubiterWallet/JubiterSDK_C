// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PrivateKey.h"
#include "Transaction.h"
#include "Tron.pb.h"

namespace TW::Tron {

/// Helper class that performs Tron transaction signing.
class Signer {
  public:
//    Signer() = delete;

    /// Signs the given transaction.
//    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    bool sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;
    // JuBiter-defined
    /// Verifies the given signature.
    bool verify(const PublicKey& publicKey, Transaction& transaction) const noexcept;

    // JuBiter-defined
    /// Computes the transaction hash.
    Data hash(const Transaction& transaction) const noexcept;

    // JuBiter-defined
    /// Signs the given bytestring.
    bool sign(const PrivateKey& privateKey, Bytestring& bytestring) const noexcept;
    // JuBiter-defined
    /// Verifies the given signature.
    bool verify(const PublicKey& publicKey, Bytestring& bytestring) const noexcept;

    // JuBiter-defined
    /// Computes the transaction hash.
    Data hash(const Bytestring& bytestring) const noexcept;
}; // class Signer end

} // namespace TW::Tron end
