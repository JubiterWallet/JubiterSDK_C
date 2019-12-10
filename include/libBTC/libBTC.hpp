#pragma once

#ifndef __libBTC__
#define __libBTC__

#include "JUB_SDK_BTC.h"
#include "context/ContextBTC.h"

#include "mSIGNA/stdutils/uchar_vector.h"

namespace jub {

namespace btc {

constexpr JUB_UINT32 version1 = 0x01;
constexpr JUB_UINT32 sequence = 0xffffffff;
constexpr JUB_BYTE   marker	  = 0x00;
constexpr JUB_BYTE   flag     = 0x01;

constexpr JUB_BYTE p2shVersion = 0x05;
constexpr JUB_BYTE p2pkhVersion = 0x00;

constexpr JUB_BYTE p2shVersion_LTC = 0x32;
constexpr JUB_BYTE p2pkhVersion_LTC = 0x30;

constexpr char segwit_hrl[] = "bc";

JUB_RV buildScriptPubFromAddress(const std::string& address, uchar_vector& scriptPub);

JUB_RV serializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw);

} // namespace btc end

} // namespace jub end

#endif
