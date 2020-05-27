# JuBiter-SDK-Demo-iOS
> Demo on iOS for JuBitSDK, supports for NFC and bluetooth devices.

> Depends on:
> - jsoncpp([1.8.4](https://github.com/open-source-parsers/jsoncpp.git))
> - JuBiterSDK([nfc branch](https://github.com/JubiterWallet/JubiterSDK_C.git))


## submodule
```bash
cd deps
git submodule add        https://github.com/open-source-parsers/jsoncpp.git jsoncpp
git submodule add -b nfc https://github.com/JubiterWallet/JubiterSDK_C.git JubiterSDK
```

```bash
cd deps/jsoncpp
git checkout 1.8.4
```


## init & update submodule
```bash
git submodule update --init --recursive
```


## JuBiterSDKDemo
>  However, supports for bluetooth devices is not complete yet.

Using 'JuBiterSDKDemo.xcworkspace' to build.
