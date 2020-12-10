# Device operation related module
Function list:
* [JUB_GetDeviceType](#JUB_GetDeviceType)
* [JUB_GetDeviceInfo](#JUB_GetDeviceInfo)
* [JUB_GetDeviceCert](#JUB_GetDeviceCert)
* [JUB_SendOneApdu](#JUB_SendOneApdu)
* [JUB_IsInitialize](#JUB_IsInitialize)
* [JUB_IsBootLoader](#JUB_IsBootLoader)
* [JUB_EnumApplets](#JUB_EnumApplets)
* [JUB_EnumSupportCoins](#JUB_EnumSupportCoins)
* [JUB_GetAppletVersion](#JUB_GetAppletVersion)
* **Common module**
    * [JUB_ClearContext](#JUB_ClearContext)
    * [JUB_ShowVirtualPwd](#JUB_ShowVirtualPwd)
    * [JUB_CancelVirtualPwd](#JUB_CancelVirtualPwd)
    * [JUB_VerifyPIN](#JUB_VerifyPIN)
    * [JUB_FreeMemory](#JUB_FreeMemory)
    * [JUB_GetVersion](#JUB_GetVersion)

* * *

In [JUB_SDK_DEV.h](../include/JUB_SDK_DEV.h).
### JUB_GetDeviceType
* **Function:** Get device type.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
* **OUT param:**
commode - Communication mode, including 
            JUB_ENUM_COMMODE::SWI
            JUB_ENUM_COMMODE::HID
            JUB_ENUM_COMMODE::BLE
            JUB_ENUM_COMMODE::NFC
deviceClass - device class, including
            JUB_ENUM_DEVICE::VD (Virtual hardware)
            JUB_ENUM_DEVICE::BLADE (JuBiter Blade)
            JUB_ENUM_DEVICE::BIO (JuBiter Bio)
            JUB_ENUM_DEVICE::LITE (JuBiter Lite)
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetDeviceType(
    IN JUB_UINT16 deviceID,
    OUT JUB_ENUM_COMMODE_PTR commode,
    OUT JUB_ENUM_DEVICE_PTR deviceClass
);
```

### JUB_GetDeviceInfo
* **Function:** Get Device Information.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
* **OUT param:**
info - JUB_DEVICE_INFO
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetDeviceInfo(
    IN JUB_UINT16 deviceID,
    OUT JUB_DEVICE_INFO_PTR info
);
```

### JUB_GetDeviceCert
* **Function:** Read device's certificate.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
* **OUT param:**
cert - Device's certificate.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetDeviceCert(
    IN JUB_UINT16 deviceID,
    OUT JUB_CHAR_PTR_PTR cert
);
```

### JUB_SendOneApdu
* **Function:** Send one APDU to device.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
apdu - APDU command
* **OUT param:**
response - APDU command's response.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_SendOneApdu(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR apdu,
    OUT JUB_CHAR_PTR_PTR response
);
```

### JUB_IsInitialize
* **Function:** Check if device is initialized.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_ENUM_BOOL JUB_IsInitialize(
    IN JUB_UINT16 deviceID
);
```

### JUB_IsBootLoader
* **Function:** Check if device is in boot loader status.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
* **OUT param:**
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_ENUM_BOOL JUB_IsBootLoader(
    IN JUB_UINT16 deviceID
);
```

### JUB_EnumApplets
* **Function:** Enumerates has been installed applets on device.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
* **OUT param:**
appList - applet ID list in blank separated.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_EnumApplets(
    IN JUB_UINT16 deviceID,
    OUT JUB_CHAR_PTR_PTR appList
);
```

### JUB_EnumSupportCoins
* **Function:** Enumerates coins which has been supported by device.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
* **OUT param:**
coinsList - coin list in blank separated.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_EnumSupportCoins(
    IN JUB_UINT16 deviceID,
    OUT JUB_CHAR_PTR_PTR coinsList
);
```

### JUB_GetAppletVersion
* **Function:** Get version of the specified applet.
* **IN param:**
deviceID - device ID from JUB_connectDevice/JUB_connectNFCDevice.
appID - applet ID from JUB_EnumApplets.
* **OUT param:**
version - The version of the specified applet.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GetAppletVersion(
    IN JUB_UINT16 deviceID,
    IN JUB_CHAR_CPTR appID,
    OUT JUB_CHAR_PTR_PTR version
);
```

* * *
## Common module
In [JUB_SDK_COMM.h](../include/JUB_SDK_COMM.h).

### JUB_ClearContext
* **Function:** Clear context.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ClearContext(
    IN JUB_UINT16 contextID
);
```

### JUB_ShowVirtualPwd
* **Function:** Show virtual PIN keyboard on device.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
* **OUT param:**
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ShowVirtualPwd(
    IN JUB_UINT16 contextID
);
```

### JUB_CancelVirtualPwd
* **Function:** Cancel virtual PIN keyboard on device.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_CancelVirtualPwd(
    IN JUB_UINT16 contextID
);
```

### JUB_VerifyPIN
* **Function:** Verify device's user PIN.
* **IN param:**
contextID - context ID from JUB_CreateContextXXX.
pinMix - User's PIN.
* **OUT param:**
pretry - if OK, retry is meaningless value.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_VerifyPIN(
    IN JUB_UINT16 contextID,
    IN JUB_CHAR_CPTR pinMix,
    OUT JUB_ULONG_PTR pretry
);
```

### JUB_FreeMemory
* **Function:** Free memory.
* **IN param:**
memPtr - memory pointer need to be freed.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_FreeMemory(
    IN JUB_CHAR_CPTR memPtr
);
```

### JUB_GetVersion
* **Function:** Get SDK's version.
* **IN param:** none.
* **OUT param:** SDK ersion.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_CHAR_PTR JUB_GetVersion(void);
```
