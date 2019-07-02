//
//  JubSDKCore.m
//  JubSDKCore
//
//  Created by Pan Min on 2019/6/22.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore.h"
#import "JUB_SDK.h"

//typedef struct {
//    JUB_CHAR label[32];
//    JUB_CHAR sn[24];
//    JUB_UINT16 pinRetry;
//    JUB_UINT16 pinMaxRetry;
//    JUB_CHAR bleVersion[4];
//    JUB_CHAR firmwareVersion[4];
//} JUB_DEVICE_INFO;
@implementation DeviceInfo
@synthesize label;
@synthesize sn;
@synthesize pinRetry;
@synthesize pinMaxRetry;
@synthesize bleVersion;
@synthesize firmwareVersion;
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

//typedef struct {
//    JUB_CHAR_PTR        main_path;
//    int                    chainID;
//} CONTEXT_CONFIG_ETH;
@implementation ContextConfigETH
@synthesize mainPath;
@synthesize chainID;
@end

//typedef struct {
//    JUB_ENUM_BOOL change;
//    JUB_UINT64    addressIndex;
//} BIP32_Path;
@implementation BIP32Path
@synthesize change;
@synthesize addressIndex;
@end

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

//typedef int(*DEV_ReadCallBack)(JUB_ULONG devHandle, JUB_BYTE_PTR data, JUB_UINT32 dataLen);
//typedef void(*DEV_ScanCallBack)(JUB_BYTE_PTR devName, JUB_BYTE_PTR uuid, JUB_UINT32 type);
//typedef void(*DEV_DiscCallBack)(JUB_BYTE_PTR uuid);
//typedef struct _DEVICE_INIT_PARAM_ {
//    void* param;
//    DEV_ReadCallBack callBack;
//    DEV_ScanCallBack scanCallBack;
//    DEV_DiscCallBack discCallBack;
//} DEVICE_INIT_PARAM;
@implementation DeviceInitParam
@synthesize param;
@synthesize readCallBack;
@synthesize scanCallBack;
@synthesize discCallBack;
@end


@interface JubSDKCore()
@property NSInteger lastError;
@end

@implementation JubSDKCore
- (NSUInteger)JUB_LastError
{
    return _lastError;
}

//JUB_RV JUB_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE]);
- (NSMutableArray*)JUB_ListDeviceHid
{
    _lastError = JUBR_OK;
    
    JUB_UINT16 deviceIDs[MAX_DEVICE];
    JUB_RV rv = JUB_ListDeviceHid(deviceIDs);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSMutableArray *deviceIDArray =  [NSMutableArray arrayWithCapacity:MAX_DEVICE];
    for (int i=0; i<MAX_DEVICE; ++i) {
        if (0 != deviceIDs[i]) {
            [deviceIDArray addObject:[NSNumber numberWithInt:deviceIDs[i]]];
        }
    }
    
    return deviceIDArray;
}

//JUB_RV JUB_ConnetDeviceHid(IN JUB_UINT16 deviceID);
- (void)JUB_ConnetDeviceHid:(NSUInteger)deviceID
{
    _lastError = JUB_ConnetDeviceHid(deviceID);
}

//JUB_RV JUB_DisconnetDeviceHid(IN JUB_UINT16 deviceID);
- (void)JUB_DisconnetDeviceHid:(NSUInteger)deviceID
{
    _lastError = JUB_DisconnetDeviceHid(deviceID);
}

//JUB_RV JUB_GetDeviceInfo(IN JUB_UINT16 deviceID,
//                         OUT JUB_DEVICE_INFO_PTR info);
- (DeviceInfo*)JUB_GetDeviceInfo:(NSUInteger)deviceID
{
    _lastError = JUBR_OK;
    
    JUB_DEVICE_INFO deviceInfo;
    JUB_RV rv = JUB_GetDeviceInfo(deviceID,
                                  &deviceInfo);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    DeviceInfo* info = [[DeviceInfo alloc] init];
    info.label = [NSString stringWithCString:deviceInfo.label
                                    encoding:NSUTF8StringEncoding];
    info.sn    = [NSString stringWithCString:deviceInfo.sn
                                    encoding:NSUTF8StringEncoding];
    info.pinRetry    = deviceInfo.pinRetry;
    info.pinMaxRetry = deviceInfo.pinMaxRetry;
    info.bleVersion  = [NSString stringWithCString:deviceInfo.bleVersion
                                          encoding:NSUTF8StringEncoding];
    info.firmwareVersion = [NSString stringWithCString:deviceInfo.firmwareVersion
                                              encoding:NSUTF8StringEncoding];
    
    return info;
}

//JUB_RV JUB_GetDeviceCert(IN JUB_UINT16 deviceID,
//                         OUT JUB_CHAR_PTR_PTR cert);
- (NSString*)JUB_GetDeviceCert:(NSUInteger)deviceID
{
    _lastError = JUBR_OK;
    
    char *cert = nil;
    JUB_RV rv = JUB_GetDeviceCert(deviceID,
                                  &cert);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
//    NSString *strCert = [[NSString alloc] initWithCString:cert
//                                                 encoding:NSUTF8StringEncoding];
    NSString *strCert = [NSString stringWithCString:cert
                                           encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(cert);
    
    return strCert;
}

//JUB_RV JUB_SendOneApdu(IN JUB_UINT16 deviceID,
//                       IN JUB_CHAR_PTR apdu ,
//                       OUT JUB_CHAR_PTR_PTR response);
- (NSString*)JUB_SendOneApdu:(NSUInteger)deviceID
                        apdu:(NSString*)apdu
{
    _lastError = JUBR_OK;
    
    JUB_CHAR_PTR response = nil;
    JUB_RV rv = JUB_SendOneApdu(deviceID,
                                (JUB_CHAR_PTR)[apdu UTF8String],
                                &response);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
//    NSString *strResp = [[NSString alloc] initWithCString:response
//                                                 encoding:NSUTF8StringEncoding];
    NSString *strResp = [NSString stringWithCString:response
                                           encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(response);
    
    return strResp;
}

//JUB_ENUM_BOOL JUB_IsInitialize(IN JUB_UINT16 deviceID);
- (JUB_NS_ENUM_BOOL)JUB_IsInitialize:(NSUInteger)deviceID
{
    _lastError = JUBR_OK;
    
    JUB_ENUM_BOOL bRv = JUB_IsInitialize(deviceID);
    JUB_NS_ENUM_BOOL bRet;
    switch (bRv) {
        case BOOL_FALSE:
        bRet = BOOL_NS_FALSE;
        break;
        case BOOL_TRUE:
        bRet = BOOL_NS_TRUE;
        break;
        case BOOL_NR_ITEMS:
        bRet = BOOL_NS_NR_ITEMS;
        break;
    }
    
    return bRet;
}

//JUB_ENUM_BOOL JUB_IsBootLoader(IN JUB_UINT16 deviceID);
- (JUB_NS_ENUM_BOOL)JUB_IsBootLoader:(NSUInteger)deviceID
{
    _lastError = JUBR_OK;
    
    JUB_ENUM_BOOL bRv = JUB_IsBootLoader(deviceID);
    JUB_NS_ENUM_BOOL bRet;
    switch (bRv) {
        case BOOL_FALSE:
        bRet = BOOL_NS_FALSE;
        break;
        case BOOL_TRUE:
        bRet = BOOL_NS_TRUE;
        break;
        case BOOL_NR_ITEMS:
        bRet = BOOL_NS_NR_ITEMS;
        break;
    }
    
    return bRet;
}

//JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextBTC:(NSUInteger)deviceID
                               cfg:(ContextConfigBTC*)cfg
{
    _lastError = JUBR_OK;
    
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
        _lastError = JUBR_ARGUMENTS_BAD;
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
        _lastError = JUBR_ARGUMENTS_BAD;
        break;
    }
    if (JUBR_OK != _lastError) {
        return 0;
    }
    cfgBTC.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextBTC(cfgBTC,
                                     deviceID,
                                     &contextID);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID);
- (void)JUB_ClearContext:(NSUInteger)deviceID
{
    _lastError = JUB_ClearContext(deviceID);
}

//JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
//                            IN JUB_CHAR_PTR USDT_to,
//                            IN JUB_UINT64 amount,
//                            OUT OUTPUT_BTC outputs[2]);
- (NSArray*)JUB_BuildUSDTOutputs:(NSUInteger)contextID
                          USDTTo:(NSString*)USDTTo
                          amount:(NSUInteger)amount
{
    _lastError = JUBR_OK;
    
    OUTPUT_BTC outputs[2];
    JUB_RV rv = JUB_BuildUSDTOutputs(contextID,
                                     (JUB_CHAR_PTR)[USDTTo UTF8String],
                                     amount,
                                     outputs);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
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
            _lastError = JUBR_ERROR;
            break;
        }
    }
    if (JUBR_OK != _lastError) {
        return nil;
    }
    
    NSArray *outputArray = [NSArray arrayWithObjects:outputP2pkh, outputReturn0, nil];
    
    return outputArray;
}

//JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
//                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
//                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
//                              IN JUB_UINT32 lockTime,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionBTC:(NSUInteger)contextID
                         inputArray:(NSMutableArray*)inputArray
                        outputArray:(NSMutableArray*)outputArray
                           lockTime:(NSUInteger)lockTime
{
    _lastError = JUBR_OK;
    
    NSUInteger iCnt = [inputArray count];
    NSUInteger oCnt = [outputArray count];
    if (   0 == iCnt
        || 0 == oCnt
        ) {
        _lastError = JUBR_ARGUMENTS_BAD;
        return nil;
    }
    
    INPUT_BTC* pInputs = (INPUT_BTC*)malloc(iCnt);
    int i = 0;
    for (InputBTC* input in inputArray) {
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
        ++i;
    }
    OUTPUT_BTC* pOutputs = (OUTPUT_BTC*)malloc(oCnt);
    i = 0;
    for (OutputBTC* output in outputArray) {
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
                _lastError = JUBR_ARGUMENTS_BAD;
                break;
            }
            memcpy(pOutputs[i].outputReturn0.data,
                   (JUB_CHAR_PTR)[((OReturn0*)output).data UTF8String],
                   pOutputs[i].outputReturn0.dataLen);
        }
        else {
            _lastError = JUBR_ARGUMENTS_BAD;
            break;
        }
        ++i;
    }
    if (JUBR_OK != _lastError) {
        free(pInputs); pInputs = nil;
        free(pOutputs); pOutputs = nil;
        return nil;
    }
    
    JUB_CHAR_PTR raw = nil;
    JUB_RV rv = JUB_SignTransactionBTC(contextID,
                                       pInputs, iCnt,
                                       pOutputs, oCnt,
                                       (JUB_UINT32)lockTime,
                                       &raw);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
//    NSString *strRaw = [[NSString alloc] initWithCString:raw
//                                                encoding:NSUTF8StringEncoding];
    NSString *strRaw = [NSString stringWithCString:raw
                                          encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(raw);
    
    return strRaw;
}

//JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_ShowVirtualPwd:(NSUInteger)contextID
{
    _lastError = JUB_ShowVirtualPwd(contextID);
}

//JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_CancelVirtualPwd:(NSUInteger)contextID
{
    _lastError = JUB_CancelVirtualPwd(contextID);
}

//JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID,
//                     IN JUB_CHAR_PTR pinMix,
//                     OUT JUB_ULONG_PTR retry);
- (NSUInteger)JUB_VerifyPIN:(NSUInteger)contextID
                     pinMix:(NSString*)pinMix
{
    _lastError = JUBR_OK;
    
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_VerifyPIN(contextID,
                              (JUB_CHAR_PTR)[pinMix UTF8String],
                              &retry);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return retry;
    }
    
    return retry;
}

//JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
//                        IN BIP32_Path path,
//                        OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetHDNodeBTC:(NSUInteger)contextID
                         path:(BIP32Path*)path
{
    _lastError = JUBR_OK;
    
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
    JUB_CHAR_PTR xpub = nil;
    JUB_RV rv = JUB_GetHDNodeBTC(contextID,
                                 p,
                                 &xpub);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
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
    _lastError = JUBR_OK;
    
    JUB_CHAR_PTR xpub = nil;
    JUB_RV rv = JUB_GetMainHDNodeBTC(contextID,
                                     &xpub);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
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
    _lastError = JUBR_OK;
    
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
    JUB_CHAR_PTR address = nil;
    JUB_RV rv = JUB_GetAddressBTC(contextID,
                                  bip32Path,
                                  isShow,
                                  &address);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
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
    _lastError = JUBR_OK;
    
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
    JUB_CHAR_PTR address = nil;
    JUB_RV rv = JUB_SetMyAddressBTC(contextID,
                                    bip32Path,
                                    &address);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strAddress = [NSString stringWithCString:address
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(address);
    
    return strAddress;
}

//JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextETH:(NSUInteger)deviceID
                               cfg:(ContextConfigETH*)cfg
{
    _lastError = JUBR_OK;
    
    CONTEXT_CONFIG_ETH cfgETH;
    cfgETH.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    cfgETH.chainID = (int)cfg.chainID;
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextETH(cfgETH,
                                     deviceID,
                                     &contextID);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
//                         IN BIP32_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressETH:(NSUInteger)contextID
                          path:(BIP32Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow
{
    _lastError = JUBR_OK;
    
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
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_GetAddressETH(contextID,
                                  bip32Path,
                                  isShow,
                                  &address);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strAddress = [NSString stringWithCString:address
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(address);
    
    return strAddress;
}

//JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
//                        IN JUB_ETH_PUB_FORMAT format,
//                        IN BIP32_Path path,
//                        OUT JUB_CHAR_PTR_PTR pubkey);
- (NSString*)JUB_GetHDNodeETH:(NSUInteger)contextID
                       format:(JUB_NS_ETH_PUB_FORMAT)format
                         path:(BIP32Path*)path
{
    _lastError = JUBR_OK;
    
    JUB_ETH_PUB_FORMAT fmt;
    switch (format) {
        case NS_HEX:
        fmt = HEX;
        break;
        case NS_XPUB:
        fmt = XPUB;
        break;
        default:
        _lastError = JUBR_ARGUMENTS_BAD;
        return nil;
    }
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
    JUB_CHAR_PTR pubkey = nil;
    JUB_RV rv = JUB_GetHDNodeETH(contextID,
                                 fmt,
                                 bip32Path,
                                 &pubkey);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strPubkey = [NSString stringWithCString:pubkey
                                             encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(pubkey);
    
    return strPubkey;
}

//JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
//                            IN JUB_ETH_PUB_FORMAT format,
//                            OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetMainHDNodeETH:(NSUInteger)contextID
                           format:(JUB_NS_ETH_PUB_FORMAT)format
{
    _lastError = JUBR_OK;
    
    JUB_ETH_PUB_FORMAT fmt;
    switch (format) {
        case NS_HEX:
        fmt = HEX;
        break;
        case NS_XPUB:
        fmt = XPUB;
        break;
        default:
        _lastError = JUBR_ARGUMENTS_BAD;
        return nil;
    }
    JUB_CHAR_PTR xpub = nil;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID,
                                     fmt,
                                     &xpub);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strXpub = [NSString stringWithCString:xpub
                                           encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(xpub);
    
    return strXpub;
}

//JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
//                           IN BIP32_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressETH:(NSUInteger)contextID
                            path:(BIP32Path*)path
{
    _lastError = JUBR_OK;
    
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
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_SetMyAddressETH(contextID,
                                    bip32Path,
                                    &address);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strAddress = [NSString stringWithCString:address
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(address);
    
    return strAddress;
}

//JUB_RV JUB_SignTransactionETH(IN JUB_UINT16 contextID,
//                              IN BIP32_Path path,
//                              IN JUB_UINT32 nonce,
//                              IN JUB_UINT32 gasLimit,
//                              IN JUB_CHAR_PTR gasPriceInWei,
//                              IN JUB_CHAR_PTR to,
//                              IN JUB_CHAR_PTR valueInWei,
//                              IN JUB_CHAR_PTR input,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionETH:(NSUInteger)contextID
                               path:(BIP32Path*)path
                              nonce:(NSUInteger)nonce
                           gasLimit:(NSUInteger)gasLimit
                      gasPriceInWei:(NSString*)gasPriceInWei
                                 to:(NSString*)to
                         valueInWei:(NSString*)valueInWei
                              input:(NSString*)input
{
    _lastError = JUBR_OK;
    
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
    JUB_CHAR_PTR raw = nil;
    JUB_RV rv = JUB_SignTransactionETH(contextID,
                                       bip32Path,
                                       (JUB_UINT32)nonce,
                                       (JUB_UINT32)gasLimit,
                                       (JUB_CHAR_PTR)[gasPriceInWei UTF8String],
                                       (JUB_CHAR_PTR)[to UTF8String],
                                       (JUB_CHAR_PTR)[valueInWei UTF8String],
                                       (JUB_CHAR_PTR)[input UTF8String],
                                       &raw);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strRaw = [NSString stringWithCString:raw
                                          encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(raw);
    
    return strRaw;
}

//JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
//                            IN JUB_CHAR_PTR token_to, IN JUB_CHAR_PTR token_value,
//                            OUT JUB_CHAR_PTR_PTR abi);
- (NSString*)JUB_BuildERC20AbiETH:(NSUInteger)contextID
                          tokenTo:(NSString*)tokenTo
                       tokenValue:(NSString*)tokenValue
{
    _lastError = JUBR_OK;
    
    JUB_CHAR_PTR abi = nil;
    JUB_RV rv = JUB_BuildERC20AbiETH(contextID,
                                     (JUB_CHAR_PTR)[tokenTo UTF8String],
                                     (JUB_CHAR_PTR)[tokenValue UTF8String],
                                     &abi);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strAbi = [NSString stringWithCString:abi
                                          encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(abi);
    
    return strAbi;
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
    _lastError = JUB_SetUnitBTC(contextID,
                                u);
}

//JUB_RV JUB_SetTimeOut(IN JUB_UINT16 contextID, IN JUB_UINT16 timeout);
- (void)JUB_SetTimeOut:(NSUInteger)contextID
               timeout:(NSUInteger)timeout
{
    _lastError = JUB_SetTimeOut(contextID,
                                timeout);
}

//JUB_RV JUB_QueryBattery(IN JUB_UINT16 deviceID,
//                        OUT JUB_BYTE_PTR percent);
- (NSString*)JUB_QueryBattery:(NSUInteger)deviceID
{
    _lastError = JUBR_OK;
    
    JUB_BYTE percent = 0x00;
    JUB_RV rv = JUB_QueryBattery(deviceID,
                                 &percent);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    return [NSString stringWithFormat:@"%c", percent];
}

////JUB_RV JUB_FreeMemory(IN JUB_CHAR_CPTR memPtr);
//- (void) JUB_FreeMemory :(NSString*)memPtr;

//JUB_RV JUB_EnumApplets(IN JUB_UINT16 deviceID,
//                       OUT JUB_CHAR_PTR_PTR appList);
- (NSString*)JUB_EnumApplets:(NSUInteger)deviceID
{
    _lastError = JUBR_OK;
    
    JUB_CHAR_PTR appList = nil;
    JUB_RV rv = JUB_EnumApplets(deviceID,
                                &appList);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strAppList = [NSString stringWithCString:appList
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(appList);
    
    return strAppList;
}

//JUB_RV Jub_EnumSupportCoins(IN JUB_UINT16 deviceID,
//                            OUT JUB_CHAR_PTR_PTR coinList);
- (NSString*)Jub_EnumSupportCoins:(NSUInteger)deviceID
{
    _lastError = JUBR_OK;
    
    JUB_CHAR_PTR coinList = nil;
    JUB_RV rv = Jub_EnumSupportCoins(deviceID,
                                     &coinList);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strCoinList = [NSString stringWithCString:coinList
                                               encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(coinList);
    
    return strCoinList;
}

//JUB_RV JUB_GetAppletVersion(IN JUB_UINT16 deviceID,
//                            IN JUB_CHAR_PTR appID,
//                            OUT JUB_CHAR_PTR_PTR version);
- (NSString*)JUB_GetAppletVersion:(NSUInteger)deviceID
                            appID:(NSString*)appID
{
    _lastError = JUBR_OK;
    
    JUB_CHAR_PTR version = nil;
    JUB_RV rv = JUB_GetAppletVersion(deviceID,
                                     (JUB_CHAR_PTR)[appID UTF8String],
                                     &version);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return nil;
    }
    
    NSString* strVersion = [NSString stringWithCString:version
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(version);
    
    return strVersion;
}

//JUB_CHAR_PTR JUB_GetVersion(void);
- (NSString*)JUB_GetVersion
{
    _lastError = JUBR_OK;
    
    return [NSString stringWithCString:JUB_GetVersion()
                              encoding:NSUTF8StringEncoding];
}

//JUB_RV JUB_initDevice(IN DEVICE_INIT_PARAM param);
- (void) JUB_initDevice :(DeviceInitParam*)param
{
    DEVICE_INIT_PARAM parameter;
    parameter.param = param.param;
    parameter.callBack     = param.readCallBack;
    parameter.scanCallBack = param.scanCallBack;
    parameter.discCallBack = param.discCallBack;
    
    _lastError = JUB_initDevice(parameter);
}

//JUB_RV JUB_enumDevices(void);
- (void)JUB_enumDevices
{
    _lastError = JUB_enumDevices();
}

//JUB_RV JUB_stopEnumDevices(void);
- (void)JUB_stopEnumDevices
{
    _lastError = JUB_stopEnumDevices();
}

//JUB_RV JUB_connectDevice(JUB_BYTE_PTR bBLEUUID,  /**< ble device UUID */
//                         JUB_UINT32 connectType, /**< ble device connect type */
//                         JUB_UINT16* pDeviceID,  /**< output ble device connect handle */
//                         JUB_UINT32 timeout);
- (NSUInteger)JUB_connectDevice:(NSString*)deviceName
                    connectType:(NSUInteger)connectType
                        timeout:(NSUInteger)timeout
{
    _lastError = JUBR_OK;
    
    JUB_UINT16 deviceID = 0;
    JUB_RV rv = JUB_connectDevice((JUB_BYTE_PTR)[deviceName UTF8String],
                                  (JUB_UINT32)connectType,
                                  &deviceID,
                                  (JUB_UINT32)timeout);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return deviceID;
    }
    
    return NSUInteger(deviceID);
}

//JUB_RV JUB_cancelConnect(JUB_BYTE_PTR bBLEUUID);
- (void)JUB_cancelConnect:(NSString*)deviceName
{
    _lastError = JUB_cancelConnect((JUB_BYTE_PTR)[deviceName UTF8String]);
}

//JUB_RV JUB_disconnectDevice(JUB_UINT16 deviceID);
- (void)JUB_disconnectDevice:(NSUInteger)deviceID
{
    _lastError = JUB_disconnectDevice(deviceID);
}

//JUB_RV JUB_isDeviceConnect(JUB_UINT16 deviceID);
- (void)JUB_isDeviceConnect:(NSUInteger)deviceID
{
    _lastError = JUB_isDeviceConnect(deviceID);
}

@end
