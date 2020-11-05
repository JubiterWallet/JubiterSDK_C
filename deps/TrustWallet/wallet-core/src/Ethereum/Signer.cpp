// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

using namespace TW;
using namespace TW::Ethereum;

//std::tuple<uint256_t, uint256_t, uint256_t> Signer::values(const uint256_t& chainID,
//                                                           const Data& signature) noexcept {
//    boost::multiprecision::uint256_t r, s, v;
//    import_bits(r, signature.begin(), signature.begin() + 32);
//    import_bits(s, signature.begin() + 32, signature.begin() + 64);
//    import_bits(v, signature.begin() + 64, signature.begin() + 65);
//    v += 27;
//
//    boost::multiprecision::uint256_t newV;
//    if (chainID != 0) {
//        import_bits(newV, signature.begin() + 64, signature.begin() + 65);
//        newV += 35 + chainID + chainID;
//    } else {
//        newV = v;
//    }
//    return std::make_tuple(r, s, newV);
//}
// JuBiter-modified
std::tuple<Data, Data, Data> Signer::values(const Data& chainID,
const Data& signature) noexcept {
    Data r, s;
    uint8_t v;
    std::copy(std::begin(signature), std::begin(signature) + 32, std::back_inserter(r));
    std::copy(std::begin(signature) + 32, std::begin(signature) + 64, std::back_inserter(s));
    v = signature[64];

    Data newV;
    if (0 != chainID.size()) {
        v += (35 + chainID[0] + chainID[0]);
    }
    else {
        v += 27;
    }
    newV.push_back(v);

    return std::make_tuple(r, s, newV);
}

std::tuple<Data, Data, Data>
Signer::sign(const Data& chainID, const PrivateKey& privateKey, const Data& hash) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return values(chainID, signature);
}

//Proto::SigningOutput Signer::sign(const TW::Ethereum::Proto::SigningInput &input) const noexcept {
//    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
//
//    auto transaction = Transaction(
//            /* nonce: */ load(input.nonce()),
//            /* gasPrice: */ load(input.gas_price()),
//            /* gasLimit: */ load(input.gas_limit()),
//            /* to: */ Address(input.to_address()),
//            /* amount: */ load(input.amount()),
//            /* payload: */ Data(input.payload().begin(), input.payload().end())
//    );
//
//    sign(key, transaction);
//
//    auto protoOutput = Proto::SigningOutput();
//
//    auto encoded = RLP::encode(transaction);
//    protoOutput.set_encoded(encoded.data(), encoded.size());
//
//    auto v = store(transaction.v);
//    protoOutput.set_v(v.data(), v.size());
//
//    auto r = store(transaction.r);
//    protoOutput.set_r(r.data(), r.size());
//
//    auto s = store(transaction.s);
//    protoOutput.set_s(s.data(), s.size());
//
//    return protoOutput;
//}
//
void Signer::sign(const PrivateKey &privateKey, Transaction &transaction) const noexcept {
    auto hash = this->hash(transaction);
    auto tuple = Signer::sign(chainID, privateKey, hash);

    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

// JuBiter-defined
bool Signer::verify(const PublicKey& publicKey, Transaction& transaction) const noexcept {
    Data signature;
    std::copy(transaction.r.begin(), transaction.r.end(), std::back_inserter(signature));
    std::copy(transaction.s.begin(), transaction.s.end(), std::back_inserter(signature));
    std::copy(transaction.v.begin(), transaction.v.end(), std::back_inserter(signature));
    return publicKey.verify(signature, this->hash(transaction));
}

// JuBiter-defined
bool Signer::verify(const Data chainID, const PublicKey& publicKey, Transaction& transaction) const noexcept {
    Data signature;
    std::copy(transaction.r.begin(), transaction.r.end(), std::back_inserter(signature));
    std::copy(transaction.s.begin(), transaction.s.end(), std::back_inserter(signature));
    std::copy(transaction.v.begin(), transaction.v.end(), std::back_inserter(signature));

    int v = signature[signature.size()-1];
    if (0 != chainID.size()) {
        v -= (35 + chainID[0] + chainID[0]);
    }
    else {
        v += 27;
    }

    return publicKey.verify(signature, this->hash(transaction), v);
}

Data Signer::hash(const Transaction &transaction) const noexcept {
    auto encoded = Data();
    append(encoded, RLP::encode(transaction.nonce));
    append(encoded, RLP::encode(transaction.gasPrice));
    append(encoded, RLP::encode(transaction.gasLimit));
    append(encoded, RLP::encode(transaction.to.bytes));
    append(encoded, RLP::encode(transaction.amount));
    append(encoded, RLP::encode(transaction.payload));
    append(encoded, RLP::encode(chainID));
    // JuBiter-modified
    append(encoded, RLP::encode(Data{0}));
    append(encoded, RLP::encode(Data{0}));
    return Hash::keccak256(RLP::encodeList(encoded));
}

// JuBiter-defined
void Signer::sign(const PrivateKey &privateKey, const Data& bytestring, Data& signature) const noexcept {
    auto hash = this->hash(bytestring);
    auto tuple = Signer::sign(chainID, privateKey, hash);

    signature.clear();
    // r
    std::copy(std::get<0>(tuple).begin(), std::get<0>(tuple).end(), std::back_inserter(signature));
    // s
    std::copy(std::get<1>(tuple).begin(), std::get<1>(tuple).end(), std::back_inserter(signature));
    // v
    std::copy(std::get<2>(tuple).begin(), std::get<2>(tuple).end(), std::back_inserter(signature));
}

// JuBiter-defined
bool Signer::verify(const Data chainID, const PublicKey& publicKey, const Data& bytestring, const Data& signature) const noexcept {

    int v = signature[signature.size()-1];
    if (0 != chainID.size()) {
        v -= (35 + chainID[0] + chainID[0]);
    }
    else {
        v += 27;
    }

    return publicKey.verify(signature, this->hash(bytestring), v);
}

// JuBiter-defined
/// Computes the bytestring hash.
Data Signer::hash(const Data &bytestring) const noexcept {
    auto encoded = Data();

    // encode(b : 𝔹⁸ⁿ) = "\x19Ethereum Signed Message:\n" ‖ len(b) ‖ b where len(b) is the ascii-decimal encoding of the number of bytes in b.
    std::string pr = "Ethereum Signed Message:\n";

    encoded.push_back(0x19);
    std::copy(pr.begin(), pr.end(), std::back_inserter(encoded));
    auto sz = std::to_string(bytestring.size());
    std::copy(sz.begin(), sz.end(), std::back_inserter(encoded));
    std::copy(bytestring.begin(), bytestring.end(), std::back_inserter(encoded));

    return Hash::keccak256(encoded);
}
