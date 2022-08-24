// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../BinaryCoding.h"
#include "../PublicKey.h"
#include "Data.h"
#include "Hash.h"
#include "Signer.h"
#include "Solana/Address.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <vector>

using namespace TW;
using namespace TW::Solana;
using namespace std;

size_t getShortVecLength(const unsigned char *p) {
    auto v = *p++;
    if (v & 0x80) {
        v <<= 7;
        v |= *p;
    }

    return v;
}

CompiledInstruction CompiledInstruction::Compile(const Instruction &instruction,
                                                 const std::vector<Address> &addresses) {
    auto accountIndex = [&](auto &address) {
        auto it = std::find(addresses.begin(), addresses.end(), address);
        if (it == addresses.end()) {
            throw std::invalid_argument("address not found");
        }
        auto dist = std::distance(addresses.begin(), it);
        return (uint8_t)dist;
    };

    auto programIdIndex = accountIndex(instruction.programId);
    auto accounts       = std::vector<uint8_t>();

    for (auto &account : instruction.accounts) {
        accounts.push_back(accountIndex(account.account));
    }
    return {programIdIndex, accounts, instruction.data};
}

void Message::addAccount(const AccountMeta &account) {
    bool inSigned = (std::find(signedAccounts.begin(), signedAccounts.end(), account.account) != signedAccounts.end());
    bool inUnsigned =
        (std::find(unsignedAccounts.begin(), unsignedAccounts.end(), account.account) != unsignedAccounts.end());
    bool inReadOnly =
        (std::find(readOnlyAccounts.begin(), readOnlyAccounts.end(), account.account) != readOnlyAccounts.end());
    if (account.isSigner) {
        if (!inSigned) {
            signedAccounts.push_back(account.account);
        }
    } else if (!account.isReadOnly) {
        if (!inSigned && !inUnsigned) {
            unsignedAccounts.push_back(account.account);
        }
    } else {
        if (!inSigned && !inUnsigned && !inReadOnly) {
            readOnlyAccounts.push_back(account.account);
        }
    }
}

void Message::addAccountKeys(const Address &account) {
    if (std::find(accountKeys.begin(), accountKeys.end(), account) == accountKeys.end()) {
        accountKeys.push_back(account);
    }
}

void Message::compileAccounts() {
    for (auto &instr : instructions) {
        for (auto &address : instr.accounts) {
            addAccount(address);
        }
    }
    // add programIds (read-only, at end)
    for (auto &instr : instructions) {
        addAccount(AccountMeta{instr.programId, false, true});
    }

    header = MessageHeader{(uint8_t)signedAccounts.size(), 0, (uint8_t)readOnlyAccounts.size()};

    // merge the three buckets
    accountKeys.clear();
    for (auto &a : signedAccounts) {
        addAccountKeys(a);
    }
    for (auto &a : unsignedAccounts) {
        addAccountKeys(a);
    }
    for (auto &a : readOnlyAccounts) {
        addAccountKeys(a);
    }

    compileInstructions();
}

void Message::compileInstructions() {
    compiledInstructions.clear();
    for (auto instruction : instructions) {
        compiledInstructions.emplace_back(CompiledInstruction::Compile(instruction, accountKeys));
    }
}

Transaction Transaction::decode(const Data &raw) {
    if (raw.size() < 65 + 3 + 1 + 32 * 2 + 3) {
        throw "Invalid transaction";
    }

    auto *p   = raw.data();
    auto *end = p + raw.size();

    // sig count
    auto count = getShortVecLength(p);
    p += count > 0x7f ? 2 : 1;

    // sig contents
    auto sigs = std::vector<Signature>();
    for (auto i = 0; i < count; i++) {
        auto sig = Signature(Data{p, p + 64});
        sigs.push_back(sig);
        p += 64;
    }

    // Message
    // MessageHeader
    auto header = MessageHeader{
        *p++,
        *p++,
        *p++,
    };

    // accounts
    auto pubKeys = std::vector<PublicKey>();

    count = getShortVecLength(p);
    p += count > 0x7f ? 2 : 1;

    for (auto i = 0; i < count; i++) {
        auto key = PublicKey(Data{p, p + 32}, TWPublicKeyTypeED25519);
        pubKeys.push_back(key);
        p += 32;
    }

    // recent hash
    auto content = std::array<uint8_t, Hash::size>();
    std::copy(p, p + 32, content.begin());
    auto recentHash = Hash{content};
    p += 32;

    auto instructions = std::vector<CompiledInstruction>();
    // instructions
    count = getShortVecLength(p);
    p += count > 0x7f ? 2 : 1;

    for (auto i = 0; i < count; i++) {
        auto programId = *p++;
        // shadow outside `count`
        auto count = getShortVecLength(p);
        p += count > 0x7f ? 2 : 1;
        auto accountIndexs = Data{p, p + count};

        p += count;
        count = getShortVecLength(p);
        p += count > 0x7f ? 2 : 1;
        auto data = Data{p, p + count};
        p += count;
        instructions.push_back({programId, accountIndexs, data});
    }

    if (p != end) {
        throw "Invalid transaction";
    }

    auto msg      = Message{};
    msg.header    = header;
    auto accounts = std::vector<Address>{};
    std::transform(pubKeys.begin(), pubKeys.end(), std::back_inserter(accounts),
                   [](auto &key) { return Address{key}; });

    msg.recentBlockhash      = recentHash;
    msg.compiledInstructions = instructions;

    return Transaction{msg, sigs};
}

std::vector<uint8_t> Transaction::serialize() const {
    Data buffer;

    append(buffer, shortVecLength<Signature>(this->signatures));
    for (auto signature : this->signatures) {
        Data signature_vec(signature.bytes.begin(), signature.bytes.end());
        append(buffer, signature_vec);
    }
    append(buffer, this->messageData());

    return buffer;
}

Data Transaction::messageData() const {
    Data buffer;

    buffer.push_back(this->message.header.numRequiredSignatures);
    buffer.push_back(this->message.header.numCreditOnlySignedAccounts);
    buffer.push_back(this->message.header.numCreditOnlyUnsignedAccounts);
    append(buffer, shortVecLength<Address>(this->message.accountKeys));
    for (auto account_key : this->message.accountKeys) {
        Data account_key_vec(account_key.bytes.begin(), account_key.bytes.end());
        append(buffer, account_key_vec);
    }
    Data recentBlockhash(this->message.recentBlockhash.bytes.begin(), this->message.recentBlockhash.bytes.end());
    append(buffer, recentBlockhash);

    // apppend compiled instructions
    append(buffer, shortVecLength<CompiledInstruction>(message.compiledInstructions));
    for (auto instruction : message.compiledInstructions) {
        buffer.push_back(instruction.programIdIndex);
        append(buffer, shortVecLength<uint8_t>(instruction.accounts));
        append(buffer, instruction.accounts);
        append(buffer, shortVecLength<uint8_t>(instruction.data));
        append(buffer, instruction.data);
    }

    return buffer;
}

uint8_t Transaction::getAccountIndex(Address publicKey) const {
    auto item = std::find(this->message.accountKeys.begin(), this->message.accountKeys.end(), publicKey);
    if (item == this->message.accountKeys.end()) {
        throw std::invalid_argument("publicKey not found in message.accountKeys");
    }
    return (uint8_t)std::distance(this->message.accountKeys.begin(), item);
}

bool Signature::operator==(const Signature &v) const { return bytes == v.bytes; }
