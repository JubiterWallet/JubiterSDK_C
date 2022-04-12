// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Extrinsic.h"
#include <TrustWalletCore/TWSS58AddressType.h>
#include <map>
#include "utility/util.h"

using namespace TW;
using namespace TW::Polkadot;

static constexpr uint8_t signedBit = 0x80;
static constexpr uint8_t sigTypeEd25519 = 0x00;
static constexpr uint8_t sigTypeSR25519 = 0x01;
static constexpr uint8_t extrinsicFormat = 4;
static constexpr uint32_t multiAddrSpecVersion = 28;
static constexpr uint32_t multiAddrSpecVersionKsm = 2028;

static const std::string balanceTransfer = "Balances.transfer";
static const std::string balanceTransferKeepAlive = "Balances.transfer_keep_alive";
static const std::string balanceTransferAll = "Balances.transfer_all";
static const std::string utilityBatch = "Utility.batch";
static const std::string stakingBond = "Staking.bond";
static const std::string stakingBondExtra = "Staking.bond_extra";
static const std::string stakingUnbond = "Staking.unbond";
static const std::string stakingWithdrawUnbond = "Staking.withdraw_unbonded";
static const std::string stakingPayoutStakers = "Staking.payout_stakers";
static const std::string stakingNominate = "Staking.nominate";
static const std::string stakingChill = "Staking.chill";

// Readable decoded call index can be found from https://polkascan.io
static std::map<const std::string, Data> polkadotCallIndices = {
    {balanceTransfer,       Data{0x05, 0x00}},
    {balanceTransferKeepAlive, Data{0x05, 0x03}},
    {balanceTransferAll,    Data{0x05, 0x04}},
    {utilityBatch,          Data{0x1a, 0x00}},
    {stakingBond,           Data{0x07, 0x00}},
    {stakingBondExtra,      Data{0x07, 0x01}},
    {stakingUnbond,         Data{0x07, 0x02}},
    {stakingWithdrawUnbond, Data{0x07, 0x03}},
    {stakingNominate,       Data{0x07, 0x05}},
    {stakingChill,          Data{0x07, 0x06}},
    {stakingPayoutStakers,  Data{0x07, 0x12}},
};

static std::map<const std::string, Data> kusamaCallIndices = {
    {balanceTransfer,       Data{0x04, 0x00}},
    {balanceTransferKeepAlive, Data{0x04, 0x03}},
    {balanceTransferAll,    Data{0x04, 0x04}},
    {stakingBond,           Data{0x06, 0x00}},
    {stakingBondExtra,      Data{0x06, 0x01}},
    {stakingUnbond,         Data{0x06, 0x02}},
    {stakingWithdrawUnbond, Data{0x06, 0x03}},
    {stakingNominate,       Data{0x06, 0x05}},
    {stakingChill,          Data{0x06, 0x06}},
    {stakingPayoutStakers,  Data{0x06, 0x12}},
};

// modified by JuBiter
static Data getCallIndex(TWSS58AddressType network, const std::string& key) {
    switch (network) {
//    case TWSS58AddressTypeWestend:
    case TWSS58AddressTypePolkadot:
        return polkadotCallIndices[key];
    case TWSS58AddressTypeWestend:
    case TWSS58AddressTypeKusama:
        return kusamaCallIndices[key];
    }
}

bool Extrinsic::encodeRawAccount(TWSS58AddressType network, uint32_t specVersion) {
    if ((network == TWSS58AddressTypePolkadot && specVersion >= multiAddrSpecVersion) ||
        (network == TWSS58AddressTypeKusama && specVersion >= multiAddrSpecVersionKsm) ||
        (network == TWSS58AddressTypeWestend && specVersion >= multiAddrSpecVersion)) {
            return false;
        }
    return true;
}

Data Extrinsic::encodeEraNonceTip() const {
    Data data;
    // era
    append(data, era);
    // nonce
    append(data, encodeCompact(nonce));
    // tip
    append(data, encodeCompact(tip));
    return data;
}

//Data Extrinsic::encodeCall(const Proto::SigningInput& input) {
//    // call index from MetadataV11
//    Data data;
//    auto network = TWSS58AddressType(input.network());
//    if (input.has_balance_call()) {
//        data = encodeBalanceCall(input.balance_call(), network, input.spec_version());
//    //Temporarily default transaction type - Balances Transfer
//    } else if (input.has_staking_call()) {
//        data = encodeStakingCall(input.staking_call(), network, input.spec_version());
//    }
//    return data;
//}

// JuBiter-modified
//Data Extrinsic::encodeBalanceCall(const Proto::Balance& balance, TWSS58AddressType network, uint32_t specVersion) {
Data Extrinsic::encodeBalanceCall(TWSS58AddressType network, uint32_t specVersion, std::string to, std::string val, bool keep_alive) {
    Data data;
//    auto transfer = balance.transfer();
//    auto address = SS58Address(transfer.to_address(), network);
    auto address = SS58Address(to, network);
    // call index
    if ("" == val) {
        append(data, getCallIndex(network, balanceTransferAll));
    }
    else if (keep_alive) {
        append(data, getCallIndex(network, balanceTransferKeepAlive));
    }
    else {
        append(data, getCallIndex(network, balanceTransfer));
    }
    // destination
    append(data, encodeAccountId(address.keyBytes(), encodeRawAccount(network, specVersion)));
    if ("" == val) {
        append(data, keep_alive ? 0x01 : 0x00);
    }
    else {
    // value
//    auto value = load(transfer.value());
    auto value = load(val);
    append(data, encodeCompact(value));
    }
    return data;
}

//Data Extrinsic::encodeBatchCall(const std::vector<Data>& calls, TWSS58AddressType network) {
//    Data data;
//    append(data, getCallIndex(network, utilityBatch));
//    append(data, encodeVector(calls));
//    return data;
//}

// JuBiter-added
Data Extrinsic::encodeStakingCall(TWSS58AddressType network,
                                  //uint32_t specVersion,
                                  const uint32_t type, const std::string& val) {
    Data data;
    switch (type) {
        case 1: {   // BOND_EXTRA
//        case Proto::Staking::kBondExtra: {
//            auto value = load(staking.bond().value());
            auto value = load(val);
            // call index
            append(data, getCallIndex(network, stakingBondExtra));
            // value
            append(data, encodeCompact(value));
//        } break;
        } break;
        case 2: {   // UNBOND
//        case Proto::Staking::kUnbond: {
//            auto value = load(staking.unbond().value());
            auto value = load(val);
            // call index
            append(data, getCallIndex(network, stakingUnbond));
            // value
            append(data, encodeCompact(value));
//        } break;
        } break;
        case 0:     // BOND
        case 3:     // WITHDRAW_UNBONDED
        case 5:     // NOMINATE
        case 6:     // CHILL
        default: {
        } break;
    }

    return data;
}


// JuBiter-added
Data Extrinsic::encodeStakingWithdrawUnbondedCall(TWSS58AddressType network,
                                                  const int32_t slashing_spans) {
    Data data;
//    case Proto::Staking::kWithdrawUnbonded: {
//        auto spans = staking.withdraw_unbonded().slashing_spans();
        // call index
        append(data, getCallIndex(network, stakingWithdrawUnbond));
        // num_slashing_spans
        encode32LE(slashing_spans, data);
//    } break;
    return data;
}


Data Extrinsic::encodeStakingNominateCall(TWSS58AddressType network, uint32_t specVersion,
                                  const std::vector<SS58Address>& accountIds) {
    Data data;
//    case Proto::Staking::kNominate: {
//        std::vector<SS58Address> accountIds;
//        for (auto& n : staking.nominate().nominators()) {
//            accountIds.push_back(SS58Address(n, network));
//        }
        // call index
        append(data, getCallIndex(network, stakingNominate));
        // nominators
        append(data, encodeAccountIds(accountIds, encodeRawAccount(network, specVersion)));
//    } break;
    return data;
}


// JuBiter-added
Data Extrinsic::encodeStakingPayoutStakersCall(TWSS58AddressType network,
                                               const std::vector<uint8_t>& validator_stash, const int32_t era) {
    Data data;
    // call index
    append(data, getCallIndex(network, stakingPayoutStakers));
    // validator_stash
    append(data, validator_stash);
    // era
    encode32LE(era, data);
    return data;
}


Data Extrinsic::encodeStakingCall(TWSS58AddressType network) {
    Data data;
//    case Proto::Staking::kChill:
//        // call index
//        append(data, getCallIndex(network, stakingChill));
//        break;
    return data;
}


// JuBiter-modified
//Data Extrinsic::encodeStakingCall(const Proto::Staking& staking, TWSS58AddressType network, uint32_t specVersion) {
//    Data data;
//    switch (staking.message_oneof_case()) {
//        case Proto::Staking::kBond:
//            {
//                auto address = SS58Address(staking.bond().controller(), byte(network));
//                auto value = load(staking.bond().value());
//                auto reward = byte(staking.bond().reward_destination());
//                // call index
//                append(data, getCallIndex(network, stakingBond));
//                // controller
//                append(data, encodeAccountId(address.keyBytes(), encodeRawAccount(network, specVersion)));
//                // value
//                append(data, encodeCompact(value));
//                // reward destination
//                append(data, reward);
//            }
//            break;
//
//        case Proto::Staking::kBondAndNominate:
//            {
//                // encode call1
//                Data call1;
//                {
//                    auto staking1 = Proto::Staking();
//                    auto bond = staking1.mutable_bond();
//                    bond->set_controller(staking.bond_and_nominate().controller());
//                    bond->set_value(staking.bond_and_nominate().value());
//                    bond->set_reward_destination(staking.bond_and_nominate().reward_destination());
//                    // recursive call
//                    call1 = encodeStakingCall(staking1, network, specVersion);
//                }
//
//                // encode call2
//                Data call2;
//                {
//                    auto staking2 = Proto::Staking();
//                    auto nominate = staking2.mutable_nominate();
//                    for (auto i = 0; i < staking.bond_and_nominate().nominators_size(); ++i) {
//                        nominate->add_nominators(staking.bond_and_nominate().nominators(i));
//                    }
//                    // recursive call
//                    call2 = encodeStakingCall(staking2, network, specVersion);
//                }
//
//                auto calls = std::vector<Data>{call1, call2};
//                data = encodeBatchCall(calls, network);
//            }
//            break;
//
//        case Proto::Staking::kBondExtra:
//            {
//                auto value = load(staking.bond().value());
//                // call index
//                append(data, getCallIndex(network, stakingBondExtra));
//                // value
//                append(data, encodeCompact(value));
//            }
//            break;
//
//        case Proto::Staking::kUnbond:
//            {
//                auto value = load(staking.unbond().value());
//                // call index
//                append(data, getCallIndex(network, stakingUnbond));
//                // value
//                append(data, encodeCompact(value));
//            }
//            break;
//
//        case Proto::Staking::kWithdrawUnbonded:
//            {
//                auto spans = staking.withdraw_unbonded().slashing_spans();
//                // call index
//                append(data, getCallIndex(network, stakingWithdrawUnbond));
//                // num_slashing_spans
//                encode32LE(spans, data);
//            }
//            break;
//
//        case Proto::Staking::kNominate:
//            {
//                std::vector<SS58Address> accountIds;
//                for (auto& n : staking.nominate().nominators()) {
//                    accountIds.push_back(SS58Address(n, network));
//                }
//                // call index
//                append(data, getCallIndex(network, stakingNominate));
//                // nominators
//                append(data, encodeAccountIds(accountIds, encodeRawAccount(network, specVersion)));
//            }
//            break;
//
//        case Proto::Staking::kChill:
//            // call index
//            append(data, getCallIndex(network, stakingChill));
//            break;
//
//        default:
//            break;
//    }
//    return data;
//}

Data Extrinsic::encodePayload() const {
    Data data;
    // call
    append(data, call);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // specVersion
    encode32LE(specVersion, data);
    // transactionVersion
    encode32LE(version, data);
    // genesis hash
    append(data, genesisHash);
    // block hash
    append(data, blockHash);
    return data;
}

// JuBiter-modified
Data Extrinsic::encodeSignature(const PublicKey& signer, const Data& signature, const TWCurve curve) const {
    return encodeSignature(signer.bytes, signature, curve);
}

// JuBiter-defined
Data Extrinsic::encodeSignature(const TW::Data& publicKey, const Data& signature, const TWCurve curve) const {
    Data data;
    // version header
    append(data, Data{extrinsicFormat | signedBit});
    // signer public key
//    append(data, encodeAccountId(signer.bytes, encodeRawAccount(network, specVersion)));
    append(data, encodeAccountId(publicKey, encodeRawAccount(network, specVersion)));
    // signature type
//    append(data, sigTypeEd25519);
    append(data, curve == TWCurve::TWCurveED25519 ? sigTypeEd25519 : sigTypeSR25519);
    // signature
    append(data, signature);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // call
    append(data, call);
    // append length
    encodeLengthPrefix(data);
    return data;
}
