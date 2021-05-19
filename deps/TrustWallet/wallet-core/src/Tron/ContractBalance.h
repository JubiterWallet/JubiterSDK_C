// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Contract.hpp"
#include "balance_contract.pb.h"


namespace TW::Tron {


::protocol::ResourceCode toInternal(const int source);


//message TransferContract {
//    // Sender address.
//    string owner_address = 1;
//
//    // Recipient address.
//    string to_address = 2;
//
//    // Amount to send.
//    int64 amount = 3;
//}
class TransferContract : public Contract {

public:
    TransferContract() {}
    TransferContract(std::string owner_address,
                     std::string to_address,
                     int64_t amount)
        : Contract(owner_address)
        , to_address(to_address)
        , amount(amount)
    {}
    virtual ~TransferContract() {
        clear();
    }

    void clear() {
        Contract::clear();
        to_address = "";
        amount = 0;
    }

    void from_internal(const ::protocol::TransferContract& contract);
    ::protocol::TransferContract to_internal() const;

    virtual bool calculateOffset() override;

    virtual TW::Data toAddressSize() const;
    virtual TW::Data toAddressOffset(const size_t offset) const;

    virtual TW::Data amountSize() const;
    virtual TW::Data amountOffset(const size_t offset) const;

protected:
    virtual pb_length_delimited getToAddress() const;
    virtual pb_varint getAmount() const;

    virtual size_t toAddressIndex(const size_t offset) const;
    virtual size_t    amountIndex(const size_t offset) const;

    size_t toAddrSize = 0;
    size_t toAddrIndex = 0;

    size_t amtSize = 0;
    size_t amtIndex = 0;

public:
    // Recipient address.
    std::string to_address;
    // Amount to send.
    int64_t amount;
};  // class TransferContract end


//message UnfreezeBalanceContract {
//    // Sender address
//    string owner_address = 1;
//    // Resource type: BANDWIDTH | ENERGY
//    string resource = 10;
//    // Receiver address
//    string receiver_address = 15;
//}
class UnfreezeBalanceContract : public Contract {

public:
    UnfreezeBalanceContract() {}
    UnfreezeBalanceContract(std::string owner_address,
//                            std::string resource,
//                            protocol_ResourceCode resource,
                            ::protocol::ResourceCode resource,
                            std::string receiver_address)
        : Contract(owner_address)
        , resource(resource)
        , receiver_address(receiver_address)
    {}
    virtual ~UnfreezeBalanceContract() {
        clear();
    }

    void clear() {
        Contract::clear();
        receiver_address = "";
    }

    // When resource == bandwidth, pb will not encode this item,
    // so we need to override it's serialize()
    TW::Data serialize();

    void from_internal(const ::protocol::UnfreezeBalanceContract& contract);
    ::protocol::UnfreezeBalanceContract to_internal() const;

    virtual bool calculateOffset() override;

    virtual TW::Data resourceSize() const;
    virtual TW::Data resourceOffset(const size_t offset) const;

    virtual TW::Data receiverAddressSize() const;
    virtual TW::Data receiverAddressOffset(const size_t offset) const;

    virtual size_t        resourceIndex(const size_t offset) const;
    virtual size_t receiverAddressIndex(const size_t offset) const;

protected:
    virtual pb_varint getResource() const;
    virtual pb_length_delimited getReceiverAddress() const;

    size_t resSize = 0;
    size_t resIndex = 0;

    size_t rxAddrSize = 0;
    size_t rxAddrIndex = 0;

protected:
    // Resource type: BANDWIDTH | ENERGY
//    std::string resource;
//    protocol_ResourceCode resource;
    ::protocol::ResourceCode resource;
    // Receiver address
    std::string receiver_address;
};  // class UnfreezeBalanceContract end


//message FreezeBalanceContract {
//    // Sender address.
//    string owner_address = 1;
//    // Frozen balance. Minimum 1
//    int64 frozen_balance = 2;
//    // Frozen duration
//    int64 frozen_duration = 3;
//    // Resource type: BANDWIDTH | ENERGY
//    string resource = 10;
//    // Receiver address
//    string receiver_address = 15;
//}
class FreezeBalanceContract : public UnfreezeBalanceContract {

public:
    FreezeBalanceContract() {}
    FreezeBalanceContract(std::string owner_address,
                          int64_t frozen_balance,
                          int64_t frozen_duration,
//                          std::string resource,
//                          protocol_ResourceCode resource,
                          ::protocol::ResourceCode resource,
                          std::string receiver_address)
        : UnfreezeBalanceContract(owner_address, resource, receiver_address)
        , frozen_balance(frozen_balance)
        , frozen_duration(frozen_duration)
    {}
    virtual ~FreezeBalanceContract() {
        clear();
    }

    void clear() {
        UnfreezeBalanceContract::clear();
        frozen_balance  = 0;
        frozen_duration = 0;
    }

    // When resource == bandwidth, pb will not encode this item,
    // so we need to override it's serialize()
    TW::Data serialize();

    void from_internal(const ::protocol::FreezeBalanceContract& contract);
    ::protocol::FreezeBalanceContract to_internal() const;

    virtual bool calculateOffset() override;

    virtual TW::Data frozenBalanceSize() const;
    virtual TW::Data frozenBalanceOffset(const size_t offset) const;

    virtual TW::Data frozenDurationSize() const;
    virtual TW::Data frozenDurationOffset(const size_t offset) const;

private:
    virtual pb_varint getFrozenBalance() const;
    virtual pb_varint getFrozenDuration() const;

    virtual size_t   frozenBalanceIndex(const size_t offset) const;
    virtual size_t  frozenDurationIndex(const size_t offset) const;

    size_t frozenBalSize = 0;
    size_t frozenBalIndex = 0;

    size_t frozenDSize = 0;
    size_t frozenDIndex = 0;

private:
    // Frozen balance. Minimum 1
    int64_t frozen_balance;
    // Frozen duration
    int64_t frozen_duration;
};  // class FreezeBalanceContract end


//message WithdrawBalanceContract {
//    // Sender address
//    string owner_address = 1;
//}
class WithdrawBalanceContract : public Contract {

public:
    void from_internal(const ::protocol::WithdrawBalanceContract& contract);
    ::protocol::WithdrawBalanceContract to_internal() const;

protected:
    // Resource type: BANDWIDTH | ENERGY
//    std::string resource;
//    protocol_ResourceCode resource;
    ::protocol::ResourceCode resource;
    // Receiver address
    std::string receiver_address;
};  // class WithdrawBalanceContract end


} // namespace TW::Tron
