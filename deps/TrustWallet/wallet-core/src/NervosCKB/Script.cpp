// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../NervosCKB/Script.h"

#include "Address.h"
#include "CashAddress.h"
#include "SegwitAddress.h"

#include "../Base58.h"

#include "../BinaryCoding.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PublicKey.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>

using namespace TW;
using namespace TW::NervosCKB;


Data Script::hash() const {

    Hasher hasher;
    hasher_InitParam(&hasher, HasherType::HASHER_BLAKE2B_PERSONAL,
                     personal_ckb, sizeof(personal_ckb)/sizeof(uint8_t));

    TW::Data blake(HASHER_DIGEST_LENGTH);
    if (0 != hasher_InitParam_Raw(&hasher,
                                  &bytes[0], (uint32_t)bytes.size(),
                                  &blake[0])) {
        return {};
    }

    return blake;
}


/// Builds a lock script from a public key hash.
bool Script::buildLockScript(const TW::Data& codeHash, const uint8_t type, const TW::Data& args, TW::Data& scriptLock) {

    if (TW::Hash::sha256Size != codeHash.size()) {
        return false;
    }

    if (   ScriptHashType::DATA != type
        && ScriptHashType::TYPE != type
        ) {
        return false;
    }

    std::copy(std::begin(codeHash), std::end(codeHash), std::back_inserter(scriptLock));
    scriptLock.push_back(type);
    TW::NervosCKB::Script(TW::Data(args)).encode(scriptLock);

    return true;
}


/// Builds a type script from a public key hash.
bool Script::buildTypeScript(const TW::Data& codeHash, const uint8_t type, const TW::Data& args, TW::Data& scriptType) {

    if (0 == codeHash.size()
        ) {
        scriptType.clear();
        return true;
    }

    return Script::buildLockScript(codeHash, type, args, scriptType);
}


bool Script::parseScript(const TW::Data& srp, TW::Data& codeHash, uint8_t& type, TW::Data& args) {

    if (TW::Hash::sha256Size >= srp.size()
        ||               0 >= srp.size()
        ) {
        return false;
    }

    size_t index = TW::Hash::sha256Size;
    codeHash.clear();
    std::copy(std::begin(srp), std::begin(srp)+index, std::back_inserter(codeHash));

    type = srp[index];
    if (   ScriptHashType::DATA != type
        && ScriptHashType::TYPE != type
        ) {
        return false;
    }

    index += sizeof(uint8_t)/sizeof(uint8_t);
    if (index == srp.size()) {
        return false;
    }

    Data temp(std::begin(srp)+index, std::end(srp));
    Script script;
    if (!script.decode(temp)) {
        return false;
    }

    args.clear();
    std::copy(std::begin(script.bytes), std::end(script.bytes), std::back_inserter(args));

    return true;
}


std::vector<TW::Data> Script::serializeScript(const TW::Data& srp) {

    TW::Data codeHash;
    uint8_t type = 0;
    TW::Data args;
    if (!parseScript(srp, codeHash, type, args)) {
        return {};
    }

    std::vector<TW::Data> table;
    table.push_back(codeHash);
    table.push_back({type});
    TW::Data serializedArgs;
    TW::NervosCKB::Script(TW::Data(args)).encode(serializedArgs);
    table.push_back(serializedArgs);

    return table;
}


void Script::encode(Data& data) const {

    encode32LE((uint32_t)bytes.size(), data);
    std::copy(std::begin(bytes), std::end(bytes), std::back_inserter(data));
}


Data Script::encode() const {

    Data data;
    encode(data);

    return data;
}


/// Decodes the script.
bool Script::decode(const Data& data) {

    size_t index = 0;

    uint32_t sz = decode32LE(&data[index]);
    index += sizeof(uint32_t)/sizeof(uint8_t);
    if (sz != (data.size()-index)) {
        return false;
    }

    bytes.clear();
    std::copy(std::begin(data)+index, std::end(data), std::back_inserter(bytes));

    return true;
}


/// Return the size of the script,
/// including the number of bytes that represent the number of script bytes(decodeVarInt())
size_t Script::size() {

    return (sizeof(uint32_t)/sizeof(uint8_t) + bytes.size());
}
