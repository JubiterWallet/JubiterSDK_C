//
//  JubSDKCore+HCash.h
//  JubSDKCore
//
//  Created by Pan Min on 2019/7/12.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore.h"
#import "JubSDKCore+COIN_BTC.h"

NS_ASSUME_NONNULL_BEGIN

//typedef stContextCfg CONTEXT_CONFIG_HC;
typedef ContextConfig ContextConfigHC;

//typedef struct {
//    JUB_UINT64  amount;
//    BIP44_Path  path;
//} INPUT_HC;
@interface InputHC : NSObject
@property (atomic, assign) NSInteger amount;
@property (atomic, strong) BIP44Path* path;
@end

//typedef struct {
//    JUB_ENUM_BOOL   change_address;
//    BIP44_Path      path;
//} OUTPUT_HC;
@interface OutputHC : NSObject
@property (atomic, assign) JUB_NS_ENUM_BOOL isChangeAddress;
@property (atomic, strong) BIP44Path* path;
@end

@interface JubSDKCore (COIN_HCash)

//JUB_RV JUB_CreateContextHC(IN CONTEXT_CONFIG_HC cfg,
//                           IN JUB_UINT16 deviceID,
//                           OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextHC:(NSUInteger)deviceID
                              cfg:(ContextConfigHC*)cfg;

//JUB_RV JUB_GetAddressHC(IN JUB_UINT16 contextID,
//                        IN BIP44_Path path,
//                        IN JUB_ENUM_BOOL bShow,
//                        OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressHC:(NSUInteger)contextID
                         path:(BIP44Path*)path
                        bShow:(JUB_NS_ENUM_BOOL)bShow;

//JUB_RV JUB_GetHDNodeHC(IN JUB_UINT16 contextID,
//                       IN BIP44_Path path,
//                       OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetHDNodeHC:(NSUInteger)contextID
                        path:(BIP44Path*)path;

//JUB_RV JUB_GetMainHDNodeHC(IN JUB_UINT16 contextID,
//                           OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetMainHDNodeHC:(NSUInteger)contextID;

//JUB_RV JUB_SignTransactionHC(IN JUB_UINT16 contextID,
//                             IN INPUT_HC inputs[], IN JUB_UINT16 iCount,
//                             IN OUTPUT_HC outputs[], IN JUB_UINT16 oCount,
//                             IN JUB_CHAR_PTR unsigned_trans,
//                             OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionHC:(NSUInteger)contextID
                        inputArray:(NSArray*)inputArray
                       outputArray:(NSArray*)outputArray
                     unsignedTrans:(NSString*)unsignedTrans;


@end // @interface JubSDKCore (HCash)

NS_ASSUME_NONNULL_END
