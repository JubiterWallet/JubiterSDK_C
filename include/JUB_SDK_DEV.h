//
//  JUB_SDK_DEV.h
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DEV_h
#define JUB_SDK_DEV_h

#include "JUB_SDK.h"

#define JUBR_INIT_DEVICE_LIB_ERROR  0x00000060UL
#define JUBR_CONNECT_DEVICE_ERROR   0x00000061UL
#define JUBR_TRANSMIT_DEVICE_ERROR  0x00000062UL
#define JUBR_NOT_CONNECT_DEVICE     0x00000063UL
#define JUBR_DEVICE_PIN_ERROR       0x00000064UL
#define JUBR_USER_CANCEL            0x00000065UL
#define JUBR_ERROR_ARGS             0x00000066UL
#define JUBR_PIN_LOCKED             0x00000067UL

#define JUBR_DEVICE_BUSY            0x00001001
#define JUBR_TRANSACT_TIMEOUT       0x00001002
#define JUBR_OTHER_ERROR            0x00001003
#define JUBR_CMD_ERROR              0x00001004
#define JUBR_BT_BOND_FAILED         0x00001005


//token errors
#define JUBR_UNKNOWN_COS_ERROR				 0x40000000UL
#define JUBR_NOT_FOUND_MASTER_KEY_TYPE       0x40006401UL
#define JUBR_NOT_FOUND_ECC_KEY_TYPE          0x40006402UL
#define JUBR_NOT_FOUND_ENTROPY_SIZE          0x40006403UL
#define JUBR_WRONG_ENTROPY_SIZE              0x40006404UL
#define JUBR_NOT_FOUND_MASTER_KEY_SIZE       0x40006405UL
#define JUBR_WRONG_MASTER_KEY_SIZE           0x40006406UL

#define JUBR_NOT_FOUND_PASS_PHRASE           0x40006411UL
#define JUBR_NOT_FOUND_MNEMONICS             0x40006412UL
#define JUBR_NOT_FOUND_MNMONICE_IN_DICT      0x40006413UL
#define JUBR_MNOMENIC_LEN_OUT_OF_LIMIT       0x40006414UL
#define JUBR_ENTROPY_BITS_SIZE_MUST_BE_THE_TIMES_OF_EIGHT     0x40006415UL
#define JUBR_MNMONIC_WORD_COUNT_MUST_BE_TIMES_OF_THREE        0x40006416UL
#define JUBR_NEED_TO_GENERATE_MNOMENIC_FIRST 0x40006417UL

#define JUBR_CLA_NOT_SUPPORTED               0x40006E00UL
#define JUBR_WRONG_LENGTH                    0x40006C00UL
#define JUBR_WRONG_OFFSET                    0x40006B00UL
#define JUBR_INS_NOT_SUPPORTED               0x40006D00UL
#define JUBR_WRONG_P1P2                      0x40006A86UL
#define JUBR_WRONG_P3                        0x40006700UL
#define JUBR_WRONG_LE                        0x40006C00UL
#define JUBR_FILE_INVALID                    0x40006283UL
#define JUBR_AUTHENTICATION_FAILED           0x40006300UL
#define JUBR_NO_MESSAGE_SENT                 0x40006580UL
#define JUBR_WRITE_NVM_FAILED                0x40006581UL
#define JUBR_COMMUNICATION_FAILED            0x40006600UL
#define JUBR_REFERENCE_KEY_NOT_FOUND         0x40006980UL
#define JUBR_FILE_TYPE_INCORRECT             0x40006981UL
#define JUBR_SECURITY_STATUS_NOT_SATIFIED    0x40006982UL
#define JUBR_AUTHENTICATION_BLOCK            0x40006983UL
#define JUBR_DATA_INVALID                    0x40006984UL
#define JUBR_CONDITIONS_NOT_SATISFIED        0x40006985UL
#define JUBR_COMMAND_NOT_ALLOWED             0x40006986UL
#define JUBR_MAC_INVALID                     0x40006988UL
#define JUBR_DATA_PARAMETER_INCORRECT        0x40006A80UL
#define JUBR_FUNC_NOT_SUPPORTED              0x40006A81UL
#define JUBR_FILE_NOT_FOUND                  0x40006A82UL
#define JUBR_RECORD_NOT_FOUNT                0x40006A83UL
#define JUBR_FILE_FULL                       0x40006A84UL
#define JUBR_REFERENCE_DATA_NOT_FOUND        0x40006A88UL
#define JUBR_FID_EXIST_ALREADY               0x40006A89UL
#define JUBR_DFNAME_EXIST_ALREADY            0x40006A8AUL
#define JUBR_BINARY_ADDR_OUT                 0x40006B00UL
#define JUBR_KEY_INVALID                     0x400063C0UL


#define JUBR_CUSTOM_DEFINED         0x80000000UL
#define JUBR_EOS_APP_INDEP_OK       JUBR_CUSTOM_DEFINED + 1

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Remove c++ features for swift framework
typedef struct stDevicdInfo {
    JUB_CHAR label[32];
    JUB_CHAR sn[24];
    JUB_UINT16 pinRetry;
    JUB_UINT16 pinMaxRetry;
    JUB_CHAR bleVersion[4];
    JUB_CHAR firmwareVersion[4];
//
//     stDevicdInfo();
//    ~stDevicdInfo() = default;
} JUB_DEVICE_INFO;
typedef JUB_DEVICE_INFO* JUB_DEVICE_INFO_PTR;
// Remove c++ features for swift framework end

typedef enum {
    STRENGTH128 = 128,
    STRENGTH192 = 192,
    STRENGTH256 = 256
} JUB_ENUM_MNEMONIC_STRENGTH;

// The definition is the same as the COS definition
typedef enum {
    SECP256K1 = 0,
    NIST256P1,
    ED25519
} JUB_ENUM_CURVES;

/*****************************************************************************
 * @function name : JUB_GetDeviceInfo
 * @in  param : deviceID - device ID
 * @out param : info - device info
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetDeviceInfo(IN JUB_UINT16 deviceID,
                         OUT JUB_DEVICE_INFO_PTR info);

/*****************************************************************************
 * @function name : JUB_GetDeviceCert
 * @in  param : deviceID - device ID
 * @out param : cert - device certificate
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetDeviceCert(IN JUB_UINT16 deviceID,
                         OUT JUB_CHAR_PTR_PTR cert);

/*****************************************************************************
 * @function name : JUB_SendOneApdu
 * @in  param : deviceID - device ID
 *            : apdu - one apdu
 * @out param : response
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SendOneApdu(IN JUB_UINT16 deviceID,
                       IN JUB_CHAR_CPTR apdu ,
                       OUT JUB_CHAR_PTR_PTR response);

/*****************************************************************************
 * @function name : JUB_IsInitialize
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_ENUM_BOOL JUB_IsInitialize(IN JUB_UINT16 deviceID);

/*****************************************************************************
 * @function name : JUB_IsBootLoader
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_ENUM_BOOL JUB_IsBootLoader(IN JUB_UINT16 deviceID);

/*****************************************************************************
 * @function name : JUB_SetTimeOut
 * @in  param : contextID - context ID
 *            : timeout - how many s
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTimeOut(IN JUB_UINT16 contextID, IN JUB_UINT16 timeout);

/*****************************************************************************
 * @function name : JUB_EnumApplets
 * @in  param : deviceID - device ID
 * @out param : appList - applet list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EnumApplets(IN JUB_UINT16 deviceID,
                       OUT JUB_CHAR_PTR_PTR appList);

/*****************************************************************************
 * @function name : JUB_EnumSupportCoins
 * @in  param : deviceID - device ID
 * @out param : coinsList - coin list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EnumSupportCoins(IN JUB_UINT16 deviceID,
                            OUT JUB_CHAR_PTR_PTR coinsList);

/*****************************************************************************
 * @function name : JUB_GetAppletVersion
 * @in  param : deviceID - device ID
 *            : appID - applet ID
 * @out param : version - applet version
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAppletVersion(IN JUB_UINT16 deviceID,
                            IN JUB_CHAR_CPTR appID,
                            OUT JUB_CHAR_PTR_PTR version);

/*****************************************************************************
* @function name : JUB_Reset
* @in  param : deviceID - device ID
*                     : curve - curve
* @out param :
* @last change : default User's PIN is '5555'.
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_Reset(IN JUB_UINT16 deviceID);

/*****************************************************************************
* @function name : JUB_GenerateSeed
* @in  param : deviceID - device ID
*                     : pinMix - User's PIN
*                     : curve - curve
* @out param :
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GenerateSeed(IN JUB_UINT16 deviceID,
                        IN JUB_CHAR_CPTR pinMix,
                        IN JUB_ENUM_CURVES curve);

/*****************************************************************************
 * @function name : JUB_ImportSeed
 * @in  param : deviceID - device ID
 *          : pinMix - User's PIN
 *          : strength - JUB_ENUM_MNEMONIC_STRENGTH
 *          : entropy - entropy
 *          : seed - seed
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ImportSeed(IN JUB_UINT16 deviceID,
                      IN JUB_CHAR_CPTR pinMix,
                      IN JUB_ENUM_MNEMONIC_STRENGTH strength,
                      IN JUB_CHAR_CPTR entropy,
                      IN JUB_CHAR_CPTR seed);

/*****************************************************************************
 * @function name : JUB_ExportMnemonic
 * @in  param : deviceID - device ID
 *          : pinMix - User's PIN
 * @out param : mnemonic - mnemonic
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ExportMnemonic(IN JUB_UINT16 deviceID,
                          IN JUB_CHAR_CPTR pinMix,
                          OUT JUB_CHAR_PTR_PTR mnemonic);

/*****************************************************************************
* @function name : JUB_VerifyPIN
* @in  param : deviceID - device ID
*                     : pinMix - old PIN
*                     : pinNew - new PIN
* @out param :
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ChangePIN(IN JUB_UINT16 deviceID,
                     IN JUB_CHAR_CPTR pinMix,
                     IN JUB_CHAR_CPTR pinNew);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_DEV_h */
