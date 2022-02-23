// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <cstdint>
#include "../Data.h"
//#include "Address.h"
#include <google/protobuf/message.h>
#include <google/protobuf/any.pb.h>

#include "pbparse/pb_varint.hpp"
#include "pbparse/pb_length_delimited.hpp"

namespace TW::Tron {


class PbParser {

public:
    PbParser() {}
    virtual ~PbParser() {
        clear();
    }

    void clear() {
        pb.clear();
    }

    bool isValid() {
        return !(0 == pb.size());
    }

    void deserialize(const Data& o, google::protobuf::Message& decode) {

        bool status = decode.ParseFromArray(&o[0], (int)o.size());
        if (status) {
            // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
            save(o);
            from_internal(decode);
        }
    }

protected:
    virtual bool search(const size_t offset, const TW::Data &itemInpb, size_t& index) const {

        auto it = std::search(std::begin(pb)+offset, std::end(pb),
                              itemInpb.begin(), itemInpb.end());
        if (pb.end() == it) {
            index = it - pb.end();
            return false;
        }
        else {
            index = (it - pb.begin());
            return true;
        }
    }

    void save(const Data& o) {
        // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
        pb.clear();
        std::copy(std::begin(o), std::end(o), std::back_inserter(pb));
    }

    void from_internal(const google::protobuf::Message& msg) {

        size_t sz = msg.ByteSizeLong();
        TW::Data o(sz);
        bool b = msg.SerializeToArray(&o[0], (int)sz);
        if (b) {
            // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
            save(o);
        }
    }

    Data pb;    // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
};  // class PbParser end


} // namespace TW::Tron end
