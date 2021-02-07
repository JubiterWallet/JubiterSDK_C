//
//  TaggedAddresses.hpp
//  JubSDKCore
//
//  Created by Administrator on 2021/2/4.
//  Copyright © 2021 sherlockirene. All rights reserved.
//
#ifndef __TaggedAddresses__
#define __TaggedAddresses__
#include "Address.h"
#include "../HexCoding.h"



namespace TW::Ripple {

// JuBiter-defined
//https://github.com/xrp-community/standards-drafts/issues/6
class TaggedAddresses :
public Address {


public:
    enum JUB_TAGGED_ADDRESS_FLAGS_TYPE {
        NO_TAG = 0x00,
        TAG_32 = 0x01,
        TAG_64 = 0x02
    };

    static const uint16_t mainnetPrefix = 0x0544;
    static const uint16_t testnetPrefix = 0x0493;

    TaggedAddresses() {}
    ~TaggedAddresses() {
        clear();
    }

    /// Initializes a Ripple X-address with a string representation.
    explicit TaggedAddresses(const std::string& x_address);
    explicit TaggedAddresses(const TW::Data& decoded);

    std::array<byte, 2> prefixes;
    std::array<byte, 1> flags;
    std::array<byte, 8> tag;

    void clear() {
        std::memset(prefixes.begin(), 0x00, prefixes.size());
        std::memset(flags.begin(),    0x00, flags.size());
        std::memset(tag.begin(),      0x00, tag.size());
    }

    // [← 2 byte prefix →|← 160 bits of account ID →|← 8 bits of flags →|← 64 bits of tag →]
    static const size_t size = 2 + TW::Ripple::Address::hashSize + 1 + 8;

    virtual void decode(const TW::Data& encode);

    /// Returns a string representation of the address.
    virtual std::string string() const override;

    /// Returns r-address and tag of the X-address.
    virtual bool parse(std::string& oAddress, uint64_t& oTag) const;

    static bool isValid(const std::string& string);
};
}
#endif
