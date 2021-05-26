//
//  TaggedAddresses.cpp
//  JubSDKCore
//
//  Created by Administrator on 2021/2/4.
//  Copyright © 2021 sherlockirene. All rights reserved.
//

#include "TaggedAddresses.h"
#include "../Base58.h"
#include "../BinaryCoding.h"

using namespace TW::Ripple;
std::string g_addreStr;
std::string g_tagStr;


// JuBiter-defined
TaggedAddresses::TaggedAddresses(const std::string& x_address) {

    decode(Base58::ripple.decodeCheck(x_address));
}

// JuBiter-defined
TaggedAddresses::TaggedAddresses(const TW::Data& decoded) {

    decode(decoded);
}

// JuBiter-defined
void TaggedAddresses::decode(const TW::Data& encode) {

    clear();

    size_t index = 0;
    std::copy(encode.begin()+index, encode.begin()+index+prefixes.size(), prefixes.begin());
    index += prefixes.size();
    /// see type prefix: https://developers.ripple.com/base58-encodings.html
    bytes[0] = 0x00;
    
    std::copy(encode.begin()+index, encode.begin()+index+TW::Ripple::Address::hashSize, bytes.begin()+1);
    index += TW::Ripple::Address::hashSize;
    std::copy(encode.begin()+index, encode.begin()+index+flags.size(), flags.begin());
    index += flags.size();
    std::copy(encode.begin()+index, encode.begin()+index+tag.size(), tag.begin());
}

// JuBiter-defined
/// Returns a string representation of the address.
std::string TaggedAddresses::string() const {

    std::vector<byte> encode(size);

    size_t index = 0;
    std::copy(prefixes.begin(), prefixes.end(), encode.begin());
    index += prefixes.size();
    std::copy(bytes.begin(), bytes.end(), encode.begin()+index);
    index += bytes.size();
    std::copy(flags.begin(), flags.end(), encode.begin()+index);
    index += flags.size();
    std::copy(tag.begin(), tag.end(), encode.begin()+index);
    return Base58::ripple.encodeCheck(encode);
}


/// Returns r-address and tag of the X-address.
bool TaggedAddresses::parse(std::string& oAddress, uint64_t& oTag) const {
    
    if (!Address::isValid(bytes)) {
        return false;
    }
    
    Address r(bytes);
    oAddress = r.string();
    std::vector<byte> vTag(tag.size());
    std::copy(tag.begin(), tag.end(), vTag.begin());
    oTag = TW::decode64LE(&vTag[0]);

    return true;
}
// JuBiter-defined
bool TaggedAddresses::isValid(const std::string &string) {

    TW::Data decoded = Base58::ripple.decodeCheck(string);
    if (decoded.size() != TaggedAddresses::size) {
        return false;
    }
    auto tagged = TaggedAddresses(decoded);
    if (   TaggedAddresses::mainnetPrefix != TW::decode16BE(&tagged.prefixes[0])
        && TaggedAddresses::testnetPrefix != TW::decode16BE(&tagged.prefixes[0])
        ) {
        return false;
    }

    if (   JUB_TAGGED_ADDRESS_FLAGS_TYPE::NO_TAG != tagged.flags[0]
        && JUB_TAGGED_ADDRESS_FLAGS_TYPE::TAG_32 != tagged.flags[0]
        && JUB_TAGGED_ADDRESS_FLAGS_TYPE::TAG_64 != tagged.flags[0]
        ) {
        return false;
    }

    return true;
}
