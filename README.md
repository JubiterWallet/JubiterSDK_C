### master branch
> Our customers:
> - HyperPay
---
### API redefinition include:
+ Supports multiple types of HID & bluetooth & NFC devices, added functions as below, see 'JUB_SDK_DEV.h':
  + JUB_GetDeviceType()

+ Add "devName" param, see 'JUB_SDK_DEV_BLE.h':
  + JUB_connectDevice()
  + JUB_cancelConnect()

+ Add "version" param, see 'JUB_SDK_BTC.h':
  + JUB_SignTransactionBTC()
  + JUB_SignTransactionHC();
+ Rename Jub_EnumSupportCoins() as JUB_EnumSupportCoins();


### NFC module include:
+ Supports NFC(`#define NFC_MODE`), added functions as below, see 'JUB_SDK_DEV_NFC.h':
  + JUB_Reset()
  + JUB_GenerateSeed()
  + JUB_ImportMnemonic()
  + JUB_ExportMnemonic()
  + JUB_GetRootKeyStatus()
  + JUB_ChangePIN()
  + JUB_SetLabel()


### Bio module include:
+ Supports BIO(`#define BLE_MODE || #define HID_MODE`), added functions as below, see 'JUB_SDK_DEV_BIO.h':
  + Authentication for operating fingerprint:
    + JUB_IdentityVerify()
    + JUB_IdentityVerifyPIN()
    + JUB_IdentityShowNineGrids()
    + JUB_IdentityCancelNineGrids()
  + Operating fingerprint:
    + JUB_EnrollFingerprint()
    + JUB_EnumFingerprint()
    + JUB_EraseFingerprint()
    + JUB_DeleteFingerprint()
    + JUB_VerifyFingerprint()


### Tron module include:
+ Supports Tron, added functions as below, see 'JUB_SDK_TRX.h':
 + JUB_CreateContextTRX()
 + JUB_GetAddressTRX()
 + JUB_GetHDNodeTRX()
 + JUB_GetMainHDNodeTRX()
 + JUB_SetMyAddressTRX()
 + JUB_SignTransactionTRX()
 + JUB_PackContractTRX()

---
### Demo reference:
+ [nfcDemo-Android](https://github.com/JubiterWallet/nfcDemo-Android)
+ [JuBiterSDKDemo-iOS](https://github.com/JubiterWallet/JuBiterSDKDemo-iOS)
---
### **编译工程文件已经迁移到CMake**
1.  安装CMake 3+
2.  Mac和Linux需要安装hidapi-dev
3.  windows需要使用cygwin,不支持visual studio,需要在cygwin环境下,安装hidapi-dev
4.  Android需要安装NDK20+

```bash
git submodule update --init --recursive
```
---
### **MacOS**
+ 请使用3.18.4版本，brew的3.16与3.19不保证会成功，可能会出现无法找到framework的问题
+ 暂时先不考虑M1芯片
+ MacOS由于CMake的Bug，无法打出FatLib，需要分平台打包，具体见 https://gitlab.kitware.com/cmake/cmake/-/issues/21282
+ 暂时关闭了RPATH，如果需要参照 https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling 进行修改
+ 打包framework无法使用make体系，只能使用Xcode
```bash
mkdir buildMacOS & cd buildMacOS
cmake -G Xcode -DCMAKE_OSX_ARCHITECTURES=x86_64 ..
cmake --build . --config Release
```
---
### **Linux,Cygwin**
```bash
mkdir build & cd build
cmake ..
make
```
---
### **Android**
可以直接将cmake直接引入Android Studio的项目里,也可以通过命令行进行操作
ANDROID_ABI可以是armeabi-v7a,arm64-v8a,x86,x86_64,mips,mips64
```bash
mkdir buildAndroid & cd buildAndroid

cmake -G "Unix Makefiles" -DANDROID_ABI=arm64-v8a -DCMAKE_BUILD_TYPE=Release -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=/path/to/sdk/ndk-bundle/build/cmake/android.toolchain.cmake ..

make
```
---
### **iOS**

+ 由于通讯库，暂时无法打模拟器
+ 由于通讯库，暂时无法支持BITCODE
```bash
mkdir buildIOS & cd buildIOS
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../deps/ios-cmake/ios.toolchain.cmake -DENABLE_BITCODE=0 -DPLATFORM=OS64
cmake --build . --config Release
```

---
### **macOS Xcode Project file generation**
```bash
mkdir build & cd build
cmake -G Xcode ..
```

> ### Special Note for macOS
> Remove dot-underscore files. Merges ._* files with corresponding native files.
> When got error like :
```bash
xxx/JubiterSDK_C/src/._JUB_SDK_DEV.cpp:2:3932: error: 
      source file is not valid UTF-8
  ...<FF><FF>
     ^
xxx/JubiterSDK_C/src/._JUB_SDK_DEV.cpp:2:3933: error: 
      source file is not valid UTF-8
  ...<FF>
     ^
```
> Using command to solves this problem:
```bash
dot_clean <directory>
```
---
### **JUB_SDK for Production tools**
```bash
mkdir build & cd build
cmake -DdevOnly=1 ..
make
```
Only the following device operation related interfaces are exported:
> JUB_ConnetDeviceHid
> JUB_DisconnetDeviceHid
> JUB_ListDeviceHid
> JUB_EnumApplets
> JUB_GetAppletVersion
> JUB_GetDeviceCert
> JUB_GetDeviceInfo
> JUB_IsBootLoader
> JUB_IsInitialize
> JUB_SendOneApdu
> JUB_SetTimeOut
