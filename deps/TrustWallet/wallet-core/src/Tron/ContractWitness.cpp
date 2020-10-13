// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractWitness.h"
#include "../HexCoding.h"


namespace TW::Tron {


::protocol::Vote VoteWitnessContractVote::from_internal(const ::protocol::VoteWitnessContract_Vote& vote) {

    ::protocol::Vote encode;
    encode.clear_vote_address();
    if (0 < vote.vote_address().size()) {
        Data addr = TW::Tron::Address::toHex(vote.vote_address());
        if (0 < addr.size()) {
            *encode.mutable_vote_address() = {addr.begin(), addr.end()};
        }
        else {
            *encode.mutable_vote_address() = {vote.vote_address().begin(), vote.vote_address().end()};
        }
    }

    encode.clear_vote_count();
    encode.set_vote_count(vote.vote_count());

    return encode;
}


::protocol::VoteWitnessContract_Vote VoteWitnessContractVote::to_internal(const ::protocol::Vote& vote) {

    ::protocol::VoteWitnessContract_Vote encode;

    encode.clear_vote_address();
    if (0 < vote.vote_address().size()) {
        *encode.mutable_vote_address() = {vote.vote_address().begin(), vote.vote_address().end()};
    }

    encode.clear_vote_count();
    encode.set_vote_count(vote.vote_count());

    return encode;
}


void VoteWitnessContract::from_internal(const ::protocol::VoteWitnessContract& contract) {

    owner_address = TW::Tron::Address::fromHex(contract.owner_address());

    if (0 < contract.votes_size()) {
        for (int i=0; i<contract.votes_size(); ++i) {
            Vote vote;
            vote.from_internal(VoteWitnessContractVote::from_internal(contract.votes(i)));
            votes.push_back(vote);
        }
    }

    support = contract.support();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::VoteWitnessContract VoteWitnessContract::to_internal() const {

    ::protocol::VoteWitnessContract encode;

    encode.clear_owner_address();
    if (0 < owner_address.size()) {
        Data address = TW::Tron::Address::toHex(owner_address);
        if (0 < address.size()) {
            *encode.mutable_owner_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_owner_address() = owner_address;
        }
    }

    encode.clear_votes();
    if (0 < votes.size()) {
        for (int i=0; i<votes.size(); ++i) {
            ::protocol::VoteWitnessContract_Vote* en = encode.add_votes();
            *en = VoteWitnessContractVote::to_internal(votes[i].to_internal());
        }
    }

    encode.clear_support();
    if (support) {
        encode.set_support(support);
    }

    return encode;
}


void VoteWitnessContract::deserialize(const Data& o) {

    ::protocol::VoteWitnessContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data VoteWitnessContract::serialize() {

    auto data = Data();

    ::protocol::VoteWitnessContract encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto ou = Data(szSize);

    bool status = encode.SerializeToArray(&ou[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(ou), std::end(ou), std::begin(data));
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }

    return data;
}


pb_length_delimited VoteWitnessContract::getVoteAddress(const size_t index) const {

    return pb_length_delimited(::protocol::VoteWitnessContract_Vote::kVoteAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               votes[index].vote_address);
}


pb_varint VoteWitnessContract::getVoteCount(const size_t index) const {

    return pb_varint(::protocol::VoteWitnessContract_Vote::kVoteCountFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT32,
                     votes[index].vote_count);
}


} // namespace TW::Tron end
