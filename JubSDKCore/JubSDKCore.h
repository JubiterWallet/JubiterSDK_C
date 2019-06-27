//
//  JubSDKCore.h
//  JubSDKCore
//
//  Created by Pan Min on 2019/6/22.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import <Foundation/Foundation.h>

//typedef enum {
//    BOOL_FALSE = 0,
//    BOOL_TRUE,
//    BOOL_NR_ITEMS
//} JUB_ENUM_BOOL;
typedef NS_ENUM(NSInteger, JUB_NS_ENUM_BOOL) {
    BOOL_NS_FALSE = 0,
    BOOL_NS_TRUE,
    BOOL_NS_NR_ITEMS
};

//typedef enum {
//    COINBTC,
//    COINBCH,
//    COINLTC,
//    COINUSDT,
//    Default = COINBTC
//} JUB_ENUM_COINTYPE_BTC;
typedef NS_ENUM(NSInteger, JUB_NS_ENUM_COINTYPE_BTC) {
    NS_COINBTC,
    NS_COINBCH,
    NS_COINLTC,
    NS_COINUSDT,
    NS_Default = NS_COINBTC
};

//typedef enum {
//    HEX = 0x00,
//    XPUB = 0x01
//} JUB_ETH_PUB_FORMAT;
typedef NS_ENUM(NSInteger, JUB_NS_ETH_PUB_FORMAT) {
    NS_HEX  = 0x00,
    NS_XPUB = 0x01
};

//typedef enum {
//    p2pkh = 0,
//    //p2pwpkh,
//    p2sh_p2wpkh
//    /*
//     p2sh_multisig,
//     p2wsh_multisig,
//     p2sh_p2wsh_multisig,
//     */
//} JUB_BTC_TRANS_TYPE;
typedef NS_ENUM(NSInteger, JUB_NS_BTC_TRANS_TYPE) {
    ns_p2pkh = 0,
    ns_p2sh_p2wpkh
};

//typedef enum {
//    P2PKH = 0x00,
//    RETURN0 = 0x01
//} OUTPUT_BTC_TYPE;
typedef NS_ENUM(NSInteger, JUB_NS_OUTPUT_BTC_TYPE) {
    NS_P2PKH = 0x00,
    NS_RETURN0 = 0x01
};

//typedef enum {
//    BTC = 0x00,
//    cBTC,
//    mBTC,
//    uBTC,
//    Satoshi
//} JUB_BTC_UNIT_TYPE;
typedef NS_ENUM(NSInteger, JUB_NS_BTC_UNIT_TYPE) {
    NS_BTC = 0x00,
    NS_cBTC,
    NS_mBTC,
    NS_uBTC,
    NS_Satoshi
};

//typedef struct {
//    JUB_CHAR label[32];
//    JUB_CHAR sn[24];
//    JUB_UINT16 pin_retry;
//    JUB_UINT16 pin_max_retry;
//    JUB_CHAR ble_version[4];
//    JUB_CHAR firmware_version[4];
//} JUB_DEVICE_INFO;
@interface DeviceInfo : NSObject
@property (nonatomic, assign) NSString* label;
@property (nonatomic, assign) NSString* sn;
@property (nonatomic, assign) NSInteger pinRetry;
@property (nonatomic, assign) NSInteger pinMaxRetry;
@property (nonatomic, assign) NSString* bleVersion;
@property (nonatomic, assign) NSString* firmwareVersion;
@end

//typedef struct {
//    JUB_ENUM_COINTYPE_BTC   cointype;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };
//
//    JUB_CHAR_PTR            main_path;
//    JUB_BTC_TRANS_TYPE        transtype;
//} CONTEXT_CONFIG_BTC;
@interface ContextConfigBTC : NSObject
@property (nonatomic, assign) JUB_NS_ENUM_COINTYPE_BTC coinType;
@property (nonatomic, assign) NSString* mainPath;
@property (nonatomic, assign) JUB_NS_BTC_TRANS_TYPE transType;
@end

//typedef struct {
//    JUB_CHAR_PTR        main_path;
//    int                    chainID;
//} CONTEXT_CONFIG_ETH;
@interface ContextConfigETH : NSObject
@property (nonatomic, assign) NSString* mainPath;
@property (nonatomic, assign) NSInteger chainID;
@end

//typedef struct {
//    JUB_ENUM_BOOL change;
//    JUB_UINT64    addressIndex;
//} BIP32_Path;
@interface BIP32Path : NSObject
@property (nonatomic, assign) JUB_NS_ENUM_BOOL change;
@property (nonatomic, assign) NSInteger addressIndex;
@end

//typedef struct {
//    JUB_CHAR_PTR    preHash;
//    JUB_UINT16      preIndex;
//    JUB_UINT64        amount;
//    BIP32_Path      path;
//} INPUT_BTC;
@interface InputBTC : NSObject
@property (nonatomic, assign) NSString* preHash;
@property (nonatomic, assign) NSInteger preIndex;
@property (nonatomic, assign) NSInteger amount;
@property (nonatomic, assign) BIP32Path* path;
@end

//typedef struct {
//    JUB_CHAR_PTR    address;
//    JUB_UINT64        amount;
//    JUB_ENUM_BOOL   change_address;
//    BIP32_Path      path;
//} OUTPUT_P2PKH;
@interface OP2pkh : NSObject
@property (nonatomic, assign) NSString* address;
@property (nonatomic, assign) NSInteger amount;
@property (nonatomic, assign) JUB_NS_ENUM_BOOL isChangeAddress;
@property (nonatomic, assign) BIP32Path* path;
@end

//typedef struct {
//    JUB_UINT64        amount;
//    JUB_UINT16      data_len;
//    JUB_BYTE        data[40];
//} OUTPUT_RETURN0;
@interface OReturn0 : NSObject
@property (nonatomic, assign) NSInteger amount;
@property (nonatomic, assign) NSString* data;
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
@interface OutputBTC : NSObject
@property (nonatomic, assign) JUB_NS_OUTPUT_BTC_TYPE type;
@property (nonatomic, assign) OP2pkh* p2pkh;
@property (nonatomic, assign) OReturn0* return0;
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
typedef  int(*ReadCallBack)(unsigned long int devHandle, unsigned char* data, unsigned int uiDataLen);
typedef void(*ScanCallBack)(unsigned char* devName, unsigned char* uuid, unsigned int type);
typedef void(*DiscCallBack)(unsigned char* uuid);
@interface DeviceInitParam : NSObject
{
    void* param;
    ReadCallBack readCallBack;
    ScanCallBack scanCallBack;
    DiscCallBack discCallBack;
}
@property (nonatomic, assign) void* param;
@property (nonatomic, assign) ReadCallBack readCallBack;
@property (nonatomic, assign) ScanCallBack scanCallBack;
@property (nonatomic, assign) DiscCallBack discCallBack;
@end


@interface JubSDKCore : NSObject
- (NSUInteger)JUB_LastError;

//JUB_RV JUB_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE]);
- (NSMutableArray*)JUB_ListDeviceHid;

//JUB_RV JUB_ConnetDeviceHid(IN JUB_UINT16 deviceID);
- (void)JUB_ConnetDeviceHid:(NSUInteger)deviceID;

//JUB_RV JUB_DisconnetDeviceHid(IN JUB_UINT16 deviceID);
- (void)JUB_DisconnetDeviceHid:(NSUInteger)deviceID;

//JUB_RV JUB_GetDeviceInfo(IN JUB_UINT16 deviceID,
//                         OUT JUB_DEVICE_INFO_PTR info);
- (DeviceInfo*)JUB_GetDeviceInfo:(NSUInteger)deviceID;

//JUB_RV JUB_GetDeviceCert(IN JUB_UINT16 deviceID,
//                         OUT JUB_CHAR_PTR_PTR cert);
- (NSString*)JUB_GetDeviceCert:(NSUInteger)deviceID;

//JUB_RV JUB_SendOneApdu(IN JUB_UINT16 deviceID,
//                       IN JUB_CHAR_PTR apdu ,
//                       OUT JUB_CHAR_PTR_PTR response);
- (NSString*)JUB_SendOneApdu:(NSUInteger)deviceID
                        apdu:(NSString*)apdu;

//JUB_ENUM_BOOL JUB_IsInitialize(IN JUB_UINT16 deviceID);
- (JUB_NS_ENUM_BOOL)JUB_IsInitialize:(NSUInteger)deviceID;

//JUB_ENUM_BOOL JUB_IsBootLoader(IN JUB_UINT16 deviceID);
- (JUB_NS_ENUM_BOOL)JUB_IsBootLoader:(NSUInteger)deviceID;

//JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextBTC:(NSUInteger)deviceID
                               cfg:(ContextConfigBTC*)cfg;

//JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID);
- (void)JUB_ClearContext:(NSUInteger)deviceID;

//JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
//                            IN JUB_CHAR_PTR USDT_to,
//                            IN JUB_UINT64 amount,
//                            OUT OUTPUT_BTC outputs[2]);
- (NSArray*)JUB_BuildUSDTOutputs:(NSUInteger)contextID
                          USDTTo:(NSString*)USDTTo
                          amount:(NSUInteger)amount;

//JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
//                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
//                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
//                              IN JUB_UINT32 lockTime,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionBTC:(NSUInteger)contextID
                         inputArray:(NSMutableArray*)inputArray
                        outputArray:(NSMutableArray*)outputArray
                           lockTime:(NSUInteger)lockTime;

//JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_ShowVirtualPwd:(NSUInteger)contextID;

//JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_CancelVirtualPwd:(NSUInteger)contextID;

//JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID,
//                     IN JUB_CHAR_PTR pinMix,
//                     OUT JUB_ULONG_PTR retry);
- (NSUInteger)JUB_VerifyPIN:(NSUInteger)contextID
                     pinMix:(NSString*)pinMix;

//JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
//                        IN BIP32_Path path,
//                        OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetHDNodeBTC:(NSUInteger)contextID
                         path:(BIP32Path*)path;

//JUB_RV JUB_GetMainHDNodeBTC(IN JUB_UINT16 contextID,
//                            OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetMainHDNodeBTC:(NSUInteger)contextID;

//JUB_RV JUB_GetAddressBTC(IN JUB_UINT16 contextID,
//                         IN BIP32_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressBTC:(NSUInteger)contextID
                          path:(BIP32Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow;

//JUB_RV JUB_SetMyAddressBTC(IN JUB_UINT16 contextID,
//                           IN BIP32_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressBTC:(NSUInteger)contextID
                            path:(BIP32Path*)path;

//JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextETH:(NSUInteger)deviceID
                               cfg:(ContextConfigETH*)cfg;

//JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
//                         IN BIP32_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressETH:(NSUInteger)contextID
                          path:(BIP32Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow;

//JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
//                        IN JUB_ETH_PUB_FORMAT format,
//                        IN BIP32_Path path,
//                        OUT JUB_CHAR_PTR_PTR pubkey);
- (NSString*)JUB_GetHDNodeETH:(NSUInteger)contextID
                       format:(JUB_NS_ETH_PUB_FORMAT)format
                         path:(BIP32Path*)path;

//JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
//                            IN JUB_ETH_PUB_FORMAT format,
//                            OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetMainHDNodeETH:(NSUInteger)contextID
                           format:(JUB_NS_ETH_PUB_FORMAT)format;

//JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
//                           IN BIP32_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressETH:(NSUInteger)contextID
                            path:(BIP32Path*)path;

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
                              input:(NSString*)input;

//JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
//                            IN JUB_CHAR_PTR token_to, IN JUB_CHAR_PTR token_value,
//                            OUT JUB_CHAR_PTR_PTR abi);
- (NSString*)JUB_BuildERC20AbiETH:(NSUInteger)contextID
                          tokenTo:(NSString*)tokenTo
                       tokenValue:(NSString*)tokenValue;

//JUB_RV JUB_SetUnitBTC(IN JUB_UINT16 contextID,
//                      IN JUB_BTC_UNIT_TYPE unit);
- (void)JUB_SetUnitBTC:(NSUInteger)contextID
                  unit:(JUB_NS_BTC_UNIT_TYPE)unit;

//JUB_RV JUB_SetTimeOut(IN JUB_UINT16 contextID, IN JUB_UINT16 timeout);
- (void)JUB_SetTimeOut:(NSUInteger)contextID
               timeout:(NSUInteger)timeout;

//JUB_RV JUB_QueryBattery(IN JUB_UINT16 deviceID,
//                        OUT JUB_BYTE_PTR percent);
- (NSString*)JUB_QueryBattery:(NSUInteger)deviceID;

//JUB_RV JUB_FreeMemory(IN JUB_CHAR_CPTR memPtr);
//- (void) JUB_FreeMemory :(NSString*)memPtr;

//JUB_RV JUB_EnumApplets(IN JUB_UINT16 deviceID,
//                       OUT JUB_CHAR_PTR_PTR appList);
- (NSString*)JUB_EnumApplets:(NSUInteger)deviceID;

//JUB_RV Jub_EnumSupportCoins(IN JUB_UINT16 deviceID,
//                            OUT JUB_CHAR_PTR_PTR coinList);
- (NSString*)Jub_EnumSupportCoins:(NSUInteger)deviceID;

//JUB_RV JUB_GetAppletVersion(IN JUB_UINT16 deviceID,
//                            IN JUB_CHAR_PTR appID,
//                            OUT JUB_CHAR_PTR_PTR version);
- (NSString*)JUB_GetAppletVersion:(NSUInteger)deviceID
                            appID:(NSString*)appID;

//JUB_CHAR_PTR JUB_GetVersion(void);
- (NSString*)JUB_GetVersion;

//JUB_RV JUB_initDevice(IN DEVICE_INIT_PARAM param);
- (void) JUB_initDevice :(DeviceInitParam*)param;

//JUB_RV JUB_enumDevices(void);
- (void)JUB_enumDevices;

//JUB_RV JUB_stopEnumDevices(void);
- (void)JUB_stopEnumDevices;

//JUB_RV JUB_connectDevice(JUB_BYTE_PTR bBLEUUID,  /**< ble device UUID */
//                         JUB_UINT32 connectType, /**< ble device connect type */
//                         JUB_UINT16* pDevice_ID, /**< output ble device connect handle */
//                         JUB_UINT32 timeout);
- (NSUInteger)JUB_connectDevice:(NSString*)deviceName
                    connectType:(NSUInteger)connectType
                        timeout:(NSUInteger)timeout;

//JUB_RV JUB_cancelConnect(JUB_BYTE_PTR bBLEUUID);
- (void)JUB_cancelConnect:(NSString*)deviceName;

//JUB_RV JUB_disconnectDevice(JUB_UINT16 deviceID);
- (void)JUB_disconnectDevice:(NSUInteger)deviceID;

//JUB_RV JUB_isDeviceConnect(JUB_UINT16 deviceID);
- (void)JUB_isDeviceConnect:(NSUInteger)deviceID;

@end
