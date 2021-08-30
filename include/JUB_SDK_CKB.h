//
//  JUB_SDK_CKB.h
//  JubSDK
//
//  Created by panmin on 2021/6/15.
//  Copyright © 2021 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_CKB_h
#define JUB_SDK_CKB_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


// Remove c++ features for swift framework
typedef struct stContextCfgCKB {
    JUB_CHAR_PTR            mainPath;
    JUB_ENUM_BOOL           mainNet;
//
//     stContextCfgCKB();
//    virtual ~stContextCfgCKB() {}
} CONTEXT_CONFIG_CKB;
// Remove c++ features for swift framework end


// Remove c++ features for swift framework
typedef struct stCellScript {
    JUB_CHAR_PTR    codeHash;
    JUB_UINT16      hashType;
    JUB_CHAR_PTR    args;
//
//     stCellScript();
//    ~stCellScript() = default;
} CELL_SCRIPT;
// Remove c++ features for swift framework end


// Remove c++ features for swift framework
typedef struct stCellDep {
    JUB_UINT8       type;
    JUB_CHAR_PTR    txHash;
    JUB_UINT32      index;
//
//     stCellDep();
//    ~stCellDep() = default;
} CELL_DEP;
// Remove c++ features for swift framework end


// Remove c++ features for swift framework
typedef struct stCellInput {
    JUB_CHAR_PTR    preHash;
    JUB_UINT16      preIndex;

    JUB_UINT64      since;

    JUB_UINT64      capacity;
    CELL_SCRIPT     lock;

    BIP44_Path      path;
//
//     stCellInput();
//    ~stCellInput() = default;
} CELL_INPUT;
// Remove c++ features for swift framework end


// Remove c++ features for swift framework
typedef struct stCellOutput {
    JUB_UINT64      capacity;
    CELL_SCRIPT     lock;
    CELL_SCRIPT     type;
    JUB_CHAR_PTR    data;

    BIP44_Path      path;
//
//     stCellOutput();
//    ~stCellOutput() = default;
} CELL_OUTPUT;
// Remove c++ features for swift framework end


/*****************************************************************************
 * @function name : JUB_CreateContextCKB
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextCKB(IN CONTEXT_CONFIG_CKB cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);


/*****************************************************************************
 * @function name : JUB_GetHDNodeCKB
 * @in  param : contextID - context ID
 *          : path
 * @out param : hex - xpub in HEX
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeCKB(IN JUB_UINT16 contextID,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR hex);


/*****************************************************************************
 * @function name : JUB_GetMainHDNodeCKB
 * @in  param : contextID - context ID
 * @out param : hex - xpub in HEX
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeCKB(IN JUB_UINT16 contextID,
                            OUT JUB_CHAR_PTR_PTR hex);


/*****************************************************************************
 * @function name : JUB_GetAddressCKB
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressCKB(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);


/*****************************************************************************
 * @function name : JUB_CheckAddressCKB
 * @in  param : contextID - context ID
 *          : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressCKB(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address);


///*****************************************************************************
// * @function name : JUB_SetMyAddressCKB
// * @in  param : contextID - context ID
// *          : path
// * @out param : address
// * @last change :
// *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_SetMyAddressCKB(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
//
//
/*****************************************************************************
 * @function name : JUB_SignTransactionCKB
 * @in  param : contextID - context ID
 *          : version
 *          : inputs
 *          : iCount
 *          : outputs
 *          : oCount
 *          : lockTime
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionCKB(IN JUB_UINT16 contextID,
                              IN JUB_UINT32 version,
                              IN CELL_DEP deps[], IN JUB_UINT16 depCount,
                              IN CELL_INPUT   inputs[], IN JUB_UINT16 iCount,
                              IN CELL_OUTPUT outputs[], IN JUB_UINT16 oCount,
                              OUT JUB_CHAR_PTR_PTR raw);


/*****************************************************************************
 * @function name : JUB_IsValidAddressCKB
 * @in  param : address
 * @out param: format - address format
 *          : codeHashIndex - code hash index
 *          : codeHash - code hash
 *          : args - args to encode
 * @last change :
 *****************************************************************************/
JUB_RV JUB_IsValidAddressCKB(IN JUB_CHAR_CPTR address,
                             OUT JUB_INT32_PTR format,
                             OUT JUB_INT32_PTR codeHashIndex,
                             OUT JUB_CHAR_PTR_PTR codeHash,
                             OUT JUB_CHAR_PTR_PTR args);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_CKB_h */
