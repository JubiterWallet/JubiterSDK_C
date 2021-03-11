# Introduction
JuBiter SDK is an open source, cross-platform, mobile-focused library implementing low-level cryptographic wallet functionality for a certain number of blockchains. It is a core part of the JuBiter Wallet, and some other projects. Most of the code is C++ with a set of strict C interfaces, and idiomatic interfaces for supported languages: Swift for iOS and Java for Android.

# Supported Blockchains
JuBiter Core supports more than 20 blockchains: Bitcoin, Ethereum, and some major blockchain platforms.

| Index | Name | Symbol | URL |
| --- | --- | --- | --- |
| 0000 | Bitcoin | BTC | https://bitcoin.org |
| 0002 | Litecoin | LTC | https://litecoin.org |
| 0005 | Dash | DASH | https://dash.org |
| 0060 | Ethereum | ETH | https://ethereum.org |
| 0061 | Ethereum Classic | ETC |https://ethereumclassic.org |
| 0144 | Ripple | XRP | https://ripple.com/xrp |
| 0145 | Bitcoin Cash | BCH |https://bitcoincash.org |
| 0171 | Hcash | HC | https://h.cash/ |
| 0194 | EOS | EOS | http://eos.io |
| 0195 | Tron | TRX | https://tron.network |
| 0461 | Filecoin | FIL | https://filecoin.io |
| 2301 | Qtum | QTUM | https://qtum.org |
| ---- | USDT | USDT | https://tether.to |

# API
JuBiter SDK supports software & hardware implementation for JuBiter wallet. Hardware support mainly JuBiter series products, including JuBiter Blade, JuBiter Bio and JuBiter Lite.


|| JuBiter Blade | JuBiter Bio | JuBiter Lite | JuBiter Crypto |
| --- | --- | --- | --- | --- |
| HID | &#x2611; | &#x2611; | &#x2611; | &#x2612; |
| BT | &#x2611; | &#x2611; | &#x2612; | &#x2612; |
| NFC | &#x2612; | &#x2612; | &#x2611; | &#x2612; |
| Impl in software | &#x2612; | &#x2612; | &#x2612;|  &#x2611; |
| VM(simulator) | &#x2611; | &#x2611; | &#x2612;(In the plan) |  &#x2612; |


Accordingly, JuBiter SDK is divided into the following modules, common module, device operation related module, Blade related module, Bio related module, Lite related module, coin related module, and software wallet module, etc.

There are two IDs in JuBiter SDK, deviceID and contextID. The first one is used to operate and connect hardware devices, and the last one is used for device-related and coin-related operations in a coin context. So, deviceID is obtained through the device operation related interface, and contextID is obtained through the coin-related interface.

### API include:
- Device in HID mode related module (see [here](docs/README_DEV_HID.md))
- Device in BLE mode related module (see [here](docs/README_DEV_BLE.md))
- Device in NFC mode related module (see [here](docs/README_DEV_NFC.md))
- Virtual Device(simulator for applet) in PCSC mode related module (see [here](docs/README_DEV_SIM.md))
- Device operation related module (see [here](docs/README_DEV.md))
- Coin related module (see [here](docs/README_COIN.md))
- Software wallet module (see [here](docs/README_SW.md))

# Dependency
| Name | URL | Note |
| ---- | ---- | ---- |
| **Trust Wallet Core** | https://github.com/trustwallet/wallet-core.git | Source level integration, based on v2.2.10. The code added by JuBiter is identified as "JuBiter-defined". |
| **trezor-crypto** | https://github.com/trezor/trezor-crypto.git| Source level integration, based on 'master'. The code added by JuBiter is identified as "JuBiter-defined". |
| **JSON for Modern C++** | https://github.com/nlohmann/json.git | Source level integeration, based on v3.9.1. |
| **C++ Big Integer Library** | https://mattmccutchen.net/bigint | Source level integeration, based on 'bigint-2010.04.30.tar.bz2. |
| **uint256_t** | https://github.com/calccrypto/uint256_t.git | An unsigned 256 bit integer type for C++. |
|  |  |  |
| **Bluetooth communication library** for JuBiter Blade & Bio | --- | JuBiter developed. |
| **NFC communication library** for JuBiter Lite | --- | JuBiter developed. |

# Code branch management
- 'Master' is the development line, continuously integrated development of new coins and SDK features.
- 'v2.x.x' is the current version of Jubiter Wallet 2.0.

* * *
# Compiler Installation
### **编译工程文件已经迁移到 CMake**
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


* * *
# Demo reference
+ [nfcDemo-Android](https://github.com/JubiterWallet/nfcDemo-Android)
+ [JuBiterSDKDemo-iOS](https://github.com/JubiterWallet/JuBiterSDKDemo-iOS)
