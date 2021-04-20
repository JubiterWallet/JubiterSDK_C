//
//  JUB_SDK_test_fil.hpp
//  JubSDKTest
//
//  Created by panmin on 2020/11/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_fil_hpp
#define JUB_SDK_test_fil_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_FIL.h"

void FIL_test(const char* json_file);

void set_my_address_test_FIL(JUB_UINT16 contextID);
void  get_address_pubkey_FIL(JUB_UINT16 contextID);

void   transaction_test_FIL(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_FIL(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_fil_hpp */

//return Cbor::Encode::array({
//    Cbor::Encode::uint(0),                        // version
//    Cbor::Encode::bytes(to.bytes),                // to address
//    Cbor::Encode::bytes(from.bytes),              // from address
//    Cbor::Encode::uint(nonce),                    // nonce
//    Cbor::Encode::bytes(encodeVaruint(value)),    // value
//    Cbor::Encode::bytes(encodeVaruint(gasPrice)), // gas price
//    cborGasLimit,                                 // gas limit
//    Cbor::Encode::uint(0),                        // abi.MethodNum (0 => send)
//    Cbor::Encode::bytes(Data())                   // data (empty)
//});
//89005501b4cc7e39534b3c2bd01bd4cb4fb631a6d2ee840b583103a0313631258c08c529834a5ca414928f3cc00907ecbf8e7be95d204b29a7b458e0997f3fb0cb9a75390b7a928ce109d7184142001c460002540be4001a046bba0a0040
//89
//00
//55 01b4cc7e39534b3c2bd01bd4cb4fb631a6d2ee840b
//58 31 03a0313631258c08c529834a5ca414928f3cc00907ecbf8e7be95d204b29a7b458e0997f3fb0cb9a75390b7a928ce109d7
//1841
//42001c
//460002540be400
//1a046bba0a
//00
//40
