// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/nist256p1.h>
#include <TrezorCrypto/secp256k1.h>

using namespace TW;
using namespace TW::EOS;

void Signer::sign(const PrivateKey& privateKey, Type type, Transaction& transaction) const {
    if (!transaction.isValid()) {
        throw std::invalid_argument("Invalid transaction!");
    }

    // values for Legacy and ModernK1
    TWCurve curve = TWCurveSECP256k1;
    auto canonicalChecker = isCanonical;

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

    TW::Data uncompressed(PublicKey::secp256k1ExtendedSize);
    if (1 != ecdsa_uncompress_pubkey(&secp256k1, &publicKey.bytes[0], &uncompressed[0])) {
        return false;
    }

    // values for Legacy and ModernK1
    TWCurve curve = TWCurveSECP256k1;
    auto canonicalChecker = isCanonical;

    //  Values for ModernR1
    if (type == Type::ModernR1) {
        curve = TWCurveNIST256p1;
        canonicalChecker = nullptr;
    }
    else if (type == Type::Legacy) {
        type = Type::ModernK1;
    }

    bool bSuccess = true;
    for (const auto& signature:transaction.signatures) {
        bSuccess = publicKey.verify(signature.data, hash(transaction), canonicalChecker);
        if (!bSuccess) {
            break;
        }
    }

    return bSuccess;
}

// JuBiter-defined
/// Recovers the given signature.
bool Signer::recover(PublicKey& publicKey, Type type, Transaction& transaction) noexcept {

    if (!transaction.isValid()) {
        return false;
    }

    TW::Data uncompressed(PublicKey::secp256k1ExtendedSize);
    if (1 != ecdsa_uncompress_pubkey(&secp256k1, &publicKey.bytes[0], &uncompressed[0])) {
        return false;
    }

    // values for Legacy and ModernK1
    TWCurve curve = TWCurveSECP256k1;
    auto canonicalChecker = isCanonical;

    //  Values for ModernR1
    if (type == Type::ModernR1) {
        curve = TWCurveNIST256p1;
        canonicalChecker = nullptr;
    }
    else if (type == Type::Legacy) {
        type = Type::ModernK1;
    }

    bool bSuccess = true;
    for (int i=0; i<transaction.signatures.size(); ++i) {
        TW::Data recoverSign = transaction.signatures[i].data;

        bSuccess = publicKey.recover(recoverSign, hash(transaction), canonicalChecker);
        if (!bSuccess) {
            break;
        }

        transaction.signatures[i].data = recoverSign;
    }

    return bSuccess;
}

TW::Data Signer::hash(const Transaction& transaction) const noexcept {
    Data hashInput(chainID);
    transaction.serialize(hashInput);

    Data cfdHash(Hash::sha256Size); // default value for empty cfd
    if (transaction.contextFreeData.size()) {
        cfdHash = Hash::sha256(transaction.contextFreeData);
    }

    append(hashInput, cfdHash);
    return Hash::sha256(hashInput);
}

// canonical check for EOS
int Signer::isCanonical(uint8_t by, uint8_t sig[64]) {
    return !(sig[0] & 0x80) 
        && !(sig[0] == 0 && !(sig[1] & 0x80))
        && !(sig[32] & 0x80)
        && !(sig[32] == 0 && !(sig[33] & 0x80));
}
