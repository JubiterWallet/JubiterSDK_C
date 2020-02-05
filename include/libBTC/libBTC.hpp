#pragma once

#ifndef __libBTC__
#define __libBTC__

#include "JUB_SDK_BTC.h"
#include "context/BTCContext.h"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/Transaction.h"

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

constexpr JUB_BYTE  p2shVersion_QTUM = 0x32;
constexpr JUB_BYTE p2pkhVersion_QTUM = 0x3a;


JUB_RV verifyTx(const TWCoinType& coin,
                const TW::Bitcoin::Transaction& tx,
                const TWBitcoinSigHashType& hashType,
                const std::vector<JUB_UINT64>& vInputAmount,
                const std::vector<TW::PublicKey>& vInputPublicKey);


JUB_RV verifyTx(const bool witness,
                const TWCoinType& coin,
                const uchar_vector& signedRaw,
                const TWBitcoinSigHashType& hashType,
                const std::vector<JUB_UINT64>& vInputAmount,
                const std::vector<TW::Data>& vInputPublicKey, TWPublicKeyType publicKeyType);


JUB_RV verifyPayToPublicKeyHashScriptSig(const TWCoinType& coin,
                                         const TW::Bitcoin::Transaction& tx,
                                         const size_t index, const TWBitcoinSigHashType& hashType, const uint64_t amount,
                                         const TW::Data& signature,
                                         const TW::PublicKey publicKey,
                                         bool witness=false);


JUB_RV serializeUnsignedTx(const uint32_t coin,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           TW::Bitcoin::Transaction& tx);


JUB_RV serializeUnsignedTx(const uint32_t coin,
                           const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw);

JUB_RV serializeTx(const uint32_t coin,
                   const JUB_ENUM_BTC_TRANS_TYPE& type,
                   const std::vector<INPUT_BTC>& vInputs,
                   const std::vector<OUTPUT_BTC>& vOutputs,
                   const JUB_UINT32 lockTime,
                   const std::vector<uchar_vector>& vSignatureRaw,
                   uchar_vector& signedRaw);

JUB_RV buildScriptPubFromAddress(const std::string& address, uchar_vector& scriptPub);

JUB_RV serializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                           const std::vector<INPUT_BTC>& vInputs,
                           const std::vector<OUTPUT_BTC>& vOutputs,
                           const JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw);


} // namespace btc end
} // namespace jub end

#endif
