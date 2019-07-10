#ifndef JUB_SDK_H
#define JUB_SDK_H

// Generic helper definitions for shared library support
#if defined _MSC_VER || defined __CYGWIN__
    #define JUB_COINCORE_DLL_IMPORT extern "C" __declspec(dllimport)
    #define JUB_COINCORE_DLL_EXPORT extern "C" __declspec(dllexport)
    #define JUB_COINCORE_DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define JUB_COINCORE_DLL_IMPORT __attribute__((visibility("default")))
        #define JUB_COINCORE_DLL_EXPORT __attribute__((visibility("default")))
        #define JUB_COINCORE_DLL_LOCAL  __attribute__((visibility("internal")))
    #else
        #define JUB_COINCORE_DLL_IMPORT
        #define JUB_COINCORE_DLL_EXPORT
        #define JUB_COINCORE_DLL_LOCAL
    #endif // #if __GNUC__ >= 4
#endif // #if defined _MSC_VER || defined __CYGWIN__

// Tag to deprecate functions and methods.
// Gives a compiler warning when they are used.
#if defined _MSC_VER || defined __CYGWIN__
    #define BC_DEPRECATED __declspec(deprecated)
#else
    #if __GNUC__ >= 4
        #define JUB_DEPRECATED __attribute__((deprecated))
    #else
        #define JUB_DEPRECATED
    #endif // #if __GNUC__ >= 4
#endif // #if defined _MSC_VER || defined __CYGWIN__

// Avoid namespace conflict between boost::placeholders and std::placeholders.
#define BOOST_BIND_NO_PLACEHOLDERS

// Define so we can have better visibility of lcov exclusion ranges.
#define LCOV_EXCL_START(text)
#define LCOV_EXCL_STOP()

#if defined(_WIN32)
    #define HID_MODE
#elif defined(__APPLE__)
// see https://sourceforge.net/p/predef/mailman/message/34497133/
    #include <TargetConditionals.h>
    #if TARGET_OS_OSX // mac osx
        #define HID_MODE
    #elif TARGET_OS_IOS // ios
        #define BLE_MODE
    #endif // #if TARGET_OS_OSX
#elif defined(__ANDROID__)
    #define BLE_MODE
#else //other
    #define HID_MODE
#endif // #if defined(_WIN32)

/* === Library typedef: === */
#ifndef IN
    #define IN
#endif // #ifndef IN

#ifndef OUT
    #define OUT
#endif // #ifndef OUT

#ifndef INOUT
    #define INOUT
#endif // #ifndef INOUT

#define JUB_TRUE    1
#define JUB_FALSE   0

#ifndef JUB_DISABLE_TRUE_FALSE
    #ifndef FALSE
        #define FALSE JUB_FALSE
    #endif // #ifndef FALSE

    #ifndef TRUE
        #define TRUE JUB_TRUE
    #endif // #ifndef TRUE
#endif // #ifndef JUB_DISABLE_TRUE_FALSE

/* an unsigned 8-bit value */
typedef unsigned char JUB_BYTE;

/* an unsigned 8-bit character */
typedef JUB_BYTE JUB_UCHAR;

/* an unsigned/signed 8-bit character, decide by complie*/
typedef char JUB_CHAR;

/* an 8-bit UTF-8 character */
typedef JUB_BYTE JUB_UTF8UCHAR;

/* a BYTE-sized Boolean flag */
typedef JUB_BYTE JUB_BBOOL;

/* an unsigned value, at least 32 bits long */
typedef unsigned long int JUB_ULONG;

/* a signed value, the same size as a JUB_ULONG */
typedef signed long int  JUB_LONG;

typedef unsigned int JUB_UINT32;

typedef unsigned short JUB_UINT16;

/* uint64 */
typedef unsigned long long JUB_UINT64;

/* signed uint64 */
typedef signed long long JUB_INT64;

#define JUB_PTR *
typedef JUB_CHAR JUB_PTR            JUB_CHAR_PTR;
typedef JUB_CHAR_PTR JUB_PTR        JUB_CHAR_PTR_PTR;
typedef const JUB_CHAR JUB_PTR      JUB_CHAR_CPTR;
typedef const JUB_BYTE JUB_PTR      JUB_BYTE_CPTR;

typedef JUB_BYTE JUB_PTR            JUB_BYTE_PTR;
typedef JUB_UCHAR JUB_PTR           JUB_UCHAR_PTR;
typedef JUB_UTF8UCHAR JUB_PTR       JUB_UTF8UCHAR_PTR;
typedef JUB_ULONG JUB_PTR           JUB_ULONG_PTR;
typedef JUB_UINT64 JUB_PTR			JUB_UINT64_PTR;
typedef void JUB_PTR                JUB_VOID_PTR;

/* Pointer to a JUB_VOID_PTR-- i.e., pointer to pointer to void */
typedef JUB_VOID_PTR JUB_PTR JUB_VOID_PTR_PTR;

typedef JUB_ULONG JUB_RV;

#define MAX_DEVICE                  8

#define JUBR_OK                     0x00000000UL
#define JUBR_ERROR                  0x00000001UL
#define JUBR_HOST_MEMORY            0x00000002UL
#define JUBR_ARGUMENTS_BAD          0x00000003UL
#define JUBR_IMPL_NOT_SUPPORT		0x00000004UL
#define JUBR_MEMORY_NULL_PTR		0x00000005UL
#define JUBR_DATABASE_ERROR			0x00000006UL
#define JUBR_DATABASE_EMPTY			0x00000007UL
#define JUBR_INVALID_MEMORY_PTR		0x00000008UL
#define JUBR_REPEAT_MEMORY_PTR		0x00000009UL
#define JUBR_BUFFER_TOO_SMALL		0x0000000AUL

#define JUBR_COIN_TYPE_ALREADY_EXIST 0x00000010UL
#define JUBR_COIN_TYPE_NOT_EXIST	0x00000011UL
#define JUBR_ADD_COIN_TYPE_ERROR	0x00000012UL
#define JUBR_UNKNOWN_COIN_TYPE	    0x00000013UL

#define JUBR_WALLET_ALREADY_EXIST	0x00000020UL
#define JUBR_CREATE_WALLET_ERROR	0x00000021UL
#define JUBR_WALLET_NOT_EXIST    	0x00000022UL
#define JUBR_WALLET_LOADED    		0x00000023UL
#define JUBR_WALLET_NOT_LOAD		0x00000024UL
#define JUBR_WALLETS_EMPTY          0x00000025UL

#define JUBR_INVALID_BIP_PATH		0x00000030UL
#define JUBR_INVALID_MNEMONIC		0x00000031UL
#define JUBR_INVALID_ADDRESS		0x00000032UL
#define JUBR_INSUFFICIENT_COINS		0x00000033UL
#define JUBR_TOO_MANY_INPUTS		0x00000034UL
#define JUBR_NOT_SUPPORT_INPUTS		0x00000035UL
#define JUBR_INVAIDE_TRANSACTION	0x00000036UL
#define JUBR_NO_REDEEMSCRIPT		0x00000037UL

#define JUBR_VERIFY_PIN_ERROR		0x00000040UL
#define JUBR_CHANGE_PIN_ERROR		0x00000041UL
#define JUBR_PIN_NOT_SET			0x00000042UL

#define JUBR_INIT_DEVICE_LIB_ERROR	0x00000060UL
#define JUBR_CONNECT_DEVICE_ERROR	0x00000061UL
#define JUBR_TRANSMIT_DEVICE_ERROR	0x00000062UL
#define JUBR_NOT_CONNECT_DEVICE	    0x00000063UL
#define JUBR_DEVICE_PIN_ERROR	    0x00000064UL
#define JUBR_USER_CANCEL    	    0x00000065UL
#define JUBR_ERROR_ARGS    	        0x00000066UL
#define JUBR_PIN_LOCKED    	        0x00000067UL

#define JUBR_ACCT_SYNC_DATA_FINISH  0x00000070UL

#define JUBR_DEVICE_BUSY            0x00001001
#define JUBR_TRANSACT_TIMEOUT       0x00001002
#define JUBR_OTHER_ERROR            0x00001003
#define JUBR_CMD_ERROR              0x00001004
#define JUBR_BT_BOND_FAILED         0x00001005

#define JUBR_CUSTOM_DEFINED         0x80000000UL

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef enum {
    BOOL_FALSE = 0,
    BOOL_TRUE,
    BOOL_NR_ITEMS
} JUB_ENUM_BOOL;

typedef enum {
    COINBTC,
    COINBCH,
    COINLTC,
    COINUSDT,
    Default = COINBTC
} JUB_ENUM_COINTYPE_BTC;

typedef struct {
	JUB_CHAR label[32];
	JUB_CHAR sn[24];
	JUB_UINT16 pinRetry;
	JUB_UINT16 pinMaxRetry;
	JUB_CHAR bleVersion[4];
	JUB_CHAR firmwareVersion[4];
} JUB_DEVICE_INFO;
typedef JUB_DEVICE_INFO* JUB_DEVICE_INFO_PTR;

typedef enum {
	p2pkh = 0,
	//p2pwpkh,
	p2sh_p2wpkh
	/*
	p2sh_multisig,
	p2wsh_multisig,
	p2sh_p2wsh_multisig,
	*/
} JUB_BTC_TRANS_TYPE;

typedef enum {
	BTC = 0x00,
	cBTC,
	mBTC,
	uBTC,
	Satoshi
} JUB_BTC_UNIT_TYPE;

typedef enum {
	HEX = 0x00,
	XPUB = 0x01
} JUB_ETH_PUB_FORMAT;

typedef struct {
	JUB_ENUM_BOOL change;
	JUB_UINT64    addressIndex;
} BIP32_Path;

typedef struct {
    JUB_ENUM_COINTYPE_BTC   coinType;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };

	JUB_CHAR_PTR			mainPath;
	JUB_BTC_TRANS_TYPE		transType;
} CONTEXT_CONFIG_BTC;

typedef struct {
	JUB_CHAR_PTR		mainPath;
	int					chainID;
} CONTEXT_CONFIG_ETH;

typedef struct {
	JUB_CHAR_PTR	preHash;
	JUB_UINT16      preIndex;
	JUB_UINT64		amount;
	BIP32_Path      path;
} INPUT_BTC;

typedef enum {
	P2PKH = 0x00,
	RETURN0 = 0x01
} OUTPUT_BTC_TYPE;

typedef struct {
	JUB_CHAR_PTR	address;
	JUB_UINT64		amount;
	JUB_ENUM_BOOL   changeAddress;
	BIP32_Path      path;
} OUTPUT_P2PKH;

typedef struct {
	JUB_UINT64		amount;
	JUB_UINT16      dataLen;
	JUB_BYTE        data[40];	
} OUTPUT_RETURN0;

typedef struct {
	OUTPUT_BTC_TYPE type;
	union {
		OUTPUT_P2PKH   outputP2pkh;
		OUTPUT_RETURN0 outputReturn0;
	};
} OUTPUT_BTC;

/*****************************************************************************
* @function name : JUB_ListDeviceHid
* @in  param :
* @out param : deviceIDs - device ID list
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE]);

/*****************************************************************************
* @function name : JUB_ConnetDeviceHid
* @in  param : deviceID - device ID
* @out param :
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ConnetDeviceHid(IN JUB_UINT16 deviceID);

/*****************************************************************************
* @function name : JUB_DisconnetDeviceHid
* @in  param : deviceID - device ID
* @out param : 
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_DisconnetDeviceHid(IN JUB_UINT16 deviceID);

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
                       IN JUB_CHAR_PTR apdu ,
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
* @function name : JUB_CreateContextBTC
* @in  param : cfg
*            : deviceID - device ID
* @out param : contextID
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
* @function name : JUB_ClearContext
* @in  param : contextID - context ID
* @out param : 
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID);

/*****************************************************************************
* @function name : JUB_BuildUSDTOutputs
* @in  param : contextID - context ID
*            : USDTTo - to address
*            : amount
* @out param : outputs
* @last change : build the return0 and dust 2 outputs
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_PTR USDTTo,
                            IN JUB_UINT64 amount,
                            OUT OUTPUT_BTC outputs[2]);

/*****************************************************************************
* @function name : JUB_SignTransactionBTC
* @in  param : contextID - context ID
*            : inputs
*            : iCount
*            : outputs
*            : oCount
*            : lockTime
* @out param : raw
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
                              IN JUB_UINT32 lockTime,
                              OUT JUB_CHAR_PTR_PTR raw);

/*****************************************************************************
* @function name : JUB_ShowVirtualPwd
* @in  param : contextID - context ID
* @out param : 
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID);

/*****************************************************************************
* @function name : JUB_CancelVirtualPwd
* @in  param : contextID - context ID
* @out param : 
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID);

/*****************************************************************************
* @function name : JUB_VerifyPIN
* @in  param : contextID - context ID
*            : pinMix
* @out param : retry
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID,
                     IN JUB_CHAR_PTR pinMix,
                     OUT JUB_ULONG_PTR pretry);

/*****************************************************************************
* @function name : JUB_GetHDNodeBTC
* @in  param : contextID - context ID
*            : path
* @out param : xpub
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
                        IN BIP32_Path path,
                        OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
* @function name : JUB_GetMainHDNodeBTC
* @in  param : contextID - context ID
* @out param : xpub
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeBTC(IN JUB_UINT16 contextID,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
* @function name : JUB_GetAddressBTC
* @in  param : contextID - context ID
*            : path
*            : bShow
* @out param : address
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressBTC(IN JUB_UINT16 contextID,
                         IN BIP32_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
* @function name : JUB_SetMyAddressBTC
* @in  param : contextID - context ID
*            : path
* @out param : address
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressBTC(IN JUB_UINT16 contextID,
                           IN BIP32_Path path,
                           OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
* @function name : JUB_CreateContextETH
* @in  param : cfg
*            : deviceID - device ID
* @out param : contextID
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
* @function name : JUB_GetAddressETH
* @in  param : contextID - context ID
*            : path
*            : bShow
* @out param : address
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
                         IN BIP32_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
* @function name : JUB_GetHDNodeETH
* @in  param : contextID - context ID
*            : format - JUB_ETH_PUB_FORMAT::HEX (0x00) for hex;
*                       JUB_ETH_PUB_FORMAT::XPUB(0x01) for xpub
*            : path
* @out param : pubkey
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
                        IN JUB_ETH_PUB_FORMAT format,
                        IN BIP32_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
* @function name : JUB_GetMainHDNodeETH
* @in  param : contextID - context ID
*            : format - JUB_ETH_PUB_FORMAT::HEX (0x00) for hex;
*                       JUB_ETH_PUB_FORMAT::XPUB(0x01) for xpub
* @out param : xpub
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
                            IN JUB_ETH_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
* @function name : JUB_SetMyAddressETH
* @in  param : contextID - context ID
*            : path
* @out param : address
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
                           IN BIP32_Path path,
                           OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
* @function name : JUB_SignTransactionETH
* @in  param : contextID - context ID
*            : path
*            : nonce - nonce
*            : gasLimit - gas limit
*            : gasPriceInWei - gas price in wei
*            : to
*            : valueInWei - value in wei
*            : input
* @out param : raw
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionETH(IN JUB_UINT16 contextID,
                              IN BIP32_Path path,
                              IN JUB_UINT32 nonce,
                              IN JUB_UINT32 gasLimit,
                              IN JUB_CHAR_PTR gasPriceInWei,
                              IN JUB_CHAR_PTR to,
                              IN JUB_CHAR_PTR valueInWei,
                              IN JUB_CHAR_PTR input,
                              OUT JUB_CHAR_PTR_PTR raw);

/*****************************************************************************
* @function name : JUB_BuildERC20AbiETH
* @in  param : contextID - context ID
*            : token_to
*            : token_value
* @out param : abi
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_PTR tokenTo, IN JUB_CHAR_PTR tokenValue,
                            OUT JUB_CHAR_PTR_PTR abi);

/*****************************************************************************
* @function name : JUB_SetUnitBTC
* @in  param : contextID - context ID
*            : unit
* @out param :
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetUnitBTC(IN JUB_UINT16 contextID,
                      IN JUB_BTC_UNIT_TYPE unit);

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
* @function name : JUB_QueryBattery
* @in  param : deviceID - device ID
* @out param : percent
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_QueryBattery(IN JUB_UINT16 deviceID,
                        OUT JUB_BYTE_PTR percent);

/*****************************************************************************
* @function name : JUB_FreeMemory
* @in  param : memPtr
* @out param : 
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_FreeMemory(IN JUB_CHAR_CPTR memPtr);

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
* @function name : Jub_EnumSupportCoins
* @in  param : deviceID - device ID
* @out param : coinsList - coin list
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV Jub_EnumSupportCoins(IN JUB_UINT16 deviceID,
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
                            IN JUB_CHAR_PTR appID,
                            OUT JUB_CHAR_PTR_PTR version);

/*****************************************************************************
* @function name : JUB_GetVersion
* @in  param :
* @out param : 
* @last change : 
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_CHAR_PTR JUB_GetVersion(void);

/// ble device APIs //////////////////////////////////////////
#define DEV_SUCCESS 0               /**< no error */
#define DEV_TIMEOUT 1               /**< conn time out */
#define DEV_COMMUNICATION_ERROR 612 /**< generic error */
#define DEV_RESPONSE_TIMEOUT    613 /**< timeout */
#define DEV_NOT_SUPPORTED       614 /**< request is not supported */
#define DEV_NO_DEVICE           615 /**< no device>*/

typedef int(*DEV_ReadCallBack)(JUB_ULONG devHandle, JUB_BYTE_PTR data, JUB_UINT32 dataLen);

typedef void(*DEV_ScanCallBack)(JUB_BYTE_PTR devName, JUB_BYTE_PTR uuid, JUB_UINT32 type);

typedef void(*DEV_DiscCallBack)(JUB_BYTE_PTR uuid);

typedef struct _DEVICE_INIT_PARAM_ {
	void* param;
	DEV_ReadCallBack callBack;
	DEV_ScanCallBack scanCallBack;
	DEV_DiscCallBack discCallBack;
} DEVICE_INIT_PARAM;

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_initDevice(IN DEVICE_INIT_PARAM param);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_enumDevices(void);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_stopEnumDevices(void);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_connectDevice(
	JUB_BYTE_PTR bBLEUUID,  /**< ble device UUID */
	JUB_UINT32 connectType, /**< ble device connect type */
	JUB_UINT16* pDeviceID,  /**< output ble device connect handle */
	JUB_UINT32 timeout);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_cancelConnect(JUB_BYTE_PTR bBLEUUID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_disconnectDevice(JUB_UINT16 deviceID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_isDeviceConnect(JUB_UINT16 deviceID);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_CORE_H */
