//
//  JubSDKCore+COIN_BTC.mm
//  JubSDKCore
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore+COIN_BTC.h"
#import "JUB_SDK_BTC.h"
#include "mSIGNA/stdutils/uchar_vector.h"

//typedef struct stInput {
//    SCRIPT_BTC_TYPE type;
//    JUB_CHAR_PTR    preHash;
//    JUB_UINT16      preIndex;
//    JUB_UINT32      nSequence;
//    JUB_UINT64      amount;
//    BIP44_Path      path;
//} INPUT_BTC;
@implementation InputBTC
@synthesize type;
@synthesize preHash;
@synthesize preIndex;
@synthesize nSequence;
@synthesize amount;
@synthesize path;
@end

//typedef struct stOutput {
//    JUB_CHAR_PTR    address;
//    JUB_UINT64      amount;
//    JUB_ENUM_BOOL   change_address;
//    BIP44_Path      path;
//
//     stOutput();
//    ~stOutput() = default;
//} OUTPUT;
@implementation StdOutput
@synthesize address;
@synthesize amount;
@synthesize isChangeAddress;
@synthesize path;
@end

//typedef struct stOutputReturn0 {
//    JUB_UINT64      amount;
//    JUB_UINT16      dataLen;
//    JUB_BYTE        data[40];
//
//     stOutputReturn0();
//    ~stOutputReturn0() = default;
//} OUTPUT_RETURN0;
@implementation OReturn0
@synthesize amount;
@synthesize data;
@end

//typedef struct stOutputQRC20 {
//    JUB_UINT16      dataLen;
//    JUB_BYTE        data[200];
//
//     stOutputQRC20();
//    ~stOutputQRC20() = default;
//} OUTPUT_QRC20;
@implementation Qrc20
@synthesize data;
@end

//typedef struct stOutputBTC {
//    JUB_ENUM_SCRIPT_BTC_TYPE type;
//    union {
//        OUTPUT stdOutput;
//        OUTPUT_RETURN0 return0;
//        OUTPUT_QRC20 qrc20;
//    };
//
//     stOutputBTC();
//    ~stOutputBTC() = default;
//} OUTPUT_BTC;
//@interface StdOutput : NSObject
//@property (atomic, copy  ) NSString* _Nonnull address;
//@property (atomic, assign) NSInteger amount;
//@property (atomic, assign) JUB_NS_ENUM_BOOL isChangeAddress;
//@property (atomic, strong) BIP44Path* path;
//@end
//
//@interface OReturn0 : NSObject
//@property (atomic, assign) NSInteger amount;
//@property (atomic, copy  ) NSString* _Nonnull data;
//@end
//
//@interface Qrc20 : NSObject
//@property (atomic, copy  ) NSString* _Nonnull data;
//@end
@implementation OutputBTC
@synthesize type;
@synthesize stdOutput;
@synthesize return0;
@synthesize qrc20;
@end

//typedef struct stContextCfgBTC : stContextCfg {
//    JUB_ENUM_COINTYPE_BTC   coinType;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };
//    JUB_BTC_TRANS_TYPE      transType;
//} CONTEXT_CONFIG_BTC;
@implementation ContextConfigBTC
@synthesize coinType;
@synthesize transType;
@end

extern JUB_ENUM_BOOL    (^inlineBool)(JUB_NS_ENUM_BOOL);
extern JUB_NS_ENUM_BOOL (^inlineNSBool)(JUB_ENUM_BOOL);

// JUB_NS_ENUM_COINTYPE_BTC -> JUB_ENUM_COINTYPE_BTC
JUB_ENUM_COINTYPE_BTC (^inlineCoinType)(JUB_NS_ENUM_COINTYPE_BTC) = ^(JUB_NS_ENUM_COINTYPE_BTC argument) {
    JUB_ENUM_COINTYPE_BTC coinType;
    switch (argument) {
        case NS_COINBTC:
            coinType = COINBTC;
            break;
        case NS_COINBCH:
            coinType = COINBCH;
            break;
        case NS_COINLTC:
            coinType = COINLTC;
            break;
        case NS_COINUSDT:
            coinType = COINUSDT;
            break;
        case NS_COINQTUM:
            coinType = COINQTUM;
            break;
        default:
            coinType = Default;
            break;
    }
    
    return coinType;
};

// JUB_NS_SCRIPT_BTC_TYPE -> JUB_ENUM_SCRIPT_BTC_TYPE
JUB_ENUM_SCRIPT_BTC_TYPE (^inlineScriptBTCType)(JUB_NS_SCRIPT_BTC_TYPE) = ^(JUB_NS_SCRIPT_BTC_TYPE argument) {
    JUB_ENUM_SCRIPT_BTC_TYPE type;
    switch (argument) {
        case NS_P2PKH:
            type = P2PKH;
            break;
        case NS_RETURN0:
            type = RETURN0;
            break;
//        case NS_P2SH_MULTISIG:
//            type = P2SH_MULTISIG;
//            break;
        case NS_QRC20:
            type = QRC20;
            break;
        default:
            type = P2PKH;
            break;
    }
    
    return type;
};

// JUB_NS_BTC_TRANS_TYPE <- JUB_BTC_TRANS_TYPE
JUB_NS_SCRIPT_BTC_TYPE (^inlineNSScriptBTCType)(JUB_ENUM_SCRIPT_BTC_TYPE) = ^(JUB_ENUM_SCRIPT_BTC_TYPE argument) {
    JUB_NS_SCRIPT_BTC_TYPE type;
    switch (argument) {
        case P2PKH:
            type = NS_P2PKH;
            break;
        case RETURN0:
            type = NS_RETURN0;
            break;
//        case P2SH_MULTISIG:
//            type = NS_P2SH_MULTISIG;
//            break;
        case QRC20:
            type = NS_QRC20;
            break;
        default:
            type = NS_SCRIPT_BTC_TYPE_NS;
            break;
    }
    
    return type;
};

// JUB_NS_BTC_TRANS_TYPE -> JUB_ENUM_BTC_TRANS_TYPE
JUB_ENUM_BTC_TRANS_TYPE (^inlineTransType)(JUB_NS_BTC_TRANS_TYPE) = ^(JUB_NS_BTC_TRANS_TYPE argument) {
    JUB_ENUM_BTC_TRANS_TYPE transType;
    switch (argument) {
        case ns_p2pkh:
            transType = p2pkh;
            break;
        case ns_p2sh_p2wpkh:
            transType = p2sh_p2wpkh;
            break;
//        case ns_p2sh_multisig:
//            transType = p2sh_multisig;
//            break;
        default:
            transType = trans_type_ns_item;
            break;
    }
    
    return transType;
};

// JUB_NS_BTC_UNIT_TYPE -> JUB_ENUM_BTC_UNIT_TYPE
JUB_ENUM_BTC_UNIT_TYPE (^inlineBTCUnitType)(JUB_NS_BTC_UNIT_TYPE) = ^(JUB_NS_BTC_UNIT_TYPE argument) {
    JUB_ENUM_BTC_UNIT_TYPE u;
    switch (argument) {
        case NS_BTC:
            u = BTC;
            break;
        case NS_cBTC:
            u = cBTC;
            break;
        case NS_mBTC:
            u = mBTC;
            break;
        case NS_uBTC:
            u = uBTC;
            break;
        case NS_Satoshi:
            u = Satoshi;
            break;
        default:
            u = ns;
            break;
    }
    return u;
};

// InputBTC* -> INPUT_BTC
INPUT_BTC (^inlineTransInputBTC)(InputBTC*) = ^(InputBTC* inputBTC) {
    INPUT_BTC input;
    
    input.type = inlineScriptBTCType(inputBTC.type);
    input.preHash = (JUB_CHAR_PTR)[inputBTC.preHash UTF8String];
    input.preIndex = inputBTC.preIndex;
    input.nSequence = (JUB_UINT32)inputBTC.nSequence;
    if (0 == input.nSequence) {
        input.nSequence = 0xffffffff;
    }
    input.amount = inputBTC.amount;
    input.path.addressIndex = inputBTC.path.addressIndex;
    input.path.change = inlineBool(inputBTC.path.change);
    
    return input;
};

// InputBTC* <- INPUT_BTC
InputBTC* (^inlineNSTransInputBTC)(INPUT_BTC) = ^(INPUT_BTC inputBTC) {
    InputBTC* input = [[InputBTC alloc] init];
    
    input.type = inlineNSScriptBTCType(inputBTC.type);
    if (NULL != inputBTC.preHash) {
        input.preHash = [[NSString alloc] init];
        input.preHash = [[NSString stringWithCString:inputBTC.preHash
                                            encoding:NSUTF8StringEncoding] copy];
    }
    else {
        input.preHash = @"";
    }
    JUB_FreeMemory(inputBTC.preHash);
    input.preIndex = inputBTC.preIndex;
    input.nSequence = (JUB_UINT32)inputBTC.nSequence;
    if (0 == input.nSequence) {
        input.nSequence = 0xffffffff;
    }
    input.amount = inputBTC.amount;
    input.path.addressIndex = inputBTC.path.addressIndex;
    input.path.change = inlineNSBool(inputBTC.path.change);
    
    return input;
};

std::vector<unsigned char> HexArray2CharPtr(unsigned char* ch, unsigned int chLen) {
    std::vector<unsigned char> v;
    if (nullptr == ch) {
        return v;
    }
    
    for (unsigned int i=0; i<chLen; ++i) {
        v.push_back(ch[i]);
    }
    
    return v;
}

// OutputBTC* -> OUTPUT_BTC
OUTPUT_BTC (^inlineTransOutputBTC)(OutputBTC*) = ^(OutputBTC* outputBTC) {
    OUTPUT_BTC output;
    
    output.type = inlineScriptBTCType(outputBTC.type);
    switch (outputBTC.type) {
        case NS_P2PKH:
//        case NS_P2SH_MULTISIG:
            output.stdOutput.address = (JUB_CHAR_PTR)[outputBTC.stdOutput.address UTF8String];
            output.stdOutput.amount = outputBTC.stdOutput.amount;
            output.stdOutput.changeAddress = inlineBool(outputBTC.stdOutput.isChangeAddress);
            output.stdOutput.path.addressIndex = outputBTC.stdOutput.path.addressIndex;
            output.stdOutput.path.change = inlineBool(outputBTC.stdOutput.path.change);
            break;
        case NS_RETURN0:
        {
            output.return0.amount = outputBTC.return0.amount;
            NSString *data = outputBTC.return0.data;
            uchar_vector vData([data UTF8String]);
            NSInteger dataLen = vData.size();
            output.return0.dataLen = dataLen;
            if (sizeof(output.return0.data)/sizeof(JUB_BYTE) > dataLen) {
                for (NSInteger i=0; i<dataLen; ++i) {
                    output.return0.data[i] = vData[i];
                }
            }
            break;
        }
        case NS_QRC20:
        {
            NSString *data = outputBTC.qrc20.data;
            uchar_vector vData([data UTF8String]);
            NSInteger dataLen = vData.size();
            output.qrc20.dataLen = dataLen;
            if (sizeof(output.qrc20.data)/sizeof(JUB_BYTE) > dataLen) {
                for (NSInteger i=0; i<dataLen; ++i) {
                    output.qrc20.data[i] = vData[i];
                }
            }
            break;
        }
        case NS_SCRIPT_BTC_TYPE_NS:
        default:
            break;
    }
    
    return output;
};

// OutputBTC* <- OUTPUT_BTC
OutputBTC* (^inlineNSTransOutputBTC)(OUTPUT_BTC) = ^(OUTPUT_BTC outputBTC) {
    OutputBTC* output = [[OutputBTC alloc] init];
    
    output.type = inlineNSScriptBTCType(outputBTC.type);
    switch (outputBTC.type) {
        case P2PKH:
//        case P2SH_MULTISIG:
            output.stdOutput = [[StdOutput alloc] init];
            if (NULL != outputBTC.stdOutput.address) {
                output.stdOutput.address = [[NSString stringWithCString:outputBTC.stdOutput.address
                                                               encoding:NSUTF8StringEncoding] copy];
                JUB_FreeMemory(outputBTC.stdOutput.address);
            }
            else {
                output.stdOutput.address = @"";
            }
            output.stdOutput.amount = outputBTC.stdOutput.amount;
            output.stdOutput.isChangeAddress = inlineNSBool(outputBTC.stdOutput.changeAddress);
            output.stdOutput.path = [[BIP44Path alloc] init];
            output.stdOutput.path.addressIndex = outputBTC.stdOutput.path.addressIndex;
            output.stdOutput.path.change = inlineNSBool(outputBTC.stdOutput.path.change);
            break;
        case RETURN0:
        {
            output.return0 = [[OReturn0 alloc] init];
            output.return0.amount = output.return0.amount;
            uchar_vector vData = HexArray2CharPtr(outputBTC.return0.data, outputBTC.return0.dataLen);
            output.return0.data = [NSString stringWithFormat:@"%s", vData.getHex().c_str()];
            break;
        }
        case QRC20:
        {
            output.qrc20 = [[Qrc20 alloc] init];
            uchar_vector vData = HexArray2CharPtr(outputBTC.qrc20.data, outputBTC.qrc20.dataLen);
            output.qrc20.data = [NSString stringWithFormat:@"%s", vData.getHex().c_str()];
            break;
        }
        default:
            break;
    }
    
    return output;
};

@implementation JubSDKCore (COIN_BTC)

//JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextBTC:(NSUInteger)deviceID
                               cfg:(ContextConfigBTC*)cfg
{
    self.lastError = JUBR_OK;
    
    JUB_ENUM_COINTYPE_BTC coinType = inlineCoinType(cfg.coinType);
    JUB_ENUM_BTC_TRANS_TYPE transType = inlineTransType(cfg.transType);
    if (trans_type_ns_item == transType) {
        self.lastError = JUBR_ARGUMENTS_BAD;
        return 0;
    }
    
    JUB_CHAR_PTR mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    if (!mainPath) {
        self.lastError = JUBR_ARGUMENTS_BAD;
        return 0;
    }
    
    JUB_RV rv = JUBR_ERROR;
    JUB_UINT16 contextID = 0;
    switch (transType) {
        case p2pkh:
        case p2sh_p2wpkh:
        {
            CONTEXT_CONFIG_BTC ctxCfg;
            ctxCfg.coinType = coinType;
            ctxCfg.transType = transType;
            ctxCfg.mainPath = mainPath;
            rv = JUB_CreateContextBTC(ctxCfg,
                                      deviceID,
                                      &contextID);
            break;
        }
        default:
            rv = JUBR_ARGUMENTS_BAD;
            break;
    }
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
//                        IN BIP44_Path path,
//                        OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetHDNodeBTC:(NSUInteger)contextID
                         path:(BIP44Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP44_Path p;
    p.change = inlineBool(path.change);
    p.addressIndex = path.addressIndex;
    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetHDNodeBTC(contextID,
                                 p,
                                 &xpub);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strXpub = [NSString stringWithCString:xpub
                                           encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(xpub);
    
    return strXpub;
}

//JUB_RV JUB_GetMainHDNodeBTC(IN JUB_UINT16 contextID,
//                            OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetMainHDNodeBTC:(NSUInteger)contextID
{
    self.lastError = JUBR_OK;
    
    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeBTC(contextID,
                                     &xpub);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strXpub = [NSString stringWithCString:xpub
                                           encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(xpub);
    
    return strXpub;
}

//JUB_RV JUB_GetAddressBTC(IN JUB_UINT16 contextID,
//                         IN BIP44_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressBTC:(NSUInteger)contextID
                          path:(BIP44Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_ENUM_BOOL isShow = inlineBool(bShow);
    JUB_CHAR_PTR address = nullptr;
    JUB_RV rv = JUB_GetAddressBTC(contextID,
                                  bip44Path,
                                  isShow,
                                  &address);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strAddress = [NSString stringWithCString:address
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(address);
    
    return strAddress;
}

//JUB_RV JUB_SetMyAddressBTC(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressBTC:(NSUInteger)contextID
                            path:(BIP44Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR address = nullptr;
    JUB_RV rv = JUB_SetMyAddressBTC(contextID,
                                    bip44Path,
                                    &address);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strAddress = [NSString stringWithCString:address
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(address);
    
    return strAddress;
}

//JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
//                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
//                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
//                              IN JUB_UINT32 lockTime,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionBTC:(NSUInteger)contextID
                         inputArray:(NSArray*)inputArray
                        outputArray:(NSArray*)outputArray
                           lockTime:(NSUInteger)lockTime
{
    self.lastError = JUBR_OK;
    
    NSUInteger iCnt = [inputArray count];
    NSUInteger oCnt = [outputArray count];
    if (   0 == iCnt
        || 0 == oCnt
        ) {
        self.lastError = JUBR_ARGUMENTS_BAD;
        return @"";
    }
    
    INPUT_BTC* pInputs = (INPUT_BTC*)malloc(sizeof(INPUT_BTC)*iCnt+1);
    memset(pInputs, 0x00, sizeof(INPUT_BTC)*iCnt+1);
    for (NSUInteger i=0; i < inputArray.count; ++i) {
        InputBTC* input = inputArray[i];
        pInputs[i] = inlineTransInputBTC(input);
    }
    OUTPUT_BTC* pOutputs = (OUTPUT_BTC*)malloc(sizeof(OUTPUT_BTC)*oCnt+1);
    memset(pOutputs, 0x00, sizeof(OUTPUT_BTC)*oCnt+1);
    for (NSUInteger i=0; i < outputArray.count; ++i) {
        OutputBTC* output = outputArray[i];
        if (JUB_NS_SCRIPT_BTC_TYPE::NS_SCRIPT_BTC_TYPE_NS == output.type) {
            self.lastError = JUBR_ARGUMENTS_BAD;
        }
        pOutputs[i] = inlineTransOutputBTC(output);
    }
    if (JUBR_OK != self.lastError) {
        if (nullptr != pInputs) {
            free(pInputs); pInputs = nullptr;
        }
        if (nullptr != pOutputs) {
            free(pOutputs); pOutputs = nullptr;
        }
        return @"";
    }
    
    JUB_CHAR_PTR raw = nullptr;
    JUB_RV rv = JUB_SignTransactionBTC(contextID,
                                       pInputs, iCnt,
                                       pOutputs, oCnt,
                                       (JUB_UINT32)lockTime,
                                       &raw);
    if (nullptr != pInputs) {
        free(pInputs); pInputs = nullptr;
    }
    if (nullptr != pOutputs) {
        free(pOutputs); pOutputs = nullptr;
    }
    
    self.lastError = rv;
    if (JUBR_OK != rv) {
        return @"";
    }
    
    NSString *strRaw = [NSString stringWithCString:raw
                                          encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(raw);
    
    return strRaw;
}

//JUB_RV JUB_SetUnitBTC(IN JUB_UINT16 contextID,
//                      IN JUB_BTC_UNIT_TYPE unit);
- (void)JUB_SetUnitBTC:(NSUInteger)contextID
                  unit:(JUB_NS_BTC_UNIT_TYPE)unit
{
    self.lastError = JUB_SetUnitBTC(contextID,
                                    inlineBTCUnitType(unit));
}

//JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
//                            IN JUB_CHAR_PTR USDT_to,
//                            IN JUB_UINT64 amount,
//                            OUT OUTPUT_BTC outputs[2]);
- (NSArray*)JUB_BuildUSDTOutputs:(NSUInteger)contextID
                          USDTTo:(NSString*)USDTTo
                          amount:(NSUInteger)amount
{
    self.lastError = JUBR_OK;
    
    OUTPUT_BTC outputs[2];
    JUB_RV rv = JUB_BuildUSDTOutputs(contextID,
                                     (JUB_CHAR_PTR)[USDTTo UTF8String],
                                     amount,
                                     outputs);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @[];
    }
    
    OutputBTC* outputP2pkh = [[OutputBTC alloc] init];
    OutputBTC* outputReturn0 = [[OutputBTC alloc] init];
    for (int i=0; i<sizeof(outputs)/sizeof(OUTPUT_BTC); ++i) {
        if (P2PKH == outputs[i].type) {
            outputP2pkh = inlineNSTransOutputBTC(outputs[i]);
        }
        else if (RETURN0 == outputs[i].type) {
            outputReturn0 = inlineNSTransOutputBTC(outputs[i]);
        }
        else {
            self.lastError = JUBR_ARGUMENTS_BAD;
            break;
        }
    }
    if (JUBR_OK != self.lastError) {
        return @[];
    }
    
    NSArray *outputArray = [NSArray arrayWithObjects:outputP2pkh, outputReturn0, nil];
    
    return outputArray;
}

//JUB_RV JUB_BuildQRC20Outputs(IN JUB_UINT16 contextID,
//                             IN JUB_CHAR_PTR contractAddress, IN JUB_UINT8 decimal, IN JUB_CHAR_PTR symbol,
//                             IN JUB_UINT64 gasLimit, IN JUB_UINT64 gasPrice,
//                             IN JUB_CHAR_PTR to, IN JUB_CHAR_PTR value,
//                             OUT OUTPUT_BTC outputs[1]);
- (NSArray*)JUB_BuildQRC20Outputs:(NSUInteger)contextID
                     contractAddr:(NSString*)contractAddr
                          decimal:(NSUInteger)decimal
                           symbol:(NSString*)symbol
                         gasLimit:(NSUInteger)gasLimit
                         gasPrice:(NSUInteger)gasPrice
                               to:(NSString*)to
                            value:(NSString*)value
{
    self.lastError = JUBR_OK;
    
    OUTPUT_BTC outputs[1];
    JUB_RV rv = JUB_BuildQRC20Outputs(contextID,
                                      (JUB_CHAR_PTR)[contractAddr UTF8String], decimal, (JUB_CHAR_PTR)[symbol UTF8String],
                                      gasLimit, gasPrice,
                                      (JUB_CHAR_PTR)[to UTF8String], (JUB_CHAR_PTR)[value UTF8String],
                                      outputs);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return nil;
    }
    
    OutputBTC* qrc20 = [[OutputBTC alloc] init];
    for (int i=0; i<sizeof(outputs)/sizeof(OUTPUT_BTC); ++i) {
        if (QRC20 == outputs[i].type) {
            qrc20 = inlineNSTransOutputBTC(outputs[i]);
        }
        else {
            self.lastError = JUBR_ARGUMENTS_BAD;
            break;
        }
    }
    if (JUBR_OK != self.lastError) {
        return nil;
    }
    
    NSArray *outputArray = [NSArray arrayWithObjects:qrc20, nil];
    
    return outputArray;
}

@end
