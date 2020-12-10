# Software wallet module
In [JUB_CORE.h](../include/JUB_CORE.h).

Function list:
* [JUB_GenerateMnemonic_soft](#JUB_GenerateMnemonic_soft)
* [JUB_CheckMnemonic](#JUB_CheckMnemonic)
* [JUB_GenerateSeed_soft](#JUB_GenerateSeed_soft)
* [JUB_SeedToMasterPrivateKey_soft](#JUB_SeedToMasterPrivateKey_soft)
* **BTC series related**
    * [JUB_CreateContextBTC_soft](#JUB_CreateContextXXX_soft)
* **ETH series related**
    * [JUB_CreateContextETH_soft](#JUB_CreateContextXXX_soft)
* **EOS related**
    * [JUB_CreateContextEOS_soft](#JUB_CreateContextXXX_soft)
* **XRP related**
    * [JUB_CreateContextXRP_soft](#JUB_CreateContextXXX_soft)
* **TRX related**
    * [JUB_CreateContextTRX_soft](#JUB_CreateContextXXX_soft)

### JUB_GenerateMnemonic_soft
* **Function:** Generate mnemonic for software wallet.
* **IN param:**
strength - mnemonic strength:
    JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128(12 words),
    JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH192(18 words),
    JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH256(24 words)
* **OUT param:**
mnemonic - generated mnemonic.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GenerateMnemonic_soft(
    IN JUB_ENUM_MNEMONIC_STRENGTH strength,
    OUT JUB_CHAR_PTR_PTR mnemonic
);
```

### JUB_CheckMnemonic
* **Function:** Check if the mnemonic is valid.
* **IN param:**
mnemonic - mnemonic.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_CheckMnemonic(
    IN JUB_CHAR_CPTR mnemonic
);
```

### JUB_GenerateSeed_soft
* **Function:** Generate seed from mnemonic.
* **IN param:**
mnemonic - mnemonic
passphrase - passphrase
progress_callback - can be null.
* **OUT param:**
seed - generated seed from mnemonic.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GenerateSeed_soft(
    IN JUB_CHAR_CPTR mnemonic,
    IN JUB_CHAR_CPTR passphrase,
    OUT JUB_BYTE seed[64],
    void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)
);
```

### JUB_SeedToMasterPrivateKey_soft
* **Function:** Get master private key from a seed.
* **IN param:**
seed - seed.
seed_len - the length of seed.
curve - JUB_ENUM_CURVES::SECP256K1/NIST256P1/ED25519
* **OUT param:**
prikeyInXPRV - private key in xprv format.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SeedToMasterPrivateKey_soft(
    IN JUB_BYTE_CPTR seed,
    IN JUB_UINT16 seed_len,
    IN JUB_ENUM_CURVES curve,
    OUT JUB_CHAR_PTR_PTR prikeyInXPRV
);
```

### JUB_CreateContextXXX_soft
* **Function:** JUB_CreateContextBTC_soft
* **IN param:**
cfg - CONTEXT_CONFIG_BTC/ETH/EOS/XRP/TRX
XPRVorXPUB - xpub or xprv.
* **OUT param:**
contextID - context ID for the specific coin.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_CreateContextBTC_soft(
    IN CONTEXT_CONFIG_BTC cfg,
    IN JUB_CHAR_CPTR XPRVorXPUB,
    OUT JUB_UINT16* contextID);
```
