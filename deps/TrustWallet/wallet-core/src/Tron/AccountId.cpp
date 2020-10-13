// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AccountId.h"
#include "Address.h"
#include "../HexCoding.h"

using namespace std;


namespace TW::Tron {


void AccountId::from_internal(const ::protocol::AccountId& acct) {

    name    = acct.name();
    address = TW::Tron::Address::fromHex(acct.address());
}


::protocol::AccountId AccountId::to_internal() const {

    ::protocol::AccountId encode;

    encode.clear_name();
    if (0 < name.size()) {
        *encode.mutable_name() = name;
    }

    encode.clear_address();
    if (0 < address.size()) {
        Data addr = TW::Tron::Address::toHex(address);
        if (0 < addr.size()) {
            *encode.mutable_address() = {addr.begin(), addr.end()};
        }
        else {
            *encode.mutable_address() = address;
        }
    }

    return encode;
}


void AccountId::deserialize(const Data& o) {

    ::protocol::AccountId decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data AccountId::serialize() {

    auto data = Data();

    ::protocol::AccountId encode = to_internal();
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
