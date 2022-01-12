// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PrivateKey.h"
#include "Extrinsic.h"

namespace TW::Polkadot {

/// Helper class that performs Polkadot transaction signing.
class Signer {
public:
    /// Hide default constructor
//    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    // JuBiter-modified
//    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    Data sign(const Data& private_key, const Extrinsic& extrinsic) noexcept;
    // JuBiter-defined
    /// Verifies the given signature.
    bool verify(const Data& public_key, const Extrinsic& extrinsic, const Data& signature) noexcept;
};

} // namespace TW::Polkadot
