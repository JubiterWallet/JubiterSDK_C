#pragma once

#ifndef __libBTC__
#define __libBTC__

#include "JUB_SDK_BTC.h"

#include <vector>

#include "airbitz-core/abcd/util/Data.hpp"
#include "bitcoin/base58.h"
#include "bitcoin/bech32.h"
#include "machine/opcode.hpp"

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

template<int frombits, int tobits, bool pad>
bool Convertbits(abcd::DataChunk& out, const abcd::DataChunk& in) {

    int acc = 0;
    int bits = 0;
    const int maxv = (1 << tobits) - 1;
    const int maxAcc = (1 << (frombits + tobits - 1)) - 1;
    for (size_t i = 0; i < in.size(); ++i) {
        int value = in[i];
        acc = ((acc << frombits) | value) & maxAcc;
        bits += frombits;
        while (bits >= tobits) {
            bits -= tobits;
            out.push_back((acc >> bits) & maxv);
        }
    }

    if (pad) {
        if (bits) {
            out.push_back((acc << (tobits - bits)) & maxv);
        }
    }
    else if (   (bits >= frombits)
             || ((acc << (tobits - bits)) & maxv)
             ) {
        return false;
    }

    return true;
}

JUB_RV buildScriptPubFromAddress(std::string address, uchar_vector& scriptPub) {

    abcd::DataChunk vScriptPub;
    bool rv = base58::DecodeBase58Check(address, vScriptPub);
    if (rv) {
        if (   p2shVersion     == vScriptPub[0]
            || p2shVersion_LTC == vScriptPub[0]
            ) { // p2sh
            scriptPub << libbitcoin::machine::opcode::hash160;
            scriptPub & uchar_vector(vScriptPub.begin() + 1, vScriptPub.end());
            scriptPub << libbitcoin::machine::opcode::equal;
        }
        else if (   p2pkhVersion     == vScriptPub[0]
                 || p2pkhVersion_LTC == vScriptPub[0]
                 ) { //p2pkh
            scriptPub << libbitcoin::machine::opcode::dup;
            scriptPub << libbitcoin::machine::opcode::hash160;
            scriptPub & uchar_vector(vScriptPub.begin()+1, vScriptPub.end());
            scriptPub << libbitcoin::machine::opcode::equalverify;
            scriptPub << libbitcoin::machine::opcode::checksig;
        }
        else {
            return JUBR_ERROR;
        }

        return JUBR_OK;
    }
    else {
        std::pair<std::string, abcd::DataChunk> dec = bech32::decode(address);
        if (dec.first != std::string(segwit_hrl)) {
            return JUBR_ERROR;
        }
        abcd::DataChunk conv;
        Convertbits<5, 8, false>(conv, abcd::DataChunk(dec.second.begin() + 1, dec.second.end()));

        if (0x14 == conv.size()) { //p2wpkh
            scriptPub << (JUB_BYTE)0x00;
            scriptPub & conv;
        }
        else if (0x20 == conv.size()) { //p2wsh
            scriptPub << (JUB_BYTE)0x00;
            scriptPub & conv;
        }
        else {
            return JUBR_ERROR;
        }
    }

    return JUBR_ERROR;
}

JUB_RV serializeUnsignedTX(JUB_BTC_TRANS_TYPE type, std::vector<INPUT_BTC> vInputs,
                           std::vector<OUTPUT_BTC> vOutputs,
                           JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw) {

    uchar_vector unsingedTrans;
    unsingedTrans << version1;
    if (p2sh_p2wpkh == type) {
        unsingedTrans << marker;
        unsingedTrans << flag;
    }
    //input
    unsingedTrans << (JUB_BYTE)vInputs.size();
    for (auto input : vInputs) {
        //preHash
        uchar_vector preHash = std::string(input.preHash);
        preHash.reverse();
        unsingedTrans << preHash;
        //preIndex
        unsingedTrans << (JUB_UINT32)input.preIndex;
        //sig
        unsingedTrans << (JUB_BYTE)0;
        unsingedTrans << sequence;
    }
    //output
    unsingedTrans << (JUB_BYTE)vOutputs.size();
    for (auto output : vOutputs) {
        switch (output.type) {
        case OUTPUT_BTC_TYPE::STANDARD:
        {
            //amount
            unsingedTrans << (uint64_t)output.outputStandard.amount;
            //script_pub
            uchar_vector scriptPub;
            if (JUBR_OK != buildScriptPubFromAddress(output.outputStandard.address, scriptPub)) {
                return JUBR_ERROR;
            }
            unsingedTrans && scriptPub;
            break;
        } // case OUTPUT_BTC_TYPE::P2PKH end
        case OUTPUT_BTC_TYPE::RETURN0:
        {
            unsingedTrans << (uint64_t)output.outputReturn0.amount;
            uchar_vector scriptPub;
            scriptPub << (JUB_BYTE)0x6a; //op_return0
            scriptPub << (JUB_BYTE)output.outputReturn0.dataLen;
            scriptPub.insert(scriptPub.end(), output.outputReturn0.data, output.outputReturn0.data + output.outputReturn0.dataLen);

            unsingedTrans && scriptPub;
            break;
        } // case OUTPUT_BTC_TYPE::RETURN0 end
        default:
            break;
        } // switch (output.type) end
    }

    //witness 00
    if (p2sh_p2wpkh == type) {
        unsingedTrans << (JUB_BYTE)0x00;
    }

    unsingedTrans << lockTime;
    unsignedRaw = unsingedTrans;

    return JUBR_OK;
}

} // namespace btc end

} // namespace jub end

#endif
