// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Program.h"
#include "../Hash.h"
#include "Address.h"
#include "Data.h"
#include "Transaction.h"

#include <TrezorCrypto/ed25519-donna/ed25519-donna.h>

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

using namespace TW;
using namespace TW::Solana;

Address StakeProgram::addressFromValidatorSeed(const Address &fromAddress, const Address &validatorAddress,
                                               const Address &programId) {
    Data extended    = fromAddress.vector();
    std::string seed = validatorAddress.string();
    Data vecSeed(seed.begin(), seed.end());
    vecSeed.resize(32);
    Data additional = programId.vector();
    extended.insert(extended.end(), vecSeed.begin(), vecSeed.end());
    extended.insert(extended.end(), additional.begin(), additional.end());
    Data hash = TW::Hash::sha256(extended);
    return Address(hash);
}

Address StakeProgram::addressFromRecentBlockhash(const Address &fromAddress, const Hash &recentBlockhash,
                                                 const Address &programId) {
    Data extended    = fromAddress.vector();
    std::string seed = recentBlockhash.encoded();
    Data vecSeed(seed.begin(), seed.end());
    vecSeed.resize(32);
    Data additional = programId.vector();
    extended.insert(extended.end(), vecSeed.begin(), vecSeed.end());
    extended.insert(extended.end(), additional.begin(), additional.end());
    Data hash = TW::Hash::sha256(extended);
    return Address(hash);
}

/*
 * Based on solana-program-library code, get_associated_token_address()
 * https://github.com/solana-labs/solana-program-library/blob/master/associated-token-account/program/src/lib.rs#L35
 * https://github.com/solana-labs/solana-program-library/blob/master/associated-token-account/program/src/lib.rs#L19
 */
Address TokenProgram::defaultTokenAddress(const Address &mainAddress, const Address &tokenMintAddress) {
    auto programId = Address(TOKEN_PROGRAM_ID_ADDRESS);

    std::vector<Data> seeds = {TW::data(mainAddress.bytes.data(), mainAddress.bytes.size()),
                               TW::data(programId.bytes.data(), programId.bytes.size()),
                               TW::data(tokenMintAddress.bytes.data(), tokenMintAddress.bytes.size())};

    auto result = findProgramAddress(seeds, Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS));
    return std::get<0>(result);
}

/*
 * Based on solana code, find_program_address()
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L193
 */
std::tuple<Address, uint8_t> TokenProgram::findProgramAddress(const std::vector<TW::Data> &seeds,
                                                              const Address &programId) {
    Address result(Data(32));
    uint8_t seed = 255;

    // cycle through seeds for the rare case when result is not valid
    for (; seed >= 0; --seed) {
        auto seedsCopy = std::vector<Data>{seeds};
        // add extra seed
        seedsCopy.push_back({seed});
        auto address = createProgramAddress(seedsCopy, Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS));

        auto publicKey = PublicKey(address.vector(), TWPublicKeyTypeED25519);
        if (!publicKey.isValidED25519()) {
            result = address;
            break;
        }
        // try next seed
    }
    return std::make_tuple(result, seed);
}

/*
 * Based on solana code, create_program_address()
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L135
 */
Address TokenProgram::createProgramAddress(const std::vector<TW::Data> &seeds, const Address &programId) {
    // concatenate seeds
    Data hashInput;
    for (auto &seed : seeds) {
        append(hashInput, seed);
    }
    // append programId
    append(hashInput, TW::data(programId.bytes.data(), programId.bytes.size()));
    append(hashInput, TW::data("ProgramDerivedAddress"));
    // compute hash
    Data hash = TW::Hash::sha256(hashInput.data(), hashInput.size());
    return Address(hash);
}
