// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Hash.h"
#include "../PrivateKey.h"

using namespace TW;
using namespace TW::Polkadot;

static constexpr size_t hashTreshold = 256;

// JuBiter-modified
//Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
Data Signer::sign(const Data& private_key, const Extrinsic& extrinsic) noexcept {
//    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto privateKey = PrivateKey(private_key);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
//    auto extrinsic = Extrinsic(input);
    auto payload = extrinsic.encodePayload();

    // check if need to hash
    if (payload.size() > hashTreshold) {
        payload = Hash::blake2b(payload, 32);
    }
    auto signature = privateKey.sign(payload, TWCurveED25519);
    if (!publicKey.verify(signature, payload)) {
        return {};
    }
//    auto encoded = extrinsic.encodeSignature(publicKey, signature);
    auto encoded = extrinsic.encodeSignature(publicKey, signature, TWCurveED25519);

//    auto protoOutput = Proto::SigningOutput();
//    protoOutput.set_encoded(encoded.data(), encoded.size());
//    return protoOutput;
    return encoded;
}

// JuBiter-defined
/// Verifies the given signature.
bool Signer::verify(const Data& public_key, const Extrinsic& extrinsic, const Data& signature) noexcept {
    auto publicKey = PublicKey(public_key, TWPublicKeyTypeED25519);
    auto payload = extrinsic.encodePayload();

    return publicKey.verify(signature, payload);
}
