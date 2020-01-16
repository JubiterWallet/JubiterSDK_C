// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace jub {
namespace eth {

/// Checksum types for Ethereum-based blockchains.
enum ChecksumType {
    eip55 = 0,
    wanchain = 1,
}; // enum ChecksumType end

std::string checksumed(const std::string& address, enum ChecksumType type);


} // namespace eth end
} // namespace jub end
