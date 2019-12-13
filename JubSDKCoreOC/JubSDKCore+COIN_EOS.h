//
//  JubSDKCore+COIN_EOS.h
//  JubSDKCore
//
//  Created by Pan Min on 2019/10/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore.h"

//typedef struct {
//    JUB_CHAR_PTR    main_path;
//} CONTEXT_CONFIG_EOS;
@interface ContextConfigEOS : NSObject
@property (atomic, copy  ) NSString* _Nonnull mainPath;
@end

//typedef enum class JubEOSActionType {
//    XFER   = 0x00, // transfer
//      DELE = 0x01, //   delegatebw
//    UNDELE = 0x02, // undelegatebw
//    BUYRAM = 0x03, //  buyrambytes
//   SELLRAM = 0x04, // sellrambytes
//    NS_ITEM_ACTION_TYPE
//} JUB_ENUM_EOS_ACTION_TYPE;
typedef NS_ENUM(NSInteger, JUB_NS_EOS_ACTION_TYPE) {
    NS_XFER   = 0x00,
    NS_DELE   = 0x01,
    NS_UNDELE = 0x02,
    NS_BUYRAM = 0x03,
    NS_SELLRAM = 0x04,
    NS_ITEM_ACTION_TYPE_NS
};

//typedef struct stTransferAction {
//    JUB_CHAR_PTR from;
//    JUB_CHAR_PTR to;
//    JUB_CHAR_PTR asset;
//    JUB_CHAR_PTR memo;
//} JUB_ACTION_TRANSFER;
@interface XferAction : NSObject
@property (atomic, copy  ) NSString* _Nonnull from;
@property (atomic, copy  ) NSString* _Nonnull to;
@property (atomic, copy  ) NSString* _Nonnull asset;
@property (atomic, copy  ) NSString* _Nonnull memo;
@end

//typedef struct stDelegateAction {
//    JUB_CHAR_PTR from;
//    JUB_CHAR_PTR receiver;
//    JUB_CHAR_PTR netQty; // stake_net_quantity/unstake_net_quantity
//    JUB_CHAR_PTR cpuQty; // stake_cpu_quantity/unstake_cpu_quantity
//    bool bStake;
//} JUB_ACTION_DELEGATE;
@interface DeleAction : NSObject
@property (atomic, copy  ) NSString* _Nonnull from;
@property (atomic, copy  ) NSString* _Nonnull receiver;
@property (atomic, copy  ) NSString* _Nonnull netQty;
@property (atomic, copy  ) NSString* _Nonnull cpuQty;
@property (atomic, assign) JUB_NS_ENUM_BOOL bStake;
@end

//typedef struct stBuyRamAction {
//    JUB_CHAR_PTR payer;
//    JUB_CHAR_PTR quant;
//    JUB_CHAR_PTR receiver;
//} JUB_ACTION_BUYRAM;
@interface BuyRamAction : NSObject
@property (atomic, copy  ) NSString* _Nonnull payer;
@property (atomic, copy  ) NSString* _Nonnull quant;
@property (atomic, copy  ) NSString* _Nonnull receiver;
@end

//typedef struct stSellRamAction {
//    JUB_CHAR_PTR account;
//    JUB_CHAR_PTR bytes;
//} JUB_ACTION_SELLRAM;
@interface SellRamAction : NSObject
@property (atomic, copy  ) NSString* _Nonnull account;
@property (atomic, copy  ) NSString* _Nonnull bytes;
@end

//typedef struct stActionEOS {
//    JUB_ENUM_EOS_ACTION_TYPE type;
//    JUB_CHAR_PTR currency;
//    JUB_CHAR_PTR name;
//    union {
//        JUB_ACTION_TRANSFER transfer;
//        JUB_ACTION_DELEGATE delegate;
//        JUB_ACTION_BUYRAM buyRam;
//        JUB_ACTION_SELLRAM sellRam;
//    };
//} JUB_ACTION_EOS;
@interface ActionEOS : NSObject
@property (atomic, assign) JUB_NS_EOS_ACTION_TYPE type;
@property (atomic, copy  ) NSString* _Nonnull currency;
@property (atomic, copy  ) NSString* _Nonnull name;
@property (atomic, strong) XferAction* _Nonnull xfer;
@property (atomic, strong) DeleAction* _Nonnull dele;
@property (atomic, strong) BuyRamAction* _Nonnull buyRam;
@property (atomic, strong) SellRamAction* _Nonnull sellRam;
@end

NS_ASSUME_NONNULL_BEGIN

@interface JubSDKCore (COIN_EOS)

//JUB_RV JUB_CreateContextEOS_soft(IN CONTEXT_CONFIG_EOS cfg,
//                                 IN JUB_CHAR_PTR masterPriInXPRV,
//                                 OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextEOS_soft:(ContextConfigEOS*)cfg
                        masterPriInXPRV:(NSString*)masterPriInXPRV;

//JUB_RV JUB_CreateContextEOS(IN CONTEXT_CONFIG_EOS cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextEOS:(NSUInteger)deviceID
                               cfg:(ContextConfigEOS*)cfg;

//JUB_RV JUB_GetAddressEOS(IN JUB_UINT16 contextID,
//                         IN BIP44_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressEOS:(NSUInteger)contextID
                          path:(BIP44Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow;

//JUB_RV JUB_SetMyAddressEOS(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressEOS:(NSUInteger)contextID
                            path:(BIP44Path*)path;

//JUB_RV JUB_SignTransactionEOS(IN JUB_UINT16 contextID,
//                              IN BIP44_Path path,
//                              IN JUB_CHAR_PTR chainID,
//                              IN JUB_CHAR_PTR expiration,
//                              IN JUB_CHAR_PTR referenceBlockId,
//                              IN JUB_CHAR_PTR referenceBlockTime,
//                              IN JUB_CHAR_PTR actionsInJSON,
//                              OUT JUB_CHAR_PTR_PTR rawInJSON);
- (NSString*)JUB_SignTransactionEOS:(NSUInteger)contextID
                               path:(BIP44Path*)path
                            chainID:(NSString*)chainID
                         expiration:(NSString*)expiration
                   referenceBlockId:(NSString*)referenceBlockId
                 referenceBlockTime:(NSString*)referenceBlockTime
                             action:(NSString*)actionsInJSON;

//JUB_RV JUB_BuildActionEOS(IN JUB_UINT16 contextID,
//                          IN JUB_ACTION_EOS_PTR actions,
//                          IN JUB_UINT16 actionCount,
//                          OUT JUB_CHAR_PTR_PTR actionsInJSON);
- (NSString*)JUB_BuildActionEOS:(NSUInteger)contextID
                         action:(NSMutableArray*)actionArray;

@end

NS_ASSUME_NONNULL_END
