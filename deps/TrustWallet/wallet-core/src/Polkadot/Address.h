// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"
#include  "../SS58Address.h"
#include  <TrustWalletCore/TWSS58AddressType.h>

#include <string>

namespace TW::Polkadot {

class Address: public SS58Address {
  public:
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string, const bool bTest=false) { return SS58Address::isValid(string, (false == bTest) ? TWSS58AddressTypePolkadot : TWSS58AddressTypeWestend); }

    /// Initializes a Polkadot address with a string representation.
    Address(const std::string& string, const bool bTest=false): SS58Address(string, (false == bTest) ? TWSS58AddressTypePolkadot : TWSS58AddressTypeWestend) {}

    /// Initializes a Polkadot address with a public key.
    Address(const PublicKey& publicKey, const bool bTest=false): SS58Address(publicKey, (false == bTest) ? TWSS58AddressTypePolkadot : TWSS58AddressTypeWestend) {}
};
} // namespace TW::Polkadot

