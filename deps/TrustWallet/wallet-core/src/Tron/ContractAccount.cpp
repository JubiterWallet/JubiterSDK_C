// Copyright © 2017-2022 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractAccount.h"

namespace TW::Tron {


void AccountPermissionUpdateContract::from_internal(const ::protocol::AccountPermissionUpdateContract& contract) {

    Contract::from_internal(contract);

    owner_address = TW::Tron::Address::fromHex(contract.owner_address());
    if (contract.has_owner()) {
        owner.from_internal(contract.owner());
    }

    if (contract.has_witness()) {
        witness.from_internal(contract.witness());
    }

    for (int i=0; i<contract.actives_size(); ++i) {
        TW::Tron::Permission perm;
        perm.from_internal(contract.actives(i));
        actives.push_back(perm);
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


/// Converts an external TransferContract to an internal one used for signing.
::protocol::AccountPermissionUpdateContract AccountPermissionUpdateContract::to_internal() const {

    if (!owner.isValid()
        || 0 == actives.size()
    ) {
        return ::protocol::AccountPermissionUpdateContract();
    }

    ::protocol::AccountPermissionUpdateContract encode;

    // AccountPermissionUpdateContract.owner_address
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

    // AccountPermissionUpdateContract.owner
    encode.clear_owner();
    *encode.mutable_owner() = owner.to_internal();

    // AccountPermissionUpdateContract.witness
    encode.clear_witness();
    if (witness.isValid()) {
        *encode.mutable_witness() = witness.to_internal();
    }

    // AccountPermissionUpdateContract.actives
    encode.clear_actives();
    for (const auto& active : actives) {
        ::protocol::Permission *en = encode.add_actives();
        *en = active.to_internal();
    }

    return encode;
}


bool AccountPermissionUpdateContract::calculateOffset() {

    if (!Contract::calculateOffset()) {
        return false;
    }

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    pb_length_delimited pbOwner   = getOwner();
    pb_length_delimited pbWitness = getWitness();
    Data pbActives = getActives();
    if (   !pbOwner.isValid()
        || pbActives.empty()
        ) {
        return false;
    }

    size_t    szOwner = 0;
    if (search(szOwnerAddress,
               pbOwner.serialize(),
               _ownerIndex)
        ) {
        szOwner = pbOwner.size();

        _ownerSize  = pbOwner.sizeValue();
        _ownerIndex += pbOwner.sizeTag() + pbOwner.sizeLength();
    }

    size_t    szWitness = 0;
    if (search(szOwnerAddress+szOwner,
               pbWitness.serialize(),
               _witnessIndex)
        ) {
        szWitness = pbWitness.size();

        _witnessSize  = pbWitness.sizeValue();
        _witnessIndex += pbWitness.sizeTag() + pbWitness.sizeLength();
    }

    if (search(szOwnerAddress+szOwner+szWitness,
               pbActives,
               _activesIndex)
        ) {
        _activesSize  = pbActives.size();
    }

    return true;
}


uint64_t AccountPermissionUpdateContract::ownerOffset(const uint64_t offset) const {

    return (0 == _ownerIndex) ? _ownerIndex : (_ownerIndex + offset);
}


uint64_t AccountPermissionUpdateContract::activesOffset(const uint64_t offset) const {

    return activeOffset(offset, 0);
}


uint64_t AccountPermissionUpdateContract::activeOffset(const uint64_t offset, const size_t index) const {

    return activeIndex(offset, index);
}


pb_length_delimited AccountPermissionUpdateContract::getOwner() const {

    if (!owner.isValid()) {
        return pb_length_delimited();
    }

    return pb_length_delimited(::protocol::AccountPermissionUpdateContract::kOwnerFieldNumber,
                               WireFormatLite::FieldType::TYPE_MESSAGE,
                               owner.serialize());
}


pb_length_delimited AccountPermissionUpdateContract::getWitness() const {

    if (!witness.isValid()) {
        return pb_length_delimited();
    }

    return pb_length_delimited(::protocol::AccountPermissionUpdateContract::kWitnessFieldNumber,
                               WireFormatLite::FieldType::TYPE_MESSAGE,
                               witness.serialize());
}


Data AccountPermissionUpdateContract::getActives() const {

    std::vector<pb_length_delimited> vActives;
    for (const auto& active : actives) {
        vActives.push_back(
            pb_length_delimited(::protocol::AccountPermissionUpdateContract::kActivesFieldNumber,
                                WireFormatLite::FieldType::TYPE_MESSAGE,
                                active.serialize())
        );
    }

    size_t actives_len = 0;
    for (const auto& active : vActives) {
        actives_len += active.size();
    }
    if (0 >= actives_len) {
        return {};
    }

    Data data(actives_len);
    size_t index = 0;
    for (const auto& active : vActives) {
        auto encode = active.serialize();
        std::copy(encode.begin(), encode.end(), data.begin()+index);
        index += active.size();
    }

    return data;
}


pb_length_delimited AccountPermissionUpdateContract::getActive(const size_t index) const {

    Permission active;
    for (int i=0; i<actives.size(); ++i) {
        if (i == index) {
            active = actives[i];
            break;
        }
    }

    if (!active.isValid()) {
        return pb_length_delimited();
    }

    return pb_length_delimited(::protocol::AccountPermissionUpdateContract::kActivesFieldNumber,
                               WireFormatLite::FieldType::TYPE_MESSAGE,
                               active.serialize());
}


uint64_t AccountPermissionUpdateContract::activeIndex(const uint64_t offset, const size_t index) const {

    pb_length_delimited pbActive = getActive(index);
    if (!pbActive.isValid()) {
        return offset;
    }

    size_t activeIndex = 0;
    if (search(_activesIndex,
               pbActive.serialize(),
               activeIndex)
        ) {
        activeIndex += pbActive.sizeTag() + pbActive.sizeLength();
    }

    return (0 == activeIndex) ? activeIndex : (activeIndex + offset);
}


} // namespace TW::Tron end
