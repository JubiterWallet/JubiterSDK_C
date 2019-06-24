#pragma once

#ifndef __libBTC__
#define __libBTC__

#include <JUB_SDK.h>
#include <vector>
#include <formats/base58.h>
#include <formats/bech32.h>

namespace jub {

namespace btc {

constexpr JUB_UINT32 Version1 = 0x01;
constexpr JUB_UINT32 sequence = 0xffffffff;
constexpr JUB_BYTE   marker	  = 0x00;
constexpr JUB_BYTE   flag     = 0x01;

constexpr JUB_BYTE p2sh_version = 0x05;
constexpr JUB_BYTE p2pkh_version = 0x00;

constexpr JUB_BYTE p2sh_version_LTC = 0x32;
constexpr JUB_BYTE p2pkh_version_LTC = 0x30;

constexpr char segwit_hrl[] = "bc";

typedef std::vector<uint8_t> bech32_data;

template<int frombits, int tobits, bool pad>
bool convertbits(bech32_data& out, const bech32_data& in) {

    int acc = 0;
    int bits = 0;
    const int maxv = (1 << tobits) - 1;
    const int max_acc = (1 << (frombits + tobits - 1)) - 1;
    for (size_t i = 0; i < in.size(); ++i) {
        int value = in[i];
        acc = ((acc << frombits) | value) & max_acc;
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

JUB_RV buildScriptPubFromAddress(std::string address, uchar_vector& script_pub) {

    std::vector<JUB_BYTE> vScript_pub;
    bool rv = base58::DecodeBase58Check(address, vScript_pub);
    if (rv) {
        if (   p2sh_version     == vScript_pub[0]
            || p2sh_version_LTC == vScript_pub[0]
            ) { // p2sh
            script_pub << OP_HASH160;
            script_pub & uchar_vector(vScript_pub.begin() + 1, vScript_pub.end());
            script_pub << OP_EQUAL;
        }
        else if (   p2pkh_version     == vScript_pub[0]
                 || p2pkh_version_LTC == vScript_pub[0]
                 ) { //p2pkh
            script_pub << OP_DUP;
            script_pub << OP_HASH160;
            script_pub & uchar_vector(vScript_pub.begin()+1,vScript_pub.end());
            script_pub << OP_EQUALVERIFY;
            script_pub << OP_CHECKSIG;
        }
        else {
            return JUBR_ERROR;
        }

        return JUBR_OK;
    }
    else {
        std::pair<std::string, bech32_data> dec = bech32::decode(address);
        if (dec.first != std::string(segwit_hrl)) {
            return JUBR_ERROR;
        }
        bech32_data conv;
        convertbits<5, 8, false>(conv, bech32_data(dec.second.begin() + 1, dec.second.end()));

        if (0x14 == conv.size()) { //p2wpkh
            script_pub << (JUB_BYTE)0x00;
            script_pub & conv;
        }
        else if (0x20 == conv.size()) { //p2wsh
            script_pub << (JUB_BYTE)0x00;
            script_pub & conv;
        }
        else {
            return JUBR_ERROR;
        }
    }

    return JUBR_ERROR;
}

JUB_RV serializeUnsignedTX(JUB_BTC_TRANS_TYPE type,std::vector<INPUT_BTC> inputs,
                           std::vector<OUTPUT_BTC> outputs,
                           JUB_UINT32 lockTime,
                           uchar_vector& unsigned_raw) {

    uchar_vector unsinged_trans;
    unsinged_trans << Version1;
    if (p2sh_p2wpkh == type) {
        unsinged_trans << marker;
        unsinged_trans << flag;
    }
    //input
    unsinged_trans << (JUB_BYTE)inputs.size();
    for (auto input : inputs) {
        //preHash
        uchar_vector preHash = std::string(input.preHash);
        preHash.reverse();
        unsinged_trans << preHash;
        //preIndex
        unsinged_trans << (JUB_UINT32)input.preIndex;
        //sig
        unsinged_trans << (JUB_BYTE)0;
        unsinged_trans << sequence;
    }
    //output
    unsinged_trans << (JUB_BYTE)outputs.size();
    for (auto output : outputs) {
        switch (output.type) {
        case OUTPUT_BTC_TYPE::P2PKH:
        {
            //amount
            unsinged_trans << (uint64_t)output.output_p2pkh.amount;
            //script_pub
            uchar_vector script_pub;
            if (JUBR_OK != buildScriptPubFromAddress(output.output_p2pkh.address, script_pub)) {
                return JUBR_ERROR;
            }
            unsinged_trans && script_pub;
            break;
        } // case OUTPUT_BTC_TYPE::P2PKH end
        case OUTPUT_BTC_TYPE::RETURN0:
        {
            unsinged_trans << (uint64_t)output.output_return0.amount;
            uchar_vector script_pub;
            script_pub <<(JUB_BYTE) 0x6a; //op_return0
            script_pub <<(JUB_BYTE)output.output_return0.data_len;
            script_pub.insert(script_pub.end(), output.output_return0.data, output.output_return0.data + output.output_return0.data_len);

            unsinged_trans && script_pub;
            break;
        } // case OUTPUT_BTC_TYPE::RETURN0 end
        default:
            break;
        } // switch (output.type) end
    }

    //witness 00
    if (p2sh_p2wpkh == type) {
        unsinged_trans << (JUB_BYTE)0x00;
    }

    unsinged_trans << lockTime;
    unsigned_raw = unsinged_trans;

    return JUBR_OK;
}

} // namespace btc end

} // namespace jub end

#endif
