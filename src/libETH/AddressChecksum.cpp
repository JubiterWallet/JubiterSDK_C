// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <libETH/AddressChecksum.h>
#include <cctype>
#include <utility/util.h>
#include <TrezorCrypto/sha3.h>

namespace jub {
namespace eth {
std::string checksumed(const std::string& address, enum ChecksumType type) {
    
    JUB_BYTE digest[32] = {0};
    keccak_256(reinterpret_cast<const JUB_BYTE*>(address.data()),address.size(),digest);
    const auto hash = uchar_vector(digest,digest+32).getHex();
    //const auto hash = hex(Hash::keccak256(addressString));

    std::string string = "0x";
    for (auto i = 0; i < std::min(address.size(), hash.size()); i += 1) {
        const auto a = address[i];
        const auto h = hash[i];
        if (a >= '0' && a <= 9) {
            string.push_back(a);
        } else if ((h >= '8' && h <= '9') || (h >= 'a' && h <= 'f')) {
            switch (type) {
            case eip55:
                string.push_back(static_cast<char>(toupper(a)));
                break;
            case wanchain:
                string.push_back(static_cast<char>(tolower(a)));
                break;
            }
        } else {
            switch (type) {
            case eip55:
                string.push_back(static_cast<char>(tolower(a)));
                break;
            case wanchain:
                string.push_back(static_cast<char>(toupper(a)));
                break;
            }
        }
    }

    return string;
}
}
}
