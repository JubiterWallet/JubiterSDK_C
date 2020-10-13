// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <cstdint>
#include "../Data.h"

#include "Tron.pb.h"


namespace TW::Tron {


/// Vote point for implementation of Tron coin.
//// vote message
//message Vote {
//  // the super rep address
//  bytes vote_address = 1;
//  // the vote num to this super rep.
//  int64 vote_count = 2;
//}
class Vote {

public:
    Vote() {}
    Vote(std::string vote_address,
         int64_t vote_count)
        : vote_address(vote_address)
        , vote_count(vote_count)
    {}
    virtual ~Vote() {}

    void from_internal(const ::protocol::Vote& vote);
    ::protocol::Vote to_internal() const;

    void deserialize(const Data& o);
    Data serialize();

public:
    // Vote addresses
    std::string vote_address;
    int64_t vote_count;
};  // class Vote end


} // namespace TW::Tron
