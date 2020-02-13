// Copyright © 2017-2019 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

// JuBiter-defined
static const uint32_t TxSerializeFull = 0x00;
static const uint32_t TxSerializeNoWitness = 0x01;
static const uint32_t TxSerializeOnlyWitness = 0x02;
static const uint32_t TxSerializeWitnessSigning = 0x03;
static const uint32_t TxSerializeWitnessValueSigning = 0x04;

// JuBiter-defined
extern bool TxSerializeIsFull(uint32_t type);

TW_EXTERN_C_END

