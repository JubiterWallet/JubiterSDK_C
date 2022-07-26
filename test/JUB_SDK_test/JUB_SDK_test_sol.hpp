//
//  JUB_SDK_test_sol.hpp
//  JubSDKTest
//
//  Created by panmin on 2022/7/20.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_sol_hpp
#define JUB_SDK_test_sol_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_SOL.h"
//#include "../../include/JUB_SDK_BTC.h"


void SOL_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file);

//void set_my_address_test_ETH(JUB_UINT16 contextID);
void  get_address_pubkey_SOL(JUB_UINT16 contextID);

//void   transaction_test_ETH(JUB_UINT16 contextID, Json::Value root, bool bOptionalAccessLists=false, bool bWithAccessList=false);
//JUB_RV transaction_proc_ETH(JUB_UINT16 contextID, Json::Value root, bool bOptionalAccessLists=false, bool bWithAccessList=false);
//
//void   transaction_test_ERC20_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV transaction_proc_ERC20_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   transaction_test_ERC721_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV transaction_proc_ERC721_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   transaction_test_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV transaction_proc_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   batch_transaction_test_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV batch_transaction_proc_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   transaction_test_contr_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV transaction_proc_contr_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   bytestring_test_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV bytestring_proc_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   typedata_test_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV typedata_proc_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   uniswap_test_ETH(JUB_UINT16 contextID, Json::Value root);
//JUB_RV uniswap_proc_ETH(JUB_UINT16 contextID, Json::Value root);
//
//void   transaction_test_EIP1559_ETH(JUB_UINT16 contextID, Json::Value root, bool bWithAccessList=false);
//JUB_RV transaction_proc_EIP1559_ETH(JUB_UINT16 contextID, Json::Value root, bool bWithAccessList=false);

#endif /* JUB_SDK_test_sol_hpp */
