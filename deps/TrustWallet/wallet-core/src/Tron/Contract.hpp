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
#include <google/protobuf/message.h>
#include <google/protobuf/any.pb.h>

#include "PbParser.hpp"

namespace TW::Tron {


/// Contract point for implementation of Tron coin.
class Contract : public PbParser {

public:
    Contract() {}
    Contract(std::string owner_address)
        : owner_address(owner_address)
    {}
    virtual ~Contract() {
        clear();
    }

    void clear() {
        PbParser::clear();
        owner_address = "";
    }

    bool isValid() {
        return !("" == owner_address);
    }

    template<typename U, typename T>
    static Data serialize(T& contract) {

        auto data = Data();

        U encode = contract.to_internal();
        size_t szSize = encode.ByteSizeLong();
        auto ou = Data(szSize);

        bool status = encode.SerializeToArray(&ou[0], (int)szSize);
        if (status) {
            data.resize(szSize);
            std::copy(std::begin(ou), std::end(ou), std::begin(data));
        }

        // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
        contract.save(data);

        // Calculate the offset of each item
        if (!contract.calculateOffset()) {
            data.clear();
            return {};
        }

        return data;
    }

protected:
    virtual bool calculateOffset();
    virtual bool calculateOffset(const size_t offset);

    virtual pb_length_delimited getOwnerAddress() const;

    size_t szOwnerAddress = 0;
    size_t ownerAddrSize = 0;
//    size_t ownerAddrIndex = 0;

public:
    // Sender address.
    std::string owner_address;
};  // class Contract end


} // namespace TW::Tron end
