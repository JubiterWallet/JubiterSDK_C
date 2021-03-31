// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionContract.hpp"
#include "AccountId.h"

#include "HexCoding.h"


using namespace std;


namespace TW::Tron {


::protocol::Transaction_Contract_ContractType to_internal(const int32_t type) {

    ::protocol::Transaction_Contract_ContractType encode;
    switch (type) {
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountCreateContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountCreateContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferAssetContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferAssetContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_VoteAssetContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_VoteAssetContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_VoteWitnessContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_VoteWitnessContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WitnessCreateContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WitnessCreateContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AssetIssueContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AssetIssueContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WitnessUpdateContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WitnessUpdateContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ParticipateAssetIssueContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ParticipateAssetIssueContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountUpdateContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountUpdateContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_FreezeBalanceContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_FreezeBalanceContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UnfreezeBalanceContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UnfreezeBalanceContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WithdrawBalanceContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WithdrawBalanceContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UnfreezeAssetContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UnfreezeAssetContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateAssetContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateAssetContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalCreateContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalCreateContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalApproveContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalApproveContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalDeleteContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalDeleteContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_SetAccountIdContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_SetAccountIdContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_CustomContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_CustomContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_CreateSmartContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_CreateSmartContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TriggerSmartContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TriggerSmartContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_GetContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_GetContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateSettingContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateSettingContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeCreateContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeCreateContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeInjectContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeInjectContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeWithdrawContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeWithdrawContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeTransactionContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeTransactionContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateEnergyLimitContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateEnergyLimitContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountPermissionUpdateContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountPermissionUpdateContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ClearABIContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ClearABIContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateBrokerageContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateBrokerageContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ShieldedTransferContract:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ShieldedTransferContract;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_Transaction_Contract_ContractType_INT_MIN_SENTINEL_DO_NOT_USE_:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_Transaction_Contract_ContractType_INT_MIN_SENTINEL_DO_NOT_USE_;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_Transaction_Contract_ContractType_INT_MAX_SENTINEL_DO_NOT_USE_:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_Transaction_Contract_ContractType_INT_MAX_SENTINEL_DO_NOT_USE_;
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferContract:
    default:
        encode = ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferContract;
        break;
    }

    return encode;
}


void TransactionContract::from_internal(const ::protocol::Transaction_Contract& contract) {

    type = contract.type();
    if (contract.has_parameter()) {
        parameter = contract.parameter();
    }
    if (0 < contract.provider().size()) {
        provider.resize(contract.provider().size());
        std::copy(std::begin(contract.provider()), std::end(contract.provider()), provider.begin());
    }
    if (0 < contract.contractname().size()) {
        contractName.resize(contract.contractname().size());
        std::copy(std::begin(contract.contractname()), std::end(contract.contractname()), contractName.begin());
    }
    permission_id = contract.permission_id();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::Transaction_Contract TransactionContract::to_internal() const {

    ::protocol::Transaction_Contract encode;

    encode.clear_provider();
    if (0 < provider.size()) {
        *encode.mutable_provider() = {provider.begin(), provider.end()};
    }

    encode.clear_contractname();

    if (0 < contractName.size()) {
        *encode.mutable_contractname() = {contractName.begin(), contractName.end()};
    }

    encode.clear_parameter();
    if (0 < parameter.ByteSizeLong()) {
        *encode.mutable_parameter() = parameter;
    }

    encode.clear_type();
    encode.set_type(TW::Tron::to_internal(type));

    encode.clear_permission_id();
    if (0 < permission_id) {
        encode.set_permission_id(permission_id);
    }

    return encode;
}


void TransactionContract::deserialize(const Data& o) {

    ::protocol::Transaction_Contract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data TransactionContract::serialize() {

    auto data = Data();

    ::protocol::Transaction_Contract encode = to_internal();

    size_t szSize = encode.ByteSizeLong();
    auto o = Data(szSize);
    bool status = encode.SerializeToArray(&o[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(o), std::end(o), std::begin(data));
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }

    return data;
}


size_t TransactionContract::parameterValueIndex(const size_t offset) const {

    return (paramValueIndex + offset);
}


size_t TransactionContract::parameterValueIndex(const Data& param) {

    size_t paramValueOffset = 0;

    Data contract = param;
    Data::iterator it = contract.begin();
    Data o;
    switch (type) {
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferContract:
    {
        TransferContract transfer;
        if (!from_parameter<::protocol::TransferContract>(transfer)) {
            o.clear();
            break;
        }
        o = Contract::serialize<::protocol::TransferContract>(transfer);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TransferAssetContract:
    {
        TransferAssetContract transferAsset;
        if (!from_parameter<::protocol::TransferAssetContract>(transferAsset)) {
            o.clear();
            break;
        }
        o = Contract::serialize<::protocol::TransferAssetContract>(transferAsset);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_VoteAssetContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_VoteWitnessContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WitnessCreateContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AssetIssueContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WitnessUpdateContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ParticipateAssetIssueContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountUpdateContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_FreezeBalanceContract:
    {
        FreezeBalanceContract freezeBalance;
        if (!from_parameter<::protocol::FreezeBalanceContract>(freezeBalance)) {
            o.clear();
            break;
        }
        o = Contract::serialize<::protocol::FreezeBalanceContract>(freezeBalance);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UnfreezeBalanceContract:
    {
        UnfreezeBalanceContract unfreezeBalance;
        if (!from_parameter<::protocol::UnfreezeBalanceContract>(unfreezeBalance)) {
            o.clear();
            break;
        }
        o = Contract::serialize<::protocol::UnfreezeBalanceContract>(unfreezeBalance);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_WithdrawBalanceContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UnfreezeAssetContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateAssetContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalCreateContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalApproveContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ProposalDeleteContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_SetAccountIdContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_CustomContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_CreateSmartContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_TriggerSmartContract:
    {
        TriggerSmartContract triggerSmart;
        if (!from_parameter<::protocol::TriggerSmartContract>(triggerSmart)) {
            o.clear();
            break;
        }
        o = Contract::serialize<::protocol::TriggerSmartContract>(triggerSmart);
        break;
    }
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_GetContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateSettingContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeCreateContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeInjectContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeWithdrawContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ExchangeTransactionContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateEnergyLimitContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountPermissionUpdateContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ClearABIContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_UpdateBrokerageContract:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_ShieldedTransferContract:
    {
//        ShieldedTransferContract shieldedTransfer;
//        if (!from_parameter(shieldedTransfer)) {
//            o.clear();
//            break;
//        }
//        o = shieldedTransfer.serialize();
        break;
    }
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_Transaction_Contract_ContractType_INT_MIN_SENTINEL_DO_NOT_USE_:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_Transaction_Contract_ContractType_INT_MAX_SENTINEL_DO_NOT_USE_:
        break;
    case ::protocol::Transaction_Contract_ContractType::Transaction_Contract_ContractType_AccountCreateContract:
        break;
    default:
        break;
    }

    if (0 < o.size()) {
        it = std::search(contract.begin(), contract.end(), o.begin(), o.end());
        paramValueOffset = it - contract.begin();
    }

    return paramValueOffset;
}


bool TransactionContract::calculateOffset() {

    pb_varint pbType = getType();
    pb_length_delimited pbParameter = getParameter();
    if (        !pbType.isValid()
        || !pbParameter.isValid()
        ) {
        return false;
    }
    size_t szType = pbType.size();
    Data o = pbParameter.serialize();

    paramValueIndex += szType;
    paramValueIndex += parameterValueIndex(o);

    return true;
}


pb_varint TransactionContract::getType() const {

    return pb_varint(::protocol::Transaction_Contract::kTypeFieldNumber,
                     WireFormatLite::FieldType::TYPE_ENUM,
                     type);
}


pb_length_delimited TransactionContract::getParameter() const {

    pb_length_delimited pb;

    size_t szSize = parameter.ByteSizeLong();
    auto o = Data(szSize);
    bool status = parameter.SerializeToArray(&o[0], (int)szSize);
    if (!status) {
        return pb;
    }

    return pb_length_delimited(::protocol::Transaction_Contract::kParameterFieldNumber,
                               WireFormatLite::FieldType::TYPE_MESSAGE,
                               o);
}


} // namespace TW::Tron end
