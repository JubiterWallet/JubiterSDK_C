#ifndef JUB_SDK_HCASH_H
#define JUB_SDK_HCASH_H

#include "JUB_SDK_BTC.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef CONTEXT_CONFIG CONTEXT_CONFIG_HC;

// Remove c++ features for swift framework
typedef struct stInputHC {
	JUB_UINT64		amount;
	BIP44_Path      path;
//
//     stInputHC();
//    ~stInputHC() = default;
} INPUT_HC;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stOutputHC {
	JUB_ENUM_BOOL   changeAddress;
	BIP44_Path      path;
//
//     stOutputHC();
//    ~stOutputHC() = default;
} OUTPUT_HC;
// Remove c++ features for swift framework end

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextHC(IN CONTEXT_CONFIG_HC cfg,
                           IN JUB_UINT16 deviceID,
                           OUT JUB_UINT16* contextID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeHC(IN JUB_UINT16 contextID,
                           OUT JUB_CHAR_PTR_PTR xpub);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeHC(IN JUB_UINT16 contextID,
                       IN BIP44_Path path,
                       OUT JUB_CHAR_PTR_PTR xpub);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressHC(IN JUB_UINT16 contextID,
                        IN BIP44_Path path,
                        IN JUB_ENUM_BOOL bShow,
                        OUT JUB_CHAR_PTR_PTR address);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionHC(IN JUB_UINT16 contextID,
                             IN JUB_UINT32 version,
                             IN INPUT_HC inputs[], IN JUB_UINT16 iCount,
                             IN OUTPUT_HC outputs[], IN JUB_UINT16 oCount,
                             IN JUB_CHAR_CPTR unsignedTrans,
                             OUT JUB_CHAR_PTR_PTR raw);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif /* JUB_SDK_HCASH_H */
