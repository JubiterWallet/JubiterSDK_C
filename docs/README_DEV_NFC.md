# Lite related module
In [JUB_SDK_DEV_NFC.h](../include/JUB_SDK_DEV_NFC.h).

Function list:
* [JUB_initNFCDevice](#JUB_initNFCDevice)
* [JUB_connectNFCDevice](#JUB_connectNFCDevice)
* [JUB_disconnectNFCDevice](#JUB_disconnectNFCDevice)
* [JUB_isDeviceNFCConnect](#JUB_isDeviceNFCConnect)
* [JUB_setNFCAlertMessage](#JUB_setNFCAlertMessage)
* [JUB_SetLabel](#JUB_SetLabel)
* [JUB_Reset](#JUB_Reset)
* [JUB_GenerateSeed](#JUB_GenerateSeed)
* [JUB_ImportMnemonic](#JUB_ImportMnemonic)
* [JUB_ExportMnemonic](#JUB_ExportMnemonic)
* [JUB_GetRootKeyStatus](#JUB_GetRootKeyStatus)
* [JUB_ChangePIN](#JUB_ChangePIN)

### JUB_initNFCDevice
* **Function:** Initialize lite device.
* **IN param:**
param - NFC_DEVICE_INIT_PARAM.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_initNFCDevice(
    IN NFC_DEVICE_INIT_PARAM param
);
```

### JUB_connectNFCDevice
* **Function:** Connect NFC Device.
* **IN param:**
bUUID - nfc device UUID
* **OUT param:** none.
pDeviceID - NFC device connect handle.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_connectNFCDevice(
    JUB_BYTE_PTR bUUID,
    JUB_UINT16* pDeviceID
);
```

### JUB_disconnectNFCDevice
* **Function:** Disconnect NFC device.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_disconnectNFCDevice(
    JUB_UINT16 deviceID
);
```

### JUB_isDeviceNFCConnect
* **Function:** Check if NFC is connected.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_isDeviceNFCConnect(
    JUB_UINT16 deviceID
);
```

### JUB_setNFCAlertMessage
* **Function:** Set NFC alert message.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
msg - message showing in NFC alert view.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_setNFCAlertMessage(
    JUB_UINT16 deviceID,
    JUB_CHAR_CPTR msg
);
```

### JUB_SetLabel
* **Function:** Set label.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
label - device label.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SetLabel(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR label
);
```

### JUB_Reset
* **Function:** Reset NFC.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** Erase mnemonic and user's PIN. Need to set user's PIN via JUB_ChangePIN.
```
JUB_RV JUB_Reset(
    IN JUB_UINT16 deviceID
);
```

### JUB_GenerateSeed
* **Function:** Generate seed
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
pinMix - User's PIN
curve - JUB_ENUM_CURVES::SECP256K1
* **OUT param:**
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GenerateSeed(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR pinMix,
    IN JUB_ENUM_CURVES curve
);
```

### JUB_ImportMnemonic
* **Function:** Import mnemonic.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
pinMix - User's PIN
mnemonic - mnemonic, the length is 12, 18 or 24.
* **OUT param:**
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ImportMnemonic(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR pinMix,
    IN JUB_CHAR_CPTR mnemonic
);
```

### JUB_ExportMnemonic
* **Function:** Export mnemonic.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
pinMix - User's PIN
* **OUT param:**
mnemonic - exported mnemonic.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ExportMnemonic(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR pinMix,
    OUT JUB_CHAR_PTR_PTR mnemonic
);
```

### JUB_GetRootKeyStatus
* **Function:** Get root key's status.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
* **OUT param:**
status - 
        JUB_ENUM_NFC_ROOT_KEY_STATUS::HAS_PIN,
        JUB_ENUM_NFC_ROOT_KEY_STATUS::RESETTED,
        JUB_ENUM_NFC_ROOT_KEY_STATUS::HAS_ROOT_KEY

* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetRootKeyStatus(
    IN JUB_UINT16 deviceID,
    OUT JUB_ENUM_NFC_ROOT_KEY_STATUS_PTR status
);
```

### JUB_ChangePIN
* **Function:** Change PIN.
* **IN param:**
deviceID - device ID from JUB_connectNFCDevice.
pinMix - old User's PIN
pinNew - new User's PIN
* **OUT param:**
pretry - if OK, retry is meaningless value.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ChangePIN(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR pinMix,
    IN JUB_CHAR_CPTR pinNew,
    OUT JUB_ULONG_PTR pretry
);
```