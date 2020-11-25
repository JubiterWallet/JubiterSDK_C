// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ContractBalance.h"
#include "asset_issue_contract.pb.h"

namespace TW::Tron {


//message TransferAssetContract {
//    // Asset name.
//    string asset_name = 1;
//
//    // Sender address.
//    string owner_address = 2;
//
//    // Recipient address.
//    string to_address = 3;
//
//    // Amount to send.
//    int64 amount = 4;
//}
class TransferAssetContract : public TransferContract {

public:
    TransferAssetContract() {}
    TransferAssetContract(std::string asset_name,
                          std::string owner_address,
                          std::string to_address,
                          int64_t amount)
        : asset_name(asset_name)
        , TransferContract(owner_address, to_address, amount)
    {}
    virtual ~TransferAssetContract() {
        clear();
    }

    void clear() {
        asset_name = "";
        TransferContract::clear();
    }

    void from_internal(const ::protocol::TransferAssetContract& contract);
    ::protocol::TransferAssetContract to_internal() const;

    void deserialize(const Data& o);
    Data serialize();

    virtual TW::Data assetNameSize() const;
    virtual TW::Data assetNameOffset(const size_t offset) const;

    virtual TW::Data toAddressSize() const override;
    virtual TW::Data toAddressOffset(const size_t offset) const override;

    virtual TW::Data amountSize() const override;
    virtual TW::Data amountOffset(const size_t offset) const override;

private:
    virtual bool calculateOffset() override;

    virtual pb_length_delimited getAssetName() const;
    virtual pb_length_delimited getOwnerAddress() const override;
    virtual pb_length_delimited getToAddress() const override;
    virtual pb_varint getAmount() const override;

    virtual size_t assetNameIndex(const size_t offset) const;
    virtual size_t toAddressIndex(const size_t offset) const override;
    virtual size_t    amountIndex(const size_t offset) const override;

    size_t asNameSize  = 0;
    size_t asNameIndex = 0;

public:
    // Asset name.
    std::string asset_name;
};  // class TransferAssetContract end


//message UnfreezeAssetContract {
//    // Sender address
//    string owner_address = 1;
//}
class UnfreezeAssetContract : public Contract {

public:
    UnfreezeAssetContract() {}
    UnfreezeAssetContract(std::string owner_address)
        : Contract(owner_address)
    {}
    virtual ~UnfreezeAssetContract() {
        clear();
    }

    void clear() {
        Contract::clear();
    }

    void from_internal(const ::protocol::UnfreezeAssetContract& contract);
    ::protocol::UnfreezeAssetContract to_internal() const;

    void deserialize(const Data& o);
    Data serialize();
};  // class UnfreezeAssetContract end


} // namespace TW::Tron end
