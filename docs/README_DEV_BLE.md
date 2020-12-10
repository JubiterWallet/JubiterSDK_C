# Blade related module
In [JUB_SDK_DEV_BLE.h](../include/JUB_SDK_DEV_BLE.h) & [JUB_SDK_DEV_BIO.h](../include/JUB_SDK_DEV_BIO.h).

Function list:
* [JUB_initDevice](#JUB_initDevice)
* [JUB_enumDevices](#JUB_enumDevices)
* [JUB_stopEnumDevices](#JUB_stopEnumDevices)
* [JUB_connectDevice](#JUB_connectDevice)
* [JUB_cancelConnect](#JUB_cancelConnect)
* [JUB_disconnectDevice](#JUB_disconnectDevice)
* [JUB_isDeviceConnect](#JUB_isDeviceConnect)
* [JUB_QueryBattery](#JUB_QueryBattery)
* **Bio related module**
    * [JUB_IdentityVerify](#JUB_IdentityVerify)
    * [JUB_IdentityVerifyPIN](#JUB_IdentityVerifyPIN)
    * [JUB_IdentityShowNineGrids](#JUB_IdentityShowNineGrids)
    * [JUB_IdentityCancelNineGrids](#JUB_IdentityCancelNineGrids)
    * [JUB_EnrollFingerprint](#JUB_EnrollFingerprint)
    * [JUB_EnumFingerprint](#JUB_EnumFingerprint)
    * [JUB_EraseFingerprint](#JUB_EraseFingerprint)
    * [JUB_DeleteFingerprint](#JUB_DeleteFingerprint)
    * [JUB_VerifyFingerprint](#JUB_VerifyFingerprint)

### JUB_connectDevice
* **Function:** Connect Blade.
* **IN param:** 
devName - BLE device Name
bBLEUUID - BLE device UUID
connectType - BLE connect type
timeout - BLE time-out
* **OUT param:** 
pDeviceID - ble device connect handle.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_connectDevice(
    IN JUB_BYTE_PTR devName,
    IN JUB_BYTE_PTR bBLEUUID,
    IN JUB_UINT32 connectType,
    OUT JUB_UINT16* pDeviceID,
    IN JUB_UINT32 timeout
);
```

### JUB_cancelConnect
* **Function:** Cancel to connect Blade.
* **IN param:** 
devName - device Name
bBLEUUID - BLE UUID
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_cancelConnect(
    IN JUB_BYTE_PTR devName,
    IN JUB_BYTE_PTR bBLEUUID
);
```

### JUB_disconnectDevice
* **Function:** Disconnect Blade.
* **IN param:** 
deviceID - device ID from JUB_connectDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_disconnectDevice(
    IN JUB_UINT16 deviceID
);
```

### JUB_isDeviceConnect
* **Function:** Identity verification.
* **IN param:** 
deviceID - device ID from JUB_connectDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_isDeviceConnect(
    IN JUB_UINT16 deviceID
);
```

* * *
# Bio related module
### JUB_IdentityVerify
* **Function:** Identity verification.
* **IN param:** 
deviceID - device ID from JUB_connectDevice.
mode - the mode for verify identity, the following values are valid:
    * JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_DEVICE
    * JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_FPGT
* **OUT param:**
retry - if OK, retry is meaningless value.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_IdentityVerify(
    IN JUB_UINT16 deviceID,
    IN JUB_ENUM_IDENTITY_VERIFY_MODE mode,
    OUT JUB_ULONG_PTR pretry
);
```

### JUB_IdentityVerifyPIN
* **Function:** Identity for PIN verification.
* **IN param:**
deviceID - device ID from JUB_connectDevice.
mode - the mode for verify identity, the following values are valid: JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_9GRIDS, JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_APDU
* **OUT param:**
retry - if OK, retry is meaningless value.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_IdentityVerifyPIN(
    IN JUB_UINT16 deviceID,
    IN JUB_ENUM_IDENTITY_VERIFY_MODE mode,
    IN JUB_CHAR_CPTR pinMix,
    OUT JUB_ULONG_PTR pretry
);
```

### JUB_IdentityShowNineGrids
* **Function:** Show nine grides for Identity verification.
* **IN param:**
deviceID - device ID from JUB_connectDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_IdentityShowNineGrids(
    IN JUB_UINT16 deviceID
);
```

### JUB_IdentityCancelNineGrids
* **Function:** Cancel nine grides for Identity verification.
* **IN param:**
deviceID - device ID from JUB_connectDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_IdentityCancelNineGrids(
    IN JUB_UINT16 deviceID
);
```

### JUB_EnrollFingerprint
* **Function:** Enroll fingerprint.
* **IN param:**
deviceID - device ID from JUB_connectDevice.
fpTimeout - timeout(8~60s) for fingerprint operation.
fgptIndex - fingerprint Index, start from 0, end at ptimes.
* **OUT param:**
ptimes - the number of times it takes to enroll one fingerprint correctly.
fgptID - fingerprint ID.
* **Return:** JUB_OK or !JUB_OK for error.
* **Note:** To successfully enter one fingerprint, fingerprints must be taken several times. During the collection, if the current operation fails, the collection can be retried from current index or restarted the whole collection. The fingerprint fragments will be removed after the device is powered off.
```
JUB_RV JUB_EnrollFingerprint(
    IN JUB_UINT16 deviceID,
    IN JUB_UINT16 fpTimeout,
    INOUT JUB_BYTE_PTR fgptIndex,
    OUT JUB_ULONG_PTR ptimes,
    OUT JUB_BYTE_PTR fgptID
);
```

### JUB_EnumFingerprint
* **Function:** Enumerates fingerprints.
* **IN param:**
deviceID - device ID from JUB_connectDevice.
* **OUT param:** none.
fgptList - fingerprint ID list in blank separated.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_EnumFingerprint(
    IN JUB_UINT16 deviceID,
    OUT JUB_CHAR_PTR_PTR fgptList
);
```

### JUB_DeleteFingerprint
* **Function:** Delete a fingerprint.
* **IN param:**
deviceID - device ID from JUB_connectDevice.
fpTimeout - timeout(8~60s) for fingerprint operation.
fgptID - fingerprint ID.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_DeleteFingerprint(
    IN JUB_UINT16 deviceID,
    IN JUB_UINT16 fpTimeout,
    IN JUB_BYTE fgptID
);
```

### JUB_EraseFingerprint
* **Function:** Erase all fingerprints.
* **IN param:**
deviceID - device ID from JUB_connectDevice.
fpTimeout - timeout(8~60s) for fingerprint operation.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_EraseFingerprint(
    IN JUB_UINT16 deviceID,
    IN JUB_UINT16 fpTimeout
);
```

### JUB_VerifyFingerprint
* **Function:** Fingerprint verification.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
* **OUT param:**
pretry - if OK, retry is meaningless value.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_VerifyFingerprint(
    IN JUB_UINT16 contextID,
    OUT JUB_ULONG_PTR pretry
);
```
