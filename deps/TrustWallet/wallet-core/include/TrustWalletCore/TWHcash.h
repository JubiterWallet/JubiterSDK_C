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
// Hash type bits from the end of a signature.
static const uint32_t SigHashOld = 0x00;
static const uint32_t SigHashAll = 0x01;
static const uint32_t SigHashNone =  0x02;
static const uint32_t SigHashSingle = 0x03;
static const uint32_t SigHashAllValue = 0x04;
static const uint32_t SigHashAnyOneCanPay = 0x80;
// sigHashMask defines the number of bits of the hash type which is used
// to identify which outputs are signed.
static const uint32_t sigHashMask = 0x1F;

// JuBiter-defined
extern bool SigHashIsAll(uint32_t type);

TW_EXTERN_C_END

