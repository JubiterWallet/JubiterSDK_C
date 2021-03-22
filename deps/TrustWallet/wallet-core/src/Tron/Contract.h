// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <cstdint>
#include "../Data.h"
#include "Address.h"

#include "pbparse/pb_varint.hpp"
#include "pbparse/pb_length_delimited.hpp"

namespace TW::Tron {


/// Contract point for implementation of Tron coin.
class Contract {

public:
    Contract() {}
    Contract(std::string owner_address)
        : owner_address(owner_address)
    {}
    virtual ~Contract() {
        clear();
    }

    void clear() {
        owner_address = "";
    }

protected:
    virtual bool calculateOffset();
    virtual bool isItemExist(const size_t offset, const TW::Data &itemInpb) const;

    virtual pb_length_delimited getOwnerAddress() const;

    void save(const Data& o) {
        // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
        pb.clear();
        std::copy(std::begin(o), std::end(o), std::back_inserter(pb));
    }

    size_t ownerAddrSize = 0;
//    size_t ownerAddrIndex = 0;
    Data pb;    // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX

public:
    // Sender address.
    std::string owner_address;
};  // class Contract end


} // namespace TW::Tron end
