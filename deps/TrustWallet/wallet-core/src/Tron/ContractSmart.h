// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Contract.hpp"
#include "smart_contract.pb.h"

namespace TW::Tron {

//message SmartContract {
//    message ABI {
//        message Entry {
//            enum EntryType {
//                UnknownEntryType = 0;
//                Constructor = 1;
//                Function = 2;
//                Event = 3;
//                Fallback = 4;
//            }
//            message Param {
//                bool indexed = 1;
//                string name = 2;
//                string type = 3;
//                // SolidityType type = 3;
//            }
//            enum StateMutabilityType {
//                UnknownMutabilityType = 0;
//                Pure = 1;
//                View = 2;
//                Nonpayable = 3;
//                Payable = 4;
//            }
//
//            bool anonymous = 1;
//            bool constant = 2;
//            string name = 3;
//            repeated Param inputs = 4;
//            repeated Param outputs = 5;
//            EntryType type = 6;
//            bool payable = 7;
//            StateMutabilityType stateMutability = 8;
//        }
//        repeated Entry entrys = 1;
//    }
//    bytes origin_address = 1;
//    bytes contract_address = 2;
//    ABI abi = 3;
//    bytes bytecode = 4;
//    int64 call_value = 5;
//    int64 consume_user_resource_percent = 6;
//    string name = 7;
//    int64 origin_energy_limit = 8;
//    bytes code_hash = 9;
//    bytes trx_hash = 10;
//}
class SmartContract {

public:
    SmartContract() {}
    virtual ~SmartContract() {}

    static void deserialize(const Data& o);
}; // cleass SmartContract end


//message CreateSmartContract {
//    bytes owner_address = 1;
//    SmartContract new_contract = 2;
//    int64 call_token_value = 3;
//    int64 token_id = 4;
//}
class CreateSmartContract : public Contract {

public:
    CreateSmartContract() {}
    CreateSmartContract(std::string owner_address,
                        ::protocol::SmartContract new_contract,
                        int64_t call_token_value,
                        int64_t token_id)
        : Contract(owner_address)
        , new_contract(new_contract)
        , call_token_value(call_token_value)
        , token_id(token_id)
    {}
    virtual ~CreateSmartContract() {
        clear();
    }

    void clear() {
        Contract::clear();
    }

    void from_internal(const ::protocol::CreateSmartContract& contract);
    ::protocol::CreateSmartContract to_internal() const;

private:
    ::protocol::SmartContract new_contract;
    int64_t call_token_value;
    int64_t token_id;
}; // class CreateSmartContract end


//message TriggerSmartContract {
//    bytes owner_address = 1;
//    bytes contract_address = 2;
//    int64 call_value = 3;
//    bytes data = 4;
//    int64 call_token_value = 5;
//    int64 token_id = 6;
//}
class TriggerSmartContract : public Contract {

public:
    TriggerSmartContract() {}
    TriggerSmartContract(std::string owner_address,
                         std::string contract_address,
                         int64_t call_value,
                         Data data,
                         int64_t call_token_value,
                         int64_t token_id)
        : Contract(owner_address)
        , contract_address(contract_address)
        , call_value(call_value)
        , data(data)
        , call_token_value(call_token_value)
        , token_id(token_id)
    {}
    virtual ~TriggerSmartContract() {
        clear();
    }

    void clear() {
        Contract::clear();
        contract_address = "";
        call_value = 0;
        data.clear();
        call_token_value = 0;
        token_id = 0;
    }

    void from_internal(const ::protocol::TriggerSmartContract& contract);
    ::protocol::TriggerSmartContract to_internal() const;

    virtual bool calculateOffset() override;

    virtual TW::Data contractAddressSize() const;
    virtual TW::Data contractAddressOffset(const size_t offset) const;

    virtual TW::Data callValueSize() const;
    virtual TW::Data callValueOffset(const size_t offset) const;

    virtual TW::Data dataSize() const;
    virtual TW::Data dataOffset(const size_t offset) const;
    virtual pb_length_delimited getData() const;

private:
    virtual pb_length_delimited getContractAddress() const;
    virtual pb_varint getCallValue() const;
    virtual pb_varint getCallTokenValue() const;

    virtual size_t contractAddressIndex(const size_t offset) const;
    virtual size_t       callValueIndex(const size_t offset) const;
    virtual size_t            dataIndex(const size_t offset) const;

private:
    std::string contract_address;
    int64_t call_value;
    Data data;
    int64_t call_token_value;
    int64_t token_id;

    size_t contractAddrSize = 0;
    size_t contractAddrIndex = 0;

    size_t callVSize = 0;
    size_t callVIndex = 0;

    size_t dSize = 0;
    size_t dIndex = 0;
//    size_t callTokenVIndex = 0;
//    size_t tokenIDIndex = 0;
};  // class TriggerSmartContract end


} // namespace TW::Tron
