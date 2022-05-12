// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "RLP.h"
#include "HexCoding.h"
#include "EIP712.h"

using namespace TW::Ethereum;
using namespace TW;

static const Data EmptyListEncoded = parse_hex("c0");

// JuBiter-defined
Data Signature::serialize() const {
    Data encoded;
    if (1 == v.size() && 0x80 == v[0]) {
        append(encoded, 0x80);
    }
    else {
        append(encoded, RLP::encode(v));
    }
    append(encoded, RLP::encode(r));
    append(encoded, RLP::encode(s));
    return encoded;
}

// JuBiter-modified
Data TransactionBase::preHash(const Data& chainID) const {
    auto preimage = serialize(chainID);
    return (0 == preimage.size() ? Data() : Hash::keccak256(preimage));
}

// JuBiter-defined
Data TransactionBase::encoded(const Signature& signature) const {
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
Data TransactionBase::encoded(const Signature& signature, const Data& chainID) const {
    return encoded(signature);
}

// JuBiter-defined
bool TransactionBase::decoded(const Data& encoded, Signature& signature) {
    if (0 == encoded.size()) {
        return false;
    }

    std::copy(std::begin(encoded),      std::begin(encoded) + 32, std::back_inserter(signature.r));
    std::copy(std::begin(encoded) + 32, std::begin(encoded) + 64, std::back_inserter(signature.s));
    std::copy(std::begin(encoded) + 64, std::begin(encoded) + 65, std::back_inserter(signature.v));
    return true;
}

Data TransactionNonTyped::serialize(const Data& chainID) const {
    Data encoded;
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    append(encoded, RLP::encode(payload));
    append(encoded, RLP::encode(chainID));
    // JuBiter-modified
//    append(encoded, RLP::encode(0));
//    append(encoded, RLP::encode(0));
    append(encoded, RLP::encode(Data{0}));
    append(encoded, RLP::encode(Data{0}));
    return RLP::encodeList(encoded);
}

Data TransactionNonTyped::encoded(const Signature& signature, const Data& chainID) const {
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
bool TransactionNonTyped::decoded(const Data& encoded, Signature& signature) {
    auto decodedItems = RLP::decode(encoded);
    if (0 > decodedItems.remainder.size()) {
        return false;
    }

    for (size_t i=0; i<decodedItems.decoded.size(); ++i) {
        switch (i) {
            case 0: { nonce = decodedItems.decoded[i]; } break;
            case 1: { gasPrice = decodedItems.decoded[i]; } break;
            case 2: { gasLimit = decodedItems.decoded[i]; } break;
            case 3: { to = Address(decodedItems.decoded[i]); } break;
            case 4: { amount = decodedItems.decoded[i]; } break;
            case 5: { payload = decodedItems.decoded[i]; } break;
            case 6: { signature.v = decodedItems.decoded[i]; } break;
            case 7: { signature.r = decodedItems.decoded[i]; } break;
            case 8: { signature.s = decodedItems.decoded[i]; } break;
            default: {} break;
        }
    }

    return (isValid() && signature.isValid());
}

// JuBiter-defined
Data TransactionPersonal::preHash() const {
    return TransactionBase::preHash({});
}

// JuBiter-defined
Data TransactionPersonal::serialize(const Data& chainID) const {
    return serialize();
}

// JuBiter-defined
Data TransactionPersonal::serialize() const {
    auto encoded = Data();

    // encode(b : 𝔹⁸ⁿ) = "\x19Ethereum Signed Message:\n" ‖ len(b) ‖ b where len(b) is the ascii-decimal encoding of the number of bytes in b.
    std::string pr = "Ethereum Signed Message:\n";

    encoded.push_back(0x19);
    std::copy(pr.begin(), pr.end(), std::back_inserter(encoded));
    auto sz = std::to_string(payload.size());
    std::copy(sz.begin(), sz.end(), std::back_inserter(encoded));
    std::copy(payload.begin(), payload.end(), std::back_inserter(encoded));

    return encoded;
}

// JuBiter-defined
bool TransactionPersonal::decoded(const Data& encoded, Signature& signature) {
    return false;
}

// JuBiter-defined
Data TransactionTypedData::preHash() const {
    return TransactionBase::preHash({});
}

// JuBiter-defined
Data TransactionTypedData::serialize(const Data& chainID) const {
    return serialize();
}

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
        return {};
    }

    Data domainSeparator = jub::eth::EIP712::typed_data_envelope(
                                "EIP712Domain",
                                typedData["domain"],
                                bMetamaskV4Compat);
    if (domainSeparator.empty()) {
        return {};
    }

    Data hashStructMessage = jub::eth::EIP712::typed_data_envelope(
                                typedData["primaryType"].get<std::string>().c_str(),
                                typedData["message"],
                                bMetamaskV4Compat);
    if (hashStructMessage.empty()) {
        return {};
    }

    std::copy(domainSeparator.begin(), domainSeparator.end(), std::back_inserter(encoded));
    std::copy(hashStructMessage.begin(), hashStructMessage.end(), std::back_inserter(encoded));

    return encoded;
}

// JuBiter-defined
bool TransactionTypedData::decoded(const Data& encoded, Signature& signature) {
    return false;
}

// JuBiter-defined
std::vector<Access> TransactionTyped::jsonToAccessList(const std::string& accessListInJSON) {
    std::vector<Access> vAccessList;
    if (0 >= accessListInJSON.length()) {
        return vAccessList;
    }

    nlohmann::json jsonAccessList = nlohmann::json::parse(accessListInJSON);
    if (nlohmann::detail::value_t::array != jsonAccessList.type()) {
        return {};
    }
    for (size_t i=0; i<jsonAccessList.size(); ++i) {
        if (nlohmann::detail::value_t::array != jsonAccessList[i].type()
            || 2 != jsonAccessList[i].size()
            ) {
            return {};
        }

        if (   nlohmann::detail::value_t::string != jsonAccessList[i][0].type()
            || nlohmann::detail::value_t::array  != jsonAccessList[i][1].type()
            ) {
            return {};
        }

        Access access;
        access.address = Address(jsonAccessList[i][0].get<std::string>());
        for (size_t j=0; j<jsonAccessList[i][1].size(); ++j) {
            const auto data = parse_hex(jsonAccessList[i][1][j].get<std::string>());
            access.storageKeys.push_back(data);
        }
        vAccessList.push_back(access);
    }

    return vAccessList;
}

// JuBiter-defined
Data TransactionTyped::encodeAccessList() const {
    auto encoded = Data();
    auto accessItem = Data();
    for (size_t i=0; i<accessList.size(); ++i) {
        accessItem.clear();
        append(accessItem, RLP::encode(Data(accessList[i].address.bytes.begin(), accessList[i].address.bytes.end())));

        Data storageKeys;
        if (0 == accessList[i].storageKeys.size()) {
            storageKeys = EmptyListEncoded;
        }
        else {
            storageKeys = RLP::encodeList(accessList[i].storageKeys);
        }
        append(accessItem, storageKeys);

        append(encoded, RLP::encodeList(accessItem));
    }

    return RLP::encodeList(encoded);
}

// JuBiter-defined
bool TransactionTyped::decodeAccessList(const Data& chainID, const Signature& signature, const Data& encoded) {
    // find the index of the begin of access list
    size_t offsetBegin = 0;
    auto part = serializePart(chainID);
    if (0 >= part.size()) {
        return false;
    }
    auto itInEncoded = TW::search(
                            std::begin(encoded), std::end(encoded),
                            std::begin(part), std::end(part)
    );
    if (itInEncoded != encoded.end()) {
        offsetBegin = (itInEncoded - std::begin(encoded)) + part.size();
    }
    // find the index of the end of access list
    size_t offsetEnd = 0;
    auto signatureRaw = signature.serialize();
    itInEncoded = TW::search(
                        std::begin(encoded)+offsetBegin, std::end(encoded),
                        std::begin(signatureRaw), std::end(signatureRaw)
    );
    if (itInEncoded != signatureRaw.end()) {
        offsetEnd = itInEncoded - std::begin(encoded);
    }

    auto encodedAccessList = Data(std::begin(encoded)+offsetBegin, std::begin(encoded)+offsetEnd);
    auto accessListItems = RLP::decode(encodedAccessList);
    Data remainder;
    for (size_t i=0; i<accessListItems.decoded.size(); ++i) {
        Access access;
        access.address = Address(accessListItems.decoded[i]);
        if (0 == remainder.size()) {
            remainder = encodedAccessList;
        }
        auto itInAccessList = TW::search(
                                    std::begin(remainder), std::end(remainder),
                                    std::begin(access.address.bytes), std::end(access.address.bytes)
        );
        if (itInAccessList != remainder.end()) {
            offsetBegin = (itInAccessList - std::begin(remainder)) + access.address.bytes.size();
        }
        auto storageKeys = Data(std::begin(remainder)+offsetBegin, std::end(remainder));
        auto storageKeysItems = RLP::decode(storageKeys);
        for (size_t j=0; j<storageKeysItems.decoded.size(); ++j) {
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
Data TransactionOptionalAccessList::serializePart(const Data& chainID) const {
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
Data TransactionOptionalAccessList::serialize(const Data& chainID) const {
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
    }
    else {
        append(encoded, encodeAccessList());
    }
    append(encoded, RLP::encode(Data{0}));
    append(encoded, RLP::encode(Data{0}));
    append(encoded, RLP::encode(Data{0}));

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

// JuBiter-defined
Data TransactionOptionalAccessList::encoded(const Signature& signature, const Data& chainID) const {
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
    }
    else {
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
bool TransactionOptionalAccessList::decoded(const Data& encoded, Signature& signature) {
    // parse transaction type
    auto decodedMainItems = RLP::decode(encoded);
    if (   1 != decodedMainItems.decoded.size()
        ||  0 > decodedMainItems.remainder.size()
        ) {
        return false;
    }
    if (   1 != decodedMainItems.decoded[0].size()
        || TransactionType::TxType_OptionalAccessList != decodedMainItems.decoded[0][0]
        ) {
        return false;
    }
    type = (TransactionType)decodedMainItems.decoded[0][0];

    auto decodedItems = RLP::decode(decodedMainItems.remainder);
    if (0 > decodedItems.remainder.size()) {
        return false;
    }
    size_t accessIndex = ITEM_ACCESS_LIST_INDEX-1;
    if (ITEM_COUNT == decodedItems.decoded.size()) {
        accessIndex = ITEM_ACCESS_LIST_INDEX;
    }

    // parse tx
    Data chainId;
    for (size_t i=0; i<decodedItems.decoded.size(); ++i) {
        switch (i) {
            case 0: { chainId = decodedItems.decoded[i]; } break;
            case 1: { nonce = decodedItems.decoded[i]; } break;
            case 2: { gasPrice = decodedItems.decoded[i]; } break;
            case 3: { gasLimit = decodedItems.decoded[i]; } break;
            case 4: { to = Address(decodedItems.decoded[i]); } break;
            case 5: { amount = decodedItems.decoded[i]; } break;
            case 6: { payload = decodedItems.decoded[i]; } break;
            default: { } break;
        }
    }

    // parse signature
    if (decodedItems.decoded.size() != (accessIndex+1+3)) {
        return false;
    }
    // Because usesReplayProtection() is not used for Eip1559, the v value might be 0x80.
    // When v is 0x80, this v item is null in RLP code.
    // Here we deal with v value 0x80.
    auto v = decodedItems.decoded[accessIndex+1];
    signature.v = (0 == v.size() ? parse_hex("80") : v);
    signature.r = decodedItems.decoded[accessIndex+2];
    signature.s = decodedItems.decoded[accessIndex+3];

    // decode access list
    bool b = false;
    if (ITEM_ACCESS_LIST_INDEX == accessIndex) {
        b = decodeAccessList(chainId, signature, encoded);
    }
    if (!b) {
        return b;
    }

    return (isValid() && signature.isValid());
}

// JuBiter-defined
Data TransactionEip1559::serializePart(const Data& chainID) const {
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

Data TransactionEip1559::serialize(const Data& chainID) const {
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
    }
    else {
        append(encoded, encodeAccessList());
    }

    Data envelope;
    append(envelope, static_cast<uint8_t>(type));
    append(envelope, RLP::encodeList(encoded));
    return envelope;
}

Data TransactionEip1559::encoded(const Signature& signature, const Data& chainID) const {
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
    }
    else {
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
bool TransactionEip1559::decoded(const Data& encoded, Signature& signature) {
    // parse transaction type
    auto decodedMainItems = RLP::decode(encoded);
    if (   1 != decodedMainItems.decoded.size()
        ||  0 > decodedMainItems.remainder.size()
        ) {
        return false;
    }
    if (   1 != decodedMainItems.decoded[0].size()
        || TransactionType::TxType_Eip1559 != decodedMainItems.decoded[0][0]
        ) {
        return false;
    }
    type = (TransactionType)decodedMainItems.decoded[0][0];

    auto decodedItems = RLP::decode(decodedMainItems.remainder);
    if (0 > decodedItems.remainder.size()) {
        return false;
    }

    size_t accessIndex = ITEM_ACCESS_LIST_INDEX-1;
    if (ITEM_COUNT == decodedItems.decoded.size()) {
        accessIndex = ITEM_ACCESS_LIST_INDEX;
    }

    // parse tx
    Data chainId;
    for (size_t i=0; i<decodedItems.decoded.size(); ++i) {
        switch (i) {
            case 0: { chainId = decodedItems.decoded[i]; } break;
            case 1: { nonce = decodedItems.decoded[i]; } break;
            case 2: { maxInclusionFeePerGas = decodedItems.decoded[i]; } break;
            case 3: { maxFeePerGas = decodedItems.decoded[i]; } break;
            case 4: { gasLimit = decodedItems.decoded[i]; } break;
            case 5: { to = Address(decodedItems.decoded[i]); } break;
            case 6: { amount = decodedItems.decoded[i]; } break;
            case 7: { payload = decodedItems.decoded[i]; } break;
            default: {} break;
        }
    }

    // parse signature
    if (decodedItems.decoded.size() != (accessIndex+1+3)) {
        return false;
    }
    // Because usesReplayProtection() is not used for Eip1559, the v value might be 0x80.
    // When v is 0x80, this v item is null in RLP code.
    // Here we deal with v value 0x80.
    auto v = decodedItems.decoded[accessIndex+1];
    signature.v = (0 == v.size() ? parse_hex("80") : v);
    signature.r = decodedItems.decoded[accessIndex+2];
    signature.s = decodedItems.decoded[accessIndex+3];

    // decode access list
    bool b = false;
    if (ITEM_ACCESS_LIST_INDEX == accessIndex) {
        b = decodeAccessList(chainId, signature, encoded);
    }
    if (!b) {
        return b;
    }

    return (isValid() && signature.isValid());
}
