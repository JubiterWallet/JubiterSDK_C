// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "Address.h"
#include "Data.h"
#include "EIP712.h"
#include "HexCoding.h"
#include "RLP.h"
#include <string>
#include <vector>

namespace TW::Ethereum {
static const Data EmptyListEncoded = parse_hex("c0");

std::vector<Access> accessListFromJson(const std::string &json) {
    if (json.empty()) {
        return {};
    }

    std::vector<Access> vAccessList;

    nlohmann::json jsonAccessList = nlohmann::json::parse(json);
    if (nlohmann::detail::value_t::array != jsonAccessList.type()) {
        return {};
    }

    // https://eips.ethereum.org/EIPS/eip-2930
    for (const auto &item : jsonAccessList) {
        if (!item.is_array() || item.size() != 2) {
            return {};
        }
        auto itm0 = item[0];
        auto itm1 = item[1];
        if (!itm0.is_string() || !itm1.is_array()) {
            return {};
        }

        auto address = Address{itm0.get<std::string>()};
        auto storageKeys = std::vector<TW::Data>();
        for (const auto &key : itm1) {
            const auto data = parse_hex(key.get<std::string>());
            storageKeys.push_back(data);
        }
        vAccessList.push_back({address, storageKeys});
    }

    return vAccessList;
}

// JuBiter-defined
Data Signature::serialize() const {
    Data encoded;
    if (1 == v.size() && 0x80 == v[0]) {
        append(encoded, 0x80);
    } else {
        append(encoded, RLP::encode(v));
    }
    append(encoded, RLP::encode(r));
    append(encoded, RLP::encode(s));
    return encoded;
}

// JuBiter-modified
Data TransactionBase::preHash(const Data &chainID) const {
    auto preimage = serialize(chainID);
    return (0 == preimage.size() ? Data() : Hash::keccak256(preimage));
}

// JuBiter-defined
Data TransactionBase::encoded(const Signature &signature) const {
    if (signature.empty()) {
        return {};
    }

    Data vSignature;
    std::copy(std::begin(signature.r), std::end(signature.r), std::back_inserter(vSignature));
    std::copy(std::begin(signature.s), std::end(signature.s), std::back_inserter(vSignature));
    std::copy(std::begin(signature.v), std::end(signature.v), std::back_inserter(vSignature));
    return vSignature;
}

// JuBiter-modified
Data TransactionBase::encoded(const Signature &signature, const Data &chainID) const { return encoded(signature); }

// JuBiter-defined
bool TransactionBase::decoded(const Data &encoded, Signature &signature) {
    if (0 == encoded.size()) {
        return false;
    }

    std::copy(std::begin(encoded), std::begin(encoded) + 32, std::back_inserter(signature.r));
    std::copy(std::begin(encoded) + 32, std::begin(encoded) + 64, std::back_inserter(signature.s));
    std::copy(std::begin(encoded) + 64, std::begin(encoded) + 65, std::back_inserter(signature.v));
    return true;
}

Data TransactionNonTyped::serialize(const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(Data{0}));
    append(encoded, RLP::encode(Data{0}));
    return RLP::encodeList(encoded);
}

Data TransactionNonTyped::encoded(const Signature &signature, const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    append(encoded, RLP::encode(signature.v));
    append(encoded, RLP::encode(signature.r));
    append(encoded, RLP::encode(signature.s));
    return RLP::encodeList(encoded);
}

// JuBiter-defined
bool TransactionNonTyped::decoded(const Data &encoded, Signature &signature) {
    auto decodedItems = RLP::decode(encoded);
    if (0 > decodedItems.remainder.size()) {
        return false;
    }

    for (size_t i = 0; i < decodedItems.decoded.size(); ++i) {
        switch (i) {
        case 0: {
            nonce = decodedItems.decoded[i];
        } break;
        case 1: {
            gasPrice = decodedItems.decoded[i];
        } break;
        case 2: {
            gasLimit = decodedItems.decoded[i];
        } break;
        case 3: {
            to = Address(decodedItems.decoded[i]);
        } break;
        case 4: {
            amount = decodedItems.decoded[i];
        } break;
        case 5: {
            payload = decodedItems.decoded[i];
        } break;
        case 6: {
            signature.v = decodedItems.decoded[i];
        } break;
        case 7: {
            signature.r = decodedItems.decoded[i];
        } break;
        case 8: {
            signature.s = decodedItems.decoded[i];
        } break;
        default: {
        } break;
        }
    }

    return (isValid() && signature.isValid());
}

// JuBiter-defined
Data TransactionPersonal::preHash() const { return TransactionBase::preHash({}); }

// JuBiter-defined
Data TransactionPersonal::serialize(const Data &chainID) const { return serialize(); }

// JuBiter-defined
Data TransactionPersonal::serialize() const {
    auto encoded = Data();

    // encode(b : 𝔹⁸ⁿ) = "\x19Ethereum Signed Message:\n" ‖ len(b) ‖ b where len(b) is the ascii-decimal encoding of the
    // number of bytes in b.
    std::string pr = "Ethereum Signed Message:\n";

    encoded.push_back(0x19);
    std::copy(pr.begin(), pr.end(), std::back_inserter(encoded));
    auto sz = std::to_string(payload.size());
    std::copy(sz.begin(), sz.end(), std::back_inserter(encoded));
    std::copy(payload.begin(), payload.end(), std::back_inserter(encoded));

    return encoded;
}

// JuBiter-defined
bool TransactionPersonal::decoded(const Data &encoded, Signature &signature) { return false; }

// JuBiter-defined
Data TransactionTypedData::preHash() const { return TransactionBase::preHash({}); }

// JuBiter-defined
Data TransactionTypedData::serialize(const Data &chainID) const { return serialize(); }

// JuBiter-defined
Data TransactionTypedData::serialize() const {
    auto encoded = Data();
    encoded.push_back(0x19);
    encoded.push_back(0x01);

    std::string typedDataInJSON(payload.begin(), payload.end());
    nlohmann::json typedData = nlohmann::json::parse(typedDataInJSON);
    if (nlohmann::detail::value_t::object != typedData.type()) {
        return {};
    }

    if (!jub::eth::EIP712::parseJSON(typedData)) {
        jub::eth::EIP712::clearJSON();
        return {};
    }

    Data domainSeparator = jub::eth::EIP712::typed_data_envelope(
                            jub::eth::EIP712::EIP712Domain(),
                    typedData[jub::eth::EIP712::domainEnter()],
                    bMetamaskV4Compat);
    if (domainSeparator.empty()) {
        jub::eth::EIP712::clearJSON();
        return {};
    }

    Data hashStructMessage = jub::eth::EIP712::typed_data_envelope(
                    typedData[jub::eth::EIP712::primaryTypeEnter()].get<std::string>().c_str(),
                    typedData[jub::eth::EIP712::messageEnter()],
                    bMetamaskV4Compat);
    jub::eth::EIP712::clearJSON();
    if (hashStructMessage.empty()) {
        return {};
    }

    std::copy(domainSeparator.begin(), domainSeparator.end(), std::back_inserter(encoded));
    std::copy(hashStructMessage.begin(), hashStructMessage.end(), std::back_inserter(encoded));

    return encoded;
}

// JuBiter-defined
bool TransactionTypedData::decoded(const Data &encoded, Signature &signature) { return false; }

// JuBiter-defined
Data TransactionTyped::encodeAccessList() const {
    auto encoded = Data();
    for (const auto &access : accessList) {
        auto item = Data();
        append(item, RLP::encode(access.address.bytes));
        append(item, RLP::encodeList(access.storageKeys));
        append(encoded, RLP::encodeList(item));
    }

    return RLP::encodeList(encoded);
}

// JuBiter-defined
bool TransactionTyped::decodeAccessList(const Data &chainID, const Signature &signature, const Data &encoded) {
    // find the index of the begin of access list
    size_t offsetBegin = 0;
    auto part = serializePart(chainID);
    if (0 >= part.size()) {
        return false;
    }
    auto itInEncoded = std::search(encoded.begin(), encoded.end(), part.begin(), part.end());
    if (itInEncoded != encoded.end()) {
        offsetBegin = (itInEncoded - encoded.begin()) + part.size();
    }
    // find the index of the end of access list
    size_t offsetEnd = 0;
    auto signatureRaw = signature.serialize();
    itInEncoded = std::search(encoded.begin() + offsetBegin, encoded.end(), signatureRaw.begin(), signatureRaw.end());
    if (itInEncoded != signatureRaw.end()) {
        offsetEnd = itInEncoded - encoded.begin();
    }

    auto encodedAccessList = Data(encoded.begin() + offsetBegin, encoded.begin() + offsetEnd);
    auto accessListItems = RLP::decode(encodedAccessList);
    Data remainder;
    for (size_t i = 0; i < accessListItems.decoded.size(); ++i) {
        Access access;
        access.address = Address(accessListItems.decoded[i]);
        if (0 == remainder.size()) {
            remainder = encodedAccessList;
        }
        auto itInAccessList =
            std::search(remainder.begin(), remainder.end(), access.address.bytes.begin(), access.address.bytes.end());
        if (itInAccessList != remainder.end()) {
            offsetBegin = (itInAccessList - remainder.begin()) + access.address.bytes.size();
        }
        auto storageKeys = Data(remainder.begin() + offsetBegin, remainder.end());
        auto storageKeysItems = RLP::decode(storageKeys);
        for (size_t j = 0; j < storageKeysItems.decoded.size(); ++j) {
            access.storageKeys.push_back(storageKeysItems.decoded[j]);
        }
        accessList.push_back(access);

        // update remainder
        remainder.clear();
        remainder = storageKeysItems.remainder;
    }

    return true;
}

// JuBiter-defined
Data TransactionOptionalAccessList::serializePart(const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    return encoded;
}

// JuBiter-defined
Data TransactionOptionalAccessList::serialize(const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    if (0 == accessList.size()) {
        append(encoded, EmptyListEncoded); // empty accessList
    } else {
        append(encoded, encodeAccessList());
    }
    // append(encoded, RLP::encode(Data{0}));
    // append(encoded, RLP::encode(Data{0}));
    // append(encoded, RLP::encode(Data{0}));

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

// JuBiter-defined
Data TransactionOptionalAccessList::encoded(const Signature &signature, const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    if (0 == accessList.size()) {
        append(encoded, EmptyListEncoded); // empty accessList
    } else {
        append(encoded, encodeAccessList());
    }
    append(encoded, RLP::encode(signature.v));
    append(encoded, RLP::encode(signature.r));
    append(encoded, RLP::encode(signature.s));

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

// JuBiter-defined
bool TransactionOptionalAccessList::decoded(const Data &encoded, Signature &signature) {
    // parse transaction type
    auto decodedMainItems = RLP::decode(encoded);
    if (1 != decodedMainItems.decoded.size() || 0 > decodedMainItems.remainder.size()) {
        return false;
    }
    if (1 != decodedMainItems.decoded[0].size() ||
        TransactionType::TxType_OptionalAccessList != decodedMainItems.decoded[0][0]) {
        return false;
    }
    type = (TransactionType)decodedMainItems.decoded[0][0];

    auto decodedItems = RLP::decode(decodedMainItems.remainder);
    if (0 > decodedItems.remainder.size()) {
        return false;
    }
    size_t accessIndex = ITEM_ACCESS_LIST_INDEX - 1;
    if (ITEM_COUNT == decodedItems.decoded.size()) {
        accessIndex = ITEM_ACCESS_LIST_INDEX;
    }

    // parse tx
    Data chainId;
    for (size_t i = 0; i < decodedItems.decoded.size(); ++i) {
        switch (i) {
        case 0: {
            chainId = decodedItems.decoded[i];
        } break;
        case 1: {
            nonce = decodedItems.decoded[i];
        } break;
        case 2: {
            gasPrice = decodedItems.decoded[i];
        } break;
        case 3: {
            gasLimit = decodedItems.decoded[i];
        } break;
        case 4: {
            to = Address(decodedItems.decoded[i]);
        } break;
        case 5: {
            amount = decodedItems.decoded[i];
        } break;
        case 6: {
            payload = decodedItems.decoded[i];
        } break;
        default: {
        } break;
        }
    }

    // parse signature
    if (decodedItems.decoded.size() != (accessIndex + 1 + 3)) {
        return false;
    }
    // Because usesReplayProtection() is not used for Eip1559, the v value might be 0x80.
    // When v is 0x80, this v item is null in RLP code.
    // Here we deal with v value 0x80.
    auto v = decodedItems.decoded[accessIndex + 1];
    signature.v = (0 == v.size() ? parse_hex("80") : v);
    signature.r = decodedItems.decoded[accessIndex + 2];
    signature.s = decodedItems.decoded[accessIndex + 3];

    // decode access list
    bool b = true;
    if (ITEM_ACCESS_LIST_INDEX == accessIndex) {
        b = decodeAccessList(chainId, signature, encoded);
    }
    if (!b) {
        return b;
    }

    return (isValid() && signature.isValid());
}

// JuBiter-defined
Data TransactionEip1559::serializePart(const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(maxInclusionFeePerGas));
    append(encoded, RLP::encode(maxFeePerGas));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    return encoded;
}

Data TransactionEip1559::serialize(const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(maxInclusionFeePerGas));
    append(encoded, RLP::encode(maxFeePerGas));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    // JuBiter-modified
    if (0 == accessList.size()) {
        append(encoded, EmptyListEncoded); // empty accessList
    } else {
        append(encoded, encodeAccessList());
    }

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

Data TransactionEip1559::encoded(const Signature &signature, const Data &chainID) const {
    Data encoded;
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(maxInclusionFeePerGas));
    append(encoded, RLP::encode(maxFeePerGas));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    // JuBiter-modified
    if (0 == accessList.size()) {
        append(encoded, EmptyListEncoded); // empty accessList
    } else {
        append(encoded, encodeAccessList());
    }
    append(encoded, RLP::encode(signature.v));
    append(encoded, RLP::encode(signature.r));
    append(encoded, RLP::encode(signature.s));

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

// JuBiter-defined
bool TransactionEip1559::decoded(const Data &encoded, Signature &signature) {
    // parse transaction type
    auto decodedMainItems = RLP::decode(encoded);
    if (1 != decodedMainItems.decoded.size() || 0 > decodedMainItems.remainder.size()) {
        return false;
    }
    if (1 != decodedMainItems.decoded[0].size() || TransactionType::TxType_Eip1559 != decodedMainItems.decoded[0][0]) {
        return false;
    }
    type = (TransactionType)decodedMainItems.decoded[0][0];

    auto decodedItems = RLP::decode(decodedMainItems.remainder);
    if (0 > decodedItems.remainder.size()) {
        return false;
    }

    size_t accessIndex = ITEM_ACCESS_LIST_INDEX - 1;
    if (ITEM_COUNT == decodedItems.decoded.size()) {
        accessIndex = ITEM_ACCESS_LIST_INDEX;
    }

    // parse tx
    Data chainId;
    for (size_t i = 0; i < decodedItems.decoded.size(); ++i) {
        switch (i) {
        case 0: {
            chainId = decodedItems.decoded[i];
        } break;
        case 1: {
            nonce = decodedItems.decoded[i];
        } break;
        case 2: {
            maxInclusionFeePerGas = decodedItems.decoded[i];
        } break;
        case 3: {
            maxFeePerGas = decodedItems.decoded[i];
        } break;
        case 4: {
            gasLimit = decodedItems.decoded[i];
        } break;
        case 5: {
            to = Address(decodedItems.decoded[i]);
        } break;
        case 6: {
            amount = decodedItems.decoded[i];
        } break;
        case 7: {
            payload = decodedItems.decoded[i];
        } break;
        default: {
        } break;
        }
    }

    // parse signature
    if (decodedItems.decoded.size() != (accessIndex + 1 + 3)) {
        return false;
    }
    // Because usesReplayProtection() is not used for Eip1559, the v value might be 0x80.
    // When v is 0x80, this v item is null in RLP code.
    // Here we deal with v value 0x80.
    auto v = decodedItems.decoded[accessIndex + 1];
    signature.v = (0 == v.size() ? parse_hex("80") : v);
    signature.r = decodedItems.decoded[accessIndex + 2];
    signature.s = decodedItems.decoded[accessIndex + 3];

    // decode access list
    bool b = true;
    if (ITEM_ACCESS_LIST_INDEX == accessIndex) {
        b = decodeAccessList(chainId, signature, encoded);
    }
    if (!b) {
        return b;
    }

    return (isValid() && signature.isValid());
}

} // namespace TW::Ethereum
