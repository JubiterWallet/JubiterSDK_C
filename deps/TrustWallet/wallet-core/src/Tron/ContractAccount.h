// Copyright © 2017-2022 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Tron.h"
#include "Contract.hpp"
#include "account_contract.pb.h"


namespace TW::Tron {


//message AccountPermissionUpdateContract {
//  bytes owner_address = 1;
//  Permission owner = 2; //Empty is invalidate
//  Permission witness = 3; //Can be empty
//  repeated Permission actives = 4; //Empty is invalidate
//}
class AccountPermissionUpdateContract : public Contract {

public:
    AccountPermissionUpdateContract() {}
    AccountPermissionUpdateContract(const std::string& owner_address,
                                    const Permission& owner,
                                    const Permission& witness,
                                    const std::vector<Permission>& actives)
        : Contract(owner_address)
        , owner(owner)
        , witness(witness)
        , actives(actives)
    {}
    virtual ~AccountPermissionUpdateContract() {
        clear();
    }

    void clear() {
        Contract::clear();
        owner.clear();
        witness.clear();
        actives.clear();
    }

    void from_internal(const ::protocol::AccountPermissionUpdateContract& contract);
    ::protocol::AccountPermissionUpdateContract to_internal() const;

    virtual bool calculateOffset() override;

    virtual uint64_t ownerOffset(const uint64_t offset) const;
    virtual uint64_t activesOffset(const uint64_t offset) const;
    virtual uint64_t activeOffset(const uint64_t offset, const size_t index) const;

protected:
    virtual pb_length_delimited getOwner() const;
    virtual pb_length_delimited getWitness() const;
    virtual Data getActives() const;
    virtual pb_length_delimited getActive(const size_t index) const;

    virtual uint64_t activeIndex(const uint64_t offset, const size_t index) const;

    size_t _ownerSize = 0;
    size_t _ownerIndex = 0;

    size_t _witnessSize = 0;
    size_t _witnessIndex = 0;

    size_t _activesSize = 0;
    size_t _activesIndex = 0;

public:
    Permission owner;                   // Modified owner permission
    Permission witness;                 // Modified witness permission (if it is a witness)
    std::vector<Permission> actives;    // Modified actives permission
};  // class AccountPermissionUpdateContract end


} // namespace TW::Tron
