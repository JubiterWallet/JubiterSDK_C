// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Contract.h"
#include "Vote.h"
#include "witness_contract.pb.h"

namespace TW::Tron {


class VoteWitnessContractVote : public Vote {
public:
    static ::protocol::Vote from_internal(const ::protocol::VoteWitnessContract_Vote& vote);
    static ::protocol::VoteWitnessContract_Vote to_internal(const ::protocol::Vote& vote);
};  // class VoteWitnessContractVote end


//message VoteWitnessContract {
//    message Vote {
//        ...
//    }
//    string owner_address = 1;
//    repeated Vote votes = 2;
//    bool support = 3;
//}
class VoteWitnessContract : public Contract {

public:
    VoteWitnessContract() {};
    VoteWitnessContract(std::string owner_address,
                        std::vector<Vote> votes,
                        bool support)
        : Contract(owner_address)
        , votes(votes)
        , support(support)
    {}
    virtual ~VoteWitnessContract() {
        clear();
    }

    void clear() {
        Contract::clear();
        votes.clear();
        support = false;
    }

    void from_internal(const ::protocol::VoteWitnessContract& contract);
    ::protocol::VoteWitnessContract to_internal() const;

    void deserialize(const Data& o);
    Data serialize();

private:
    virtual pb_length_delimited getVoteAddress(const size_t index) const;
    virtual pb_varint getVoteCount(const size_t index) const;

private:
    std::vector<Vote> votes;
    bool support;
};  // class VoteWitnessContract end


} // namespace TW::Tron
