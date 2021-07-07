// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/OutPoint.h"

namespace TW::Hcash {

// JuBiter-defined
/// Hcash transaction out-point reference.
class OutPoint :
    public TW::Bitcoin::OutPoint {
  public:
    uint8_t outputTree = 0;

    OutPoint() {}
   ~OutPoint() {}

    /// Initializes an out-point reference with a hash and an index.
    template <typename T>
    OutPoint(const T& h, uint32_t index, uint8_t outputTree = 0) : outputTree(outputTree), OutPoint(h, index) {}

    /// Encodes the out-point into the provided buffer.
    virtual void encode(std::vector<uint8_t>& data) const override;

    /// Decodes the out-point into the provided buffer.
    virtual bool decode(const std::vector<uint8_t>& data) override;

    virtual size_t size() override;

    friend bool operator<(const OutPoint& a, const OutPoint& b) {
        int cmp = std::memcmp(a.hash.data(), b.hash.data(), 32);
        return cmp < 0 || (cmp == 0 && a.index < b.index) || (a.outputTree != b.outputTree);
    }

    friend bool operator==(const OutPoint& a, const OutPoint& b) {
        int cmp = std::memcmp(a.hash.data(), b.hash.data(), 32);
        return (cmp == 0 && a.index == b.index && a.outputTree == b.outputTree);
    }

    friend bool operator!=(const OutPoint& a, const OutPoint& b) { return !(a == b); }
}; // class OutPoint end

} // namespace TW::Hcash
