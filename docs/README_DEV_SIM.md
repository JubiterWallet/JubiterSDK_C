# SIM module
In [JUB_SDK_DEV_SIM.h](../include/JUB_SDK_DEV_SIM.h).

Function list:
* [JUB_ListDeviceSIM](#JUB_ListDeviceSIM)
* [JUB_ListLiteSIM](#JUB_ListLiteSIM)
* [JUB_ConnetDeviceSIM](#JUB_ConnetDeviceSIM)
* [JUB_DisconnetDeviceSIM](#JUB_DisconnetDeviceSIM)

### JUB_ListDeviceSIM
* **Function:** Enumerates virtual device(simulator for applet), only for JuBiter Blade/Bio in PCSC mode.
* **IN param:**
ip - server's ip infomation file name.
prod - device communication type.
* **OUT param:**
deviceIDs[MAX_DEVICE] - deviceID list.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ListLiteSIM(
    IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, IN LITE_DEVICE_INIT_PARAM param,
    OUT JUB_UINT16 deviceIDs[MAX_DEVICE]
);
```

### JUB_ListLiteSIM
* **Function:** Enumerates virtual device(simulator for applet), only for JuBiter Lite in PCSC mode.
* **IN param:**
ip - server's ip infomation file name.
prod - device communication type.
param - additional parameter for Lite.
* **OUT param:**
deviceIDs[MAX_DEVICE] - deviceID list.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ListLiteSIM(
    IN JUB_CHAR_CPTR ip, IN JUB_ENUM_DEVICE prod, IN LITE_DEVICE_INIT_PARAM param,
    OUT JUB_UINT16 deviceIDs[MAX_DEVICE]
);
```

### JUB_ConnetDeviceSIM
* **Function:** Connect virtual device.
* **IN param:**
deviceID - device ID from JUB_ListDeviceSIM/JUB_ListLiteSIM.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ConnetDeviceSIM(
    IN JUB_UINT16 deviceID
);
```

### JUB_DisconnetDeviceSIM
* **Function:** Disconnect virtual device.
* **IN param:**
deviceID - device ID from JUB_ListDeviceSIM/JUB_ListLiteSIM.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_DisconnetDeviceSIM(
    IN JUB_UINT16 deviceID
);
```
