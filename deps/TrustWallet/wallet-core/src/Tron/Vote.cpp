// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Vote.h"
#include "Address.h"
#include "../HexCoding.h"


namespace TW::Tron {


void Vote::from_internal(const ::protocol::Vote& vote) {

    vote_address = TW::Tron::Address::fromHex(vote.vote_address());
    vote_count   = vote.vote_count();
}


::protocol::Vote Vote::to_internal() const {

    ::protocol::Vote encode;

    encode.clear_vote_address();
    if (0 < vote_address.size()) {
        Data address = TW::Tron::Address::toHex(vote_address);
        if (0 < address.size()) {
            *encode.mutable_vote_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_vote_address() = vote_address;
        }
    }

    encode.clear_vote_count();
    if (0 < vote_count) {
        encode.set_vote_count(vote_count);
    }

    return encode;
}


void Vote::deserialize(const Data& o) {

    ::protocol::Vote decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data Vote::serialize() {

    auto data = Data();

    ::protocol::Vote encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto o = Data(szSize);

    bool status = encode.SerializeToArray(&o[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(o), std::end(o), std::begin(data));
    }

    return data;
}


} // namespace TW::Tron end
