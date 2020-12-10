# HID module
In [JUB_SDK_DEV_HID.h](../include/JUB_SDK_DEV_HID.h).

Function list:
* [JUB_ListDeviceHid](#JUB_ListDeviceHid)
* [JUB_ConnetDeviceHid](#JUB_ConnetDeviceHid)
* [JUB_DisconnetDeviceHid](#JUB_DisconnetDeviceHid)

### JUB_ListDeviceHid
* **Function:** Enumerates HID device, only for JuBiter Blade/Bio in HID mode.
* **IN param:** none.
* **OUT param:**
deviceIDs[MAX_DEVICE] - deviceID list.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ListDeviceHid(
    OUT JUB_UINT16 deviceIDs[MAX_DEVICE]
);
```

### JUB_ConnetDeviceHid
* **Function:** Connect HID device.
* **IN param:**
deviceID - device ID from JUB_ListDeviceHid.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_ConnetDeviceHid(
    IN JUB_UINT16 deviceID
);
```

### JUB_DisconnetDeviceHid
* **Function:** Disconnect HID device.
* **IN param:**
deviceID - device ID from JUB_ListDeviceHid.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_DisconnetDeviceHid(
    IN JUB_UINT16 deviceID
);
```
