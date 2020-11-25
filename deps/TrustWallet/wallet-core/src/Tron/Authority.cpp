// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Authority.h"
#include "BinaryCoding.h"

using namespace std;


namespace TW::Tron {


void Authority::from_internal(const ::protocol::authority& auth) {

    account.from_internal(auth.account());
    permission_name = auth.permission_name();
}


::protocol::authority Authority::to_internal() const {

    ::protocol::authority encode;

    encode.clear_account();
    *encode.mutable_account() = account.to_internal();

    encode.clear_permission_name();
    if (0 < permission_name.size()) {
        *encode.mutable_permission_name() = permission_name;
    }

    return encode;
}


void Authority::deserialize(const Data& o) {

    ::protocol::authority decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data Authority::serialize() {

    auto data = Data();

    ::protocol::authority encode = to_internal();
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
