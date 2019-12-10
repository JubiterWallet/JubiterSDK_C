#pragma once

#ifndef __libBCH__
#define __libBCH__

#include "JUB_SDK.h"

#include <vector>

#include "airbitz-core/abcd/util/Data.hpp"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "bitcoinABC/cashaddr.h"
#include "machine/opcode.hpp"

namespace jub {

namespace bch {

constexpr JUB_UINT32 version1 = 0x01;
constexpr JUB_UINT32 sequence = 0xffffffff;

constexpr JUB_BYTE TypeBitsP2PKH = 0x00;
constexpr JUB_BYTE TypeBitsP2SH  = 0x08;

JUB_RV buildScriptPubFromAddress(const std::string& address, uchar_vector& scriptPub);

JUB_RV serializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw);
} // namespace bch end

} // namespace jub end

#endif
