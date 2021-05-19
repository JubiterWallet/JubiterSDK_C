// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <cstdint>
#include "ContractAssetIssue.h"
#include "ContractBalance.h"
//#include "ContractShield.h"
#include "ContractSmart.h"
//#include "ContractVoteAsset.h"
//#include "ContractWitness.h"


#include "pbparse/pb_varint.hpp"
#include "pbparse/pb_length_delimited.hpp"

namespace TW::Tron {

//message Contract {
//  enum ContractType {
//    AccountCreateContract = 0;
//    TransferContract = 1;
//    TransferAssetContract = 2;
//    VoteAssetContract = 3;
//    VoteWitnessContract = 4;
//    WitnessCreateContract = 5;
//    AssetIssueContract = 6;
//    WitnessUpdateContract = 8;
//    ParticipateAssetIssueContract = 9;
//    AccountUpdateContract = 10;
//    FreezeBalanceContract = 11;
//    UnfreezeBalanceContract = 12;
//    WithdrawBalanceContract = 13;
//    UnfreezeAssetContract = 14;
//    UpdateAssetContract = 15;
//    ProposalCreateContract = 16;
//    ProposalApproveContract = 17;
//    ProposalDeleteContract = 18;
//    SetAccountIdContract = 19;
//    CustomContract = 20;
//    CreateSmartContract = 30;
//    TriggerSmartContract = 31;
//    GetContract = 32;
//    UpdateSettingContract = 33;
//    ExchangeCreateContract = 41;
//    ExchangeInjectContract = 42;
//    ExchangeWithdrawContract = 43;
//    ExchangeTransactionContract = 44;
//    UpdateEnergyLimitContract = 45;
//    AccountPermissionUpdateContract = 46;
//    ClearABIContract = 48;
//    UpdateBrokerageContract = 49;
//    ShieldedTransferContract = 51;
//  }
//  ContractType type = 1;
//  google.protobuf.Any parameter = 2;
//  bytes provider = 3;
//  bytes ContractName = 4;
//  int32 Permission_id = 5;
//}
/// Contract point for implementation of Tron coin.
class TransactionContract {

public:
    TransactionContract() {}
    virtual ~TransactionContract() {
        clear();
    }

    void clear() {
        type = 0;
        parameter.Clear();
        provider.clear();
        contractName.clear();
        permission_id = 0;
    }

    void from_internal(const ::protocol::Transaction_Contract& contract);
    ::protocol::Transaction_Contract to_internal() const;

    template<typename U, typename T>
    bool from_parameter(T& contract) {
        U pb;
        bool status = parameter.UnpackTo(&pb);
        if (!status) {
            return status;
        }

        contract.from_internal(pb);

        return true;
    }

    template<typename T>
    static ::google::protobuf::Any to_parameter(const T& contract) {
        ::google::protobuf::Any pb;
        pb.PackFrom(contract.to_internal());

        return pb;
    }

    void deserialize(const Data& o);
    Data serialize();

    virtual size_t parameterValueIndex(const Data& param);
    virtual size_t parameterValueIndex(const size_t offset) const;

private:
    virtual bool calculateOffset();

    virtual pb_varint getType() const;

public:
    virtual pb_length_delimited getParameter();

public:
    // Contract Type.
    int32_t type;
    ::google::protobuf::Any parameter;
    Data provider;
    Data contractName;
    int32_t permission_id;

private:
    size_t paramValueIndex = 0;
};  // class TransactionContract end

::protocol::Transaction_Contract_ContractType to_internal(const int32_t type);

} // namespace TW::Tron
