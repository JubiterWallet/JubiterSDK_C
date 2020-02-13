// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"

namespace TW::Bitcoin {

// JuBiter-defined
/// Hcash transaction out-point reference.
class HcashOutPoint :
    public OutPoint {
  public:
    uint8_t outputTree = 0;

    HcashOutPoint() {}
   ~HcashOutPoint() {}

    /// Initializes an out-point reference with a hash and an index.
    template <typename T>
    HcashOutPoint(const T& h, uint32_t index, uint8_t outputTree = 0) : outputTree(outputTree), OutPoint(h, index) {}

    /// Encodes the out-point into the provided buffer.
    virtual void encode(std::vector<uint8_t>& data) const override;

    /// Decodes the out-point into the provided buffer.
    virtual bool decode(const std::vector<uint8_t>& data) override;

    virtual size_t size() override;

    friend bool operator<(const HcashOutPoint& a, const HcashOutPoint& b) {
        int cmp = std::memcmp(a.hash.data(), b.hash.data(), 32);
        return cmp < 0 || (cmp == 0 && a.index < b.index) || (a.outputTree != b.outputTree);
    }

    friend bool operator==(const HcashOutPoint& a, const HcashOutPoint& b) {
        int cmp = std::memcmp(a.hash.data(), b.hash.data(), 32);
        return (cmp == 0 && a.index == b.index && a.outputTree == b.outputTree);
    }

    friend bool operator!=(const HcashOutPoint& a, const HcashOutPoint& b) { return !(a == b); }
}; // class HcashOutPoint end

} // namespace TW::Bitcoin
