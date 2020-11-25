// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#if 0

#include "Contract.h"

#include "shield_contract.pb.h"


namespace TW::Tron {


//message ShieldedTransferContract {
//    bytes transparent_from_address = 1; // transparent address
//    int64 from_amount = 2;
//    repeated SpendDescription spend_description = 3;
//    repeated ReceiveDescription receive_description = 4;
//    bytes binding_signature = 5;
//    bytes transparent_to_address = 6; // transparent address
//    int64 to_amount = 7; // the amount to transparent to_address
//}
class ShieldedTransferContract {

public:
    ShieldedTransferContract() {}
    virtual ~ShieldedTransferContract() {
        clear();
    }

    void clear() {
        transparent_from_address.clear();
        from_amount = 0;
        transparent_to_address.clear();
        to_amount = 0;
    }

    void from_internal(const ::protocol::ShieldedTransferContract& contract);
    ::protocol::ShieldedTransferContract to_internal() const;

    void deserialize(const Data& o);
    Data serialize();

private:
    virtual bool calculateOffset();

    virtual pb_length_delimited getTransparentFromAddress() const;
    virtual pb_length_delimited getTransparentToAddress() const;
    virtual pb_varint getFromAmount() const;
    virtual pb_varint getToAmount() const;

public:
    // transparent address
    Data transparent_from_address;
    int64_t from_amount;
    // transparent address
    Data transparent_to_address;
    // the amount to transparent to_address
    int64_t to_amount;
};  // class ShieldedTransferContract end


} // namespace TW::Tron

#endif