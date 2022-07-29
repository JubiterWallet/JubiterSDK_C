// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "Serialization.h"
#include <nlohmann/json.hpp>

#include "Transaction.h"

#include <chrono>
#include <cassert>

using namespace std::chrono;

namespace TW::Tron {
const std::string TRANSFER_TOKEN_FUNCTION = "0xa9059cbb";

size_t base58Capacity = 128;


//
//protocol_VoteAssetContract to_internal(const VoteAssetContract& voteContract) {
//    auto internal = protocol_VoteAssetContract();
//    const auto ownerAddress = Base58::bitcoin.decodeCheck(voteContract.owner_address);
//
////    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
////    internal.set_support(voteContract.support());
////    internal.set_count(voteContract.count());
//    for(int i = 0; i < voteContract.votes.size(); i++) {
//        auto voteAddress = Base58::bitcoin.decodeCheck(voteContract.votes[i].vote_address);
////        internal.add_vote_address(voteAddress.data(), voteAddress.size());
//    }
//
//    return internal;
//}
//
//protocol_VoteWitnessContract to_internal(const VoteWitnessContract& voteContract) {
//    auto internal = protocol_VoteWitnessContract();
//    const auto ownerAddress = Base58::bitcoin.decodeCheck(voteContract.owner_address);
//
////    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
////    internal.set_support(voteContract.support());
//    for(int i = 0; i < voteContract.votes.size(); i++) {
//        auto voteAddress = Base58::bitcoin.decodeCheck(voteContract.votes[i].vote_address);
////        auto vote = internal.add_votes();
////
////        vote->set_vote_address(voteAddress.data(), voteAddress.size());
////        vote->set_vote_count(voteContract.votes(i).vote_count());
//    }
//
//    return internal;
//}
//
//protocol_WithdrawBalanceContract to_internal(const WithdrawBalanceContract& withdrawContract) {
//    auto internal = protocol_WithdrawBalanceContract();
//    const auto ownerAddress = Base58::bitcoin.decodeCheck(withdrawContract.owner_address);
//
////    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
//
//    return internal;
//}
//
//protocol_TriggerSmartContract to_internal(const TriggerSmartContract& triggerSmartContract) {
//    auto internal = protocol_TriggerSmartContract();
//    const auto ownerAddress = Base58::bitcoin.decodeCheck(triggerSmartContract.owner_address);
//    const auto contractAddress = Base58::bitcoin.decodeCheck(triggerSmartContract.contract_address);
//
////    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
////    internal.set_contract_address(contractAddress.data(), contractAddress.size());
////    internal.set_call_value(triggerSmartContract.call_value());
////    internal.set_data(triggerSmartContract.data().data(), triggerSmartContract.data().size());
////    internal.set_call_token_value(triggerSmartContract.call_token_value());
////    internal.set_token_id(triggerSmartContract.token_id());
//
//    return internal;
//}
//
//protocol_TriggerSmartContract to_internal(const TransferTRC20Contract& transferTrc20Contract) {
//    auto toAddress = Base58::bitcoin.decodeCheck(transferTrc20Contract.to_address);
////    // amount is 256 bits, big endian
////    Data amount = data(transferTrc20Contract.amount());
////
////    // Encode smart contract call parameters
////    auto contract_params = parse_hex(TRANSFER_TOKEN_FUNCTION);
////    pad_left(toAddress, 32);
////    pad_left(amount, 32);
////    append(contract_params, toAddress);
////    append(contract_params, amount);
////
//    auto triggerSmartContract = TriggerSmartContract();
////    triggerSmartContract.set_owner_address(transferTrc20Contract.owner_address());
////    triggerSmartContract.set_contract_address(transferTrc20Contract.contract_address());
////    triggerSmartContract.set_data(contract_params.data(), contract_params.size());
//
//    return to_internal(triggerSmartContract);
//}

///// Converts an external BlockHeader to an internal one used for signing.
//protocol::BlockHeader to_internal(const Proto::BlockHeader& header) {
//    auto internal = protocol::BlockHeader();
//
//    internal.mutable_raw_data()->set_timestamp(header.timestamp());
//    internal.mutable_raw_data()->set_tx_trie_root(header.tx_trie_root());
//    internal.mutable_raw_data()->set_parent_hash(header.parent_hash());
//    internal.mutable_raw_data()->set_number(header.number());
//    internal.mutable_raw_data()->set_witness_address(header.witness_address());
//    internal.mutable_raw_data()->set_version(header.version());
//
//    return internal;
//}
//
//Data getBlockHash(const protocol::BlockHeader& header) {
//    const auto data = header.raw_data().SerializeAsString();
//    return Hash::sha256(data);
//}
//
//void setBlockReference(const Proto::Transaction& transaction, protocol::Transaction& internal) {
//    const auto blockHash = getBlockHash(to_internal(transaction.block_header()));
//    assert(blockHash.size() > 15);
//    internal.mutable_raw_data()->set_ref_block_hash(blockHash.data() + 8, 8);
//
//    const auto blockHeight = transaction.block_header().number();
//    auto heightData = Data();
//    encode64LE(blockHeight, heightData);
//    std::reverse(heightData.begin(), heightData.end());
//    internal.mutable_raw_data()->set_ref_block_bytes(heightData.data() + heightData.size() - 2, 2);
//}
//
//Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
//    auto internal = protocol::Transaction();
//    auto output = Proto::SigningOutput();
//
//    if (input.transaction().has_transfer()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_TransferContract);
//
//        auto transfer = to_internal(input.transaction().transfer());
//        google::protobuf::Any any;
//        any.PackFrom(transfer);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_transfer_asset()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_TransferAssetContract);
//
//        auto transfer = to_internal(input.transaction().transfer_asset());
//        google::protobuf::Any any;
//        any.PackFrom(transfer);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_freeze_balance()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_FreezeBalanceContract);
//
//        auto freeze_balance = to_internal(input.transaction().freeze_balance());
//        google::protobuf::Any any;
//        any.PackFrom(freeze_balance);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_unfreeze_balance()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_UnfreezeBalanceContract);
//
//        auto unfreeze_balance = to_internal(input.transaction().unfreeze_balance());
//        google::protobuf::Any any;
//        any.PackFrom(unfreeze_balance);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_unfreeze_asset()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_UnfreezeAssetContract);
//
//        auto unfreeze_asset = to_internal(input.transaction().unfreeze_asset());
//        google::protobuf::Any any;
//        any.PackFrom(unfreeze_asset);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_vote_asset()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_VoteAssetContract);
//
//        auto vote_asset = to_internal(input.transaction().vote_asset());
//        google::protobuf::Any any;
//        any.PackFrom(vote_asset);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_vote_witness()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_VoteWitnessContract);
//
//        auto vote_witness = to_internal(input.transaction().vote_witness());
//        google::protobuf::Any any;
//        any.PackFrom(vote_witness);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_withdraw_balance()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_WithdrawBalanceContract);
//
//        auto withdraw = to_internal(input.transaction().withdraw_balance());
//        google::protobuf::Any any;
//        any.PackFrom(withdraw);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_trigger_smart_contract()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_TriggerSmartContract);
//
//        auto trigger_smart_contract = to_internal(input.transaction().trigger_smart_contract());
//        google::protobuf::Any any;
//        any.PackFrom(trigger_smart_contract);
//        *contract->mutable_parameter() = any;
//    } else if (input.transaction().has_transfer_trc20_contract()) {
//        auto contract = internal.mutable_raw_data()->add_contract();
//        contract->set_type(protocol::Transaction_Contract_ContractType_TriggerSmartContract);
//
//        auto trigger_smart_contract = to_internal(input.transaction().transfer_trc20_contract());
//        google::protobuf::Any any;
//        any.PackFrom(trigger_smart_contract);
//        *contract->mutable_parameter() = any;
//    }
//
//    // Get default timestamp and expiration
//    const uint64_t now = duration_cast< milliseconds >(
//            system_clock::now().time_since_epoch()
//    ).count();
//    const uint64_t timestamp = input.transaction().timestamp() == 0
//            ? now
//            : input.transaction().timestamp();
//    const uint64_t expiration = input.transaction().expiration() == 0
//            ? timestamp + 10 * 60 * 60 * 1000 // 10 hours
//            : input.transaction().expiration();
//
//    internal.mutable_raw_data()->set_timestamp(timestamp);
//    internal.mutable_raw_data()->set_expiration(expiration);
//    internal.mutable_raw_data()->set_fee_limit(input.transaction().fee_limit());
//    setBlockReference(input.transaction(), internal);
//
//    output.set_ref_block_bytes(internal.raw_data().ref_block_bytes());
//    output.set_ref_block_hash(internal.raw_data().ref_block_hash());
//
//    const auto serialized = internal.raw_data().SerializeAsString();
//    const auto hash = Hash::sha256(Data(serialized.begin(), serialized.end()));
//
//    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
//    const auto signature = key.sign(hash, TWCurveSECP256k1);
//
//    const auto json = transactionJSON(internal, hash, signature).dump();
//
//    output.set_id(hash.data(), hash.size());
//    output.set_signature(signature.data(), signature.size());
//    output.set_json(json.data(), json.size());
//
//    return output;
//}

bool Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept {

    const auto serialized = transaction.raw_data.serialize();
    const auto hash = Hash::sha256(Data(serialized.begin(), serialized.end()));

    transaction.txID = hash;
    Data signature = privateKey.sign(hash, TWCurveSECP256k1);
    if (0 > signature.size()) {
        return false;
    }
    transaction.signature = signature;

    return true;
}


// JuBiter-defined
/// Verifies the given signature.
bool Signer::verify(const PublicKey& publicKey, Transaction& transaction) const noexcept {
    Transaction tempTx(transaction);
    tempTx.signature.clear();
    auto serialized = tempTx.raw_data.serialize();
    const auto hash = Hash::sha256(Data(serialized.begin(), serialized.end()));

//    if (!publicKey.verify(transaction.signature, hash)) {
//        return false;
//    }

    if (Address(publicKey.recover(transaction.signature, hash)).string() != Address(publicKey.extended()).string()) {
        return false;
    }

    transaction.txID = hash;

    return true;
}


// JuBiter-defined
/// Computes the transaction hash.
Data Signer::hash(const Transaction& transaction) const noexcept {
    Transaction tempTx(transaction);
    const auto serialized = tempTx.raw_data.serialize();
    return Hash::sha256(Data(serialized.begin(), serialized.end()));
}


// JuBiter-defined
/// Signs the given bytestring.
bool Signer::sign(const PrivateKey& privateKey, Bytestring& bytestring) const noexcept {

    const auto h = hash(bytestring);

    Data signature = privateKey.sign(h, TWCurveSECP256k1);
    if (0 > signature.size()) {
        return false;
    }
    bytestring.signature = signature;

    return true;
}


// JuBiter-defined
/// Verifies the given signature.
bool Signer::verify(const PublicKey& publicKey, Bytestring& bytestring) const noexcept {
    Bytestring tempBys(bytestring);
    const auto serialized = tempBys.to_internal();
    const auto h = hash(tempBys);

    if (Address(publicKey.recover(bytestring.signature, h)).string() != Address(publicKey.extended()).string()) {
        return false;
    }

    return true;
}


// JuBiter-defined
/// Computes the transaction hash.
Data Signer::hash(const Bytestring& bytestring) const noexcept {
    Bytestring tempBys(bytestring);
    const auto serialized = tempBys.to_internal();
    return Hash::keccak256(&serialized[0], serialized.size());
}
} // namespace TW::Tron end
