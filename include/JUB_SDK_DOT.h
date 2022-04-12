//
//  JUB_SDK_DOT.h
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DOT_h
#define JUB_SDK_DOT_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//typedef CONTEXT_CONFIG_DOT CONTEXT_CONFIG_DOT;
typedef enum {
    COINDOT  = 0x00,
    COINKSM  = 0x01,
} JUB_ENUM_COINTYPE_DOT;

typedef struct stContextCfgDOT {
    JUB_CHAR_PTR            mainPath;
    JUB_ENUM_BOOL           mainNet;
    JUB_ENUM_COINTYPE_DOT   coinType;// = { JUB_ENUM_COINTYPE_DOT::COINDOT };
    JUB_ENUM_CURVES         curve; //(sr2551 & ed25519)
} CONTEXT_CONFIG_DOT;

typedef struct stDotBalancesCall {
    JUB_CHAR_PTR to;
    JUB_CHAR_PTR value;
    JUB_BBOOL keep_alive;
} JUB_BALANCES_CALL_DOT;

typedef enum {
    BALANCE_XFER            = 0x005, // balanceTransfer
    UTILITY_BATCH           = 0x01a, // utilityBatch
    STAKING_BOND            = 0x007, // stakingBond
    STAKING_BOND_EXTRA      = 0x107, // stakingBondExtra
    STAKING_UNBOND          = 0x207, // stakingUnbond
    STAKING_WITHDRAW_UNBONDED = 0x307, // stakingWithdrawUnbonded
    STAKING_NOMINATE        = 0x507, // stakingNominate
    STAKING_CHILL           = 0x607, // stakingChill
    STAKING_PAYOUT_STAKERS  = 0x1207, // stakingPayoutStakers
    NS_ITEM_DOT_EXTRINSIC_TYPE
} JUB_ENUM_DOT_EXTRINSIC_TYPE;

typedef enum {
    BOND                = ((STAKING_BOND            & 0xFF00) >> 8), // Bond
//    BOND_AND_NOMINATE   = 2, // BondAndNominate
    BOND_EXTRA          = ((STAKING_BOND_EXTRA      & 0xFF00) >> 8), // BondExtra
    UNBOND              = ((STAKING_UNBOND          & 0xFF00) >> 8), // Unbond
    WITHDRAW_UNBONDED   = ((STAKING_WITHDRAW_UNBONDED& 0xFF00) >> 8), // WithdrawUnbonded
    NOMINATE            = ((STAKING_NOMINATE        & 0xFF00) >> 8), // Nominate
    CHILL               = ((STAKING_CHILL           & 0xFF00) >> 8), // Chill
    PAYOUT_STAKERS      = ((STAKING_PAYOUT_STAKERS  & 0xFF00) >> 8), // PayoutStakers
    NS_ITEM_DOT_STAKING_TYPE
} JUB_ENUM_DOT_STAKING_TYPE;

typedef struct stDotStakingCall {
//    typedef struct stDotStakingBond {
//        JUB_CHAR_PTR controller;
//        JUB_CHAR_PTR value;
//        JUB_UINT8 reward_destination;
//    } JUB_STAKING_CALL_BOND;
//    typedef struct stDotStakingBondAndNominate {
//        JUB_CHAR_PTR controller;
//        JUB_CHAR_PTR value;
//    } JUB_STAKING_CALL_BOND_AND_NOMINATE;
    typedef struct stDotStakingBondExtra {
        JUB_CHAR_PTR value;
    } JUB_STAKING_CALL_BOND_EXTRA;
    typedef struct stDotStakingUnbond {
        JUB_CHAR_PTR value;
    } JUB_STAKING_CALL_UNBOND;
    typedef struct stDotStakingWithdrawUnbonded {
        JUB_INT32 slashing_spans;
    } JUB_STAKING_CALL_WITHDRAW_UNBOND;
    typedef struct stDotStakingNominate {
        JUB_CHAR_PTR nominators[50];
        JUB_INT32 nominators_n;
    } JUB_STAKING_CALL_NOMINATE;
    typedef struct stDotStakingPayoutStakers {
        JUB_CHAR_PTR validator_stash;
        JUB_INT32 era;
    } JUB_STAKING_CALL_PAYOUT_STAKERS;

    JUB_ENUM_DOT_STAKING_TYPE type;
    union {
//        JUB_STAKING_CALL_BOND bond;
//        JUB_STAKING_CALL_BOND_AND_NOMINATE bondAndNominate;
        JUB_STAKING_CALL_BOND_EXTRA extra;
        JUB_STAKING_CALL_UNBOND unbond;
        JUB_STAKING_CALL_WITHDRAW_UNBOND withdrawUnbonded;
        JUB_STAKING_CALL_NOMINATE nominate;
        JUB_STAKING_CALL_PAYOUT_STAKERS payoutStakers;
    };
} JUB_STAKING_CALL_DOT;

typedef struct stTxDOT {
    JUB_CHAR_PTR genesisHash;
    JUB_CHAR_PTR blockHash;
    JUB_UINT64 nonce;
    JUB_UINT32 specVersion;
    JUB_UINT32 transaction_version;
    JUB_UINT64 blockNumber;
    JUB_UINT64 eraPeriod;   // [Optional]
    JUB_CHAR_PTR tip;       // [Optional]

    JUB_ENUM_DOT_EXTRINSIC_TYPE type;
    union {
        JUB_BALANCES_CALL_DOT balances;
        JUB_STAKING_CALL_DOT staking;
    };
} JUB_TX_DOT;
/*****************************************************************************
 * @function name : JUB_CreateContextDOT
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextDOT(IN CONTEXT_CONFIG_DOT cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressDOT
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressDOT(IN JUB_UINT16 contextID,
                         IN JUB_CHAR_PTR path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_GetHDNodeDOT
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeDOT(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN JUB_CHAR_PTR path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeDOT
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeDOT(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_CheckAddressDOT
 * @in  param : contextID - context ID
 *            : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressDOT(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address);

/*****************************************************************************
 * @function name : JUB_SetMyAddressDOT
 * @in  param : contextID - context ID
 *          : path
 * @out param : address
 * @last change :
 *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_SetMyAddressDOT(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
//
/*****************************************************************************
 * @function name : JUB_SignTransactionDOT
 * @in  param : contextID - context ID
 *            : tx - JUB_TX_DOT
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionDOT(IN JUB_UINT16 contextID,
                              IN JUB_CHAR_PTR path,
                              IN JUB_TX_DOT tx,
                              OUT JUB_CHAR_PTR_PTR raw);

/*****************************************************************************
 * @function name : JUB_IsValidAddressDOT
 * @in  param : address
 * @out param : none
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IsValidAddressDOT(IN JUB_CHAR_CPTR address);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_DOT_h */
