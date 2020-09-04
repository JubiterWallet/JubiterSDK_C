// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/nist256p1.h>

using namespace TW;
using namespace TW::EOS;

void Signer::sign(const PrivateKey& privateKey, Type type, Transaction& transaction) const {
    if (!transaction.isValid()) {
        throw std::invalid_argument("Invalid transaction!");
    }

    // values for Legacy and ModernK1
    TWCurve curve = TWCurveSECP256k1;
    auto canonicalChecker = is_canonical;

    //  Values for ModernR1
    if (type == Type::ModernR1) {
        curve = TWCurveNIST256p1;
        canonicalChecker = nullptr;
    }
    else if (type == Type::Legacy) {
        type = Type::ModernK1;
    }

    const Data result = privateKey.sign(hash(transaction), curve, canonicalChecker);

    transaction.signatures.push_back(Signature(result, type));
}

// JuBiter-defined
/// Verifies the given signature.
bool Signer::verify(const PublicKey& publicKey, Type type, const Transaction& transaction) const noexcept {

    if (!transaction.isValid()) {
        return false;
    }

    // values for Legacy and ModernK1
    TWCurve curve = TWCurveSECP256k1;
    auto canonicalChecker = is_canonical;

    //  Values for ModernR1
    if (type == Type::ModernR1) {
        curve = TWCurveNIST256p1;
        canonicalChecker = nullptr;
    }
    else if (type == Type::Legacy) {
        type = Type::ModernK1;
    }

    bool bSuccess = false;
    for (const auto& signature:transaction.signatures) {
        Data rs(signature.data.size()-1, 0x00);
        std::copy(std::begin(signature.data)+1, std::end(signature.data), std::begin(rs));

        // check if the signature is acceptable or retry
        uint8_t by = 0;
        uint8_t sig[64] = {0x00,};
        std::copy(std::begin(rs), std::end(rs), std::begin(sig));
        if (canonicalChecker && !canonicalChecker(by, sig)) {
            bSuccess = false;
            break;
        }

        bSuccess = publicKey.verify(rs, hash(transaction));
        if (!bSuccess) {
            break;
        }
    }

    return bSuccess;
}

TW::Data Signer::hash(const Transaction& transaction) const noexcept {
    Data hashInput(chainID);
    transaction.serialize(hashInput);

    Data cfdHash(Hash::sha256Size);             // default value for empty cfd
    if (transaction.contextFreeData.size()) {
        cfdHash = Hash::sha256(transaction.contextFreeData);
    }

    append(hashInput, cfdHash);
    return Hash::sha256(hashInput);
}

// canonical check for EOS
int Signer::is_canonical(uint8_t by, uint8_t sig[64]) {
    return !(sig[0] & 0x80)
        && !(sig[0] == 0 && !(sig[1] & 0x80))
        && !(sig[32] & 0x80)
        && !(sig[32] == 0 && !(sig[33] & 0x80));
}
