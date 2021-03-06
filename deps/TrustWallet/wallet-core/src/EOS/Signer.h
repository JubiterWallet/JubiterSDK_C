// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Prefixes.h"
#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

#include <stdexcept>

namespace TW::EOS {

/// Helper class that performs EOS transaction signing.
class Signer {
public:
    const Data chainID;

    /// Initializes a signer with a chain identifier.
    Signer(const Data& chainID) : chainID(chainID) {}

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Type type, Transaction& transaction) const;
    // JuBiter-defined
    /// Verifies the given signature.
    bool verify(const PublicKey& publicKey, Type type, const Transaction& transaction) const noexcept;
    // JuBiter-defined
    /// Recovers the given signature.
    bool recover(PublicKey& publicKey, Type type, Transaction& transaction) noexcept;

    /// Computes the transaction hash.
    Data hash(const Transaction& transaction) const noexcept;

    static int isCanonical(uint8_t by, uint8_t sig[64]);
};

} // namespace TW::EOS
