#pragma once

#ifndef __libBCH__
#define __libBCH__

#include <JUB_SDK.h>
#include <vector>
#include <libBCH/cashaddr.h>

namespace jub {

namespace bch {

constexpr JUB_UINT32 version1 = 0x01;
constexpr JUB_UINT32 sequence = 0xffffffff;

typedef std::vector<uint8_t> Data;

constexpr JUB_BYTE TypeBitsP2PKH = 0x00;
constexpr JUB_BYTE TypeBitsP2SH  = 0x08;

template<int frombits, int tobits, bool pad>
bool convertbits(Data& out, const Data& in) {

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

JUB_RV buildScriptPubFromAddress(std::string address, uchar_vector& scriptPub) {

    auto pubkeyDataBit5 = cashaddr::Decode(address);
    Data pubkeyHash;
    convertbits<5, 8, false>(pubkeyHash, Data(pubkeyDataBit5.second.begin(), pubkeyDataBit5.second.end()));

    if (TypeBitsP2PKH == pubkeyHash[0]) { //p2pkh
        scriptPub << OP_DUP;
        scriptPub << OP_HASH160;
        scriptPub & uchar_vector(pubkeyHash.begin() + 1, pubkeyHash.end());
        scriptPub << OP_EQUALVERIFY;
        scriptPub << OP_CHECKSIG;
    }
    else if (TypeBitsP2SH == pubkeyHash[0]) { //p2sh
        scriptPub << OP_HASH160;
        scriptPub & uchar_vector(pubkeyHash.begin() + 1, pubkeyHash.end());
        scriptPub << OP_EQUAL;
    }
    else {
        return JUBR_ERROR;
    }

    return JUBR_OK;
}

JUB_RV serializeUnsignedTX(JUB_BTC_TRANS_TYPE type,
                           std::vector<INPUT_BTC> vInputs,
                           std::vector<OUTPUT_BTC> vOutputs,
                           JUB_UINT32 lockTime,
                           uchar_vector& unsignedRaw) {

    uchar_vector unsingedTrans;
    unsingedTrans << version1;

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
        case OUTPUT_BTC_TYPE::P2PKH:
        {
            //amount
            unsingedTrans << (uint64_t)output.outputP2pkh.amount;
            //script_pub
            uchar_vector scriptPub;
            if (JUBR_OK != buildScriptPubFromAddress(output.outputP2pkh.address, scriptPub)) {
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
            scriptPub << output.outputReturn0.dataLen;
            scriptPub.insert(scriptPub.end(), output.outputReturn0.data, output.outputReturn0.data + output.outputReturn0.dataLen);

            unsingedTrans && scriptPub;
            break;
        } // case OUTPUT_BTC_TYPE::RETURN0 end
        default:
            break;
        } // switch (output.type) end
    }

    unsingedTrans << lockTime;
    unsignedRaw = unsingedTrans;

    return JUBR_OK;
}

} // namespace bch end

} // namespace jub end

#endif
