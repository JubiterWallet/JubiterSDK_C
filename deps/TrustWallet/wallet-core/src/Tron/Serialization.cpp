// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"
#include "../HexCoding.h"
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

using json = nlohmann::json;

namespace TW::Tron {


string resourceCodeName(::protocol::ResourceCode type) {
    string name = "";
    switch (type) {
    case ::protocol::ResourceCode::BANDWIDTH:
        name = "BANDWIDTH";
        break;
    case protocol::ResourceCode::ENERGY:
        name = "ENERGY";
        break;
    default:
        break;
    }
    return name;
}


json valueJSON(const ::protocol::TransferContract& contract) {
    json valueJSON;
    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());
    valueJSON["to_address"] = TW::Tron::Address::fromHex(contract.to_address());
    valueJSON["amount"] = contract.amount();

    return valueJSON;
}


json valueJSON(const ::protocol::TransferAssetContract& contract) {
    json valueJSON;
    valueJSON["asset_name"] = contract.asset_name();
    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());
    valueJSON["to_address"] = TW::Tron::Address::fromHex(contract.to_address());
    valueJSON["amount"] = contract.amount();

    return valueJSON;
}


//json valueJSON(const ::protocol::VoteAssetContract& contract) {
//    json valueJSON;
//
//    vector<string> vote_address;
//    for (int i=0; i<contract.vote_address_size(); ++i) {
//        vote_address.push_back(TW::Tron::Address::fromHex(contract.vote_address(i)));
//    }
//
//    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());
//    valueJSON["vote_address"] = vote_address;
//    valueJSON["support"] = contract.support();
//    valueJSON["count"] = contract.count();
//
//    return valueJSON;
//}
//
//
//json voteJSON(const ::protocol::VoteWitnessContract_Vote& vote) {
//    json voteJSON;
//    voteJSON["vote_address"] = TW::Tron::Address::fromHex(vote.vote_address());
//    voteJSON["vote_count"] = vote.vote_count();
//
//    return voteJSON;
//}
//
//
//json valueJSON(const ::protocol::VoteWitnessContract& contract) {
//    json valueJSON;
//
//    vector<json> votes;
//    for (int i=0; i<contract.votes_size(); ++i) {
//        votes.push_back(voteJSON(contract.votes(i)));
//    }
//
//    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());
//    valueJSON["votes"] = votes;
//    valueJSON["support"] = contract.support();
//
//    return valueJSON;
//}
//
//
json valueJSON(const ::protocol::FreezeBalanceContract& contract) {
    json valueJSON;
    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());
    valueJSON["frozen_balance"] = contract.frozen_balance();
    valueJSON["frozen_duration"] = contract.frozen_duration();
    valueJSON["resource"] = resourceCodeName(contract.resource());
    valueJSON["receiver_address"] = TW::Tron::Address::fromHex(contract.receiver_address());

    return valueJSON;
}


json valueJSON(const ::protocol::UnfreezeBalanceContract& contract) {
    json valueJSON;
    valueJSON["owner_address"] = contract.owner_address();
    valueJSON["resource"] = resourceCodeName(contract.resource());
    valueJSON["receiver_address"] = contract.receiver_address();

    return valueJSON;
}


json valueJSON(const ::protocol::WithdrawBalanceContract& contract) {
    json valueJSON;
    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());

    return valueJSON;
}


json valueJSON(const ::protocol::UnfreezeAssetContract& contract) {
    json valueJSON;
    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());

    return valueJSON;
}


json valueJSON(const ::protocol::TriggerSmartContract& contract) {
    json valueJSON;
    valueJSON["owner_address"] = TW::Tron::Address::fromHex(contract.owner_address());
    valueJSON["contract_address"] = TW::Tron::Address::fromHex(contract.contract_address());
    valueJSON["data"] = hex(contract.data());
    if (contract.call_value() > 0) {
        valueJSON["call_value"] = contract.call_value();
    }
    if (contract.call_token_value() > 0) {
        valueJSON["call_token_value"] = contract.call_token_value();
    }
    if (contract.token_id() > 0) {
        valueJSON["token_id"] = contract.token_id();
    }

    return valueJSON;
}


json parameterJSON(const ::google::protobuf::Any &parameter, protocol::Transaction_Contract_ContractType type) {
    json paramJSON;
    paramJSON["type_url"] = parameter.type_url();

    switch (type) {
    case ::protocol::Transaction_Contract_ContractType_TransferContract: {
        ::protocol::TransferContract contract;
        parameter.UnpackTo(&contract);
        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_TransferAssetContract: {
        ::protocol::TransferAssetContract contract;
        parameter.UnpackTo(&contract);
        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_VoteAssetContract: {
//        ::protocol::VoteAssetContract contract;
//        parameter.UnpackTo(&contract);
//        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_VoteWitnessContract: {
//        ::protocol::VoteWitnessContract contract;
//        parameter.UnpackTo(&contract);
//        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_FreezeBalanceContract: {
        ::protocol::FreezeBalanceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_UnfreezeBalanceContract: {
        ::protocol::UnfreezeBalanceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_WithdrawBalanceContract: {
        ::protocol::WithdrawBalanceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_UnfreezeAssetContract: {
        ::protocol::UnfreezeAssetContract contract;
        parameter.UnpackTo(&contract);
        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_TriggerSmartContract: {
        ::protocol::TriggerSmartContract contract;
        parameter.UnpackTo(&contract);
        paramJSON["value"] = valueJSON(contract);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType_AccountCreateContract:
    default:
        break;
    }

    return paramJSON;
}


json contractJSON(const ::protocol::Transaction_Contract &contract) {
    json contractJSON;
    std::string type = "";
    std::string typeURL = contract.parameter().type_url();
    std::size_t found = typeURL.find_last_of(".");
    if (found!=std::string::npos) {
        type = typeURL.substr(found+1);
    }
    contractJSON["type"] = type;
    contractJSON["parameter"] = parameterJSON(contract.parameter(), contract.type());
    return contractJSON;
}


json raw_dataJSON(const ::protocol::Transaction_raw &raw) {
    json raw_dataJSON;

    raw_dataJSON["ref_block_bytes"] = hex(raw.ref_block_bytes());
    raw_dataJSON["ref_block_hash"]  = hex(raw.ref_block_hash());
    if (raw.ref_block_num() > 0) {
        raw_dataJSON["ref_block_num"] = raw.ref_block_num();
    }
    if (raw.fee_limit() > 0) {
        raw_dataJSON["fee_limit"] = raw.fee_limit();
    }
    raw_dataJSON["timestamp"] = raw.timestamp();
    raw_dataJSON["expiration"] = raw.expiration();
    if (0 < raw.contract_size()) {
        raw_dataJSON["contract"] = json::array({ contractJSON(raw.contract(0)) });
    }

    return raw_dataJSON;
}


json transactionJSON(const ::protocol::Transaction_raw& raw, const TW::Data& txID, const TW::Data& signature) {
    json transactionJSON;
    transactionJSON["raw_data"] = raw_dataJSON(raw);
    transactionJSON["txID"] = hex(txID);
    transactionJSON["signature"] = json::array({ hex(signature) });

    return transactionJSON;
}


} // namespace TW::Tron end
