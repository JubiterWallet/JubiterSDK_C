// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"
#include "uint256.h"
#include  "ScaleCodec.h"
#include <TrustWalletCore/TWCurve.h>

namespace TW::Polkadot {

// ExtrinsicV4
class Extrinsic {
  public:
    Data blockHash;
    Data genesisHash;
    uint64_t nonce;
    // Runtime spec version
    uint32_t specVersion;
    // transaction version
    uint32_t version;
    // balances::TakeFees
    uint256_t tip;
    // encoded Era data
    Data era;
    // encoded Call data
    Data call;
    // network
    TWSS58AddressType network;

    // JuBiter-defined
    Extrinsic(Data blockHash, Data genesisHash, uint64_t nonce, uint32_t specVersion, uint32_t transaction_version,  std::string value, SS58Address to, std::string tip, TWSS58AddressType netWork, uint64_t blockNumber, uint64_t eraPeriod)
    : blockHash(blockHash.begin(), blockHash.end())
    , genesisHash(genesisHash.begin(),genesisHash.end())
    , nonce(nonce)
    , specVersion(specVersion)
    , version(transaction_version)
    , tip(load(tip)) {
        if(blockNumber && eraPeriod) {
            era = encodeEra(blockNumber, eraPeriod);
        } else {
            era = encodeCompact(0);
        }
        network = netWork;
        call = encodeCall(network, specVersion, to.string(), value);
     }

//    Extrinsic(const Proto::SigningInput& input)
//        : blockHash(input.block_hash().begin(), input.block_hash().end())
//        , genesisHash(input.genesis_hash().begin(), input.genesis_hash().end())
//        , nonce(input.nonce())
//        , specVersion(input.spec_version())
//        , version(input.transaction_version())
//        , tip(load(input.tip())) {
//        if (input.has_era()) {
//            era = encodeEra(input.era().block_number(), input.era().period());
//        } else {
//          // immortal era
//          era = encodeCompact(0);
//        }
//        network = TWSS58AddressType(input.network());
//        call = encodeCall(input);
//    }
    
    // JuBiter-defined
    static Data encodeCall(TWSS58AddressType netWork,uint32_t specVersion, std::string to, std::string value);
//    static Data encodeCall(const Proto::SigningInput& input);
    
    // Payload to sign.
    Data encodePayload() const;
    // Encode final data with signer public key and signature.
//    Data encodeSignature(const PublicKey& signer, const Data& signature) const;
    // JuBiter -defined
    Data encodeSignature(const TW::Data& publicKey, const Data& signature, const TWCurve curve) const;
  protected:
    
    static bool encodeRawAccount(TWSS58AddressType network, uint32_t specVersion);
    // JuBiter-defined
    static Data encodeBalanceCall(TWSS58AddressType network, uint32_t specVersion, std::string to, std::string Value);
    
//    static Data encodeBalanceCall(const Proto::Balance& balance, TWSS58AddressType network, uint32_t specVersion);
//    static Data encodeStakingCall(const Proto::Staking& staking, TWSS58AddressType network, uint32_t specVersion);
    static Data encodeBatchCall(const std::vector<Data>& calls, TWSS58AddressType network);
    Data encodeEraNonceTip() const;
};

} // namespace TW::Polkadot
