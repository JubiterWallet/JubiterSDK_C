//
//  JubSDKCore+COIN_BTC.h
//  JubSDKCore
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore.h"

NS_ASSUME_NONNULL_BEGIN

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
//    p2pkh = 0,
//    //p2pwpkh,
//    p2sh_p2wpkh
///*
//    p2sh_multisig,
//    p2wsh_multisig,
//    p2sh_p2wsh_multisig,
//*/
//} JUB_BTC_TRANS_TYPE;
typedef NS_ENUM(NSInteger, JUB_NS_BTC_TRANS_TYPE) {
    ns_p2pkh = 0,
    ns_p2sh_p2wpkh
};

//typedef enum {
//    P2PKH   = 0x00,
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
//    JUB_CHAR_PTR    preHash;
//    JUB_UINT16      preIndex;
//    JUB_UINT64      amount;
//    BIP32_Path      path;
//} INPUT_BTC;
@interface InputBTC : NSObject
@property (atomic, copy  ) NSString* _Nonnull preHash;
@property (atomic, assign) NSInteger preIndex;
@property (atomic, assign) NSInteger amount;
@property (atomic, strong) BIP32Path* path;
@end

//typedef struct {
//    JUB_CHAR_PTR    address;
//    JUB_UINT64      amount;
//    JUB_ENUM_BOOL   change_address;
//    BIP32_Path      path;
//} OUTPUT_P2PKH;
@interface OP2pkh : NSObject
@property (atomic, copy  ) NSString* _Nonnull address;
@property (atomic, assign) NSInteger amount;
@property (atomic, assign) JUB_NS_ENUM_BOOL isChangeAddress;
@property (atomic, strong) BIP32Path* path;
@end

//typedef struct {
//    JUB_UINT64      amount;
//    JUB_UINT16      data_len;
//    JUB_BYTE        data[40];
//} OUTPUT_RETURN0;
@interface OReturn0 : NSObject
@property (atomic, assign) NSInteger amount;
@property (atomic, copy  ) NSString* _Nonnull data;
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
//@property (nonatomic, strong) OP2pkh* output;
//@end
//
//@interface OutputReturn0 : NSObject
//@property (nonatomic, assign) JUB_NS_OUTPUT_BTC_TYPE type;
//@property (nonatomic, strong) OReturn0* output;
//@end
@interface OutputBTC : NSObject
@property (atomic, assign) JUB_NS_OUTPUT_BTC_TYPE type;
@property (atomic, strong) OP2pkh* p2pkh;
@property (atomic, strong) OReturn0* return0;
@end


//typedef struct {
//    JUB_ENUM_COINTYPE_BTC   cointype;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };
//
//    JUB_CHAR_PTR            main_path;
//    JUB_BTC_TRANS_TYPE      transtype;
//} CONTEXT_CONFIG_BTC;
@interface ContextConfigBTC : NSObject
@property (atomic, assign) JUB_NS_ENUM_COINTYPE_BTC coinType;
@property (atomic, copy  ) NSString* _Nonnull mainPath;
@property (atomic, assign) JUB_NS_BTC_TRANS_TYPE transType;
@end


@interface JubSDKCore (COIN_BTC)

//JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextBTC:(NSUInteger)deviceID
                               cfg:(ContextConfigBTC*)cfg;

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

//JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
//                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
//                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
//                              IN JUB_UINT32 lockTime,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionBTC:(NSUInteger)contextID
                         inputArray:(NSArray*)inputArray
                        outputArray:(NSArray*)outputArray
                           lockTime:(NSUInteger)lockTime;

//JUB_RV JUB_SetUnitBTC(IN JUB_UINT16 contextID,
//                      IN JUB_BTC_UNIT_TYPE unit);
- (void)JUB_SetUnitBTC:(NSUInteger)contextID
                  unit:(JUB_NS_BTC_UNIT_TYPE)unit;

//JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
//                            IN JUB_CHAR_PTR USDT_to,
//                            IN JUB_UINT64 amount,
//                            OUT OUTPUT_BTC outputs[2]);
- (NSArray*)JUB_BuildUSDTOutputs:(NSUInteger)contextID
                          USDTTo:(NSString*)USDTTo
                          amount:(NSUInteger)amount;

@end

NS_ASSUME_NONNULL_END
