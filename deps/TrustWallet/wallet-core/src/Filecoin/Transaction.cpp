// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include <algorithm>
#include "../Base32.h"
#include "../Base64.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace TW;
using namespace TW::Filecoin;

// encodeVaruint encodes a 256-bit number into a big endian encoding, omitting leading zeros.
static Data encodeVaruint(const uint256_t& value) {
    Data data;
    encode256BE(data, value, 256);
    size_t i = 0;
    for (i = 0; i < data.size(); ++i) {
        if (data[i] != 0) {
            break;
        }
    }
    Data small;
    small.assign(data.begin() + i - 1, data.end());
    return small;
}

// cidPrefix is the CID + Multihash prefix of transaction CIDs.
const Data cidPrefix = {
    // CIDv1 with CBOR codec
    0x01,
    0x71,
    // Blake2b-256 with 32 byte output
    0xa0,
    0xe4,
    0x02,
    0x20,
};

// JuBiter-defined
Data Transaction::getCidPrefix() {
    return cidPrefix;
}

//// JuBiter-defined
//Data Transaction::getNonce() const {
//    Data vNonce = Cbor::Encode::uint(nonce).encoded();
//    return Data(vNonce.begin()+1, vNonce.end());
//}
//
//// JuBiter-defined
//Data Transaction::getValue() const {
//    Data vValue = Cbor::Encode::bytes(encodeVaruint(value)).encoded();
//    return Data(vValue.begin()+1, vValue.end());
//}
//
//// JuBiter-defined
//Data Transaction::getGasFeeCap() const {
//    Data vGasFeeCap = Cbor::Encode::bytes(encodeVaruint(gasFeeCap)).encoded();
//    return Data(vGasFeeCap.begin()+1, vGasFeeCap.end());
//}
//
//// JuBiter-defined
//Data Transaction::getGasPremium() const {
//    Data vGasPremium = Cbor::Encode::bytes(encodeVaruint(gasPremium)).encoded();
//    return Data(vGasPremium.begin()+1, vGasPremium.end());
//}
//
//// JuBiter-defined
//Data Transaction::getGasLimit() const {
//    Cbor::Encode cborGasLimit = gasLimit >= 0 ? Cbor::Encode::uint((uint64_t)gasLimit)
//                                              : Cbor::Encode::negInt((uint64_t)(-gasLimit - 1));
//    Data vGasLimit = cborGasLimit.encoded();
//    return Data(vGasLimit.begin()+1, vGasLimit.end());
//}
//
Cbor::Encode Transaction::message() const {
    Cbor::Encode cborGasLimit = gasLimit >= 0 ? Cbor::Encode::uint((uint64_t)gasLimit)
                                              : Cbor::Encode::negInt((uint64_t)(-gasLimit - 1));
    return Cbor::Encode::array({
        Cbor::Encode::uint(version),                  // version
        Cbor::Encode::bytes(to.bytes),                // to address
        Cbor::Encode::bytes(from.bytes),              // from address
        Cbor::Encode::uint(nonce),                    // nonce
        Cbor::Encode::bytes(encodeVaruint(value)),    // value
        cborGasLimit,                                 // gas limit
        Cbor::Encode::bytes(encodeVaruint(gasFeeCap)), // gas fee cap
        Cbor::Encode::bytes(encodeVaruint(gasPremium)),// gas premium
        Cbor::Encode::uint(0),                        // abi.MethodNum (0 => send)
        Cbor::Encode::bytes(Data())                   // data (empty)
    });
}

Data Transaction::cid() const {
    Data cid;
    cid.reserve(cidPrefix.size() + 32);
    cid.insert(cid.end(), cidPrefix.begin(), cidPrefix.end());

    Data hash = Hash::blake2b(message().encoded(), 32);
    cid.insert(cid.end(), hash.begin(), hash.end());
    return cid;
}

// JuBiter-defined
std::string str_tolower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); }
    );
    return s;
}

// JuBiter-defined
std::string Transaction::cid(const Data& cid) {
    std::string multibase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    std::string upper = TW::Base32::encode(cid, multibase.c_str());
    std::string lower = str_tolower(upper);

    return ("b"+lower);
}

// JuBiter-defined
std::string Transaction::cid(const Transaction& tx) {
    return Transaction::cid(tx.cid());
}

std::string Transaction::serialize(Data& signature) const {
    json tx = {
        {"Message", json{
                {"To", to.string()},
                {"From", from.string()},
                {"Nonce", nonce},
                {"Value", toString(value)},
                {"GasPremium", toString(gasPremium)},
                {"GasFeeCap", toString(gasFeeCap)},
                {"GasLimit", gasLimit},
                {"CID", json{
                        {"/", Transaction::cid(cid())},
                    }
                }
            }
        },
        {"Signature", json{
                {"Type", 1},    // Prepend IKTSecp256k1 type
                {"Data", Base64::encode(signature)},
            }
        },
        {"CID", json{
                {"/", Transaction::cid(cid())},
            }
        },
    };
    return tx.dump();
}
