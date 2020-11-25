// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Contract.h"
#include "Vote.h"
#include "vote_asset_contract.pb.h"

namespace TW::Tron {


//message VoteAssetContract {
//    // Sender address
//    string owner_address = 1;
//    // Vote addresses
//    repeated string vote_address = 2;
//    bool support = 3;
//    int32 count = 5;
//}
class VoteAssetContract : public Contract {

public:
    VoteAssetContract() {}
    VoteAssetContract(std::string owner_address,
                      std::vector<Vote> votes,
                      bool support,
                      int32_t count)
        : Contract(owner_address)
        , votes(votes)
        , support(support)
        , count(count)
    {}
    virtual ~VoteAssetContract() {
        clear();
    }

    void clear() {
        Contract::clear();
        votes.clear();
        support = false;
        count = 0;
    }

    void from_internal(const ::protocol::VoteAssetContract& contract);
    ::protocol::VoteAssetContract to_internal() const;

    void deserialize(const Data& o);
    Data serialize();

protected:
    virtual bool calculateOffset() override;

    virtual pb_length_delimited getVoteAddress(const size_t index) const;
    virtual pb_varint getVoteCount() const;

private:
    std::vector<Vote> votes;
    bool support;
    int32_t count;
};  // class VoteAssetContract end


} // namespace TW::Tron
