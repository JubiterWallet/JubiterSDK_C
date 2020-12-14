# Coin related module
In [JUB_SDK_BTC.h](../include/JUB_SDK_BTC.h), [JUB_SDK_ETH.h](../include/JUB_SDK_ETH.h), [JUB_SDK_EOS.h](../include/JUB_SDK_EOS.h), [JUB_SDK_XRP.h](../include/JUB_SDK_XRP.h), [JUB_SDK_TRX.h](../include/JUB_SDK_TRX.h), [JUB_SDK_FIL.h](../include/JUB_SDK_FIL.h).

Function list:
* [JUB_CreateContext](#JUB_CreateContextXXX)BTC/ETH/EOS/XRP/TRX/FIL
* [JUB_GetMainHDNode](#JUB_GetMainHDNodeXXX)BTC/ETH/EOS/XRP/TRX/FIL
* [JUB_GetHDNode](#JUB_GetHDNodeXXX)BTC/ETH/EOS/XRP/TRX/FIL
* [JUB_GetAddress](#JUB_GetAddressXXX)BTC/ETH/EOS/XRP/TRX/FIL
* [JUB_SetMyAddress](#JUB_SetMyAddressXXX)BTC/ETH/EOS/XRP/TRX/FIL
* **BTC series related module**
    * [JUB_CheckAddressBTC](#JUB_CheckAddressBTC)
    * [JUB_SetUnitBTC](#JUB_SetUnitBTC)
    * [JUB_SignTransactionBTC](#JUB_SignTransactionBTC)
    * [JUB_BuildUSDTOutputs](#JUB_BuildUSDTOutputs)
    * [JUB_BuildQRC20Outputs](#JUB_BuildQRC20Outputs)
* **ETH series related module**
    * [JUB_SignTransactionETH](#JUB_SignTransactionETH)
    * [JUB_BuildERC20AbiETH](#JUB_BuildERC20AbiETH)
* **FIL related module**
    * [JUB_SignTransactionFIL](#JUB_SignTransactionFIL)
* **EOS related module**
    * [JUB_SignTransactionEOS](#JUB_SignTransactionEOS)
    * [JUB_BuildActionEOS](#JUB_BuildActionEOS)
    * [JUB_CalculateMemoHash](#JUB_CalculateMemoHash)
* **XRP related module**
    * [JUB_SignTransactionXRP](#JUB_SignTransactionXRP)
* **TRX related module**
    * [JUB_PackContractTRX](#JUB_PackContractTRX)
    * [JUB_SignTransactionTRX](#JUB_SignTransactionTRX)

### JUB_CreateContextXXX
* **Function:** Create context for the specified coin.
* **IN param:**
    cfg - CONTEXT_CONFIG_BTC/ETH/EOS/XRP
    deviceID - device ID from JUB_conneceDevice/JUB_connectNFCDevice.
* **OUT param:**
contextID - context ID for the specified coin.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_CreateContextBTC(
    IN CONTEXT_CONFIG_BTC cfg,
    IN JUB_UINT16 deviceID,
    OUT JUB_UINT16* contextID
);
```

### JUB_GetMainHDNodeXXX
* **Function:** Get main hard derivative node for the specified coin.
* **IN param:**
contextID - context ID for the specified coin.
* **OUT param:**
xpub - xpub.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetMainHDNodeBTC(
    IN JUB_UINT16 contextID,
    OUT JUB_CHAR_PTR_PTR xpub
);
```

### JUB_GetHDNodeXXX
* **Function:** Get hard derivative node for the specified coin.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
path - path in BIP44.
* **OUT param:**
xpub - xpub.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetHDNodeBTC(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    OUT JUB_CHAR_PTR_PTR xpub
);
```

### JUB_GetAddressXXX
* **Function:** Get address for the specified coin in the specified path.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
path - path in BIP44.
bShow - if show in the device.
* **OUT param:**
address - address in the specified path.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetAddressBTC(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    IN JUB_ENUM_BOOL bShow,
    OUT JUB_CHAR_PTR_PTR address
);
```

### JUB_SetMyAddressXXX
* **Function:** Set the default address for the specified coin.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
path - path in BIP44.
address - address in the specified path.
* **OUT param:**
address - address in the specified path.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SetMyAddressBTC(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    OUT JUB_CHAR_PTR_PTR address
);
```

## BTC series related module
### JUB_CheckAddressBTC
* **Function:** Check address if it is valid for the specified coin.
* **IN param:**
contextID - context ID from JUB_CreateContextBTC.
address - address in the specified path.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_CheckAddressBTC(
    IN JUB_UINT16 contextID,
    IN JUB_CHAR_CPTR address
);
```

### JUB_SetUnitBTC
* **Function:** Set unit for BTC.
* **IN param:**
contextID - context ID from JUB_CreateContextBTC.
unit - JUB_ENUM_BTC_UNIT_TYPE::BTC/cBTC/mBTC/uBTC/Satoshi.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** Will affect the amount of units displayed on the device.
```
JUB_RV JUB_SetUnitBTC(
    IN JUB_UINT16 contextID,
    IN JUB_ENUM_BTC_UNIT_TYPE unit
);
```

### JUB_SignTransactionBTC
* **Function:** Sign transaction for BTC.
* **IN param:**
contextID - context ID from JUB_CreateContextBTC.
version - the version of BTC tx.
inputs - the input array of BTC tx.
iCount - the count of the input array.
outputs - the output array of BTC tx.
oCount - the count of the output array.
lockTime - the lock time for BTC tx.
* **OUT param:**
raw - signed BTC tx in binary byte stream
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SignTransactionBTC(
    IN JUB_UINT16 contextID,
    IN JUB_UINT32 version,
    IN INPUT_BTC inputs[],
    IN JUB_UINT16 iCount,
    IN OUTPUT_BTC outputs[],
    IN JUB_UINT16 oCount,
    IN JUB_UINT32 lockTime,
    OUT JUB_CHAR_PTR_PTR raw
);
```

### JUB_BuildUSDTOutputs
* **Function:** Build USDT outputs.
* **IN param:**
contextID - context ID from JUB_CreateContextBTC.
USDTTo - To address for USDT.
amount - amount for USDT.
* **OUT param:**
outputs - output array for USDT in BTC output way.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** Used in conjunction with JUB_SignTransactionBTC for USDT transactions.
```
JUB_RV JUB_BuildUSDTOutputs(
    IN JUB_UINT16 contextID,
    IN JUB_CHAR_CPTR USDTTo,
    IN JUB_UINT64 amount,
    OUT OUTPUT_BTC outputs[2]
);
```

### JUB_BuildQRC20Outputs
* **Function:** Build outputs in QRC-20.
* **IN param:**
contextID - context ID from JUB_CreateContextBTC.
contractAddress - contract address of QRC-20.
decimal - decimal for QRC-20 token.
symbol - symbol for QRC-20 token.
gasLimit - gas limit.
gasPrice - gas price.
to - to address for QRC-20 token tx.
value - amount for QRC-20 token tx.
* **OUT param:**
outputs[1] - QRC-20 token output in BTC way.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** Used in conjunction with JUB_SignTransactionBTC for QTUM transactions.
```
JUB_RV JUB_BuildQRC20Outputs(
    IN JUB_UINT16 contextID,
    IN JUB_CHAR_CPTR contractAddress,
    IN JUB_UINT8 decimal,
    IN JUB_CHAR_CPTR symbol,
    IN JUB_UINT64 gasLimit,
    IN JUB_UINT64 gasPrice,
    IN JUB_CHAR_CPTR to,
    IN JUB_CHAR_CPTR value,
    OUT OUTPUT_BTC outputs[1]
);
```

## ETH series related module
### JUB_SignTransactionETH
* **Function:** Sign ETH transaction.
* **IN param:**
contextID - context ID from JUB_CreateContextETH.
path - path in BIP44.
nonce - nonce.
gasLimit - gas limit.
gasPriceInWei - gas price in Wei.
to - to address.
valueInWei - amount in Wei of ETH tx.
input - empyt string for ETH tx or ERC-20 ABI from JUB_BuildERC20AbiETH.
* **OUT param:**
raw - signed ETH/ETH token tx.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SignTransactionETH(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    IN JUB_UINT32 nonce,
    IN JUB_UINT32 gasLimit,
    IN JUB_CHAR_CPTR gasPriceInWei,
    IN JUB_CHAR_CPTR to,
    IN JUB_CHAR_CPTR valueInWei,
    IN JUB_CHAR_CPTR input,
    OUT JUB_CHAR_PTR_PTR raw
);
```

### JUB_BuildERC20AbiETH
* **Function:** Build ETH ERC-20 ABI.
* **IN param:**
contextID - context ID from JUB_CreateContextETH.
tokenName - ERC-20 token name.
unitDP - unit.
contractAddress - contract address for ERC-20.
tokenTo - ERC-20 token to address.
tokenValue - ERC-20 token amount.
* **OUT param:**
abi - ERC-20 ABI.

**Return:** JUB_OK or !JUB_OK for error.
**Note:** Used in conjunction with JUB_SignTransactionETH for ERC-20 token transactions.
```
JUB_RV JUB_BuildERC20AbiETH(
    IN JUB_UINT16 contextID,
    IN JUB_CHAR_CPTR tokenName,
    IN JUB_UINT16 unitDP,
    IN JUB_CHAR_CPTR contractAddress,
    IN JUB_CHAR_CPTR tokenTo,
    IN JUB_CHAR_CPTR tokenValue,
    OUT JUB_CHAR_PTR_PTR abi);
```

## FIL related module
### JUB_SignTransactionFIL
* **Function:** Sign FIL transaction.
* **IN param:**
contextID - context ID from JUB_CreateContextFIL.
path - path in BIP44.
nonce - nonce.
gasLimit - gas limit.
gasPriceInAtto - gas price in attoFIL.
to - to address.
valueInAtto - amount in attoFIL of FIL tx.
input - empyt string for FIL tx or...
* **OUT param:**
raw - signed FIL/FIL token tx.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SignTransactionFIL(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    IN JUB_UINT64 nonce,
    IN JUB_UINT64 gasLimit,
    IN JUB_CHAR_CPTR gasPriceInAtto,
    IN JUB_CHAR_CPTR to,
    IN JUB_CHAR_CPTR valueInAtto,
    IN JUB_CHAR_CPTR input,
    OUT JUB_CHAR_PTR_PTR raw
);
```

## EOS related module
### JUB_SignTransactionEOS
* **Function:** Sign EOS transaction.
* **IN param:**
contextID - context ID from JUB_CreateContextEOS.
path - path in BIP44.
chainID - chain ID.
expiration - tx expiration.
referenceBlockId - tx's reference block ID.
referenceBlockTime - tx's reference block time.
actionsInJSON - action in JSON from JUB_BuildActionEOS.
* **OUT param:**
rawInJSON - signed raw in JSON.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** Used in conjunction with JUB_BuildActionEOS for EOS transactions.
```
JUB_RV JUB_SignTransactionEOS(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    IN JUB_CHAR_CPTR chainID,
    IN JUB_CHAR_CPTR expiration,
    IN JUB_CHAR_CPTR referenceBlockId,
    IN JUB_CHAR_CPTR referenceBlockTime,
    IN JUB_CHAR_CPTR actionsInJSON,
    OUT JUB_CHAR_PTR_PTR rawInJSON
);
```

### JUB_BuildActionEOS
* **Function:** Build EOS specific action.
* **IN param:**
contextID - context ID from JUB_CreateContextEOS.
actions - action array.
actionCount - the count of action array.
* **OUT param:**
actionsInJSON - EOS actions in JSON.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** Used in conjunction with JUB_SignTransactionEOS for EOS transactions.
```
JUB_RV JUB_BuildActionEOS(
    IN JUB_UINT16 contextID,
    IN JUB_ACTION_EOS_PTR actions,
    IN JUB_UINT16 actionCount,
    OUT JUB_CHAR_PTR_PTR actionsInJSON
);
```

### JUB_CalculateMemoHash
* **Function:** Calculate memo hash for EOS transfer tx.
* **IN param:**
memo - memo on EOS transfer tx.
* **OUT param:**
memoHash - memo hash.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** Used in conjunction with JUB_SignTransactionEOS for EOS transfer transactions. The memo hash of EOS transfer tx will show on JuBiter device. This value can be used at app for valid transfer tx.
```
JUB_RV JUB_CalculateMemoHash(
    IN JUB_CHAR_CPTR memo,
    OUT JUB_CHAR_PTR_PTR memoHash
);
```

## XRP related module
### JUB_SignTransactionXRP
* **Function:** Sign XRP transaction.
* **IN param:**
contextID - context ID from JUB_CreateContextXRP.
path - path in BIP44.
tx - JUB_TX_XRP.
* **OUT param:**
raw - signed XRP tx.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SignTransactionXRP(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    IN JUB_TX_XRP tx,
    OUT JUB_CHAR_PTR_PTR raw
);
```

## TRX related module
### JUB_SignTransactionTRX
* **Function:** Sign TRX transaction.
* **IN param:**
contextID - context ID from JUB_CreateContextTRX.
path - path in BIP44.
packedContractInPb - packed contract in protobuf
* **OUT param:**
rawInJSON - signed TRX tx in JSON.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SignTransactionTRX(
    IN JUB_UINT16 contextID,
    IN BIP44_Path path,
    IN JUB_CHAR_CPTR packedContractInPb,
    OUT JUB_CHAR_PTR_PTR rawInJSON
);
```

### JUB_PackContractTRX
* **Function:** Pack contract in protobuf.
* **IN param:**
contextID - context ID from JUB_CreateContextTRX.
tx - JUB_TX_TRX.
* **OUT param:**
packedContractInPB - packed contract in protobuf
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_PackContractTRX(
    IN JUB_UINT16 contextID,
    IN JUB_TX_TRX tx,
    OUT JUB_CHAR_PTR_PTR packedContractInPB
);
```
