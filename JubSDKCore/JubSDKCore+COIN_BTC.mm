//
//  JubSDKCore+COIN_BTC.mm
//  JubSDKCore
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore+COIN_BTC.h"
#import "JUB_SDK_BTC.h"

//typedef struct {
//    JUB_CHAR_PTR    preHash;
//    JUB_UINT16      preIndex;
//    JUB_UINT64        amount;
//    BIP32_Path      path;
//} INPUT_BTC;
@implementation InputBTC
@synthesize preHash;
@synthesize preIndex;
@synthesize amount;
@synthesize path;
@end

//typedef struct {
//    JUB_CHAR_PTR    address;
//    JUB_UINT64        amount;
//    JUB_ENUM_BOOL   change_address;
//    BIP32_Path      path;
//} OUTPUT_P2PKH;
@implementation OP2pkh
@synthesize address;
@synthesize amount;
@synthesize isChangeAddress;
@synthesize path;
@end

//typedef struct {
//    JUB_UINT64        amount;
//    JUB_UINT16      data_len;
//    JUB_BYTE        data[40];
//} OUTPUT_RETURN0;
@implementation OReturn0
@synthesize amount;
@synthesize data;
@end

//typedef struct {
//    OUTPUT_BTC_TYPE type;
//    union {
//        OUTPUT_P2PKH   output_p2pkh;
//        OUTPUT_RETURN0 output_return0;
//    };
//} OUTPUT_BTC;
//@interface OutputP2pkh : NSObject
//@property (nonatomic, assign) JUB_NS_OUTPUT_BTC_TYPE type;
//@property (nonatomic, assign) OP2pkh* output;
//@end
//
//@interface OutputReturn0 : NSObject
//@property (nonatomic, assign) JUB_NS_OUTPUT_BTC_TYPE type;
//@property (nonatomic, assign) OReturn0* output;
//@end
@implementation OutputBTC
@synthesize type;
@synthesize p2pkh;
@synthesize return0;
@end

//typedef struct {
//    JUB_ENUM_COINTYPE_BTC   cointype;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };
//
//    JUB_CHAR_PTR            main_path;
//    JUB_BTC_TRANS_TYPE        transtype;
//} CONTEXT_CONFIG_BTC;
@implementation ContextConfigBTC
@synthesize coinType;
@synthesize mainPath;
@synthesize transType;
@end

@implementation JubSDKCore (COIN_BTC)

//JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextBTC:(NSUInteger)deviceID
                               cfg:(ContextConfigBTC*)cfg
{
    self.lastError = JUBR_OK;
    
    CONTEXT_CONFIG_BTC cfgBTC;
    switch (cfg.coinType) {
        case NS_COINBTC:
            cfgBTC.coinType = COINBTC;
            break;
        case NS_COINBCH:
            cfgBTC.coinType = COINBCH;
            break;
        case NS_COINLTC:
            cfgBTC.coinType = COINLTC;
            break;
        case NS_COINUSDT:
            cfgBTC.coinType = COINUSDT;
            break;
        default:
            self.lastError = JUBR_ARGUMENTS_BAD;
            break;
    }
    switch (cfg.transType) {
        case ns_p2pkh:
            cfgBTC.transType = p2pkh;
            break;
        case ns_p2sh_p2wpkh:
            cfgBTC.transType = p2sh_p2wpkh;
            break;
        default:
            self.lastError = JUBR_ARGUMENTS_BAD;
            break;
    }
    if (JUBR_OK != self.lastError) {
        return 0;
    }
    cfgBTC.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextBTC(cfgBTC,
                                     deviceID,
                                     &contextID);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
//                        IN BIP32_Path path,
//                        OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetHDNodeBTC:(NSUInteger)contextID
                         path:(BIP32Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP32_Path p;
    switch (path.change) {
        case BOOL_NS_FALSE:
            p.change = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            p.change = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            p.change = BOOL_NR_ITEMS;
            break;
    }
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
//                         IN BIP32_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressBTC:(NSUInteger)contextID
                          path:(BIP32Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow
{
    self.lastError = JUBR_OK;
    
    BIP32_Path bip32Path;
    switch (path.change) {
        case BOOL_NS_FALSE:
            bip32Path.change = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            bip32Path.change = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            bip32Path.change = BOOL_NR_ITEMS;
            break;
    }
    bip32Path.addressIndex = path.addressIndex;
    JUB_ENUM_BOOL isShow;
    switch (bShow) {
        case BOOL_NS_FALSE:
            isShow = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            isShow = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            isShow = BOOL_NR_ITEMS;
            break;
    }
    JUB_CHAR_PTR address = nullptr;
    JUB_RV rv = JUB_GetAddressBTC(contextID,
                                  bip32Path,
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
//                           IN BIP32_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressBTC:(NSUInteger)contextID
                            path:(BIP32Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP32_Path bip32Path;
    switch (path.change) {
        case BOOL_NS_FALSE:
            bip32Path.change = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            bip32Path.change = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            bip32Path.change = BOOL_NR_ITEMS;
            break;
    }
    bip32Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR address = nullptr;
    JUB_RV rv = JUB_SetMyAddressBTC(contextID,
                                    bip32Path,
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
        pInputs[i].preHash = (JUB_CHAR_PTR)[input.preHash UTF8String];
        pInputs[i].preIndex = input.preIndex;
        pInputs[i].amount = input.amount;
        pInputs[i].path.addressIndex = input.path.addressIndex;
        switch (input.path.change) {
            case BOOL_NS_FALSE:
                pInputs[i].path.change = BOOL_FALSE;
                break;
            case BOOL_NS_TRUE:
                pInputs[i].path.change = BOOL_TRUE;
                break;
            case BOOL_NS_NR_ITEMS:
                pInputs[i].path.change = BOOL_NR_ITEMS;
                break;
        }
    }
    OUTPUT_BTC* pOutputs = (OUTPUT_BTC*)malloc(sizeof(OUTPUT_BTC)*oCnt+1);
    memset(pOutputs, 0x00, sizeof(OUTPUT_BTC)*oCnt+1);
    for (NSUInteger i=0; i < outputArray.count; ++i) {
//        if ([output isKindOfClass:[OutputP2pkh class]]) {
//            pOutputs[i].type = P2PKH;
//            pOutputs[i].output_p2pkh.address = (JUB_CHAR_PTR)[((OP2pkh*)output).address UTF8String];
//            pOutputs[i].output_p2pkh.amount = ((OP2pkh*)output).amount;
//            switch (((OP2pkh*)output).isChangeAddress) {
//                case BOOL_NS_FALSE:
//                    pOutputs[i].output_p2pkh.change_address = BOOL_FALSE;
//                    break;
//                case BOOL_NS_TRUE:
//                    pOutputs[i].output_p2pkh.change_address = BOOL_TRUE;
//                    break;
//                case BOOL_NS_NR_ITEMS:
//                    pOutputs[i].output_p2pkh.change_address = BOOL_NR_ITEMS;
//                    break;
//            }
//            pOutputs[i].output_p2pkh.path.addressIndex = ((OP2pkh*)output).path.addressIndex;
//            switch (((OP2pkh*)output).path.change) {
//                case BOOL_NS_FALSE:
//                    pOutputs[i].output_p2pkh.path.change = BOOL_FALSE;
//                    break;
//                case BOOL_NS_TRUE:
//                    pOutputs[i].output_p2pkh.path.change = BOOL_TRUE;
//                    break;
//                case BOOL_NS_NR_ITEMS:
//                    pOutputs[i].output_p2pkh.path.change = BOOL_NR_ITEMS;
//                    break;
//            }
//        }
//        else if ([output isKindOfClass:[OutputReturn0 class]]) {
//            pOutputs[i].type = RETURN0;
//            pOutputs[i].output_return0.amount = ((OReturn0*)output).amount;
//            pOutputs[i].output_return0.data_len = [((OReturn0*)output).data length];
//            if (sizeof(pOutputs[i].output_return0.data)/sizeof(JUB_BYTE) > pOutputs[i].output_return0.data_len) {
//                _lastError = JUBR_ARGUMENTS_BAD;
//                break;
//            }
//            memcpy(pOutputs[i].output_return0.data,
//                   (JUB_CHAR_PTR)[((OReturn0*)output).data UTF8String],
//                   pOutputs[i].output_return0.data_len);
//        }
//        else {
//            _lastError = JUBR_ARGUMENTS_BAD;
//            break;
//        }
        OutputBTC* output = outputArray[i];
        if (NS_P2PKH == output.type) {
            pOutputs[i].type = P2PKH;
            pOutputs[i].outputP2pkh.address = (JUB_CHAR_PTR)[output.p2pkh.address UTF8String];
            pOutputs[i].outputP2pkh.amount = output.p2pkh.amount;
            switch (output.p2pkh.isChangeAddress) {
                case BOOL_NS_FALSE:
                    pOutputs[i].outputP2pkh.changeAddress = BOOL_FALSE;
                    break;
                case BOOL_NS_TRUE:
                    pOutputs[i].outputP2pkh.changeAddress = BOOL_TRUE;
                    break;
                case BOOL_NS_NR_ITEMS:
                    pOutputs[i].outputP2pkh.changeAddress = BOOL_NR_ITEMS;
                    break;
            }
            pOutputs[i].outputP2pkh.path.addressIndex = output.p2pkh.path.addressIndex;
            switch (output.p2pkh.path.change) {
                case BOOL_NS_FALSE:
                    pOutputs[i].outputP2pkh.path.change = BOOL_FALSE;
                    break;
                case BOOL_NS_TRUE:
                    pOutputs[i].outputP2pkh.path.change = BOOL_TRUE;
                    break;
                case BOOL_NS_NR_ITEMS:
                    pOutputs[i].outputP2pkh.path.change = BOOL_NR_ITEMS;
                    break;
            }
        }
        else if (NS_RETURN0 == output.type) {
            pOutputs[i].type = RETURN0;
            pOutputs[i].outputReturn0.amount = output.return0.amount;
            pOutputs[i].outputReturn0.dataLen = [output.return0.data length];
            if (sizeof(pOutputs[i].outputReturn0.data)/sizeof(JUB_BYTE) > pOutputs[i].outputReturn0.dataLen) {
                self.lastError = JUBR_ARGUMENTS_BAD;
                break;
            }
            memcpy(pOutputs[i].outputReturn0.data,
                   (JUB_CHAR_PTR)[((OReturn0*)output).data UTF8String],
                   pOutputs[i].outputReturn0.dataLen);
        }
        else {
            self.lastError = JUBR_ARGUMENTS_BAD;
            break;
        }
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
    if (JUBR_OK != rv) {
        self.lastError = rv;
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
    JUB_BTC_UNIT_TYPE u;
    switch (unit) {
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
    }
    self.lastError = JUB_SetUnitBTC(contextID,
                                    u);
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

//    OutputP2pkh* outputP2pkh;
//    OutputReturn0* outputReturn0;
    OutputBTC* outputP2pkh;
    OutputBTC* outputReturn0;
    for (int i=0; i<sizeof(outputs)/sizeof(OUTPUT_BTC); ++i) {
        if (P2PKH == outputs[i].type) {
//            outputP2pkh.type = NS_P2PKH;
//            outputP2pkh.output.address = [NSString stringWithCString:outputs[i].output_p2pkh.address
//                                                            encoding:NSUTF8StringEncoding];
//            outputP2pkh.output.amount = outputs[i].output_p2pkh.amount;
//            switch (outputs[i].output_p2pkh.change_address) {
//                case BOOL_FALSE:
//                    outputP2pkh.output.isChangeAddress = BOOL_NS_FALSE;
//                    break;
//                case BOOL_TRUE:
//                    outputP2pkh.output.isChangeAddress = BOOL_NS_TRUE;
//                    break;
//                case BOOL_NR_ITEMS:
//                    outputP2pkh.output.isChangeAddress = BOOL_NS_NR_ITEMS;
//                    break;
//            }
//            switch (outputs[i].output_p2pkh.path.change) {
//                case BOOL_FALSE:
//                    outputP2pkh.output.path.change = BOOL_NS_FALSE;
//                    break;
//                case BOOL_TRUE:
//                    outputP2pkh.output.path.change = BOOL_NS_TRUE;
//                    break;
//                case BOOL_NR_ITEMS:
//                    outputP2pkh.output.path.change = BOOL_NS_NR_ITEMS;
//                    break;
//            }
//            outputP2pkh.output.path.addressIndex = outputs[i].output_p2pkh.path.addressIndex;
            outputP2pkh.type = NS_P2PKH;
            outputP2pkh.p2pkh.address = [NSString stringWithCString:outputs[i].outputP2pkh.address
                                                           encoding:NSUTF8StringEncoding];
            outputP2pkh.p2pkh.amount = outputs[i].outputP2pkh.amount;
            switch (outputs[i].outputP2pkh.changeAddress) {
                case BOOL_FALSE:
                    outputP2pkh.p2pkh.isChangeAddress = BOOL_NS_FALSE;
                    break;
                case BOOL_TRUE:
                    outputP2pkh.p2pkh.isChangeAddress = BOOL_NS_TRUE;
                    break;
                case BOOL_NR_ITEMS:
                    outputP2pkh.p2pkh.isChangeAddress = BOOL_NS_NR_ITEMS;
                    break;
            }
            switch (outputs[i].outputP2pkh.path.change) {
                case BOOL_FALSE:
                    outputP2pkh.p2pkh.path.change = BOOL_NS_FALSE;
                    break;
                case BOOL_TRUE:
                    outputP2pkh.p2pkh.path.change = BOOL_NS_TRUE;
                    break;
                case BOOL_NR_ITEMS:
                    outputP2pkh.p2pkh.path.change = BOOL_NS_NR_ITEMS;
                    break;
            }
            outputP2pkh.p2pkh.path.addressIndex = outputs[i].outputP2pkh.path.addressIndex;
        }
        else if (RETURN0 == outputs[i].type) {
//            outputReturn0.type = NS_RETURN0;
//            outputReturn0.output.amount = outputs[i].output_return0.amount;
//            outputReturn0.output.data = [NSString stringWithCharacters:(const unichar *)outputs[i].output_return0.data
//                                                                length:(NSUInteger)outputs[i].output_return0.data_len];
            outputReturn0.type = NS_RETURN0;
            outputReturn0.return0.amount = outputs[i].outputReturn0.amount;
            outputReturn0.return0.data = [NSString stringWithCharacters:(const unichar *)outputs[i].outputReturn0.data
                                                                 length:(NSUInteger)outputs[i].outputReturn0.dataLen];
        }
        else {
            self.lastError = JUBR_ERROR;
            break;
        }
    }
    if (JUBR_OK != self.lastError) {
        return @[];
    }

    NSArray *outputArray = [NSArray arrayWithObjects:outputP2pkh, outputReturn0, nil];

    return outputArray;
}

@end
