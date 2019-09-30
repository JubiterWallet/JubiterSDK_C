// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Action.h"
#include "../HexCoding.h"
#include "../Bravo/Serialization.h"
#include "../Bravo/Deserialization.h"
#include "mSIGNA/stdutils/uchar_vector.h"

using namespace TW;
using namespace TW::EOS;
using json = nlohmann::json;

// JuBiter-defined
size_t PermissionLevel::size() {
    return (sizeof(uint64_t)/sizeof(uint8_t))*2;
}

void PermissionLevel::serialize(Data& o) const {
    actor.serialize(o);
    permission.serialize(o);
}

json PermissionLevel::serialize() const noexcept {
    json obj;
    obj["actor"] = actor.string();
    obj["permission"] = permission.string();
    return obj;
}

// JuBiter-defined
void Action::deserialize(const Data& o) noexcept {
    uchar_vector vAction(o);

    // account
    account.value = vAction.read_le_uint64();

    // name
       name.value = vAction.read_le_uint64();

    std::string strAcct = account.string();
    std::string strName = name.string();

    // Collection - authorizations
    uchar_vector vTempAuthorizations(vAction.get_cur_it(), vAction.end());
    Data tempTempAuthorizations(vTempAuthorizations);
    uint64_t authorizationCnt = 0;
    int varIntByteSize = 0;
    Bravo::decodeVarInt64(tempTempAuthorizations, authorizationCnt, varIntByteSize);
    vAction.reset_it(varIntByteSize);
    for (uint64_t i=0; i<authorizationCnt; ++i) {
        Name actor, permission;
        actor.value = vAction.read_le_uint64();
        permission.value = vAction.read_le_uint64();

        std::string strActor = actor.string();
        std::string strPermission = permission.string();

        PermissionLevel auth(actor, permission);
        authorization.push_back(auth);
    }

    // data
    uchar_vector vTempData(vAction.get_cur_it(), vAction.end());
    Data tempData(vTempData);
    uint64_t dataSize = 0;
    varIntByteSize = 0;
    Bravo::decodeVarInt64(tempData, dataSize, varIntByteSize);
    vAction.reset_it(varIntByteSize);
    data = vAction.read_vector(dataSize);
}

// JuBiter-defined
size_t Action::size() {
    uint64_t authorizationCnt = authorization.size();
    Data authorizationVarInt64Len;
    Bravo::encodeVarInt64(authorizationCnt, authorizationVarInt64Len);

    uint64_t dataSize = data.size();
    Data dataVarInt64Len;
    Bravo::encodeVarInt64(dataSize, dataVarInt64Len);

    return (  Name::size()
            + Name::size()
            + authorizationVarInt64Len.size() + PermissionLevel::size() * authorizationCnt
            + dataVarInt64Len.size() + dataSize);
}

void Action::serialize(Data& o) const {
    account.serialize(o);
    name.serialize(o);
    Bravo::encodeCollection(authorization, o);
    Bravo::encodeVarInt64(data.size(), o);
    append(o, data);
}

json Action::serialize() const noexcept {
    json obj;
    if (   0 == account.value
        || 0 == name.value
        || 0 == authorization.size()
        || 0 == data.size()
        ) {
        return obj;
    }

    obj["account"] = account.string();
    obj["name"] = name.string();
    obj["authorizations"] = Bravo::encodeCollection(authorization);
//    obj["data"] = hex(data);
    obj["hex_data"] = hex(data);

    return obj;
}

TransferAction::TransferAction( const std::string& currency,
                                const std::string& from, 
                                const std::string& to, 
                                const Bravo::Asset& asset, 
                                const std::string& memo) {
    account = Name(currency);
    name = Name("transfer");
    authorization.push_back(PermissionLevel(Name(from), Name("active")));

    setData(from, to, asset, memo);
}

void TransferAction::setData(const std::string& from, const std::string& to, const Bravo::Asset& asset, const std::string& memo) {
    if (asset.amount <= 0) {
        throw std::invalid_argument("Amount in a transfer action must be greater than zero.");
    }

    this->from = Name(from);
    this->to = Name(to);
    this->asset = asset;
    Bravo::encodeString(memo, this->memo);

//    Name(from).serialize(data);
    this->from.serialize(data);
//    Name(to).serialize(data);
    this->to.serialize(data);
//    asset.serialize(data);
    this->asset.serialize(data);
//    Bravo::encodeString(memo, data);
    copy(this->memo.begin(), this->memo.end(), back_inserter(data));
}

// JuBiter-defined
void TransferAction::deserialize(const Data& o) noexcept {
    uchar_vector vTransfer(o);
    from.value = vTransfer.read_le_uint64();
      to.value = vTransfer.read_le_uint64();

    //Bravo::Asset
    asset.amount = vTransfer.read_le_uint64();
    asset.symbol = vTransfer.read_le_uint64();

    //memo
    uchar_vector vTemp(vTransfer.get_cur_it(), vTransfer.end());
    Data temp(vTemp);
    memo = temp;
}

// JuBiter-defined
void TransferAction::serialize(Data& o) const {
    append(o, data);
}

// JuBiter-defined
json TransferAction::serialize() const noexcept {
    json obj = Action::serialize();

    json data;
    data["from"] = from.string();
    data["to"] = to.string();
    data["quantity"] = asset.string();
    std::string memo;
    int varIntByteSize = 0;
    Bravo::decodeString(this->memo, memo, varIntByteSize);
    data["memo"] = memo;

    if (obj.empty()) {
        return data;
    }
    else {
        obj["data"] = data;
        return obj;
    }
}
