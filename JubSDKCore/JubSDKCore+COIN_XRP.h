//
//  JubSDKCore+COIN_XRP.h
//  JubSDKCore
//
//  Created by Pan Min on 2019/11/29.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore.h"

//typedef struct {
//    JUB_CHAR_PTR        mainPath;
//} CONTEXT_CONFIG_XRP;
@interface ContextConfigXRP : NSObject
@property (atomic, copy  ) NSString* _Nonnull mainPath;
@end

//typedef enum class JubXRPPubFormat {
//    HEX = 0x00,
//    XRP = 0x01
//} JUB_ENUM_XRP_PUB_FORMAT;
typedef NS_ENUM(NSInteger, JUB_NS_XRP_PUB_FORMAT) {
    NS_HEX  = 0x00,
    NS_XPUB = 0x01
};

//typedef enum class JubXRPTxType {
//    PYMT = 0x00,
//    NS_ITEM_TX_TYPE
//} JUB_ENUM_XRP_TX_TYPE;
typedef NS_ENUM(NSInteger, JUB_NS_XRP_TX_TYPE) {
    NS_PYMT  = 0x00,
    NS_TX_XRP_TYPE_NS
};

//typedef enum class JubXRPPaymentType {
//    DXRP   = 0x00,  // Direct XRP payments
////    FX     = 0x01,  // Cross-currency payments(foreign exchange)
////    CHECKS = 0x02,  // Checks
////    ESCROW = 0x03,  // Escrow
////    PRTL   = 0x04,  // Partial Payments
////    CHAN   = 0X05,  // Payment Channels
//    NS_ITEM_PYMT_TYPE
//} JUB_ENUM_XRP_PYMT_TYPE;
typedef NS_ENUM(NSInteger, JUB_NS_XRP_PYMT_TYPE) {
    NS_DXRP  = 0x00,
//    NS_FX,
//    NS_CHECKS,
//    NS_ESCROW,
//    NS_PRTL,
//    NS_CHAN,
    NS_DXRP_XRP_TYPE_NS
};

//typedef struct stAmount {
//    JUB_CHAR_PTR currency;
//    JUB_CHAR_PTR value;
//    JUB_CHAR_PTR issuer;
//} JUB_PYMT_AMOUNT;
@interface PymtAmount : NSObject
@property (atomic, copy  ) NSString* _Nonnull currency;
@property (atomic, copy  ) NSString* _Nonnull value;
@property (atomic, copy  ) NSString* _Nonnull issuer;
@end

//typedef struct stPaymentXRP {
//    JUB_ENUM_XRP_PYMT_TYPE type;
//    JUB_PYMT_AMOUNT amount;
//    JUB_CHAR_PTR destination;
//    JUB_CHAR_PTR destinationTag;
//    JUB_CHAR_PTR invoiceID;     // [Optional]
//    JUB_PYMT_AMOUNT sendMax;    // [Optional]
//    JUB_PYMT_AMOUNT deliverMin; // [Optional]
//} JUB_PYMT_XRP;
@interface Payment : NSObject
@property (atomic, assign) JUB_NS_XRP_PYMT_TYPE type;
@property (atomic, strong) PymtAmount* _Nonnull amount;
@property (atomic, copy  ) NSString* _Nonnull destination;
@property (atomic, copy  ) NSString* _Nonnull destinationTag;
@property (atomic, copy  ) NSString* _Nonnull invoiceID;    // [Optional]
@property (atomic, strong) PymtAmount* _Nonnull sendMax;    // [Optional]
@property (atomic, strong) PymtAmount* _Nonnull deliverMin; // [Optional]
@end

//typedef struct stTxXRP {
//    JUB_CHAR_PTR account;
//    JUB_ENUM_XRP_TX_TYPE type;
//    JUB_CHAR_PTR fee;
//    JUB_CHAR_PTR sequence;
//    JUB_CHAR_PTR accountTxnID;
//    JUB_CHAR_PTR flags;
//    JUB_CHAR_PTR lastLedgerSequence;
//    JUB_CHAR_PTR memos;
//    JUB_CHAR_PTR sourceTag;
//    union {
//        JUB_PYMT_XRP pymt;
//    };
//} JUB_TX_XRP;
@interface TxXRP : NSObject
@property (atomic, copy  ) NSString* _Nonnull account;
@property (atomic, assign) JUB_NS_XRP_TX_TYPE type;
@property (atomic, copy  ) NSString* _Nonnull fee;
@property (atomic, copy  ) NSString* _Nonnull sequence;
@property (atomic, copy  ) NSString* _Nonnull accountTxnID;
@property (atomic, copy  ) NSString* _Nonnull flags;
@property (atomic, copy  ) NSString* _Nonnull lastLedgerSequence;
@property (atomic, copy  ) NSString* _Nonnull memos;
@property (atomic, copy  ) NSString* _Nonnull sourceTag;
@property (atomic, strong) Payment* _Nonnull pymt;
@end

NS_ASSUME_NONNULL_BEGIN

@interface JubSDKCore (COIN_XRP)

//JUB_RV JUB_CreateContextXRP(IN CONTEXT_CONFIG_XRP cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextXRP:(NSUInteger)deviceID
                               cfg:(ContextConfigXRP*)cfg;

//JUB_RV JUB_GetAddressXRP(IN JUB_UINT16 contextID,
//                         IN BIP44_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressXRP:(NSUInteger)contextID
                          path:(BIP44Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow;

//JUB_RV JUB_SetMyAddressXRP(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressXRP:(NSUInteger)contextID
                            path:(BIP44Path*)path;

//JUB_RV JUB_SignTransactionXRP(IN JUB_UINT16 contextID,
//                              IN BIP44_Path path,
//                              IN JUB_TX_XRP tx,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionXRP:(NSUInteger)contextID
                               path:(BIP44Path*)path
                                 tx:(TxXRP*)tx;

@end

NS_ASSUME_NONNULL_END
