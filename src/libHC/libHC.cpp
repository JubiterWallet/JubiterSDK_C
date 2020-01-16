/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "libHC/libHC.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <TrezorCrypto/blake256.h>
#include "utility/util.h"

namespace jub {
namespace hc {


JUB_RV DeserializeTx(const std::string& raw, TX_Hcash& tx) {

    TX_Hcash txTemp;
    uchar_vector rawTx = raw;
    try {
        //only support full
        txTemp.version = rawTx.read_le_uint32();
        auto input_num = rawTx.read_uint8();
        //input
        for (size_t i = 0; i < input_num;i++) {
            INPUT_HCash input;
            uchar_vector preHash = rawTx.read_vector(32);
            input.preHash = preHash.getReverse();
            input.preIndex = rawTx.read_le_uint32();
            input.tree = rawTx.read_uint8();
            input.sequence = rawTx.read_le_uint32();
            txTemp.inputs.emplace_back(input);
        }
        auto output_num = rawTx.read_uint8();
        for (size_t i =0 ; i< output_num ; i++) {
            OUTPUT_HCash output;
            output.amount = rawTx.read_le_uint64();
            output.version = rawTx.read_le_uint16();
            auto scriptPk_size = rawTx.read_compact_size();
            output.scriptPk = rawTx.read_vector(scriptPk_size);
            txTemp.outputs.emplace_back(output);
        }
        txTemp.lockTime = rawTx.read_le_uint32();
        txTemp.expiry = rawTx.read_le_uint32();
        //witness begin
        auto input_num_witness = rawTx.read_uint8();
        if (input_num != input_num_witness) {
            JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
        }

        for (size_t i = 0; i < input_num;i++) {
            txTemp.inputs[i].amount = rawTx.read_le_uint64();
            txTemp.inputs[i].blockHeight = rawTx.read_le_uint32();
            txTemp.inputs[i].blockIndex = rawTx.read_le_uint32();
            auto scriptSig_size = rawTx.read_compact_size();
            if (scriptSig_size != 0x00) {
                txTemp.inputs[i].scriptSig = rawTx.read_vector(scriptSig_size);
            }
        }
        tx = txTemp;
    }
    catch(...) {
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }

    return JUBR_OK;
}

JUB_RV SerializeTx(const TX_Hcash& tx, std::string& raw) {

    uchar_vector rawTx;
    //only support full
    rawTx << (tx.version | ((JUB_UINT32)TxSerializeFull<<16));
    //not support many inputs
    rawTx << (JUB_UINT8)tx.inputs.size();

    for (INPUT_HCash input:tx.inputs) {
        rawTx << input.preHash.getReverse();
        rawTx << input.preIndex;
        rawTx << input.tree;
        rawTx << input.sequence;
    }
    //output
    rawTx << (JUB_UINT8)tx.outputs.size();
    for (OUTPUT_HCash output:tx.outputs) {
        rawTx << (uint64_t)output.amount;
        rawTx << output.version;
        rawTx && output.scriptPk;
    }

    rawTx << tx.lockTime;
    rawTx << tx.expiry;

    //witness
    rawTx << (JUB_UINT8)tx.inputs.size();
    for (INPUT_HCash input:tx.inputs) {
        rawTx << (uint64_t)input.amount;
        rawTx << input.blockHeight;
        rawTx << input.blockIndex;
        if (0 == input.scriptSig.size()) {
            rawTx << (JUB_UINT8)0x00;
        }
        else {
            rawTx && input.scriptSig;
        }
    }

    raw = rawTx.getHex();

    return JUBR_OK;
}

JUB_RV SerializePreimage(const TX_Hcash& tx , size_t i, const uchar_vector& redeemScript, uchar_vector& preimage) {

    uchar_vector rawTx;

    //only TxSerializeNoWitness
    rawTx << (tx.version | ((JUB_UINT32)TxSerializeNoWitness<<16));
    //not support many inputs
    rawTx << (JUB_UINT8)tx.inputs.size();

    for (INPUT_HCash input:tx.inputs) {
        rawTx << input.preHash.getReverse();
        rawTx << input.preIndex;
        rawTx << input.tree;
        rawTx << input.sequence;
    }

    //output
    rawTx << (JUB_UINT8)tx.outputs.size();
    for (OUTPUT_HCash output:tx.outputs) {
        rawTx << (uint64_t)output.amount;
        rawTx << output.version;
        rawTx && output.scriptPk;
    }

    rawTx << tx.lockTime;
    rawTx << tx.expiry;

    JUB_BYTE hash [32] = {0};
    blake256(&rawTx[0], rawTx.size(), hash);
    uchar_vector NoWitnessHash(hash, 32);
    rawTx.clear();

    //only TxSerializeWitnessSigning
    rawTx << (tx.version | ((JUB_UINT32)TxSerializeWitnessSigning<<16));

    rawTx << (JUB_UINT8)tx.inputs.size();
    for (size_t j =0; j < tx.inputs.size(); j++) {
        if (i == j) {
            rawTx && redeemScript;
        }
        else {
            rawTx << (JUB_UINT8)0x00;
        }
    }

    blake256(&rawTx[0], rawTx.size(), hash);
    uchar_vector OnlyWitnessHash(hash, 32);
    rawTx.clear();

    preimage << (JUB_UINT32)0x01;
    preimage << NoWitnessHash << OnlyWitnessHash;

    return JUBR_OK;
}


} // namespace hc end
} // namespace jub end
