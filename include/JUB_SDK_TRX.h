//
//  JUB_SDK_TRX.h
//  JubSDK
//
//  Created by Pan Min on 2020/09/10.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_TRX_h
#define JUB_SDK_TRX_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef CONTEXT_CONFIG CONTEXT_CONFIG_TRX;

// Remove c++ features for swift framework
//typedef enum class JubTRXContractType {
typedef enum {
     NS_ITEM_TRX_CONTRACT =  0,
            XFER_CONTRACT =  1, // TransferContract(balance_contract.proto)
      XFER_ASSET_CONTRACT =  2, // TransferAssetContract(asset_issue_contract.proto)
         FRZ_BLA_CONTRACT = 11, // TransferContract(balance_contract.proto)
       UNFRZ_BLA_CONTRACT = 12, // TransferContract(balance_contract.proto)
    CREATE_SMART_CONTRACT = 30, // CreateSmartContract(smart_contract.proto)
      TRIG_SMART_CONTRACT = 31, // TriggerSmartContract(smart_contract.proto)
} JUB_ENUM_TRX_CONTRACT_TYPE;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stTronTransferContract {
    JUB_CHAR_PTR owner_address;
    JUB_CHAR_PTR    to_address;
    JUB_INT64 amount;
//
//     stTronTransferContract();
//    ~stTronTransferContract() = default;
} JUB_XFER_CONTRACT_TRX;
typedef JUB_XFER_CONTRACT_TRX* JUB_XFER_CONTRACT_TRX_PTR;
// Remove c++ features for swift framework end

// TRC-10(https://tronprotocol.github.io/documentation-en/mechanism-algorithm/trc10/)
// Remove c++ features for swift framework
typedef struct stTronTransferAssetContract {
    JUB_CHAR_PTR asset_name;
    JUB_CHAR_PTR owner_address;
    JUB_CHAR_PTR    to_address;
    JUB_INT64       amount;
//
//     stTronTransferAssetContract();
//    ~stTronTransferAssetContract() = default;
} JUB_XFER_ASSET_CONTRACT_TRX;
typedef JUB_XFER_ASSET_CONTRACT_TRX* JUB_XFER_ASSET_CONTRACT_TRX_PTR;
// Remove c++ features for swift framework end

typedef enum {
    BANDWIDTH = 0x00,
       ENERGY = 0x01
} JUB_ENUM_RESOURCE_CODE;

// https://cn.developers.tron.network/reference#freezebalance
// Remove c++ features for swift framework
typedef struct stTronFreezeBalanceContract {
    JUB_CHAR_PTR owner_address;
    JUB_INT64   frozen_balance;
    JUB_INT64   frozen_duration;
    JUB_ENUM_RESOURCE_CODE resource;
    JUB_CHAR_PTR receiver_address;
//
//     stTronFreezeBalanceContract();
//    ~stTronFreezeBalanceContract() = default;
} JUB_FRZ_BLA_CONTRACT_TRX;
typedef JUB_FRZ_BLA_CONTRACT_TRX* JUB_FRZ_BLA_CONTRACT_TRX_PTR;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stTronUnfreezeBalanceContract {
    JUB_CHAR_PTR owner_address;
    JUB_ENUM_RESOURCE_CODE resource;
    JUB_CHAR_PTR receiver_address;
//
//     stTronUnfreezeBalanceContract();
//    ~stTronUnfreezeBalanceContract() = default;
} JUB_UNFRZ_BLA_CONTRACT_TRX;
typedef JUB_UNFRZ_BLA_CONTRACT_TRX* JUB_UNFRZ_BLA_CONTRACT_TRX_PTR;
// Remove c++ features for swift framework end

// TRC-20(https://tronprotocol.github.io/documentation-en/contracts/trc20/)
// Remove c++ features for swift framework
typedef struct stTronCreateSmartContract {
    JUB_CHAR_PTR owner_address;
    JUB_CHAR_PTR bytecode;
    JUB_INT64 call_token_value;
    JUB_INT64 token_id;
//
//    stTronCreateSmartContract();
//   ~stTronCreateSmartContract() = default;
} JUB_CREATE_SMART_CONTRACT_TRX;
typedef JUB_CREATE_SMART_CONTRACT_TRX* JUB_CREATE_SMART_CONTRACT_TRX_PTR;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stTronTriggerSmartContract {
    JUB_CHAR_PTR owner_address;
    JUB_CHAR_PTR contract_address;
    JUB_INT64 call_value;
    JUB_CHAR_PTR data;
    JUB_INT64 call_token_value;
    JUB_INT64 token_id;
//
//    stTronTriggerSmartContract();
//   ~stTronTriggerSmartContract() = default;
} JUB_TRIG_SMART_CONTRACT_TRX;
typedef JUB_TRIG_SMART_CONTRACT_TRX* JUB_TRIG_SMART_CONTRACT_TRX_PTR;
// Remove c++ features for swift framework end

//// Remove c++ features for swift framework
//// TRC-20
//typedef struct stTronShieldedTRC20Contract {
//    JUB_CHAR_PTR from_address;
//    JUB_INT64    from_amount;
//    JUB_CHAR_PTR   to_address;
//    JUB_INT64      to_amount;
////
////     stTronShieldedTRC20Contract();
////    ~stTronShieldedTRC20Contract() = default;
//} JUB_SHIELDED_TRC20_CONTRACT_TRX;
//typedef JUB_SHIELDED_TRC20_CONTRACT_TRX* JUB_SHIELDED_TRC20_CONTRACT_TRX_PTR;
//// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stTronContract {
    JUB_ENUM_TRX_CONTRACT_TYPE type;
    union {
              JUB_XFER_CONTRACT_TRX transfer;
        JUB_XFER_ASSET_CONTRACT_TRX transferAsset;
           JUB_FRZ_BLA_CONTRACT_TRX freezeBalance;
         JUB_UNFRZ_BLA_CONTRACT_TRX unfreezeBalance;
      JUB_CREATE_SMART_CONTRACT_TRX createSmart;
        JUB_TRIG_SMART_CONTRACT_TRX triggerSmart;
    };
//
//     stTronContract();
//    ~stTronContract() = default;
} JUB_CONTRACT_TRX;
typedef JUB_CONTRACT_TRX* JUB_CONTRACT_TRX_PTR;
// Remove c++ features for swift framework end

//// Remove c++ features for swift framework
//message raw {
//  bytes ref_block_bytes = 1;
//  int64 ref_block_num = 3;
//  bytes ref_block_hash = 4;
//  int64 expiration = 8;
//  repeated authority auths = 9;
//  // transaction note
//  bytes data = 10;
//  //only support size = 1, repeated list here for extension
//  repeated Contract contract = 11;
//  // scripts not used
//  bytes scripts = 12;
//  int64 timestamp = 14;
//  int64 fee_limit = 18;
//}
typedef struct stTxTRX {
    JUB_CHAR_PTR ref_block_bytes;
    JUB_CHAR_PTR ref_block_num; // [Optional]
    JUB_CHAR_PTR ref_block_hash;
    JUB_CHAR_PTR expiration;
    JUB_CHAR_PTR data;          // [Optional]
    JUB_CONTRACT_TRX_PTR contracts;
    JUB_UINT16 contractCount;
    JUB_CHAR_PTR scripts;       // [Optional]
    JUB_CHAR_PTR timestamp;
    JUB_CHAR_PTR fee_limit;     // [Optional]
//
//     stTxTRX();
//    ~stTxTRX() = default;
//
//    bool isValid() const;
} JUB_TX_TRX;
//// Remove c++ features for swift framework end


/*****************************************************************************
 * @function name : JUB_CreateContextTRX
 * @in  param : cfg
 *            : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextTRX(IN CONTEXT_CONFIG_TRX cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressTRX
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressTRX(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
//                         IN BIP48_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_CheckAddressTRX
 * @in  param : contextID - context ID
 *          : address
 * @out param : addrInHex: base58 decode
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressTRX(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address, OUT JUB_CHAR_PTR_PTR addrInHex);

/*****************************************************************************
 * @function name : JUB_GetHDNodeTRX
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeTRX(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
//                        IN BIP48_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeTRX
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeTRX(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

///*****************************************************************************
// * @function name : JUB_SetMyAddressTRX
// * @in  param : contextID - context ID
// *            : path
// * @out param : address
// * @last change :
// *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_SetMyAddressTRX(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
////                           IN BIP48_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
//
/*****************************************************************************
 * @function name : JUB_SignTransactionTRX
 * @in  param : contextID - context ID
 *          : path
 *          : packedContractInPb - packed contract in protobuf
 * @out param : rawInJSON
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionTRX(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_CHAR_CPTR packedContractInPb,
                              OUT JUB_CHAR_PTR_PTR rawInJSON);


/*****************************************************************************
 * @function name : JUB_SetTRC10Asset
 * @in  param : contextID - context ID
 *          : assetName - TRX asset name
 *          : unitDP - unit decimal place
 *          : assetID - asset ID
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTRC10Asset(IN JUB_UINT16 contextID,
                         IN JUB_CHAR_CPTR assetName,
                         IN JUB_UINT16 unitDP,
                         IN JUB_CHAR_CPTR assetID);


/*****************************************************************************
 * @function name : JUB_SetTRC20Token
 * @in  param : contextID - context ID
 *          : tokenName - TRX token name
 *          : unitDP - unit decimal place
 *          : contractAddress - contract address
 * @out param : none.
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTRC20Token(IN JUB_UINT16 contextID,
                         IN JUB_CHAR_CPTR tokenName,
                         IN JUB_UINT16 unitDP,
                         IN JUB_CHAR_CPTR contractAddress);


/*****************************************************************************
 * @function name : JUB_BuildTRC20TransferAbi
 * @in  param : contextID - context ID
 *          : tokenTo - token to
 *          : tokenValue - value for token transaction
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildTRC20TransferAbi(IN JUB_UINT16 contextID,
                                 IN JUB_CHAR_CPTR tokenTo, IN JUB_CHAR_CPTR tokenValue,
                                 OUT JUB_CHAR_PTR_PTR abi);


/*****************************************************************************
 * @function name : JUB_SetTRC721Token
 * @in  param : contextID - context ID
 *          : nfTokenName - ERC-721 Non-Fungible Token Name
 *          : contractAddress - ERC-721 Non-Fungible Token contract address
 * @out param : none.
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTRC721Token(IN JUB_UINT16 contextID,
                          IN JUB_CHAR_CPTR nfTokenName,
                          IN JUB_CHAR_CPTR contractAddress);


/*****************************************************************************
 * @function name : JUB_BuildTRC721TransferAbi
 * @in  param : contextID - context ID
 *          : tokenFrom - The current owner of the NFT
 *          : tokenTo - The new owner
 *          : tokenID - The NFT to transfer
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildTRC721TransferAbi(IN JUB_UINT16 contextID,
                                  IN JUB_CHAR_CPTR tokenFrom, IN JUB_CHAR_CPTR tokenTo, IN JUB_CHAR_CPTR tokenID,
                                  OUT JUB_CHAR_PTR_PTR abi);


/*****************************************************************************
 * @function name : JUB_PackContractTRX
 * @in  param : contextID - context ID
 *          : tx - transaction
 * @out param : packedContractInPB - packed contract in protobuf
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_PackContractTRX(IN JUB_UINT16 contextID,
                           IN JUB_TX_TRX tx,
                           OUT JUB_CHAR_PTR_PTR packedContractInPB);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_TRX_h */
