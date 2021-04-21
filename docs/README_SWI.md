# Software interface related module
In [JUB_SDK_DEV_SWI.h](../include/JUB_SDK_DEV_SWI.h), virtual the software implementation interface as a device whose device name is randomly generated.

Function list:
* [JUB_ConnetDeviceSWI](#JUB_ConnetDeviceSWI)
* [JUB_DisconnetDeviceSWI](#JUB_DisconnetDeviceSWI)
* [JUB_ImportMasterPrivateKey](#JUB_ImportMasterPrivateKey)
* [JUB_ImportMnemonic](#JUB_ImportMnemonic)

### JUB_ConnetDeviceSWI
* **Function:** Connect software interface.
* **IN param:** none
* **OUT param:**
pDeviceID - software interface connect handle.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ConnetDeviceSWI(
    JUB_UINT16_PTR pDeviceID
);
```

### JUB_DisconnetDeviceSWI
* **Function:** Disconnect software interface.
* **IN param:**
deviceID - device ID from JUB_ConnetDeviceSWI.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_DisconnetDeviceSWI(
    IN JUB_UINT16 deviceID
);
```

### JUB_ImportMasterPrivateKey
* **Function:** Import master xprv or xpub to the software interface.
* **IN param:**
deviceID - device ID.
curve - JUB_ENUM_CURVES::SECP256K1/NIST256P1/ED25519
masterPrivateKey - master xprv or xpub.
* **OUT param:**
prikeyInXPRV - private key in xprv format.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ImportMasterPrivateKey(
    IN JUB_UINT16 deviceID,
    IN JUB_ENUM_CURVES curve,
    IN JUB_CHAR_CPTR masterPrivateKey
);
```

### JUB_ImportMnemonic
* **Function:** Import mnemonic to the software interface
* **IN param:**
deviceID - device ID.
passphrase - passphrase.
mnemonic - mnemonic.
* **OUT param:**
contextID - context ID for the specific coin.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ImportMnemonic(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR passphrase,
    IN JUB_CHAR_CPTR mnemonic
);
```
