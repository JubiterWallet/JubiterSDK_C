// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractVoteAsset.h"
#include "../HexCoding.h"


namespace TW::Tron {


void VoteAssetContract::from_internal(const ::protocol::VoteAssetContract& contract) {

    owner_address = TW::Tron::Address::fromHex(contract.owner_address());

    if (0 < contract.vote_address_size()) {
        for (int i=0; i<contract.vote_address_size(); ++i) {
            Vote vote;
            vote.vote_address = TW::Tron::Address::fromHex(contract.vote_address(i));
            votes.push_back(vote);
        }
    }

    support = contract.support();

    count = contract.count();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::VoteAssetContract VoteAssetContract::to_internal() const {

    ::protocol::VoteAssetContract encode;

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

    encode.clear_vote_address();
    if (0 < votes.size()) {
        std::vector<std::string> owner_address;
        for (const auto& vote : votes) {
            std::string *en = encode.add_vote_address();
            Data address = TW::Tron::Address::toHex(vote.vote_address);
            std::copy(address.begin(), address.end(), en->begin());
        }
    }

    encode.clear_support();
    if (support) {
        encode.set_support(support);
    }

    encode.clear_count();
    if (0 < count) {
        encode.set_count(count);
    }

    return encode;
}


void VoteAssetContract::deserialize(const Data& o) {

    ::protocol::VoteAssetContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data VoteAssetContract::serialize() {

    auto data = Data();

    ::protocol::VoteAssetContract encode = to_internal();
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


bool VoteAssetContract::calculateOffset() {

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    pb_varint pbCount = getVoteCount();

    if (!pbOwnerAddress.isValid()
        ||     !pbCount.isValid()
        ) {
        return false;
    }
    size_t szOwnerAddress = pbOwnerAddress.size();
    size_t szVoteAddress = 0;
    pb_length_delimited pbVoteAddress;
    for (size_t i=0; i<votes.size(); ++i) {
        pbVoteAddress = getVoteAddress(i);
        if (!pbVoteAddress.isValid()) {
            return false;
        }
        szVoteAddress += pbVoteAddress.size();
    }

    return true;
}


pb_length_delimited VoteAssetContract::getVoteAddress(const size_t index) const {

    return pb_length_delimited(::protocol::VoteAssetContract::kVoteAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(votes[index].vote_address));
}


pb_varint VoteAssetContract::getVoteCount() const {

    return pb_varint(::protocol::VoteAssetContract::kCountFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT32,
                     count);
}


} // namespace TW::Tron end
