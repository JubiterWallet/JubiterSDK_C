// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Script.h"

#include "Address.h"
#include "CashAddress.h"
#include "SegwitAddress.h"

#include "../Base58.h"
//#include "../Coin.h"

#include "../BinaryCoding.h"
#include "../Data.h"
//#include "../Decred/Address.h"
//#include "../Groestlcoin/Address.h"
#include "../Hash.h"
#include "../PublicKey.h"
//#include "../Zcash/TAddress.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>
#include <cstdint>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

Data Script::hash() const { return Hash::ripemd(Hash::sha256(bytes)); }

bool Script::isPayToScriptHash() const {
    // Extra-fast test for pay-to-script-hash
    return bytes.size() == 23 && bytes[0] == OP_HASH160 && bytes[1] == 0x14 && bytes[22] == OP_EQUAL;
}

bool Script::isPayToWitnessScriptHash() const {
    // Extra-fast test for pay-to-witness-script-hash
    return bytes.size() == 22 && bytes[0] == OP_0 && bytes[1] == 0x14;
}

bool Script::isMultisigProgram() const {
    // x sig1 sig2 ... m pub1 pub2 ... n
    if (bytes.size() < 0x91) return false;
    return bytes.back() == OP_CHECKMULTISIG;
}

bool Script::isWitnessProgram() const {
    // [nVersion][Data]
    if (bytes.size() < 4 || bytes.size() > 42) {
        return false;
    }
    if (bytes[0] != OP_0 && (bytes[0] < OP_1 || bytes[0] > OP_16)) {
        return false;
    }
    return bytes[1] + 2 == bytes.size();
}

bool Script::matchPayToPubkey(Data &result) const {
    if (bytes.size() == PublicKey::secp256k1ExtendedSize + 2 && bytes[0] == PublicKey::secp256k1ExtendedSize &&
        bytes.back() == OP_CHECKSIG) {
        result.clear();
        std::copy(std::begin(bytes) + 1, std::begin(bytes) + 1 + PublicKey::secp256k1Size, std::back_inserter(result));
        return true;
    }
    if (bytes.size() == PublicKey::secp256k1Size + 2 && bytes[0] == PublicKey::secp256k1Size &&
        bytes.back() == OP_CHECKSIG) {
        result.clear();
        std::copy(std::begin(bytes) + 1, std::begin(bytes) + 1 + PublicKey::secp256k1Size, std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToPubkeyHash(Data &result) const {
    if (bytes.size() == 25 && bytes[0] == OP_DUP && bytes[1] == OP_HASH160 && bytes[2] == 20 &&
        bytes[23] == OP_EQUALVERIFY && bytes[24] == OP_CHECKSIG) {
        result.clear();
        std::copy(std::begin(bytes) + 3, std::begin(bytes) + 3 + 20, std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToScriptHash(Data &result) const {
    if (!isPayToScriptHash()) {
        return false;
    }
    result.clear();
    std::copy(std::begin(bytes) + 2, std::begin(bytes) + 22, std::back_inserter(result));
    return true;
}

bool Script::matchPayToWitnessPublicKeyHash(Data &result) const {
    if (bytes.size() == 22 && bytes[0] == OP_0 && bytes[1] == 0x14) {
        result.clear();
        std::copy(std::begin(bytes) + 2, std::end(bytes), std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToWitnessScriptHash(Data &result) const {
    if (bytes.size() == 34 && bytes[0] == OP_0 && bytes[1] == 0x20) {
        result.clear();
        std::copy(std::begin(bytes) + 2, std::end(bytes), std::back_inserter(result));
        return true;
    }
    return false;
}

/// Decodes a small integer
static inline int decodeNumber(uint8_t opcode) {
    if (opcode == OP_0) {
        return 0;
    }
    assert(opcode >= OP_1 && opcode <= OP_16);
    return static_cast<int>(opcode) - static_cast<int>(OP_1 - 1);
}

// only support one signature
bool Script::matchMultisig(std::vector<Data> &keys, int &required) const {
    keys.clear();
    required = 0;

    if (bytes.size() < 1 || bytes.back() != OP_CHECKMULTISIG) {
        return false;
    }

    size_t it = 0;
    uint8_t opcode;
    Data operand;

    auto op = getScriptOp(it, opcode, operand);
    if (!op || !TWOpCodeIsSmallInteger(opcode)) {
        return false;
    }
    required = decodeNumber(opcode);
    while (true) {
        auto res = getScriptOp(it, opcode, operand);
        if (!res) {
            break;
        }
        if (!PublicKey::isValid(operand, TWPublicKeyTypeSECP256k1)) {
            break;
        }
        keys.push_back(operand);
    }

    if (!TWOpCodeIsSmallInteger(opcode)) {
        return false;
    }

    auto expectedCount = decodeNumber(opcode);
    if (keys.size() != expectedCount || expectedCount < required) {
        return false;
    }
    if (it + 1 != bytes.size()) {
        return false;
    }

    return true;
}

bool Script::matchMultisigScriptSig(std::vector<Data> &sigs, std::vector<Data> &keys, int &m, int& n) const {
    sigs.clear();
    keys.clear();
    m = 0;

    if (bytes.size() < 1 || bytes.back() != OP_CHECKMULTISIG) {
        return false;
    }

    size_t it = 0;
    uint8_t opcode;
    Data operand;
    // skip unused value
    it += 1;
    // sigs
    // 0 sig1 sig2 ... m pub1 pub2 ... n
    while (true) {
        auto op = getScriptOp(it, opcode, operand);
        // der encoded signature
        if (!op || operand[0] != 0x30) {
            break;
        }
        sigs.push_back(operand);
    }
    
    auto redeem = Script(std::move(operand));
    if (!redeem.matchMultisig(keys, n)) {
        return false;
    }
    m = keys.size();
    return true;
}


// JuBiter-defined
/// Matches the script to a scriptSig for a pay-to-public-key-hash (P2PKH).
bool Script::matchPayToPublicKeyHashScriptSig(Data &signature, Data &publicKey) const {
    size_t index = 0;
    // [signature] [publicKey]

    size_t size = 0;
    Data tempSignature(bytes.begin() + index, bytes.end());
    size_t signatureLen = decodeVarInt(tempSignature, size);
    Script signScript(tempSignature.begin() + size, tempSignature.begin() + size + signatureLen);
    index = signScript.size();

    Data tempPublicKey(bytes.begin() + index, bytes.end());
    size_t publicKeyLen = decodeVarInt(tempPublicKey, size);
    Script pubkeyScript(tempPublicKey.begin() + size, tempPublicKey.begin() + size + publicKeyLen);
    index += pubkeyScript.size();
    if (bytes.end() != (bytes.begin() + index)) {
        return false;
    }

    signature = signScript.bytes;
    publicKey = pubkeyScript.bytes;

    return true;
}

bool Script::getScriptOp(size_t &index, uint8_t &opcode, Data &operand) const {
    operand.clear();

    // Read instruction
    if (index >= bytes.size()) {
        return false;
    }

    opcode = bytes[index];
    index += 1;

    if (opcode > OP_PUSHDATA4) {
        return true;
    }

    // Immediate operand
    size_t size = 0;
    if (opcode < OP_PUSHDATA1) {
        size = static_cast<size_t>(opcode);
    } else if (opcode == OP_PUSHDATA1) {
        if (bytes.size() - index < 1) {
            return false;
        }
        size = index;
        index += 1;
    } else if (opcode == OP_PUSHDATA2) {
        if (bytes.size() - index < 2) {
            return false;
        }
        size = static_cast<size_t>(decode16LE(bytes.data() + index));
        index += 2;
    } else if (opcode == OP_PUSHDATA4) {
        if (bytes.size() - index < 4) {
            return false;
        }
        size = static_cast<size_t>(decode32LE(bytes.data() + index));
        index += 4;
    }
    if (bytes.size() - index < size) {
        return false;
    }
    operand = Data(bytes.begin() + index, bytes.begin() + index + size);
    index += size;

    return true;
}

Script Script::buildPayToPublicKeyHash(const Data &hash) {
    assert(hash.size() == 20);
    Script script;
    script.bytes.push_back(OP_DUP);
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    script.bytes.insert(script.bytes.end(), hash.begin(), hash.end());
    script.bytes.push_back(OP_EQUALVERIFY);
    script.bytes.push_back(OP_CHECKSIG);
    return script;
}

Script Script::buildPayToScriptHash(const Data &scriptHash) {
    assert(scriptHash.size() == 20);
    Script script;
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    script.bytes.insert(script.bytes.end(), scriptHash.begin(), scriptHash.end());
    script.bytes.push_back(OP_EQUAL);
    return script;
}

Script Script::buildPayToWitnessProgram(uint8_t version, const Data &program) {
    assert(version >= 0 && version <= 16);
    // https://github.com/bitcoin/bips/blob/master/bip-0141.mediawiki#witness-program
    // https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#script-validation-rules
    assert((version == 0 && (program.size() == 20 || program.size() == 32)) || (version == 1 && program.size() == 32));

    Script script;
    // should push number value which defined in Bitcoin opcode
    script.bytes.push_back(version == 0 ? OP_0 : 0x50 + version);
    script.bytes.push_back(static_cast<byte>(program.size()));
    script.bytes.insert(script.bytes.end(), program.begin(), program.end());
    return script;
}

Script Script::buildPayToWitnessPubkeyHash(const Data &hash) { return Script::buildPayToWitnessProgram(0, hash); }

Script Script::buildPayToWitnessScriptHash(const Data &scriptHash) {
    return Script::buildPayToWitnessProgram(0, scriptHash);
}

Script Script::buildPayToTaprootKeyPathSpending(const Data &program) {
    return Script::buildPayToWitnessProgram(1, program);
}

// JuBiter-defined
/// Builds a redeem script for pay-to-script-hash (P2SH).
Script Script::buildRedeemScript(const uint8_t m, const uint8_t n, const std::vector<Data> &publicKeys) {
    uint8_t op_m = OP_RESERVED + m;
    uint8_t op_n = OP_RESERVED + n;
    //[m] [pubkey1] [pubkey2] ... [pubkeyn] [n] [OP_CHECKMULTISIG]
    Script script;
    script.bytes.push_back(op_m);
    for (const auto &publicKey : publicKeys) {
        script.bytes.push_back(publicKey.size());
        script.bytes.insert(script.bytes.end(), publicKey.begin(), publicKey.end());
    }
    script.bytes.push_back(op_n);
    script.bytes.push_back(OP_CHECKMULTISIG);
    return script;
}

// JuBiter-defined
/// Builds a redeem script tlv (applet use) for pay-to-script-hash (P2SH).
Script Script::buildRedeemScript(const uint8_t m, const uint8_t n, const std::vector<Data> &publicKeys, const uint8_t orderedIndex) {
    uint8_t op_m = OP_RESERVED + m;
    uint8_t op_n = OP_RESERVED + n;
    //redeemScriptTlv: orderedIndex [m] <pubkey1> <pubkey2> ... <pubkeyn> [n] [OP_CHECKMULTISIG]
    Script script;
    script.bytes.push_back(orderedIndex);
    script.bytes.push_back(op_m);
    for(size_t i=0; i<publicKeys.size(); ++i) {
        if (i == orderedIndex) {
            continue;
        }
        script.bytes.insert(script.bytes.end(), publicKeys[i].size());
        script.bytes.insert(script.bytes.end(), publicKeys[i].begin(), publicKeys[i].end());
    }
    script.bytes.push_back(op_n);
    script.bytes.push_back(OP_CHECKMULTISIG);
    return script;
}

// JuBiter-defined
/// Builds a scriptSig for pay-to-public-key-hash (P2PKH) script.
Script Script::buildPayToPublicKeyHashScriptSig(const Data &signature, const Data &publicKey) {
    // [signature] [publicKey]
    Script script;
    script += Script(signature.begin(), signature.end());
    script += Script(publicKey.begin(), publicKey.end());
    return script;
}

// JuBiter-defined
/// Builds a scriptSig for pay-to-public-key-hash (P2WPKH) script.
std::vector<Data> Script::buildPayToPublicKeyHashScriptSigWitness(Data signature, Data publicKey) {
    std::vector<Data> scriptWitness;
    scriptWitness.push_back(signature);
    scriptWitness.push_back(publicKey);
    return scriptWitness;
}

// JuBiter-defined
/// Builds a scriptSig for the redeem of pay-to-script-hash (P2SH) script.
Script Script::buildPayToScriptHashWitness(const Data &redeemScript, const std::vector<Data> &signatures) {
    // {[signature1] [signature2]} [witness script]
    Script script;
    for (const auto &signature : signatures) {
        script.bytes.insert(script.bytes.end(), signature.begin(), signature.end());
    }
    script.bytes.insert(script.bytes.end(), redeemScript.begin(), redeemScript.end());
    return script;
}

// JuBiter-defined
/// Builds a witness for pay-to-taproot-key-path-spending
std::vector<Data> Script::buildPayToTaprootKeyPathSpendingWitness(const Data &signature) { return {signature}; }

// JuBiter-defined
/// Builds a return0 script from a script.
Script Script::buildReturn0(const Data &data, const Data &check, int offset) {
    Script script;
    // Check if
    if (!check.empty()) {
        const std::vector<std::size_t> founds = find_all_indexes(data, check);
        if (std::end(founds) == std::find(std::begin(founds), std::end(founds), offset)) {
            return script;
        }
    }
    script.bytes.push_back(OP_RETURN);
    script.bytes.push_back(data.size());
    script.bytes.insert(script.bytes.end(), data.begin(), data.end());
    return script;
}

Script Script::buildPayToScriptMultisigScriptSig(const std::vector<Data> &signatures, const std::vector<Data>& publicKeys, int m, int n) {
    Script script;
    // https://en.bitcoin.it/wiki/Script OP_CHECKMULTISIG
    // x sig1, sig2 ... m pub1, pub2 ... n OP_CHECKMULTISIG
    // push a unused value
    script.bytes.push_back(OP_0);
    for (auto& sig: signatures) {
        script += Script(Data(sig));
    }
    auto redeem = buildRedeemScript(m, n, publicKeys);
    script += redeem;
    return script;
}

void Script::encode(Data &data) const {
    encodeVarInt(bytes.size(), data);
    std::copy(std::begin(bytes), std::end(bytes), std::back_inserter(data));
}

// JuBiter-defined
void Script::encodeZero(Data &data) const {
    TW::Bitcoin::Script zeroScript;
    zeroScript.encode(data);
}

// JuBiter-defined
/// Decodes the script.
bool Script::decode(const Data &data) {

    size_t szScript = decodeVarInt(data, _varIntSize);
    if (0 == szScript) {
        return true;
    }
    std::copy(std::begin(data) + _varIntSize, std::begin(data) + _varIntSize + szScript, std::back_inserter(bytes));

    return true;
}

// JuBiter-defined
/// Return the size of the script,
/// including the number of bytes that represent the number of script bytes(decodeVarInt())
size_t Script::size() {
    if (0 == _varIntSize) {
        Data temp;
        _varIntSize = encodeVarInt(bytes.size(), temp);
    }

    return (_varIntSize + bytes.size());
}

Script Script::buildForAddress(const std::string &string, enum TWCoinType coin) {
    if (Address::isValid(string)) {
        auto address = Address(string);
        auto p2pkh   = TWCoinTypeP2pkhPrefix(coin);
        auto p2sh    = TWCoinTypeP2shPrefix(coin);
        if (p2pkh == address.bytes[0]) {
            // address starts with 1/L
            auto data = Data();
            data.reserve(Address::size - 1);
            std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
            return buildPayToPublicKeyHash(data);
        } else if (p2sh == address.bytes[0]) {
            // address starts with 3/M
            auto data = Data();
            data.reserve(Address::size - 1);
            std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
            return buildPayToScriptHash(data);
        }
    } else if (SegwitAddress::isValid(string)) {
        auto result = SegwitAddress::decode(string);
        auto addr   = result.first;
        // address starts with bc/ltc
        return buildPayToWitnessProgram(addr.witnessVersion, addr.witnessProgram);
    } else if (CashAddress::isValid(string)) {
        auto address        = CashAddress(string);
        auto bitcoinAddress = address.legacyAddress();
        return buildForAddress(bitcoinAddress.string(), TWCoinTypeBitcoinCash);
        //    } else if (Decred::Address::isValid(string)) {
        //        auto bytes = Base58::bitcoin.decodeCheck(string, Hash::blake256d);
        //        if (bytes[1] == TW::p2pkhPrefix(TWCoinTypeDecred)) {
        //            return buildPayToPublicKeyHash(Data(bytes.begin() + 2, bytes.end()));
        //        }
        //        if (bytes[1] == TWCoinTypeP2shPrefix(TWCoinTypeDecred)) {
        //            return buildPayToScriptHash(Data(bytes.begin() + 2, bytes.end()));
        //        }
        //    } else if (Groestlcoin::Address::isValid(string)) {
        //        auto address = Groestlcoin::Address(string);
        //        auto data = Data();
        //        data.reserve(Address::size - 1);
        //        std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
        //        if (address.bytes[0] == TWCoinTypeP2pkhPrefix(TWCoinTypeGroestlcoin)) {
        //            return buildPayToPublicKeyHash(data);
        //        }
        //        if (address.bytes[0] == TWCoinTypeP2shPrefix(TWCoinTypeGroestlcoin)) {
        //            return buildPayToScriptHash(data);
        //        }
        //    } else if (Zcash::TAddress::isValid(string)) {
        //        auto address = Zcash::TAddress(string);
        //        auto data = Data();
        //        data.reserve(Address::size - 2);
        //        std::copy(address.bytes.begin() + 2, address.bytes.end(), std::back_inserter(data));
        //        if (address.bytes[1] == TWCoinTypeP2pkhPrefix(TWCoinTypeZcash)) {
        //            return buildPayToPublicKeyHash(data);
        //        } else if (address.bytes[1] == TWCoinTypeP2shPrefix(TWCoinTypeZcash)) {
        //            return buildPayToScriptHash(data);
        //        }
    }
    return {};
}

// JuBiter-defined
bool Script::parseWitnessStackToPayToWitnessScriptHash(const std::vector<Data> &scriptWitness, Data &signature,
                                                       Data &publicKey) {

    if (2 != scriptWitness.size()) {
        return false;
    }

    signature = scriptWitness[0];
    publicKey = scriptWitness[1];

    return true;
}
