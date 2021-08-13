// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../NervosCKB/Script.h"
#include "../NervosCKB/WitnessArgs.h"
#include "../NervosCKB/Serialization.hpp"


using namespace TW;
using namespace TW::NervosCKB;

extern const uint8_t personal_ckb[16];


size_t WitnessArgs::size() const {

    if (isZero()) {
        return sizeZero();
    }

    return (Script(std::begin(lock), std::end(lock)).encode().size()
            + inputType.size()
            + outputType.size());
}


size_t WitnessArgs::sizeZero() const {

    return sizeof(uint64_t)/sizeof(uint8_t);
}


// Preimage is null, i.e. Encode64LE (0x00000000)
bool WitnessArgs::isZero() const {

    return (0 == lock.size());
}


Data WitnessArgs::encodeZero() const {

    return encode64LE(0x00);
}


Data WitnessArgs::serialize() const {

    if (isZero()) {
        return encodeZero();
    }

    return serializeTable(WitnessArgs::serialize(*this));
}


std::vector<TW::Data> WitnessArgs::serialize(const WitnessArgs& args) {

    std::vector<TW::Data> table;
    table.push_back(Script(std::begin(args.lock), std::end(args.lock)).encode());
    table.push_back(args.inputType);
    table.push_back(args.outputType);

    return table;
}
