// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PrivateKey.h"
#include "PublicKey.h"
#include "Transaction.h"
#include <string>
#include <vector>

//#include "../proto/Solana.pb.h"

namespace TW::Solana {

/// Helper class that performs Solana transaction signing.
class Signer {
  public:
    /// Signs the given transaction.
    static void sign(const std::vector<PrivateKey> &privateKeys, Transaction &transaction);

    static bool verify(const std::vector<PublicKey> &pubKeys, const Transaction &transaction);

    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string &json, const Data &key);

    static void signUpdateBlockhash(const std::vector<PrivateKey> &privateKeys, Transaction &transaction,
                                    Solana::Hash &recentBlockhash);
    static Data signRawMessage(const std::vector<PrivateKey> &privateKeys, const Data messageData);
};

} // namespace TW::Solana
