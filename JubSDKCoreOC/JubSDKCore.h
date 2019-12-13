//
//  JubSDKCore.h
//  JubSDKCore
//
//  Created by Pan Min on 2019/6/22.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import <Foundation/Foundation.h>

#define JUBR_OK                     0x00000000UL

#define JUBR_ERROR                  0x00000001UL
#define JUBR_HOST_MEMORY            0x00000002UL
#define JUBR_ARGUMENTS_BAD          0x00000003UL
#define JUBR_IMPL_NOT_SUPPORT       0x00000004UL
#define JUBR_MEMORY_NULL_PTR        0x00000005UL

#define JUBR_INVALID_MEMORY_PTR     0x00000008UL
#define JUBR_REPEAT_MEMORY_PTR      0x00000009UL
#define JUBR_BUFFER_TOO_SMALL       0x0000000AUL

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
//    STRENGTH128 = 128,
//    STRENGTH192 = 192,
//    STRENGTH256 = 256
//} JUB_ENUM_MNEMONIC_STRENGTH;
typedef NS_ENUM(NSInteger, JUB_NS_ENUM_MNEMONIC_STRENGTH) {
    JUB_NS_ENUM_MNEMONIC_STRENGTH128 = 128,
    JUB_NS_ENUM_MNEMONIC_STRENGTH192 = 192,
    JUB_NS_ENUM_MNEMONIC_STRENGTH256 = 256,
    JUB_NS_ENUM_MNEMONIC_STRENGTH_NR_ITEMS
};

//typedef enum {
//    secp256k1 = 0,
//    ed25519,
//    nist256p1
//} JUB_ENUM_CURVES;
typedef NS_ENUM(NSInteger, JUB_NS_ENUM_CURVES) {
    JUB_NS_ENUM_SECP256K1 = 0,
    JUB_NS_ENUM_ED25519,
    JUB_NS_ENUM_NIST256P1,
    JUB_NS_ENUM_NR_ITEMS
};

//typedef struct {
//    JUB_ENUM_BOOL change;
//    JUB_UINT64    addressIndex;
//} BIP44_Path;
@interface BIP44Path : NSObject
@property (atomic, assign) JUB_NS_ENUM_BOOL change;
@property (atomic, assign) NSInteger addressIndex;
@end

//typedef struct stContextCfg {
//    JUB_CHAR_PTR            main_path;
//} CONTEXT_CONFIG;
@interface ContextConfig : NSObject
@property (atomic, copy  ) NSString* mainPath;
@end

typedef void (*ProgressCallback)(unsigned int current, unsigned int total);

@interface JubSDKCore : NSObject
@property (atomic, assign) NSUInteger lastError;

- (NSUInteger)JUB_LastError;

//JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID);
- (void)JUB_ClearContext:(NSUInteger)contextID;

//JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_ShowVirtualPwd:(NSUInteger)contextID;

//JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_CancelVirtualPwd:(NSUInteger)contextID;

//JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID,
//                     IN JUB_CHAR_PTR pinMix,
//                     OUT JUB_ULONG_PTR retry);
- (NSUInteger)JUB_VerifyPIN:(NSUInteger)contextID
                     pinMix:(NSString*)pinMix;

//JUB_CHAR_PTR JUB_GetVersion(void);
- (NSString*)JUB_GetVersion;

//JUB_RV JUB_GenerateMnemonic(IN JUB_ENUM_MNEMONIC_STRENGTH strength,
//                            OUT JUB_CHAR_PTR_PTR mnemonic);
- (NSString*)JUB_GenerateMnemonic:(JUB_NS_ENUM_MNEMONIC_STRENGTH)strength;

//JUB_RV JUB_CheckMnemonic(IN JUB_CHAR_CPTR mnemonic);
- (void)JUB_CheckMnemonic:(NSString*)mnemonic;

//JUB_RV JUB_GenerateSeed(IN JUB_CHAR_CPTR mnemonic, IN JUB_CHAR_CPTR passphrase,
//                        OUT JUB_BYTE seed[64],
//                        void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total));
- (NSString*)JUB_GenerateSeed:(NSString*)mnemonic
                   passphrase:(NSString*)passphrase
                     callback:(ProgressCallback)callback;

//JUB_RV JUB_SeedToMasterPrivateKey(IN JUB_BYTE_PTR seed, IN JUB_UINT16 seed_len,
//                                  IN JUB_ENUM_CURVES curve,
//                                  OUT JUB_CHAR_PTR_PTR prikeyInXPRV);
- (NSString*)JUB_SeedToMasterPrivateKey:(NSString*)seed
                                seedLen:(NSInteger)seedLen
                                  curve:(JUB_NS_ENUM_CURVES)curve;

@end
