//
//  JubSDKCore+COIN_XRP.mm
//  JubSDKCore
//
//  Created by Pan Min on 2019/11/29.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore+COIN_XRP.h"
#import "JUB_SDK_XRP.h"
#import "JUB_core.h"

//typedef struct {
//    JUB_CHAR_PTR        mainPath;
//} CONTEXT_CONFIG_XRP;
@implementation ContextConfigXRP
@synthesize mainPath;
@end

//typedef struct stAmount {
//    JUB_CHAR_PTR currency;
//    JUB_CHAR_PTR value;
//    JUB_CHAR_PTR issuer;
//} JUB_PYMT_AMOUNT;
@implementation PymtAmount
@synthesize currency;
@synthesize value;
@synthesize issuer;
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
@implementation Payment
@synthesize type;
@synthesize amount;
@synthesize destination;
@synthesize destinationTag;
@synthesize invoiceID;
@synthesize sendMax;
@synthesize deliverMin;
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
@implementation TxXRP
@synthesize account;
@synthesize type;
@synthesize fee;
@synthesize sequence;
@synthesize accountTxnID;
@synthesize flags;
@synthesize lastLedgerSequence;
@synthesize memos;
@synthesize sourceTag;
@synthesize pymt;
@end

extern bool (^inlinebool)(JUB_NS_ENUM_BOOL);
extern JUB_NS_ENUM_BOOL (^inlineNSbool)(bool);

// JUB_NS_XRP_TX_TYPE -> JUB_ENUM_XRP_TX_TYPE
JUB_ENUM_XRP_TX_TYPE (^inlineXRPTxType)(JUB_NS_XRP_TX_TYPE) = ^(JUB_NS_XRP_TX_TYPE argument) {
    JUB_ENUM_XRP_TX_TYPE u;
    switch (argument) {
        case NS_PYMT:
            u = JUB_ENUM_XRP_TX_TYPE::PYMT;
            break;
        default:
            u = JUB_ENUM_XRP_TX_TYPE::NS_ITEM_TX_TYPE;
            break;
    }
    
    return u;
};

// JUB_NS_XRP_TX_TYPE <- JUB_ENUM_XRP_TX_TYPE
JUB_NS_XRP_TX_TYPE (^inlineNSXRPTxType)(JUB_ENUM_XRP_TX_TYPE) = ^(JUB_ENUM_XRP_TX_TYPE argument) {
    JUB_NS_XRP_TX_TYPE u;
    switch (argument) {
        case JUB_ENUM_XRP_TX_TYPE::PYMT:
            u = NS_PYMT;
            break;
        default:
            u = NS_TX_XRP_TYPE_NS;
            break;
    }
    
    return u;
};

// JUB_NS_XRP_PYMT_TYPE -> JUB_ENUM_XRP_PYMT_TYPE
JUB_ENUM_XRP_PYMT_TYPE (^inlineXRPPymtType)(JUB_NS_XRP_PYMT_TYPE) = ^(JUB_NS_XRP_PYMT_TYPE argument) {
    JUB_ENUM_XRP_PYMT_TYPE u;
    switch (argument) {
        case NS_DXRP:
            u = JUB_ENUM_XRP_PYMT_TYPE::DXRP;
            break;
        default:
            u = JUB_ENUM_XRP_PYMT_TYPE::NS_ITEM_PYMT_TYPE;
            break;
    }
    
    return u;
};

// JUB_NS_XRP_PYMT_TYPE <- JUB_ENUM_XRP_PYMT_TYPE
JUB_NS_XRP_PYMT_TYPE (^inlineNSXRPPymtType)(JUB_ENUM_XRP_PYMT_TYPE) = ^(JUB_ENUM_XRP_PYMT_TYPE argument) {
    JUB_NS_XRP_PYMT_TYPE u;
    switch (argument) {
        case JUB_ENUM_XRP_PYMT_TYPE::DXRP:
            u = NS_DXRP;
            break;
        default:
            u = NS_DXRP_XRP_TYPE_NS;
            break;
    }
    
    return u;
};

// PymtAmount* -> JUB_PYMT_AMOUNT
JUB_PYMT_AMOUNT (^inlinePymtAmount)(PymtAmount*) = ^(PymtAmount* pymtAmount) {
    JUB_PYMT_AMOUNT amount;
    
    amount.currency = (JUB_CHAR_PTR)[pymtAmount.currency UTF8String];
    amount.value    = (JUB_CHAR_PTR)[pymtAmount.value UTF8String];
    amount.issuer   = (JUB_CHAR_PTR)[pymtAmount.issuer UTF8String];
    
    return amount;
};

// PymtAmount* <- JUB_PYMT_AMOUNT
PymtAmount* (^inlineNSPymtAmount)(JUB_PYMT_AMOUNT&) = ^(JUB_PYMT_AMOUNT& pymtAmount) {
    PymtAmount* amount = [[PymtAmount alloc] init];
    
    amount.currency = [[NSString alloc] init];
    amount.currency = [[NSString stringWithCString:pymtAmount.currency
                                          encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(pymtAmount.currency);
    
    amount.value = [[NSString alloc] init];
    amount.value = [[NSString stringWithCString:pymtAmount.value
                                       encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(pymtAmount.value);

    amount.issuer = [[NSString alloc] init];
    amount.issuer = [[NSString stringWithCString:pymtAmount.issuer
                                        encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(pymtAmount.issuer);
    
    return amount;
};

// Payment* -> JUB_PYMT_XRP
JUB_PYMT_XRP (^inlinePayment)(Payment*) = ^(Payment* pymt) {
    JUB_PYMT_XRP payment;
    
    payment.type           = inlineXRPPymtType(pymt.type);
    payment.amount         = inlinePymtAmount(pymt.amount);
    payment.destination    = (JUB_CHAR_PTR)[pymt.destination UTF8String];
    payment.destinationTag = (JUB_CHAR_PTR)[pymt.destinationTag UTF8String];
    payment.invoiceID      = (JUB_CHAR_PTR)[pymt.invoiceID UTF8String];
    payment.sendMax        = inlinePymtAmount(pymt.sendMax);
    payment.deliverMin     = inlinePymtAmount(pymt.deliverMin);
    
    return payment;
};

// Payment* <- JUB_PYMT_XRP
Payment* (^inlineNSPayment)(JUB_PYMT_XRP&) = ^(JUB_PYMT_XRP& pymt) {
    Payment* payment = [[Payment alloc] init];
    
    payment.type   = inlineNSXRPPymtType(pymt.type);
    payment.amount = inlineNSPymtAmount(pymt.amount);
    
    payment.destination = [[NSString alloc] init];
    payment.destination = [[NSString stringWithCString:pymt.destination
                                              encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(pymt.destination);
    
    payment.destinationTag = [[NSString alloc] init];
    payment.destinationTag = [[NSString stringWithCString:pymt.destinationTag
                                                 encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(pymt.destinationTag);
    
    payment.invoiceID = [[NSString alloc] init];
    payment.invoiceID = [[NSString stringWithCString:pymt.invoiceID
                                            encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(pymt.invoiceID);
    
    payment.sendMax    = inlineNSPymtAmount(pymt.sendMax);
    payment.deliverMin = inlineNSPymtAmount(pymt.deliverMin);
    
    return payment;
};

// TxXRP* -> JUB_TX_XRP
JUB_TX_XRP (^inlineTxXRP)(TxXRP*) = ^(TxXRP* xrpTx) {
    JUB_TX_XRP tx;
    
    tx.account   = (JUB_CHAR_PTR)[xrpTx.account UTF8String];
    
    tx.type      = inlineXRPTxType(xrpTx.type);
    tx.fee       = (JUB_CHAR_PTR)[xrpTx.fee UTF8String];
    tx.sequence  = (JUB_CHAR_PTR)[xrpTx.sequence UTF8String];
    tx.accountTxnID = (JUB_CHAR_PTR)[xrpTx.accountTxnID UTF8String];
    tx.flags     = (JUB_CHAR_PTR)[xrpTx.flags UTF8String];
    tx.lastLedgerSequence = (JUB_CHAR_PTR)[xrpTx.lastLedgerSequence UTF8String];
    tx.memos     = (JUB_CHAR_PTR)[xrpTx.memos UTF8String];
    tx.sourceTag = (JUB_CHAR_PTR)[xrpTx.sourceTag UTF8String];
    
    switch (tx.type) {
        case JUB_ENUM_XRP_TX_TYPE::PYMT:
//            inlinePayment(xrpTx.pymt);
            tx.pymt.type   = inlineXRPPymtType(xrpTx.pymt.type);
            tx.pymt.amount = inlinePymtAmount(xrpTx.pymt.amount);
            tx.pymt.destination    = (JUB_CHAR_PTR)[xrpTx.pymt.destination UTF8String];
            tx.pymt.destinationTag = (JUB_CHAR_PTR)[xrpTx.pymt.destinationTag UTF8String];
            tx.pymt.invoiceID = (JUB_CHAR_PTR)[xrpTx.pymt.invoiceID UTF8String];
            tx.pymt.sendMax    = inlinePymtAmount(xrpTx.pymt.sendMax);
            tx.pymt.deliverMin = inlinePymtAmount(xrpTx.pymt.deliverMin);
            break;
        case JUB_ENUM_XRP_TX_TYPE::NS_ITEM_TX_TYPE:
        default:
            break;
    }
    
    return tx;
};

// TxXRP* <- JUB_TX_XRP
TxXRP* (^inlineNSTxXRP)(JUB_TX_XRP) = ^(JUB_TX_XRP xrpTx) {
    TxXRP* tx = [[TxXRP alloc] init];
    
    tx.account = [[NSString alloc] init];
    tx.account = [[NSString stringWithCString:xrpTx.account
                                     encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.account);
    
    tx.type = inlineNSXRPTxType(xrpTx.type);
    
    tx.fee = [[NSString alloc] init];
    tx.fee = [[NSString stringWithCString:xrpTx.fee
                                 encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.fee);
    
    tx.sequence = [[NSString alloc] init];
    tx.sequence = [[NSString stringWithCString:xrpTx.sequence
                                      encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.sequence);
    
    tx.accountTxnID = [[NSString alloc] init];
    tx.accountTxnID = [[NSString stringWithCString:xrpTx.accountTxnID
                                          encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.accountTxnID);
    
    tx.flags = [[NSString alloc] init];
    tx.flags = [[NSString stringWithCString:xrpTx.flags
                                   encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.flags);
    
    tx.lastLedgerSequence = [[NSString alloc] init];
    tx.lastLedgerSequence = [[NSString stringWithCString:xrpTx.lastLedgerSequence
                                                encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.lastLedgerSequence);
    
    tx.memos = [[NSString alloc] init];
    tx.memos = [[NSString stringWithCString:xrpTx.memos
                                   encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.memos);
    
    tx.sourceTag = [[NSString alloc] init];
    tx.sourceTag = [[NSString stringWithCString:xrpTx.sourceTag
                                       encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xrpTx.sourceTag);
    
    switch (xrpTx.type) {
        case JUB_ENUM_XRP_TX_TYPE::PYMT:
//            inlineNSPayment(xrpTx.pymt);
            tx.pymt.type   = inlineNSXRPPymtType(xrpTx.pymt.type);
            
            tx.pymt.amount = inlineNSPymtAmount(xrpTx.pymt.amount);
            
            tx.pymt.destination = [[NSString alloc] init];
            tx.pymt.destination = [[NSString stringWithCString:xrpTx.pymt.destination
                                                      encoding:NSUTF8StringEncoding] copy];
            JUB_FreeMemory(xrpTx.pymt.destination);
            
            tx.pymt.destinationTag = [[NSString alloc] init];
            tx.pymt.destinationTag = [[NSString stringWithCString:xrpTx.pymt.destinationTag
                                                         encoding:NSUTF8StringEncoding] copy];
            JUB_FreeMemory(xrpTx.pymt.destinationTag);
            
            tx.pymt.invoiceID = [[NSString alloc] init];
            tx.pymt.invoiceID = [[NSString stringWithCString:xrpTx.pymt.invoiceID
                                                    encoding:NSUTF8StringEncoding] copy];
            JUB_FreeMemory(xrpTx.pymt.invoiceID);
            
            tx.pymt.sendMax    = inlineNSPymtAmount(xrpTx.pymt.sendMax);
            tx.pymt.deliverMin = inlineNSPymtAmount(xrpTx.pymt.deliverMin);
            break;
        case JUB_ENUM_XRP_TX_TYPE::NS_ITEM_TX_TYPE:
        default:
            break;
    }
    
    return tx;
};

extern JUB_ENUM_BOOL (^inlineBool)(JUB_NS_ENUM_BOOL);

@implementation JubSDKCore (COIN_XRP)

//JUB_RV JUB_CreateContextXRP_soft(IN CONTEXT_CONFIG_XRP cfg,
//                                 IN JUB_CHAR_PTR masterPriInXPRV,
//                                 OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextXRP_soft:(ContextConfigXRP*)cfg
                        masterPriInXPRV:(NSString*)masterPriInXPRV
{
    self.lastError = JUBR_OK;
    
    CONTEXT_CONFIG_XRP cfgXRP;
    cfgXRP.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextXRP_soft(cfgXRP,
                                          (JUB_CHAR_PTR)[masterPriInXPRV UTF8String],
                                          &contextID);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_CreateContextXRP(IN CONTEXT_CONFIG_XRP cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextXRP:(NSUInteger)deviceID
                               cfg:(ContextConfigXRP*)cfg
{
    self.lastError = JUBR_OK;
    
    CONTEXT_CONFIG_XRP cfgXRP;
    cfgXRP.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextXRP(cfgXRP,
                                     deviceID,
                                     &contextID);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_GetAddressXRP(IN JUB_UINT16 contextID,
//                         IN BIP44_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressXRP:(NSUInteger)contextID
                          path:(BIP44Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_ENUM_BOOL isShow = inlineBool(bShow);
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_GetAddressXRP(contextID,
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

//JUB_RV JUB_SetMyAddressXRP(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressXRP:(NSUInteger)contextID
                            path:(BIP44Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_SetMyAddressXRP(contextID,
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

//JUB_RV JUB_SignTransactionXRP(IN JUB_UINT16 contextID,
//                              IN BIP44_Path path,
//                              IN JUB_TX_XRP tx,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionXRP:(NSUInteger)contextID
                               path:(BIP44Path*)path
                                 tx:(TxXRP*)tx
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_TX_XRP xrpTx = inlineTxXRP(tx);
    JUB_CHAR_PTR raw = nullptr;
    JUB_RV rv = JUB_SignTransactionXRP(contextID,
                                       bip44Path,
                                       xrpTx,
                                       &raw);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strRaw = [NSString stringWithCString:raw
                                          encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(raw);
    
    return strRaw;
}

@end
