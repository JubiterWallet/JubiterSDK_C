编译工程文件已经迁移到CMake
1.  安装CMake 3+
2.  Mac和Linux需要安装hidapi-dev
3.  windows需要使用cygwin,不支持visual studio,需要在cygwin环境下,安装hidapi-dev
4.  Android需要安装NDK20+

```bash
git submodule update --init --recursive
```

### **macOS,Linux,Cygwin**
```bash
mkdir build & cd build
cmake ..
make
```

### **Android**
可以直接将cmake直接引入Android Studio的项目里,也可以通过命令行进行操作
ANDROID_ABI可以是armeabi-v7a,arm64-v8a,x86,x86_64,mips,mips64
```bash
mkdir buildAndroid & cd buildAndroid

cmake -G "Unix Makefiles" -DANDROID_ABI=arm64-v8a -DCMAKE_BUILD_TYPE=Release -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=/path/to/sdk/ndk-bundle/build/cmake/android.toolchain.cmake ..

make
```

### **iOS**
- **.a**
```bash
sh ./builds/ios-build
```
- **framework**
To Do

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