//
//  JUB_SDK_ETH.h
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_ETH_h
#define JUB_SDK_ETH_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


// Remove c++ features for swift framework
//typedef struct stContextCfgETH : stContextCfg {
typedef struct stContextCfgETH {
    JUB_CHAR_PTR            mainPath;
    JUB_UINT32              chainID;
//
//    stContextCfgETH();
//    virtual ~stContextCfgETH() {}
} CONTEXT_CONFIG_ETH;
// Remove c++ features for swift framework end


// Remove c++ features for swift framework
typedef struct stERC20TokenInfo {
    JUB_CHAR_PTR tokenName;
    JUB_UINT16 unitDP;
    JUB_CHAR_PTR contractAddress;

//    stERC20TokenInfo() {
//        unitDP = 0;
//    }
//    virtual ~stERC20TokenInfo() {}
} ERC20_TOKEN_INFO;
// Remove c++ features for swift framework end

/*****************************************************************************
 * @function name : JUB_CreateContextETH
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);


/*****************************************************************************
 * @function name : JUB_GetHDNodeETH
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX (0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01) for xpub
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);


/*****************************************************************************
 * @function name : JUB_GetMainHDNodeETH
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX (0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01) for xpub
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);


/*****************************************************************************
 * @function name : JUB_GetAddressETH
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);


/*****************************************************************************
 * @function name : JUB_SetMyAddressETH
 * @in  param : contextID - context ID
 *          : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           OUT JUB_CHAR_PTR_PTR address);


/*****************************************************************************
 * @function name : JUB_SignTransactionETH (EIP-155)
 * @in  param : contextID - context ID
 *          : path
 *          : nonce - nonce
 *          : gasLimit - gas limit
 *          : gasPriceInWei - gas price in wei
 *          : to
 *          : valueInWei - value in wei
 *          : input
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionETH(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_UINT32 nonce,
                              IN JUB_UINT32 gasLimit,
                              IN JUB_CHAR_CPTR gasPriceInWei,
                              IN JUB_CHAR_CPTR to,
                              IN JUB_CHAR_CPTR valueInWei,
                              IN JUB_CHAR_CPTR input,
                              OUT JUB_CHAR_PTR_PTR raw);


/*****************************************************************************
 * @function name : JUB_SignTypedTransactionETH (EIP-2718 -- EIP-2930)
 * @in  param : contextID - context ID
 *          : path
 *          : nonce - nonce
 *          : gasLimit - gas limit
 *          : gasPriceInWei - gas price in wei
 *          : to
 *          : valueInWei - value in wei
 *          : data
 *          : accessListInJSON - access list in JSON (See EIP-2930 for definition)
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTypedTransaction2930ETH(IN JUB_UINT16 contextID,
                                       IN BIP44_Path path,
                                       IN JUB_UINT32 nonce,
                                       IN JUB_UINT32 gasLimit,
                                       IN JUB_CHAR_CPTR gasPriceInWei,
                                       IN JUB_CHAR_CPTR to,
                                       IN JUB_CHAR_CPTR valueInWei,
                                       IN JUB_CHAR_CPTR data,
                                       IN JUB_CHAR_CPTR accessListInJSON,
                                       OUT JUB_CHAR_PTR_PTR raw);


/*****************************************************************************
 * @function name : JUB_SignTypedTransactionETH (EIP-2718 -- EIP-1559)
 * @in  param : contextID - context ID
 *          : path
 *          : nonce - nonce
 *          : gasLimit - the maximum amount of gas units that can be consumed by the transaction.
 *                  Units of gas represent computational steps
 *          : maxPriorityFeePerGas - the maximum amount of gas to be included as a tip to the miner
 *          : maxFeePerGas - the maximum amount of gas willing to be paid for the transaction
 *                      (inclusive of baseFeePerGas and maxPriorityFeePerGas)
 *          : destination - the receiving address (if an externally-owned account, the transaction will transfer value.
 *                   If a contract account, the transaction will execute the contract code)
 *          : valueInWei - amount of ETH to transfer from sender to recipient (in WEI, a denomination of ETH)
 *          : data - optional field to include arbitrary data
 *          : accessListInJSON - access list in JSON (See EIP-2930 for definition)
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTypedTransaction1559ETH(IN JUB_UINT16 contextID,
                                       IN BIP44_Path path,
                                       IN JUB_UINT32 nonce,
                                       IN JUB_UINT32 gasLimit,
                                       IN JUB_CHAR_CPTR maxPriorityFeePerGas,
                                       IN JUB_CHAR_CPTR maxFeePerGas,
                                       IN JUB_CHAR_CPTR destination,
                                       IN JUB_CHAR_CPTR valueInWei,
                                       IN JUB_CHAR_CPTR data,
                                       IN JUB_CHAR_CPTR accessListInJSON,
                                       OUT JUB_CHAR_PTR_PTR raw);


/*****************************************************************************
 * @function name : JUB_SignContractETH
 * @in  param : contextID - context ID
 *          : path
 *          : nonce - nonce
 *          : gasLimit - gas limit
 *          : gasPriceInWei - gas price in wei
 *          : to
 *          : valueInWei - value in wei
 *          : input
 * @out param : raw
 * @last change :
 * @note: Compatible with ERC-20.
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignContractETH(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           IN JUB_UINT32 nonce,
                           IN JUB_UINT32 gasLimit,
                           IN JUB_CHAR_PTR gasPriceInWei,
                           IN JUB_CHAR_PTR to,
                           IN JUB_CHAR_PTR valueInWei,
                           IN JUB_CHAR_PTR input,
                           OUT JUB_CHAR_PTR_PTR raw);


/*****************************************************************************
 * @function name : JUB_SignBytestringETH
 * @in  param : contextID - context ID
 *          : path
 *          : data - message to be signed
 * @out param : signature
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignBytestringETH(IN JUB_UINT16 contextID,
                             IN BIP44_Path path,
                             IN JUB_CHAR_CPTR data,
                             OUT JUB_CHAR_PTR_PTR signature);


/*****************************************************************************
 * @function name : JUB_SignTypedDataETH
 * @in  param : contextID - context ID
 *          : path
 *          : data - typed structured data in JSON
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTypedDataETH(IN JUB_UINT16 contextID,
                            IN BIP44_Path path,
                            IN JUB_CHAR_CPTR dataInJSON,
                            IN JUB_BBOOL isMetamaskV4Compat,
                            OUT JUB_CHAR_PTR_PTR signature);


/*****************************************************************************
 * @function name : JUB_SetERC20TokensETH
 * @in  param : contextID - context ID
 *          : tokens - token info list
 *          : iCount - token info count
 * @out param : ERC20 abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetERC20TokensETH(IN JUB_UINT16 contextID,
                             IN ERC20_TOKEN_INFO tokens[],
                             IN JUB_UINT16 iCount);


/*****************************************************************************
 * @function name : JUB_SetERC20TokenETH
 * @in  param : contextID - context ID
 *          : tokenName - ETH token name
 *          : unitDP - unit decimal place
 *          : contractAddress - contract address
 * @out param : none.
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetERC20TokenETH(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_CPTR tokenName,
                            IN JUB_UINT16 unitDP,
                            IN JUB_CHAR_CPTR contractAddress);


/*****************************************************************************
 * @function name : JUB_BuildERC20TransferAbiETH
 * @in  param : contextID - context ID
 *          : tokenTo - token to
 *          : tokenValue - value for token transaction
 * @out param : ERC20 abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildERC20TransferAbiETH(IN JUB_UINT16 contextID,
                                    IN JUB_CHAR_CPTR tokenTo, IN JUB_CHAR_CPTR tokenValue,
                                    OUT JUB_CHAR_PTR_PTR abi);


/*****************************************************************************
 * @function name : JUB_SetERC721TokenETH
 * @in  param : contextID - context ID
 *          : nfTokenName - ERC-721 Non-Fungible Token Name
 *          : contractAddress - ERC-721 Non-Fungible Token contract address
 * @out param : none,
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetERC721TokenETH(IN JUB_UINT16 contextID,
                             IN JUB_CHAR_CPTR nfTokenName,
                             IN JUB_CHAR_CPTR contractAddress);


/*****************************************************************************
 * @function name : JUB_BuildERC721TransferAbiETH
 * @in  param : contextID - context ID
 *          : tokenFrom - The current owner of the NFT
 *          : tokenTo - The new owner
 *          : tokenID - The NFT to transfer
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildERC721TransferAbiETH(IN JUB_UINT16 contextID,
                                     IN JUB_CHAR_CPTR tokenFrom, IN JUB_CHAR_CPTR tokenTo, IN JUB_CHAR_CPTR tokenID,
                                     OUT JUB_CHAR_PTR_PTR abi);


/*****************************************************************************
 * @function name : JUB_BuildERC1155TransferAbiETH
 * @in  param : contextID - context ID
 *          : tokenFrom - Source address
 *          : tokenTo - Target address
 *          : tokenID - ID of the token type
 *          : tokenValue - Transfer amount
 *          : data - Additional data with no specified format, MUST be sent unaltered in call to `onERC1155Received` on `_to`
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildERC1155TransferAbiETH(IN JUB_UINT16 contextID,
                                      IN JUB_CHAR_CPTR tokenFrom, IN JUB_CHAR_CPTR tokenTo,
                                      IN JUB_CHAR_CPTR tokenID, IN JUB_CHAR_CPTR tokenValue, IN JUB_CHAR_CPTR data,
                                      OUT JUB_CHAR_PTR_PTR abi);


/*****************************************************************************
 * @function name : JUB_BuildERC1155BatchTransferAbiETH
 * @in  param : contextID - context ID
 *          : tokenFrom - Source address
 *          : tokenTo - Target address
 *          : tokenID[] - IDs of each token type (order and length must match _values array)
 *          : tokenValue[] - Transfer amounts per token type (order and length must match _ids array)
 *          : data - Additional data with no specified format, MUST be sent unaltered in call to `onERC1155Received` on `_to`
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildERC1155BatchTransferAbiETH(IN JUB_UINT16 contextID,
                                           IN JUB_CHAR_CPTR tokenFrom, IN JUB_CHAR_CPTR tokenTo,
                                           IN JUB_CHAR_CPTR tokenIDs[], IN JUB_UINT16 idCount,
                                           IN JUB_CHAR_CPTR tokenValues[], IN JUB_UINT16 valueCount,
                                           IN JUB_CHAR_CPTR data,
                                           OUT JUB_CHAR_PTR_PTR abi);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_ETH_h */
