/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   libHC.h
 * Author: zxc_work
 *
 * Created on 2019年8月18日, 下午3:18
 */

#ifndef LIBHC_H
#define LIBHC_H

#include <vector>
#include "../JUB_SDK_COMM.h"
#include "mSIGNA/stdutils/uchar_vector.h"

namespace jub {

namespace hc {
    
#define hcashDPUB 0x02fda926 
#define hcashDPRV 0x02fda4e8
    
typedef struct {
	uchar_vector    preHash;
	JUB_UINT32      preIndex;
    JUB_UINT8       tree;
    JUB_UINT32      sequence;
    JUB_UINT64      amount;
    JUB_UINT32      blockHeight;
    JUB_UINT32      blockIndex;
    uchar_vector    scriptSig;
} INPUT_HCash;

typedef struct {
	JUB_UINT64      amount;
	JUB_UINT16      version;
    uchar_vector    scriptPk;
} OUTPUT_HCash;

typedef struct{
    JUB_UINT32                 version;
    std::vector<INPUT_HCash>   inputs;
    std::vector<OUTPUT_HCash>  outputs;
    JUB_UINT32                 lockTime;
    JUB_UINT32                 expiry;
} TX_Hcash;

typedef enum {
    TxSerializeFull = 0,
    TxSerializeNoWitness,
    TxSerializeOnlyWitness,
    TxSerializeWitnessSigning,
    TxSerializeWitnessValueSigning,
} JUB_HC_SERIALIZE_TYPE;

JUB_RV DeserializeTx(const std::string& raw, TX_Hcash& tx);
JUB_RV SerializeTx(const TX_Hcash& tx, std::string& raw);
JUB_RV SerializePreimage(const TX_Hcash& tx, const size_t i, const uchar_vector& redeemScript, uchar_vector& preimage);
}//namespace HC

}//namespace JUB

#endif /* LIBHC_H */

