// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <vector>
#include <map>

using namespace TW;

namespace TW::NervosCKB {

struct emptyWitness {
    Data lock = Data(65);
};


struct WitnessArgs {
    Data lock = TW::Data(65);
    Data  inputType = TW::Data(0);
    Data outputType = TW::Data(0);

    WitnessArgs(const Data& lock=Data(65), uint8_t inputType=0, uint8_t outputType=0)
        : lock(std::move(lock)), inputType(inputType), outputType(outputType) {}
    WitnessArgs(const uint8_t zero)
        : inputType(Data(0)), outputType(Data(0)) {
            lock.clear();
        }
    ~WitnessArgs() {}

    WitnessArgs& operator=(const WitnessArgs& rhs) {

        this->lock.clear();
        this->lock.insert(std::end(this->lock), std::begin(rhs.lock), std::end(rhs.lock));

        this->inputType.clear();
        this->inputType.insert(std::end(this->inputType), std::begin(rhs.inputType), std::end(rhs.inputType));

        this->outputType.clear();
        this->outputType.insert(std::end(this->outputType), std::begin(rhs.outputType), std::end(rhs.outputType));

        return *this;
    }

    bool operator==(const WitnessArgs& rhs) {
        return       this->lock == rhs.lock
            && this->inputType  == rhs.inputType
            && this->outputType == rhs.outputType;
    }

    size_t size() const;
    size_t sizeZero() const;

    // Preimage is null, i.e. Encode64LE (0x00000000)
    bool isZero() const;

    virtual Data encodeZero() const;

    /// Serializes the witnessArgs into the provided buffer.
    virtual Data serialize() const;

    static std::vector<Data> serialize(const WitnessArgs& args);
};

} // namespace TW::NervosCKB
